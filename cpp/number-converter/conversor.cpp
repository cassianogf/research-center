//Conversor - binary - octal - decimal - hexadecimal
//Note: Requires Windows to execute

#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <math.h>
#include <tchar.h>
#include <strsafe.h>
#define MAX_THREADS 4
#define BUF_SIZE 255


typedef struct calculadora 
{
	char decimal[11];
	char octal[11];
	char binario[34];
	char hexadecimal[11];
	char binarioAS1[40];
	char binarioAS2[40];
	char resultadoA[80];
	char resultadoB[80];
} Calculadora;

typedef struct MyData
{
	char res[40];
	int val;
	int opt;
	int n;
	Calculadora *c1;
} *PMYDATA, MYDATA;

void iniciarVariaveis(Calculadora *calc);
Calculadora criarThread(Calculadora calc, int opt, int integer);


////////////////////////////////////AREA DE CONVERSÃO//////////////////////////////
// "PESO" nesse contexto significa x.10^n
int obterPeso(int num)
{
	int peso=0;
	while (num > 9)
	{
		num /= 10;
		peso++;
	}
	return peso;
}


// Conversão Decimal
char* conversaoDecimalBinario(int decimal_i, char *res)
{
	itoa(decimal_i,res,2);
	return res;
}

char* conversaoDecimalOctal(int decimal_i, char *res)
{
	char temp_octal[13], octal_s[13];
	int result=0, i=0, lenght=0;

	if(decimal_i < 8)
	{
		octal_s[0] = (char)decimal_i+'0';
		octal_s[1] = '\0';
		strcpy(res, octal_s);
		return res;
	}

	do
	{
		result = decimal_i % 8;
		decimal_i /= 8;
		temp_octal[i] = (char)result + '0';
		temp_octal[i+1] = '\0';
		i++;
	} while(decimal_i/8 > 8 || decimal_i > 7);

	temp_octal[i] = (char)decimal_i + '0';
	temp_octal[i+1] = '\0';
	lenght = strlen(temp_octal);

	for(int i=0; i<lenght; i++)
	{
		octal_s[i] = temp_octal[lenght-i-1];
		octal_s[i+1] = '\0';
	} 
	strcpy(res, octal_s);
	return res;
}


char* conversaoDecimalHexadecimal(int decimal_i, char *res)
{
	char buffer[33], decimal_s[10];

	strcpy(decimal_s, itoa(decimal_i,buffer,16));
	strupr(decimal_s);
	strcpy(res,decimal_s);

	return res;
}
Calculadora conversaoDecimal(Calculadora calc)
{
	int decimal_integer=0, lenght=strlen(calc.decimal);

	for(int i=1; i<lenght+1; i++)
	{
		decimal_integer += (((calc.decimal[i-1] - '0')) * pow((double)10, lenght-i));
	}
	

	calc = criarThread(calc, 0, decimal_integer);

	return calc;
}


//Conversão Octal
char* conversaoOctalDecimal(int octal_i, char *res)
{
	int atual=1, temp=0, peso = obterPeso(octal_i);
	char buffer[33];

	atual = octal_i;
	octal_i=0;
	do
	{
		temp = atual;
		temp /= pow((float)10,peso);
		octal_i += temp*(pow((float)8, peso));
		atual -= (temp*pow((float)10,peso));
		peso--;	
	} 
	while(atual > 0);

	strcpy(res, itoa(octal_i, buffer, 10));
	return res;
}

char* conversaoOctalBinario(int octal_i, char *res)
{
	int atual=1, temp=0, peso = obterPeso(octal_i);
	char buffer[33];

	atual = octal_i;
	octal_i=0;
	do
	{
		temp = atual;
		temp /= pow((float)10,peso);
		
		octal_i += temp*(pow((float)8, peso));
		atual -= (temp*pow((float)10,peso));
		peso--;
	} 
	while(atual > 0);

	strcpy(res, itoa(octal_i, buffer, 2));
	return res;
}

