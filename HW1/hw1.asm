# Homework #1
# name: Guanming Lin
# sbuid: 109299260

#Enter an integer number:
#1682137665
#
#2's complement: 1682137665 0x64436241 01100100010000110110001001000001 1682137665
#1's complement: 1682137665 0x64436241 01100100010000110110001001000001 1682137665
#Sign Magnitude: 1682137665 0x64436241 01100100010000110110001001000001 1682137665
#Neg 2's complement: -1682137665 0x9bbc9dbf 10011011101111001001110110111111 -1682137665
#16-bit 2's comp: 25153 0x00006241 00000000000000000110001001000001 25153
#-- program is finished running --

.text
.globl main
main:
  la $a0,str
  li $v0,4
  syscall
  
  li $v0,5
  syscall
  move $s0,$v0
  
  la $a0,twos
  li $v0,4
  syscall
  move $a0,$s0
  li $v0,1
  syscall
  la $a0,space
  li $v0,4
  syscall
  move $a0,$s0
  li $v0,34
  syscall
  la $a0,space
  li $v0,4
  syscall
  move $a0,$s0
  li $v0,35
  syscall
  la $a0,space
  li $v0,4
  syscall
  move $a0,$s0
  li $v0,1
  syscall

  la $a0,ones
  li $v0,4
  syscall
  move $s1,$s0
  bltzal $s1,negones
  move $a0,$s1
  li $v0,100
  syscall
  la $a0,space
  li $v0,4
  syscall
  move $a0,$s1
  li $v0,34
  syscall
  la $a0,space
  li $v0,4
  syscall
  move $a0,$s1
  li $v0,35
  syscall
  la $a0,space
  li $v0,4
  syscall
  move $a0,$s1
  li $v0,1
  syscall

  la $a0,sign
  li $v0,4
  syscall
  move $s2,$s0
  bltzal $s2,negsign
  move $a0,$s2
  li $v0,101
  syscall
  la $a0,space
  li $v0,4
  syscall
  move $a0,$s2
  li $v0,34
  syscall
  la $a0,space
  li $v0,4
  syscall
  move $a0,$s2
  li $v0,35
  syscall
  la $a0,space
  li $v0,4
  syscall
  move $a0,$s2
  li $v0,1
  syscall

  la $a0,negtwos
  li $v0,4
  syscall
  sub $a0,$zero,$s0
  li $v0,1
  syscall
  move $s3,$a0
  la $a0,space
  li $v0,4
  syscall
  move $a0,$s3
  li $v0,34
  syscall
  la $a0,space
  li $v0,4
  syscall
  move $a0,$s3
  li $v0,35
  syscall
  la $a0,space
  li $v0,4
  syscall
  move $a0,$s3
  li $v0,1
  syscall

  la $a0,sixteen
  li $v0,4
  syscall
  li $t0,65535
  and $s4,$s0,$t0
  move $a0,$s4
  li $v0,102
  syscall
  la $a0,space
  li $v0,4
  syscall
  move $a0,$s4
  li $v0,34
  syscall
  la $a0,space
  li $v0,4
  syscall
  move $a0,$s4
  li $v0,35
  syscall
  la $a0,space
  li $v0,4
  syscall
  move $a0,$s4
  li $v0,1
  syscall

  li $v0,10
  syscall
  
negones:
  li $t0,1
  sub $s1,$s0,$t0
  jr $ra

negsign:
  sub $t0,$zero,$s0
  li $t1,10000000000000000000000000000000
  or $s2,$t0,$t1
  jr $ra

.data
str: .asciiz "Enter an integer number:\n"
twos: .asciiz "\n2's complement: "
ones: .asciiz "\n1's complement: "
sign: .asciiz "\nSign Magnitude: "
negtwos: .asciiz "\nNeg 2's complement: "
sixteen: .asciiz "\n16-bit 2's comp: "
space: .asciiz " "
