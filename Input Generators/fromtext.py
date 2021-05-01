import random
import string
fr = open("bible.txt", "r")
f = open("biblepositions.in", "w")
ss = fr.read()
s = ""
for i in range(len(ss)):
    c = ss[i]
    if(c == "\n"):
        s += " "
        continue
    if (c == " " and s[len(s)-1] == " "):
        continue
    s += c
f.write(s + "\n")
f.close()