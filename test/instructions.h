#ifndef CA_INSTRUCTIONS_H
#define CA_INSTRUCTIONS_H

#define OPCODE(x)      (x & 0b1111111)
#define RD(x)          ((x & (0b11111                << 7))  >> 7)
#define FUNCT3(x)      ((x & (0b111                  << 12)) >> 12)
#define RS1(x)         ((x & (0b11111                << 15)) >> 15)
#define RS2(x)         ((x & (0b11111                << 20)) >> 20)
#define SHAMT(x)       (RS2(x))
#define FUNCT7(x)      ((x & (0b1111111              << 25)) >> 25)

#define I_TYPE_IMM(x)  ((x & (0b111111111111         << 20)) >> 20)
#define I_TYPE_SIGN(x)  (x & (0b1 << 11))
#define I_TYPE_VALUE(x) (x & 0b111111111111)

#define S_TYPE_IMM0(x) ((x & (0b1111111 << 25)) >> 20)
#define S_TYPE_IMM1(x) ((x & (0b11111   << 7))  >> 7)
#define S_TYPE_IMM(x)  ((S_TYPE_IMM0(x) | S_TYPE_IMM1(x)))
#define S_TYPE_SIGN(x)  (x & (0b1 << 11))
#define S_TYPE_VALUE(x) (x & 0b111111111111)

#define B_TYPE_IMM0(x) ((x & (0b1      << 31)) >> 20)
#define B_TYPE_IMM1(x) ((x & (0b1      << 7))  << 3)
#define B_TYPE_IMM2(x) ((x & (0b111111 << 25)) >> 21)
#define B_TYPE_IMM3(x) ((x & (0b1111   << 8))  >> 8)
#define B_TYPE_IMM(x)  ((B_TYPE_IMM0(x) | B_TYPE_IMM1(x) | B_TYPE_IMM2(x) | B_TYPE_IMM3(x)) << 1)
#define B_TYPE_SIGN(x)  (x & (0b1 << 12))
#define B_TYPE_VALUE(x) (x & 0b111111111111)

#define U_TYPE_IMM(x)  ((x & (0b11111111111111111111 << 12)))

#define J_TYPE_IMM0(x) ((x & (0b1          << 31)) >> 11)
#define J_TYPE_IMM1(x) ((x & (0b1111111111 << 21)) >> 20)
#define J_TYPE_IMM2(x) ((x & (0b1          << 20)) >> 9)
#define J_TYPE_IMM3(x) (x & (0b11111111   << 12))
#define J_TYPE_IMM(x)  ((J_TYPE_IMM0(x) | J_TYPE_IMM1(x) | J_TYPE_IMM2(x) | J_TYPE_IMM3(x)))

// OPCODE
#define B_TYPE 0b1100011
#define S_TYPE 0b0100011
#define R_TYPE 0b0110011

#define I_TYPE0 0b0000011
#define I_TYPE1 0b0010011
#define JALR 0b1100111

#define JAL 0b1101111

#define LUI 0b0110111
#define AUIPC 0b0010111

// FUNCT3

// I_TYPE0
#define LB 0b000
#define LH 0b001
#define LW 0b010
#define LBU 0b100
#define LHU 0b101

// I_TYPE1
#define ADDI 0b000
#define SLTI 0b010
#define SLTIU 0b011
#define XORI 0b100
#define ORI 0b110
#define ANDI 0b111
#define SLLI 0b001
#define SRLI_SRAI 0b101

// B_TYPE
#define BEQ 0b000
#define BNE 0b001
#define BLT 0b100
#define BGE 0b101
#define BLTU 0b110
#define BGEU 0b111

// S_TYPE
#define SB 0b000
#define SH 0b001
#define SW 0b010

// R_TYPE
#define ADD_SUB 0b000
#define SLL 0b001
#define SLT 0b010
#define SLTU 0b011
#define XOR 0b100
#define SRL_SRA 0b101
#define OR 0b110
#define AND 0b111

// FUNCT7

// I_TYPE1
#define SRLI_7 0b0000000
#define SRAI_7 0b0100000

// R_TYPE
#define ADD_7 0b0000000
#define SUB_7 0b0100000
#define SRL_7 0b0000000
#define SRA_7 0b0100000

#endif //CA_INSTRUCTIONS_H
