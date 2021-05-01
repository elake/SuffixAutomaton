import random
import string
s = ""
n = 10
f = open("1.in", "w")
for i in range(1):
    for _ in range((2**31)//2):
        s += random.choice(string.ascii_letters)
    f.write(s + "\n")
f.close()