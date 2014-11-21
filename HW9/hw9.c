/*
Homework #9
name: Guanming Lin
sbuid: 109299260
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include "hw9.h"

#ifdef CSE220
	#define cse220(instructions, ...)\
	do {\
		int i;\
		Instr *currInstr;\
		printf("CSE220: R-Type List:\n");\
		currInstr = instructions->head;\
		for (i = 0; i < instructions->count; i++) {\
			printf("CSE220: %p uid: %u pretty: %u, mnemonic: %s next: %p prev: %p\n", currInstr, currInstr->uid, currInstr->pretty, currInstr->mnemonic, currInstr->next, currInstr->prev);\
			currInstr = currInstr->next;\
		}\
		printf("CSE220: I-Type List:\n");\
		currInstr = instructions->next->head;\
		for (i = 0; i < instructions->next->count; i++) {\
			printf("CSE220: %p uid: %u pretty: %u, mnemonic: %s next: %p prev: %p\n", currInstr, currInstr->uid, currInstr->pretty, currInstr->mnemonic, currInstr->next, currInstr->prev);\
			currInstr = currInstr->next;\
		}\
		printf("CSE220: J-Type List:\n");\
		currInstr = instructions->next->next->head;\
		for (i = 0; i < instructions->next->next->count; i++) {\
			printf("CSE220: %p uid: %u pretty: %u, mnemonic: %s next: %p prev: %p\n", currInstr, currInstr->uid, currInstr->pretty, currInstr->mnemonic, currInstr->next, currInstr->prev);\
			currInstr = currInstr->next;\
		}\
		printf("CSE220: There is %zu r-type, %zu i-type, and %zu j-type nodes.\n", instructions->count, instructions->next->count, instructions->next->next->count);\
		printf("CSE220: The InstrType node takes up %zu bytes in memory.\n", sizeof(InstrType));\
		printf("CSE220: The Instr node takes up %zu bytes in memory.\n", sizeof(Instr));\
		printf("CSE220: Your program allocated %zu nodes that takes up %zu bytes in memory.\n", 3 + instructions->count + instructions->next->count + instructions->next->next->count, 3 * sizeof(InstrType) +  (instructions->count + instructions->next->count + instructions->next->next->count) * sizeof(Instr));\
	} while(0);
#else
	#define cse220(instructions, ...)
#endif

void print_help(char **argv);
InstrType *instruction_list(char *instruction_mapping);
int parse_binary(InstrType *instructions, char *input, char *output);
void free_instructions(InstrType *instructions);

void print_help (char **argv) {
	printf("Usage: %s [-h] [-m INSTRUCTION_MAPPING] -i INPUT_FILE -o OUTPUT_FILE\n", argv[0]);
	printf("%-25s%s\n", "-h", "Displays this help menu.");
	printf("%-25s%s\n", "-m INSTRUCTION_MAPPING", "File that contains the instruction mapping. If this option is not provided it defaults to instruction_mapping.txt.");
	printf("%-25s%s\n", "-i INPUT_FILE", "This should be a binary file that contains the .text section of a Mars220 MIPS binary.");
	printf("%-25s%s\n", "-o OUTPUT_FILE", "This can be any file on the system or \"-\" which specifies stdout.");
}

InstrType *instruction_list(char *instruction_mapping) {
	InstrType *rtype, *itype, *jtype;
	FILE *mapf;
	char type;
	uint32_t uid;
	char mnemonic[16];
	uint32_t pretty;
	int num_read = 0;
	InstrType *instrtype;
	Instr *instr, *currInstr;
	int i;

	if ((rtype = (InstrType *)malloc(sizeof(InstrType))) == NULL) {
		return NULL;
	}
	if ((itype = (InstrType *)malloc(sizeof(InstrType))) == NULL) {
		free(rtype);
		return NULL;
	}
	if ((jtype = (InstrType *)malloc(sizeof(InstrType))) == NULL) {
		free(rtype);
		free(itype);
		return NULL;
	}

	rtype->type = 'r';
	rtype->count = 0;
	rtype->next = itype;
	rtype->prev = NULL;
	rtype->head = NULL;
	itype->type = 'i';
	itype->count = 0;
	itype->next = jtype;
	itype->prev = rtype;
	itype->head = NULL;
	jtype->type = 'j';
	jtype->count = 0;
	jtype->next = NULL;
	jtype->prev = itype;
	jtype->head = NULL;

	mapf = fopen(instruction_mapping, "r");
	if (!mapf) {
		free(rtype);
		free(itype);
		free(jtype);
		return NULL;
	}

	while ((num_read = fscanf(mapf, "%c %x %s %u\n", &type, &uid, mnemonic, &pretty))) {
		if (num_read == 4) {
			if (type == 'r') {
				instrtype = rtype;
			}
			else if (type == 'i') {
				instrtype = itype;
			}
			else if (type == 'j') {
				instrtype = jtype;
			}
			else {
				return NULL;
			}
			instr = (Instr *)malloc(sizeof(Instr));
			if (instr == NULL) {
				free_instructions(rtype);
				fclose(mapf);
			}
			instr->uid = uid;
			if (type == 'i' || type == 'j') {
				instr->uid = instr->uid >> 26;
			}
			instr->pretty = pretty;
			instr->mnemonic = (char *)malloc(strlen(mnemonic + 1));
			if (instr->mnemonic == NULL) {
				free_instructions(rtype);
				fclose(mapf);
			}
			strcpy(instr->mnemonic, mnemonic);
			instr->next = NULL;
			instr->prev = NULL;
			if (instrtype->count == 0) {
				instrtype->head = instr;
				instrtype->count++;
			}
			else {
				currInstr = instrtype->head;
				for (i = 0; i < instrtype->count; i++) {
					if (strcmp(mnemonic, currInstr->mnemonic) < 0) {
						instrtype->count++;
						if (i == 0) {
							instr->next = instrtype->head;
							instrtype->head->prev = instr;
							instrtype->head = instr;
							break;
						}
						else {
							instr->next = currInstr;
							instr->prev = currInstr->prev;
							currInstr->prev->next = instr;
							currInstr->prev = instr;
							break;
						}
					}
					if (i == instrtype->count - 1) {
						instrtype->count++;
						currInstr->next = instr;
						instr->prev = currInstr;
						instr->next = NULL;
						break;
					}
					currInstr = currInstr->next;
				}
			}
		}
		if (feof(mapf)) {
			break;
		}
		else if(num_read != 4) {
			free_instructions(rtype);
			fclose(mapf);
			return NULL;
		}
	}
	fclose(mapf);
	return rtype;
}

int parse_binary(InstrType *instructions, char *input, char *output) {
	FILE *inf, *outf;
	inf = fopen(input, "rb");
	unsigned char bytes[4];
	uint32_t instruction;
	uint32_t uid;
	InstrType *instrtype;
	Instr *instr;
	int i;
	int num_read = 0;
	const char *register_map[32];
	register_map[0] = "zero";
	register_map[1] = "at";
	register_map[2] = "v0";
	register_map[3] = "v1";
	register_map[4] = "a0";
	register_map[5] = "a1";
	register_map[6] = "a2";
	register_map[7] = "a3";
	register_map[8] = "t0";
	register_map[9] = "t1";
	register_map[10] = "t2";
	register_map[11] = "t3";
	register_map[12] = "t4";
	register_map[13] = "t5";
	register_map[14] = "t6";
	register_map[15] = "t7";
	register_map[16] = "s0";
	register_map[17] = "s1";
	register_map[18] = "s2";
	register_map[19] = "s3";
	register_map[20] = "s4";
	register_map[21] = "s5";
	register_map[22] = "s6";
	register_map[23] = "s7";
	register_map[24] = "t8";
	register_map[25] = "t9";
	register_map[26] = "k0";
	register_map[27] = "k1";
	register_map[28] = "gp";
	register_map[29] = "sp";
	register_map[30] = "fp";
	register_map[31] = "ra";
	if (strcmp(output, "-") == 0) {
		outf = stdout;
	}
	else {
		outf = fopen(output, "w");
	}
	if ((num_read = fread(&bytes, sizeof(unsigned char), 4, inf)) != 4) {
		fclose(inf);
		fclose(outf);
		return EXIT_FAILURE;
	}
	if ((bytes[0] == 0x57 && bytes[1] == 0x6f && bytes[2] == 0x6e && bytes[3] == 0x67) ||
		(bytes[3] == 0x57 && bytes[2] == 0x6f && bytes[1] == 0x6e && bytes[0] == 0x67)) {
		num_read = 0;
		while ((num_read = fread(&bytes, sizeof(unsigned char), 4, inf)) == 4) {
			instruction = (bytes[0] << 24) + (bytes[1] << 16) + (bytes[2] << 8) + bytes[3];
			if (instruction >> 26 == 0) {
				uid = (instruction << 26) >> 26;
				instrtype = instructions;
			}
			else if (instruction >> 26 == 2 || instruction >> 26 == 3) {
				uid = (instruction >> 26);
				instrtype = instructions->next->next;
			}
			else {
				uid = (instruction >> 26);
				instrtype = instructions->next;
			}
			instr = instrtype->head;
			for (i = 0; i < instrtype->count; i++) {
				if (instr->uid == uid) {
					if (instr->pretty == 0x0) {
						fprintf(outf, "%s $%s\n", instr->mnemonic, register_map[(instruction << 16) >> 27]);
					}
					if (instr->pretty == 0x1) {
						fprintf(outf, "%s $%s, $%s\n", instr->mnemonic, register_map[(instruction << 6) >> 27], register_map[(instruction << 11) >> 27]);
					}
					if (instr->pretty == 0x2) {
						fprintf(outf, "%s $%s, $%s, 0x%x\n", instr->mnemonic, register_map[(instruction << 11) >> 27], register_map[(instruction << 6) >> 27], (instruction << 16) >> 16);
					}
					if (instr->pretty == 0x3) {
						fprintf(outf, "%s $%s, $%s, $%s\n", instr->mnemonic, register_map[(instruction << 16) >> 27], register_map[(instruction << 11) >> 27], register_map[(instruction << 6) >> 27]);
					}
					if (instr->pretty == 0x4) {
						fprintf(outf, "%s $%s, 0x%x($%s)\n", instr->mnemonic, register_map[(instruction << 11) >> 27], (instruction << 16) >> 16, register_map[(instruction << 6) >> 27]);
					}
					if (instr->pretty == 0x5) {
						fprintf(outf, "%s\n", instr->mnemonic);
					}
					if (instr->pretty == 0x6) {
						fprintf(outf, "%s 0x%x\n", instr->mnemonic, (instruction << 6) >> 6);
					}
					if (instr->pretty == 0x7) {
						fprintf(outf, "%s $%s, 0x%x\n", instr->mnemonic, register_map[(instruction << 6) >> 27], (instruction << 16) >> 16);
					}
					if (instr->pretty == 0x8) {
						fprintf(outf, "%s $%s, $%s, 0x%x\n", instr->mnemonic, register_map[(instruction << 16) >> 27], register_map[(instruction << 6) >> 27], (instruction << 21) >> 27);
					}
					if (instr->pretty == 0x9) {
						fprintf(outf, "%s $%s, $%s, 0x%x\n", instr->mnemonic, register_map[(instruction << 6) >> 27], register_map[(instruction << 11) >> 27], (instruction << 16) >> 16);
					}
					break;
				}
				instr = instr->next;
			}
		}
		if (num_read != 0) {
			fclose(inf);
			fclose(outf);
			return EXIT_FAILURE;
		}
	}
	else {
		fclose(inf);
		fclose(outf);
		return EXIT_FAILURE;
	}
	fclose(inf);
	if (outf != stdout) {
		fclose(outf);
	}
	return EXIT_SUCCESS;
}

void free_instructions(InstrType *instructions) {
	InstrType *instrtype;
	Instr *instr;
	int i;
	instrtype = instructions;
	instr = instrtype->head;
	for (i = 0; i < instrtype->count; i++) {
		if (instr != NULL) {
			if (instr->mnemonic != NULL) {
				free(instr->mnemonic);
			}
			instr->prev = NULL;
			if (instr->next != NULL) {
				instr = instr->next;
				free(instr->prev);
				instr->prev = NULL;
			}
			else {
				free(instr);
			}
		}
	}
	if (instrtype->next != NULL) {
		instrtype = instrtype->next;
		free(instrtype->prev);
		instrtype->prev = NULL;
		instr = instrtype->head;
		for (i = 0; i < instrtype->count; i++) {
			if (instr != NULL) {
				if (instr->mnemonic != NULL) {
					free(instr->mnemonic);
				}
				instr->prev = NULL;
				if (instr->next != NULL) {
					instr = instr->next;
					free(instr->prev);
					instr->prev = NULL;
				}
				else {
					free(instr);
				}
			}
		}
	}
	else {
		free(instrtype);
		return;
	}
	if (instrtype->next != NULL) {
		instrtype = instrtype->next;
		free(instrtype->prev);
		instrtype->prev = NULL;
		instr = instrtype->head;
		for (i = 0; i < instrtype->count; i++) {
			if (instr != NULL) {
				if (instr->mnemonic != NULL) {
					free(instr->mnemonic);
				}
				instr->prev = NULL;
				if (instr->next != NULL) {
					instr = instr->next;
					free(instr->prev);
					instr->prev = NULL;
				}
				else {
					free(instr);
				}
			}
		}
	}
	else {
		free(instrtype);
		return;
	}
	free(instrtype);
}

int main(int argc, char **argv) {
	char *mapping;
	char *input;
	char *output;
	int mflag = 0;
	int iflag = 0;
	int oflag = 0;
	int c;
	InstrType *instructions;
	opterr = 0;

	while ((c = getopt(argc, argv, "m:i:o:h")) != -1) {
		switch(c) {
		case 'm':
			mflag = 1;
			mapping = optarg;
			break;
		case 'i':
			iflag = 1;
			input = optarg;
			break;
		case 'o':
			oflag = 1;
			output = optarg;
			break;
		case 'h':
			print_help(argv);
			return EXIT_SUCCESS;
		case '?':
			return EXIT_FAILURE;
		}
	}
	if (optind == 1) {
		print_help(argv);
		return 0;
	}

	if (mflag == 0) {
		mapping = "instruction_mapping.txt";
	}

	if (iflag == 0 || oflag == 0) {
		return EXIT_FAILURE;
	}

	instructions = instruction_list(mapping);
	if (instructions == NULL) {
		return EXIT_FAILURE;
	}
	cse220(instructions);
	if (parse_binary(instructions, input, output) == EXIT_FAILURE) {
		free_instructions(instructions);
		return EXIT_FAILURE;
	}
	free_instructions(instructions);
	return EXIT_SUCCESS;
}