char* conversaoOctalHexadecimal(int octal_i, char*hexadecimal)
{
	int atual=1, temp=0, peso = obterPeso(octal_i);
	char buffer[33], octal_s[8];

	atual = octal_i;
	octal_i=0;
	do
	{
		temp = atual;
		temp /= pow((float)10,peso);
		
		octal_i += temp*(pow((float)8, peso));
		atual -= temp*pow((float)10,peso);
		peso--;
	} 
	while(atual > 0);
	
	strcpy(octal_s, itoa(octal_i, buffer, 16));
	strupr(octal_s);
	strcpy(hexadecimal, octal_s);
	
	return hexadecimal;
}

Calculadora conversaoOctal(Calculadora calc)
{
	int octal_integer=0, lenght=strlen(calc.octal);

	for(int i=1; i<lenght+1; i++)
	{
		octal_integer += (((calc.octal[i-1] - '0')) * pow((double)10, lenght-i));
	}
	
	calc = criarThread(calc, 1, octal_integer);

	return calc;
}

//Conversão Binário
char* conversaoBinarioDecimal(char *binario, char *res)
{
	char buffer[33];
	int alga=0, nDec = 0, tam = 0,  i=0;
	double pos = 0;

	tam = strlen (binario);
	for (i = tam - 1; i >= 0; i--){
		if (binario[i] == '1'){
			alga = binario[i] - '0';
			nDec += (pow(2, pos));
		}
		pos++;
	}

	strcpy(res, itoa(nDec, buffer, 10));
	return res;
}

char* conversaoBinarioOctal(char *binario, char *octal)
{
	//Transformação Binário -> Decimal
	char buffer[33];
	int alga=0, decimal_i=0, tam=0, i=0;
	double pos = 0;

	tam = strlen (binario);
	for (i = tam - 1; i >= 0; i--){
		if (binario[i] == '1'){
			alga = binario[i] - '0';
			decimal_i += (pow(2, pos));
		}
		pos++;
	}
	i=0;

	//Transformação Decimal -> Octal
	char temp_octal[12];
	int res=0, lenght=0;

	if(decimal_i < 8)
	{
		octal[0] = (char)decimal_i+'0';
		octal[1] = '\0';
		return octal;
	}

	do
	{
		res = decimal_i % 8;
		decimal_i /= 8;
		temp_octal[i] = (char)res + '0';
		temp_octal[i+1] = '\0';
		i++;
	} while(decimal_i/8 > 8 || decimal_i > 7);

	temp_octal[i] = (char)decimal_i + '0';
	temp_octal[i+1] = '\0';
	lenght = strlen(temp_octal);

	for(int i=0; i<lenght; i++)
	{
		octal[i] = temp_octal[lenght-i-1];
		octal[i+1] = '\0';
	} 
	return octal;
}
char* conversaoBinarioHexadecimal(char *binario, char *res)
{
	char buffer[33], binario_s[34];
	int alga=0, nDec = 0, tam = 0,  i=0;
	double pos = 0;

	tam = strlen (binario);
	for (i = tam - 1; i >= 0; i--){
		if (binario[i] == '1'){
			alga = binario[i] - '0';
			nDec += (pow(2, pos));
		}
		pos++;
	}
	strcpy(binario_s, itoa(nDec, buffer, 16));
	strupr(binario_s);
	strcpy(res, binario_s);
	return res;
}

Calculadora conversaoBinario(Calculadora calc)
{
	calc = criarThread(calc, 2, 0);
	return calc;
}


// Coversão Hexadecimal
char* conversaoHexadecimalDecimal(char *hexadecimal, char *decimal)
{
	int lenght = strlen(hexadecimal), res=0;
	char buffer[33];

	for(int i=0; i<lenght;i++)
	{
		if(hexadecimal[i] >= '0' && hexadecimal[i] <= '9')
		{
			res+= (hexadecimal[i]-'0')*(pow((double)16, lenght-i-1));
		}
		else
		{
			res+= (hexadecimal[i]-55)*(pow((double)16, lenght-i-1));
		}
	}
	
	strcpy(decimal, itoa(res,buffer,10)); 
	return decimal;
}

