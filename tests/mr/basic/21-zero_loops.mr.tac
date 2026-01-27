  JUMP main  # Entry Jump
main:  # Main Program
  COPY _t0, 5
  COPY i, _t0
  COPY _t1, 10
  COPY _t2, 0
  COPY i, _t1  # For Init
L0:
  JUMP_LT L1, i, _t2
  COPY _t3, i
  WRITE , _t3
  SUB i, i, 1  # Dec iter
  JUMP L0
L1:  # For End
  COPY _t4, i
  WRITE , _t4
  HALT   # HALT
