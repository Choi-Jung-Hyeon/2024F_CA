#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define OPCODE_MASK   0x7F
#define RD_MASK       0xF80
#define FUNCT3_MASK   0x7000
#define RS1_MASK      0xF8000
#define RS2_MASK      0x1F00000
#define FUNCT7_MASK   0xFE000000

#define RD_SHIFT      7
#define FUNCT3_SHIFT  12
#define RS1_SHIFT     15
#define RS2_SHIFT     20
#define FUNCT7_SHIFT  25

#define MEMORY_SIZE 65536
#define SPECIAL_ADDRESS 0x10000000

unsigned int reg[32] = {0};  // 레지스터 파일 초기화
unsigned char memory[MEMORY_SIZE];  // 메모리 초기화
unsigned int PC = 0;

int signExtend(int value, int bits) {
    int shift = 32 - bits;
    return (value << shift) >> shift;
}

void execute_add(unsigned int rd, unsigned int rs1, unsigned int rs2) {
    reg[rd] = reg[rs1] + reg[rs2];
    reg[0] = 0;
}

void execute_sub(unsigned int rd, unsigned int rs1, unsigned int rs2) {
    reg[rd] = reg[rs1] - reg[rs2];
    reg[0] = 0;
}

void execute_sll(unsigned int rd, unsigned int rs1, unsigned int rs2) {
    reg[rd] = reg[rs1] << (reg[rs2] & 0x1F);
    reg[0] = 0;
}

void execute_slt(unsigned int rd, unsigned int rs1, unsigned int rs2) {
    reg[rd] = ((int)reg[rs1] < (int)reg[rs2]) ? 1 : 0;
    reg[0] = 0;
}

void execute_sltu(unsigned int rd, unsigned int rs1, unsigned int rs2) {
    reg[rd] = (reg[rs1] < reg[rs2]) ? 1 : 0;
    reg[0] = 0;
}

void execute_xor(unsigned int rd, unsigned int rs1, unsigned int rs2) {
    reg[rd] = reg[rs1] ^ reg[rs2];
    reg[0] = 0;
}

void execute_srl(unsigned int rd, unsigned int rs1, unsigned int rs2) {
    reg[rd] = reg[rs1] >> (reg[rs2] & 0x1F);
    reg[0] = 0;
}

void execute_sra(unsigned int rd, unsigned int rs1, unsigned int rs2) {
    reg[rd] = (int)reg[rs1] >> (reg[rs2] & 0x1F);
    reg[0] = 0;
}

void execute_or(unsigned int rd, unsigned int rs1, unsigned int rs2) {
    reg[rd] = reg[rs1] | reg[rs2];
    reg[0] = 0;
}

void execute_and(unsigned int rd, unsigned int rs1, unsigned int rs2) {
    reg[rd] = reg[rs1] & reg[rs2];
    reg[0] = 0;
}

void execute_addi(unsigned int rd, unsigned int rs1, int imm) {
    reg[rd] = reg[rs1] + imm;
    reg[0] = 0;
}

void execute_slti(unsigned int rd, unsigned int rs1, int imm) {
    reg[rd] = ((int)reg[rs1] < imm) ? 1 : 0;
    reg[0] = 0;
}

void execute_sltiu(unsigned int rd, unsigned int rs1, int imm) {
    reg[rd] = (reg[rs1] < (unsigned int)imm) ? 1 : 0;
    reg[0] = 0;
}

void execute_xori(unsigned int rd, unsigned int rs1, int imm) {
    reg[rd] = reg[rs1] ^ imm;
    reg[0] = 0;
}

void execute_ori(unsigned int rd, unsigned int rs1, int imm) {
    reg[rd] = reg[rs1] | imm;
    reg[0] = 0;
}

void execute_andi(unsigned int rd, unsigned int rs1, int imm) {
    reg[rd] = reg[rs1] & imm;
    reg[0] = 0;
}

void execute_slli(unsigned int rd, unsigned int rs1, int imm) {
    reg[rd] = reg[rs1] << imm;
    reg[0] = 0;
}

void execute_srli(unsigned int rd, unsigned int rs1, int imm) {
    reg[rd] = reg[rs1] >> imm;
    reg[0] = 0;
}

void execute_srai(unsigned int rd, unsigned int rs1, int imm) {
    reg[rd] = (int)reg[rs1] >> imm;
    reg[0] = 0;
}

