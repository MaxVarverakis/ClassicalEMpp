import json
import matplotlib.pyplot as plt

# Open the JSON file
with open('./analysis/python_cpp_comparison.json', 'r') as f:
    # Load the JSON data into a Python dictionary
    data = json.load(f)

numPts = [i**2 for i in data['numPoints']]

plt.scatter(numPts[:-1], data['python'][:-1], c='b', marker='s')
plt.scatter(numPts, data['c++'], c='r', marker='o', s=14)
plt.xscale('log')
plt.xlabel('Number of grid points used in simulation')
plt.ylabel('Execution time [s]')
plt.legend(['Python','C++'])
plt.show()
