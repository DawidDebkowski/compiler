  JUMP main  # Entry Jump#Entry Jump
main:  # Main Program#Main Program
  COPY _t0, 5
  COPY _t1, 5
  ADD _t2, _t1, 0
  STORE _t2, _t0
  COPY _t3, 10
  COPY _t4, 5
  ADD _t5, _t4, 5
  STORE _t5, _t3
  COPY _t7, 5
  ADD _t8, _t7, 0
  LOAD _t6, _t8
  WRITE , _t6
  COPY _t10, 5
  ADD _t11, _t10, 5
  LOAD _t9, _t11
  WRITE , _t9
  HALT   # HALT#HALT
