using namespace std;

#include "sc_t-2.h"
#include <string.h>
#include <stdio.h>

//	==================================================================
//	methods to be implemented
//	------------------------------------------------------------------

void initHs() {
	// load the initial values for H's
	H[0] = 0x67452301;
	H[1] = 0xefcdab89;
	H[2] = 0x98badcfe;
	H[3] = 0x10325476;
	H[4] = 0xc3d2e1f0;
}

//	------------------------------------------------------------------

void initKs() {
	// load the initial values for K's
	int i;
	for (i = 0; i < 20; i++)   K[i] = 0x5a827999;
	for (i = 20; i < 40; i++)   K[i] = 0x6ed9eba1;
	for (i = 40; i < 60; i++)   K[i] = 0x8f1bbcdc;
	for (i = 60; i < 80; i++)   K[i] = 0xca62c1d6;
}

//	------------------------------------------------------------------

int getMessage(int k) {
	// copy message k from msg[k] to binmsg
	int msgLen = strlen(msg[k]);
	memcpy(binmsg, msg[k], msgLen);
	return msgLen;
}

//	------------------------------------------------------------------

int paddMessage(int messLen) {
	// pad the binmsg array according to the specification
	int paddingLen = 64 - ((messLen + 8) % 64); // Calculate padding length
	binmsg[messLen] = 0x80; // Add 1 bit after the message
	memset(binmsg + messLen + 1, 0, paddingLen - 1); // Fill remaining with zeros
	uint64_t bitLen = messLen * 8; // Message length in bits
	for (int i = 0; i < 8; i++) {
		binmsg[messLen + paddingLen - 8 + i] = (bitLen >> (56 - 8 * i)) & 0xFF; // Append message length in bits
	}
	return (messLen + paddingLen) / 64; // Return the number of blocks
}

//	------------------------------------------------------------------

void getWsfromM(int currentBlock) {
	// fill out the W[] from the current block
	for (int i = 0; i < 16; i++) {
		W[i] = (binmsg[currentBlock * 64 + i * 4] << 24) | (binmsg[currentBlock * 64 + i * 4 + 1] << 16) | (binmsg[currentBlock * 64 + i * 4 + 2] << 8) | binmsg[currentBlock * 64 + i * 4 + 3];
	}
	for (int i = 16; i < 80; i++) {
		W[i] = S(W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16], 1);
	}
}

//	------------------------------------------------------------------

void getAsfromHs() {
	// initialize A, B, C, D, E from H's
	A = H[0];
	B = H[1];
	C = H[2];
	D = H[3];
	E = H[4];
}

//	------------------------------------------------------------------

void displayDigest(uint32 H[]) {
	printf(" digest value - %9x %9x %9x %9x %9x\n\n",
		H[0], H[1], H[2], H[3], H[4]);
}

//	==================================================================

int main(int argc, char* argv[]) {
    int messLen;
    int numBlocks;

    FILE* inputFile = fopen("input.txt", "r");

    if (inputFile == NULL) {
        printf("Nu am putut deschide fisierul de intrare.\n");
        return 1;
    }
    fseek(inputFile, 0, SEEK_END);
    messLen = ftell(inputFile);
    rewind(inputFile);
    fread(binmsg, 1, messLen, inputFile);
    fclose(inputFile);

    initKs();
    initHs();
    numBlocks = paddMessage(messLen);

    for (int i = 0; i < numBlocks; i++) {
        getWsfromM(i);
        getAsfromHs();

        for (int j = 0; j < 80; j++) {
            TEMP = S(A, 5);
            int cit = j / 20;

            switch (cit) {
            case 0:
                TEMP += F0(B, C, D);
                break;
            case 1:
                TEMP += F1(B, C, D);
                break;
            case 2:
                TEMP += F2(B, C, D);
                break;
            case 3:
                TEMP += F3(B, C, D);
                break;
            }

            TEMP = TEMP + E + W[j] + K[j];

            E = D;
            D = C;
            C = S(B, 30);
            B = A;
            A = TEMP;
        }

        H[0] += A;
        H[1] += B;
        H[2] += C;
        H[3] += D;
        H[4] += E;
    }

    FILE* outputFile = fopen("output.txt", "w");
    if (outputFile == NULL) {
        printf("Nu am putut deschide fisierul de iesire.\n");
        return 1;
    }
    displayDigest(H);
    fclose(outputFile);

    return 0;
}