  JUMP main  # Entry Jump#Entry Jump
main:  # Main Program#Main Program
  COPY _t0, 24
  COPY n, _t0
  COPY _t1, n
  COPY _t2, 59
  ADD _t3, _t2, 0
  STORE _t3, _t1
  COPY _t5, n
  COPY _t7, 59
  ADD _t8, _t7, 0
  LOAD _t6, _t8
  SUB _t4, _t5, _t6
  COPY _t9, 59
  ADD _t10, _t9, n
  STORE _t10, _t4
  COPY _t14, 59
  ADD _t15, _t14, 0
  LOAD _t12, _t15
  COPY _t13, 1
  ADD _t11, _t12, _t13
  COPY j, _t11
  COPY _t17, 59
  ADD _t18, _t17, 0
  LOAD _t16, _t18
  COPY _t20, 59
  ADD _t21, _t20, n
  LOAD _t19, _t21
  COPY i, _t16  # For Init#For Init
  SUB _t22, _t16, _t19
  ADD _t22, _t22, 1
L0:
  JUMP_LEQ L1, _t22, 0
  COPY _t24, i
  COPY _t25, 1
  ADD _t23, _t24, _t25
  COPY _t26, 7
  ADD _t27, _t26, i
  STORE _t27, _t23
  COPY _t29, j
  COPY _t30, i
  SUB _t28, _t29, _t30
  COPY _t31, 33
  ADD _t32, _t31, i
  STORE _t32, _t28
  SUB _t22, _t22, 1  # Dec count#Dec count
  SUB i, i, 1  # Dec iter#Dec iter
  JUMP L0
L1:  # For End#For End
  COPY _t34, 59
  ADD _t35, _t34, n
  LOAD _t33, _t35
  COPY j, _t33
  COPY _t37, 59
  ADD _t38, _t37, n
  LOAD _t36, _t38
  COPY _t40, 59
  ADD _t41, _t40, 0
  LOAD _t39, _t41
  COPY i, _t36  # For Init#For Init
  SUB _t42, _t39, _t36
  ADD _t42, _t42, 1
L2:
  JUMP_LEQ L3, _t42, 0
  COPY _t46, 7
  ADD _t47, _t46, i
  LOAD _t44, _t47
  COPY _t48, 33
  ADD _t49, _t48, i
  LOAD _t45, _t49
  MUL _t43, _t44, _t45
  COPY _t50, 59
  ADD _t51, _t50, i
  STORE _t51, _t43
  SUB _t42, _t42, 1  # Dec count#Dec count
  ADD i, i, 1  # Inc iter#Inc iter
  JUMP L2
L3:  # For End#For End
  COPY _t52, 0
  COPY _t53, n
  COPY i, _t52  # For Init#For Init
  SUB _t54, _t53, _t52
  ADD _t54, _t54, 1
L4:
  JUMP_LEQ L5, _t54, 0
  COPY _t56, 59
  ADD _t57, _t56, i
  LOAD _t55, _t57
  WRITE , _t55
  SUB _t54, _t54, 1  # Dec count#Dec count
  ADD i, i, 1  # Inc iter#Inc iter
  JUMP L4
L5:  # For End#For End
  HALT   # HALT#HALT
