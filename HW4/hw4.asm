# Homework #4
# name: Guanming Lin
# sbuid: 109299260

.text

remainder:
  bge $a0, $a1, remainder_recursive
  move $v0, $a0
  jr $ra

remainder_recursive:
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  sub $a0, $a0, $a1
  jal remainder
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  jr $ra
  
printBase:
  li $t0, 2
  blt $a1, $t0, invalid_base
  li $t0, 9
  bgt $a1, $t0, invalid_base
  bge $a0, $a1, printBase_recursive
  j print_remainder
  
printBase_recursive:
  addi $sp, $sp, -8
  sw $ra, 4($sp)
  sw $a0, 0($sp)
  div $a0, $a1
  mflo $a0
  jal printBase
  lw $a0, 0($sp)
  addi $sp, $sp, 4
  jal print_remainder
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  jr $ra
  
invalid_base:
  la $a0, str_invalid_base
  li $v0, PRINT_STRING
  jr $ra
  
print_remainder:
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal remainder
  move $a0, $v0
  li $v0, 1
  syscall
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  jr $ra  
  
strlen:
  move $v0, $zero
  
count_characters:
  lb $t0, 0($a0)
  li $t1, '\0'
  beq $t0, $t1, null_character
  addi $a0, $a0, 1
  addi $v0, $v0, 1
  j count_characters
  
null_character:
  jr $ra
  
minimum:
  blt $a0, $a1, min1
  blt $a1, $a2, min2
  move $v0, $a2 #1 < 0, 2 < 1
  jr $ra
  
min1:
  blt $a0, $a2, min3
  move $v0, $a2 #0 < 1, 2 < 0
  jr $ra
  
min2:
  move $v0, $a1 #1 < 0, 1 < 2
  jr $ra
  
min3:
  move $v0, $a0 #0 < 1, 0 < 2
  jr $ra
  
levenshteinDistance:
  beqz $a1, s_zero
  beqz $a3, t_zero
  j levenshtein_recursion
  
levenshtein_recursion:
  addi $sp, $sp, -12
  sw $ra, 8($sp)
  sw $a1, 4($sp)
  sw $a3, 0($sp)
  addi $a1, $a1, -1
  jal levenshteinDistance
  lw $a1, 4($sp)
  lw $a3, 0($sp)
  addi $sp, $sp, 8
  addi $t3, $v0, 1
  addi $sp, $sp, -12
  sw $a1, 8($sp)
  sw $a3, 4($sp)
  sw $t3, 0($sp)
  addi $a3, $a3, -1
  jal levenshteinDistance
  lw $a1, 8($sp)
  lw $a3, 4($sp)
  lw $t3, 0($sp)
  addi $sp, $sp, 12
  addi $t4, $v0, 1
  addi $sp, $sp, -16
  sw $a1, 12($sp)
  sw $a3, 8($sp)
  sw $t3, 4($sp)
  sw $t4, 0($sp)
  addi $a1, $a1, -1
  addi $a3, $a3, -1
  jal levenshteinDistance
  lw $a1, 12($sp)
  lw $a3, 8($sp)
  lw $t3, 4($sp)
  lw $t4, 0($sp)
  addi $sp, $sp, 16
  addi $sp, $sp, -16
  sw $a0, 12($sp)
  sw $a1, 8($sp)
  sw $a2, 4($sp)
  sw $a3, 0($sp)
  addi $t5, $v0, 0
  addi $a1, $a1, -1
  addi $a3, $a3, -1
  add $a0, $a0, $a1
  add $a2, $a2, $a3
  lb $t6, 0($a0)
  lb $t7, 0($a2)
  beq $t6, $t7, levenshtein_done
  addi $t5, $t5, 1 #match
  
levenshtein_done:
  move $a0, $t3
  move $a1, $t4
  move $a2, $t5
  jal minimum
  lw $a0, 12($sp)
  lw $a1, 8($sp)
  lw $a2, 4($sp)
  lw $a3, 0($sp)
  addi $sp, $sp, 16
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  jr $ra 

