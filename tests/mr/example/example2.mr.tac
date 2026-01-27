proc_pa:  # Procedure pa
  COPY _t1, a
  COPY _t2, b
  ADD _t0, _t1, _t2
  COPY a, _t0
  COPY _t4, a
  COPY _t5, b
  SUB _t3, _t4, _t5
  COPY b, _t3
  RETURN 
proc_pb:  # Procedure pb
  PARAM , 13
  PARAM , 14
  CALL , pa
  PARAM , 13
  PARAM , 14
  CALL , pa
  RETURN 
proc_pc:  # Procedure pc
  PARAM , 13
  PARAM , 14
  CALL , pb
  PARAM , 13
  PARAM , 14
  CALL , pb
  PARAM , 13
  PARAM , 14
  CALL , pb
  RETURN 
proc_pd:  # Procedure pd
  PARAM , 13
  PARAM , 14
  CALL , pc
  PARAM , 13
  PARAM , 14
  CALL , pc
  PARAM , 13
  PARAM , 14
  CALL , pc
  PARAM , 13
  PARAM , 14
  CALL , pc
  RETURN 
main:  # Main Program
  READ _t6
  COPY a, _t6
  READ _t7
  COPY b, _t7
  PARAM , 13
  PARAM , 14
  CALL , pd
  COPY _t8, a
  WRITE , _t8
  COPY _t9, b
  WRITE , _t9
  HALT   # HALT
