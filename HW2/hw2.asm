# Homework #2
# name: Guanming Lin
# sbuid: 109299260

.text
.globl main

main:
  la $a0,menu
  li $v0,4
  syscall
  
  li $v0,5
  syscall
  li $t0,1
  li $t1,2
  li $t2,3
  beq $v0,$t0,le
  beq $v0,$t1,be
  beq $v0,$t2,exit
  
  la $a0,error
  li $v0,4
  syscall
  j main
#$s7=1 : little endian, $s7=2 : big endian  
le:

  li $s7,1
  j read
  
be:
  li $s7,2
  j read
   
read:
  la $a0,prompt
  li $v0,4
  syscall

  li $v0,8
  la $a0,path
  li $a1,256
  syscall
  move $s0,$a0
  move $s1,$s0
  #$s0 = input file address
  la $a0,prompt2
  li $v0,4
  syscall
  
  li $v0,8
  la $a0,path2
  li $a1,256
  syscall
  move $s2,$a0
  move $s3,$s2
  #$s2 = output file address
loop:
  
  lb $t0,($s1)
  addi $s1,$s1,1
  li $t1,'\n'
  bne $t0,$t1,loop
  addi $s1,$s1,-1
  sb $0,($s1)
  #replace '\n' with '\0' of input path
loop2:
  lb $t0,($s3)
  addi $s3,$s3,1
  li $t1,'\n'
  bne $t0,$t1,loop2
  addi $s3,$s3,-1
  sb $0,($s3)
  #replace '\n' with '\0' of output path
cont:
  move $a0,$s0
  li $a1,0
  li $v0,13
  syscall
  bltz $v0,openerror
  move $s6,$v0
  #$s6 = input file descriptor
  j cont2
  
openerror:
  la $a0,error2
  li $v0,4
  syscall
  j read

cont2:
  move $a0,$s6
  la $a1,byte
  li $a2,1
  li $v0,14
  syscall
  lbu $a1,0($a1)
  li $t0,0xef
  bne $t0,$a1,bomerror
  la $a1,byte
  li $v0,14
  syscall
  lbu $a1,0($a1)
  li $t0,0xbb
  bne $t0,$a1,bomerror
  la $a1,byte
  li $v0,14
  syscall
  lbu $a1,0($a1)
  li $t0,0xbf
  bne $t0,$a1,bomerror
  #UTF-8 BOM
  move $a0,$s2
  li $a1,1
  li $a2,0
  li $v0,13
  syscall
  bltz $v0,openerror
  move $s5,$v0
  #$s5 = output file descriptor
  li $t0,1
  li $t1,2
  beq $s7,$t0,lewrite
  beq $s7,$t1,bewrite
  
lewrite:
  move $a0,$s5
  la $t0,ff
  move $a1,$t0
  li $a2,1
  li $v0,15
  syscall
  la $t1,fe
  move $a1,$t1
  li $v0,15
  syscall
  #write UTF-16LE BOM
  j loople
  
bewrite:
  move $a0,$s5
  la $t0,fe
  move $a1,$t0
  li $a2,1
  li $v0,15
  syscall
  la $t1,ff
  move $a1,$t1
  li $v0,15
  syscall
  #write UTF-16BE BOM
  j loopbe
  
bomerror:
  la $a0,error3
  li $v0,4
  syscall
  j read


#LE case
loople:
  move $a0,$s6
  la $a1,byte
  li $a2,1
  li $v0,14
  syscall
  beqz $v0,exit
  lbu $a1,0($a1)
  srl $t0,$a1,3
  li $t1,30
  beq $t0,$t1,fourle
  srl $t0,$a1,4
  li $t1,14
  beq $t0,$t1,threele
  srl $t0,$a1,5
  li $t1,6
  beq $t0,$t1,twole
  srl $t0,$a1,7
  beq $t0,$zero,onele
  la $a0,error4
  li $v0,4
  syscall
  j read

