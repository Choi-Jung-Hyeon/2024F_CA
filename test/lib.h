#ifndef CA_LIB_H
#define CA_LIB_H

#include <stdio.h>

static unsigned int inst_num = 0;

void read_instruction_from_buf(unsigned int* inst, char const* buf, size_t idx);
void parse_b_type(unsigned int inst);
void parse_s_type(unsigned int inst);
void parse_r_type(unsigned int inst);
void parse_i_type0(unsigned int inst);
void parse_i_type1(unsigned int inst);
void parse_jalr(unsigned int inst);
void parse_jal(unsigned int inst);
void parse_lui(unsigned int inst);
void parse_auipc(unsigned int inst);

void parse_funct7_i_type1(unsigned int inst);
void parse_funct7_r_type0(unsigned int inst);
void parse_funct7_r_type1(unsigned int inst);

// B_TYPE
void process_beq(unsigned int inst, unsigned int rs1, unsigned int rs2, int l1);
void process_bne(unsigned int inst, unsigned int rs1, unsigned int rs2, int l1);
void process_blt(unsigned int inst, unsigned int rs1, unsigned int rs2, int l1);
void process_bge(unsigned int inst, unsigned int rs1, unsigned int rs2, int l1);
void process_bltu(unsigned int inst, unsigned int rs1, unsigned int rs2, int l1);
void process_bgeu(unsigned int inst, unsigned int rs1, unsigned int rs2, int l1);

// S_TYPE
void process_sb(unsigned int inst, unsigned int rs1, unsigned int rs2, int l1);
void process_sh(unsigned int inst, unsigned int rs1, unsigned int rs2, int l1);
void process_sw(unsigned int inst, unsigned int rs1, unsigned int rs2, int l1);

// R_TYPE
void process_add(unsigned int inst, unsigned int rs1, unsigned int rs2, unsigned int rd);
void process_sub(unsigned int inst, unsigned int rs1, unsigned int rs2, unsigned int rd);
void process_sll(unsigned int inst, unsigned int rs1, unsigned int rs2, unsigned int rd);
void process_slt(unsigned int inst, unsigned int rs1, unsigned int rs2, unsigned int rd);
void process_sltu(unsigned int inst, unsigned int rs1, unsigned int rs2, unsigned int rd);
void process_xor(unsigned int inst, unsigned int rs1, unsigned int rs2, unsigned int rd);
void process_srl(unsigned int inst, unsigned int rs1, unsigned int rs2, unsigned int rd);
void process_sra(unsigned int inst, unsigned int rs1, unsigned int rs2, unsigned int rd);
void process_or(unsigned int inst, unsigned int rs1, unsigned int rs2, unsigned int rd);
void process_and(unsigned int inst, unsigned int rs1, unsigned int rs2, unsigned int rd);

// I_TYPE0
void process_lb(unsigned int inst, unsigned int rs1, unsigned int rd, int l1);
void process_lh(unsigned int inst, unsigned int rs1, unsigned int rd, int l1);
void process_lw(unsigned int inst, unsigned int rs1, unsigned int rd, int l1);
void process_lbu(unsigned int inst, unsigned int rs1, unsigned int rd, int l1);
void process_lhu(unsigned int inst, unsigned int rs1, unsigned int rd, int l1);

// I_TYPE1
void process_addi(unsigned int inst, unsigned int rs1, unsigned int rd, int l1);
void process_slti(unsigned int inst, unsigned int rs1, unsigned int rd, int l1);
void process_sltiu(unsigned int inst, unsigned int rs1, unsigned int rd, int l1);
void process_xori(unsigned int inst, unsigned int rs1, unsigned int rd, int l1);
void process_ori(unsigned int inst, unsigned int rs1, unsigned int rd, int l1);
void process_andi(unsigned int inst, unsigned int rs1, unsigned int rd, int l1);
void process_slli(unsigned int inst, unsigned int rs1, unsigned int rd, int l1);
void process_srli(unsigned int inst, unsigned int rs1, unsigned int rd, int l1);
void process_srai(unsigned int inst, unsigned int rs1, unsigned int rd, int l1);

// JARL
void process_jalr(unsigned int inst, unsigned int rs1, unsigned int rd, int l1);

// JAL
void process_jal(unsigned int inst, unsigned int rd, int l1);

// LUI
void process_lui(unsigned int inst, unsigned int rd, int l1);

// AUIPC
void process_auipc(unsigned int inst, unsigned int rd, int l1);

// UNKNOWN
void process_unknown(unsigned int inst);

#endif //CA_LIB_H
