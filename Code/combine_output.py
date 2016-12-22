import csv

# c = csv.writer(open("combined.csv", "wb"))

# for i in range(0,32):
# 	file = open('output/output_proc_'+str(i)+'.txt', 'r')
# 	lines = file.readlines()
# 	for line in lines:
# 		line = [float(numeric_string) for numeric_string in (line.split(','))]
# 		print line
# 		c.writerow(line)

rows=[]
dict={} #key=nprocs, value=time_lapse
with open('combined.csv') as csvfile:
	readCSV = csv.reader(csvfile, delimiter=',')
	for row in readCSV:
		print "ok"
		rows.append(row)
		nprocs = row[0]
		time_lapse = row[2]
		if nprocs not in dict.keys():
			dict[nprocs] = time_lapse
		else:
			if time_lapse > dict[nprocs]:
				dict[nprocs] = time_lapse

print dict

c = csv.writer(open("combined_final_wine.csv", "wb"))
for key in dict.keys():
	c.writerow([key,dict[key]])

