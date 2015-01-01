.text
.org 0
li $sp,8192
li $fp,8192
jal main
BREAK

main:
jr $ra // Return
la $7, const_0 // Load int
lw $3, ($7)
sw $3, -12($fp)
jr $ra // Return
lw $3, -12($fp) // Assign int
sw $3, -12($fp)
jr $ra // Return
la $7, const_1 // Load int
lw $3, ($7)
sw $3, -16($fp)
jr $ra // Return
lw $3, -16($fp) // Assign int
sw $3, -16($fp)
jr $ra // Return
lw $3, 0($fp) // Assign int
sw $3, 0($fp)
jr $ra // Return
lw $3, -4($fp) // Assign int
sw $3, -4($fp)
jr $ra // Return
lw $4, -20($fp) // ADD
lw $5, -24($fp)
add $3, $4, $5
sw $3, -28($fp)
jr $ra // Return
lw $3, -28($fp) // Assign int
sw $3, -28($fp)
jr $ra // Return
la $3, const_2 // Load string
sw $3, -32($fp)
jr $ra // Return
lw $3, -8($fp) // Assign int
sw $3, -8($fp)
jr $ra // Return
la $3, const_3 // Load string
sw $3, -40($fp)
jr $ra // Return
lw $9, -32($fp)
print_string $9
lw $9, -36($fp)
print_int $9
lw $9, -40($fp)
print_string $9
jr $ra // Return

.data
const_0:	.word 5
const_1:	.word 17
const_2:	.asciz "Vysledek je: \0"
const_3:	.asciz "!\0"