s_zero:
  move $v0, $a3
  jr $ra

t_zero:
  move $v0, $a1
  jr $ra

rd100Chars:
  move $s0, $a0
  move $a0, $a1
  li $v0, PRINT_STRING
  syscall
  li $v0, READ_STRING
  move $s1, $a1
  move $a0, $s0
  move $s2, $a2
  li $a1, 101
  syscall
  addi $sp, $sp, -16
  sw $ra, 0($sp)
  sw $s0, 4($sp)
  sw $s1, 8($sp)
  sw $s2, 12($sp)
  jal strlen
  lw $ra, 0($sp)
  lw $s0, 4($sp)
  lw $s1, 8($sp)
  lw $s2, 12($sp)
  addi $sp, $sp, 16
  li $t0, 100
  bne $v0, $t0, rderror
  jr $ra
  
rderror:
  move $a0, $s2
  li $v0, PRINT_STRING
  syscall
  move $a0, $s0
  move $a1, $s1
  j rd100Chars
  
printIntMatrix:
  move $t0, $a0
  li $t1, 5
printRow:
  lw $a0, 0($t0)
  li $v0, 1
  syscall
  li $a0, ' '
  li $v0, 11
  syscall
  lw $a0, 4($t0)
  li $v0, 1
  syscall
  li $a0, ' '
  li $v0, 11
  syscall
  lw $a0, 8($t0)
  li $v0, 1
  syscall
  li $a0, ' '
  li $v0, 11
  syscall
  lw $a0, 12($t0)
  li $v0, 1
  syscall
  li $a0, ' '
  li $v0, 11
  syscall
  lw $a0, 16($t0)
  li $v0, 1
  syscall
  li $a0, '\n'
  li $v0, 11
  syscall
  addi $t1, $t1, -1
  addi $t0, $t0, 20
  bgtz $t1, printRow 
  jr $ra

matrixMult:
  lw $t0, i
  li $t1, 5
  bge $t0, $t1, matrixMult_done
  lw $t2, j
  bge $t2, $t1, matrixMult2
  lw $t3, k
  bge $t3, $t1, matrixMult3
  li $t1, 20
  addi $sp, $sp, -12
  sw $a0, 0($sp)
  sw $a1, 4($sp)
  sw $a2, 8($sp)
  mul $t5, $t0, $t1
  li $t1, 4
  mul $t6, $t2, $t1
  add $t5, $t5, $t6
  add $a2, $a2, $t5
  li $t1, 20
  mul $t5, $t0, $t1
  li $t1, 4
  mul $t6, $t3, $t1
  add $t5, $t5, $t6
  add $a0, $a0, $t5
  li $t1, 20
  mul $t5, $t3, $t1
  li $t1, 4
  mul $t6, $t2, $t1
  add $t5, $t5, $t6
  add $a1, $a1, $t5
  lw $t5, 0($a0)
  lw $t6, 0($a1)
  mul $t1, $t5, $t6
  lw $t7, 0($a2)
  addu $t7, $t7, $t1
  sw $t7, 0($a2)
  lw $a0, 0($sp)
  lw $a1, 4($sp)
  lw $a2, 8($sp)
  addi $sp, $sp, 12
  addi $t3, $t3, 1
  sw $t3, k
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal matrixMult
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  
matrixMult3:
  li $t3, 0
  sw $t3, k
  lw $t2, j
  addi $t2, $t2, 1
  sw $t2, j
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal matrixMult
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  
matrixMult2:
  li $t2, 0
  sw $t2, j
  lw $t0, i
  addi $t0, $t0, 1
  sw $t0, i
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal matrixMult
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  
matrixMult_done:
  li $t0, '\0'
  sb $t0, 100($a2)
  jr $ra

.data
str_invalid_base: .asciiz "Invalid base!"
.align 2
m1: .space 101
.align 2
m2: .space 101
.align 2
result: .space 101
.align 2
i: .word 0
j: .word 0
k: .word 0
