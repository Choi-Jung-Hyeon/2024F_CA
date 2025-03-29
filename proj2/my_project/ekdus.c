#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define OP 0x07
#define RD 0x05
#define FUNC3 0x03
#define RS1 0x05
#define RS2 0x05
#define FUNC7 0x07


/*instructions to support
add, sub, addi
xor, or, and, xori, ori, andi
slli, srli, srai, sll, srl, sra
slti, slt
auipc, lui
jal, jalr
beq, bne, blt, bge
lw, sw

excluded:
bltu, bgeu, lb, lh, lbu, lhu,
sb, sh, sltiu, sltu
*/

#define LUI 0x37
#define AUIPC 0x17
#define JAL 0x6F
#define JALR 0x67
#define BRANCH 0x63
#define LOAD 0x03
#define STORE 0x23
#define IMMARITH 0x13
#define ARITHMETIC 0x33

uint32_t registers[32] = { 0, };
uint8_t memory[0x10010000] = {0xFF, };
uint32_t pc = 0;


void error(){
   printf("unknown instruction\n");
   pc += 4;
}

int sign(int original, int bit) {
   int mask = 1 << (bit - 1); 
   int MSB = original & mask;
   if (MSB != 0) {
      original |= ~((1 << bit) - 1);
   }
   return original;
}

void printRegi(){
   for(int i=0; i<32; i++){
      printf("x%d: 0x%08x\n", i, registers[i]);
   }
}

void updateRegi(uint32_t rd, uint32_t value) {
    if (rd != 0) { // registers[0]은 항상 0이어야 함
        registers[rd] = value;
    }
} 

void instTOmem(uint32_t address, uint32_t instruction){
   memory[address] = (instruction >> 24) & 0xFF;
   memory[address+1] = (instruction >> 16) & 0xFF;
   memory[address+2] = (instruction >> 8) & 0xFF;
   memory[address+3] = (instruction) & 0xFF;
}

uint32_t memTOinst(uint32_t address) {
   uint32_t instruction = 0;
   instruction |= memory[address] << 24;
   instruction |= memory[address + 1] << 16;
   instruction |= memory[address + 2] << 8;
   instruction |= memory[address + 3];
   return instruction;
}

void store_a_word(int rs2){
   uint8_t byte = registers[rs2];
   printf("%c", byte);
}

void load_a_word(int rd){
   int target;
   scanf("%d", &target);
   updateRegi(rd, target);
}

