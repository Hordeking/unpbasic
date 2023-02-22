#include <cstring>
#include <iostream>
	using namespace std;

#ifndef UNPBASIC_H
	#define UNPBASIC_H

//This is the default key set used in GWBASIC.	
const uint8_t Key1[] = {0x9a, 0xf7, 0x19, 0x83, 0x24, 0x63, 0x43, 0x83, 0x75, 0xcd, 0x8d, 0x84, 0xa9};
const uint8_t Key2[] = {0x7c, 0x88, 0x59, 0x74, 0xe0, 0x97, 0x26, 0x77, 0xc4, 0x1d, 0x1e};

const size_t nKey1 = sizeof(Key1);
const size_t nKey2 = sizeof(Key2);


void protect(uint8_t * buffer, const uint8_t input[], const size_t size, const uint8_t Key1[], const uint8_t Key2[], const size_t nKey1, const size_t nKey2);

void unprotect(uint8_t * buffer, const uint8_t input[], const size_t size, const uint8_t Key1[], const uint8_t Key2[], const size_t nKey1, const size_t nKey2);

void ascii_to_token(uint8_t * buffer, const uint8_t input[]);

string token_to_ascii(const uint8_t input[], size_t size);


#endif
