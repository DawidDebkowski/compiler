  JUMP main  # Entry Jump
proc_set:  # Procedure set
  PROLOGUE 5
  LOAD _t0, val
  LOAD _t1, idx
  COPY _t2, arr
  ADD _t3, _t2, _t1
  STORE _t3, _t0
  RETURN 5
main:  # Main Program
  COPY _t4, 4611686018427387904
  COPY maxint, _t4
  COPY _t6, 4611686018427387904
  COPY _t7, 1
  ADD _t5, _t6, _t7
  COPY e, _t5
  COPY _t8, 111
  COPY c, _t8
  COPY _t9, 222
  COPY d, _t9
  READ _t10
  COPY b, _t10
  COPY _t11, 12
  PARAM , _t11
  PARAM , 11
  PARAM , 28
  CALL , proc_set
  COPY _t12, 12
  PARAM , _t12
  PARAM , 9
  PARAM , 29
  CALL , proc_set
  COPY _t14, -4611686018427387892
  ADD _t15, _t14, maxint
  LOAD _t13, _t15
  WRITE , _t13
  COPY _t17, -4611686018427387892
  ADD _t18, _t17, e
  LOAD _t16, _t18
  WRITE , _t16
  COPY _t19, 4611686018427387911
  COPY far, _t19
  COPY _t20, 333
  COPY _t21, -4611686018427387895
  ADD _t22, _t21, far
  STORE _t22, _t20
  COPY _t24, -4611686018427387895
  ADD _t25, _t24, far
  LOAD _t23, _t25
  WRITE , _t23
  COPY _t26, 15
  PARAM , _t26
  PARAM , 10
  PARAM , 27
  CALL , proc_set
  COPY _t28, -4611686018427387895
  ADD _t29, _t28, far
  LOAD _t27, _t29
  WRITE , _t27
  HALT   # HALT
