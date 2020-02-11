import csv

with open('PrrSSRBResult') as csv_file:
    csv_reader = csv.reader(csv_file, delimiter='\t')
    line_count = 0
    csv_data = [row for row in csv_reader]
    fields = csv_data[0]
    csv1_data = []
    csv2_data = []
    for row in csv_data[1:]:
        
        
        data = [row[0],row[2],row[4],row[6]]
        print(data)
        if data[1] <= data[0]:
            csv1_data.append(data)
        else:
            csv2_data.append(data)

file1 = "PrrSSRB_B<=S.csv"
  

with open(file1, 'w') as csvfile: 

    csvwriter = csv.writer(csvfile) 
    csvwriter.writerow(fields) 
    csvwriter.writerows(csv1_data)


file2 = "PrrSSRB_B>S.csv"
  

with open(file2,'w') as csvfile: 

    csvwriter = csv.writer(csvfile) 
    csvwriter.writerow(fields) 
    csvwriter.writerows(csv2_data)
