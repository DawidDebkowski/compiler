  JUMP main  # Entry Jump
main:  # Main Program
  COPY _t0, 5
  COPY a, _t0
  COPY _t1, 10
  COPY b, _t1
  COPY _t2, a
  WRITE , _t2
  COPY _t3, b
  WRITE , _t3
  COPY _t4, a
  COPY _t5, b
  JUMP_GEQ L2, _t4, _t5
  COPY _t6, 111
  WRITE , _t6
  JUMP L1
L2:
  COPY _t7, 0
  WRITE , _t7
L1:
  HALT   # HALT
