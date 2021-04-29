import random
import string
s = ""
n = 10000
f = open("1.in", "w")
for i in range(100):
    for _ in range(n):
        s += random.choice(string.ascii_letters)
    f.write(s + "\n")
f.close()