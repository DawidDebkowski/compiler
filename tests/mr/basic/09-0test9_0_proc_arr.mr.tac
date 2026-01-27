  JUMP main  # Entry Jump
proc_set:  # Procedure set
  PROLOGUE 5
  LOAD _t0, val
  LOAD _t1, idx
  COPY _t2, arr
  ADD _t3, _t2, _t1
  STORE _t3, _t0
  LOAD _t5, val
  COPY _t6, 2
  MUL _t4, _t5, _t6
  STORE val, _t4
  RETURN 5
main:  # Main Program
  COPY _t7, 0
  COPY z, _t7
  COPY _t8, 5
  COPY f, _t8
  COPY _t9, 111
  COPY one, _t9
  COPY _t10, 9
  PARAM , _t10
  PARAM , 16
  PARAM , 18
  CALL , proc_set
  COPY _t11, 9
  PARAM , _t11
  PARAM , 17
  PARAM , 18
  CALL , proc_set
  COPY _t13, 9
  ADD _t14, _t13, 0
  LOAD _t12, _t14
  WRITE , _t12
  COPY _t16, 9
  ADD _t17, _t16, 5
  LOAD _t15, _t17
  WRITE , _t15
  HALT   # HALT
