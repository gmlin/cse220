/*
Homework #7
name: Guanming Lin
sbuid: 109299260
*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include "hw7.h"

#define ENCRYPT 0
#define DECRYPT 1
#ifdef CSE220
	#define cse220(fmt, ...)\
	do {\
		printf("CSE220: " fmt, ##__VA_ARGS__);\
	} while(0);
	#define tabula_recta(tab, ...)\
	do {\
		printf("CSE220: Tabula Recta\n\n");\
		int i;\
		int j;\
		for (i = 0; i < ALPHABET_SIZE; i++) {\
			for (j = 0; j < ALPHABET_SIZE; j++) {\
				printf("%c ", *(*(tabula + i) + j));\
			}\
			printf("\n");\
		}\
	} while(0);
#else
	#define cse220(fmt, ...)
	#define tabula_recta(tab, ...)
#endif


void print_help(char **argv);
int substitution(char **argv, int mode);
int autokey(char **argv, int mode);
void setup_alphabet(char *alp, int shift);
void setup_tabula(int shift);
void apply_substitution(char *str);
void apply_autokey(char *str, int mode);
char shift(char c, int n);

void print_help (char **argv) {
	printf("Usage: %s [-s | -a | -h]\n", *argv);
	printf("    -s        Substitution cipher\n");
	printf("              Additional parameters: [-e | -d] n INPUT_FILE OUTPUT_FILE\n");
	printf("                  -e          Encrypt using the substitution cipher.\n");
	printf("                  -d          Decrypt using the substitution cipher.\n");
	printf("                   n          The amount of position to shift by.\n");
	printf("                   INPUT_FILE  This can be any file on the file system or -\n");
	printf("                               which specifies stdin.\n");
	printf("                   OUTPUT_FILE This can be any file on the file system or -\n");
	printf("                               which specifies stdin.\n");
	printf("    -a        Autokey cipher\n");
	printf("              Additional parameters: [-e | -d] n INPUT_FILE KEY_FILE OUTPUT_FILE\n");
	printf("                  -e          Encrypt using the autokey cipher.\n");
	printf("                  -d          Decrypt using the autokey cipher.\n");
	printf("                   n          The initial shift value\n");
	printf("                   INPUT_FILE  This can be any file on the file system or -\n");
	printf("                               which specifies stdin.\n");
	printf("                   KEY_FILE    This can be any file on the file system or -\n");
	printf("                               which specifies stdin.\n");
	printf("                   OUTPUT_FILE This can be any file on the file system or -\n");
	printf("                               which specifies stdin.\n");
	printf("    -h        Display this help menu.\n");
}

int substitution(char **argv, int mode) {
	int shift;
	char *input;
	char *output;
	FILE *inf;
	FILE *outf;
	if (sscanf(*(argv + optind), "%d", &shift) != 1) {
		return EXIT_FAILURE;
	}
	cse220("shift amount: %d\n", shift);
	input = *(argv + optind + 1);
	output = *(argv + optind + 2);
	if (strcmp(input, "-") == 0) {
		inf = stdin;
		cse220("input file: STD_IN\n");
	}
	else {
		inf = fopen(input, "r");
		if (!inf) {
			return EXIT_FAILURE;
		}
		cse220("input file: %s\n", input);
	}
	if (strcmp(output, "-") == 0) {
		outf = stdout;
		cse220("output file: STD_OUT\n");
	}
	else {
		outf = fopen(output, "w");
		if (!outf) {
			fclose(inf);
			return EXIT_FAILURE;
		}
		cse220("output file: %s\n", output);
	}
	cse220("cipher type: substitution cipher\n");
	if (mode == DECRYPT) {
		cse220("cipher operation: decryption\n");
		shift = 0 - shift;
	}
	else if (mode == ENCRYPT) {
		cse220("cipher operation: encryption\n");
	}
	setup_alphabet(alphabet, shift);
	while (fgets(plaintext, BUFFER_SIZE, inf)) {
		apply_substitution(plaintext);
		fprintf(outf, "%s", ciphertext);
	}
	fclose(inf);
	fclose(outf);
	return EXIT_SUCCESS;
}

int autokey(char **argv, int mode) {
	int shift;
	char *input;
	char *keyc;
	char *output;
	FILE *inf;
	FILE *keyf;
	FILE *outf;
	if (sscanf(*(argv + optind), "%d", &shift) != 1) {
		return EXIT_FAILURE;
	}
	cse220("initial shift amount: %d\n", shift);
	input = *(argv + optind + 1);
	keyc = *(argv + optind + 2);
	output = *(argv + optind + 3);
	if (strcmp(input, "-") == 0) {
		inf = stdin;
		cse220("input file: STD_IN\n");
	}
	else {
		inf = fopen(input, "r");
		if (!inf) {
			return EXIT_FAILURE;
		}
		cse220("input file: %s\n", input);
	}
	if (strcmp(keyc, "-") == 0) {
		keyf = stdin;
	}
	else {
		keyf = fopen(keyc, "r");
		if (!keyf) {
			fclose(inf);
			return EXIT_FAILURE;
		}
	}
	if (strcmp(output, "-") == 0) {
		outf = stdout;
		cse220("output file: STD_OUT\n");
	}
	else {
		outf = fopen(output, "w");
		if (!outf) {
			fclose(inf);
			fclose(keyf);
			return EXIT_FAILURE;
		}
		cse220("output file: %s\n", input);
	}
	cse220("cipher type: autokey cipher\n");
	if (mode == DECRYPT) {
		shift = 0 - shift;
		cse220("cipher operation: decryption\n");
	}
	else if (mode == ENCRYPT) {
		cse220("cipher operation: encryption\n");
	}
	setup_tabula(shift);
	tabula_recta(tabula);
	while (fgets(plaintext, BUFFER_SIZE, inf)) {
		if (!fgets(key, KEY_SIZE, keyf)) {
			return EXIT_FAILURE;
		}
		apply_autokey(plaintext, mode);
		fprintf(outf, "%s", ciphertext);
	}
	fclose(inf);
	fclose(keyf);
	fclose(outf);
	return EXIT_SUCCESS;
}

void setup_alphabet(char *alp, int shift) {
	int i;
	if (shift < 0) {
		shift = ALPHABET_SIZE - shift % ALPHABET_SIZE;
	}
	else {
		shift = shift % ALPHABET_SIZE;
	}
	for (i = 0; i < ALPHABET_SIZE; i++) {
		if ('A' + shift + i > 'Z') {
			*(alp + i) = 'A' + shift + i - 'Z' + 'A' - 1;
		}
		else {
			*(alp + i) = 'A' + shift + i;
		}
	}
}

void setup_tabula(int shift) {
	int i;
	if (shift < 0) {
		shift = ALPHABET_SIZE - shift % ALPHABET_SIZE;
	}
	else {
		shift = shift % ALPHABET_SIZE;
	}
	for (i = 0; i < ALPHABET_SIZE; i++) {
		setup_alphabet(*(tabula + i), shift + i);
	}
}

void apply_substitution(char *str) {
	int i;
	for (i = 0; i < strlen(str); i++) {
		if (*(str + i) >= 'A' && *(str + i) <= 'Z') {
			*(ciphertext + i) = *(alphabet + *(str + i) - 'A');
		}
		else if (*(str + i) >= 'a' && *(str + i) <= 'z') {
			*(ciphertext + i) = *(alphabet + *(str + i) - 'a');
		}
		else {
			*(ciphertext + i) = *(str + i);
		}
	}
	*(ciphertext + strlen(str)) = '\0';
}

void apply_autokey(char *str, int mode) {
	int i;
	int j;
	int row;
	int col;
	int l = 0;
	int padded = 0;
	if (mode == ENCRYPT) {
		for (j = 0; j < strlen(key); j++) {
			if (*(key + j) >= 'a' && *(key + j) <= 'z') {
				*(key + j) = *(key + j) - 'a' + 'A';
			}
			if (*(key + j) >= 'A' && *(key + j) <= 'Z') {
				*(buffer + l) = *(key + j);
				l++;
			}
		}
		*(buffer + l) = '\0';
		strcpy(key, buffer);
		for (i = 0; i < strlen(str); i++) {
			if (*(str + i) >= 'a' && *(str + i) <= 'z') {
				*(str + i) = *(str + i) - 'a' + 'A';
			}
			if (i >= strlen(key)) {
				while (!(*(str + i - strlen(key) + padded)  >= 'A' && *(str + i - strlen(key) + padded) <= 'Z')) {
					padded++;
				}
				*(key + i) = *(str + i - strlen(key) + padded);
				padded++;
				*(key + i + 1) = '\0';
			}
			for (j = 0; j < ALPHABET_SIZE; j++) {
				if (*(*tabula + j) == *(str + i)) {
					col = j;
				}
				if (*(*(tabula + j)) == *(key + i)) {
					row = j;
				} 
			}
			if (*(str + i) >= 'A' && *(str + i) <= 'Z') {
				*(ciphertext + i) = *(*(tabula + row) + col);
			}
			else {
				strcpy(buffer, key);
				strcpy(buffer + i, str + i);
				strcpy(buffer + i + 1, key + i);
				strcpy(key, buffer);
				*(ciphertext + i) = *(str + i);
			}
		}
	}
	else if (mode == DECRYPT) {
		for (i = 0; i < strlen(str); i++) {
			if (*(str + i) >= 'a' && *(str + i) <= 'z') {
				*(str + i) = *(str + i) - 'a' + 'A';
			}
			if (*(key + i) >= 'a' && *(key + i) <= 'z') {
				*(key + i) = *(key + i) - 'a' + 'A';
			}
			for (j = 0; j < ALPHABET_SIZE; j++) {
				if (*(*(tabula + j)) == *(key + i)) {
					row = j;
				} 
			}
			for (j = 0; j < ALPHABET_SIZE; j++) {
				if (*(*(tabula + row) + j) == *(str + i)) {
					col = j;
				}
			}
			if (*(key + i) >= 'A' && *(key + i) <= 'Z') {
				*(ciphertext + i) = *(*tabula + col);
			}
		}
	}
	*(ciphertext + strlen(str)) = '\0';
}

int main(int argc, char **argv) {
	int sflag = 0;
	int aflag = 0;
	int hflag = 0;
	int eflag = 0;
	int dflag = 0;
	int c;
	opterr = 0; /* disable getopt error message */
	while ((c = getopt(argc, argv, "sahed")) != -1) {
		switch(c) {
		case 's':
			sflag = 1;
			break;
		case 'a':
			aflag = 1;
			break;
		case 'h':
			hflag = 1;
			break;
		case 'e':
			eflag = 1;
			break;
		case 'd':
			dflag = 1;
			break;
		case '?': /* invalid flag character */
			return EXIT_FAILURE;
		}
	}
	if (optind == 1) {
		print_help(argv);
		return EXIT_FAILURE;
	}
	if (sflag + aflag + hflag != 1 || eflag + dflag != 1) { /* -s, -a, -h used together, -e and -d used together */
		return EXIT_FAILURE;
	}
	if (sflag) {
		if (argc - optind != 3) {
			return EXIT_FAILURE;
		}
		if (eflag) {
			return substitution(argv, ENCRYPT);
		}
		else if (dflag) {
			return substitution(argv, DECRYPT);
		}
		else {
			return EXIT_FAILURE;
		}
	}
	if (aflag) {
		if (argc - optind != 4) {
			return EXIT_FAILURE;
		}
		if (eflag) {
			autokey(argv, ENCRYPT);
		}
		else if (dflag) {
			autokey(argv, DECRYPT);
		}
		else {
			return EXIT_FAILURE;
		}
	}
	if (hflag) {
		print_help(argv);
		return EXIT_SUCCESS;
	}
	return EXIT_FAILURE;
}