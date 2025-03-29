// // x0 - x31 레지스터 계산
// for (int i = 0; i < user_inst; i++)
// {
//     if (curr_i > all_size)
//         break;
//     reg[0] = 0;
//     buffer_ = buffer[curr_i]; // memory 매번 접근하는 거 방지
//     opcode = buffer_ << 25 >> 25;
//     rd = buffer_ << 20 >> 27;
//     funct3 = buffer_ << 17 >> 29;
//     rs1 = buffer_ << 12 >> 27; // rs
//     rs2 = buffer_ << 7 >> 27;  // rt
//     funct7 = buffer_ >> 25;
//     rs2_ = rs2;
//     switch (opcode)
//     {

//     case 0x37:
//         /* code lui */
//         imm1 = buffer_ >> 12 << 12;
//         reg[rd] = imm1;

//         break;

//     case 0x17:
//         /* code auipc */
//         imm1 = buffer_ >> 12 << 12;
//         reg[rd] = curr_i * 4 + imm1;

//         break;
//     case 0x6f:
//         /* code jal */

//         imm1 = ((buffer_ & 0x80000000) >> 11) | ((buffer_ & 0x7FE00000) >> 20) | ((buffer_ & 0x100000) >> 9) | (buffer_ & 0xFF000);

//         if (imm1 & 0x80000) // 20 bit 부호확장
//             imm1 = static_cast<int>(static_cast<unsigned>(imm1) | 0xFFFF0000);

//         reg[rd] = (curr_i * 4) + 4;
//         curr_i = curr_i + (imm1 / 4);
//         continue;
//     case 0x67:
//         /* code jalr */
//         imm1 = (buffer_ & 0xFFF00000) >> 20u;

//         reg[rd] = (curr_i * 4) + 4;
//         curr_i = curr_i + ((reg[rs1] + imm1) / 4);

//         continue;
//     case 0x63:
//         imm1 = ((buffer_ & 0x80000000) >> 19) | ((buffer_ & 0x7E000000) >> 20) | ((buffer_ & 0xF00) >> 7) | ((buffer_ & 0x80) << 4);
//         // Check if the most significant bit (bit 11) is set
//         if (imm1 & 0x800)
//         {
//             // Convert imm1 to a negative number
//             imm1 = static_cast<int>(static_cast<unsigned>(imm1) | 0xFFFFF000);
//         }
//         switch (funct3)
//         {
//         case 0x0:
//             // beq
//             if ((int)reg[rs1] == (int)reg[rs2])
//             {
//                 curr_i = curr_i + (imm1 / 4);
//                 continue; // curr_i ++ 안 하고 넘어가게
//             }

//             break;
//         case 0x1:
//             // bne
//             if ((int)reg[rs1] != (int)reg[rs2])
//             {
//                 curr_i = curr_i + (imm1 / 4);
//                 continue; // curr_i ++ 안 하고 넘어가게
//             }

//             break;
//         case 0x4:
//             // blt
//             if ((int)reg[rs1] < (int)reg[rs2])
//             {
//                 curr_i = curr_i + (imm1 / 4);
//                 continue; // curr_i ++ 안 하고 넘어가게
//             }

//             break;
//         case 0x5:
//             // bge
//             if ((int)reg[rs1] >= (int)reg[rs2])
//             {
//                 curr_i = curr_i + (imm1 / 4);
//                 continue; // curr_i ++ 안 하고 넘어가게
//             }

//             break;

//         default:
//             cout << "unknown instruction" << endl;
//             break;
//         }
//         break;
//     case 0x3:
//         switch (funct3)
//         {
//         // static_cast<unsigned>(imm1)
//         case 0x2:
//             // lw
//             // 4byte, 부호확장

//             imm1 = (buffer_ >> 20);
//             if (imm1 & 0x800)
//                 imm1 = static_cast<int>(static_cast<unsigned>(imm1) | 0xFFFFF000);

//             if (flag == 0)
//             {
//                 reg[rd] = data[((reg[rs1] + imm1) & 0xFFFF) / 4];
//                 cout << "x" << rd << ": " << hex << reg[rd] << endl;
//                 cout << "주소lw 저쟝 즁: " << hex << ((reg[rs1] + imm1) & 0xFFFF) << endl
//                      << endl;
//             }
//             if (flag == 1)
//             {
//                 cin >> user_lw;
//                 reg[rd] = user_lw;
//             }
//             break;

//         default:
//             cout << "unknown instruction" << endl;
//             break;
//         }
//         break;
//     case 0x23:
//         /* code sb - sw */
//         imm1 = (funct7 << 5) | rd;

