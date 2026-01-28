  JUMP main  # Entry Jump#Entry Jump
proc_shuffle:  # Procedure shuffle#Procedure shuffle
  PROLOGUE 5
  COPY _t0, 5
  COPY q, _t0
  COPY _t1, 1
  COPY w, _t1
  COPY _t2, 1
  LOAD _t3, n
  COPY i, _t2  # For Init#For Init
  SUB _t4, _t3, _t2
  ADD _t4, _t4, 1
L0:
  JUMP_LEQ L1, _t4, 0
  COPY _t6, w
  COPY _t7, q
  MUL _t5, _t6, _t7
  COPY w, _t5
  COPY _t9, w
  LOAD _t10, n
  MOD _t8, _t9, _t10
  COPY w, _t8
  COPY _t11, w
  COPY _t12, t
  ADD _t13, _t12, i
  STORE _t13, _t11
  SUB _t4, _t4, 1  # Dec count#Dec count
  ADD i, i, 1  # Inc iter#Inc iter
  JUMP L0
L1:  # For End#For End
  COPY _t14, 0
  LOAD _t15, n
  COPY _t16, t
  ADD _t17, _t16, _t15
  STORE _t17, _t14
  RETURN 5
proc_sort:  # Procedure sort#Procedure sort
  PROLOGUE 10
  COPY _t18, 2
  LOAD _t19, n
  COPY i, _t18  # For Init#For Init
  SUB _t20, _t19, _t18
  ADD _t20, _t20, 1
L2:
  JUMP_LEQ L3, _t20, 0
  COPY _t22, t
  ADD _t23, _t22, i
  LOAD _t21, _t23
  COPY x, _t21
  COPY _t24, i
  COPY j, _t24
L4:  # While Start#While Start
  COPY _t25, j
  COPY _t26, 1
  JUMP_LEQ L5, _t25, _t26
  COPY _t28, j
  COPY _t29, 1
  SUB _t27, _t28, _t29
  COPY k, _t27
  COPY _t32, t
  ADD _t33, _t32, k
  LOAD _t30, _t33
  COPY _t31, x
  JUMP_LEQ L6, _t30, _t31
  COPY _t35, t
  ADD _t36, _t35, k
  LOAD _t34, _t36
  COPY _t37, t
  ADD _t38, _t37, j
  STORE _t38, _t34
  COPY _t40, j
  COPY _t41, 1
  SUB _t39, _t40, _t41
  COPY j, _t39
  JUMP L7
L6:
  COPY _t42, j
  COPY k, _t42
  COPY _t43, 1
  COPY j, _t43
L7:
  JUMP L4
L5:  # While End#While End
  COPY _t44, x
  COPY _t45, t
  ADD _t46, _t45, k
  STORE _t46, _t44
  SUB _t20, _t20, 1  # Dec count#Dec count
  ADD i, i, 1  # Inc iter#Inc iter
  JUMP L2
L3:  # For End#For End
  RETURN 10
proc_write:  # Procedure write#Procedure write
  PROLOGUE 16
  COPY _t47, 1
  LOAD _t48, n
  COPY i, _t47  # For Init#For Init
  SUB _t49, _t48, _t47
  ADD _t49, _t49, 1
L8:
  JUMP_LEQ L9, _t49, 0
  COPY _t51, t
  ADD _t52, _t51, i
  LOAD _t50, _t52
  WRITE , _t50
  SUB _t49, _t49, 1  # Dec count#Dec count
  ADD i, i, 1  # Inc iter#Inc iter
  JUMP L8
L9:  # For End#For End
  RETURN 16
main:  # Main Program#Main Program
  COPY _t53, 23
  COPY n, _t53
  COPY _t54, 18
  PARAM , _t54
  PARAM , 43
  CALL , proc_shuffle
  COPY _t55, 18
  PARAM , _t55
  PARAM , 43
  CALL , proc_write
  COPY _t56, 1234567890
  WRITE , _t56
  COPY _t57, 18
  PARAM , _t57
  PARAM , 43
  CALL , proc_sort
  COPY _t58, 18
  PARAM , _t58
  PARAM , 43
  CALL , proc_write
  HALT   # HALT#HALT
