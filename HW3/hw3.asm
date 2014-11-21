# Homework #3
# name: Guanming Lin
# sbuid: 109299260

.text

toRadians:
  l.s $f0, pi
  mul.s $f0, $f12, $f0
  li $t0, 180
  mtc1 $t0, $f4
  cvt.s.w $f4, $f4
  div.s $f0, $f0, $f4
  jr $ra

arcLength:
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal toRadians
  mul.s $f0, $f0, $f13
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  jr $ra

atof:
  move $t0, $zero
  lb $t1, 0($a0)
  li $t2, '-'
  beq $t1, $t2, minus
  j plus
  
minus:
  li $t3, 1 #0 for positive, 1 for negative
  addi $a0, $a0, 1

plus:
  li $t2, '+'
  bne $t1, $t2, inf
  li $t3, 0
  addi $a0, $a0, 1
  
inf:
  lb $t1, 0($a0)
  li $t2, 'I'
  bne $t1, $t2, nan
  addi $a0, $a0, 1
  lb $t1, 0($a0)
  li $t2, 'n'
  bne $t1, $t2, done
  addi $a0, $a0, 1
  lb $t1, 0($a0)
  li $t2, 'f'
  bne $t1, $t2, done
  li $t0, 0x7f800000
  j getsign
  
nan:
  lb $t1, 0($a0)
  li $t2, 'N'
  bne $t1, $t2, exp
  addi $a0, $a0, 1
  lb $t1, 0($a0)
  li $t2, 'a'
  bne $t1, $t2, done
  addi $a0, $a0, 1
  lb $t1, 0($a0)
  li $t2, 'N'
  bne $t1, $t2, done
  li $t0, 0x7fffffff
  j getsign
  
exp:
  lb $t1, 0($a0)
  li $t2, '\n'
  beq $t1, $t2, done
  li $t2, '.'
  beq $t1, $t2, dec
  li $t2, '9'
  bgt $t1, $t2, done
  li $t2, '0'
  blt $t1, $t2, done
  sub $t1, $t1, $t2
  li $t2, 10
  mul $t0, $t0, $t2
  add $t0, $t0, $t1
  addi $a0, $a0, 1
  j exp

dec:
  addi $a0, $a0, 1
  li $t4, 0
  li $t5, 0
  li $t6, 1
  
man:
  lb $t1, 0($a0)
  li $t2, '\n'
  beq $t1, $t2, getdivisor
  li $t2, '9'
  bgt $t1, $t2, getdivisor
  li $t2, '0'
  blt $t1, $t2, getdivisor
  sub $t1, $t1, $t2
  li $t2, 10
  mul $t4, $t4, $t2
  add $t4, $t4, $t1
  addi $t5, $t5, 1
  addi $a0, $a0, 1
  j man
  
getdivisor:
  beqz $t5, divide
  li $t2, 10
  mul $t6, $t6, $t2
  addi $t5, $t5, -1
  j getdivisor
  
divide:
  mtc1 $t4, $f4
  mtc1 $t6, $f6
  cvt.s.w $f4, $f4
  cvt.s.w $f6, $f6
  div.s $f4, $f4, $f6
  
done:
  mtc1 $t0, $f0
  cvt.s.w $f0, $f0
  add.s $f0, $f0, $f4
  mtc1 $zero, $f4
  mfc1 $t0, $f0

getsign:
  sll $t3, $t3, 31
  or $t0, $t0, $t3
  mtc1 $t0, $f0
  jr $ra
  
print_parts:
  mfc1 $t0, $f12
  andi $t1, $t0, 0x80000000
  li $v0, PRINT_STRING
  la $a0, sign
  syscall
  beqz $t1, positive
  
negative:
  li $v0, 1
  li $a0, 1
  syscall
  li $v0, PRINT_STRING
  la $a0, negsign
  syscall
  j exponent
  
positive:
  li $v0, 1
  li $a0, 0
  syscall
  li $v0, PRINT_STRING
  la $a0, possign
  syscall
  
exponent:
  srl $t1, $t0, 23
  andi $t1, $t1, 255
  li $v0, PRINT_STRING
  la $a0, exponentstring
  syscall
  li $t2, 128
  li $t3, 8
  
printexp:
  beqz $t3, decexp
  move $t4, $t1
  and $t4, $t4, $t2
  li $v0, 11
  beqz $t4, printzero
  li $a0, '1'
  syscall
  addi $t3, $t3, -1
  srl $t2, $t2, 1
  j printexp

