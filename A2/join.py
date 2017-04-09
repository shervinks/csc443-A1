import MapReduce
import sys

"""
Word Count Example in the Simple Python MapReduce Framework
"""

mr = MapReduce.MapReduce()

# =============================
# Do not modify above this line

def mapper(record):
    # key: document identifier
    # value: document contents
    key = record[1]
    mr.emit_intermediate(key, record)

def reducer(key, list_of_values):
    # key: word
    # value: list of occurrence counts
    order = []
    line_item = []
    for t in list_of_values:
        if t[0] == 'order':
            order.append(t)
        else:
            line_item.append(t)
    
    for o in order:
        for l in line_item:
            mr.emit(o + l)
            
# Do not modify below this line
# =============================
if __name__ == '__main__':
    inputdata = open(sys.argv[1])
    # The line below is to test it in wing ide
    #inputdata = open('c:\\Users\\ashiq\\Downloads\\csc443_winter_2017-master\\csc443_winter_2017-master\\a2_mapreduce\\input\\records.json')
    mr.execute(inputdata, mapper, reducer)
