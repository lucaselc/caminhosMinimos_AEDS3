#!/usr/bin/env python
import os
import sys
import json
import collector
import subprocess

def step(func):
    def wrap(*args, **kwargs):
        print("Running", func.__name__, "...")
        return func(*args, **kwargs)

    return wrap

@step
def analyze():
    runtimes = []
    for f in filenames:
        print("Running test", f)
        result = subprocess.run(['./runner', '-i', f, '-o', '/dev/null'], capture_output=True)
        runtimes.append(float(result.stdout.split(b'\n')[1]))
    obj = { 'runtimes': runtimes, 'counts': counts }
    with open('results.json', 'w') as f:
        json.dump(obj, f)
    return obj

@step
def analyze_multi():
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

@step
def graph(name, times: list[float], counts, output):
    import matplotlib.pyplot as plt
    import numpy as np
    from scipy.optimize import curve_fit

    x_values = counts
    y_values = times

    def f(x, a, b):
        return a * x * np.log2(x) + b
    
    params, covariance = curve_fit(f, x_values, y_values)

    print(y_values)

    # Generate x values for plotting
    x_plot = np.linspace(min(x_values), max(x_values), 1000)

    # Plot the data points
    plt.scatter(x_values, y_values, label='Data')

    # Plot the fitted curve
    plt.plot(x_plot, f(x_plot, *params), label='Fitted curve', color='orange')

    # plt.xscale('log')

    plt.ylabel('Tempo em segundos (físico)')
    plt.xlabel('Quantidade de arestas')
    plt.title(name)

    plt.savefig(output)

    plt.close()


@step
def graph_multi(name, times: list[collector.AvgTimes], counts, output):
    graph(name, [x.phys for x in times], counts, output)

@step
def loadjson(name):
    with open(name) as r:
        return json.load(r)

counts = loadjson('counts.json')
counts.sort()
filenames = [f'test_{x}.txt' for x in counts]

if not os.path.exists('results.json'):
    obj = analyze()
else:
    obj = loadjson('results.json')

counts = obj['counts']
runtimes = obj['runtimes']
# runtimes = [collector.AvgTimes(*x) for x in obj['runtimes']]
graph('Tempos de Execução', runtimes, counts, 'runtimes.svg')



