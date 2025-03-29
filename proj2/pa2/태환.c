#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MAX 100000
#define NINE 9999
// 토요일 9시추가
#define MemoryMax 65536
double pow(double a, double b){
    if(b==0) return 1;
    else if(b==1) return a;
    else return a*pow(a,b-1);
}

char* my_htob(char c) { // hex를 binary code로 변경
 	int n, arr[4] = { 0, };
 	n = (int)c - 48;
     char *temp = (char*)malloc(sizeof(char)*4);

 	if (n > 9) n -= 7;

 	for (int PC = 0; PC < 4; PC++) {
 		arr[PC] = (n % 2);
 		n /= 2;
 	}

 	for (int PC = 0; PC < 4; PC++) {
         temp[3-PC] = arr[PC]+48;
		
 	}
     return temp; 
}

int signedbintodec(char* bin) {   // binary를 signed decimal로 변경
	int len = strlen(bin);  
	int dec = 0;
	for (int PC = 0; PC < len; PC++) {
		int n = bin[PC]-48;
		if (PC == 0) dec += (-1) * pow(2, len - PC - 1) * n;
		else dec += pow(2, len - PC - 1) * n;
	}
	return dec;
}
int unsignedbintodec(char* bin) {   // binary를 unsigned decimal로 변경
	int len = strlen(bin);  
	int dec = 0;
	for (int PC = 0; PC < len; PC++) {
        
		int n = bin[PC]-48;
		dec += n * pow(2,len-PC-1);

	}
	return dec;
}

char* intohex(int dec)
{
    int m, n;
    int k = 0;
    char c_hex[] = "0123456789abcdef";
    char* hex = (char*)malloc(sizeof(char)*10);
    while (1)
    {
        m = dec / 16; // 16으로 나눈 몫
        n = dec % 16; // 16으로 나눈 나머지 
        hex[k++] = c_hex[n];
        if (m <= 0) 
            break;
        dec = m;
    }
    for(int PC = 0 ; PC < k/2 ; PC++)
    {
        char temp = hex[PC];
        hex[PC] = hex[k-1-PC];
        hex[k-1-PC] = temp;
    }
    
    
    return hex;

}
int nUmberCheck(int arr[9999])
{
	int i, count = 0;
	for (i = 0; i <= NINE - 1; i++)
	{
		if (arr[i] != 2)
		{
			count += 1;
		}
		else continue;
	}

	return count;
}

