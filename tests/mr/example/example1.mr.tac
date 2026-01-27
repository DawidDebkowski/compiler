  JUMP main  # Entry Jump
proc_de:  # Procedure de
  PROLOGUE 
  LOAD _t1, m
  LOAD _t0, _t1
  COPY a, _t0
  LOAD _t3, n
  LOAD _t2, _t3
  COPY b, _t2
  COPY _t4, 1
  LOAD _t5, x
  STORE _t5, _t4
  COPY _t6, 0
  LOAD _t7, y
  STORE _t7, _t6
  LOAD _t9, n
  LOAD _t8, _t9
  COPY r, _t8
  LOAD _t13, m
  LOAD _t11, _t13
  COPY _t12, 1
  SUB _t10, _t11, _t12
  COPY s, _t10
L0:  # While Start
  COPY _t14, b
  COPY _t15, 0
  JUMP_LEQ L1, _t14, _t15
  COPY _t17, a
  COPY _t18, b
  MOD _t16, _t17, _t18
  COPY reszta, _t16
  COPY _t20, a
  COPY _t21, b
  DIV _t19, _t20, _t21
  COPY iloraz, _t19
  COPY _t22, b
  COPY a, _t22
  COPY _t23, reszta
  COPY b, _t23
  COPY _t24, r
  COPY rr, _t24
  COPY _t26, iloraz
  COPY _t27, r
  MUL _t25, _t26, _t27
  COPY tmp, _t25
  LOAD _t30, x
  LOAD _t28, _t30
  COPY _t29, tmp
  JUMP_GEQ L4, _t28, _t29
  LOAD _t34, n
  LOAD _t32, _t34
  COPY _t33, iloraz
  MUL _t31, _t32, _t33
  COPY r, _t31
  JUMP L3
L4:
  COPY _t35, 0
  COPY r, _t35
L3:
  COPY _t37, r
  LOAD _t39, x
  LOAD _t38, _t39
  ADD _t36, _t37, _t38
  COPY r, _t36
  COPY _t41, r
  COPY _t42, tmp
  SUB _t40, _t41, _t42
  COPY r, _t40
  COPY _t43, s
  COPY ss, _t43
  COPY _t45, iloraz
  COPY _t46, s
  MUL _t44, _t45, _t46
  COPY tmp, _t44
  LOAD _t49, y
  LOAD _t47, _t49
  COPY _t48, tmp
  JUMP_GEQ L7, _t47, _t48
  LOAD _t53, m
  LOAD _t51, _t53
  COPY _t52, iloraz
  MUL _t50, _t51, _t52
  COPY s, _t50
  JUMP L6
L7:
  COPY _t54, 0
  COPY s, _t54
L6:
  COPY _t56, s
  LOAD _t58, y
  LOAD _t57, _t58
  ADD _t55, _t56, _t57
  COPY s, _t55
  COPY _t60, s
  COPY _t61, tmp
  SUB _t59, _t60, _t61
  COPY s, _t59
  COPY _t62, rr
  LOAD _t63, x
  STORE _t63, _t62
  COPY _t64, ss
  LOAD _t65, y
  STORE _t65, _t64
  JUMP L0
L1:  # While End
  COPY _t66, a
  LOAD _t67, z
  STORE _t67, _t66
  RETURN 
main:  # Main Program
  READ _t68
  COPY m, _t68
  READ _t69
  COPY n, _t69
  PARAM , 19
  PARAM , 20
  PARAM , 21
  PARAM , 22
  PARAM , 23
  CALL , proc_de
  COPY _t70, x
  WRITE , _t70
  COPY _t71, y
  WRITE , _t71
  COPY _t72, nwd
  WRITE , _t72
  HALT   # HALT
