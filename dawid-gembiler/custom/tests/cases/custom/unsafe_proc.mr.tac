  JUMP main  # Entry Jump
proc_p:  # Procedure p
  PROLOGUE 5
  COPY _t0, 123
  COPY _t1, arr
  ADD _t2, _t1, 3000
  STORE _t2, _t0
  COPY _t4, arr
  ADD _t5, _t4, 3000
  LOAD _t3, _t5
  WRITE , _t3
  RETURN 5
main:  # Main Program
  COPY _t6, 7
  PARAM , _t6
  CALL , proc_p
  HALT   # HALT
