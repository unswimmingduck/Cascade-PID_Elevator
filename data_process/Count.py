import pandas as pds


# reading data
f = open("") ## path of the file you got the data copy from terminal in Proteus   
# 遍历数据，并对数据进行处理
for line in f:
    num = line.split("_")
num.pop(-1)

result = []
# calculate the rotation speed
for i in num:
    if int(i) != 0:
        result.append(((6/(int(i)/1000)))/360)
        
input = [50 for i in range(len(result))]

title = ['input', 'result']
DataForm = pds.DataFrame({"input":input, "result":result})
DataForm.to_csv("F:\stm32\system.csv")


