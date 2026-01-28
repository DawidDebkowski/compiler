  JUMP main  # Entry Jump#Entry Jump
main:  # Main Program#Main Program
  COPY _t0, 1
  COPY i, _t0
  COPY _t1, 5
  COPY _t2, 4
  ADD _t3, _t2, i
  STORE _t3, _t1
  COPY _t5, 4
  ADD _t6, _t5, i
  LOAD _t4, _t6
  WRITE , _t4
  COPY _t8, 4
  ADD _t9, _t8, 1
  LOAD _t7, _t9
  WRITE , _t7
  COPY _t10, 2
  COPY i, _t10
  COPY _t11, 7
  COPY _t12, 4
  ADD _t13, _t12, i
  STORE _t13, _t11
  COPY _t15, 4
  ADD _t16, _t15, i
  LOAD _t14, _t16
  WRITE , _t14
  COPY _t18, 4
  ADD _t19, _t18, 2
  LOAD _t17, _t19
  WRITE , _t17
  COPY _t20, 3
  COPY i, _t20
  COPY _t24, 4
  ADD _t25, _t24, 1
  LOAD _t22, _t25
  COPY _t26, 4
  ADD _t27, _t26, 2
  LOAD _t23, _t27
  ADD _t21, _t22, _t23
  COPY _t28, 4
  ADD _t29, _t28, i
  STORE _t29, _t21
  COPY _t31, 4
  ADD _t32, _t31, i
  LOAD _t30, _t32
  WRITE , _t30
  COPY _t34, 4
  ADD _t35, _t34, 3
  LOAD _t33, _t35
  WRITE , _t33
  HALT   # HALT#HALT
