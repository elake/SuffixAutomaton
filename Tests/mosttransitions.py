import random
import string
n = 10000
f = open("mosttransitions.in", "w")
for i in range(50):
    s = "a"
    for _ in range(n*(i+1)):
        s += "b"
    f.write(s + "c\n")
f.close()