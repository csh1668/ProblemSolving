
num = 0

f1, f2 = open(f'random{num}.out'), open(f'random{num}.ans')

for i, (l1, l2) in enumerate(zip(f1.readlines(), f2.readlines())):
    if l1 != l2:
        print('wrong at:', i, l1, 'vs', l2)