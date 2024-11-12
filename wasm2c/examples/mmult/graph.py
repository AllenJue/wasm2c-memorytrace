import matplotlib
import matplotlib.pyplot as plt
import numpy as np
matplotlib.use('Agg') # to avoid backend errors, save as img

# read in the data
f = open('mmm_times.txt', 'r')
data = f.readlines()
f.close()

# parse the data
input_sizes = []
times = []

# since the input sizes may have been repeated, get the sum first then divide by the number of times it was repeated to get the average
sizes_map = {}

for line in data:
    line = line.strip().split()
    # if line is empty, skip it
    if not line:
        continue
    size = int(line[0])
    time = float(line[1])

    if size in sizes_map:
        sizes_map[size].append(time)
    else:
        sizes_map[size] = [time]

# get the average time for each input size
for size in sizes_map:
    # print(size, sizes_map[size])
    input_sizes.append(size)
    times.append(sum(sizes_map[size]) / len(sizes_map[size]))

# print(times)

# plot the data
plt.plot(input_sizes, times)
plt.xlabel('Input Size')
plt.ylabel('Time (s)')
plt.title('MMult WASM Benchmark')
plt.xscale('log', basex=2)
plt.xticks(input_sizes, input_sizes)
# save the plot
plt.savefig('mmm_out.png')