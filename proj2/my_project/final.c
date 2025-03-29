#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned int pc = 0;
unsigned int reg[32] = {0};
unsigned char mem[65536];

int sign(int val, int bit){
    return (val << (32 - bit)) >> (32 - bit);
}

int main(int argc, char** argv){
    if(argc < 3){
        return -1;
    }
    int inst = atoi(argv[2]);

    FILE* fp = fopen(argv[1], "rb");
    if(fp == NULL) {
        return -1;
    }

    fseek(fp, 0, SEEK_END);
    long total_inst = ftell(fp);
    rewind(fp);
    int* buf = (int*)malloc(total_inst);
    if(buf == NULL){
        fclose(fp);
        return -1;
    }

    size_t cnt = fread(buf, sizeof(int), total_inst/sizeof(int), fp);
    if(cnt != total_inst/sizeof(int)){
        fclose(fp);
        free(buf);
        return -1;
    }
    fclose(fp);

    if(argc == 4){
        inst = atoi(argv[3]);
        FILE* dp = fopen(argv[2], "rb");
        if(dp == NULL){
            return -1;
        }

        fseek(dp, 0, SEEK_END);
        long total_size = ftell(dp);
        rewind(dp);

        size_t d_cnt = fread(mem, sizeof(char), total_size/sizeof(char), dp);
        if(d_cnt != total_size/sizeof(char)){
            fclose(dp);
            return -1;
        }
        fclose(dp);
    }
 
    for(int i = 0; i < inst; ++i){
        if(pc / 4 >= cnt){
            break;
        }
        
        unsigned int INST = buf[pc / 4];
        unsigned int opcode = INST & 0x7F;
        unsigned int rd = (INST & 0xF80) >> 7;
        unsigned int func3 = (INST & 0x7000) >> 12;
        unsigned int rs1 = (INST & 0xF8000) >> 15;
        unsigned int rs2 = (INST & 0x1F00000) >> 20;
        unsigned int func7 = (INST & 0xFE000000) >> 25;

        int imm_i = sign(INST >> 20, 12);
        int imm_s = sign(((INST & 0xFE000000) >> 20) | ((INST >> 7) & 0x1F), 12);
        int imm_b = sign(((INST & 0x80000000) >> 19) | ((INST & 0x7E000000) >> 20) | ((INST & 0x00000F00) >> 7) | ((INST & 0x00000080) << 4), 13);
        int imm_j = sign(((INST & 0x80000000) >> 11) | ((INST & 0x7FE00000) >> 20) | ((INST & 0x00100000) >> 9) | (INST & 0x000FF000), 21);
        int imm_u = INST & 0xFFFFF000;
        //printf("inst %d: %08x ", id, buf[id]);

        switch(opcode){
            case 0x03:
                switch(func3){
                    /*
                    case 0x0:
                        printf("lb x%d, %d(x%d)\n", rd, imm, rs1);
                        break;
                    case 0x1:
                        printf("lh x%d, %d(x%d)\n", rd, imm, rs1);
                        break;
                    */
                    case 0x2:{
                        //printf("lw x%d, %d(x%d)\n", rd, imm_i, rs1);
                        int addr_lw = reg[rs1] + imm_i - 0x10000000;
                        if(addr_lw == 0x10000000){
                            int input;
                            scanf("%d", &input);
                            reg[rd] = input;
                        }
                        else if(0x0000FFFF <= addr_lw || addr_lw < 0){
                            return -1;
                        }
                        else{
                            reg[rd] = (mem[addr_lw + 3] << 24) | (mem[addr_lw + 2] << 16) | (mem[addr_lw + 1] << 8) | mem[addr_lw];
                        }
                        reg[0] = 0;
                        break;
                    }
                    /*
                    case 0x4:
                        printf("lbu x%d, %d(x%d)\n", rd, imm, rs1);
                        break;
                    case 0x5:
                        printf("lhu x%d, %d(x%d)\n", rd, imm, rs1);
                        break;
                    */
                    default:
                        //printf("unknown instruction\n");
                        break;
                }
                break;

            case 0x13:
                switch(func3) {
                    case 0x0:
                        //printf("addi x%d, x%d, %d\n", rd, rs1, imm_i);
                        reg[rd] = reg[rs1] + imm_i;
                        reg[0] = 0;
                        break;
                    case 0x1:
                        //printf("slli x%d, x%d, %d\n", rd, rs1, imm_i);
                        reg[rd] = reg[rs1] << (imm_i & 0x1F);
                        reg[0] = 0;
                        break;
                    case 0x2:
                        //printf("slti x%d, x%d, %d\n", rd, rs1, imm_i);
                        if((int)reg[rs1] < imm_i){
                            reg[rd] = 1;
                        }
                        else{
                            reg[rd] = 0;
                        }
                        reg[0] = 0;
                        break;
                    case 0x3:
                        //printf("sltiu x%d, x%d, %d\n", rd, rs1, imm_i);
                        if(reg[rs1] < ((unsigned int)imm_i)){
                            reg[rd] = 1;
                        }
                        else{
                            reg[rd] = 0;
                        }
                        reg[0] = 0;
                        break;
                    case 0x4:
                        //printf("xori x%d, x%d, %d\n", rd, rs1, imm_i);
                        reg[rd] = reg[rs1] ^ imm_i;
                        reg[0] = 0;
                        break;
                    case 0x5:
                        /*
                        if(MSB & 0x1) {
                            //printf("srai x%d, x%d, %d\n", rd, rs1, imm & 0x1F);
                            Reg[rd] = Reg[rs1] ^ imm;
                        }
                        else {
                            //printf("srli x%d, x%d, %d\n", rd, rs1, imm & 0x1F);
                            if((int)imm != 0){
                                Reg[rd] = Reg[rs1] >> (unsigned int)imm;
                                Reg[rd] = Reg[rd] & ((0x7FFFFFFF >> imm) - 1);
                            }
                            else{
                                Reg[rd] = Reg[rs1];
                            }
                        }
                        */
                        switch(func7){
                            case 0x00:
                                //printf("srli x%d, x%d, %d\n", rd, rs1, imm_i & 0x1F);
                                reg[rd] = reg[rs1] >> (imm_i & 0x1F);
                                reg[0] = 0;
                                break;
                            case 0x20:
                                //printf("srai x%d, x%d, %d\n", rd, rs1, imm_i & 0x1F);
                                reg[rd] = (int)reg[rs1] >> (imm_i & 0x1F);
                                reg[0] = 0;
                                break;
                            default:
                                break;
                        }
                        break;
                    case 0x6:
                        //printf("ori x%d, x%d, %d\n", rd, rs1, imm_i);
                        reg[rd] = reg[rs1] | imm_i;
                        reg[0] = 0;
                        break;
                    case 0x7:
                        //printf("andi x%d, x%d, %d\n", rd, rs1, imm_i);
                        reg[rd] = reg[rs1] & imm_i;
                        reg[0] = 0;
                        break;
                    default:
                        //printf("unknown instruction\n");
                        break;
                }
                break;   

            case 0x17:
                //printf("auipc x%d, %d\n", rd, imm_u);
                reg[rd] = pc + imm_u;
                reg[0] = 0;
                break;

            case 0x23:
                switch(func3) {
                    /*
                    case 0x0:
                        printf("sb x%d, %d(x%d)\n", rs2, imm_s, rs1);
                        break;
                    case 0x1:
                        printf("sh x%d, %d(x%d)\n", rs2, imm_s, rs1);
                        break;
                    */
                    case 0x2:{
                        //printf("sw x%d, %d(x%d)\n", rs2, imm_s, rs1);
                        int addr_sw = reg[rs1] + imm_s - 0x10000000;

                        if(0 <= addr_sw && addr_sw < 0x0000FFFF){
                            mem[addr_sw] = reg[rs2] & 0xFF;
                            mem[addr_sw + 1] = (reg[rs2] >> 8) & 0xFF;
                            mem[addr_sw + 2] = (reg[rs2] >> 16) & 0xFF;
                            mem[addr_sw + 3] = (reg[rs2] >> 24) & 0xFF;
                        }
                        else if(addr_sw == 0x10000000){
                            if(reg[rs2] < 0x7F){
                                printf("%c", reg[rs2]);
                            }
                        }
                        else{
                            return -1;
                        }
                        break;
                    }
                    default:
                        //printf("unknown instruction\n");
                        break;
                }
                break;

            case 0x33:
                switch(func3) {
                    case 0x0:
                        switch(func7) {
                            case 0x00:
                                //printf("add x%d, x%d, x%d\n", rd, rs1, rs2);
                                reg[rd] = reg[rs1] + reg[rs2];
                                reg[0] = 0;
                                break;
                            case 0x20:
                                //printf("sub x%d, x%d, x%d\n", rd, rs1, rs2);
                                reg[rd] = reg[rs1] - reg[rs2];
                                reg[0] = 0;
                                break;
                            default:
                                //printf("unknown instruction\n");
                                break;
                        }
                        break;
                    case 0x1:
                        //printf("sll x%d, x%d, x%d\n", rd, rs1, rs2);
                        reg[rd] = reg[rs1] << (reg[rs2] & 0x1F);
                        reg[0] = 0;
                        break;
                    case 0x2:
                        //printf("slt x%d, x%d, x%d\n", rd, rs1, rs2);
                        if((int)reg[rs1] < (int)reg[rs2]){
                            reg[rd] = 1;
                        }
                        else{
                            reg[rd] = 0;
                        }
                        reg[0] = 0;
                        break;
                    case 0x3:
                        //printf("sltu x%d, x%d, x%d\n", rd, rs1, rs2);
                        if(reg[rs1] < reg[rs2]){
                            reg[rd] = 1;
                        }
                        else{
                            reg[rd] = 0;
                        }
                        reg[0] = 0;
                        break;
                    case 0x4:
                        //printf("xor x%d, x%d, x%d\n", rd, rs1, rs2);
                        reg[rd] = reg[rs1] ^ reg[rs2];
                        reg[0] = 0;
                        break;
                    case 0x5:
                        /*
                        if(MSB & 0x1) {
                            //printf("sra x%d, x%d, x%d\n", rd, rs1, rs2);
                            Reg[rd] = Reg[rs1] >> Reg[rs2];
                        }
                        else {
                            //printf("srl x%d, x%d, x%d\n", rd, rs1, rs2);
                            if(Reg[rs2] != 0){
                                Reg[rd] = Reg[rs1] >> (unsigned int)Reg[rs2];
                                Reg[rd] = Reg[rd] & ((0x7FFFFFFF >> Reg[rs2]) - 1);
                            }
                            else{
                                Reg[rd] = Reg[rs1];
                            }
                        }
                        */
                        switch(func7){
                            case 0x00:
                                //printf("srl x%d, x%d, x%d\n", rd, rs1, rs2);
                                reg[rd] = reg[rs1] >> (reg[rs2] & 0x1F);
                                reg[0] = 0;
                                break;
                            case 0x20:
                                //printf("sra x%d, x%d, x%d\n", rd, rs1, rs2);
                                reg[rd] = (int)reg[rs1] >> (reg[rs2] & 0x1F);
                                reg[0] = 0;
                                break;
                            default:
                                //printf("unknown instruction\n");
                                break;
                        }
                        break;
                    case 0x6:
                        //printf("or x%d, x%d, x%d\n", rd, rs1, rs2);
                        reg[rd] = reg[rs1] | reg[rs2];
                        reg[0] = 0;
                        break;
                    case 0x7:
                        //printf("and x%d, x%d, x%d\n", rd, rs1, rs2);
                        reg[rd] = reg[rs1] & reg[rs2];
                        reg[0] = 0;
                        break;
                    default:
                        //printf("unknown instruction\n");
                        break;
                }
                break;
            
            case 0x37:
                //printf("lui x%d, %d\n", rd, imm_u);
                reg[rd] = imm_u;
                reg[0] = 0;
                break;

            case 0x63:
                /*
                if(imm_sb & 0x800){
                    imm_sb = (int)((unsigned int)imm_sb | 0xFFFFF000);
                }
                */
                switch(func3) {
                    case 0x0:
                        //printf("beq x%d, x%d, %d\n", rs1, rs2, imm_b);
                        if(reg[rs1] == reg[rs2]){
                            pc += imm_b;
                        }
                        break;
                    case 0x1:
                        //printf("bne x%d, x%d, %d\n", rs1, rs2, imm_b);
                        if(reg[rs1] != reg[rs2]){
                            pc += imm_b;
                        }
                        break;
                    case 0x4:
                        //printf("blt x%d, x%d, %d\n", rs1, rs2, imm_b);
                        if((int)reg[rs1] < (int)reg[rs2]){
                            pc += imm_b;
                        }
                        break;
                    case 0x5:
                        //printf("bge x%d, x%d, %d\n", rs1, rs2, imm_b);
                        if((int)reg[rs1] >= (int)reg[rs2]){
                            pc += imm_b;
                        }
                        break;
                    /*
                    case 0x6:
                        printf("bltu x%d, x%d, %d\n", rs1, rs2, imm_sb);
                        break;
                    case 0x7:
                        printf("bgeu x%d, x%d, %d\n", rs1, rs2, imm_sb);
                        break;
                    */
                    default:
                        //printf("unknown instruction\n");
                        break;
                }
                break;

            case 0x67:{
                //printf("jalr x%d, %d(x%d)\n", rd, imm_i, rs1);
                unsigned int temp = reg[rs1] + imm_i;
                reg[rd] = pc + 4;
                pc = temp & ~1;
                reg[0] = 0;
                break;
            }

            case 0x6F:
                /*
                //printf("jal x%d, %d\n", rd, imm_uj);
                if(imm_uj & 0x80000){
                    imm_uj = (int)((unsigned int)imm_uj | 0xFFFF0000);
                }
                Reg[rd] = (id * 4) + 4;
                id += (imm_uj / 4);
                //Reg[0] = 0;
                */
                //printf("jal x%d, %d\n", rd, imm_j);
                reg[rd] = pc + 4;
                pc += imm_j;
                reg[0] = 0;
                break;

            default:
                //printf("unknown instruction\n");
                break;
        }
        
        if(opcode != 0x6F && opcode != 0x67 && (opcode != 0x63 || ((func3 == 0x0 && reg[rs1] != reg[rs2]) || (func3 == 0x1 && reg[rs1] == reg[rs2]) || (func3 == 0x4 && (int)reg[rs1] >= (int)reg[rs2]) || (func3 == 0x5 && (int)reg[rs1] < (int)reg[rs2])))){
            pc += 4;
        }
    }

    for(int i = 0; i < 32; ++i){
        printf("x%d: 0x%08x\n", i, reg[i]);
    }

    free(buf);
    return 0;
}