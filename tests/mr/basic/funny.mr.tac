  JUMP main  # Entry Jump
main:  # Main Program
  COPY _t0, 1
  COPY i, _t0
  COPY _t1, 5
  COPY _t2, 5
  COPY _t3, _t2
  ADD _t3, _t3, i
  SUB _t3, _t3, 1
  ADD _t3, _t3, 1
  STORE _t3, _t1
  COPY _t5, 5
  COPY _t6, _t5
  ADD _t6, _t6, i
  SUB _t6, _t6, 1
  ADD _t6, _t6, 1
  LOAD _t4, _t6
  WRITE , _t4
  COPY _t8, 5
  COPY _t9, _t8
  ADD _t9, _t9, 1
  SUB _t9, _t9, 1
  ADD _t9, _t9, 1
  LOAD _t7, _t9
  WRITE , _t7
  COPY _t10, 2
  COPY i, _t10
  COPY _t11, 7
  COPY _t12, 5
  COPY _t13, _t12
  ADD _t13, _t13, i
  SUB _t13, _t13, 1
  ADD _t13, _t13, 1
  STORE _t13, _t11
  COPY _t15, 5
  COPY _t16, _t15
  ADD _t16, _t16, i
  SUB _t16, _t16, 1
  ADD _t16, _t16, 1
  LOAD _t14, _t16
  WRITE , _t14
  COPY _t18, 5
  COPY _t19, _t18
  ADD _t19, _t19, 2
  SUB _t19, _t19, 1
  ADD _t19, _t19, 1
  LOAD _t17, _t19
  WRITE , _t17
  COPY _t20, 3
  COPY i, _t20
  COPY _t24, 5
  COPY _t25, _t24
  ADD _t25, _t25, 1
  SUB _t25, _t25, 1
  ADD _t25, _t25, 1
  LOAD _t22, _t25
  COPY _t26, 5
  COPY _t27, _t26
  ADD _t27, _t27, 2
  SUB _t27, _t27, 1
  ADD _t27, _t27, 1
  LOAD _t23, _t27
  ADD _t21, _t22, _t23
  COPY _t28, 5
  COPY _t29, _t28
  ADD _t29, _t29, i
  SUB _t29, _t29, 1
  ADD _t29, _t29, 1
  STORE _t29, _t21
  COPY _t31, 5
  COPY _t32, _t31
  ADD _t32, _t32, i
  SUB _t32, _t32, 1
  ADD _t32, _t32, 1
  LOAD _t30, _t32
  WRITE , _t30
  COPY _t34, 5
  COPY _t35, _t34
  ADD _t35, _t35, 3
  SUB _t35, _t35, 1
  ADD _t35, _t35, 1
  LOAD _t33, _t35
  WRITE , _t33
  HALT   # HALT
