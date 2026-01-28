  JUMP main  # Entry Jump#Entry Jump
proc_licz:  # Procedure licz#Procedure licz
  PROLOGUE 5
  COPY _t0, 2
  LOAD _t1, n
  COPY i, _t0  # For Init#For Init
  SUB _t2, _t1, _t0
  ADD _t2, _t2, 1
L0:
  JUMP_LEQ L1, _t2, 0
  COPY _t3, 1
  COPY _t4, s
  ADD _t5, _t4, i
  STORE _t5, _t3
  SUB _t2, _t2, 1  # Dec count#Dec count
  ADD i, i, 1  # Inc iter#Inc iter
  JUMP L0
L1:  # For End#For End
  COPY _t6, 2
  LOAD _t7, n
  COPY i, _t6  # For Init#For Init
  SUB _t8, _t7, _t6
  ADD _t8, _t8, 1
L2:
  JUMP_LEQ L3, _t8, 0
  COPY _t11, s
  ADD _t12, _t11, i
  LOAD _t9, _t12
  COPY _t10, 0
  JUMP_LEQ L4, _t9, _t10
  COPY _t14, i
  COPY _t15, i
  ADD _t13, _t14, _t15
  COPY j, _t13
L5:  # While Start#While Start
  COPY _t16, j
  LOAD _t17, n
  JUMP_GT L6, _t16, _t17
  COPY _t18, 0
  COPY _t19, s
  ADD _t20, _t19, j
  STORE _t20, _t18
  COPY _t22, j
  COPY _t23, i
  ADD _t21, _t22, _t23
  COPY j, _t21
  JUMP L5
L6:  # While End#While End
L4:
  SUB _t8, _t8, 1  # Dec count#Dec count
  ADD i, i, 1  # Inc iter#Inc iter
  JUMP L2
L3:  # For End#For End
  RETURN 5
proc_wypisz:  # Procedure wypisz#Procedure wypisz
  PROLOGUE 9
  LOAD _t24, n
  COPY _t25, 2
  COPY i, _t24  # For Init#For Init
  SUB _t26, _t24, _t25
  ADD _t26, _t26, 1
L7:
  JUMP_LEQ L8, _t26, 0
  COPY _t29, s
  ADD _t30, _t29, i
  LOAD _t27, _t30
  COPY _t28, 0
  JUMP_LEQ L9, _t27, _t28
  COPY _t31, i
  WRITE , _t31
L9:
  SUB _t26, _t26, 1  # Dec count#Dec count
  SUB i, i, 1  # Dec iter#Dec iter
  JUMP L7
L8:  # For End#For End
  RETURN 9
main:  # Main Program#Main Program
  COPY _t32, 100
  COPY n, _t32
  COPY _t33, 11
  PARAM , _t33
  PARAM , 12
  CALL , proc_licz
  COPY _t34, 11
  PARAM , _t34
  PARAM , 12
  CALL , proc_wypisz
  HALT   # HALT#HALT
