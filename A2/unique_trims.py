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
    key = record[1][:-10]
    mr.emit_intermediate(key, 0)

def reducer(key, list_of_values):
    # key: word
    # value: list of occurrence counts
    mr.emit(key) # remove duplicates from list_of_values

# Do not modify below this line
# =============================
if __name__ == '__main__':
    #inputdata = open(sys.argv[1])
    # The line below is to test it in wing ide
    inputdata = open('c:\\Users\\ashiq\\Downloads\\csc443_winter_2017-master\\csc443_winter_2017-master\\a2_mapreduce\\input\\dna.json')
    mr.execute(inputdata, mapper, reducer)
