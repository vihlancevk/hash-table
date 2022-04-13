from cProfile import label
import numpy as np
import matplotlib.pyplot as plt

finput = open( "/home/kostya/hashTable/res/outputHashTable.txt", "r" )

is_name = 0
hash_function_name = ""
data = []
for line in finput:
    if ( is_name == 0 ):
        hash_function_name = line
        hash_function_name = hash_function_name.rstrip( "\n" )
        is_name = 1
        continue
    data.append( [int(x) for x in line.split()] )

finput.close()

index_size  = len( data[0] ) // 2
values_size = len( data[0] ) // 2
index  = [0 for x in range( index_size  )]
values = [0 for x in range( values_size )]

pos_index  = 0
pos_values = 0
for i in range( len( data[0] ) ):
    if i % 2 == 0:
        index[pos_index] = data[0][i]
        pos_index = pos_index + 1
    else:
        values[pos_values] = data[0][i]
        pos_values = pos_values + 1

plt.title( hash_function_name )
plt.bar( index, values, width = 1.0, bottom = 0 )
plt.show() 