printzero:
  li $a0, '0'
  syscall
  addi $t3, $t3, -1
  srl $t2, $t2, 1
  j printexp
  
decexp:
  li $v0, PRINT_STRING
  la $a0, space
  syscall
  li $v0, 1
  move $a0, $t1
  syscall
  
mantissa:
  andi $t1, $t0, 8388607
  li $v0, PRINT_STRING
  la $a0, mantissastring
  syscall
  li $t2, 4194304
  li $t3, 23
  
printman:
  beqz $t3, decman
  move $t4, $t1
  and $t4, $t4, $t2
  li $v0, 11
  beqz $t4, printzeroman
  li $a0, '1'
  syscall
  addi $t3, $t3, -1
  srl $t2, $t2, 1
  j printman
  
printzeroman:
  li $a0, '0'
  syscall
  addi $t3, $t3, -1
  srl $t2, $t2, 1
  j printman
  
decman:
  li $v0, PRINT_STRING
  la $a0, space
  syscall
  li $v0, 1
  move $a0, $t1
  syscall
  li $v0, 11
  li $a0, '\n'
  syscall
  jr $ra
  
print_binary_product:
  mfc1 $t0, $f12
  li $t2, 4194304
  li $t3, 23
  andi $t1, $t0, 0x80000000
  li $v0, PRINT_STRING
  la $a0, binaryproduct
  syscall
  li $v0, 11
  beqz $t1, normalize
  li $a0, '-'
  syscall

normalize:
  li $a0, '1'
  syscall
  li $a0, '.'
  syscall
  li $t5, 0

removetrailing:
  beqz $t3, removedone
  andi $t1, $t0, 8338607
  and $t1, $t1, $t2
  beqz $t1, nextdig
  li $t4, 23
  addi $t3, $t3, -1
  srl $t2, $t2, 1
  sub $t5, $t4, $t3
  j removetrailing
  
nextdig:
  addi $t3, $t3, -1
  srl $t2, $t2, 1
  j removetrailing

removedone:
  move $t3, $t5
  li $t2, 4194304

printfract:
  beqz $t3, decbinprint
  andi $t1, $t0, 8388607
  and $t1, $t1, $t2
  li $v0, 11
  beqz $t1, printzerofract
  li $a0, '1'
  syscall
  j next
  
printzerofract:
  li $a0, '0'
  syscall
  
next:
  addi $t3, $t3, -1
  srl $t2, $t2, 1
  j printfract
  
decbinprint:
  li $v0, PRINT_STRING
  la $a0, multiply
  syscall
  srl $t1, $t0, 23
  andi $t1, $t1, 255
  li $t2, 128
  li $t3, 8
  li $t6, 2
  li $t7, 0

printexpbin:
  beqz $t3, binexpdone
  and $t4, $t1, $t2
  beqz $t4, nextexpbin
  add $t7, $t7, $t2
  addi $t3, $t3, -1
  srl $t2, $t2, 1
  j printexpbin

nextexpbin:
  addi $t3, $t3, -1
  srl $t2, $t2, 1
  j printexpbin
  
binexpdone:
  addi $t7, $t7, -127
  bgtz $t7, printplus
  j bindone

printplus:
  li $v0, 11
  li $a0, '+'
  syscall
  
bindone:
  li $v0, 1
  move $a0, $t7
  syscall
  li $v0, 11
  li $a0, '\n'
  syscall
  jr $ra

arcLengthS:
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  la $a0, input
  jal atof
  mov.s $f13, $f0
  jal arcLength
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  jr $ra
  
product:
  move $t1, $sp
  mtc1 $zero, $f0
  
productloop:
  beqz $a0, zero
  mfc1 $t0, $f0
  beqz $t0, first
  l.s $f4, 0($t1)
  mul.s $f0, $f0, $f4
  addi $a0, $a0, -1
  addi $t1, $t1, 4
  j productloop
  
first:
  l.s $f0, 0($t1)
  addi $a0, $a0, -1
  addi $t1, $t1, 4
  j productloop
  
zero:
  jr $ra
  
.data
sign: .asciiz "Sign: "
possign: .asciiz " +\n"
negsign: .asciiz " -\n"
exponentstring: .asciiz "Exponent: "
space: .asciiz " "
mantissastring: .asciiz "\nMantissa: "
binaryproduct: .asciiz "Binary product: "
multiply: .asciiz " x 2^"
