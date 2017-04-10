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
    mr.emit_intermediate(record[0], record[1])

def reducer(key, list_of_values):
    # key: word
    # value: list of occurrence counts
    count = 0
    for v in list_of_values:
        if v not in mr.intermediate.keys():
            mr.emit((v, 0))
        elif key in mr.intermediate[v]:
            count += 1
    mr.emit((key, count))

# Do not modify below this line
# =============================
if __name__ == '__main__':
    inputdata = open(sys.argv[1])
    # The line below is to test it in wing ide
    #inputdata = open('C:\\Users\\Shervin\\Desktop\\csc443\\csc443-A1\\A2\\input\\friends.json')
    mr.execute(inputdata, mapper, reducer)
