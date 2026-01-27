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
  COPY _t4, 10
  COPY a, _t4
  COPY _t5, 15
  COPY b, _t5
  COPY _t6, 333
  COPY c, _t6
  COPY _t7, 0
  PARAM , _t7
  PARAM , 9
  PARAM , 18
  CALL , proc_set
  COPY _t8, 444
  COPY c, _t8
  COPY _t9, 0
  PARAM , _t9
  PARAM , 17
  PARAM , 18
  CALL , proc_set
  COPY _t11, 0
  ADD _t12, _t11, a
  LOAD _t10, _t12
  WRITE , _t10
  COPY _t14, 0
  ADD _t15, _t14, b
  LOAD _t13, _t15
  WRITE , _t13
  HALT   # HALT
