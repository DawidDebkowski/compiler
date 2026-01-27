  JUMP main  # Entry Jump
proc_pa:  # Procedure pa
  PROLOGUE 5
  LOAD _t1, a
  LOAD _t2, b
  ADD _t0, _t1, _t2
  STORE a, _t0
  LOAD _t4, a
  LOAD _t5, b
  SUB _t3, _t4, _t5
  STORE b, _t3
  RETURN 5
proc_pb:  # Procedure pb
  PROLOGUE 8
  LOAD _t6, a
  PARAM , _t6
  LOAD _t7, b
  PARAM , _t7
  CALL , proc_pa
  LOAD _t8, a
  PARAM , _t8
  LOAD _t9, b
  PARAM , _t9
  CALL , proc_pa
  RETURN 8
proc_pc:  # Procedure pc
  PROLOGUE 11
  LOAD _t10, a
  PARAM , _t10
  LOAD _t11, b
  PARAM , _t11
  CALL , proc_pb
  LOAD _t12, a
  PARAM , _t12
  LOAD _t13, b
  PARAM , _t13
  CALL , proc_pb
  LOAD _t14, a
  PARAM , _t14
  LOAD _t15, b
  PARAM , _t15
  CALL , proc_pb
  RETURN 11
proc_pd:  # Procedure pd
  PROLOGUE 14
  LOAD _t16, a
  PARAM , _t16
  LOAD _t17, b
  PARAM , _t17
  CALL , proc_pc
  LOAD _t18, a
  PARAM , _t18
  LOAD _t19, b
  PARAM , _t19
  CALL , proc_pc
  LOAD _t20, a
  PARAM , _t20
  LOAD _t21, b
  PARAM , _t21
  CALL , proc_pc
  LOAD _t22, a
  PARAM , _t22
  LOAD _t23, b
  PARAM , _t23
  CALL , proc_pc
  RETURN 14
main:  # Main Program
  READ _t24
  COPY a, _t24
  READ _t25
  COPY b, _t25
  PARAM , 17
  PARAM , 18
  CALL , proc_pd
  COPY _t26, a
  WRITE , _t26
  COPY _t27, b
  WRITE , _t27
  HALT   # HALT