char* conversaoHexadecimalBinario(char *hexadecimal, char *binario)
{
	int lenght=strlen(hexadecimal), temp_decimal=0, res=0, j=0, lenght_string=0;
	char temp_char='0', string_final[30], buffer[33], string_temp[5];

	for(int i=0; i<lenght;i++)
	{
		//Separação
		temp_char = hexadecimal[i];
	
		//Conversão Char->Inteiro
		if(temp_char >= '0' && temp_char <= '9')
		{
			temp_decimal = (temp_char-'0');
		}
		else
		{
			temp_decimal = (temp_char-55);
		}

		//Conversão para binário
		if(temp_decimal > 0)
		{
			do
			{
				if(temp_decimal > 1)
				{
					res += (temp_decimal % 2) * pow((double)10, j);
					temp_decimal /= 2;
				}
				else
				{
					res+= 1 * pow((double)10, j);
					temp_decimal--;
				}
				j++;
			} while(temp_decimal != 0);
		}
		else
		{
			res = 0;
		}

		j=0;

		//Binário -> String (zeros a esquerda também)
		if(i==0)
		{
			strcpy(string_final, itoa(res, buffer, 10)); 
		}
		else
		{
			strcpy(string_temp, itoa(res, buffer, 10));
			
			lenght_string = strlen(string_temp);

			while(lenght_string != 4)
			{
				for(int x=(lenght_string-1); x > -1; x--)
				{
					string_temp[x+1] = string_temp[x];
					string_temp[x] = '0';
				}
				
				string_temp[lenght_string+1] = '\0';

				lenght_string++;
			}

			strcat(string_final,string_temp);
		}

		res=0;
	}

	strcpy(binario,string_final);
	return binario;
}

char* conversaoHexadecimalOctal(char *hexadecimal, char *octal)
{
	int lenght = strlen(hexadecimal), res=0;
	char buffer[33];

	for(int i=0; i<lenght;i++)
	{
		if(hexadecimal[i] >= '0' && hexadecimal[i] <= '9')
		{
			res+= (hexadecimal[i]-'0')*(pow((double)16, lenght-i-1));
		}
		else
		{
			res+= (hexadecimal[i]-55)*(pow((double)16, lenght-i-1));
		}
	}

	strcpy(octal, itoa(res, buffer, 8));
	return octal;
}

Calculadora conversaoHexadecimal(Calculadora calc)
{
	calc = criarThread(calc, 3, 0);

	return calc;
}





/////////////////////////// FUNÇÕES DA THREAD SOMENTE /////////////////////////////

DWORD WINAPI conversaoThread( LPVOID lpParam) 
{ 
    HANDLE hStdout;
    PMYDATA pDataArray;

    TCHAR msgBuf[BUF_SIZE];
    size_t cchStringSize;
    DWORD dwChars;

    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    if( hStdout == INVALID_HANDLE_VALUE )
        return 1;
	
	

    pDataArray = (PMYDATA)lpParam;
	
	switch(pDataArray->opt)
	{
	case 0:
		strcpy(pDataArray->c1->octal, conversaoDecimalOctal(pDataArray->val, pDataArray->c1->octal));
		strcpy(pDataArray->c1->binario, conversaoDecimalBinario(pDataArray->val, pDataArray->c1->binario));
		strcpy(pDataArray->c1->hexadecimal, conversaoDecimalHexadecimal(pDataArray->val, pDataArray->c1->hexadecimal));
		break;
	case 1:
		strcpy(pDataArray->c1->decimal, conversaoOctalDecimal(pDataArray->val, pDataArray->c1->decimal));
		strcpy(pDataArray->c1->binario, conversaoOctalBinario(pDataArray->val, pDataArray->c1->binario));
		strcpy(pDataArray->c1->hexadecimal, conversaoOctalHexadecimal(pDataArray->val, pDataArray->c1->hexadecimal));
		break;
	case 2:
		strcpy(pDataArray->c1->decimal, conversaoBinarioDecimal(pDataArray->c1->binario, pDataArray->c1->decimal));
		strcpy(pDataArray->c1->octal, conversaoBinarioOctal(pDataArray->c1->binario, pDataArray->c1->octal));
		strcpy(pDataArray->c1->hexadecimal, conversaoBinarioHexadecimal(pDataArray->c1->binario, pDataArray->c1->hexadecimal));
		break;
	case 3:
		strcpy(pDataArray->c1->decimal, conversaoHexadecimalDecimal(pDataArray->c1->hexadecimal, pDataArray->c1->decimal));
		strcpy(pDataArray->c1->binario, conversaoHexadecimalBinario(pDataArray->c1->hexadecimal, pDataArray->c1->binario));
		strcpy(pDataArray->c1->octal, conversaoHexadecimalOctal(pDataArray->c1->hexadecimal, pDataArray->c1->octal));
		break;
	}


    StringCchLength(msgBuf, BUF_SIZE, &cchStringSize);
    WriteConsole(hStdout, msgBuf, (DWORD)cchStringSize, &dwChars, NULL);

    return 0; 
} 



