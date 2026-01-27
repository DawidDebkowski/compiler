  JUMP main  # Entry Jump
main:  # Main Program
  COPY _t0, 5
  COPY a, _t0
  COPY _t1, 6
  COPY b, _t1
  COPY _t2, 10
  COPY c, _t2
  COPY _t3, 4
  COPY d, _t3
  COPY _t4, a
  COPY _t5, 5
  JUMP_NEQ L1, _t4, _t5
  COPY _t6, 1
  WRITE , _t6
L1:
  COPY _t7, b
  COPY _t8, 6
  JUMP_NEQ L4, _t7, _t8
  COPY _t9, 2
  WRITE , _t9
L4:
  COPY _t10, a
  COPY _t11, d
  JUMP_EQ L7, _t10, _t11
  COPY _t12, 3
  WRITE , _t12
L7:
  COPY _t13, b
  COPY _t14, a
  JUMP_EQ L10, _t13, _t14
  COPY _t15, 4
  WRITE , _t15
L10:
  COPY _t16, a
  COPY _t17, c
  JUMP_GEQ L13, _t16, _t17
  COPY _t18, 5
  WRITE , _t18
L13:
  COPY _t19, d
  COPY _t20, a
  JUMP_GEQ L16, _t19, _t20
  COPY _t21, 6
  WRITE , _t21
L16:
  COPY _t22, c
  COPY _t23, a
  JUMP_LEQ L19, _t22, _t23
  COPY _t24, 7
  WRITE , _t24
L19:
  COPY _t25, b
  COPY _t26, d
  JUMP_LEQ L22, _t25, _t26
  COPY _t27, 8
  WRITE , _t27
L22:
  COPY _t28, a
  COPY _t29, 5
  JUMP_GT L25, _t28, _t29
  COPY _t30, 9
  WRITE , _t30
L25:
  COPY _t31, d
  COPY _t32, a
  JUMP_GT L28, _t31, _t32
  COPY _t33, 10
  WRITE , _t33
L28:
  COPY _t34, a
  COPY _t35, 5
  JUMP_LT L31, _t34, _t35
  COPY _t36, 11
  WRITE , _t36
L31:
  COPY _t37, c
  COPY _t38, a
  JUMP_LT L34, _t37, _t38
  COPY _t39, 12
  WRITE , _t39
L34:
  COPY _t40, a
  COPY _t41, 6
  JUMP_NEQ L38, _t40, _t41
  COPY _t42, 0
  WRITE , _t42
  JUMP L37
L38:
  COPY _t43, 13
  WRITE , _t43
L37:
  COPY _t44, b
  COPY _t45, 5
  JUMP_NEQ L41, _t44, _t45
  COPY _t46, 0
  WRITE , _t46
  JUMP L40
L41:
  COPY _t47, 14
  WRITE , _t47
L40:
  COPY _t48, a
  COPY _t49, d
  JUMP_NEQ L44, _t48, _t49
  COPY _t50, 0
  WRITE , _t50
  JUMP L43
L44:
  COPY _t51, 15
  WRITE , _t51
L43:
  COPY _t52, b
  COPY _t53, a
  JUMP_NEQ L47, _t52, _t53
  COPY _t54, 0
  WRITE , _t54
  JUMP L46
L47:
  COPY _t55, 16
  WRITE , _t55
L46:
  COPY _t56, a
  COPY _t57, c
  JUMP_LEQ L50, _t56, _t57
  COPY _t58, 0
  WRITE , _t58
  JUMP L49
L50:
  COPY _t59, 17
  WRITE , _t59
L49:
  COPY _t60, d
  COPY _t61, a
  JUMP_LEQ L53, _t60, _t61
  COPY _t62, 0
  WRITE , _t62
  JUMP L52
L53:
  COPY _t63, 18
  WRITE , _t63
L52:
  COPY _t64, c
  COPY _t65, a
  JUMP_GEQ L56, _t64, _t65
  COPY _t66, 0
  WRITE , _t66
  JUMP L55
L56:
  COPY _t67, 19
  WRITE , _t67
L55:
  COPY _t68, b
  COPY _t69, d
  JUMP_GEQ L59, _t68, _t69
  COPY _t70, 0
  WRITE , _t70
  JUMP L58
L59:
  COPY _t71, 20
  WRITE , _t71
L58:
  COPY _t72, a
  COPY _t73, 5
  JUMP_LEQ L62, _t72, _t73
  COPY _t74, 0
  WRITE , _t74
  JUMP L61
L62:
  COPY _t75, 21
  WRITE , _t75
L61:
  COPY _t76, d
  COPY _t77, a
  JUMP_LEQ L65, _t76, _t77
  COPY _t78, 0
  WRITE , _t78
  JUMP L64
L65:
  COPY _t79, 22
  WRITE , _t79
L64:
  COPY _t80, a
  COPY _t81, 5
  JUMP_GEQ L68, _t80, _t81
  COPY _t82, 0
  WRITE , _t82
  JUMP L67
L68:
  COPY _t83, 23
  WRITE , _t83
L67:
  COPY _t84, c
  COPY _t85, a
  JUMP_GEQ L71, _t84, _t85
  COPY _t86, 0
  WRITE , _t86
  JUMP L70
L71:
  COPY _t87, 24
  WRITE , _t87
L70:
  HALT   # HALT