void execute_lui(unsigned int rd, int imm) {
    reg[rd] = imm;
    reg[0] = 0;
}

void execute_auipc(unsigned int rd, int imm) {
    reg[rd] = PC + imm;
    reg[0] = 0;
}

void execute_jal(unsigned int rd, int imm) {
    reg[rd] = PC + 4;
    PC = PC + imm;
    reg[0] = 0;
}

void execute_jalr(unsigned int rd, unsigned int rs1, int imm) {
    reg[rd] = PC + 4;
    PC = (reg[rs1] + imm) & ~1;
    reg[0] = 0;
}

void execute_beq(unsigned int rs1, unsigned int rs2, int imm) {
    if (reg[rs1] == reg[rs2]) {
        PC = PC + imm;
    }
}

void execute_bne(unsigned int rs1, unsigned int rs2, int imm) {
    if (reg[rs1] != reg[rs2]) {
        PC = PC + imm;
    }
}

void execute_blt(unsigned int rs1, unsigned int rs2, int imm) {
    if ((int)reg[rs1] < (int)reg[rs2]) {
        PC = PC + imm;
    }
}

void execute_bge(unsigned int rs1, unsigned int rs2, int imm) {
    if ((int)reg[rs1] >= (int)reg[rs2]) {
        PC = PC + imm;
    }
}

void execute_bltu(unsigned int rs1, unsigned int rs2, int imm) {
    if (reg[rs1] < reg[rs2]) {
        PC = PC + imm;
    }
}

void execute_bgeu(unsigned int rs1, unsigned int rs2, int imm) {
    if (reg[rs1] >= reg[rs2]) {
        PC = PC + imm;
    }
}

void execute_lb(unsigned int rd, unsigned int rs1, int imm) {
    int address = reg[rs1] + imm - 0x10000000;
    if (address < 0 || address >= MEMORY_SIZE) {
        printf("lb: Memory address out of range\n");
        return;
    }
    reg[rd] = signExtend(memory[address], 8);
    reg[0] = 0;
}

void execute_lh(unsigned int rd, unsigned int rs1, int imm) {
    int address = reg[rs1] + imm - 0x10000000;
    if (address < 0 || address >= MEMORY_SIZE - 1) {
        printf("lh: Memory address out of range\n");
        return;
    }
    reg[rd] = signExtend((memory[address + 1] << 8) | memory[address], 16);
    reg[0] = 0;
}

void execute_lw(unsigned int rd, unsigned int rs1, int imm) {
    int address = reg[rs1] + imm - 0x10000000;
    if (address == SPECIAL_ADDRESS) {
        int user_input;
        scanf("%d", &user_input);
        reg[rd] = user_input;
    } else if (address < 0 || address >= 0x0000FFFF) {
        printf("lw: Memory address out of range\n");
        return;
    } else {
        reg[rd] = (memory[address + 3] << 24) | (memory[address + 2] << 16) | (memory[address + 1] << 8) | memory[address];
    }
    reg[0] = 0;
}

void execute_lbu(unsigned int rd, unsigned int rs1, int imm) {
    int address = reg[rs1] + imm - 0x10000000;
    if (address < 0 || address >= MEMORY_SIZE) {
        printf("lbu: Memory address out of range\n");
        return;
    }
    reg[rd] = memory[address];
    reg[0] = 0;
}

void execute_lhu(unsigned int rd, unsigned int rs1, int imm) {
    int address = reg[rs1] + imm - 0x10000000;
    if (address < 0 || address >= MEMORY_SIZE - 1) {
        printf("lhu: Memory address out of range\n");
        return;
    }
    reg[rd] = (memory[address + 1] << 8) | memory[address];
    reg[0] = 0;
}

void execute_sb(unsigned int rs2, unsigned int rs1, int imm) {
    int address = reg[rs1] + imm - 0x10000000;
    if (address == SPECIAL_ADDRESS) {
        printf("%c", reg[rs2] & 0xFF);
    } else if (address < 0 || address >= MEMORY_SIZE) {
        printf("sb: Memory address out of range\n");
        return;
    } else {
        memory[address] = reg[rs2] & 0xFF;
    }
}

void execute_sh(unsigned int rs2, unsigned int rs1, int imm) {
    int address = reg[rs1] + imm - 0x10000000;
    if (address < 0 || address >= MEMORY_SIZE - 1) {
        printf("sh: Memory address out of range\n");
        return;
    }
    memory[address] = reg[rs2] & 0xFF;
    memory[address + 1] = (reg[rs2] >> 8) & 0xFF;
}

