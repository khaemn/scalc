#!/usr/bin/env python3

import argparse
import random

parser = argparse.ArgumentParser(description='Generates a file with given N random integers, one per a line')
parser.add_argument('--n', type=int, help='total number of integers to generate', default=100)
parser.add_argument('--min', type=int, help='minimum integer to generate', default=0)
parser.add_argument('--max', type=int, help='maximum integer to generate', default=0)
parser.add_argument('--filename', type=str, help='output file name', default="")
parser.add_argument('--total-files', type=int, help='output file name', default=1)

args = parser.parse_args()

if (args.max <= args.min):
    args.max = args.n * 5

if (args.filename == ""):
    args.filename = "{}.txt".format(args.n)

numbers = []

for i in range (args.n):
    numbers.append(random.randint(args.min, args.max))
numbers.sort()

for filenum in range(1, args.total_files + 1):
    fname = "{}_{}".format(filenum, args.filename)
    with open(fname, 'w') as file:
        print("Writing {} random nums from {} to {} into file {}"
                .format(args.n, args.min, args.max, fname))
        for random_integer in numbers:
            file.write("{}\n".format(random_integer))
        file.close()