fourle:
  li $a0,4
  li $v0,1
  syscall
  la $a0,b
  li $v0,4
  syscall
  move $a0,$a1
  move $t0,$a1
  li $v0,34
  syscall
  la $a0,space
  li $v0,4
  syscall
  move $a0,$s6
  la $a1,byte
  li $v0,14
  syscall
  lbu $a1,0($a1)
  move $a0,$a1
  move $t1,$a1
  li $v0,34
  syscall
  la $a0,space
  li $v0,4
  syscall
  move $a0,$s6
  la $a1,byte
  li $v0,14
  syscall
  lbu $a1,0($a1)
  move $a0,$a1
  move $t2,$a1
  li $v0,34
  syscall
  la $a0,space
  li $v0,4
  syscall
  move $a0,$s6
  la $a1,byte
  li $v0,14
  syscall
  lbu $a1,0($a1)
  move $a0,$a1
  move $t3,$a1
  li $v0,34
  syscall
  andi $t0,$t0,7 #00000111
  sll $t0,$t0,18
  andi $t1,$t1,63 #00111111
  sll $t1,$t1,12
  andi $t2,$t2,63
  sll $t2,$t2,6
  andi $t3,$t3,63
  or $t0,$t0,$t1
  or $t0,$t0,$t2
  or $t0,$t0,$t3
  la $a0,space
  li $v0,4
  syscall
  la $a0,u
  li $v0,4
  syscall
  move $a0,$t0
  li $v0,34
  syscall
  move $a0,$s5
  li $t1,0x10000
  bgeu $t0,$t1,surrogatele
  la $a1,codepoint
  sb $t0,codepoint #first byte
  srl $t0,$t0,8
  li $a2,2
  sb $t0,codepoint+1 #second byte
  li $v0,15
  syscall
  la $a0,newline
  li $v0,4
  syscall
  j loople
  
threele:
  li $a0,3
  li $v0,1
  syscall
  la $a0,b
  li $v0,4
  syscall
  move $a0,$a1
  move $t0,$a1
  li $v0,34
  syscall
  la $a0,space
  li $v0,4
  syscall
  move $a0,$s6
  la $a1,byte
  li $v0,14
  syscall
  lbu $a1,0($a1)
  move $a0,$a1
  move $t1,$a1
  li $v0,34
  syscall
  la $a0,space
  li $v0,4
  syscall
  move $a0,$s6
  la $a1,byte
  li $v0,14
  syscall
  lbu $a1,0($a1)
  move $a0,$a1
  move $t2,$a1
  li $v0,34
  syscall
  andi $t0,$t0,15 #00001111
  sll $t0,$t0,12
  andi $t1,$t1,63
  sll $t1,$t1,6
  andi $t2,$t2,63
  or $t0,$t0,$t1
  or $t0,$t0,$t2
  la $a0,space
  li $v0,4
  syscall
  la $a0,u
  li $v0,4
  syscall
  move $a0,$t0
  li $v0,34
  syscall
  move $a0,$s5
  li $t1,0x10000
  bgeu $t0,$t1,surrogatele
  la $a1,codepoint
  sb $t0,codepoint
  srl $t0,$t0,8
  li $a2,2
  sb $t0,codepoint+1
  li $v0,15
  syscall
  la $a0,newline
  li $v0,4
  syscall
  j loople
  
twole:
  li $a0,2
  li $v0,1
  syscall
  la $a0,b
  li $v0,4
  syscall
  move $a0,$a1
  move $t0,$a1
  li $v0,34
  syscall
  la $a0,space
  li $v0,4
  syscall
  move $a0,$s6
  la $a1,byte
  li $v0,14
  syscall
  lbu $a1,0($a1)
  move $a0,$a1
  move $t1,$a1
  li $v0,34
  syscall
  andi $t0,$t0,31 #00011111
  sll $t0,$t0,6
  andi $t1,$t1,63
  or $t0,$t0,$t1
  la $a0,space
  li $v0,4
  syscall
  la $a0,u
  li $v0,4
  syscall
  move $a0,$t0
  li $v0,34
  syscall
  move $a0,$s5
  li $t1,0x10000
  bgeu $t0,$t1,surrogatele
  la $a1,codepoint
  sb $t0,codepoint
  srl $t0,$t0,8
  li $a2,2
  sb $t0,codepoint+1
  li $v0,15
  syscall
  la $a0,newline
  li $v0,4
  syscall
  j loople
  
