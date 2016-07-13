// util.c

#include "util.h"


// returns 2^x
int twoToThe(int x)
{
int output = 2;

for(x; x > 1; x--)
	output *= 2;
return output;
}

/************************************************
 * Decodes passed input into hex representation *
 * would a switch really be better?             *
 * **********************************************/
int decodeInput(char input)
{
	if(input == 'x'){
		return 0;
	}
	if(input == '1'){
		return 1;
	}
	if(input == '2'){
		return 2;
	}
	if(input == '3'){
		return 3;
	}
	if(input == '4'){
		return 0xc;
	}
	if(input == 'q'){
		return 4;
	}
	if(input == 'w'){
		return 5;
	}
	if(input == 'e'){
		return 6;
	}
	if(input == 'r'){
		return 0xd;
	}
	if(input == 'a'){
		return 7;
	}
	if(input == 's'){
		return 8;
	}
	if(input == 'd'){
		return 9;
	}
	if(input == 'f'){
		return 0xe;
	}
	if(input == 'z'){
		return 0xa;
	}
	if(input == 'c'){
		return 0xb;
	}
	if(input == 'v'){
		return 0xf;
	}
	return -1;
}

	
