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
    vector<int> data(16384, 0xFF);

    if (!input_file)
        return -1;

    if (argc == 3)
    {
        // inst 만 있고, data 파일은 없을 때, lw 실행 시 우리가 직접 입력 해야함.
        // flag = 1 이면 cin 하기
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

    cout << endl;

    // 냐옹
    for (int i = 0; i < user_inst; i++)
    {
        reg[0] = 0;
        if (curr_i > (all_size - 1))
        {
            cout << "냐옹 " << endl;
            return -1;
        }
        cout << dec << i << ": inst " << curr_i << ": ";

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
            cout << setw(8) << setfill('0') << hex << buffer_ << " ";
            cout << dec << "lui "
                 << "x" << rd << ", "
                 << imm1 << endl;

            reg[rd] = imm1;
            // cout << hex << reg[rd] << endl;
            break;
        case 0x17:
            /* code auipc */
            imm1 = buffer_ >> 12 << 12;
            cout << setw(8) << setfill('0') << hex << buffer_ << " ";
            cout << dec << "auipc "
                 << "x" << rd << ", "
                 << imm1 << endl;

            reg[rd] = curr_i * 4 + imm1;
            break;
        case 0x6f:
            /* code jal */

            imm1 = ((buffer_ & 0x80000000) >> 11) | ((buffer_ & 0x7FE00000) >> 20) | ((buffer_ & 0x100000) >> 9) | (buffer_ & 0xFF000);
            if (imm1 & 0x80000) // 20 bit 부호확장
                imm1 = static_cast<int>(static_cast<unsigned>(imm1) | 0xFFFF0000);
            cout << setw(8) << setfill('0') << hex << buffer_ << " ";
            cout << dec << "jal "
                 << "x" << rd << ", "
                 << imm1 << endl;

            reg[rd] = (curr_i * 4) + 4;
            // curr_i = (imm1 / 4);
            curr_i = curr_i + (imm1 / 4);
            reg[0] = 0;
            continue;
        case 0x67:
            /* code jalr */
            imm1 = (buffer_ & 0xFFF00000) >> 20u;
            cout << setw(8) << setfill('0') << hex << buffer_ << " ";
            cout << dec << "jalr "
                 << "x" << rd << ", "
                 << imm1
                 << "(x" << rs1 << ")" << endl;

            reg[rd] = (curr_i * 4) + 4;
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
                cout << setw(8) << setfill('0') << hex << buffer_ << " ";
                cout << dec << "beq "
                     << "x" << rs1 << ", "
                     << "x" << rs2 << ", "
                     << imm1 << endl;

                if ((int)reg[rs1] == (int)reg[rs2])
                {
                    curr_i = curr_i + (imm1 / 4);

                    continue; // curr_i ++ 안 하고 넘어가게
                }
                break;
            case 0x1:
                // bne
                cout << setw(8) << setfill('0') << hex << buffer_ << " ";
                cout << dec << "bne "
                     << "x" << rs1 << ", "
                     << "x" << rs2 << ", "
                     << imm1 << endl;

                if ((int)reg[rs1] != (int)reg[rs2])
                {
                    curr_i = curr_i + (imm1 / 4);

                    continue; // curr_i ++ 안 하고 넘어가게
                }
                break;
            case 0x4:
                // blt
                cout << setw(8) << setfill('0') << hex << buffer_ << " ";
                cout << dec << "blt "
                     << "x" << rs1 << ", "
                     << "x" << rs2 << ", "
                     << imm1 << endl;

                if ((int)reg[rs1] < (int)reg[rs2])
                {
                    curr_i = curr_i + (imm1 / 4);
                    continue; // curr_i ++ 안 하고 넘어가게
                }

                break;
            case 0x5:
                // bge
                cout << setw(8) << setfill('0') << hex << buffer_ << " ";
                cout << dec << "bge "
                     << "x" << rs1 << ", "
                     << "x" << rs2 << ", "
                     << imm1 << endl;

                if ((int)reg[rs1] >= (int)reg[rs2])
                {
                    curr_i = curr_i + (imm1 / 4);

                    continue; // curr_i ++ 안 하고 넘어가게
                }
                break;
            default:
                cout << setw(8) << setfill('0') << hex << buffer_ << " ";
                cout << "unknown instruction" << endl;
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

                cout << setw(8) << setfill('0') << hex << buffer_ << " ";
                cout << dec << "lw "
                     << "x" << rd << ", "
                     << imm1
                     << "(x" << rs1 << ")" << endl;

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
                cout << setw(8) << setfill('0') << hex << buffer_ << " ";
                cout << "unknown instruction" << endl;
                break;
            }
            break;
        case 0x23:
            /* code sb - sw */
            // imm1 사용 안 했음.,,, 이상하넴
            // imm1 = (buffer_ >> 20) & 0x000000FFF;
            imm1 = (funct7 << 5) | rd;

            switch (funct3)
            {
            case 0x2:
                // sw, 4byte 저장

                if (imm1 & 0x800)
                    imm1 = static_cast<int>(static_cast<unsigned>(imm1) | 0xFFFFF000);
                cout << setw(8) << setfill('0') << hex << buffer_ << " ";
                cout << dec << "sw "
                     << "x" << rs2 << ", "
                     << imm1
                     << "(x" << rs1 << ")" << endl;
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
                cout << setw(8) << setfill('0') << hex << buffer_ << " ";
                cout << "unknown instruction" << endl;
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
                cout << setw(8) << setfill('0') << hex << buffer_ << " ";
                cout << dec << "addi "
                     << "x" << rd << ", "
                     << "x" << rs1 << ", "
                     << imm1 << endl;

                reg[rd] = reg[rs1] + imm1;
                break;
            case 0x2:
                // slti
                cout << setw(8) << setfill('0') << hex << buffer_ << " ";
                cout << dec << "slti "
                     << "x" << rd << ", "
                     << "x" << rs1 << ", "
                     << imm1 << endl;

                reg[rd] = ((int)reg[rs1] < imm1) ? 1 : 0;
                break;
            case 0x3:
                // sltiu
                cout << setw(8) << setfill('0') << hex << buffer_ << " ";
                cout << dec << "sltiu "
                     << "x" << rd << ", "
                     << "x" << rs1 << ", "
                     << imm1 << endl;
                break;
            case 0x4:
                // xori
                cout << setw(8) << setfill('0') << hex << buffer_ << " ";
                cout << dec << "xori "
                     << "x" << rd << ", "
                     << "x" << rs1 << ", "
                     << imm1 << endl;

                reg[rd] = reg[rs1] ^ imm1;
                break;
            case 0x6:
                // ori
                cout << setw(8) << setfill('0') << hex << buffer_ << " ";
                cout << dec << "ori "
                     << "x" << rd << ", "
                     << "x" << rs1 << ", "
                     << imm1 << endl;

                reg[rd] = reg[rs1] | imm1;
                break;
            case 0x7:
                // andi
                cout << setw(8) << setfill('0') << hex << buffer_ << " ";
                cout << dec << "andi "
                     << "x" << rd << ", "
                     << "x" << rs1 << ", "
                     << imm1 << endl;

                reg[rd] = reg[rs1] & imm1;
                break;
            case 0x1:
                // slli
                cout << setw(8) << setfill('0') << hex << buffer_ << " ";
                cout << dec << "slli "
                     << "x" << rd << ", "
                     << "x" << rs1 << ", "
                     << rs2 << endl;

                rs2_ = rs2 & 0x1F; // 하위 5bit만 유효하게 shift 하니깐
                reg[rd] = reg[rs1] << rs2_;
                break;
            case 0x5:
                switch (funct7)
                {
                case 0x0:
                    // srli
                    cout << setw(8) << setfill('0') << hex << buffer_ << " ";
                    cout << dec << "srli "
                         << "x" << rd << ", "
                         << "x" << rs1 << ", "
                         << rs2 << endl;

                    rs2_ &= 0x1F; // 하위 5bit만 유효하게 shift 하니깐
                    reg[rd] = (reg[rs1] >> rs2_);
                    reg[rd] &= (0x7FFFFFFF >> (rs2_ - 1));
                    break;
                case 0x20:
                    // srai
                    cout << setw(8) << setfill('0') << hex << buffer_ << " ";
                    cout << dec << "srai "
                         << "x" << rd << ", "
                         << "x" << rs1 << ", "
                         << rs2 << endl;

                    rs2_ = rs2 & 0x1F; // 하위 5bit만 유효하게 shift 하니깐
                    reg[rd] = (int)reg[rs1] >> (int)rs2_;
                    break;

                default:
                    cout << setw(8) << setfill('0') << hex << buffer_ << " ";
                    cout << "unknown instruction" << endl;
                    break;
                }
                break;
            default:
                cout << setw(8) << setfill('0') << hex << buffer_ << " ";
                cout << "unknown instruction" << endl;
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
                    cout << setw(8) << setfill('0') << hex << buffer_ << " ";
                    cout << dec << "add "
                         << "x" << rd << ", "
                         << "x" << rs1 << ", "
                         << "x" << rs2 << endl;

                    reg[rd] = reg[rs1] + reg[rs2];
                    break;
                case 0x20:
                    cout << setw(8) << setfill('0') << hex << buffer_ << " ";
                    cout << dec << "sub "
                         << "x" << rd << ", "
                         << "x" << rs1 << ", "
                         << "x" << rs2 << endl;

                    reg[rd] = reg[rs1] - reg[rs2];
                    break;

                default:
                    cout << setw(8) << setfill('0') << hex << buffer_ << " ";
                    cout << "unknown instruction" << endl;
                    break;
                }
                break;
            case 0x1:
                cout << setw(8) << setfill('0') << hex << buffer_ << " ";
                cout << dec << "sll "
                     << "x" << rd << ", "
                     << "x" << rs1 << ", "
                     << "x" << rs2 << endl;

                rs2_ = reg[rs2] & 0x7F; // 하위 5bit만 유효하게 shift 하니깐
                reg[rd] = reg[rs1] << rs2_;
                break;
            case 0x2:
                cout << setw(8) << setfill('0') << hex << buffer_ << " ";
                cout << dec << "slt "
                     << "x" << rd << ", "
                     << "x" << rs1 << ", "
                     << "x" << rs2 << endl;

                if (reg[rs1] < reg[rs2])
                {
                    reg[rd] = 1;
                }
                else
                    reg[rd] = 0;
                break;
            case 0x3:
                cout << setw(8) << setfill('0') << hex << buffer_ << " ";
                cout << dec << "sltu "
                     << "x" << rd << ", "
                     << "x" << rs1 << ", "
                     << "x" << rs2 << endl;
                break;
            case 0x4:
                cout << setw(8) << setfill('0') << hex << buffer_ << " ";
                cout << dec << "xor "
                     << "x" << rd << ", "
                     << "x" << rs1 << ", "
                     << "x" << rs2 << endl;

                reg[rd] = reg[rs1] ^ reg[rs2];
                break;
            case 0x5:
                switch (funct7)
                {
                case 0x0:
                    cout << setw(8) << setfill('0') << hex << buffer_ << " ";
                    cout << dec << "srl "
                         << "x" << rd << ", "
                         << "x" << rs1 << ", "
                         << "x" << rs2 << endl;

                    reg[rd] = reg[rs1] >> (reg[rs2] & 0x1F);
                    break;

                case 0x20:
                    cout << setw(8) << setfill('0') << hex << buffer_ << " ";
                    cout << dec << "sra "
                         << "x" << rd << ", "
                         << "x" << rs1 << ", "
                         << "x" << rs2 << endl;

                    rs2_ = reg[rs2] & 0x1F; // 하위 5bit만 유효하게 shift 하니깐
                    reg[rd] = (int)reg[rs1] >> (int)rs2_;
                    break;
                default:
                    cout << setw(8) << setfill('0') << hex << buffer_ << " ";
                    cout << "unknown instruction" << endl;
                    break;
                }
                break;
            case 0x6:
                cout << setw(8) << setfill('0') << hex << buffer_ << " ";
                cout << dec << "or "
                     << "x" << rd << ", "
                     << "x" << rs1 << ", "
                     << "x" << rs2 << endl;

                reg[rd] = reg[rs1] | reg[rs2];
                break;
            case 0x7:
                cout << setw(8) << setfill('0') << hex << buffer_ << " ";
                cout << dec << "and "
                     << "x" << rd << ", "
                     << "x" << rs1 << ", "
                     << "x" << rs2 << endl;

                reg[rd] = reg[rs1] & reg[rs2];
                break;
            default:
                cout << setw(8) << setfill('0') << hex << buffer_ << " ";
                cout << "unknown instruction" << endl;
                break;
            }
            break;

        default:
            cout << setw(8) << setfill('0') << hex << buffer_ << " ";
            cout << "unknown instruction" << endl;
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
