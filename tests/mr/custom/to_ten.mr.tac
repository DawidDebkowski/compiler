  JUMP main  # Entry Jump#Entry Jump
main:  # Main Program#Main Program
  COPY _t0, 1
  COPY _t1, 10
  COPY i, _t0  # For Init#For Init
  SUB _t2, _t1, _t0
  ADD _t2, _t2, 1
L0:
  JUMP_LEQ L1, _t2, 0
  COPY _t3, i
  WRITE , _t3
  SUB _t2, _t2, 1  # Dec count#Dec count
  ADD i, i, 1  # Inc iter#Inc iter
  JUMP L0
L1:  # For End#For End
  HALT   # HALT#HALT
