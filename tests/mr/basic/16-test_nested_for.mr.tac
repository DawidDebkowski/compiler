  JUMP main  # Entry Jump
main:  # Main Program
  COPY _t0, 0
  COPY sum, _t0
  COPY _t1, 1
  COPY _t2, 2
  COPY i, _t1  # For Init
L0:
  JUMP_GT L1, i, _t2
  COPY _t3, 1
  COPY _t4, 2
  COPY i, _t3  # For Init
L2:
  JUMP_GT L3, i, _t4
  COPY _t5, 1
  COPY _t6, 2
  COPY i, _t5  # For Init
L4:
  JUMP_GT L5, i, _t6
  COPY _t7, 1
  COPY _t8, 2
  COPY i, _t7  # For Init
L6:
  JUMP_GT L7, i, _t8
  COPY _t9, 1
  COPY _t10, 2
  COPY i, _t9  # For Init
L8:
  JUMP_GT L9, i, _t10
  COPY _t11, 1
  COPY _t12, 2
  COPY i, _t11  # For Init
L10:
  JUMP_GT L11, i, _t12
  COPY _t13, 1
  COPY _t14, 2
  COPY i, _t13  # For Init
L12:
  JUMP_GT L13, i, _t14
  COPY _t15, 1
  COPY _t16, 2
  COPY i, _t15  # For Init
L14:
  JUMP_GT L15, i, _t16
  COPY _t17, 1
  COPY _t18, 2
  COPY i, _t17  # For Init
L16:
  JUMP_GT L17, i, _t18
  COPY _t19, 1
  COPY _t20, 2
  COPY i, _t19  # For Init
L18:
  JUMP_GT L19, i, _t20
  COPY _t22, sum
  COPY _t23, 1
  ADD _t21, _t22, _t23
  COPY sum, _t21
  ADD i, i, 1  # Inc iter
  JUMP L18
L19:  # For End
  ADD i, i, 1  # Inc iter
  JUMP L16
L17:  # For End
  ADD i, i, 1  # Inc iter
  JUMP L14
L15:  # For End
  ADD i, i, 1  # Inc iter
  JUMP L12
L13:  # For End
  ADD i, i, 1  # Inc iter
  JUMP L10
L11:  # For End
  ADD i, i, 1  # Inc iter
  JUMP L8
L9:  # For End
  ADD i, i, 1  # Inc iter
  JUMP L6
L7:  # For End
  ADD i, i, 1  # Inc iter
  JUMP L4
L5:  # For End
  ADD i, i, 1  # Inc iter
  JUMP L2
L3:  # For End
  ADD i, i, 1  # Inc iter
  JUMP L0
L1:  # For End
  COPY _t24, sum
  WRITE , _t24
  HALT   # HALT
