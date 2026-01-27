  JUMP main  # Entry Jump
main:  # Main Program
  COPY _t0, 20
  COPY a, _t0
  COPY _t1, 2
  COPY b, _t1
  COPY _t3, a
  COPY _t4, b
  DIV _t2, _t3, _t4
  COPY c, _t2
  COPY _t5, c
  WRITE , _t5
  HALT   # HALT