onele:
  li $a0,1
  li $v0,1
  syscall
  la $a0,b
  li $v0,4
  syscall
  move $a0,$a1
  li $v0,34
  syscall
  la $a0,space
  li $v0,4
  syscall
  la $a0,u
  li $v0,4
  syscall
  move $a0,$a1
  move $t0,$a1
  li $v0,34
  syscall
  li $t1,0x10000
  move $a0,$s5
  bgeu $t0,$t1,surrogatele
  la $a1,codepoint
  sb $t0,codepoint
  srl $t0,$t0,8
  li $a2,2
  sb $t0,codepoint+1
  li $v0,15
  syscall
  la $a0,newline
  li $v0,4
  syscall
  j loople
  
surrogatele:
  sub $t0,$t0,$t1
  srl $t2,$t0,10
  andi $t3,$t0,0x3ff
  addiu $t2,$t2,0xd800
  addiu $t3,$t3,0xdc00
  
  li $a2,2
  la $a1,codepoint
  sb $t2,codepoint
  srl $t2,$t2,8
  sb $t2,codepoint+1
  move $a0,$s5
  li $v0,15
  syscall
  sb $t3,codepoint
  srl $t3,$t3,8
  sb $t3,codepoint+1
  li $v0,15
  syscall
  la $a0,newline
  li $v0,4
  syscall
  j loople

#BE case
loopbe:
  move $a0,$s6
  la $a1,byte
  li $a2,1
  li $v0,14
  syscall
  beqz $v0,exit
  lbu $a1,0($a1)
  srl $t0,$a1,3
  li $t1,30
  beq $t0,$t1,fourbe
  srl $t0,$a1,4
  li $t1,14
  beq $t0,$t1,threebe
  srl $t0,$a1,5
  li $t1,6
  beq $t0,$t1,twobe
  srl $t0,$a1,7
  beq $t0,$zero,onebe
  la $a0,error4
  li $v0,4
  syscall
  j read

fourbe:
  li $a0,4
  li $v0,1
  syscall
  la $a0,b
  li $v0,4
  syscall
  move $a0,$a1
  move $t0,$a1
  li $v0,34
  syscall
  la $a0,space
  li $v0,4
  syscall
  move $a0,$s6
  la $a1,byte
  li $v0,14
  syscall
  lbu $a1,0($a1)
  move $a0,$a1
  move $t1,$a1
  li $v0,34
  syscall
  la $a0,space
  li $v0,4
  syscall
  move $a0,$s6
  la $a1,byte
  li $v0,14
  syscall
  lbu $a1,0($a1)
  move $a0,$a1
  move $t2,$a1
  li $v0,34
  syscall
  la $a0,space
  li $v0,4
  syscall
  move $a0,$s6
  la $a1,byte
  li $v0,14
  syscall
  lbu $a1,0($a1)
  move $a0,$a1
  move $t3,$a1
  li $v0,34
  syscall
  andi $t0,$t0,7 #00000111
  sll $t0,$t0,18
  andi $t1,$t1,63 #00111111
  sll $t1,$t1,12
  andi $t2,$t2,63
  sll $t2,$t2,6
  andi $t3,$t3,63
  or $t0,$t0,$t1
  or $t0,$t0,$t2
  or $t0,$t0,$t3
  la $a0,space
  li $v0,4
  syscall
  la $a0,u
  li $v0,4
  syscall
  move $a0,$t0
  li $v0,34
  syscall
  move $a0,$s5
  li $t1,0x10000
  bgeu $t0,$t1,surrogatebe
  la $a1,codepoint
  srl $t1,$t0,8
  li $a2,2
  sb $t1,codepoint
  sb $t0,codepoint+1
  li $v0,15
  syscall
  la $a0,newline
  li $v0,4
  syscall
  j loopbe
  
