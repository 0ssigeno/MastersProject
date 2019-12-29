
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import os
import matplotlib.pyplot as plt
from collections import defaultdict
from scipy.stats import t
import sys
def printGraph(array,name):
	plt.plot(array)
	plt.title(name)
	plt.show()


def transient(array):
	mean=np.mean(array)
	std=np.std(array)
	val1=mean+std
	val2=mean-std
	for i,val in enumerate(array):
		if val>val2 and val<val1:
		#	print(i)
			return i




def deleteNinitalColumns(matrix, value):
	for _ in range(0, value):
		for row in matrix:
			del row[0]


def deleteNvalues(array,value):
	for i in range(0,value):
		del array[0]


def meanColumn(matrix, i):
    sum=0
    numrows=len(matrix)
    for row in matrix:
    	sum=sum + row[i]
    return sum/ float(numrows)


def minRow(matrix): #rows has different lenght
	numrows = len(matrix)
	min=len(matrix[0])
	for i in range(1, numrows):
		if len(matrix[i]) < min:
			min= len(matrix[i])
	return min

def rowToStr(row):
	str1 = ''.join(str(e) for e in row)
	return(str1.replace("]","").replace("[","").replace(",",""))



#Modificare il punto di ingresso
if (len(sys.argv)!=2):
	print("Insert name of project to analyze")
	exit(0)
path="./samples/"+sys.argv[1]+"/results/";
files = os.listdir(path)

total=defaultdict(list)

risultatiPuliti="../risultatiPuliti.csv"
risultatiSporchi="../risultatiSporchi.csv"

try:
	os.remove(risultatiPuliti)
	os.remove(risultatiSporchi)
except OSError:
	pass

print("Creation of a single table with every results")

for file in files: 
	if file.endswith('.csv'):
		nameFile=file.split("#")[0]
		run=file.split("#")[1].split(".")[0]
		csv = pd.read_csv(path+file)
		modules= csv.module.unique()
		attributes= csv.name.unique()
		type=csv.type.unique()
		for module in modules:	
			for attribute in attributes:
				if str(attribute)!="nan":
					try:
						key=(nameFile,attribute) #nameFile indica la configurazione, y l'attributo che stiamo visualizando
						row= csv[(csv.type=='vector') & (csv.module==module) & (csv.name==attribute)].vecvalue.describe()
						total[key].append([float(item) for item in row[2].split()]) #ci aggiungo tutti i nuovi valori
						with open(risultatiSporchi,"a") as f:
							r=rowToStr(row)
							f.write("Configuration, "+nameFile +", Run, "+run+", Values, " + r + "\n")
							print(".",end='')				
					except Exception as e:
						pass
print()
print("-----------Done---------------")
for name in total:
	if len(total[name])>0: 
		print("Configuration, "+str(name[0]) +",Attribute, "+str(name[1]) )

		arrayMean=[]
		prefix_mean = []
		for i in range(0 , minRow(total[name])):
			mean=meanColumn(total[name],i)
			arrayMean.append(mean)
			if i == 0:
				prefix_mean.append(mean)	
			else:
				prefix_mean.append(np.mean([*prefix_mean, mean]))
		trans=transient(prefix_mean)
		print("---> Transient index: "+str(trans) )

		#decommentare i printGraph se si vuole vedere il risultato graficamente
		#printGraph(arrayMean,name)
		#cancello i valori prima del transiente sia nella matrice (calcolo degli intervalli di confidenza)
		deleteNinitalColumns(total[name],trans)
		#sia nei miei valori puliti, per poterli poi inserire
		deleteNvalues(arrayMean,trans)
		#printGraph(arrayMean,name)


		mean_of_rows = []
		minn=-1
		maxx=-1
		for i,row in enumerate(total[name]):
		
			#si richiedevano pure il minimo e il massimo, li calcolo di tutti che non mi costa nulla
			if (min(row) < minn and min(row)!=0) or (minn==-1 and min(row)>0):
 				minn=min(row)
			if max(row) > maxx:
 				maxx=max(row)
		
			mean =np.mean(row)
			std=np.std(row)
			mean_of_rows.append(mean)

		m = len(mean_of_rows)
		m_squared = np.sqrt(m)
		mean_estimate = np.mean(mean_of_rows)
		std_dev_estimate = np.std(mean_of_rows)
		t_value = t.pdf(0.05, df=m-1)
		conf_lower = mean_estimate - t_value*(std_dev_estimate/m_squared)
		conf_upper = mean_estimate + t_value*(std_dev_estimate/m_squared) 
		print("---> Condifence intervals lower: {}, upper: {}".format(conf_lower, conf_upper))
		with open(risultatiPuliti,"a") as f:
			f.write(str(name[0]) +str(name[1])+", ")
			f.write("startTransient, "+str(trans)+", ")
			f.write("confLower, "+str(conf_lower) +", confUpper, "+str(conf_upper))
			if "elaborationTime" in name[1]:
				f.write(", min, "+str(minn)+", max, "+str(maxx)) 
			f.write(", meanValues, ")
			f.write(rowToStr(arrayMean))
			f.write("\n")
			print("---> Results saved on file")
			
