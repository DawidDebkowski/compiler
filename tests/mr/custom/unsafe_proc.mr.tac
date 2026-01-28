  JUMP main  # Entry Jump#Entry Jump
proc_p:  # Procedure p#Procedure p
  PROLOGUE 5
  COPY _t0, 123
  COPY _t3, arr
  LOAD _t4, _t3
  COPY _t1, _t3
  ADD _t1, _t1, 1
  ADD _t1, _t1, 3000
  SUB _t2, _t1, _t4  # unsafe assign#unsafe assign
  STORE _t2, _t0
  COPY _t8, arr
  LOAD _t9, _t8
  COPY _t6, _t8
  ADD _t6, _t6, 1
  ADD _t6, _t6, 3000
  SUB _t7, _t6, _t9  # unsafe access#unsafe access
  LOAD _t5, _t7
  WRITE , _t5
  RETURN 5
main:  # Main Program#Main Program
  COPY _t10, 3000
  COPY _t11, 7
  STORE _t11, _t10  # init unsafe header arr#init unsafe header arr
  COPY _t12, 7
  PARAM , _t12
  CALL , proc_p
  HALT   # HALT#HALT
