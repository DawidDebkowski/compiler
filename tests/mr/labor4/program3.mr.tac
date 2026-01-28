  JUMP main  # Entry Jump#Entry Jump
proc_check:  # Procedure check#Procedure check
  PROLOGUE 5
  COPY _t0, 0
  STORE p, _t0
  LOAD _t2, n
  LOAD _t3, d
  MOD _t1, _t2, _t3
  COPY r, _t1
L0:  # While Start#While Start
  COPY _t4, r
  COPY _t5, 0
  JUMP_NEQ L1, _t4, _t5
  LOAD _t7, n
  LOAD _t8, d
  DIV _t6, _t7, _t8
  STORE n, _t6
  LOAD _t10, p
  COPY _t11, 1
  ADD _t9, _t10, _t11
  STORE p, _t9
  LOAD _t13, n
  LOAD _t14, d
  MOD _t12, _t13, _t14
  COPY r, _t12
  JUMP L0
L1:  # While End#While End
  RETURN 5
main:  # Main Program#Main Program
  READ _t15
  COPY n, _t15
  COPY _t16, 2
  COPY dzielnik, _t16
  COPY _t18, dzielnik
  COPY _t19, dzielnik
  MUL _t17, _t18, _t19
  COPY m, _t17
L2:  # While Start#While Start
  COPY _t20, n
  COPY _t21, m
  JUMP_LT L3, _t20, _t21
  PARAM , 10
  PARAM , 13
  PARAM , 12
  CALL , proc_check
  COPY _t22, potega
  COPY _t23, 0
  JUMP_LEQ L4, _t22, _t23
  COPY _t24, dzielnik
  WRITE , _t24
  COPY _t25, potega
  WRITE , _t25
L4:
  COPY _t27, dzielnik
  COPY _t28, 1
  ADD _t26, _t27, _t28
  COPY dzielnik, _t26
  COPY _t30, dzielnik
  COPY _t31, dzielnik
  MUL _t29, _t30, _t31
  COPY m, _t29
  JUMP L2
L3:  # While End#While End
  COPY _t32, n
  COPY _t33, 1
  JUMP_EQ L5, _t32, _t33
  COPY _t34, n
  WRITE , _t34
  COPY _t35, 1
  WRITE , _t35
L5:
  HALT   # HALT#HALT
