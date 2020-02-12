import csv

with open('ClassicResult') as csv_file:
    csv_reader = csv.reader(csv_file, delimiter='\t')
    line_count = 0
    csv_data = [row for row in csv_reader]
    fields = csv_data[0]
    csv1_data = []
    csv2_data = []
    csv3_data = []
    csv4_data = []
    for row in csv_data[1:]:
        
        
        data = [row[0],row[2],row[4],row[6]]
        print(data)
        if data[1] < data[0]:
            csv1_data.append(data)
        elif data[1] == data[0]:
            csv3_data.append(data)
        else:
            csv2_data.append(data)

file1 = "CL_B<S.csv"
  

with open(file1, 'w') as csvfile: 

    csvwriter = csv.writer(csvfile) 
    csvwriter.writerow(fields) 
    csvwriter.writerows(csv1_data)


file3 = "CL_B=S.csv"
  

with open(file3, 'w') as csvfile: 

    csvwriter = csv.writer(csvfile) 
    csvwriter.writerow(fields) 
    csvwriter.writerows(csv3_data)

file2 = "CL_B>S.csv"
  

with open(file2,'w') as csvfile: 

    csvwriter = csv.writer(csvfile) 
    csvwriter.writerow(fields) 
    csvwriter.writerows(csv2_data)
