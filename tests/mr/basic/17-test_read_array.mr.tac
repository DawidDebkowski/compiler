  JUMP main  # Entry Jump
main:  # Main Program
  READ _t0
  COPY _t1, 0
  ADD _t2, _t1, 10
  STORE _t2, _t0
  COPY _t3, 11
  COPY a, _t3
  READ _t4
  COPY _t5, 0
  ADD _t6, _t5, a
  STORE _t6, _t4
  COPY _t8, 0
  ADD _t9, _t8, 10
  LOAD _t7, _t9
  WRITE , _t7
  COPY _t11, 0
  ADD _t12, _t11, 11
  LOAD _t10, _t12
  WRITE , _t10
  COPY _t14, 0
  ADD _t15, _t14, a
  LOAD _t13, _t15
  WRITE , _t13
  HALT   # HALT
