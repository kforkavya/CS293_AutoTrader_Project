f=open("inputs/data2.txt", "w")
for i in range(1, 10000):
    f.write("A 1 100 b#\r\n")
for i in range(1, 10000):
    f.write("C 1 100 b#\r\n")
f.write("B 1 100 b#\r\n")
f.close()