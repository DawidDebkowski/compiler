  JUMP main  # Entry Jump
main:  # Main Program
  COPY _t0, 5
  COPY _t1, 5
  COPY _t2, _t1
  ADD _t2, _t2, 0
  SUB _t2, _t2, 0
  ADD _t2, _t2, 1
  STORE _t2, _t0
  COPY _t3, 10
  COPY _t4, 5
  COPY _t5, _t4
  ADD _t5, _t5, 5
  SUB _t5, _t5, 0
  ADD _t5, _t5, 1
  STORE _t5, _t3
  COPY _t7, 5
  COPY _t8, _t7
  ADD _t8, _t8, 0
  SUB _t8, _t8, 0
  ADD _t8, _t8, 1
  LOAD _t6, _t8
  WRITE , _t6
  COPY _t10, 5
  COPY _t11, _t10
  ADD _t11, _t11, 5
  SUB _t11, _t11, 0
  ADD _t11, _t11, 1
  LOAD _t9, _t11
  WRITE , _t9
  HALT   # HALT
