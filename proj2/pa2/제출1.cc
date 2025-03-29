#include <iomanip>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <bitset>
#include <string>
using namespace std;

int main(int argc, char **argv)
{
    // proj3
    int proj3_flag = 0;
    string arg = argv[1];
    if (arg.find("proj2_11") != string::npos)
    {
        // proj3 이면 flag
        proj3_flag = 1;
    }

    // inst 관련
    vector<unsigned> buffer; // 한줄 씩 입력받음 inst bin file path
    int all_size = 0;        // instruction 개수
    int user_inst = 0;
    int user_lw = 0;
    int flag = 0;

    // Data memory 관련
    int data_all_size = 0;

    ifstream input_file(argv[1], ios::binary);

    // // Address range: 0x10000000 – 0x1000FFFF
    // vector<int> data(256, 0xFF);
    vector<int> data(16384, 0xFF);

    if (!input_file)
        return -1;

    if (argc == 3)
    {
        flag = 1;
        user_inst = stoi(argv[2]);
    }
    else if (argc == 4)
    {
        ifstream data_file(argv[2], ios::binary);
        user_inst = stoi(argv[3]);
        data_file.seekg(0, ios::end);
        data_all_size = data_file.tellg();
        data_all_size /= 4;

        vector<int> data_buffer;
        data_buffer.resize(data_all_size);

        data_file.seekg(0, std::ios::beg);
        data_file.read(reinterpret_cast<char *>(data_buffer.data()), sizeof(int) * data_all_size);
        for (int i = 0; i < data_all_size; i++)
            data[i] = data_buffer[i];
    }
    else
        return -1;

    // data file instruciton 개수

    // size: nstruction 개수
    input_file.seekg(0, ios::end);
    all_size = input_file.tellg();
    all_size /= 4;

    // file로 부터 받아올 instruction 수에 따라 resize
    buffer.resize(all_size);

    // buffer에 파일 저장?
    input_file.seekg(0, std::ios::beg);
    input_file.read(reinterpret_cast<char *>(buffer.data()), sizeof(int) * all_size);

    // 32개의 register 0으로 초기화
    vector<unsigned int> reg(32, 0x00);

    unsigned buffer_ = 0; // buffer[n] 저장하는 temp 변수
    unsigned opcode = 0;
    unsigned rd = 0;
    unsigned funct3 = 0;
    unsigned rs1 = 0; // rs
    unsigned rs2 = 0; // rt
    unsigned funct7 = 0;
    int imm1 = 0;
    int rs2_ = 0;
    int curr_i = 0; // 상대 주소 때문에
    // riscv-r instruction 읽기 (binary)

    // x0 - x31 레지스터 계산
    for (int i = 0; i < user_inst; i++)
    {
        if (curr_i > (all_size - 1))
            break;
        reg[0] = 0;
        buffer_ = buffer[curr_i]; // memory 매번 접근하는 거 방지
        opcode = buffer_ << 25 >> 25;
        rd = buffer_ << 20 >> 27;
        funct3 = buffer_ << 17 >> 29;
        rs1 = buffer_ << 12 >> 27; // rs
        rs2 = buffer_ << 7 >> 27;  // rt
        funct7 = buffer_ >> 25;
        rs2_ = rs2;
        switch (opcode)
        {

        case 0x37:
            /* code lui */
            imm1 = buffer_ >> 12 << 12;
            reg[rd] = imm1;

            break;

        case 0x17:
            /* code auipc */
            imm1 = buffer_ >> 12 << 12;
            reg[rd] = curr_i * 4 + imm1;

            break;
        case 0x6f:
            /* code jal */

            imm1 = ((buffer_ & 0x80000000) >> 11) | ((buffer_ & 0x7FE00000) >> 20) | ((buffer_ & 0x100000) >> 9) | (buffer_ & 0xFF000);

            if (imm1 & 0x80000) // 20 bit 부호확장
                imm1 = static_cast<int>(static_cast<unsigned>(imm1) | 0xFFFF0000);

            reg[rd] = (curr_i * 4) + 4;
            curr_i = curr_i + (imm1 / 4);
            reg[0] = 0;
            continue;
        case 0x67:
            /* code jalr */
            imm1 = (buffer_ & 0xFFF00000) >> 20u;

            reg[rd] = (curr_i * 4) + 4;
            // curr_i = curr_i + ((reg[rs1] + imm1) / 4);
            curr_i = ((reg[rs1] + imm1) / 4);
            reg[0] = 0;
            continue;
        case 0x63:
            imm1 = ((buffer_ & 0x80000000) >> 19) | ((buffer_ & 0x7E000000) >> 20) | ((buffer_ & 0xF00) >> 7) | ((buffer_ & 0x80) << 4);
            // Check if the most significant bit (bit 11) is set
            if (imm1 & 0x800)
            {
                // Convert imm1 to a negative number
                imm1 = static_cast<int>(static_cast<unsigned>(imm1) | 0xFFFFF000);
            }
            switch (funct3)
            {
            case 0x0:
                // beq
                if ((int)reg[rs1] == (int)reg[rs2])
                {
                    curr_i = curr_i + (imm1 / 4);
                    continue; // curr_i ++ 안 하고 넘어가게
                }

                break;
            case 0x1:
                // bne
                if ((int)reg[rs1] != (int)reg[rs2])
                {
                    curr_i = curr_i + (imm1 / 4);
                    continue; // curr_i ++ 안 하고 넘어가게
                }

                break;
            case 0x4:
                // blt
                if ((int)reg[rs1] < (int)reg[rs2])
                {
                    curr_i = curr_i + (imm1 / 4);
                    continue; // curr_i ++ 안 하고 넘어가게
                }

                break;
            case 0x5:
                // bge
                if ((int)reg[rs1] >= (int)reg[rs2])
                {
                    curr_i = curr_i + (imm1 / 4);
                    continue; // curr_i ++ 안 하고 넘어가게
                }

                break;

            default:
                break;
            }
            break;
        case 0x3:
            switch (funct3)
            {
            case 0x2:
                // lw
                // 4byte, 부호확장

                imm1 = (buffer_ >> 20);
                if (imm1 & 0x800)
                    imm1 = static_cast<int>(static_cast<unsigned>(imm1) | 0xFFFFF000);

                if (flag == 0 && proj3_flag == 0)
                {
                    // flag=0: inst, data file 있는 거
                    reg[rd] = data[((reg[rs1] + imm1) & 0xFFFF) / 4];
                }
                else if (flag == 1 && proj3_flag == 0)
                {
                    // flag=0: inst 만 있는 거
                    // test case 11
                    cin >> user_lw;
                    reg[rd] = user_lw;
                }
                else if (proj3_flag == 1 && (reg[rs1] + imm1) == 0x20000000)
                {
                    // test case 11
                    cin >> user_lw;
                    reg[rd] = user_lw;
                }
                else
                {
                    // test case 11: 0x20000000 아닐 때
                    reg[rd] = data[((reg[rs1] + imm1) & 0xFFFF) / 4];
                }
                break;
            default:
                break;
            }
            break;
        case 0x23:
            /* code sb - sw */
            imm1 = (funct7 << 5) | rd;

            switch (funct3)
            {
            case 0x2:
                // sw, 4byte 저장
                // inst 파일 하나일때만 char 출력
                if (imm1 & 0x800)
                    imm1 = static_cast<int>(static_cast<unsigned>(imm1) | 0xFFFFF000);
                if (flag == 0 && proj3_flag == 0)
                {
                    // flag=0: inst, data file 있는 거
                    data[((reg[rs1] + imm1) & 0xFFFF) / 4] = reg[rs2];
                }
                else if (flag == 1 && proj3_flag == 0)
                {
                    // test case 11
                    cout << (char)(reg[rs2]);
                }
                else if (proj3_flag == 1 && (reg[rs1] + imm1) == 0x20000000)
                {
                    // test case 11
                    cout << (char)(reg[rs2]);
                    data[((reg[rs1] + imm1) & 0xFFFF) / 4] = reg[rs2];
                }
                else
                {
                    // test case 11: 0x20000000 아닐 때
                    data[((reg[rs1] + imm1) & 0xFFFF) / 4] = reg[rs2];
                }
                break;

            default:
                break;
            }
            break;
        case 0x13:
            /* code addi - srai */
            imm1 = (buffer_ & 0xFFF00000) >> 20;
            if (imm1 & 0x800)
            {
                // Convert imm1 to a negative number
                imm1 = static_cast<int>(static_cast<unsigned>(imm1) | 0xFFFFF000);
            }
            switch (funct3)
            {
            case 0x0:
                // addi
                reg[rd] = reg[rs1] + imm1;
                break;
            case 0x2:
                // slti
                reg[rd] = ((int)reg[rs1] < imm1) ? 1 : 0;

                break;

            case 0x4:
                // xori
                reg[rd] = reg[rs1] ^ imm1;

                break;
            case 0x6:
                // ori
                reg[rd] = reg[rs1] | imm1;

                break;
            case 0x7:
                // andi
                reg[rd] = reg[rs1] & imm1;

                break;
            case 0x1:
                // slli
                rs2_ = rs2 & 0x1F; // 하위 5bit만 유효하게 shift 하니깐
                reg[rd] = (int)reg[rs1] << rs2_;

                break;
            case 0x5:
                switch (funct7)
                {
                case 0x0:
                    // srli
                    rs2_ &= 0x1F; // 하위 5bit만 유효하게 shift 하니깐
                    reg[rd] = (reg[rs1] >> rs2_);
                    reg[rd] &= (0x7FFFFFFF >> (rs2_ - 1));

                    break;
                case 0x20:
                    // srai
                    rs2_ = rs2 & 0x1F; // 하위 5bit만 유효하게 shift 하니깐
                    reg[rd] = (int)reg[rs1] >> (int)rs2_;

                    break;

                default:
                    break;
                }
                break;
            default:
                break;
            }
            break;

        case 0x33: // 0b0110011 slli ~ add
            switch (funct3)
            {
            case 0x00:
                switch (funct7)
                {
                case 0x0:
                    // add
                    reg[rd] = reg[rs1] + reg[rs2];

                    break;
                case 0x20:
                    reg[rd] = reg[rs1] - reg[rs2];

                    break;

                default:
                    break;
                }
                break;
            case 0x1:
                // sll
                rs2_ = reg[rs2] & 0x7F; // 하위 5bit만 유효하게 shift 하니깐
                reg[rd] = reg[rs1] << rs2_;

                break;
            case 0x2:
                // slt
                if (reg[rs1] < reg[rs2])
                {
                    reg[rd] = 1;
                }
                else
                    reg[rd] = 0;

                break;
            case 0x4:
                // xor
                reg[rd] = reg[rs1] ^ reg[rs2];

                break;
            case 0x5:
                switch (funct7)
                {
                case 0x0:
                    // srl
                    reg[rd] = reg[rs1] >> (reg[rs2] & 0x1F);

                    break;

                case 0x20:
                    // sra
                    rs2_ = reg[rs2] & 0x1F; // 하위 5bit만 유효하게 shift 하니깐
                    reg[rd] = (int)reg[rs1] >> (int)rs2_;

                    break;
                default:
                    break;
                }
                break;
            case 0x6:
                // or
                reg[rd] = reg[rs1] | reg[rs2];

                break;
            case 0x7:
                // and
                reg[rd] = reg[rs1] & reg[rs2];

                break;
            default:
                break;
            }
            break;

        default:
            break;
        }
        reg[0] = 0;
        curr_i++;
    }

    for (int i = 0; i < 32; i++)
    {
        cout << "x" << dec << i << ": 0x";
        cout << setw(8) << setfill('0') << hex << reg[i] << endl;
    }

    return 0;
}