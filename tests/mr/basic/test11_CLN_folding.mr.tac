  JUMP main  # Entry Jump
proc_mult:  # Procedure mult
  PROLOGUE 
  LOAD _t3, a
  LOAD _t1, _t3
  LOAD _t4, b
  LOAD _t2, _t4
  MUL _t0, _t1, _t2
  LOAD _t5, o
  STORE _t5, _t0
  RETURN 
main:  # Main Program
  COPY _t7, 123
  COPY _t8, 124
  MUL _t6, _t7, _t8
  COPY a, _t6
  COPY _t9, 16
  COPY sixseven, _t9
  PARAM , 8
  PARAM , 11
  PARAM , 10
  CALL , proc_mult
  COPY _t10, c
  WRITE , _t10
  HALT   # HALT