char* dectobin32(int dec)
{
	int iNpuTnUmber = 0, qUotiEnt = 0, sTackCount = 0;		// qUotiEnt = 몫
	int i, u, biNaryCount, zEroCount, oCta;
	int biNaryArray[NINE];		// 나머지가 저장되는 2진수 배열
    char* bin = (char*)malloc(sizeof(char)*33);

		/*변수 초기화*/
		biNaryCount = 0, zEroCount = 0, oCta = 8;

		/*biNaryArray배열을 전부 2로 초기화 합니다.*/
		for (i = 0; i <= NINE - 1; i++)
		{
			biNaryArray[i] = 2;
		}

		
		iNpuTnUmber = dec;

		/*입력된 값이 양수일 경우*/
		if(iNpuTnUmber >= 0)
		{
			/*입력된 값을 몫변수에 저장*/
			qUotiEnt = iNpuTnUmber;

			/*입력 받은 값을 2로 나누어 나머지는 배열에 몫은 또 나누기 위해 몫에 저장합니다.*/
			for (i = 0; qUotiEnt > 0; i++)
			{
				biNaryArray[i] = qUotiEnt % 2;
				qUotiEnt = qUotiEnt / 2;
			}

			/*나머지가 몇 개 저장되었는지 확인하는 함수를 호출*/
			biNaryCount = nUmberCheck(biNaryArray);

			/*8자리를 만들기 위해 8의 배수에서 현재 배열의 갯수를 빼줍니다.*/
			while(1)
			{
				if (biNaryCount <= oCta)
				{
					zEroCount = oCta - biNaryCount;
					break;
				}
				else
				{
					oCta *= 2;
					continue;
				}
				
			}
			for (; zEroCount > 0; i++)
			{
				biNaryArray[i] = 0;
				zEroCount--;
			}

            int k = 0;
        
            for(u = 0; u < i ; u++)
            {
                bin[k++] = biNaryArray[u]+48;
            }
            
            if(k < 32)
            {
                for(int i=k;i<32;i++)
                {
                    bin[k++] = '0';
                }
            }
            for(int PC = 0 ; PC < k/2 ; PC++)
            {
                char temp2 = bin[PC];
                bin[PC] = bin[k-1-PC];
                bin[k-1-PC] = temp2;
            }
            

		}
		/*입력된 값이 음수일 경우*/
		else if(iNpuTnUmber < 0)
		{
			/*입력된 값을 * -1하여 몫변수에 저장*/
			qUotiEnt = iNpuTnUmber * -1;

			/*입력 받은 값을 2로 나누어 나머지는 배열에 몫은 또 나누기 위해 몫에 저장합니다.*/
			for (i = 0; qUotiEnt > 0; i++)
			{
				biNaryArray[i] = qUotiEnt % 2;
				qUotiEnt = qUotiEnt / 2;
			}

			/*나머지가 몇 개 저장되었는지 확인하는 함수를 호출*/
			biNaryCount = nUmberCheck(biNaryArray);

			/*배열에 저장된 나머지를 0은 1로 1은 0으로 변환합니다.*/
			for (u = 0; u < biNaryCount; u++)
			{
				if (biNaryArray[u] == 0) biNaryArray[u] = 1;
				else if (biNaryArray[u] == 1) biNaryArray[u] = 0;
			}

			/*1을 더해줍니다.*/
			for (u = 0; u < biNaryCount; u++)
			{
				if (biNaryArray[u] == 1)
				{
					biNaryArray[u] = 0;
					sTackCount = u;
				}
				else if(biNaryArray[u] == 0)
				{
					biNaryArray[u] = 1;
					break;
				}
			}

			/*8자리를 만들기 위해 8의 배수에서 현재 배열의 갯수를 빼줍니다.*/
			while (1)
			{
				if (biNaryCount <= oCta)
				{
					zEroCount = oCta - biNaryCount;
					break;
				}
				else
				{
					oCta *= 2;
					continue;
				}
			}

			for (; zEroCount > 0; i++)
			{
				biNaryArray[i] = 1;
				zEroCount--;
			}
            int k = 0;
        
            for(u = 0; u < i ; u++)
            {
                bin[k++] = biNaryArray[u]+48;
            }
            
            if(k < 32)
            {
                for(int i=k;i<32;i++)
                {
                    bin[k++] = '1';
                }
            }
            for(int PC = 0 ; PC < k/2 ; PC++)
            {
                char temp2 = bin[PC];
                bin[PC] = bin[k-1-PC];
                bin[k-1-PC] = temp2;
            }
            


		}
        return bin;
    
}

char* dectobin8(int dec)

