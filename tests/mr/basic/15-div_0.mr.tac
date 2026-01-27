  JUMP main  # Entry Jump
main:  # Main Program
  COPY _t0, 10
  COPY a, _t0
  COPY _t1, 15
  COPY b, _t1
  COPY _t3, a
  COPY _t4, b
  DIV _t2, _t3, _t4
  COPY c, _t2
  COPY _t5, c
  WRITE , _t5
  COPY _t7, 100
  COPY _t8, c
  DIV _t6, _t7, _t8
  COPY d, _t6
  COPY _t9, d
  WRITE , _t9
  HALT   # HALT
