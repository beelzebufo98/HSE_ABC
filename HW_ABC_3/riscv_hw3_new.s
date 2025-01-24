.data
    divisor: .asciz "������� �������: "
    divider: .asciz "������� ��������: "  
    ln:     .asciz "\n"
    error_zero: .asciz "������: ������� �� ����!\n"
    quotient_msg: .asciz "�������: "
    remainder_msg: .asciz "�������: "
.text
    la a0, divisor
    li a7, 4
    ecall
    li a7, 5
    ecall
    mv t0, a0

    la a0, divider
    li a7, 4
    ecall
    li a7, 5
    ecall
    mv t1, a0

    beqz t1, division_by_zero

    li t2, 0               # ���� �������� (0 - �������������, 1 - �������������)
    li t3, 0               # ���� �������� (0 - �������������, 1 - �������������)
    li t4, 0               # �������
    li t5, 0               # �������

    bltz t0, make_pos_dividend
    j check_divisor

make_pos_dividend:
    neg t0, t0             # ���� ������� �������������, ������ ��� �������������
    li t2, 1               # �������� ���� �������� ��� �������������
    j check_divisor

make_pos_divider:
    neg t1, t1             # ���� �������� �������������, ������ ��� �������������
    li t3, 1               # �������� ���� �������� ��� �������������
    j start_division

check_divisor:
    bltz t1, make_pos_divider  # ��������� ���� ��������
    j start_division

start_division:
    mv t5, t0               # �������� ������� � t5 ��� ���������� �������
    j division_loop

division_loop:
    blt t5, t1, end_division  # ����� �� �����, ���� ������� ������ ��������
    sub t5, t5, t1            
    addi t4, t4, 1            
    j division_loop 

end_division:
    xor t6, t2, t3            # ���� ����� �������� � �������� ������, ������� ����� �������������
    beqz t6, skip_negate_quotient
    neg t4, t4
    j adjust_remainder

skip_negate_quotient:
    beqz t2, adjust_remainder

adjust_remainder:
    beqz t2, finalize_result
    neg t5, t5

finalize_result:
    j output_result

division_by_zero:
    la a0, error_zero
    li a7, 4
    ecall
    li a7, 10
    ecall

output_result:
    # ������� �������
    la a0, quotient_msg
    li a7, 4
    ecall

    mv a0, t4
    li a7, 1
    ecall

    # �������� ������� ������
    la a0, ln
    li a7, 4
    ecall

    # ������� �������
    la a0, remainder_msg
    li a7, 4
    ecall

    mv a0, t5
    li a7, 1
    ecall

    # �������� ������� ������
    la a0, ln
    li a7, 4
    ecall

    # ��������� ���������
    li a7, 10
    ecall
