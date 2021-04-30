import random
import string
s = ""
n = 10000
fr = open("anna.txt", "r")
f = open("anna.in", "w")
for i in range(100):
    for _ in range(n):
        c = fr.read(1)
        while (c == "\n" or (c == " " and s[len(s)-1] == " ")):
            c = fr.read(1)
        s += c
    f.write(s + "\n")
f.close()