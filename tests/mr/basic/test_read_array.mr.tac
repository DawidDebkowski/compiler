  JUMP main  # Entry Jump
main:  # Main Program
  READ _t0
  COPY _t1, 10
  COPY _t2, _t1
  ADD _t2, _t2, 10
  SUB _t2, _t2, 10
  ADD _t2, _t2, 1
  STORE _t2, _t0
  COPY _t3, 11
  COPY a, _t3
  READ _t4
  COPY _t5, 10
  COPY _t6, _t5
  ADD _t6, _t6, a
  SUB _t6, _t6, 10
  ADD _t6, _t6, 1
  STORE _t6, _t4
  COPY _t8, 10
  COPY _t9, _t8
  ADD _t9, _t9, 10
  SUB _t9, _t9, 10
  ADD _t9, _t9, 1
  LOAD _t7, _t9
  WRITE , _t7
  COPY _t11, 10
  COPY _t12, _t11
  ADD _t12, _t12, 11
  SUB _t12, _t12, 10
  ADD _t12, _t12, 1
  LOAD _t10, _t12
  WRITE , _t10
  COPY _t14, 10
  COPY _t15, _t14
  ADD _t15, _t15, a
  SUB _t15, _t15, 10
  ADD _t15, _t15, 1
  LOAD _t13, _t15
  WRITE , _t13
  HALT   # HALT
