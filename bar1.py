import matplotlib.pyplot as plt
import numpy as np

# Collect the data from the file, ignore empty lines
with open('ClassicResult') as f:
    lines = [line.strip().split(': ') for line in f if len(line) > 1]

labels, y = zip(*lines)

# Generate indexes
ind = np.arange(len(labels))

# Convert the y values from str to int
y = map(int, y)

plt.figure()
plt.bar(ind, y, align='center')
plt.xticks(ind, labels)
plt.show()