void execute_sw(unsigned int rs2, unsigned int rs1, int imm) {
    int address = reg[rs1] + imm - 0x10000000;

    if (address >= 0 && address < 0x0000FFFF) {
        memory[address] = reg[rs2] & 0xFF;
        memory[address + 1] = (reg[rs2] >> 8) & 0xFF;
        memory[address + 2] = (reg[rs2] >> 16) & 0xFF;
        memory[address + 3] = (reg[rs2] >> 24) & 0xFF;
    }
    else if (address == 0x10000000) {
        if (reg[rs2] < 0x7F) {
            printf("%c", reg[rs2]);
        }
    }
    else {
        printf("sw: Memory address out of range\n");
        
        return;
    }
}

void print_registers() {
    for (int i = 0; i < 32; ++i) {
        printf("x%d: 0x%08x\n", i, reg[i]);
    }
}

int main(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <instruction binary file> <number of instructions> [<data binary file>]\n", argv[0]);
        return EXIT_FAILURE;
    }

    int num_instructions = atoi(argv[2]);

    FILE *inst_fp = fopen(argv[1], "rb");
    if (inst_fp == NULL) {
        perror("Error opening instruction file");
        return EXIT_FAILURE;
    }

    fseek(inst_fp, 0, SEEK_END);
    long inst_fileSize = ftell(inst_fp);
    rewind(inst_fp);

    int *buffer = (int *)malloc(inst_fileSize);
    if (buffer == NULL) {
        fclose(inst_fp);
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    size_t inst_result = fread(buffer, sizeof(int), inst_fileSize / sizeof(int), inst_fp);
    if (inst_result != inst_fileSize / sizeof(int)) {
        fclose(inst_fp);
        free(buffer);
        fprintf(stderr, "Error reading instruction file\n");
        return EXIT_FAILURE;
    }

    fclose(inst_fp);

    if (argc == 4) {
        num_instructions = atoi(argv[3]);
        FILE *data_fp = fopen(argv[2], "rb");
        if (data_fp == NULL) {
            fprintf(stderr, "Error opening data file %s: %s\n", argv[2], strerror(errno));
            return EXIT_FAILURE;
        }

        fseek(data_fp, 0, SEEK_END);
        long data_fileSize = ftell(data_fp);
        rewind(data_fp);

        size_t data_result = fread(memory, sizeof(char), data_fileSize, data_fp);
        if (data_result != data_fileSize) {
            fclose(data_fp);
            fprintf(stderr, "Error reading data file %s\n", argv[2]);
            return EXIT_FAILURE;
        }

        fclose(data_fp);
    }

    for (int i = 0; i < num_instructions; ++i) {
        if (PC / 4 >= inst_result) break;

        unsigned int instruction = buffer[PC / 4];

        unsigned int opcode = instruction & OPCODE_MASK;
        unsigned int rd     = (instruction & RD_MASK) >> RD_SHIFT;
        unsigned int funct3 = (instruction & FUNCT3_MASK) >> FUNCT3_SHIFT;
        unsigned int rs1    = (instruction & RS1_MASK) >> RS1_SHIFT;
        unsigned int rs2    = (instruction & RS2_MASK) >> RS2_SHIFT;
        unsigned int funct7 = (instruction & FUNCT7_MASK) >> FUNCT7_SHIFT;

        int immI = signExtend(instruction >> 20, 12);
        int immS = signExtend(((instruction & 0xFE000000) >> 20) | ((instruction >> 7) & 0x1F), 12);
        int immB = signExtend(((instruction & 0x80000000) >> 19) | ((instruction & 0x7E000000) >> 20) | ((instruction & 0x00000F00) >> 7) | ((instruction & 0x00000080) << 4), 13);
        int immU = instruction & 0xFFFFF000;
        int immJ = signExtend(((instruction & 0x80000000) >> 11) | ((instruction & 0x7FE00000) >> 20) | ((instruction & 0x00100000) >> 9) | (instruction & 0x000FF000), 21);

        switch (opcode) {
            case 0x33:  // R-type instructions
                switch (funct3) {
                    case 0x0:
                        if (funct7 == 0x00) {
                            execute_add(rd, rs1, rs2);
                        } else if (funct7 == 0x20) {
                            execute_sub(rd, rs1, rs2);
                        }
                        break;
                    case 0x1:
                        execute_sll(rd, rs1, rs2);
                        break;
                    case 0x2:
                        execute_slt(rd, rs1, rs2);
                        break;
                    case 0x3:
                        execute_sltu(rd, rs1, rs2);
                        break;
                    case 0x4:
                        execute_xor(rd, rs1, rs2);
                        break;
                    case 0x5:
                        if (funct7 == 0x00) {
                            execute_srl(rd, rs1, rs2);
                        } else if (funct7 == 0x20) {
                            execute_sra(rd, rs1, rs2);
                        }
                        break;
                    case 0x6:
                        execute_or(rd, rs1, rs2);
                        break;
                    case 0x7:
                        execute_and(rd, rs1, rs2);
                        break;
                    default:
                        printf("unknown instruction\n");
                }
                break;
            case 0x13:  // I-type instructions
                switch (funct3) {
                    case 0x0:
                        execute_addi(rd, rs1, immI);
                        break;
                    case 0x2:
                        execute_slti(rd, rs1, immI);
                        break;
                    case 0x3:
                        execute_sltiu(rd, rs1, immI);
                        break;
                    case 0x4:
                        execute_xori(rd, rs1, immI);
                        break;
                    case 0x6:
                        execute_ori(rd, rs1, immI);
                        break;
                    case 0x7:
                        execute_andi(rd, rs1, immI);
                        break;
                    case 0x1:
                        execute_slli(rd, rs1, immI & 0x1F);
                        break;
                    case 0x5:
                        if (funct7 == 0x00) {
                            execute_srli(rd, rs1, immI & 0x1F);
                        } else if (funct7 == 0x20) {
                            execute_srai(rd, rs1, immI & 0x1F);
                        }
                        break;
                    default:
                        printf("unknown instruction\n");
                }
                break;
            case 0x3:  // Load instructions
                switch (funct3) {
                    case 0x0:
                        execute_lb(rd, rs1, immI);
                        break;
                    case 0x1:
                        execute_lh(rd, rs1, immI);
                        break;
                    case 0x2:
                        execute_lw(rd, rs1, immI);
                        break;
                    case 0x4:
                        execute_lbu(rd, rs1, immI);
                        break;
                    case 0x5:
                        execute_lhu(rd, rs1, immI);
                        break;
                    default:
                        printf("unknown instruction\n");
                }
                break;
            case 0x23:  // Store instructions
                switch (funct3) {
                    case 0x0:
                        execute_sb(rs2, rs1, immS);
                        break;
                    case 0x1:
                        execute_sh(rs2, rs1, immS);
                        break;
                    case 0x2:
                        execute_sw(rs2, rs1, immS);
                        break;
                    default:
                        printf("unknown instruction\n");
                }
                break;
            case 0x63:  // B-type instructions
                switch (funct3) {
                    case 0x0:
                        execute_beq(rs1, rs2, immB);
                        break;
                    case 0x1:
                        execute_bne(rs1, rs2, immB);
                        break;
                    case 0x4:
                        execute_blt(rs1, rs2, immB);
                        break;
                    case 0x5:
                        execute_bge(rs1, rs2, immB);
                        break;
                    case 0x6:
                        execute_bltu(rs1, rs2, immB);
                        break;
                    case 0x7:
                        execute_bgeu(rs1, rs2, immB);
                        break;
                    default:
                        printf("unknown instruction\n");
                }
                break;
            case 0x37:  // LUI
                execute_lui(rd, immU);
                break;
            case 0x17:  // AUIPC
                execute_auipc(rd, immU);
                break;
            case 0x6F:  // JAL
                execute_jal(rd, immJ);
                break;
            case 0x67:  // JALR
                execute_jalr(rd, rs1, immI);
                break;
            default:
                printf("unknown instruction\n");
        }

        if (opcode != 0x6F && opcode != 0x67 && (opcode != 0x63 || ((funct3 == 0x0 && reg[rs1] != reg[rs2]) || (funct3 == 0x1 && reg[rs1] == reg[rs2]) || (funct3 == 0x4 && reg[rs1] >= reg[rs2]) || (funct3 == 0x5 && reg[rs1] < reg[rs2]) || (funct3 == 0x6 && reg[rs1] >= reg[rs2]) || (funct3 == 0x7 && reg[rs1] < reg[rs2])))) {
            PC += 4;
        }
    }

    print_registers();

    free(buffer);

    return EXIT_SUCCESS;
}
