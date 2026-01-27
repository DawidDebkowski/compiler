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
  LOAD _t8, val
  LOAD _t6, _t8
  COPY _t7, 2
  MUL _t5, _t6, _t7
  LOAD _t9, val
  STORE _t9, _t5
  RETURN 
main:  # Main Program
  COPY _t10, 0
  COPY z, _t10
  COPY _t11, 5
  COPY f, _t11
  COPY _t12, 111
  COPY one, _t12
  COPY _t13, 8
  PARAM , _t13
  PARAM , 15
  PARAM , 17
  CALL , proc_set
  COPY _t14, 8
  PARAM , _t14
  PARAM , 16
  PARAM , 17
  CALL , proc_set
  COPY _t16, 8
  COPY _t17, _t16
  ADD _t17, _t17, 0
  SUB _t17, _t17, 0
  ADD _t17, _t17, 1
  LOAD _t15, _t17
  WRITE , _t15
  COPY _t19, 8
  COPY _t20, _t19
  ADD _t20, _t20, 5
  SUB _t20, _t20, 0
  ADD _t20, _t20, 1
  LOAD _t18, _t20
  WRITE , _t18
  HALT   # HALT
