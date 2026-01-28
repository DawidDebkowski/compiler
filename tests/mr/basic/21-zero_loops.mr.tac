  JUMP main  # Entry Jump#Entry Jump
main:  # Main Program#Main Program
  COPY _t0, 5
  COPY i, _t0
  COPY _t1, 10
  COPY _t2, 0
  COPY i, _t1  # For Init#For Init
  SUB _t3, _t1, _t2
  ADD _t3, _t3, 1
L0:
  JUMP_LEQ L1, _t3, 0
  COPY _t4, i
  WRITE , _t4
  SUB _t3, _t3, 1  # Dec count#Dec count
  SUB i, i, 1  # Dec iter#Dec iter
  JUMP L0
L1:  # For End#For End
  COPY _t5, i
  WRITE , _t5
  HALT   # HALT#HALT
