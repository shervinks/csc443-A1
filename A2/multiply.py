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
    if record[0] == "a":
        mr.emit_intermediate(record[1], record)
    else:
        for j in range(len(mr.intermediate)):
            mr.emit_intermediate(j, record)  

def reducer(key, list_of_values):
    # key: word
    # value: list of occurrence counts

    #key: j value
    #value: record
    a = {}

    #key: j value
    #value: list of records
    b = {}
    
		#seperate elements by column
    for i in list_of_values:
        if i[0] == "a":
            a.setdefault(i[2], [])
            a[i[2]].append(i)
        else:
            b.setdefault(i[2], [])
            b[i[2]].append(i)

    value = 0

    for k in b:
        for v in b[k]:
            if v[1] in a:
                value += v[3]*a[v[1]][0][3]
        l = []
        l.append(key)
        l.append(k)
        l.append(value)
        value = 0
        mr.emit(l)
    

# Do not modify below this line
# =============================
if __name__ == '__main__':
    inputdata = open(sys.argv[1])
    # The line below is to test it in wing ide
    #inputdata = open('C:\\Users\\Shervin\\Desktop\\csc443\\csc443-A1\\A2\\input\\matrix.json')
    mr.execute(inputdata, mapper, reducer)
