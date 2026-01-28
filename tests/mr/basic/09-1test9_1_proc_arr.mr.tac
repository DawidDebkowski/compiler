  JUMP main  # Entry Jump#Entry Jump
proc_set:  # Procedure set#Procedure set
  PROLOGUE 5
  LOAD _t0, val
  LOAD _t1, idx
  COPY _t4, arr
  LOAD _t5, _t4
  COPY _t2, _t4
  ADD _t2, _t2, 1
  ADD _t2, _t2, _t1
  SUB _t3, _t2, _t5  # unsafe assign#unsafe assign
  STORE _t3, _t0
  RETURN 5
main:  # Main Program#Main Program
  COPY _t6, 4611686018427387910
  COPY _t7, 15
  STORE _t7, _t6  # init unsafe header a#init unsafe header a
  COPY _t8, 4611686018427387904
  COPY _t9, 12
  STORE _t9, _t8  # init unsafe header tab#init unsafe header tab
  COPY _t10, 4611686018427387904
  COPY maxint, _t10
  COPY _t12, 4611686018427387904
  COPY _t13, 1
  ADD _t11, _t12, _t13
  COPY e, _t11
  COPY _t14, 111
  COPY c, _t14
  COPY _t15, 222
  COPY d, _t15
  READ _t16
  COPY b, _t16
  COPY _t17, 12
  PARAM , _t17
  PARAM , 11
  PARAM , 28
  CALL , proc_set
  COPY _t18, 12
  PARAM , _t18
  PARAM , 9
  PARAM , 29
  CALL , proc_set
  COPY _t20, -4611686018427387892
  ADD _t21, _t20, maxint
  LOAD _t19, _t21
  WRITE , _t19
  COPY _t23, -4611686018427387892
  ADD _t24, _t23, e
  LOAD _t22, _t24
  WRITE , _t22
  COPY _t25, 4611686018427387911
  COPY far, _t25
  COPY _t26, 333
  COPY _t27, -4611686018427387895
  ADD _t28, _t27, far
  STORE _t28, _t26
  COPY _t30, -4611686018427387895
  ADD _t31, _t30, far
  LOAD _t29, _t31
  WRITE , _t29
  COPY _t32, 15
  PARAM , _t32
  PARAM , 10
  PARAM , 27
  CALL , proc_set
  COPY _t34, -4611686018427387895
  ADD _t35, _t34, far
  LOAD _t33, _t35
  WRITE , _t33
  HALT   # HALT#HALT
