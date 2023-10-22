f=open("sample.txt", "r")
f2=open("inputs/data1.txt", "w")
count=0
for word in f.readlines():
    count+=1
    message=word[:len(word)-1]+" 100 b#\n"
    f2.write(message)
    if count==10000:
        break
f2.close()
f.close()