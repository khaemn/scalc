#!/usr/bin/env python3

max_number = 1000000

zero = [0]
nonzero = [i for i in range(1, max_number)]
naturals = [i for i in range(0, max_number)]
odds = [i*2 + 1 for i in range(0, max_number // 2)]
evens = [i*2 for i in range(0, max_number // 2)]
empty = []

for (name, numbers )in zip(["empty", "zero", "nonzero", "naturals", "odds", "evens"],
                            [empty, zero, nonzero, naturals, odds, evens]):
    fname = "{}.txt".format(name)
    with open(fname, 'w') as file:
        for num in numbers:
            file.write("{}\n".format(num))
        file.close()
