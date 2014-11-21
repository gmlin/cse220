/*
Homework #6
name: Guanming Lin
sbuid: 109299260
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>

#define ERROR_REG 1
#define ERROR_INSTR 2
#define ERROR_IMMEDIATE 3
#define ERROR_FLAGS 4

void print_help(char **argv);
int instructions(int uflag);
int immediates(int uflag);
int registers(int uflag);

void print_help (char **argv) {
	printf("Usage: %s [-himru]\n", argv[0]);
	printf("%6s         %s\n", "-h", "Displays this help menu.");
	printf("%6s         %s\n", "-i", "Displays statistics about instruction type usages.");
	printf("%6s         %s\n", "-m", "Displays all the immediate values used in I-Type and J-Type instructions.");
	printf("%6s         %s\n", "-r", "Displays information about the registers.");
	printf("%6s         %s\n", "-u", "Displays human readable headers for the different information displayed.");
	printf("               %s\n", "Should only be used with the -i, -r, and -m flags.");
}

int instructions(int uflag) {
	int r = 0;
	int i = 0;
	int j = 0;
	char line[256];
	int status;
	int not_empty = 0;
	unsigned hex;
	unsigned opcode;
	int total = 0;
	if (uflag) {
		printf("%6s%8s%12s\n", "TYPE", "COUNT", "PERCENT");
	}
	while (1) {
		status = scanf("0x%[0-9a-fA-F]\n", line); /* check if string is valid */
		if (status == EOF) { /* exit loop */
			if (not_empty == 0)  { /* check if EOF on first run of scanf */
				return ERROR_INSTR;
			}
			break;
		}
		else if (status == 1 && strlen(line) == 8 && sscanf(line, "%x", &hex) == 1) {
			opcode = hex >> 26;
			if (opcode == 0) {
				r++;
			}
			else if (opcode == 2 || opcode == 3) {
				j++;
			}
			else {
				i++;
			}
		}
		else { /* nothing matched, invalid input */
			return ERROR_INSTR;
		}
		not_empty = 1;
	}
	total = r + i + j;
	printf("%s%8d%10.1f%%\n", "R-type", r, 100.0 * r / total);
	printf("%s%8d%10.1f%%\n", "I-type", i, 100.0 * i / total);
	printf("%s%8d%10.1f%%\n", "J-type", j, 100.0 * j / total);
	return EXIT_SUCCESS;
}

int immediates(int uflag) {
	char line[256];
	int status;
	int not_empty = 0;
	unsigned hex;
	unsigned opcode;
	unsigned immediate;
	if (uflag) {
		printf("IMMEDIATE-VALUE\n");
	}
	while (1) {
		status = scanf("0x%[0-9a-fA-F]\n", line); /* check if string is valid */
		if (status == EOF) { /* exit loop */
			if (not_empty == 0)  { /* check if EOF on first run of scanf */
				return ERROR_INSTR;
			}
			break;
		}
		else if (status == 1 && strlen(line) == 8 && sscanf(line, "%x", &hex) == 1) {
			opcode = hex >> 26;
			if (opcode == 2 || opcode == 3) {
				immediate = hex << 6;
				immediate = immediate >> 6;
				printf("0x%x\n", immediate);
			}
			else if (opcode != 0) {
				immediate = hex << 16;
				immediate = immediate >> 16;
				printf("0x%x\n", immediate);
			}
		}
		else { /* nothing matched, invalid input */
			return ERROR_IMMEDIATE; 
		}
		not_empty = 1;
	}
	return EXIT_SUCCESS;
}

int registers(int uflag) {
	char line[256];
	int status;
	int not_empty = 0;
	unsigned hex;
	unsigned opcode;
	unsigned register_number;
	int register_usage[32][4]; /* 0 - Total, 1 - R-TYPE, 2 - I-TYPE, 3 - J-TYPE */
	int total = 0;
	int i;
	int j;
	if (uflag) {
		printf("%s%8s  %8s %8s %8s%10s\n", "REG", "USE", "R-TYPE", "I-TYPE", "J-TYPE", "PERCENT");
	}
	for (i = 0; i < 32; i++) {
		for (j = 0; j < 4; j++) {
			register_usage[i][j] = 0;
		}
	}
	while (1) {
		status = scanf("0x%[0-9a-fA-F]\n", line); /* check if string is valid */
		if (status == EOF) { /* exit loop */
			if (not_empty == 0)  { /* check if EOF on first run of scanf */
				return ERROR_INSTR;
			}
			break;
		}
		else if (status == 1 && strlen(line) == 8 && sscanf(line, "%x", &hex) == 1) {
			opcode = hex >> 26;
			if (opcode == 0) {
				register_number = hex << 6;
				register_number = register_number >> 27;
				register_usage[register_number][0]++;
				register_usage[register_number][1]++;
				register_number = hex << 11;
				register_number = register_number >> 27;
				register_usage[register_number][0]++;
				register_usage[register_number][1]++;
				register_number = hex << 16;
				register_number = register_number >> 27;
				register_usage[register_number][0]++;
				register_usage[register_number][1]++;
				total += 3;
			}
			else if (!(opcode == 2 || opcode == 3)) {
				register_number = hex << 6;
				register_number = register_number >> 27;
				register_usage[register_number][0]++;
				register_usage[register_number][2]++;
				register_number = hex << 11;
				register_number = register_number >> 27;
				register_usage[register_number][0]++;
				register_usage[register_number][2]++;
				total += 2;
			}
		}
		else { /* nothing matched, invalid input */
			return ERROR_REG;
		}
		not_empty = 1;
	}
	for (i = 0; i < 32; i++) {
		printf("$%-6d %-6d %-6d   %-6d   %-6d   %-.1f%%\n", i, register_usage[i][0], register_usage[i][1], register_usage[i][2], register_usage[i][3], 100.0 * register_usage[i][0] / total);
	}
	return EXIT_SUCCESS;
}

int main(int argc, char **argv) {
	int iflag = 0;
	int mflag = 0;
	int rflag = 0;
	int uflag = 0;
	int c;
	opterr = 0; /* disable getopt error message */
	while ((c = getopt(argc, argv, "himru")) != -1) {
		switch(c) {
		case 'h':
			print_help(argv);
			return EXIT_SUCCESS;
		case 'i':
			iflag = 1;
			break;
		case 'm':
			mflag = 1;
			break;
		case 'r':
			rflag = 1;
			break;
		case 'u':
			uflag = 1;
			break;
		case '?': /* invalid flag character */
			return ERROR_FLAGS;
		}
	}
	if (optind == 1) {
		print_help(argv);
		return ERROR_FLAGS;
	}
	if ((iflag && mflag) || (iflag && rflag) || (mflag && rflag)) { /* -i, -m, -r used together */
		return ERROR_FLAGS;
	}
	if (uflag && !(iflag || mflag || rflag)) { /* -u used  without -i, -m, or -r */
		return ERROR_FLAGS;
	}
	if (iflag) {
		return instructions(uflag);
	}
	if (mflag) {
		return immediates(uflag);
	}
	if (rflag) {
		return registers(uflag);
	}
	return ERROR_FLAGS;
}