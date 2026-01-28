  JUMP main  # Entry Jump#Entry Jump
main:  # Main Program#Main Program
  COPY _t0, 0
  COPY sum, _t0
  COPY _t1, 1
  COPY _t2, 2
  COPY i, _t1  # For Init#For Init
  SUB _t3, _t2, _t1
  ADD _t3, _t3, 1
L0:
  JUMP_LEQ L1, _t3, 0
  COPY _t4, 1
  COPY _t5, 2
  COPY i, _t4  # For Init#For Init
  SUB _t6, _t5, _t4
  ADD _t6, _t6, 1
L2:
  JUMP_LEQ L3, _t6, 0
  COPY _t7, 1
  COPY _t8, 2
  COPY i, _t7  # For Init#For Init
  SUB _t9, _t8, _t7
  ADD _t9, _t9, 1
L4:
  JUMP_LEQ L5, _t9, 0
  COPY _t10, 1
  COPY _t11, 2
  COPY i, _t10  # For Init#For Init
  SUB _t12, _t11, _t10
  ADD _t12, _t12, 1
L6:
  JUMP_LEQ L7, _t12, 0
  COPY _t13, 1
  COPY _t14, 2
  COPY i, _t13  # For Init#For Init
  SUB _t15, _t14, _t13
  ADD _t15, _t15, 1
L8:
  JUMP_LEQ L9, _t15, 0
  COPY _t16, 1
  COPY _t17, 2
  COPY i, _t16  # For Init#For Init
  SUB _t18, _t17, _t16
  ADD _t18, _t18, 1
L10:
  JUMP_LEQ L11, _t18, 0
  COPY _t19, 1
  COPY _t20, 2
  COPY i, _t19  # For Init#For Init
  SUB _t21, _t20, _t19
  ADD _t21, _t21, 1
L12:
  JUMP_LEQ L13, _t21, 0
  COPY _t22, 1
  COPY _t23, 2
  COPY i, _t22  # For Init#For Init
  SUB _t24, _t23, _t22
  ADD _t24, _t24, 1
L14:
  JUMP_LEQ L15, _t24, 0
  COPY _t25, 1
  COPY _t26, 2
  COPY i, _t25  # For Init#For Init
  SUB _t27, _t26, _t25
  ADD _t27, _t27, 1
L16:
  JUMP_LEQ L17, _t27, 0
  COPY _t28, 1
  COPY _t29, 2
  COPY i, _t28  # For Init#For Init
  SUB _t30, _t29, _t28
  ADD _t30, _t30, 1
L18:
  JUMP_LEQ L19, _t30, 0
  COPY _t32, sum
  COPY _t33, 1
  ADD _t31, _t32, _t33
  COPY sum, _t31
  SUB _t30, _t30, 1  # Dec count#Dec count
  ADD i, i, 1  # Inc iter#Inc iter
  JUMP L18
L19:  # For End#For End
  SUB _t27, _t27, 1  # Dec count#Dec count
  ADD i, i, 1  # Inc iter#Inc iter
  JUMP L16
L17:  # For End#For End
  SUB _t24, _t24, 1  # Dec count#Dec count
  ADD i, i, 1  # Inc iter#Inc iter
  JUMP L14
L15:  # For End#For End
  SUB _t21, _t21, 1  # Dec count#Dec count
  ADD i, i, 1  # Inc iter#Inc iter
  JUMP L12
L13:  # For End#For End
  SUB _t18, _t18, 1  # Dec count#Dec count
  ADD i, i, 1  # Inc iter#Inc iter
  JUMP L10
L11:  # For End#For End
  SUB _t15, _t15, 1  # Dec count#Dec count
  ADD i, i, 1  # Inc iter#Inc iter
  JUMP L8
L9:  # For End#For End
  SUB _t12, _t12, 1  # Dec count#Dec count
  ADD i, i, 1  # Inc iter#Inc iter
  JUMP L6
L7:  # For End#For End
  SUB _t9, _t9, 1  # Dec count#Dec count
  ADD i, i, 1  # Inc iter#Inc iter
  JUMP L4
L5:  # For End#For End
  SUB _t6, _t6, 1  # Dec count#Dec count
  ADD i, i, 1  # Inc iter#Inc iter
  JUMP L2
L3:  # For End#For End
  SUB _t3, _t3, 1  # Dec count#Dec count
  ADD i, i, 1  # Inc iter#Inc iter
  JUMP L0
L1:  # For End#For End
  COPY _t34, sum
  WRITE , _t34
  HALT   # HALT#HALT