void ErrorHandler(LPTSTR lpszFunction) 
{ 
    // Retrieve the system error message for the last-error code.

    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError(); 

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );

    // Display the error message.

    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, 
        (lstrlen((LPCTSTR) lpMsgBuf) + lstrlen((LPCTSTR) lpszFunction) + 40) * sizeof(TCHAR)); 
    StringCchPrintf((LPTSTR)lpDisplayBuf, 
        LocalSize(lpDisplayBuf) / sizeof(TCHAR),
        TEXT("%s failed with error %d: %s"), 
        lpszFunction, dw, lpMsgBuf); 
    MessageBox(NULL, (LPCTSTR) lpDisplayBuf, TEXT("Error"), MB_OK); 

    // Free error-handling buffer allocations.

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
}

Calculadora criarThread(Calculadora calc, int opt, int integer)
{
    PMYDATA pDataArray[MAX_THREADS];
    DWORD   dwThreadIdArray[MAX_THREADS];
    HANDLE  hThreadArray[MAX_THREADS]; 

	for( int i=0; i<MAX_THREADS; i++ )
	{
        pDataArray[i] = (PMYDATA) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
                sizeof(MYDATA));

        if( pDataArray[i] == NULL )
        {
            ExitProcess(2);
        }

		pDataArray[i]->opt = opt;
		pDataArray[i]->n = i;
		pDataArray[i]->c1 = &calc;
		pDataArray[i]->val = integer;


        hThreadArray[i] = CreateThread( 
            NULL,                   // default security attributes
            0,                      // use default stack size  
            conversaoThread,       // thread function name
            pDataArray[i],          // argument to thread function 
            0,                      // use default creation flags 
            &dwThreadIdArray[i]);   // returns the thread identifier 

        if (hThreadArray[i] == NULL) 
        {
           ErrorHandler(TEXT("CreateThread"));
           ExitProcess(3);
        }
    }

    WaitForMultipleObjects(MAX_THREADS, hThreadArray, TRUE, INFINITE);

    for(int i=0; i<MAX_THREADS; i++)
    {
        CloseHandle(hThreadArray[i]);
        if(pDataArray[i] != NULL)
        {
            HeapFree(GetProcessHeap(), 0, pDataArray[i]);
            pDataArray[i] = NULL;    // Ensure address is not reused.
        }
    }
	return calc;
}

///////////////////////////// FIM FUNÇÕES DA THREAD ///////////////////////////////

void iniciarConversao(int opt, Calculadora *calc)
{

	switch(opt)
	{
	//Conversão de Decimal para os demais
	case 0:
		(*calc) = conversaoDecimal(*calc);
		break;

	//Conversão de Octal para os demais
	case 1:
		(*calc) = conversaoOctal(*calc);
		break;

	//Conversão de Binário para os demais
	case 2:
		(*calc) = conversaoBinario(*calc);
		break;

	//Conversão de Hexadecimal para os demais
	case 3:
		(*calc) = conversaoHexadecimal(*calc);
		break;
	}


}

////////////////////////////////ÁREA DE CONVERSÃO FIM////////////////////////////

///////////////////////////////ÁREA ADIÇÃO SUBTRAÇÃO///////////////////////////////

