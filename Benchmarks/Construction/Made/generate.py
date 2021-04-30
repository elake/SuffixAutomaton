import random
import string
n = 10000
f = open("1.in", "w")
for i in range(50):
    s = ""
    for _ in range(n*(i+1)):
        s += random.choice(string.ascii_letters)
    f.write(s + "\n")
f.close()