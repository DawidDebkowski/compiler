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
  STORE _t4, _t0
  RETURN 
main:  # Main Program
  COPY _t5, 10
  COPY a, _t5
  COPY _t6, 15
  COPY b, _t6
  COPY _t7, 333
  COPY c, _t7
  COPY _t8, 0
  PARAM , _t8
  PARAM , 8
  PARAM , 17
  CALL , proc_set
  COPY _t9, 444
  COPY c, _t9
  COPY _t10, 0
  PARAM , _t10
  PARAM , 9
  PARAM , 17
  CALL , proc_set
  COPY _t12, 10
  COPY _t13, _t12
  ADD _t13, _t13, a
  SUB _t13, _t13, 10
  ADD _t13, _t13, 1
  LOAD _t11, _t13
  WRITE , _t11
  COPY _t15, 10
  COPY _t16, _t15
  ADD _t16, _t16, b
  SUB _t16, _t16, 10
  ADD _t16, _t16, 1
  LOAD _t14, _t16
  WRITE , _t14
  HALT   # HALT