int conversaoBinarioDecInt(char *a)
{
	int alga=0, tam = 0, i=0, decimal=0;
	double pos = 0;

	tam = strlen (a);
	for (i = tam - 1; i >= 0; i--){
		if (a[i] == '1'){
			alga = a[i] - '0';
			decimal += (pow(2, pos));
		}
		pos++;
	}

	return decimal;
}

char *adicao(char *a, char *b)
{
	char res[80];
	int decimalA=0, decimalB=0, res_i=0;

	decimalA = conversaoBinarioDecInt(a);
	decimalB = conversaoBinarioDecInt(b);

	res_i = decimalA + decimalB;

	itoa(res_i, res, 2);

	return res;
}

char *subtracao(char *a, char *b)
{
	char res[80];
	int decimalA=0, decimalB=0, res_i=0;

	decimalA = conversaoBinarioDecInt(a);
	decimalB = conversaoBinarioDecInt(b);

	res_i = decimalA - decimalB;
	
	if(res_i>=0)
		itoa(res_i, res, 2);
	else
		itoa(0, res, 2);

	return res;
}

void iniciarBinarioAS(Calculadora *calc)
{
	strcpy(calc->resultadoA, adicao(calc->binarioAS1, calc->binarioAS2));
	strcpy(calc->resultadoB, subtracao(calc->binarioAS1, calc->binarioAS2));
}

/////////////////////////////////FIM ADIÇÃO SUBTRAÇÃO//////////////////////////////



//FUNÇÃO PARA MUDAR A COR DOS CARACTERES

void textcolor (int color)
{
  SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), color );
}


void moverCima(int *opt, int *lado)
{
	if(*opt != 0)
	{
		if(*opt < 4 && *lado == 1)
		{
			*opt -= 1;
			return;
		}

		if(*opt > 4 && *lado == 2)
		{
			*opt -= 1;
			return;
		}
	}
}
void moverBaixo(int *opt, int *lado)
{
	if(*opt != 5)
	{
		if(*opt < 3 && *lado == 1)
		{
			*opt += 1;
			return;
		}

		if(*opt > 3 && *lado == 2)
		{
			*opt += 1;
			return;
		}
	}
}

void moverEsquerda(int *opt, int*lado)
{
	if(*lado == 2)
	{
		*lado = 1;
		*opt = 0;
	}
}

void moverDireita(int *opt, int*lado)
{
	if(*lado == 1)
	{
		*lado = 2;
		*opt = 4;
	}
}
int backspace(int opt, Calculadora *calc)
{
	int i=0;
	switch (opt)
	{
		case 0:
			i=strlen(calc->decimal);
			if(i > 0)
			{
				i--;
				calc->decimal[i] = '\0';
				if(i != 0)
					return 1;
				else
					return 0;
			}
			break;
		case 1:
			i=strlen(calc->octal);
			if(i > 0)
			{
				i--;
				calc->octal[i] = '\0';
				if(i != 0)
					return 1;
				else
					return 0;
			}
			break;
		case 2:
			i=strlen(calc->binario);
			if(i > 0)
			{
				i--;
				calc->binario[i] = '\0';
				if(i != 0)
					return 1;
				else
					return 0;
			}
			break;
		case 3:
			i=strlen(calc->hexadecimal);
			if(i > 0)
			{
				i--;
				calc->hexadecimal[i] = '\0';
				if(i != 0)
					return 1;
				else
					return 0;
			}
			break;
		case 4:
			i=strlen(calc->binarioAS1);
			if(i > 0)
			{
				i--;
				calc->binarioAS1[i] = '\0';
				if(i != 0)
					return 2;
				else
				{
					calc->binarioAS1[0] = '0';
					calc->binarioAS1[1] = '\0';
					return 0;
				}
			}
			break;
		case 5:
			i=strlen(calc->binarioAS2);
			if(i > 0)
			{
				i--;
				calc->binarioAS2[i] = '\0';
				if(i != 0)
					return 2;
				else
				{	
					calc->binarioAS2[0] = '0';
					calc->binarioAS2[1] = '\0';
					return 0;
				}
			}
			break;
	}
}

