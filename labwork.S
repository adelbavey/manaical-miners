  # labwork.S
  # Written 2015 by F Lundevall
  # Skeleton file for IS1200/IS1500 lab 1 2015-2016.
  # The macros PUSH and POP are in the public domain.

.macro	PUSH reg
	addi	$sp,$sp,-4
	sw	\reg,0($sp)
.endm

.macro	POP reg
	lw	\reg,0($sp)
	addi	$sp,$sp,4
.endm

.data
.align 2

.text


.global enable_interrupt
enable_interrupt:
	ei
	jr $ra
	nop
  # You can add your own code here
  #

.global hexasc  
hexasc:
	#parameter $a0 = value to be changed to corresponding ascii
	andi $t0, $a0, 15
	
	ble $t0, 9, number
	nop
	addi $t0, $t0, 0x37
	andi $v0, $t0, 0x7f
	jr $ra
	nop
	
number:
	addi $t0, $t0, 0x30
	andi $v0, $t0, 0x7f
	jr $ra
	nop
	
########

#Not needed?
delay:	
	#parameter a0 = no. milliseconds
	addi $a0, $0, 1000	
	j while
	nop
while:
	ble $a0, $0, donewhile
	nop		
	addi $t0, $0, 0 #i	
	addi $a0, $a0, -1 #ms
	j for
	nop	
for:	
	bge $t0, 8000, while
	nop
	addi $t0, $t0, 1
	j for
	nop			
donewhile:
	jr $ra
	nop
	
	
#########


.global time2string
time2string:
	#parameters $a0 = memory adress 
	# $a1 = 16 nbcd coded digits in lsb and 16 non-important bits
	PUSH $s1
	PUSH $s2
	PUSH $s3
	PUSH $s4
	
	andi $t0, $a1, 0xffff # clear 16 msb
	andi $s1, $t0, 0xf000
	
	andi $s2, $t0, 0x0f00
	andi $s3, $t0, 0x00f0
	andi $s4, $t0, 0x000f
	
	srl $s1, $s1, 12
	srl $s2, $s2, 8
	srl $s3, $s3, 4
	
	PUSH $ra
	PUSH $a0
	
	move $a0, $s1
	jal hexasc
	nop
	move $s1, $v0
	
	move $a0, $s2
	jal hexasc
	nop
	move $s2, $v0
	
	move $a0, $s3
	jal hexasc
	nop
	move $s3, $v0
	
	move $a0, $s4
	jal hexasc
	nop
	move $s4, $v0
	
	POP $a0
	POP $ra
	
	addi $t0, $0, 0x3A
	addi $t1, $0, 0x00
	
	sb $s1, ($a0)
	sb $s2, 1($a0)
	sb $t0, 2($a0) #semicolon
	sb $s3, 3($a0)
	sb $s4, 4($a0)
	sb $t1, 5($a0) # nul byte
	
	PUSH $s4
	PUSH $s3
	PUSH $s2
	PUSH $s1
	
	jr $ra
	nop