void executeInst(uint32_t inst){
   unsigned int opcode = inst&((1<<OP)-1);
   inst = inst>>OP;
   unsigned int rd = inst&((1<<RD)-1);
   inst = inst>>RD;
   unsigned int func3 = inst&((1<<FUNC3)-1);
   inst = inst>>FUNC3;
   unsigned int rs1 = inst&((1<<RS1)-1);
   inst = inst>>RS1;
   unsigned int rs2 = inst&((1<<RS2)-1);
   inst = inst>>RS2;
   unsigned int func7 = inst&((1<<FUNC7)-1);
   inst = inst>>FUNC7;
   
   if(opcode == LUI||opcode == AUIPC||opcode == JAL){
      unsigned int imm = (func7<<13|rs2<<8|rs1<<3|func3);
      if(opcode==LUI){
         imm = sign(imm, 20);
         imm = imm<<12;
         updateRegi(rd, imm);
         pc+=4;
        //printf("lui x%d, %d(x%d)\n", rd, imm, rs1);
      }
      else if(opcode==AUIPC){
         imm = sign(imm, 20);
         imm = imm<<12;
         updateRegi(rd, pc+imm);
         pc+=4;
         //printf("auipc x%d, %d(x%d)\n", rd, imm, rs1);
      }
      else if(opcode==JAL){
         unsigned int imm19_12 = imm&((1<<8)-1);
         unsigned int imm11 = (imm>>8)&1;
         unsigned int imm10_1 = (imm>>9)&((1<<10)-1);
         unsigned int imm20 = (imm>>19)&1;
         imm = (imm20<<20)|(imm19_12<<12)|(imm11<<11)|(imm10_1<<1);
         imm = sign(imm, 20);
         updateRegi(rd, pc+4);
         pc += imm;
         //printf("jal x%d, %d(x%d)\n", rd, imm, rs1);
         }
      }
      else if(opcode == JALR||opcode == LOAD||opcode==IMMARITH){
         unsigned int imm = (func7<<5|rs2);
         imm = sign(imm, 12);
         if(opcode == JALR){
            updateRegi(rd, pc+4);
            pc = registers[rs1]+imm;
            //printf("jalr x%d, %d(x%d)\n", rd, imm, rs1);
         }
         else if(opcode == LOAD){
            switch(func3){
               case 0b010:
                  uint32_t address = (int)registers[rs1] + imm;
                  if(address == 0x20000000){
                     load_a_word(rd);
                  }
                  else{
                     //printf("%d, %d -> %d\n", (int)registers[rs1], imm, address);
                     updateRegi(rd, *(int32_t*)&memory[address]);
                  }
                  pc += 4;
                  //printf("lw x%d, %d(x%d)\n", rd, imm, rs1);
                  break;
               default:
                  error();
            }
         }
         else if(opcode == IMMARITH){
            switch(func3){
               case 0b000:
                  updateRegi(rd, (int)registers[rs1] + imm);
                  pc+=4;
                  //printf("addi x%d, x%d, %d\n", rd, rs1, imm);
                  break;
               case 0b010:
                  if((int)registers[rs1]<(int)imm){
                     updateRegi(rd, 1);
                  }
                  else{
                     updateRegi(rd, 0);
                  }
                  pc+=4;
                  //printf("slti x%d, x%d, %d\n", rd, rs1, imm);
                  break;
               case 0b100:
                  updateRegi(rd, registers[rs1]^imm);
                  pc += 4;
                  //printf("xori x%d, x%d, %d\n", rd, rs1, imm);
                  break;
               case 0b110:
                  updateRegi(rd, registers[rs1]|imm);
                  pc += 4;
                  //printf("ori x%d, x%d, %d\n", rd, rs1, imm);
                  break;
               case 0b111:
                  updateRegi(rd, registers[rs1]&imm);
                  pc += 4;
                  //printf("andi x%d, x%d, %d\n", rd, rs1, imm);
                  break;
               case 0b001:
                  //printf("%d, %d -> %d\n", registers[rs1], imm, registers[rs1]<<(imm & 0x1F));
                  updateRegi(rd, registers[rs1]<<(imm & 0x1F));
                  //printf("%d\n", registers[rd]);
                  pc += 4;
                  //printf("slli x%d, x%d, %d\n", rd, rs1, imm);
                  break;
               case 0b101:
                  unsigned int shamt = rs2;
                  if(func7 == 0b0){
                     updateRegi(rd, (uint32_t)registers[rs1]>>(shamt&0x1F));
                     //printf("srli x%d, x%d, %d\n", rd, rs1, shamt);
                  }
                  else if(func7 == 0b0100000){
                     updateRegi(rd, (int32_t)registers[rs1]>>(shamt&0x1F));
                     //printf("srai x%d, x%d, %d\n", rd, rs1, shamt);
                  }
                  pc += 4;
                  break;
               default:
                  error();
            }
      }
   }
   else{
      if(opcode == BRANCH){
         unsigned int imm = func7;
         unsigned int imm10_5 = imm &((1<<6)-1);
         unsigned int imm12 = (imm>>6)&1;
         unsigned int imm11 = rd&1;
         unsigned int imm4_1 = (rd>>1)&((1<<4)-1);
         imm = (imm12<<12|imm11<<11|imm10_5<<5|imm4_1<<1);
         imm = sign(imm, 12);
         switch(func3){
            case 0b000:
               if(registers[rs1] == registers[rs2]){
                  pc += (signed int)imm;
               }
               else{
                  pc += 4;
               }
               //printf("beq x%d, x%d, %d\n", rs1, rs2, imm);
               break;
            case 0b001:
               if(registers[rs1] != registers[rs2]){
                  pc += (signed int)imm;
               }
               else{
                  pc += 4;
               }
               //printf("bne x%d, x%d, %d\n", rs1, rs2, imm);
               break;
            case 0b100:
               if((int)registers[rs1] < registers[rs2]){
                  pc += (signed int)imm;
               }
               else{
                  pc += 4;
               }
               //printf("blt x%d, x%d, %d\n", rs1, rs2, imm);
               break;
            case 0b101:
               int component1 = (int)registers[rs1];
               int component2 = (int)registers[rs2];
               if(component1 >= component2){
                  pc += (signed int)imm;
                  //printf("%d bigger or equal %d\n", component1, component2);
               }
               else{
                  pc += 4;
                  //printf("%d NOT bigger or equal %d\n", component1, component2);
               }
               //printf("bge x%d, x%d, %d\n", rs1, rs2, imm);
               break;
            default:
               error();
         }
      }
      else if(opcode == STORE){
         unsigned int imm = (func7<<5)|rd;
         imm = sign(imm, 12);
         switch(func3){
            case 0b010:
               uint32_t address = registers[rs1]+imm;
               if(address == 0x20000000){
                  store_a_word(rs2);
               }
               else{
                  *(uint32_t*)&memory[address] = registers[rs2];
               }
               pc += 4;
               //printf("sw x%d, %d(x%d)\n", rs2, imm, rs1);
               break;
            default:
               error();
         }
      }
      else if(opcode == ARITHMETIC){
         switch(func3){
            case 0b000:
               if(func7 == 0b0){
                  updateRegi(rd, (int)registers[rs1] + (int)registers[rs2]);
                  pc += 4;
                  //printf("add x%d, x%d, x%d\n", rd, rs1, rs2);
               }
               else if(func7 == 0b0100000){
                  updateRegi(rd, (int)registers[rs1] - (int)registers[rs2]);
                  pc += 4;
                  //printf("sub x%d, x%d, x%d\n", rd, rs1, rs2);
               }
               else error();
               break;
            case 0b001:
               if(func7 == 0b0){
                  updateRegi(rd, registers[rs1]<<(registers[rs2]&0x1F));
                  pc += 4;
                  //printf("sll x%d, x%d, x%d\n", rd, rs1, rs2);
               }
               else error();
               break;
            case 0b010:
               if(func7 == 0b0){
                  if((int)registers[rs1]<(int)registers[rs2]){
                     updateRegi(rd, 1);
                  }
                  else{
                     updateRegi(rd, 0);
                  }
                  pc += 4;
                  //printf("slt x%d, x%d, x%d\n", rd, rs1, rs2);
               }
               else error();
               break;
            case 0b100:
               if(func7 == 0b0){
                  updateRegi(rd, registers[rs1]^registers[rs2]);
                  pc += 4;
                  //printf("xor x%d, x%d, x%d\n", rd, rs1, rs2);
               }
               else error();
               break;
            case 0b101:
               if(func7==0){
                  updateRegi(rd, (uint32_t)registers[rs1]>>(registers[rs2]&0x1F));
                  pc += 4;
                  //printf("srl x%d, x%d, x%d\n", rd, rs1, rs2);
               }
               else if(func7 == 0b0100000){
                  updateRegi(rd, (int32_t)registers[rs1]>>(registers[rs2]&0x1F));
                  pc += 4;
                  //printf("sra x%d, x%d, x%d\n", rd, rs1, rs2);
               }
               else error();
               break;
            case 0b110:
               if(func7 == 0b0){
                  updateRegi(rd, registers[rs1]|registers[rs2]);
                  pc += 4;
                  //printf("or x%d, x%d, x%d\n", rd, rs1, rs2);
               }
               else error();
               break;
            case 0b111:
               if(func7 == 0b0){
                  updateRegi(rd, registers[rs1]&registers[rs2]);
                  pc += 4;
                  //printf("and x%d, x%d, x%d\n", rd, rs1, rs2);
               }
               else error();
               break;
            default:
               error();
         }
      }
      else{
         error();
      }
   }
}

