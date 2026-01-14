n = 24
ta = [0] * 25
tb = [0] * 25
tc = [0] * 25

tc[0] = n
tc[n] = n - tc[0]

j = tc[0] + 1

for i in range(tc[0], tc[n] - 1, -1):
    ta[i] = i + 1
    tb[i] = j - i

j = tc[n]

for i in range(tc[n], tc[0] + 1):
    tc[i] = ta[i] * tb[i]

for i in range(0, n + 1):
    print(">", end=" ")
    print(tc[i])
