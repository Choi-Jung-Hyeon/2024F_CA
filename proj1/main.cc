#include <iomanip>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <bitset>
using namespace std;

int main(int argc, char **argv)
{
    // argc: 프로그램에 전달된 인자의 개수
    // argv의 첫 번째 요소는 실행 가능한 프로그램 파일의 경로가 될 것이고, argv의 두 번째 요소는 이진 파일의 경로
    // argv[0]: ./riscv-sim
    // argv[1]: binary file path

    // lui, auipc, jal, jalr, beq, bne, blt, bge, bltu, bgeu,
    // lb, lh, lw, lbu, lhu, sb, sh, sw, addi, slti, sltiu,
    // xori, ori, andi, slli, srli, srai, add, sub, sll, slt,
    // sltu, xor, srl, sra, or, and
    // 37개
    // cannot be interpreted, print “unknown instruction”

    vector<unsigned> buffer; // 한줄 씩 입력받음 from binary file
    size_t inst_size = 0;    // instruction 개수

    ifstream input_file(argv[1], ios::binary);
    if (!input_file)
    {
        return -1;
    }

    input_file.seekg(0, ios::end);
    inst_size = input_file.tellg();
    inst_size /= 4;
    // cout << inst_size << endl;

    // file로 부터 받아올 instruction 수에 따라 resize
    buffer.resize(inst_size);

    // buffer에 파일 저장?
    input_file.seekg(0, std::ios::beg);
    input_file.read(reinterpret_cast<char *>(buffer.data()), sizeof(int) * inst_size);

    // register 0으로 초기화
    vector<unsigned int> reg(32, 0x00);

    unsigned buffer_ = 0; // buffer[n] 저장하는 temp 변수
    unsigned opcode = 0;
    unsigned rd = 0;
    unsigned funct3 = 0;
    unsigned rs1 = 0; // rs
    unsigned rs2 = 0; // rt
    unsigned funct7 = 0;
    int imm1 = 0;
    // riscv-r instruction 읽기 (binary)
    for (size_t i = 0; i < inst_size; i++)
    {
        cout << "inst " << i << ": ";
        buffer_ = buffer[i]; // memory 매번 접근하는 거 방지
        opcode = buffer_ << 25 >> 25;
        rd = buffer_ << 20 >> 27;
        funct3 = buffer_ << 17 >> 29;
        rs1 = buffer_ << 12 >> 27; // rs
        rs2 = buffer_ << 7 >> 27;  // rt
        funct7 = buffer_ >> 25;
        switch (opcode)
        {

        case 0x37:
            /* code lui */
            imm1 = buffer_ >> 12 << 12;
            cout << setw(8) << setfill('0') << hex << buffer_ << " ";
            cout << dec << "lui "
                 << "x" << rd << ", "
                 << imm1 << endl;
            break;
        case 0x17:
            /* code auipc */
            imm1 = buffer_ >> 12 << 12;
            cout << setw(8) << setfill('0') << hex << buffer_ << " ";
            cout << dec << "auipc "
                 << "x" << rd << ", "
                 << imm1 << endl;
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
            break;
        case 0x67:
            /* code jalr */
            imm1 = (buffer_ & 0xFFF00000) >> 20u;
            cout << setw(8) << setfill('0') << hex << buffer_ << " ";
            cout << dec << "jalr "
                 << "x" << rd << ", "
                 << imm1
                 << "(x" << rs1 << ")" << endl;
            break;
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
                break;
            case 0x1:
                // bne
                cout << setw(8) << setfill('0') << hex << buffer_ << " ";
                cout << dec << "bne "
                     << "x" << rs1 << ", "
                     << "x" << rs2 << ", "
                     << imm1 << endl;
                break;
            case 0x4:
                // blt
                cout << setw(8) << setfill('0') << hex << buffer_ << " ";
                cout << dec << "blt "
                     << "x" << rs1 << ", "
                     << "x" << rs2 << ", "
                     << imm1 << endl;
                break;
            case 0x5:
                // bge
                cout << setw(8) << setfill('0') << hex << buffer_ << " ";
                cout << dec << "bge "
                     << "x" << rs1 << ", "
                     << "x" << rs2 << ", "
                     << imm1 << endl;
                break;
            case 0x6:
                // bltu
                cout << setw(8) << setfill('0') << hex << buffer_ << " ";
                cout << dec << "bltu "
                     << "x" << rs1 << ", "
                     << "x" << rs2 << ", "
                     << imm1 << endl;
                break;
            case 0x7:
                // bgeu
                cout << setw(8) << setfill('0') << hex << buffer_ << " ";
                cout << dec << "bgeu "
                     << "x" << rs1 << ", "
                     << "x" << rs2 << ", "
                     << imm1 << endl;
                break;
            default:
                cout << setw(8) << setfill('0') << hex << buffer_ << " ";
                cout << "unknown instruction" << endl;
                break;
            }
            break;
        case 0x3:
            /* code lb - lhu */
            switch (funct3)
            {
            // static_cast<unsigned>(imm1)
            case 0x0:
                // lb
                // 1byte, 부호확장
                imm1 = (buffer_ >> 20) & 0xF;

                if (imm1 & 0x8)
                {
                    // Convert imm1 to a negative number
                    imm1 = static_cast<int>(static_cast<unsigned>(imm1) | 0xFFFFFFF0);
                }
                cout << setw(8) << setfill('0') << hex << buffer_ << " ";
                cout << dec << "lb "
                     << "x" << rd << ", "
                     << imm1
                     << "(x" << rs1 << ")" << endl;
                break;
            case 0x1:
                // lh
                // 2byte, 부호확장

                imm1 = (buffer_ >> 20) & 0xFF;

                if (imm1 & 0x80)
                {
                    // Convert imm1 to a negative number
                    imm1 = static_cast<int>(static_cast<unsigned>(imm1) | 0xFFFFFF00);
                }
                cout << setw(8) << setfill('0') << hex << buffer_ << " ";
                cout << dec << "lh "
                     << "x" << rd << ", "
                     << imm1
                     << "(x" << rs1 << ")" << endl;
                break;
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
                break;
            case 0x4:
                // lbu
                // 1byte, 부호확장x
                imm1 = (buffer_ >> 20) & 0xF;
                cout << setw(8) << setfill('0') << hex << buffer_ << " ";
                cout << dec << "lbu "
                     << "x" << rd << ", "
                     << imm1
                     << "(x" << rs1 << ")" << endl;
                break;
            case 0x5:
                // lhu
                // 2byte, 부호확장x
                imm1 = (buffer_ >> 20) & 0xFF;
                cout << setw(8) << setfill('0') << hex << buffer_ << " ";
                cout << dec << "lhu "
                     << "x" << rd << ", "
                     << imm1
                     << "(x" << rs1 << ")" << endl;
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
            case 0x0:
                // sb, 1byte 저장
                imm1 &= 0xF;
                if (imm1 & 0x8)
                    imm1 = static_cast<int>(static_cast<unsigned>(imm1) | 0xFFFFFFF0);
                cout << setw(8) << setfill('0') << hex << buffer_ << " ";
                cout << dec << "sb "
                     << "x" << rs2 << ", "
                     << imm1
                     << "(x" << rs1 << ")" << endl;
                break;
            case 0x1:
                // sh, 2byte 저장
                imm1 &= 0xFF;
                if (imm1 & 0x80)
                    imm1 = static_cast<int>(static_cast<unsigned>(imm1) | 0xFFFFFF00);
                cout << setw(8) << setfill('0') << hex << buffer_ << " ";
                cout << dec << "sh "
                     << "x" << rs2 << ", "
                     << imm1
                     << "(x" << rs1 << ")" << endl;
                break;
            case 0x2:
                // sw, 4byte 저장

                if (imm1 & 0x800)
                    imm1 = static_cast<int>(static_cast<unsigned>(imm1) | 0xFFFFF000);
                cout << setw(8) << setfill('0') << hex << buffer_ << " ";
                cout << dec << "sw "
                     << "x" << rs2 << ", "
                     << imm1
                     << "(x" << rs1 << ")" << endl;
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
                break;
            case 0x2:
                // slti
                cout << setw(8) << setfill('0') << hex << buffer_ << " ";
                cout << dec << "slti "
                     << "x" << rd << ", "
                     << "x" << rs1 << ", "
                     << imm1 << endl;
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
                break;
            case 0x6:
                // ori
                cout << setw(8) << setfill('0') << hex << buffer_ << " ";
                cout << dec << "ori "
                     << "x" << rd << ", "
                     << "x" << rs1 << ", "
                     << imm1 << endl;
                break;
            case 0x7:
                // andi
                cout << setw(8) << setfill('0') << hex << buffer_ << " ";
                cout << dec << "andi "
                     << "x" << rd << ", "
                     << "x" << rs1 << ", "
                     << imm1 << endl;
                break;
            case 0x1:
                // slli
                cout << setw(8) << setfill('0') << hex << buffer_ << " ";
                cout << dec << "slli "
                     << "x" << rd << ", "
                     << "x" << rs1 << ", "
                     << rs2 << endl;
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
                    break;
                case 0x20:
                    // srai
                    cout << setw(8) << setfill('0') << hex << buffer_ << " ";
                    cout << dec << "srai "
                         << "x" << rd << ", "
                         << "x" << rs1 << ", "
                         << rs2 << endl;
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
                    break;
                case 0x20:
                    cout << setw(8) << setfill('0') << hex << buffer_ << " ";
                    cout << dec << "sub "
                         << "x" << rd << ", "
                         << "x" << rs1 << ", "
                         << "x" << rs2 << endl;
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
                break;
            case 0x2:
                cout << setw(8) << setfill('0') << hex << buffer_ << " ";
                cout << dec << "slt "
                     << "x" << rd << ", "
                     << "x" << rs1 << ", "
                     << "x" << rs2 << endl;
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
                    break;

                case 0x20:
                    cout << setw(8) << setfill('0') << hex << buffer_ << " ";
                    cout << dec << "sra "
                         << "x" << rd << ", "
                         << "x" << rs1 << ", "
                         << "x" << rs2 << endl;
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
                break;
            case 0x7:
                cout << setw(8) << setfill('0') << hex << buffer_ << " ";
                cout << dec << "and "
                     << "x" << rd << ", "
                     << "x" << rs1 << ", "
                     << "x" << rs2 << endl;
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
    }

    return 0;
}
