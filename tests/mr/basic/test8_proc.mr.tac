  JUMP main  # Entry Jump
proc_sum:  # Procedure sum
  PROLOGUE 
  LOAD _t3, a
  LOAD _t1, _t3
  LOAD _t4, b
  LOAD _t2, _t4
  ADD _t0, _t1, _t2
  LOAD _t5, out
  STORE _t5, _t0
  RETURN 
main:  # Main Program
  COPY _t6, 10
  COPY x, _t6
  COPY _t7, 20
  COPY y, _t7
  PARAM , 8
  PARAM , 9
  PARAM , 10
  CALL , proc_sum
  COPY _t8, z
  WRITE , _t8
  HALT   # HALT
