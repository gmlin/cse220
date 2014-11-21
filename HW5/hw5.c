/*
Homework #5
name: Guanming Lin
sbuid: 109299260
*/

#include <stdlib.h>
#include <stdio.h>

/* Function prototypes */
int promptForId(void);
int scrambleId(int id);
int xor(int v, int id);
int shift(int v, int n);
int printResults(int original, int scrambled);

/* Ask the user for their SBUID */
int promptForId(void) {
	int id, valid;
	char line[256];
	/* Loop until valid input is given */
	do {
		printf("Please enter your SBUID: ");
		/* Read in the whole line */
		fgets(line, sizeof(line), stdin);
		/* If we were not able to parse the string, tell the user */
		if(!(valid = sscanf(line, "%d", &id))) {
			printf("Invalid id entered.\n");
		}
	} while(!valid);
	return id;
}

/* Scramble the SBUID */
int scrambleId(int id) {
	int scrambled = id, i;
	int r;
	int rounds;
	srand(id);
	r = rand();
	/* Bound this random number between 1-10 */
	rounds = (rand() % 10) + 1;
	printf("Performing %d rounds of shifting and xoring.\n", rounds);
	/* Go through some crazy shifting operations */
	for(i = 0; i < rounds; i++)
		scrambled = shift(scrambled, r >>= 1);
	return scrambled;
}

/* Perform bitwise XOR on the SBU ID */
int xor(int v, int id) {
	int result;
	result = v ^ id;
	return result;
}

/* Shift the value right and xor */
int shift(int v, int r) {
	int shamt = 0x08000000;
	int result = 0;
	while(shamt) {
	        result += xor(v, r); /* add the return value of xor to result */
		shamt = shamt >> 1; /* Shift right by 1 */
	}
	return result;
}

int printResults(int original, int scrambled) {
        /* If the scrambled number is even */
        int isEven = scrambled % 2; 
	printf("%d -> %d\n", original, scrambled);
	if(isEven == 0) {
		printf("The scrambled result is even!\n");
		return 0;
	}
	/* else it is odd */
	printf("The scrambled result is odd!\n");
	return 127;
}

/* The program starts here */
int main() {
	int id;
	int scrambled;
	int result;
	id = promptForId();
	printf("SBUID: %d\n", id);
	/* Scramble the SBUID */
	scrambled = scrambleId(id);
	/* Print out the scrambled value */
	result = printResults(id, scrambled);
	return result;
}

/* @@GDB_OUTPUT_HERE@@

(gdb) clear
No source file specified.
(gdb) break 27
Breakpoint 1 at 0x10c08: file hw5.c, line 27.
(gdb) break 46
Breakpoint 2 at 0x10cf8: file hw5.c, line 46.
(gdb) break 53
Breakpoint 3 at 0x10d54: file hw5.c, line 53.
(gdb) run
Starting program: /export/home2/g/u/guanmlin/a.out 
[Thread debugging using libthread_db enabled]
[New Thread 1 (LWP 1)]
Please enter your SBUID: 109299260
[Switching to Thread 1 (LWP 1)]

Breakpoint 1, promptForId () at hw5.c:27
27                      if(!(valid = sscanf(line, "%d", &id))) {
(gdb) print line
$1 = "109299260\n", '\000' <repeats 46 times>, "\377\377\377\377\377\377\377\377\377:\000\000\377\277\373\300\377:@L\000\000\000\f\000\001\000\064\377\277\377\332\377\377\377\377\000\000\000\000\377\277\374\254\000\000\000\002\377\377\377\377\377\377\377\377\377\277\373\310\377:X\350\000\000\000\000\000\000\000\000\377\277\374\374\000\000\000\000\000\000\000\001\000\001\005\v\377\067\np\377 \267\200\000\001\005\v\377\067\004 \377\067\004 \000\000\000\002\006\212\317\004\000\000\000\021\000\001\003L\025\001\377\377\000\001\000\000\000\000\000\000\000\000\000\002\377\277\373\300"...
(gdb) continue
Continuing.
SBUID: 109299260
Performing 5 rounds of shifting and xoring.

Breakpoint 2, scrambleId (id=109299260) at hw5.c:46
46                      scrambled = shift(scrambled, r >>= 1);
(gdb) print i
$2 = 0
(gdb) continue
Continuing.

Breakpoint 3, xor (v=109299260, id=15732) at hw5.c:53
53              result = v ^ id;
(gdb) backtrace
#0  xor (v=109299260, id=15732) at hw5.c:53
#1  0x00010da4 in shift (v=109299260, r=15732) at hw5.c:62
#2  0x00010d14 in scrambleId (id=109299260) at hw5.c:46
#3  0x00010e9c in main () at hw5.c:89
(gdb) info frame
Stack level 0, frame at 0xffbffb00:
 pc = 0x10d54 in xor (hw5.c:53); saved pc = 0x10da4
 called by frame at 0xffbffb68
 source language c.
 Arglist at 0xffbffb00, args: v=109299260, id=15732
 Locals at 0xffbffb00, Previous frame's sp in fp
 Saved registers:
  l0 at 0xffbffb00, l1 at 0xffbffb04, l2 at 0xffbffb08, l3 at 0xffbffb0c,
  l4 at 0xffbffb10, l5 at 0xffbffb14, l6 at 0xffbffb18, l7 at 0xffbffb1c,
  i0 at 0xffbffb20, i1 at 0xffbffb24, i2 at 0xffbffb28, i3 at 0xffbffb2c,
  i4 at 0xffbffb30, i5 at 0xffbffb34, fp at 0xffbffb38, i7 at 0xffbffb3c
(gdb) clear xor
Deleted breakpoint 3 
(gdb) continue
Continuing.

Breakpoint 2, scrambleId (id=109299260) at hw5.c:46
46                      scrambled = shift(scrambled, r >>= 1);
(gdb) print i
$3 = 1
(gdb) continue
Continuing.

Breakpoint 2, scrambleId (id=109299260) at hw5.c:46
46                      scrambled = shift(scrambled, r >>= 1);
(gdb) print i
$4 = 2
(gdb) continue
Continuing.

Breakpoint 2, scrambleId (id=109299260) at hw5.c:46
46                      scrambled = shift(scrambled, r >>= 1);
(gdb) print i
$5 = 3
(gdb) continue
Continuing.

Breakpoint 2, scrambleId (id=109299260) at hw5.c:46
46                      scrambled = shift(scrambled, r >>= 1);
(gdb) print i
$6 = 4
(gdb) continue
Continuing.
109299260 -> -955179996
The scrambled result is even!
[Inferior 1 (process 18784    ) exited normally]
(gdb) quit

@@GDB_OUTPUT_HERE@@ */
