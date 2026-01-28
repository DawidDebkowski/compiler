  JUMP main  # Entry Jump#Entry Jump
main:  # Main Program#Main Program
  COPY _t0, 1
  COPY x, _t0
  COPY _t1, 2
  COPY y, _t1
  COPY _t2, 3
  COPY z, _t2
  COPY _t3, x
  COPY _t4, 1
  JUMP_NEQ L0, _t3, _t4
  COPY _t5, 1
  WRITE , _t5
  COPY _t6, y
  COPY _t7, 2
  JUMP_NEQ L2, _t6, _t7
  COPY _t8, 2
  WRITE , _t8
  COPY _t9, z
  COPY _t10, 3
  JUMP_NEQ L4, _t9, _t10
  COPY _t11, 3
  WRITE , _t11
  JUMP L5
L4:
  COPY _t12, 33
  WRITE , _t12
L5:
  JUMP L3
L2:
  COPY _t13, 22
  WRITE , _t13
L3:
  JUMP L1
L0:
  COPY _t14, 11
  WRITE , _t14
L1:
  COPY _t15, x
  COPY _t16, 1
  JUMP_EQ L6, _t15, _t16
  COPY _t17, 44
  WRITE , _t17
  JUMP L7
L6:
  COPY _t18, 4
  WRITE , _t18
  COPY _t19, y
  COPY _t20, 2
  JUMP_EQ L8, _t19, _t20
  COPY _t21, 55
  WRITE , _t21
  JUMP L9
L8:
  COPY _t22, 5
  WRITE , _t22
  COPY _t23, z
  COPY _t24, 3
  JUMP_EQ L10, _t23, _t24
  COPY _t25, 66
  WRITE , _t25
  JUMP L11
L10:
  COPY _t26, 6
  WRITE , _t26
L11:
L9:
L7:
  HALT   # HALT#HALT
