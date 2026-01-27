  JUMP main  # Entry Jump
main:  # Main Program
  COPY _t0, 100
  COPY _t1, 0
  ADD _t2, _t1, 10
  STORE _t2, _t0
  COPY _t3, 150
  COPY _t4, 0
  ADD _t5, _t4, 15
  STORE _t5, _t3
  COPY _t7, 0
  ADD _t8, _t7, 10
  LOAD _t6, _t8
  WRITE , _t6
  COPY _t10, 0
  ADD _t11, _t10, 15
  LOAD _t9, _t11
  WRITE , _t9
  HALT   # HALT
