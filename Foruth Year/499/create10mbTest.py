fp = open("p1.txt", "wb+")

x = 0
for x in range(0, 1000000):
    fp.write(b'hello bob\n')
fp.close()