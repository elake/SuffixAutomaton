import random
import string
s = ""
n = 10000
fr = open("bible.txt", "r")
f = open("bible.in", "w")
for i in range(50):
    for _ in range(n):
        c = fr.read(1)
        while (c == "\n" or (c == " " and s[len(s)-1] == " ")):
            c = fr.read(1)
        s += c
    f.write(s + "\n")
f.close()