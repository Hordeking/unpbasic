#include <cstdint>
#include <cstring>
#include <string>
#include <map>
#include <sstream>
#include <iomanip>
#include <iostream>
	using namespace std;
#include "float_msft.h"


// Edit tokens in the tokens.h file.
std::map<int, std::string> tokens{
		#include "tokens.h"
	};


void protect(uint8_t * buffer, const uint8_t input[], const size_t size, const uint8_t Key1[], const uint8_t Key2[], const size_t nKey1, const size_t nKey2)
{

	// Make sure the user allocated a buffer.
	if(!buffer)
		throw buffer;

	// Start by copying the input into the buffer, that's our working space
	std::memcpy( buffer, input, size );

	size_t idxKey1 = nKey1;
	size_t idxKey2 = nKey2;

	//Unprotect the buffer
	for(size_t idxBuffer = 0;idxBuffer<size;++idxBuffer){

		//encoded = (decoded – idxKey1) ^ Key2[idxKey2 – 1] ^ Key1[idxKey2 – 1] + idxKey2
		buffer[idxBuffer] = ((buffer[idxBuffer] - idxKey1) ^ Key2[idxKey2-1] ^ Key1[idxKey1-1]) + idxKey2;

		// Decrement both key indices.
		idxKey1 = (1==idxKey1)?nKey1:idxKey1-1;
		idxKey2 = (1==idxKey2)?nKey2:idxKey2-1;
	}

	return;
}

void unprotect(uint8_t * buffer, const uint8_t input[], const size_t size, const uint8_t Key1[], const uint8_t Key2[], const size_t nKey1, const size_t nKey2)
{
	// Start by copying the input into the buffer, that's our working space
	memcpy( buffer, input, size );

	size_t idxKey1 = nKey1;
	size_t idxKey2 = nKey2;

	//Unprotect the buffer
	for(size_t idxBuffer = 0;idxBuffer<size;++idxBuffer){

		//Start with just one byte in the "inner loop".
		//decoded = (encoded – idxKey2) ^ Key1[idxKey1 – 1] ^ Key2[idxKey2 – 1] + idxKey1
		buffer[idxBuffer] = ((buffer[idxBuffer] - idxKey2) ^ Key1[idxKey1-1] ^ Key2[idxKey2-1]) + idxKey1;

		// Decrement Both Key Indices.
		idxKey1 = (1==idxKey1)?nKey1:idxKey1-1;
		idxKey2 = (1==idxKey2)?nKey2:idxKey2-1;
	}

	return;
}


// This should convert ascii plaintext to tokenized basic.
void ascii_to_token(uint8_t * buffer, const uint8_t input[]){

	return;
}

// This will convert tokenized basic back to human readable ascii.
string token_to_ascii(const uint8_t input[], size_t size){

	std::stringstream output;

	size_t idxInput = 0;

	// We don't actually care what the first two bytes of the line are as long as they aren't zero.
	// Zero is the exit condition.
	while(0x0000!=*(unsigned short *)(input+idxInput)){

		// We need special handling for 0x3a 8f d9 :REM' , 0xb1 e9 WHILE+ , 0x3a a1 :ELSE

		bool mode_Literal = false;	// Activate if we're inside of quote marks or behind a remark.
//		bool mode_Suppress = true;	// GWBasic suppresses output for :REM', WHILE+ and :ELSE
		bool mode_Comment = false;	// Anything after a comment mark is literal

		++++idxInput;	// Advance the cursor to the line number.

		output << dec << *(unsigned short *)(input+idxInput) << " "; ++idxInput;	//Output the line number.
		// We only increment the index once because it gets incremented again as soon as it gets into the while loop.

		// This is the line information itself.
		while(0!=input[++idxInput]){

			uint8_t * cur_ch = (uint8_t *) &input[idxInput];
			uint32_t token = 0;
//			bool mode_Number = false;	//Output line numbers
			output << dec;

			// This handles the situation with inside of quote marks.
			// It has to be handled in all cases, so we deal with it here.
			if ( 0x22 == *cur_ch ) mode_Literal = !mode_Literal;

			token = *cur_ch;	// Start by letting the token = the cursor.

			if ( !(mode_Literal | mode_Comment) ){				// We do tokens if we're not literal or comment.

				// Let's deal with some special cases

				if (*cur_ch > 0x00 && *cur_ch <= 0x0A) throw  *cur_ch;	// Other than 0x00, none of these should occur. If 0x00, it shouldn't get here.

				if (*cur_ch >= 0xfd || *cur_ch == 0xb1){	// This is a two byte token
					token = input[idxInput]<<8 | input[idxInput+1]<<0;
					++idxInput;
				}

				if (*cur_ch == 0x3a){	// This is a really special case requiring extra logic.
					if ( 0xa1 == *(cur_ch+1) ) { token = 0x3aa1; ++idxInput; }	// Is it :ELSE?
					if ( 0x8f == *(cur_ch+1) && 0xd9 == *(cur_ch+2) ) { token = 0x3a8fd9; mode_Comment = true; ++++idxInput; }	// oof, it's :REM'
				}

				if ( 0x8f == *cur_ch && !mode_Literal ) { mode_Comment = true; }	// It's REM. Toggle the comment mode

				//This is where we actually do our output.

				// Numerical output
				cout << setfill('0');	// Remove when done.
				if (0x0b == *cur_ch) { output << "&o" << oct << *(unsigned short *)(cur_ch+1); ++++idxInput; continue; }
				if (0x0c == *cur_ch) { output << "&h" << hex << *(unsigned short *)(cur_ch+1); ++++idxInput; continue; }
				if (0x0e == *cur_ch) { output << dec << *(unsigned short *)(cur_ch+1); ++++idxInput; continue; }	// Apparently this one should not be >65529.
				if (0x0f == *cur_ch) { output << dec << (unsigned int) *(unsigned uint8_t *)(cur_ch+1); ++idxInput; continue; } // Only used for 11-255
				if (0x11<=*cur_ch && *cur_ch<=0x1B) { output << dec << (unsigned int) (*cur_ch-0x11); continue; }	// Number consts 0 - 10
				if (0x1c == *cur_ch) { output << dec << *(signed short *)(cur_ch+1); ++++idxInput; continue; }	//This one is apparently signed and maxes at 0x7fff

				// TODO: This is our interface. float_msft({uint32_t|uint64_t})

				// These are floats. Let's just convert them to IEEE and output them.
				if (0x1d == *cur_ch) { output << float_msft(*(uint32_t *)(cur_ch+1)); idxInput+=4; continue; }
				if (0x1f == *cur_ch) { output << float_msft(*(uint64_t *)(cur_ch+1)); idxInput+=8; continue; }
				cout << setfill(' ');	// Remove when done;

				// Floating point algo.
				// Grab mantissa.
				// Prepend 0.1mmm mmmm mmmm mmmm mmmm mmmm
				// Exponent-0x80 is shift. msb is 2^(-1+exponent-0x80). For each bit, left shift, decrement the exponent by 1 until done.

				if (*cur_ch >= 0x20 && *cur_ch <=0x7e) { output << (char) token; continue; }	// These characters are their own tokens, essentially.
				output << tokens[token];	// Default output if nothing else got caught.

			}
			else {
				// We're literal. Just dump it to output and move on.
				output << (char) token;
			}

			// Character is handled.
		}
		++idxInput;		// idx was sitting on the EOL zero. Advance it to the next char, which should be the line offset.
		output << endl;
	}

	return output.str();
}

