  JUMP main  # Entry Jump#Entry Jump
main:  # Main Program#Main Program
  COPY _t0, 10
  COPY i, _t0
  COPY _t1, 2
  COPY _t2, 3
  COPY i, _t1  # For Init#For Init
  SUB _t3, _t2, _t1
  ADD _t3, _t3, 1
L0:
  JUMP_LEQ L1, _t3, 0
  COPY _t4, i
  WRITE , _t4
  SUB _t3, _t3, 1  # Dec count#Dec count
  ADD i, i, 1  # Inc iter#Inc iter
  JUMP L0
L1:  # For End#For End
  COPY _t5, i
  WRITE , _t5
  HALT   # HALT#HALT