void atualizaTela(int erro, int opt, Calculadora calc)
{
	system("cls");
	textcolor(10);
	printf("\n           Trabalho de Organizacao e Arquitetura de Computadores\n");
	printf("\n                  Dupla: Carla Gabriela e Cassiano Guerra");
	textcolor(7);
	printf("\n\n  ");

	if(opt<4)
	{
		textcolor(12);
		printf("========== CONVERSOES ==========      ");
		textcolor(7);
		printf("=========== AD. SUB. BINARIOS =========\n\n");
	}
	else
	{
		printf("========== CONVERSOES ==========      ");
		textcolor(12);
		printf("=========== AD. SUB. BINARIOS =========\n\n");
		textcolor(7);
	}

	//Decimal
	if(opt == 0)
	{
		textcolor(11);
		printf("  Decimal: %s\n", calc.decimal);
		textcolor(7);
		printf("                                            Valor 1: %s\n", calc.binarioAS1);
	}
	else
	{		
		printf("  Decimal: %s\n", calc.decimal);

		if(opt == 4)
		{
			textcolor(11);
			printf("                                            Valor 1: %s\n", calc.binarioAS1);
			textcolor(7);
		}
		else
			printf("                                            Valor 1: %s\n", calc.binarioAS1);


	}


	//Octal
	if(opt == 1)
	{
		textcolor(11);
		printf("  Octal: %s\n", calc.octal);
		textcolor(7);
		printf("                                            Valor 2: %s\n", calc.binarioAS1);
	}
	else
	{
		printf("  Octal: %s\n", calc.octal);
		
		if(opt == 5)
		{
			textcolor(11);
			printf("                                            Valor 2: %s\n", calc.binarioAS2);
			textcolor(7);
		}
		else
			printf("                                            Valor 2: %s\n", calc.binarioAS2);
	}
	//Binário
	if(opt == 2)
	{
		textcolor(11);
		printf("  Binario: %s\n", calc.binario);
		textcolor(7);
	}
	else
		printf("  Binario: %s\n", calc.binario);

	printf("                                            Resultado Adicao: %s\n", calc.resultadoA);

	//Hexadecimal
	if(opt == 3)
	{
		textcolor(11);
		printf("  Hexadecimal: %s\n", calc.hexadecimal);
		textcolor(7);
	}
	else
		printf("  Hexadecimal: %s\n", calc.hexadecimal);

	printf("                                            Resultado Subtracao: %s\n", calc.resultadoB);
	
	

	printf("\n\n\n");
	
	switch(erro)
	{
	case 1:
		printf("Maximum number values reached.\n");
		break;
	case 2:
		printf("Digite um valor correto!\n\n");
		break;
	}
}	

void keyDecimal(char key, int* erro, Calculadora *calc)
{
	int i=strlen(calc->decimal);

	if(key > 47 && key < 58)
	{
		if(i != 9)
		{
			if(calc->decimal[0] == '0' && i == 1)
			{
				i--;
				calc->decimal[i] = key;
			}
			else
			{
				calc->decimal[i] = key;
				calc->decimal[i+1] = '\0';
				i++;
			}
		}
		else
			*erro = 1;
	}
	else
		*erro = 2;
}

void keyOctal(char key, int* erro, Calculadora *calc)
{
	int i=strlen(calc->octal);
	
	if(key > 47 && key < 56)
	{
		if(i != 9)
		{	
			if(calc->octal[0] == '0' && i == 1)
			{
				i--;
				calc->octal[i] = key;
			}
			else
			{
				calc->octal[i] = key;
				calc->octal[i+1] = '\0';
				i++;
			}
		}
		else
			*erro = 1;
	}
	else
		*erro = 2;
}

void keyBinario(char key, int* erro, Calculadora *calc)
{
	int i=strlen(calc->binario);

	if(key == '0' || key == '1')
	{
		if(i != 29)
		{
			if(calc->binario[0] == '0' && i == 1)
			{
				i--;
				calc->binario[i] = key;
			}
			else
			{
				calc->binario[i] = key;
				calc->binario[i+1] = '\0';
				i++;
			}
		}
		else
			*erro = 1;
	}
	else
		*erro = 2;
}

