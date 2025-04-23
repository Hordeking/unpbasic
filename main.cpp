//#include <algorithm>
//#include <iterator>
#include <cstdint>
#include <iomanip>
#include <string>
#include <fstream>
#include <iostream>
	using namespace std;
#include "float_msft.h"

#include <getopt.h>

#include "unpbasic.h"

void print_usage(void) {
	cerr << "Unprotect and detokenize GWBasic protected programs to console. Redirect output if you need it in a file." << endl << endl;
    cerr << "Usage: unpbasic [opts] <basfile>" << endl;
    cerr << endl;
    cerr << "	-d decrypt only - output raw tokenized basic." << endl;
//    cerr << "	-v	Verbose output" << endl;
//    cerr << "	-t	Validate only." << endl;
    cerr << "\n\nunpbasic Copyright (C) 2025  JD Fenech\nThis is free software, and you are welcome to redistribute it under certain conditions;\nThis program comes with ABSOLUTELY NO WARRANTY; for details see COPYING file." << endl;
    cerr << endl;
}

int main(int argc, char **argv){

	bool decryptOnly = false;

	// Just a test data, replace with source data buffer. This is a test case.
//	uint8_t enc[] = {0xfe, 0x9a, 0xa9, 0xbf, 0x54, 0xe2, 0x12, 0xbd, 0x59, 0x20, 0x65, 0x0d, 0x8f, 0xa2, 0x30, 0x98, 0xd3, 0x3e, 0xd3, 0xf2, 0x04, 0x13, 0xa4, 0x6c, 0xfe, 0x1a};

	// This is what the output buffer should be in the end. This is a test case.
/*	uint8_t dec[] = {
		0xff,
			  0x45, 0x12, 0x0a, 0x00,
		0x91, 0x20, 0x22, 0x48, 0x65,
		0x6c, 0x6c, 0x6f, 0x20, 0x57,
		0x6f, 0x72, 0x6c, 0x64, 0x21,
		0x22, ';',  0x0C, 0x34, 0x12,
		0x00,

		0x00, 0x10, 0x14, 0x00, 0x91,
		0x20, 0x0b, 0xff, 0x7e, ';',
		0x00, 0x00, 0x00, 0xff, 0x1a};
*/

	// Handle the command line options
	int option = 0;
	if (argc<2) { print_usage(); return -1;}
	while ((option = getopt(argc, argv,"d")) != -1) {
		switch (option) {
			/*
			case 'v' : // Verbose mode
				verbose = true;
				break;

			case 't' : // Validate object mode
				testonly = true;
				break;
			*/
			case 'd' : // Decrypt only
				decryptOnly = true;
				break;

			default: print_usage();
				exit(EXIT_FAILURE);
		}
	}

	uint8_t * buffer = new uint8_t[0xffff];	// This is the decryption buffer.

	string filename(argv[optind]);

	ifstream infile(filename, ifstream::binary);

	if (!infile.good()) {delete[] buffer; cerr << "Bad input file: " << filename << endl; return 0;}

	size_t nSize = 0;
	while (infile.good()){
		infile.read((char *) &buffer[nSize], 1);
		if ( infile.good() ) ++nSize;	// Make sure we actually read something before incrementing it.
	}

	infile.close();

	// We start at index 1 because the very first character is 0xfe.
	// We omit the last two characters because they're always 0xfe 0x1a.
	// Unprotect decrypts a raw stream. It's up to the driver to strip and add the begin and end sentinels.

	// The main func is just a test driver. The real action happens in protect and unprotect.

	//protect(buffer+1, buffer+1, nSize-2, Key1, Key2, nKey1, nKey2);

	// Unprotect the file if it's protected
	if (0xfe==buffer[0]){
		unprotect(buffer+1, buffer+1, nSize-2, Key1, Key2, nKey1, nKey2);
		buffer[0] = 0xff;	// Set the magic number to tokenized but decrypted.
	}

	if (decryptOnly)
		// Spit out the tokenized output on the console.
		for(size_t i=0 ; i<=nSize ; ++i)
			cout << buffer[i];
	else
		// Spit out the detokenized output on the console.
		if (0xff==buffer[0]) cout << token_to_ascii(buffer+1, nSize);
		
	delete[] buffer; buffer = NULL;

	return 0;
}