int main(int argc, char *argv[]) {
   if(argc != 3 && argc != 4){
      printf("error: not in a right instruction form\n");
      return 0;
   }

   FILE *file = fopen(argv[1], "rb");
   if (file == NULL) {
      return 1;
   }

   fseek(file, 0, SEEK_END); 
   long fileSize = ftell(file);
   fseek(file, 0, SEEK_SET); 
   int totInst = (fileSize+3)/4;
   int* inst = (int*)malloc(sizeof(int)*(int)totInst);
   size_t count = fread(inst, sizeof(int), totInst, file);
   if(count!=totInst){
      return 1;
   }

   for(int a = 0; a<totInst; a++){
      instTOmem(pc, inst[a]);
      pc += 4;
   }


   int instNumb=0;
   if(argc == 3){
      instNumb = atoi(argv[2]);
   }
   else if(argc == 4){ 
      FILE *dataFile = fopen(argv[2], "rb");
      fseek(dataFile, 0, SEEK_END);
      long fileSizeData = ftell(dataFile);
      fseek(dataFile, 0, SEEK_SET);
      
      fread(&memory[0x10000000], 1, fileSizeData, dataFile);

      fclose(dataFile);

      instNumb = atoi(argv[3]);
   }

   pc = 0;

   int checkInst[0x10000] = {0,};
   checkInst[4*totInst] = 2;
   int executedInst = 0;

   while((checkInst[pc] != 2) && (executedInst < instNumb)){
      executedInst++;
      uint32_t inst = memTOinst(pc);
      //printf("%d: ", executedInst);
      executeInst(inst);
   }

   printRegi();

   fclose(file);
   free(inst);

   return 0;
}