void keyHexadecimal(char key, int* erro, Calculadora *calc)
{
	int i=strlen(calc->hexadecimal);

	if((key > 47 && key < 58) || (key > 96 && key < 103) || (key > 64 && key < 71))
	{
		if(key > 96 && key < 123)
		{
			key -= 32;
		}

		if(i != 7)
		{
			if(calc->hexadecimal[0] == '0' && i == 1)
			{
				i--;
				calc->hexadecimal[i] = key;
			}
			else
			{
				calc->hexadecimal[i] = key;
				calc->hexadecimal[i+1] = '\0';
				i++;
			}
		}
		else
			*erro = 1;
	}
	else
		*erro = 2;
}


void keyBinarioAS(char key, int* erro, Calculadora *calc, int n)
{
	int i=0;
	if(n == 1)
		i=strlen(calc->binarioAS1);
	else
		i=strlen(calc->binarioAS2);

	if(key == '0' || key == '1')
	{
		if(i != 20)
		{
			if(n == 1)
			{
				if(calc->binarioAS1[0] == '0' && i == 1)
				{
					i--;
					calc->binarioAS1[i] = key;
				}
				else
				{
					calc->binarioAS1[i] = key;
					calc->binarioAS1[i+1] = '\0';
					i++;
				}
			}
			else
			{
				if(calc->binarioAS2[0] == '0' && i == 1)
				{
					i--;
					calc->binarioAS2[i] = key;
				}
				else
				{
					calc->binarioAS2[i] = key;
					calc->binarioAS2[i+1] = '\0';
					i++;
				}
			}
		}
		else
			*erro = 1;
	}
	else
		*erro = 2;
}

void keyPad()
{
	int erro=0, lado=1, opt=0;
	char key=0;
	Calculadora calc;

	iniciarVariaveis(&calc);
	atualizaTela(erro, opt, calc);

	while(key != 27)
	{
		erro=0;
		key = getch();

		if(key == 72)
		{
			moverCima(&opt, &lado);
			if(opt < 4)
				iniciarVariaveis(&calc);
		}
		else if(key == 80)
		{
			moverBaixo(&opt, &lado);
			if(opt < 5)	
				iniciarVariaveis(&calc);
		}
		else if(key == 75)
		{
			moverEsquerda(&opt, &lado);
		}
		else if(key == 77)
		{
			moverDireita(&opt, &lado);
		}
		else if(key == 8)
		{
			if(backspace(opt, &calc) == 1)
				iniciarConversao(opt, &calc);
			else if(opt < 4)
				iniciarVariaveis(&calc);
			else
				iniciarBinarioAS(&calc);
		}
		else
		{
			switch(opt)
			{
			case 0:
				keyDecimal(key, &erro, &calc);
				break;
			case 1:
				keyOctal(key, &erro, &calc);
				break;
			case 2:
				keyBinario(key, &erro, &calc);
				break;
			case 3:
				keyHexadecimal(key, &erro, &calc);
				break;
			case 4:
				keyBinarioAS(key, &erro, &calc, 1);
				break;
			case 5:
				keyBinarioAS(key, &erro, &calc, 2);
				break;
			}

			if(opt < 4)
				iniciarConversao(opt, &calc);
			else
				iniciarBinarioAS(&calc);
		}
		atualizaTela(erro, opt, calc);
	}
}

void iniciarVariaveis(Calculadora *calc)
{
	calc->decimal[0] = '0';
	calc->decimal[1] = '\0';
	strcpy(calc->octal, calc->decimal);
	strcpy(calc->binario, calc->decimal);
	strcpy(calc->hexadecimal, calc->decimal);
	strcpy(calc->binarioAS1, calc->decimal);
	strcpy(calc->binarioAS2, calc->decimal);
	strcpy(calc->resultadoA, calc->decimal);
	strcpy(calc->resultadoB, calc->decimal);
}

int reiniciarPrograma()
{
	keyPad();
	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	while(reiniciarPrograma())
	{
		
	}
	return 0;
}

