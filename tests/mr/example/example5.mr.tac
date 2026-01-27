  JUMP main  # Entry Jump
proc_power:  # Procedure power
  PROLOGUE 5
  COPY _t0, 1
  STORE d, _t0
  LOAD _t1, b
  COPY wyk, _t1
  LOAD _t3, a
  LOAD _t4, c
  MOD _t2, _t3, _t4
  COPY pot, _t2
L0:  # While Start
  COPY _t5, wyk
  COPY _t6, 0
  JUMP_LEQ L1, _t5, _t6
  COPY _t8, wyk
  COPY _t9, 2
  MOD _t7, _t8, _t9
  COPY o, _t7
  COPY _t10, o
  COPY _t11, 1
  JUMP_NEQ L3, _t10, _t11
  LOAD _t13, d
  COPY _t14, pot
  MUL _t12, _t13, _t14
  STORE d, _t12
  LOAD _t16, d
  LOAD _t17, c
  MOD _t15, _t16, _t17
  STORE d, _t15
L3:
  COPY _t19, wyk
  COPY _t20, 2
  DIV _t18, _t19, _t20
  COPY wyk, _t18
  COPY _t22, pot
  COPY _t23, pot
  MUL _t21, _t22, _t23
  COPY pot, _t21
  COPY _t25, pot
  LOAD _t26, c
  MOD _t24, _t25, _t26
  COPY pot, _t24
  JUMP L0
L1:  # While End
  RETURN 5
main:  # Main Program
  READ _t27
  COPY a, _t27
  READ _t28
  COPY b, _t28
  READ _t29
  COPY c, _t29
  PARAM , 13
  PARAM , 14
  PARAM , 15
  PARAM , 16
  CALL , proc_power
  COPY _t30, d
  WRITE , _t30
  HALT   # HALT
