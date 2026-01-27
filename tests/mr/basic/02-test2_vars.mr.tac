  JUMP main  # Entry Jump
main:  # Main Program
  COPY _t0, 10
  COPY a, _t0
  COPY _t1, 20
  COPY b, _t1
  COPY _t2, a
  WRITE , _t2
  COPY _t3, b
  WRITE , _t3
  HALT   # HALT