threebe:
  li $a0,3
  li $v0,1
  syscall
  la $a0,b
  li $v0,4
  syscall
  move $a0,$a1
  move $t0,$a1
  li $v0,34
  syscall
  la $a0,space
  li $v0,4
  syscall
  move $a0,$s6
  la $a1,byte
  li $v0,14
  syscall
  lbu $a1,0($a1)
  move $a0,$a1
  move $t1,$a1
  li $v0,34
  syscall
  la $a0,space
  li $v0,4
  syscall
  move $a0,$s6
  la $a1,byte
  li $v0,14
  syscall
  lbu $a1,0($a1)
  move $a0,$a1
  move $t2,$a1
  li $v0,34
  syscall
  andi $t0,$t0,15 #00001111
  sll $t0,$t0,12
  andi $t1,$t1,63
  sll $t1,$t1,6
  andi $t2,$t2,63
  or $t0,$t0,$t1
  or $t0,$t0,$t2
  la $a0,space
  li $v0,4
  syscall
  la $a0,u
  li $v0,4
  syscall
  move $a0,$t0
  li $v0,34
  syscall
  move $a0,$s5
  li $t1,0x10000
  bgeu $t0,$t1,surrogatebe
  la $a1,codepoint
  srl $t1,$t0,8
  li $a2,2
  sb $t1,codepoint
  sb $t0,codepoint+1
  li $v0,15
  syscall
  la $a0,newline
  li $v0,4
  syscall
  j loopbe
  
twobe:
  li $a0,2
  li $v0,1
  syscall
  la $a0,b
  li $v0,4
  syscall
  move $a0,$a1
  move $t0,$a1
  li $v0,34
  syscall
  la $a0,space
  li $v0,4
  syscall
  move $a0,$s6
  la $a1,byte
  li $v0,14
  syscall
  lbu $a1,0($a1)
  move $a0,$a1
  move $t1,$a1
  li $v0,34
  syscall
  andi $t0,$t0,31 #00011111
  sll $t0,$t0,6
  andi $t1,$t1,63
  or $t0,$t0,$t1
  la $a0,space
  li $v0,4
  syscall
  la $a0,u
  li $v0,4
  syscall
  move $a0,$t0
  li $v0,34
  syscall
  move $a0,$s5
  li $t1,0x10000
  bgeu $t0,$t1,surrogatebe
  la $a1,codepoint
  srl $t1,$t0,8
  li $a2,2
  sb $t1,codepoint
  sb $t0,codepoint+1
  li $v0,15
  syscall
  la $a0,newline
  li $v0,4
  syscall
  j loopbe
  
onebe:
  li $a0,1
  li $v0,1
  syscall
  la $a0,b
  li $v0,4
  syscall
  move $a0,$a1
  li $v0,34
  syscall
  la $a0,space
  li $v0,4
  syscall
  la $a0,u
  li $v0,4
  syscall
  move $a0,$a1
  move $t0,$a1
  li $v0,34
  syscall
  move $a0,$s5
  li $t1,0x10000
  bgeu $t0,$t1,surrogatebe
  la $a1,codepoint
  srl $t1,$t0,8
  li $a2,2
  sb $t1,codepoint
  sb $t0,codepoint+1
  li $v0,15
  syscall
  la $a0,newline
  li $v0,4
  syscall
  j loopbe
  
surrogatebe:
  sub $t0,$t0,$t1
  srl $t2,$t0,10
  andi $t3,$t0,0x3ff
  addiu $t2,$t2,0xd800
  addiu $t3,$t3,0xdc00
  
  li $a2,2
  la $a1,codepoint
  srl $t4,$t2,8
  sb $t4,codepoint
  sb $t2,codepoint+1
  move $a0,$s5
  li $v0,15
  syscall
  srl $t5,$t3,8
  sb $t5,codepoint
  sb $t3,codepoint+1
  li $v0,15
  syscall
  la $a0,newline
  li $v0,4
  syscall
  j loopbe
  
exit:
  li $v0,16
  move $a0,$s6
  syscall
  li $v0,16
  move $a0,$s5
  syscall
  li $v0,10
  syscall

.data
menu: .asciiz "1) Encode to UTF-16LE\n2) Encode to UTF-16BE\n3) Exit\n"
prompt: .asciiz "Enter path of file to convert:\n"
prompt2: .asciiz "Enter path for output file:\n"
error: .asciiz "Invalid choice, try again\n"
error2: .asciiz "Invalid path, try again\n"
error3: .asciiz "Wrong BOM, try another file\n"
error4: .asciiz "Invalid UTF-8 encoding, try another file\n"
path: .space 256
path2: .space 256
byte: .space 1
b: .asciiz " byte "
space: .asciiz " "
newline: .asciiz "\n"
u: .asciiz "U+"
ff: .byte 0xff
fe: .byte 0xfe
codepoint: .space 2