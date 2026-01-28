  JUMP main  # Entry Jump#Entry Jump
proc_s:  # Procedure s#Procedure s
  PROLOGUE 5
  COPY _t0, 1
  STORE m, _t0
  LOAD _t1, n
  COPY _t2, 2
  COPY i, _t1  # For Init#For Init
  SUB _t3, _t1, _t2
  ADD _t3, _t3, 1
L0:
  JUMP_LEQ L1, _t3, 0
  COPY _t5, i
  COPY _t6, 2
  MOD _t4, _t5, _t6
  COPY o, _t4
  COPY _t7, o
  COPY _t8, 1
  JUMP_NEQ L2, _t7, _t8
  LOAD _t10, m
  COPY _t11, i
  MUL _t9, _t10, _t11
  STORE m, _t9
  JUMP L3
L2:
  COPY _t13, i
  LOAD _t14, m
  MUL _t12, _t13, _t14
  STORE m, _t12
L3:
  SUB _t3, _t3, 1  # Dec count#Dec count
  SUB i, i, 1  # Dec iter#Dec iter
  JUMP L0
L1:  # For End#For End
  RETURN 5
proc_bc:  # Procedure bc#Procedure bc
  PROLOGUE 9
  COPY _t15, n
  PARAM , _t15
  PARAM , 14
  CALL , proc_s
  LOAD _t17, n
  LOAD _t18, k
  SUB _t16, _t17, _t18
  COPY p, _t16
  PARAM , 13
  PARAM , 15
  CALL , proc_s
  COPY _t19, k
  PARAM , _t19
  PARAM , 16
  CALL , proc_s
  COPY _t21, a
  COPY _t22, b
  DIV _t20, _t21, _t22
  STORE m, _t20
  LOAD _t24, m
  COPY _t25, c
  DIV _t23, _t24, _t25
  STORE m, _t23
  RETURN 9
main:  # Main Program#Main Program
  READ _t26
  COPY n, _t26
  READ _t27
  COPY k, _t27
  PARAM , 17
  PARAM , 18
  PARAM , 19
  CALL , proc_bc
  COPY _t28, w
  WRITE , _t28
  HALT   # HALT#HALT