{
	int iNpuTnUmber = 0, qUotiEnt = 0, sTackCount = 0;		// qUotiEnt = 몫
	int i, u, biNaryCount, zEroCount, oCta;
	int biNaryArray[NINE];		// 나머지가 저장되는 2진수 배열
    char* bin = (char*)malloc(sizeof(char)*33);

		/*변수 초기화*/
		biNaryCount = 0, zEroCount = 0, oCta = 8;

		/*biNaryArray배열을 전부 2로 초기화 합니다.*/
		for (i = 0; i <= NINE - 1; i++)
		{
			biNaryArray[i] = 2;
		}

		
		iNpuTnUmber = dec;

		/*입력된 값이 양수일 경우*/
		if(iNpuTnUmber >= 0)
		{
			/*입력된 값을 몫변수에 저장*/
			qUotiEnt = iNpuTnUmber;

			/*입력 받은 값을 2로 나누어 나머지는 배열에 몫은 또 나누기 위해 몫에 저장합니다.*/
			for (i = 0; qUotiEnt > 0; i++)
			{
				biNaryArray[i] = qUotiEnt % 2;
				qUotiEnt = qUotiEnt / 2;
			}

			/*나머지가 몇 개 저장되었는지 확인하는 함수를 호출*/
			biNaryCount = nUmberCheck(biNaryArray);

			/*8자리를 만들기 위해 8의 배수에서 현재 배열의 갯수를 빼줍니다.*/
			while(1)
			{
				if (biNaryCount <= oCta)
				{
					zEroCount = oCta - biNaryCount;
					break;
				}
				else
				{
					oCta *= 2;
					continue;
				}
				
			}
			for (; zEroCount > 0; i++)
			{
				biNaryArray[i] = 0;
				zEroCount--;
			}

            int k = 0;
        
            for(u = 0; u < i ; u++)
            {
                bin[k++] = biNaryArray[u]+48;
            }
            
            if(k < 8)
            {
                for(int i=k;i<8;i++)
                {
                    bin[k++] = '0';
                }
            }
            for(int PC = 0 ; PC < k/2 ; PC++)
            {
                char temp2 = bin[PC];
                bin[PC] = bin[k-1-PC];
                bin[k-1-PC] = temp2;
            }
            

		}
		/*입력된 값이 음수일 경우*/
		else if(iNpuTnUmber < 0)
		{
			/*입력된 값을 * -1하여 몫변수에 저장*/
			qUotiEnt = iNpuTnUmber * -1;

			/*입력 받은 값을 2로 나누어 나머지는 배열에 몫은 또 나누기 위해 몫에 저장합니다.*/
			for (i = 0; qUotiEnt > 0; i++)
			{
				biNaryArray[i] = qUotiEnt % 2;
				qUotiEnt = qUotiEnt / 2;
			}

			/*나머지가 몇 개 저장되었는지 확인하는 함수를 호출*/
			biNaryCount = nUmberCheck(biNaryArray);

			/*배열에 저장된 나머지를 0은 1로 1은 0으로 변환합니다.*/
			for (u = 0; u < biNaryCount; u++)
			{
				if (biNaryArray[u] == 0) biNaryArray[u] = 1;
				else if (biNaryArray[u] == 1) biNaryArray[u] = 0;
			}

			/*1을 더해줍니다.*/
			for (u = 0; u < biNaryCount; u++)
			{
				if (biNaryArray[u] == 1)
				{
					biNaryArray[u] = 0;
					sTackCount = u;
				}
				else if(biNaryArray[u] == 0)
				{
					biNaryArray[u] = 1;
					break;
				}
			}

			/*8자리를 만들기 위해 8의 배수에서 현재 배열의 갯수를 빼줍니다.*/
			while (1)
			{
				if (biNaryCount <= oCta)
				{
					zEroCount = oCta - biNaryCount;
					break;
				}
				else
				{
					oCta *= 2;
					continue;
				}
			}

			for (; zEroCount > 0; i++)
			{
				biNaryArray[i] = 1;
				zEroCount--;
			}
            int k = 0;
        
            for(u = 0; u < i ; u++)
            {
                bin[k++] = biNaryArray[u]+48;
            }
            
            if(k < 8)
            {
                for(int i=k;i<8;i++)
                {
                    bin[k++] = '1';
                }
            }
            for(int PC = 0 ; PC < k/2 ; PC++)
            {
                char temp2 = bin[PC];
                bin[PC] = bin[k-1-PC];
                bin[k-1-PC] = temp2;
            }
            


		}
        return bin;
    
}
char datachange(char c)
{
    char temp;
    if(c >= 48 && c <= 57)
    {
        
        temp = c - 48;
    }
    else if(c>= 97 && c <= 102)
    {
        temp = c- 87;
    }
    
    return temp;
}
int main(int argc, char** argv) {
    
    int registers[32] = {0,};

    int PC;
    
    // input bin file
    FILE* fp = fopen(argv[1], "rb");
    
    unsigned char input[MAX];
    int cnt = fread(input, sizeof(char), MAX, fp);
    fclose(fp);
    
    int n;  // instruction count

    if(argc == 3)   // input file이 data까지 있나 inst까지만 있나 확인용 
    {
        n = atoi(argv[2]);
    
    }
    
    int cnt2;
    unsigned char input2[MAX];  // data파일을 읽어오는 구간 시작 
    char**datastr;
    if(argc == 4)
    {
        FILE * fp = fopen(argv[2],"rb");
        n= atoi(argv[3]);
        cnt2 = fread(input2,sizeof(char),MAX,fp);
        fclose(fp);
        datastr = (char**)malloc(sizeof(char*) * cnt2 / 4);
        for(int i = 0; i < cnt2/4 ; i++)
        {
            datastr[i] = (char*)malloc(sizeof(char) * 8);
        }
        for (int i=0; i<cnt2 /4; i++) {
            char str_temp[32];
            for (int j=0; j<4; j++) {
                char c[8];
                sprintf(c, "%02x", input2[i*4+j]);
                // little endian (put new byte in the front)
                char *tmp = strdup(str_temp);
                strcpy(str_temp, c);
                strcat(str_temp, tmp);
            }
            datastr[i] = strdup(str_temp);
            strcpy(str_temp, "");
        }   // datastr에 data파일을 저장한다. 
    }
    char Data_Memory[MemoryMax];
    memset(Data_Memory,255,sizeof(Data_Memory));  // 64KB 
    int k = 0;
    int j= 7;
    int num = 0;
    int index = 0;
    if(argc == 4)
    {
        while(1) // data 개수만큼 초기화 하기.
        {   // 주소 1개당 byte1개 들어가야함.
            
            Data_Memory[index++] = datachange(datastr[k][6]) * 16 + datachange(datastr[k][7]);
            
            Data_Memory[index++] = datachange(datastr[k][4]) * 16 + datachange(datastr[k][5]);
            
            Data_Memory[index++] = datachange(datastr[k][2]) * 16 + datachange(datastr[k][3]);
            
            Data_Memory[index++] = datachange(datastr[k][0]) * 16 + datachange(datastr[k][1]);
            
            k++;
            if((index == MemoryMax) || (k == cnt2/4))
            {
                break;
            }
                
        }
    }
    // divide input by instructions, format little endian


    // instruction을 읽어오는 구간 시작
    char** hexstr = (char**)malloc(sizeof(char*) * cnt/4);
    for(int i=0; i<cnt/4; i++){
        hexstr[i] = (char*)malloc(sizeof(char) * 8);
    }
    
    for (int i=0; i<cnt/4; i++) {
        char str_temp[32];
        for (int j=0; j<4; j++) {
            char c[8];
            sprintf(c, "%02x", input[i*4+j]);
            // little endian (put new byte in the front)
            char *tmp = strdup(str_temp);
            strcpy(str_temp, c);
            strcat(str_temp, tmp);
        }
        hexstr[i] = strdup(str_temp);
        strcpy(str_temp, "");
    }
    // instruction 읽어오는 구간 끝 

    // 일단 초기화만 해두고, 
    char** Inst_Memory = (char**)malloc(sizeof(char*) * MemoryMax/4);  // word 갯수만큼 초기화 
    

    for(int i = 0; i < MemoryMax/4 ; i++)
    {
        Inst_Memory[i] = (char*)malloc(sizeof(char)*33); // word
    }
    // 여기가 문제 
    for(int i = 0; i < cnt/4 ; i++) // 인스트럭션 개수만큼 초기화 하기.
    {
        for (int j=0; j<8; j++) 
        {
            strcat(Inst_Memory[i],my_htob(hexstr[i][j]));
        }
    
    }

    // 남은 부분 초기화 하기.
    for(int i = (cnt/4) ; i < MemoryMax/4 ; i++)
    {
        Inst_Memory[i]="11111111111111111111111111111111"; //0xFFFFFFFF
    }
    char* Byte_Memory;
    Byte_Memory = (char*)malloc(sizeof(Byte_Memory)*8);

    int InsNum = cnt/4 + 35;    // 토요일 9시 초기화    
    
    // Calculation
    PC=0;
    for(int i = 0; i< n ; i++) {
       
        if(PC < 0 || PC > MemoryMax/4)
        {
            printf("Memory adderss error\n");
            break;
        }
        if(!strcmp(Inst_Memory[PC/4],"11111111111111111111111111111111"))
        {
           
            //printf("unknown instruction\n");
            break;
        }
        char opcode[8]; // opcode = 25,26,27,28,29,30,31 ok
        char func3[4]; // 17,18,19 ok
        char func7[8]; // 0,1,2,3,4,5,6 ok
        char imm11_0[13]; // 0,1,2,3,4,5,6,7,8,9,10,11 ok

        char imm11_5[8]; // 0,1,2,3,4,5,6 ok
        char imm4_0[6];  // 20,21,22,23,24 ok

        char imm12[2]; // 0 ok
        char imm10_5[7];  // 1,2,3,4,5,6 ok
        char imm4_1[5]; // 20,21,22,23; ok
        char imm11__24[2]; // 24; ok

        char imm31_12[21];    // 0~19 ok

        char imm20[2]; // 0 ok
        char imm10_1[11];  // 1,2,3,4,5,6,7,8,9,10 ok
        char imm11__11[2];   // 11 ok
        char imm19_12[9]; //12,13,14,15,16,17,18,19 ok

        char shamtstr[6]; // 7,8,9,10,11 ok
        char rdstr[6]; // 20,21,22,23,24 ok
        char rs1str[6]; // 12,13,14,15,16 ok
        char rs2str[6]; // 7,8,9,10,11 ok

        memcpy(opcode, &Inst_Memory[PC/4][25], 7);
        memcpy(func3, &Inst_Memory[PC/4][17], 3);
        
        memcpy(func7, &Inst_Memory[PC/4][0], 7);
        memcpy(imm11_0, &Inst_Memory[PC/4][0], 12);
        
        memcpy(imm11_5, &Inst_Memory[PC/4][0], 7);
        memcpy(imm4_0, &Inst_Memory[PC/4][20], 5);

        memcpy(imm12,&Inst_Memory[PC/4][0],1);  // 0
        memcpy(imm10_5,&Inst_Memory[PC/4][1],6);    // 1,2,3,4,5,6
        memcpy(imm4_1,&Inst_Memory[PC/4][20],4);    // 20 , 21, 22 ,23 
        memcpy(imm11__24,&Inst_Memory[PC/4][24],1); // 24

        memcpy(imm31_12, &Inst_Memory[PC/4][0], 20);

        memcpy(imm20,&Inst_Memory[PC/4][0],1);  // 0 
        memcpy(imm10_1,&Inst_Memory[PC/4][1],10);  // 1,2,3,4,5,6,7,8,9,10
        memcpy(imm11__11,&Inst_Memory[PC/4][11],1); // 11
        memcpy(imm19_12,&Inst_Memory[PC/4][12],8);  // 12,13,14,15,16,17,18,19

        memcpy(shamtstr,&Inst_Memory[PC/4][7],5);
        memcpy(rdstr, &Inst_Memory[PC/4][20], 5);
        memcpy(rs2str, &Inst_Memory[PC/4][7], 5);
        memcpy(rs1str, &Inst_Memory[PC/4][12], 5);
        opcode[7] = '\0'; 
        func3[3]= '\0'; 
        func7[7]= '\0'; 
        imm11_0[12]= '\0'; 

        imm11_5[7]= '\0'; 
        imm4_0[5]= '\0';  

        imm12[1]= '\0'; 
        imm10_5[6]= '\0'; 
        imm4_1[4]= '\0'; 
        imm11__24[1]= '\0'; 

        imm31_12[20]= '\0';    

        imm20[1]= '\0'; 
        imm10_1[10]= '\0';  
        imm11__11[1]= '\0';   
        imm19_12[8]= '\0'; 

        shamtstr[5]= '\0'; 
        rdstr[5]= '\0'; 
        rs1str[5]= '\0'; 
        rs2str[5]= '\0'; 
     

        int rs1 = unsignedbintodec(rs1str);
        int rs2 = unsignedbintodec(rs2str);
        int rd = unsignedbintodec(rdstr);

        // R-Type Instructions
		if (strcmp(opcode,"0110011")==0) {
            if (strcmp(func3,"000")== 0 && strcmp(func7,"0000000")==0) 
            {
                // printf("add ");
                registers[rd] = registers[rs1] + registers[rs2];
            }
            else if(strcmp(func3,"000")== 0 && strcmp(func7,"0100000")==0) 
            {
                // printf("sub ");
                registers[rd] = registers[rs1] - registers[rs2];
            }
            else if(strcmp(func3,"001")== 0) 
            {
                // printf("sll ");
                registers[rd] = registers[rs1] << registers[rs2];
            }
            else if(strcmp(func3,"010")== 0) 
            {
                
                // printf("slt ");
                registers[rd] = (registers[rs1] < registers[rs2])?1:0;
            }
            else if(strcmp(func3,"011")== 0) 
            {
                // printf("sltu ");
                registers[rd] = ((unsigned)registers[rs1] < (unsigned)registers[rs2])?1:0;
            }
            
            else if(strcmp(func3,"100")== 0) 
            {
                // printf("xor ");
                registers[rd] = registers[rs1] ^ registers[rs2];
            }
            else if(strcmp(func3,"101")== 0 && strcmp(func7,"0000000")==0) 
            {
                // printf("srl ");
                registers[rd] = (unsigned)registers[rs1] >> registers[rs2];
                
            }
            else if(strcmp(func3,"101")== 0 && strcmp(func7,"0100000")==0) 
            {
                // printf("sra ");
                registers[rd] = registers[rs1] >> registers[rs2];
            }
            else if(strcmp(func3,"110")== 0) 
            {
                // printf("or ");
                registers[rd] = registers[rs1] | registers[rs2];
            }
            else if(strcmp(func3,"111")== 0) 
            {
                // printf("and ");
                registers[rd] = registers[rs1] & registers[rs2];
            }
                // printf("x%d, x%d, x%d\n",rd,rs1,rs2);
			 // rd, rs1, rs2
		}
    
        // I-Type Instructions
        else if (strcmp(opcode,"0000011") ==0 || strcmp(opcode,"1100111") == 0 || strcmp(opcode,"0010011")==0) {
            
            int imm = signedbintodec(imm11_0);
            // printf("imm: %d",imm);
            if(strcmp(func3,"000")== 0 && strcmp(opcode,"0000011") ==0) 
            {
                
                // printf("lb x%d, %d(x%d)\n",rd,imm,rs1);
                if(registers[rs1]+imm < 0x10000000 || registers[rs1]+imm > 0x1000ffff)
                {
                    
                    printf("Memory adderss error\n");
                    break;
                }
                registers[rd] = signedbintodec(dectobin8((int)Data_Memory[registers[rs1]+imm-0x10000000]));
    
            }
            
            else if(strcmp(func3,"001")== 0 && strcmp(opcode,"0000011") ==0) 
            {
                
                // printf("lh x%d, %d(x%d)\n",rd,imm,rs1);
                if(registers[rs1]+imm < 0x10000000 || registers[rs1]+imm > 0x1000fffe)
                {
                    
                    printf("Memory adderss error\n");
                    break;
                }
                char * temp = (char*)malloc(sizeof(char)*17);
                strcat(temp,dectobin8((int)Data_Memory[registers[rs1]+imm-0x10000000+1]));
                strcat(temp,dectobin8((int)Data_Memory[registers[rs1]+imm-0x10000000]));
                registers[rd] = signedbintodec(temp);
                free(temp);
            }
            else if(strcmp(func3,"010")== 0 && strcmp(opcode,"0000011") ==0) 
            {
                // printf("lw x%d, %d(x%d)\n",rd,imm,rs1);
                if(registers[rs1]+imm == 0x20000000)
                {
                    int num;
                    scanf("%d",&num);
                    registers[rd] = num;
                    
                }
                else {
                    char * temp = (char*)malloc(sizeof(char)*33);  
                    strcat(temp,dectobin8((int)Data_Memory[registers[rs1]+imm-0x10000000+3]));
                    strcat(temp,dectobin8((int)Data_Memory[registers[rs1]+imm-0x10000000+2]));
                    strcat(temp,dectobin8((int)Data_Memory[registers[rs1]+imm-0x10000000+1]));
                    strcat(temp,dectobin8((int)Data_Memory[registers[rs1]+imm-0x10000000]));
                    registers[rd] = signedbintodec(temp);
                    free(temp);  
                }
                
                
            
            }
            else if(strcmp(func3,"100")== 0 && strcmp(opcode,"0000011") ==0) 
            {   
                // printf("lbu x%d, %d(x%d)\n",rd,imm,rs1);
                if((registers[rs1]+imm >= 0x10000000)&& (registers[rs1]+imm <= 0x1000ffff))
                {
                    char * temp = (char*)malloc(sizeof(char)*9);
                    strcat(temp,dectobin8((int)Data_Memory[registers[rs1]+imm-0x10000000]));
                    registers[rd] = unsignedbintodec(temp);
                    free(temp);
                }
                else if(registers[rs1]+imm == 0x20000000)
                {
                    unsigned int num;
                    scanf("%d",&num);
                    registers[rd] = num;
                    
                }
                else 
                {
                   
                    printf("Memory adderss error\n");
                    break;
                }
                
                
            }
            else if(strcmp(func3,"101")== 0 && strcmp(opcode,"0000011") ==0) 
            {
                // printf("lhu x%d, %d(x%d)\n",rd,imm,rs1);
                if(registers[rs1]+imm < 0x10000000 || registers[rs1]+imm > 0x1000fffe)
                {
                    
                    printf("Memory adderss error\n");
                    break;
                }
                char * temp = (char*)malloc(sizeof(char)*17);
                strcat(temp,dectobin8((int)Data_Memory[registers[rs1]+imm-0x10000000])+1);
                strcat(temp,dectobin8((int)Data_Memory[registers[rs1]+imm-0x10000000]));
                registers[rd] = unsignedbintodec(temp);
                free(temp);
            }
            else if(strcmp(func3,"000")== 0 && strcmp(opcode,"1100111") ==0) 
            {
                // printf("jalr x%d, x%d, %d\n",rd,rs1,imm);
                registers[rd] = PC+4;
                PC = registers[rs1] + imm-4;   // 맨마지막에 PC 추가해주니까!
            }
            
			if(strcmp(opcode,"0010011")==0 && (strcmp(func3,"000")== 0 || strcmp(func3,"010")== 0 || strcmp(func3,"011")== 0 || strcmp(func3,"100")== 0|| strcmp(func3,"110")== 0 || strcmp(func3,"111")== 0)) 
			{
                
                if(strcmp(func3,"000")== 0 &&strcmp(opcode,"0010011")==0) 
                {
                    
                    // printf("addi ");
                    registers[rd] = registers[rs1] + imm;
                    // printf("rd: %d\n",registers[rd]);
                }
                else if(strcmp(func3,"010")== 0 &&strcmp(opcode,"0010011")==0) 
                {
                    // printf("slti ");
                    registers[rd] = (registers[rs1] < imm)?1:0;
                }
                else if(strcmp(func3,"011")== 0 &&strcmp(opcode,"0010011")==0) 
                {
                    // printf("sltiu ");
                    registers[rd] = (registers[rs1] < (unsigned)imm)?1:0;
                }
                else if(strcmp(func3,"100")== 0 &&strcmp(opcode,"0010011")==0) 
                {
                    // printf("xori ");
                    registers[rd] = registers[rs1] ^ imm;
                }
                else if(strcmp(func3,"110")== 0 &&strcmp(opcode,"0010011")==0) 
                {
                    //printf("\n 이때 PC값: 0x%08d\n",PC);
                    // printf("ori ");
                    registers[rd] = registers[rs1] | imm;
                }
                else if(strcmp(func3,"111")== 0 &&strcmp(opcode,"0010011")==0) 
                {
                    // printf("andi ");
                    registers[rd] = registers[rs1] & imm;
                }
                    
                // printf("x%d x%d %d\n",rd,rs1,imm);	// ADDI ~ ANDI
            }	
			else if(strcmp(opcode,"0010011")==0 && (strcmp(func3,"001")== 0 || strcmp(func3,"101")== 0))
            {
                int shamt = unsignedbintodec(shamtstr);
                if(strcmp(func3,"001")== 0 &&strcmp(opcode,"0010011")==0) 
                {
                    // printf("slli ");
                    registers[rd] = registers[rs1] << shamt;
                }
                else if(strcmp(func3,"101")== 0 &&strcmp(opcode,"0010011")==0 && strcmp(func7,"0000000")==0) 
                {
                    // printf("srli ");
                    registers[rd] = (unsigned)registers[rs1] >> shamt;
                }
                else if(strcmp(func3,"101")== 0 &&strcmp(opcode,"0010011")==0 && strcmp(func7,"0100000")==0)
                {
                    // printf("srai ");
                    registers[rd] = registers[rs1] >> shamt;
                } 
                
                // printf("x%d x%d %d \n",rd,rs1,shamt);	// SLLI ~ SRAI
            }
   
				
        }
		// S-type Instructions
		else if (strcmp(opcode,"0100011")==0) {
                char immstr[13]="";
                strcat(immstr,imm11_5);
                strcat(immstr,imm4_0);
                immstr[12] = '\0';
                int imm = signedbintodec(immstr);
               
                if(strcmp(func3,"000")==0)
                {
                    //sb
                    if((registers[rs1]+imm >= 0x10000000) && (registers[rs1] + imm <= 0x1000ffff))
                    {
                        Data_Memory[registers[rs1]+imm-0x10000000] = (char)registers[rs2];

                    }
                    else if((registers[rs1]+imm == 0x20000000))
                    {

                        if(registers[rs2]<127)
                            printf("%c",(char)registers[rs2]);
                    }
                    else
                    {
                        break;
                    }
                }
                if(strcmp(func3,"001")==0)
                {
                    if(registers[rs1]+imm < 0x10000000 || registers[rs1]+imm > 0x1000fffe)
                    {
                        printf("Memory adderss error\n");
                        break;
                    }
                    char * temp = (char*)malloc(sizeof(char)*33);
                    char * temp2 = (char*)malloc(sizeof(char)*9);
                    temp = dectobin32(registers[rs2]);
                    int num=0;
                    int k = 1;
                    int j = 0;
                    for(int i = 16 ; i < 32; i++)
                    {
                        num++;
                        temp2[j] = temp[i];
                        if(num%8 == 0)
                        {
                            Data_Memory[registers[rs1]+imm-0x10000000+k]=(char)unsignedbintodec(temp2);
                            
                            k--;
                            j=0;
                        }
                        j++;
                        free(temp);
                        free(temp2);
                    }
                }
                if(strcmp(func3,"010")==0)
                {
                    if((registers[rs1]+imm == 0x20000000))
                    {

                        if(registers[rs2]<127)
                            printf("%c",(char)registers[rs2]);
                    }
                    else {
                        char * temp = (char*)malloc(sizeof(char)*33);
                        char * temp2 = (char*)malloc(sizeof(char)*9);
                        temp = dectobin32(registers[rs2]);
                        int num=0;
                        int k = 3;
                        int j = 0;
                        for(int i = 0 ; i < 32; i++)
                        {
                            num++;
                            temp2[j] = temp[i];
                            if(num%8 == 0)
                            {
                                Data_Memory[registers[rs1]+imm-0x10000000+k]=(char)unsignedbintodec(temp2);
                                k--;
                                j=0;
                            }
                            j++;
                        }
                        free(temp);
                        free(temp2);
                    }
                    
                }                        
		}
        // SB-type Instructions
        else if (strcmp(opcode,"1100011")==0) {
                char immstr[14] ="";
                strcat(immstr,imm12);
                strcat(immstr,imm11__24);
                strcat(immstr,imm10_5);
                strcat(immstr,imm4_1);
                immstr[12] = '0';
                immstr[13] = '\0';
                int imm = signedbintodec(immstr);
                if(strcmp(func3,"000")==0)
                {
                    if(registers[rs1]  == registers[rs2])
                        PC+=imm-4; // 맨마지막에 PC 추가해주니까!
                    
                }
                else if(strcmp(func3,"001")==0)
                {
                    
                    if(registers[rs1]  != registers[rs2])
                        PC+=imm-4;  // 맨마지막에 PC 추가해주니까!
                    
                    
                }
                else if(strcmp(func3,"100")==0)
                {
                    if(registers[rs1]  < registers[rs2])
                        PC+=imm-4;  // 맨마지막에 PC 추가해주니까!
                    
                }
                else if(strcmp(func3,"101")==0)
                {
                    if(registers[rs1]  >= registers[rs2])
                        PC+=imm-4;  // 맨마지막에 PC 추가해주니까!
                    
                }
                else if(strcmp(func3,"110")==0)
                {
                    if((unsigned)registers[rs1]  < (unsigned)registers[rs2])
                        PC+=imm-4; // 맨마지막에 PC 추가해주니까!
                    
                }
                else if(strcmp(func3,"111")==0)
                {
                    if((unsigned)registers[rs1] >= (unsigned)registers[rs2])
                        PC+=imm-4;  // 맨마지막에 PC 추가해주니까!
                    
                }
				
        } 
        
        // UJ-type Instrcutions
        else if(strcmp(opcode,"1101111")==0) {
            // 원래는 immstr 22 , 20에 널 
            char immstr[22] ="";
            strcat(immstr,imm20);
            strcat(immstr,imm19_12);
            strcat(immstr,imm11__11);
            strcat(immstr,imm10_1);
            immstr[20] = '0';
            immstr[21] = '\0';
            
            int imm = signedbintodec(immstr);
            registers[rd] = PC + 4;
            PC+=imm -4; // 맨마지막에 PC 추가해주니까!
        }
        // U-type Instructions
        else if(strcmp(opcode,"0110111")==0 || strcmp(opcode,"0010111")==0) {
			int imm = unsignedbintodec(imm31_12);
			if(strcmp(opcode,"0110111")==0)
            {
                // printf("lui x%d, %d\n",registers[rd],imm);
                registers[rd] = imm << 12;    
            }
				
			else if(strcmp(opcode,"0010111")==0)    // auipc는 고려아직안함.
            {
                // printf("auipc x%d, %d\n",registers[rd],imm);
                registers[rd] = PC + (imm << 12);   // 맨마지막에 PC 추가해주니까!

            }
				
        }
    
		
        if(rs1 == 0)
            registers[rs1] = 0;
        if(rs2 == 0)
            registers[rs2] = 0;
        if(rd == 0)
            registers[rd] = 0;

        
        
        PC+=4; 
    }
    PC+=4;

    // free allocated spaces
  
    free(hexstr);
    free(Inst_Memory);
    free(datastr);
    free(Byte_Memory);
    for(int PC=0;PC < 32; PC++)
    {
        printf("x%d: 0x%08x\n",PC,registers[PC]);
    }
    //printf("PC: 0x%08x \n",PC);

    return 0;

}
