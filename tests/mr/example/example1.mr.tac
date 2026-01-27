  JUMP main  # Entry Jump
proc_de:  # Procedure de
  PROLOGUE 5
  LOAD _t0, m
  COPY a, _t0
  LOAD _t1, n
  COPY b, _t1
  COPY _t2, 1
  STORE x, _t2
  COPY _t3, 0
  STORE y, _t3
  LOAD _t4, n
  COPY r, _t4
  LOAD _t6, m
  COPY _t7, 1
  SUB _t5, _t6, _t7
  COPY s, _t5
L0:  # While Start
  COPY _t8, b
  COPY _t9, 0
  JUMP_LEQ L1, _t8, _t9
  COPY _t11, a
  COPY _t12, b
  MOD _t10, _t11, _t12
  COPY reszta, _t10
  COPY _t14, a
  COPY _t15, b
  DIV _t13, _t14, _t15
  COPY iloraz, _t13
  COPY _t16, b
  COPY a, _t16
  COPY _t17, reszta
  COPY b, _t17
  COPY _t18, r
  COPY rr, _t18
  COPY _t20, iloraz
  COPY _t21, r
  MUL _t19, _t20, _t21
  COPY tmp, _t19
  LOAD _t22, x
  COPY _t23, tmp
  JUMP_GEQ L4, _t22, _t23
  LOAD _t25, n
  COPY _t26, iloraz
  MUL _t24, _t25, _t26
  COPY r, _t24
  JUMP L3
L4:
  COPY _t27, 0
  COPY r, _t27
L3:
  COPY _t29, r
  LOAD _t30, x
  ADD _t28, _t29, _t30
  COPY r, _t28
  COPY _t32, r
  COPY _t33, tmp
  SUB _t31, _t32, _t33
  COPY r, _t31
  COPY _t34, s
  COPY ss, _t34
  COPY _t36, iloraz
  COPY _t37, s
  MUL _t35, _t36, _t37
  COPY tmp, _t35
  LOAD _t38, y
  COPY _t39, tmp
  JUMP_GEQ L7, _t38, _t39
  LOAD _t41, m
  COPY _t42, iloraz
  MUL _t40, _t41, _t42
  COPY s, _t40
  JUMP L6
L7:
  COPY _t43, 0
  COPY s, _t43
L6:
  COPY _t45, s
  LOAD _t46, y
  ADD _t44, _t45, _t46
  COPY s, _t44
  COPY _t48, s
  COPY _t49, tmp
  SUB _t47, _t48, _t49
  COPY s, _t47
  COPY _t50, rr
  STORE x, _t50
  COPY _t51, ss
  STORE y, _t51
  JUMP L0
L1:  # While End
  COPY _t52, a
  STORE z, _t52
  RETURN 5
main:  # Main Program
  READ _t53
  COPY m, _t53
  READ _t54
  COPY n, _t54
  PARAM , 20
  PARAM , 21
  PARAM , 22
  PARAM , 23
  PARAM , 24
  CALL , proc_de
  COPY _t55, x
  WRITE , _t55
  COPY _t56, y
  WRITE , _t56
  COPY _t57, nwd
  WRITE , _t57
  HALT   # HALT
