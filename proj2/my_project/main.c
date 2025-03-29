#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    int inst;
    //int check = 0;
    if(argc == 3) {
        inst = atoi(argv[2]);
        //check = 1;
    }
    else if(argc == 4) {
        inst = atoi(argv[3]);
    }
    else{
        return -1;
    }

    FILE* fp = fopen(argv[1], "rb");
    if(fp == NULL) {
        return -1;
    }

    fseek(fp, 0, SEEK_END);
    long total_inst = ftell(fp);
    int num = (total_inst + 3) / 4;
    int* buf = (int*)malloc(num * sizeof(int));
    if(inst > num) {
        inst = num;
    }

    fseek(fp, 0, SEEK_SET);
    int cnt = fread(buf, sizeof(int), num, fp);
    if(cnt != num) {
        fclose(fp);
        free(buf);
        return -1;
    }

    int Reg[32];
    char Mem[65536];
    for(int i = 0; i < 32; i++){
        Reg[i] = 0x00;
    }

    if(inst > num){
        inst = num;
    }

    total_inst /= 4;
    int id = 0;
    for(int i = 0; i < total_inst; i++) {
        unsigned int opcode = buf[id] & 0x7F;
        unsigned int rd = (buf[id] >> 7) & 0x1F;
        unsigned int func3 = (buf[id] >> 12) & 0x7;
        unsigned int rs1 = (buf[id] >> 15) & 0x1F;
        unsigned int rs2 = (buf[id] >> 20) & 0x1F;
        unsigned int func7 = (buf[id] >> 25) & 0x7F;

        int MSB = buf[id] >> 30;
        int imm = buf[id] >> 20;
        int imm_u = buf[id] & 0xFFFFF000;
        int imm_s = ((buf[id] & (int)0xFE000000) >> 20) + ((buf[id] & 0xF80) >> 7);
        int imm_uj = ((buf[id] & (int)0x80000000) >> 11) + (buf[id] & 0xFF000) + ((buf[id] & 0x100000) >> 9) + ((buf[id] & 0x7FE00000) >> 20);
        int imm_sb = ((buf[id] & (int)0x80000000) >> 19) + ((buf[id] & 0x80) << 4) + ((buf[id] & 0x7E000000) >> 20) + ((buf[id] & 0xF00) >> 7);
        //printf("inst %d: %08x ", id, buf[id]);

        Reg[0] = 0;
        switch(opcode) {
            case 0x03:
                switch(func3) {
                    /*
                    case 0x0:
                        printf("lb x%d, %d(x%d)\n", rd, imm, rs1);
                        break;
                    case 0x1:
                        printf("lh x%d, %d(x%d)\n", rd, imm, rs1);
                        break;
                    */
                    case 0x2:
                        //printf("lw x%d, %d(x%d)\n", rd, imm, rs1);
                        if(imm & 0x800){
                            imm = (int)((unsigned int)imm | 0xFFFFF000);
                        }
                        
                        switch(check){
                            case 0:
                                Reg[rd] = Data[((Reg[rs1] + imm) & 0xFFFF) / 4];
                                break;
                            case 1:
                                
                        }
                        break;
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
                        //printf("addi x%d, x%d, %d\n", rd, rs1, imm);
                        Reg[rd] = Reg[rs1] + imm;
                        break;
                    case 0x1:
                        //printf("slli x%d, x%d, %d\n", rd, rs1, imm);
                        Reg[rd] = Reg[rs1] << imm;
                        break;
                    case 0x2:
                        //printf("slti x%d, x%d, %d\n", rd, rs1, imm);
                        if(Reg[rs1] < imm){
                            Reg[rd] = 1;
                        }
                        else{
                            Reg[rd] = 0;
                        }
                        break;
                    case 0x3:
                        //printf("sltiu x%d, x%d, %d\n", rd, rs1, imm);
                        if(Reg[rs1] < ((int)(unsigned int)imm)){
                            Reg[rd] = 1;
                        }
                        else{
                            Reg[rd] = 0;
                        }
                        break;
                    case 0x4:
                        //printf("xori x%d, x%d, %d\n", rd, rs1, imm);
                        Reg[rd] = Reg[rs1] ^ imm;
                        break;
                    case 0x5:
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
                        break;
                    case 0x6:
                        //printf("ori x%d, x%d, %d\n", rd, rs1, imm);
                        Reg[rd] = Reg[rs1] | imm;
                        break;
                    case 0x7:
                        //printf("andi x%d, x%d, %d\n", rd, rs1, imm);
                        Reg[rd] = Reg[rs1] & imm;
                        break;
                    default:
                        //printf("unknown instruction\n");
                        break;
                }
                break;   

            case 0x17:
                //printf("auipc x%d, %d\n", rd, imm_u);
                Reg[rd] = id * 4 + imm_u;
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
                    case 0x2:
                        //printf("sw x%d, %d(x%d)\n", rs2, imm_s, rs1);
                        int addr = Reg[rs1] + imm_s - 0x10000000;

                        if(0 <= addr && addr < 0x0000FFFF){
                            Mem[addr] = Reg[rs2] & 0xFF;
                            Mem[addr + 1] = (Reg[rs2] >> 8) & 0xFF;
                            Mem[addr + 2] = (Reg[rs2] >> 16) & 0xFF;
                            Mem[addr + 3] = (Reg[rs2] >> 24) & 0xFF;
                        }
                        else if(addr == 0x10000000){
                            if(Reg[rs2] < 0x7F){
                                printf("")
                            }
                        }
                        break;
                    default:
                        //printf("unknown instruction\n");
                        break;
                }
                break;

            case 0x33:
                switch(func3) {
                    case 0x0:
                        switch(func7) {
                            case 0x0:
                                //printf("add x%d, x%d, x%d\n", rd, rs1, rs2);
                                Reg[rd] = Reg[rs1] + Reg[rs2];
                                break;
                            case 0x20:
                                //printf("sub x%d, x%d, x%d\n", rd, rs1, rs2);
                                Reg[rd] = Reg[rs1] - Reg[rs2];
                                break;
                            default:
                                //printf("unknown instruction\n");
                                break;
                        }
                        break;
                    case 0x1:
                        //printf("sll x%d, x%d, x%d\n", rd, rs1, rs2);
                        Reg[rd] = Reg[rs1] << (Reg[rs2] & 0x1F);
                        break;
                    case 0x2:
                        //printf("slt x%d, x%d, x%d\n", rd, rs1, rs2);
                        if(Reg[rs1] < Reg[rs2]){
                            Reg[rd] = 1;
                        }
                        else{
                            Reg[rd] = 0;
                        }
                        break;
                    case 0x3:
                        //printf("sltu x%d, x%d, x%d\n", rd, rs1, rs2);
                        if(Reg[rs1] < (int)((unsigned int)Reg[rs2])){
                            Reg[rd] = 1;
                        }
                        else{
                            Reg[rd] = 0;
                        }
                        break;
                    case 0x4:
                        //printf("xor x%d, x%d, x%d\n", rd, rs1, rs2);
                        Reg[rd] = Reg[rs1] ^ Reg[rs2];
                        break;
                    case 0x5:
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
                        break;
                    case 0x6:
                        //printf("or x%d, x%d, x%d\n", rd, rs1, rs2);
                        Reg[rd] = Reg[rs1] | Reg[rs2];
                        break;
                    case 0x7:
                        //printf("and x%d, x%d, x%d\n", rd, rs1, rs2);
                        Reg[rd] = Reg[rs1] & Reg[rs2];
                        break;
                    default:
                        //printf("unknown instruction\n");
                        break;
                }
                break;
            
            case 0x37:
                //printf("lui x%d, %d\n", rd, imm_u);
                Reg[rd] = imm_u;
                break;

            case 0x63:
                if(imm_sb & 0x800){
                    imm_sb = (int)((unsigned int)imm_sb | 0xFFFFF000);
                }
                switch(func3) {
                    case 0x0:
                        //printf("beq x%d, x%d, %d\n", rs1, rs2, imm_sb);
                        if(Reg[rs1] == Reg[rs2]){
                            id += (imm_sb / 4);
                            continue;
                        }
                        break;
                    case 0x1:
                        //printf("bne x%d, x%d, %d\n", rs1, rs2, imm_sb);
                        if(Reg[rs1] != Reg[rs2]){
                            id += (imm_sb / 4);
                            continue;
                        }
                        break;
                    case 0x4:
                        //printf("blt x%d, x%d, %d\n", rs1, rs2, imm_sb);
                        if(Reg[rs1] < Reg[rs2]){
                            id += (imm_sb / 4);
                            continue;
                        }
                        break;
                    case 0x5:
                        //printf("bge x%d, x%d, %d\n", rs1, rs2, imm_sb);
                        if(Reg[rs1] >= Reg[rs2]){
                            id += (imm_sb / 4);
                            continue;
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

            case 0x67:
                //printf("jalr x%d, %d(x%d)\n", rd, imm, rs1);
                Reg[rd] = id * 4 + 4;
                id += (Reg[rs1] + imm) / 4;
                //Reg[0] = 0;
                continue;

            case 0x6F:
                //printf("jal x%d, %d\n", rd, imm_uj);
                if(imm_uj & 0x80000){
                    imm_uj = (int)((unsigned int)imm_uj | 0xFFFF0000);
                }
                Reg[rd] = (id * 4) + 4;
                id += (imm_uj / 4);
                //Reg[0] = 0;
                continue;

            default:
                //printf("unknown instruction\n");
                break;
        }
        Reg[0] = 0;
        id++;
    }

    for(int i = 0; i < 32; i++){
        printf("x%d: 0x", i);
        printf("%08d\n", Reg[i]);
    }

    fclose(fp);
    free(buf);
    return 0;
}