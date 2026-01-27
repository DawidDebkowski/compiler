  JUMP main  # Entry Jump
proc_sum:  # Procedure sum
  PROLOGUE 5
  LOAD _t0, a
  WRITE , _t0
  LOAD _t1, b
  WRITE , _t1
  LOAD _t2, out
  WRITE , _t2
  LOAD _t4, a
  LOAD _t5, b
  ADD _t3, _t4, _t5
  STORE out, _t3
  LOAD _t6, a
  WRITE , _t6
  LOAD _t7, b
  WRITE , _t7
  LOAD _t8, out
  WRITE , _t8
  RETURN 5
main:  # Main Program
  COPY _t9, 10
  COPY x, _t9
  COPY _t10, x
  WRITE , _t10
  COPY _t11, 20
  COPY y, _t11
  COPY _t12, y
  WRITE , _t12
  PARAM , 9
  PARAM , 10
  PARAM , 11
  CALL , proc_sum
  COPY _t13, z
  WRITE , _t13
  HALT   # HALT
