  JUMP main  # Entry Jump#Entry Jump
proc_gcd:  # Procedure gcd#Procedure gcd
  PROLOGUE 5
  LOAD _t0, a
  COPY x, _t0
  LOAD _t1, b
  COPY y, _t1
L0:  # While Start#While Start
  COPY _t2, y
  COPY _t3, 0
  JUMP_LEQ L1, _t2, _t3
  COPY _t4, x
  COPY _t5, y
  JUMP_LT L2, _t4, _t5
  COPY _t7, x
  COPY _t8, y
  SUB _t6, _t7, _t8
  COPY x, _t6
  JUMP L3
L2:
  COPY _t10, x
  COPY _t11, y
  ADD _t9, _t10, _t11
  COPY x, _t9
  COPY _t13, x
  COPY _t14, y
  SUB _t12, _t13, _t14
  COPY y, _t12
  COPY _t16, x
  COPY _t17, y
  SUB _t15, _t16, _t17
  COPY x, _t15
L3:
  JUMP L0
L1:  # While End#While End
  COPY _t18, x
  STORE c, _t18
  RETURN 5
main:  # Main Program#Main Program
  READ _t19
  COPY a, _t19
  READ _t20
  COPY b, _t20
  READ _t21
  COPY c, _t21
  READ _t22
  COPY d, _t22
  PARAM , 11
  PARAM , 12
  PARAM , 15
  CALL , proc_gcd
  PARAM , 13
  PARAM , 14
  PARAM , 16
  CALL , proc_gcd
  PARAM , 15
  PARAM , 16
  PARAM , 17
  CALL , proc_gcd
  COPY _t23, z
  WRITE , _t23
  HALT   # HALT#HALT