//         switch (funct3)
//         {
//         case 0x2:
//             // sw, 4byte 저장
//             // inst 파일 하나일때만 char 출력
//             if (imm1 & 0x800)
//                 imm1 = static_cast<int>(static_cast<unsigned>(imm1) | 0xFFFFF000);
//             if (flag == 0)
//             {
//                 data[((reg[rs1] + imm1) & 0xFFFF) / 4] = reg[rs2];
//             }
//             if (flag == 1)
//                 cout << static_cast<char>(reg[rs2]);
//             break;

//         default:
//             cout << "unknown instruction" << endl;
//             break;
//         }
//         break;
//     case 0x13:
//         /* code addi - srai */
//         imm1 = (buffer_ & 0xFFF00000) >> 20;
//         if (imm1 & 0x800)
//         {
//             // Convert imm1 to a negative number
//             imm1 = static_cast<int>(static_cast<unsigned>(imm1) | 0xFFFFF000);
//         }
//         switch (funct3)
//         {
//         case 0x0:
//             // addi
//             reg[rd] = reg[rs1] + imm1;
//             break;
//         case 0x2:
//             // slti
//             reg[rd] = (static_cast<int>(rs1) < imm1) ? 1 : 0;

//             break;

//         case 0x4:
//             // xori
//             reg[rd] = reg[rs1] ^ imm1;

//             break;
//         case 0x6:
//             // ori
//             reg[rd] = reg[rs1] | imm1;

//             break;
//         case 0x7:
//             // andi
//             reg[rd] = reg[rs1] & imm1;

//             break;
//         case 0x1:
//             // slli
//             rs2_ = rs2 & 0x1F; // 하위 5bit만 유효하게 shift 하니깐
//             reg[rd] = (int)reg[rs1] << rs2_;

//             break;
//         case 0x5:
//             switch (funct7)
//             {
//             case 0x0:
//                 // srli
//                 rs2_ &= 0x1F; // 하위 5bit만 유효하게 shift 하니깐
//                 reg[rd] = (reg[rs1] >> rs2_);
//                 reg[rd] &= (0x7FFFFFFF >> (rs2_ - 1));

//                 break;
//             case 0x20:
//                 // srai
//                 rs2_ = rs2 & 0x1F; // 하위 5bit만 유효하게 shift 하니깐
//                 reg[rd] = (int)reg[rs1] >> (int)rs2_;

//                 break;

//             default:
//                 cout << "unknown instruction" << endl;
//                 break;
//             }
//             break;
//         default:
//             cout << "unknown instruction" << endl;
//             break;
//         }
//         break;

//     case 0x33: // 0b0110011 slli ~ add
//         switch (funct3)
//         {
//         case 0x00:
//             switch (funct7)
//             {
//             case 0x0:
//                 // add
//                 reg[rd] = reg[rs1] + reg[rs2];

//                 break;
//             case 0x20:
//                 reg[rd] = reg[rs1] - reg[rs2];

//                 break;

//             default:
//                 cout << "unknown instruction" << endl;
//                 break;
//             }
//             break;
//         case 0x1:
//             // sll
//             rs2_ = reg[rs2] & 0x7F; // 하위 5bit만 유효하게 shift 하니깐
//             reg[rd] = reg[rs1] << rs2_;

//             break;
//         case 0x2:
//             // slt
//             if (reg[rs1] < reg[rs2])
//             {
//                 reg[rd] = 1;
//             }
//             else
//                 reg[rd] = 0;

//             break;
//         case 0x4:
//             // xor
//             reg[rd] = reg[rs1] ^ reg[rs2];

//             break;
//         case 0x5:
//             switch (funct7)
//             {
//             case 0x0:
//                 // srl
//                 reg[rd] = reg[rs1] >> (reg[rs2] & 0x1F);

//                 break;

//             case 0x20:
//                 // sra
//                 rs2_ = reg[rs2] & 0x1F; // 하위 5bit만 유효하게 shift 하니깐
//                 reg[rd] = reg[rs1] >> rs2_;

//                 break;
//             default:
//                 cout << "unknown instruction" << endl;
//                 break;
//             }
//             break;
//         case 0x6:
//             // or
//             reg[rd] = reg[rs1] | reg[rs2];

//             break;
//         case 0x7:
//             // and
//             reg[rd] = reg[rs1] & reg[rs2];

//             break;
//         default:
//             cout << "unknown instruction" << endl;
//             break;
//         }
//         break;

//     default:
//         cout << "unknown instruction" << endl;
//         break;
//     }
//     reg[0] = 0;
//     curr_i++;
// }
