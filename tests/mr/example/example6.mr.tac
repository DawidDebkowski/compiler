  JUMP main  # Entry Jump#Entry Jump
main:  # Main Program#Main Program
  READ _t0
  COPY n, _t0
  COPY _t1, 0
  COPY _t2, 5
  ADD _t3, _t2, 0
  STORE _t3, _t1
  COPY _t4, 1
  COPY _t5, 107
  ADD _t6, _t5, 0
  STORE _t6, _t4
  COPY _t7, 0
  COPY _t8, 209
  ADD _t9, _t8, 0
  STORE _t9, _t7
  COPY _t10, 1
  COPY _t11, 5
  ADD _t12, _t11, 1
  STORE _t12, _t10
  COPY _t13, 1
  COPY _t14, 107
  ADD _t15, _t14, 1
  STORE _t15, _t13
  COPY _t16, 1
  COPY _t17, 209
  ADD _t18, _t17, 1
  STORE _t18, _t16
  COPY _t19, 2
  COPY _t20, n
  COPY j, _t19  # For Init#For Init
  SUB _t21, _t20, _t19
  ADD _t21, _t21, 1
L0:
  JUMP_LEQ L1, _t21, 0
  COPY _t23, j
  COPY _t24, 1
  SUB _t22, _t23, _t24
  COPY k, _t22
  COPY _t26, k
  COPY _t27, 1
  SUB _t25, _t26, _t27
  COPY l, _t25
  COPY _t31, 209
  ADD _t32, _t31, k
  LOAD _t29, _t32
  COPY _t30, 1
  ADD _t28, _t29, _t30
  COPY _t33, 209
  ADD _t34, _t33, j
  STORE _t34, _t28
  COPY _t38, 5
  ADD _t39, _t38, k
  LOAD _t36, _t39
  COPY _t40, 5
  ADD _t41, _t40, l
  LOAD _t37, _t41
  ADD _t35, _t36, _t37
  COPY _t42, 5
  ADD _t43, _t42, j
  STORE _t43, _t35
  COPY _t47, 107
  ADD _t48, _t47, k
  LOAD _t45, _t48
  COPY _t49, 209
  ADD _t50, _t49, j
  LOAD _t46, _t50
  MUL _t44, _t45, _t46
  COPY _t51, 107
  ADD _t52, _t51, j
  STORE _t52, _t44
  SUB _t21, _t21, 1  # Dec count#Dec count
  ADD j, j, 1  # Inc iter#Inc iter
  JUMP L0
L1:  # For End#For End
  COPY _t54, 107
  ADD _t55, _t54, n
  LOAD _t53, _t55
  WRITE , _t53
  COPY _t57, 5
  ADD _t58, _t57, n
  LOAD _t56, _t58
  WRITE , _t56
  HALT   # HALT#HALT
