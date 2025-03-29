#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int *buffer;
    int size;
    size_t count;
    int inst_c;
    int data[32];
    for (int i = 0; i < 32; i++)
    {
        data[i] = 0xFF;
    }
    if (argc == 3)
    {
        inst_c = atoi(argv[2]);
    }
    else if (argc == 4)
    {
        inst_c = atoi(argv[3]);
    }

    FILE *fp = fopen(argv[1], "rb");
    if (NULL == fp)
    {
        return -1;
    }

    fseek(fp, 0, SEEK_END);
    size = ftell(fp);

    int numInts = (size + 3) / 4;
    printf("numInts: %d\n", numInts);
    printf("size: %d\n", size);
    buffer = (int *)malloc(numInts * sizeof(int));

    fseek(fp, 0, SEEK_SET);
    count = fread(buffer, sizeof(int), numInts, fp);
    if (count != numInts)
    {
        fclose(fp);
        free(buffer);
        return -1;
    }

    int reg[32];
    for (int i = 0; i < 32; i++)
    {
        reg[i] = 0x00;
    }

    int current_i = 0;
    if (inst_c > numInts)
    {
        inst_c = numInts;
    }
    size /= 4;
    for (int i = 0; i < size; i++)
    {
        unsigned int opcode = buffer[current_i] & 0x7F;         // opcode 추출
        unsigned int rd = (buffer[current_i] >> 7) & 0x1F;      // rd 필드 추출
        unsigned int funct3 = (buffer[current_i] >> 12) & 0x7;  // funct3 필드 추출
        unsigned int rs1 = (buffer[current_i] >> 15) & 0x1F;    // rs1 필드 추출
        unsigned int rs2 = (buffer[current_i] >> 20) & 0x1F;    // rs2 필드 추출
        unsigned int funct7 = (buffer[current_i] >> 25) & 0x7F; // funct7 필드 추출
        int imm1 = ((buffer[current_i] & (int)0xFE000000) >> 20) + ((buffer[current_i] & 0xF80) >> 7);
        int imm2 = ((buffer[current_i] & (int)0x80000000) >> 11) + (buffer[current_i] & 0xFF000) + ((buffer[current_i] & 0x100000) >> 9) + ((buffer[current_i] & 0x7FE00000) >> 20);
        int imm3 = ((buffer[current_i] & (int)0x80000000) >> 19) + ((buffer[current_i] & 0x80) << 4) + ((buffer[current_i] & 0x7E000000) >> 20) + ((buffer[current_i] & 0xF00) >> 7);
        int b_n = imm3 / 4;
        reg[0] = 0;
        int imm4 = (int)buffer[current_i] >> 20;

        printf("inst %d: %08x ", current_i, buffer[current_i]);

        // opcode와 funct3, funct7에 따라 어셈블리 명령어 출력
        switch (opcode)
        {
        case 0x13:
            switch (funct3)
            {
            case 0x0:
                printf("addi x%d, x%d, %d\n", rd, rs1, (int)buffer[current_i] >> 20);
                reg[rd] = reg[rs1] + ((int)buffer[current_i] >> 20);
                break;
            case 0x1:
                printf("slli x%d, x%d, %d\n", rd, rs1, (int)buffer[current_i] >> 20);
                reg[rd] = reg[rs1] << ((int)buffer[current_i] >> 20);
                break;
            case 0x2:

                printf("slti x%d, x%d, %d\n", rd, rs1, (int)buffer[current_i] >> 20);
                if (reg[rs1] < ((int)buffer[current_i] >> 20))
                {
                    reg[rd] = 1;
                }
                else
                {
                    reg[rd] = 0;
                }
                break;
            case 0x3:
                printf("sltiu x%d, x%d, %d\n", rd, rs1, (int)buffer[current_i] >> 20);
                if (reg[rs1] < ((unsigned int)buffer[current_i] >> 20))
                {
                    reg[rd] = 1;
                }
                else
                {
                    reg[rd] = 0;
                }
                break;
            case 0x4:
                printf("xori x%d, x%d, %d\n", rd, rs1, (int)buffer[current_i] >> 20);
                reg[rd] = reg[rs1] ^ ((int)buffer[current_i] >> 20);
                break;
            case 0x5:
                if ((buffer[current_i] >> 30) & 0x1)
                {
                    printf("srai x%d, x%d, %d\n", rd, rs1, ((int)buffer[current_i] >> 20) & 0x1F);
                    reg[rd] = reg[rs1] >> ((int)buffer[current_i] >> 20);
                }
                else
                {
                    printf("srli x%d, x%d, %d\n", rd, rs1, ((int)buffer[current_i] >> 20) & 0x1F);
                    if (((int)buffer[current_i] >> 20) != 0)
                    {
                        reg[rd] = reg[rs1] >> ((unsigned int)buffer[current_i] >> 20);
                        reg[rd] = reg[rd] & (0x7FFFFFFF >> ((int)buffer[current_i] >> 20) - 1);
                    }
                    else
                    {
                        reg[rd] = reg[rs1];
                    }
                }
                break;
            case 0x6:
                printf("ori x%d, x%d, %d\n", rd, rs1, (int)buffer[current_i] >> 20);
                reg[rd] = reg[rs1] | ((int)buffer[current_i] >> 20);
                break;
            case 0x7:
                printf("andi x%d, x%d, %d\n", rd, rs1, (int)buffer[current_i] >> 20);
                reg[rd] = reg[rs1] & ((int)buffer[current_i] >> 20);
                break;
            default:
                printf("Unknown instruction\n");
                break;
            }
            break;
        case 0x17:

            printf("auipc x%d, %d\n", rd, (int)buffer[current_i] & 0xFFFFF000);

            break;

        case 0x03:
            switch (funct3)
            {
            case 0x0:
                printf("lb x%d, %d(x%d)\n", rd, (int)buffer[current_i] >> 20, rs1);

                break;
            case 0x1:
                printf("lh x%d, %d(x%d)\n", rd, (int)buffer[current_i] >> 20, rs1);
                break;
            case 0x2:
                printf("lw x%d, %d(x%d)\n", rd, (int)buffer[current_i] >> 20, rs1);
                break;
            case 0x4:
                printf("lbu x%d, %d(x%d)\n", rd, (int)buffer[current_i] >> 20, rs1);
                break;
            case 0x5:
                printf("lhu x%d, %d(x%d)\n", rd, (int)buffer[current_i] >> 20, rs1);
                break;
            default:
                printf("Unknown instruction\n");
                break;
            }
            break;
        case 0x23:
            switch (funct3)
            {
            case 0x0:
                printf("sb x%d, %d(x%d)\n", rs2, imm1, rs1);
                break;
            case 0x1:
                printf("sh x%d, %d(x%d)\n", rs2, imm1, rs1);
                break;
            case 0x2:
                printf("sw x%d, %d(x%d)\n", rs2, imm1, rs1);
                break;
            default:
                printf("Unknown instruction\n");
                break;
            }
            break;

        case 0x33:
            switch (funct3)
            {
            case 0x0:
                if (funct7 == 0x0)
                {
                    printf("add x%d, x%d, x%d\n", rd, rs1, rs2);
                    reg[rd] = reg[rs1] + reg[rs2];
                }
                else if (funct7 == 0x20)
                {
                    printf("sub x%d, x%d, x%d\n", rd, rs1, rs2);
                    reg[rd] = reg[rs1] - reg[rs2];
                }
                else
                {
                    printf("Unknown instruction\n");
                }
                break;
            case 0x1:
                printf("sll x%d, x%d, x%d\n", rd, rs1, rs2);
                reg[rd] = reg[rs1] << (reg[rs2] & 0x1f);
                break;
            case 0x2:
                printf("slt x%d, x%d, x%d\n", rd, rs1, rs2);
                if (reg[rs1] < reg[rs2])
                {
                    reg[rd] = 1;
                }
                else
                {
                    reg[rd] = 0;
                }
                break;
            case 0x3:
                printf("sltu x%d, x%d, x%d\n", rd, rs1, rs2);
                if (reg[rs1] < (unsigned int)reg[rs2])
                {
                    reg[rd] = 1;
                }
                else
                {
                    reg[rd] = 0;
                }
                break;
            case 0x4:
                printf("xor x%d, x%d, x%d\n", rd, rs1, rs2);
                reg[rd] = reg[rs1] ^ reg[rs2];
                break;
            case 0x5:
                if ((buffer[current_i] >> 30) & 0x1)
                {
                    printf("sra x%d, x%d, x%d\n", rd, rs1, rs2);
                    reg[rd] = reg[rs1] >> reg[rs2];
                }
                else
                {
                    printf("srl x%d, x%d, x%d\n", rd, rs1, rs2);
                    if (reg[rs2] != 0)
                    {
                        reg[rd] = reg[rs1] >> (unsigned int)reg[rs2];
                        reg[rd] = reg[rd] & (0x7FFFFFFF >> reg[rs2] - 1);
                    }
                    else
                    {
                        reg[rd] = reg[rs1];
                    }
                }
                break;
            case 0x6:
                printf("or x%d, x%d, x%d\n", rd, rs1, rs2);
                reg[rd] = reg[rs1] | reg[rs2];
                break;
            case 0x7:
                printf("and x%d, x%d, x%d\n", rd, rs1, rs2);
                reg[rd] = reg[rs1] & reg[rs2];
                break;
            default:
                printf("Unknown instruction\n");
                break;
            }
            break;
        case 0x37:
            printf("lui x%d, %d\n", rd, (int)buffer[current_i] & 0xFFFFF000);
            reg[rd] = (int)buffer[current_i] & 0xFFFFF000;
            break;

        case 0x63:
            switch (funct3)
            {
            case 0x0:
                printf("beq x%d, x%d, %d\n", rs1, rs2, imm3);
                if (reg[rs1] == reg[rs2])
                {
                    current_i = current_i + b_n;
                    inst_c = inst_c - b_n + 1;
                    continue;
                }
                break;
            case 0x1:
                printf("bne x%d, x%d, %d\n", rs1, rs2, imm3);
                if (reg[rs1] != reg[rs2])
                {
                    current_i = current_i + b_n;
                    inst_c = inst_c - b_n + 1;
                    continue;
                }
                break;
            case 0x4:
                printf("blt x%d, x%d, %d\n", rs1, rs2, imm3);
                if (reg[rs1] < reg[rs2])
                {
                    current_i = current_i + b_n;
                    inst_c = inst_c - b_n + 1;
                    continue;
                }
                break;
            case 0x5:
                printf("bge x%d, x%d, %d\n", rs1, rs2, imm3);
                if (reg[rs1] >= reg[rs2])
                {
                    current_i = current_i + b_n;
                    inst_c = inst_c - b_n + 1;
                    continue;
                }
                break;
            case 0x6:
                printf("bltu x%d, x%d, %d\n", rs1, rs2, imm3);
                if (reg[rs1] < reg[rs2])
                {
                    current_i = current_i + b_n;
                    inst_c = inst_c - b_n + 1;
                    continue;
                }
                break;
            case 0x7:
                printf("bgeu x%d, x%d, %d\n", rs1, rs2, imm3);
                if (reg[rs1] > reg[rs2])
                {
                    current_i = current_i + b_n;
                    inst_c = inst_c - b_n + 1;
                    continue;
                }
                break;
            default:
                printf("Unknown instruction\n");
                break;
            }
            break;
        case 0x67:
            printf("jalr x%d, %d(x%d)\n", rd, imm4, rs1);

            break;
        case 0x6F:
            printf("jal x%d, %d\n", rd, imm2);

            break;
        default:
            printf("Unknown instruction\n");
            break;
        }
        current_i++;
        reg[0] = 0;
    }

    fclose(fp);
    free(buffer);

    return 0;
}