.data
start_txt: .asciz "Введите число элементов(1-10): "
neg_overflow: .asciz "Отрицательное переполнение"
pos_overflow: .asciz "Положительное переполнение"
txt: .asciz "Введите число:"
ln: .asciz "\n"
sum: .asciz "Сумма чисел: "
count: .asciz "Количество сложенных элементов: "
element: .asciz"Последний элемент сложения: "
count_even_num: .asciz"Количество четных: "
count_not_even_num: .asciz "Количество нечетных: "
.align 2

array: .space 40
.text
main:
  la a0 start_txt
  li  a7, 4
  ecall
  li a7, 5
  ecall
  li t1, 1          
  blt a0, t1, main # если a0 < 1, перейти к метке main
  li t1, 10         
  bgt a0, t1, main # если a0 > 10, перейти к метке main
  mv t0 a0
  la t1 array
  li t2,0
  j work
work:
  beq t0,t2,end_work
  la a0,txt
  li a7,4
  ecall
  li a7,5
  ecall
  sw a0,(t1)
  addi t2,t2,1
  addi t1,t1,4
  j work
end_work:
 li t2, 0      # i = 0
 li t3, 0      # Сумма
 la t1, array
sum_elements:
 beq t0,t2,end_sum
 lw t5, (t1)
 bgtz t5,neg_ovfw
 bltz t5,pos_ovfw
 add t6, t3, t5
 mv t3,t6
 add s2,s2,t5
 addi t2, t2, 1
 addi t1, t1, 4
 j sum_elements
neg_ovfw:
 add t6, t3, t5
 blt t6, t3, neg_overflow_occurred
 mv t3,t6
 li s2, 0
 add s2,s2,t5
 addi t2, t2, 1
 addi t1, t1, 4
 j sum_elements
pos_ovfw:
 add t6, t3, t5
 bgt t6, t3, pos_overflow_occurred
 mv t3,t6
 li s2, 0
 add s2,s2,t5
 addi t2, t2, 1
 addi t1, t1, 4
 j sum_elements
neg_overflow_occurred:
  la a0 neg_overflow
  li  a7, 4
  ecall
  la a0 ln
  li a7 4
  ecall
  j end_sum
pos_overflow_occurred:
  la a0 pos_overflow
  li  a7, 4
  ecall
  la a0 ln
  li a7 4
  ecall
  j end_sum
  
end_sum:
 la a0, sum
 li a7,4
 ecall
 mv a0, t3
 li a7,1
 ecall
 la a0 ln
 li a7 4
 ecall
 la a0 count
 li a7 4
 ecall
 mv a0, t2
 li a7, 1
 ecall
 la a0 ln
 li a7 4
 ecall
 la a0 element
 li a7 4
 ecall
 mv a0, s2
 li s2, 0
 li a7, 1
 ecall
 la a0 ln
 li a7 4
 ecall
 j update

update:
 li t1,0
 li t2, 0      # i = 0
 li t3, 0      # количество четных
 li t4, 0      # количество нечетных
 la t5, array
 j count_number
count_number:
 beq t0,t2,end_count
 lw t1, (t5)
 andi t6, t1, 1
 beq t6, zero, even_number
 j else_number
even_number:
 addi t3,t3,1
 addi t2, t2, 1
 addi t5, t5, 4
 j count_number
else_number:
 addi t4,t4,1
 addi t2, t2, 1
 addi t5, t5, 4
 j count_number
end_count:
 la a0,count_even_num
 li a7,4
 ecall
 li a7,1
 mv a0, t3
 ecall
 la a0, ln
 li a7,4
 ecall
 la a0, count_not_even_num
 li a7,4
 ecall
 li a7,1
 mv a0, t4
 ecall
 j game_over
game_over:
 li a7,10
 ecall

#it was interesting :)

