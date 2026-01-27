  JUMP main  # Entry Jump
main:  # Main Program
  COPY _t0, 10
  COPY i, _t0
  COPY _t1, 2
  COPY _t2, 3
  COPY i, _t1  # For Init
L0:
  JUMP_GT L1, i, _t2
  COPY _t3, i
  WRITE , _t3
  ADD i, i, 1  # Inc iter
  JUMP L0
L1:  # For End
  COPY _t4, i
  WRITE , _t4
  HALT   # HALT
