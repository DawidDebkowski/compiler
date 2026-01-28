  JUMP main  # Entry Jump#Entry Jump
main:  # Main Program#Main Program
  READ _t0
  COPY n, _t0
L0:  # Repeat Start#Repeat Start
  COPY _t2, n
  COPY _t3, 2
  DIV _t1, _t2, _t3
  COPY p, _t1
  COPY _t5, 2
  COPY _t6, p
  MUL _t4, _t5, _t6
  COPY p, _t4
  COPY _t7, n
  COPY _t8, p
  JUMP_LEQ L1, _t7, _t8
  COPY _t9, 1
  WRITE , _t9
  JUMP L2
L1:
  COPY _t10, 0
  WRITE , _t10
L2:
  COPY _t12, n
  COPY _t13, 2
  DIV _t11, _t12, _t13
  COPY n, _t11
  COPY _t14, n
  COPY _t15, 0
  JUMP_NEQ L0, _t14, _t15
  HALT   # HALT#HALT
