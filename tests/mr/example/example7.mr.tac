  JUMP main  # Entry Jump#Entry Jump
main:  # Main Program#Main Program
  READ _t0
  COPY a, _t0
  READ _t1
  COPY b, _t1
  READ _t2
  COPY c, _t2
  COPY _t3, 111091
  COPY i, _t3
L0:  # While Start#While Start
  COPY _t4, i
  COPY _t5, 111110
  JUMP_GT L1, _t4, _t5
  COPY _t6, 209
  COPY j, _t6
L2:  # While Start#While Start
  COPY _t7, j
  COPY _t8, 200
  JUMP_LT L3, _t7, _t8
  COPY _t9, 11
  COPY k, _t9
L4:  # While Start#While Start
  COPY _t10, k
  COPY _t11, 20
  JUMP_GT L5, _t10, _t11
  COPY _t13, a
  COPY _t14, k
  ADD _t12, _t13, _t14
  COPY a, _t12
  COPY _t16, k
  COPY _t17, 1
  ADD _t15, _t16, _t17
  COPY k, _t15
  JUMP L4
L5:  # While End#While End
  COPY _t19, b
  COPY _t20, j
  ADD _t18, _t19, _t20
  COPY b, _t18
  COPY _t22, j
  COPY _t23, 1
  SUB _t21, _t22, _t23
  COPY j, _t21
  JUMP L2
L3:  # While End#While End
  COPY _t25, c
  COPY _t26, i
  ADD _t24, _t25, _t26
  COPY c, _t24
  COPY _t28, i
  COPY _t29, 1
  ADD _t27, _t28, _t29
  COPY i, _t27
  JUMP L0
L1:  # While End#While End
  COPY _t30, a
  WRITE , _t30
  COPY _t31, b
  WRITE , _t31
  COPY _t32, c
  WRITE , _t32
  HALT   # HALT#HALT
