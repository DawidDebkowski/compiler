  JUMP main  # Entry Jump#Entry Jump
proc_factorial:  # Procedure factorial#Procedure factorial
  PROLOGUE 5
  COPY _t0, 1
  COPY _t1, s
  ADD _t2, _t1, 0
  STORE _t2, _t0
  COPY _t4, s
  ADD _t5, _t4, 0
  LOAD _t3, _t5
  COPY p, _t3
  COPY _t6, 1
  LOAD _t7, n
  COPY i, _t6  # For Init#For Init
  SUB _t8, _t7, _t6
  ADD _t8, _t8, 1
L0:
  JUMP_LEQ L1, _t8, 0
  COPY _t10, p
  COPY _t11, i
  MUL _t9, _t10, _t11
  COPY _t12, s
  ADD _t13, _t12, i
  STORE _t13, _t9
  COPY _t15, s
  ADD _t16, _t15, i
  LOAD _t14, _t16
  COPY p, _t14
  SUB _t8, _t8, 1  # Dec count#Dec count
  ADD i, i, 1  # Inc iter#Inc iter
  JUMP L0
L1:  # For End#For End
  RETURN 5
proc_bc:  # Procedure bc#Procedure bc
  PROLOGUE 9
  COPY _t17, 13
  PARAM , _t17
  COPY _t18, n
  PARAM , _t18
  CALL , proc_factorial
  LOAD _t20, n
  LOAD _t21, k
  SUB _t19, _t20, _t21
  COPY p, _t19
  LOAD _t25, n
  COPY _t26, 13
  ADD _t27, _t26, _t25
  LOAD _t23, _t27
  LOAD _t28, k
  COPY _t29, 13
  ADD _t30, _t29, _t28
  LOAD _t24, _t30
  DIV _t22, _t23, _t24
  STORE m, _t22
  LOAD _t32, m
  COPY _t34, 13
  ADD _t35, _t34, p
  LOAD _t33, _t35
  DIV _t31, _t32, _t33
  STORE m, _t31
  RETURN 9
main:  # Main Program#Main Program
  READ _t36
  COPY n, _t36
  READ _t37
  COPY k, _t37
  PARAM , 116
  PARAM , 117
  PARAM , 118
  CALL , proc_bc
  COPY _t38, w
  WRITE , _t38
  HALT   # HALT#HALT
