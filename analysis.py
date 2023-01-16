import sys
import statistics

cnf =[];
appv1 =[];
appv2 =[];
ratiov1 =[];
ratiov2=[];

while True:
    line = sys.stdin.readline();
    

    if line=='':
        break;
    array = line.split(',');
    cnf.append(float(array[0]))
    appv1.append(float(array[1]))
    appv2.append(float(array[2]))
    ratiov1.append(float(float(array[3])/float(array[5])))
    ratiov2.append(float(float(array[4])/float(array[5])))
str2='';
str2=str2+str(statistics.mean(cnf))+',';
str2=str2+str(statistics.mean(appv1))+',';
str2=str2+str(statistics.mean(appv2))+',';
str2=str2+str(statistics.stdev(cnf))+',';
str2=str2+str(statistics.stdev(appv1))+',';
str2=str2+str(statistics.stdev(appv2))+',';
str2=str2+str(statistics.mean(ratiov1))+',';
str2=str2+str(statistics.mean(ratiov2))+',';
str2=str2+str(statistics.stdev(ratiov1))+',';
str2=str2+str(statistics.stdev(ratiov2))
# print(statistics.mean(cnf))
# print(statistics.mean(appv1))
# print(statistics.mean(appv2))
# print(statistics.stdev(cnf))
# print(statistics.stdev(appv1))
# print(statistics.stdev(appv2))
# print(statistics.mean(ratiov1))
# print(statistics.mean(ratiov2))
# print(statistics.stdev(ratiov1))
# print(statistics.stdev(ratiov2))
print(str2)