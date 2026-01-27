  JUMP main  # Entry Jump
proc_set:  # Procedure set
  PROLOGUE 
  LOAD _t1, val
  LOAD _t0, _t1
  LOAD _t2, idx
  LOAD _t2, _t2
  LOAD _t3, arr
  COPY _t4, _t3
  ADD _t4, _t4, _t2
  SUB _t4, _t4, 0
  ADD _t4, _t4, 1
  STORE _t4, _t0
  RETURN 
main:  # Main Program
  COPY _t5, 4611686018427387904
  COPY maxint, _t5
  COPY _t7, 4611686018427387904
  COPY _t8, 1
  ADD _t6, _t7, _t8
  COPY e, _t6
  COPY _t9, 111
  COPY c, _t9
  COPY _t10, 222
  COPY d, _t10
  READ _t11
  COPY b, _t11
  COPY _t12, 11
  PARAM , _t12
  PARAM , 10
  PARAM , 27
  CALL , proc_set
  COPY _t13, 11
  PARAM , _t13
  PARAM , 8
  PARAM , 28
  CALL , proc_set
  COPY _t15, 11
  COPY _t16, _t15
  ADD _t16, _t16, maxint
  SUB _t16, _t16, 4611686018427387904
  ADD _t16, _t16, 1
  LOAD _t14, _t16
  WRITE , _t14
  COPY _t18, 11
  COPY _t19, _t18
  ADD _t19, _t19, e
  SUB _t19, _t19, 4611686018427387904
  ADD _t19, _t19, 1
  LOAD _t17, _t19
  WRITE , _t17
  COPY _t20, 4611686018427387911
  COPY far, _t20
  COPY _t21, 333
  COPY _t22, 14
  COPY _t23, _t22
  ADD _t23, _t23, far
  SUB _t23, _t23, 4611686018427387910
  ADD _t23, _t23, 1
  STORE _t23, _t21
  COPY _t25, 14
  COPY _t26, _t25
  ADD _t26, _t26, far
  SUB _t26, _t26, 4611686018427387910
  ADD _t26, _t26, 1
  LOAD _t24, _t26
  WRITE , _t24
  COPY _t27, 14
  PARAM , _t27
  PARAM , 9
  PARAM , 26
  CALL , proc_set
  COPY _t29, 14
  COPY _t30, _t29
  ADD _t30, _t30, far
  SUB _t30, _t30, 4611686018427387910
  ADD _t30, _t30, 1
  LOAD _t28, _t30
  WRITE , _t28
  HALT   # HALT
