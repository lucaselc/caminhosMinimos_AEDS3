
#!/usr/bin/env python
import os
import sys
import json
import collector

with open('counts.json', 'r') as f:
    counts = json.load(f)
    counts.sort()
    filenames = [f'test_{x}.txt' for x in counts]


def analyze():
    iotimes = []
    runtimes = []
    for f in filenames:
        io, run = collector.avgs_of(f)
        iotimes.append(io)
        runtimes.append(run)

    obj = {
        'counts': counts,
        'iotimes': list(map(collector.AvgTimes.toJSON, iotimes)),
        'runtimes': list(map(collector.AvgTimes.toJSON, runtimes)),
    }
    with open('results.json', 'w') as f:
        json.dump(obj, f)

    return obj

def graph(name, times: list[collector.AvgTimes], counts, output):
    import matplotlib.pyplot as plt
    import numpy as np
    from scipy.optimize import curve_fit

    times_phys = [t.phys for t in times]
    x_values = counts
    y_values = times_phys

    def f(x, a, b):
        return a * x * np.log(x) + b
    
    params, covariance = curve_fit(f, x_values, y_values)

    # Generate x values for plotting
    x_plot = np.linspace(min(x_values), max(x_values), 1000)

    # Plot the data points
    plt.scatter(x_values, y_values, label='Data')

    # Plot the fitted curve
    plt.plot(x_plot, f(x_plot, *params), label='Fitted curve', color='orange')

    plt.ylabel('Tempo em segundos (físico)')
    plt.xlabel('Quantidade de arestas')
    plt.title(name)

    plt.savefig(output)

    plt.close()

try:
    with open('results.json') as r:
        obj = json.load(r)
except FileNotFoundError:
    obj = analyze()

counts = obj['counts']
runtimes = [collector.AvgTimes(*x) for x in obj['runtimes']]
graph('Tempos de Execução', runtimes, counts, 'runtimes.svg')



