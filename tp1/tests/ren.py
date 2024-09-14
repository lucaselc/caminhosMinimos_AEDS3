#!/usr/bin/env python3
import json
import os

def get_edgec(filename: str):
    with open(filename, 'r') as f:
        args = f.readline().split(' ')
        if 'LETTER' in args[0]:
            args = f.readline().split(' ')
        return int(args[1])

def rename(filename: str):
    count = get_edgec(filename)
    counts.append(count)
    os.rename(filename, f'test_{count}.txt')

counts = []
for file in os.listdir('.'):
    if not file.endswith('.txt'):
        continue
    rename(file)

with open('counts.json', 'w') as f:
    json.dump(sorted(counts), f)


