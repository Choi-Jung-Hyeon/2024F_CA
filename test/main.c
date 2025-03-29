#include <stdio.h>
#include "instructions.h"
#include "lib.h"

#define SIZE 1024

int main(int argc, char *argv[]) {
    FILE *fs;
    char buf[SIZE];

    fs = fopen(argv[1], "rb");
    size_t num_of_insts = fread(buf, 1, SIZE, fs) / 4;

    if (argc == 3) {
        char* inst=argv[1];
        //int n=(int)argv[2];
        printf("inst %s",inst);
    } else if (argc == 4) {

    }
    /*for (int i = 0, j = 0; i != num_of_insts; ++i, j += 4) {
        unsigned int inst = 0;
        read_instruction_from_buf(&inst, buf, j);

        switch (OPCODE(inst)) {
            case B_TYPE:
                parse_b_type(inst);
                break;
            case S_TYPE:
                parse_s_type(inst);
                break;
            case R_TYPE:
                parse_r_type(inst);
                break;
            case I_TYPE0:
                parse_i_type0(inst);
                break;
            case I_TYPE1:
                parse_i_type1(inst);
                break;
            case JALR:
                parse_jalr(inst);
                break;
            case JAL:
                parse_jal(inst);
                break;
            case LUI:
                parse_lui(inst);
                break;
            case AUIPC:
                parse_auipc(inst);
                break;
            default:
                process_unknown(inst);
        }
    }*/

    return 0;
}
