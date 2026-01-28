  JUMP main  # Entry Jump#Entry Jump
proc_mult:  # Procedure mult#Procedure mult
  PROLOGUE 5
  LOAD _t1, a
  LOAD _t2, b
  MUL _t0, _t1, _t2
  STORE o, _t0
  RETURN 5
main:  # Main Program#Main Program
  COPY _t4, 123
  COPY _t5, 124
  MUL _t3, _t4, _t5
  COPY a, _t3
  COPY _t6, 16
  COPY sixseven, _t6
  PARAM , 9
  PARAM , 12
  PARAM , 11
  CALL , proc_mult
  COPY _t7, c
  WRITE , _t7
  HALT   # HALT#HALT
