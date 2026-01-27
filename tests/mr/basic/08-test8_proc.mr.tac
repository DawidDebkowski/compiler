  JUMP main  # Entry Jump
proc_sum:  # Procedure sum
  PROLOGUE 5
  LOAD _t1, a
  LOAD _t2, b
  ADD _t0, _t1, _t2
  STORE out, _t0
  RETURN 5
main:  # Main Program
  COPY _t3, 10
  COPY x, _t3
  COPY _t4, 20
  COPY y, _t4
  PARAM , 9
  PARAM , 10
  PARAM , 11
  CALL , proc_sum
  COPY _t5, z
  WRITE , _t5
  HALT   # HALT
