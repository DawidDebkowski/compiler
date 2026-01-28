  JUMP main  # Entry Jump#Entry Jump
main:  # Main Program#Main Program
  COPY _t0, 10
  COPY _t1, 0
  COPY i, _t0  # For Init#For Init
  SUB _t2, _t0, _t1
  ADD _t2, _t2, 1
L0:
  JUMP_LEQ L1, _t2, 0
  COPY _t3, i
  WRITE , _t3
  SUB _t2, _t2, 1  # Dec count#Dec count
  SUB i, i, 1  # Dec iter#Dec iter
  JUMP L0
L1:  # For End#For End
  HALT   # HALT#HALT
