from subprocess import call

KB = 1024
MB = 1024 * 1024
sizes = (  
	512,
	1 * KB,
	4 * KB,
	8 * KB,
	16 * KB,
	32 * KB,  
	1 * MB,
	2 * MB,
	4 * MB
)


print("Writing write_blocks_seq")
for i in sizes:
	print("\nBlock Size: " + str(i))
	call(["./write_blocks_seq", "edges.csv", str(i)])

print("Writing write_lines")
for i in sizes:
	print("\nBlock Size: " + str(i))
	call(["./write_lines", "edges.csv", str(i)])

print("Reading read_blocks_seq\n")
for i in sizes:
	print("\nBlock Size: " + str(i))
	call(["./read_blocks_seq", "records.dat", str(i)])

print("\n")
print("Reading read_ram_seq\n")
for i in sizes:
	print("\nBlock Size: " + str(i))
	call(["./read_ram_seq", "records.dat", str(i)])

print("\n")
print("Reading read_blocks_rand\n")
for i in sizes:
	print("\nBlock Size: " + str(i))
	call(["./read_blocks_seq", "records.dat", str(i), "10"])

print("\n")
print("Reading read_ram_rand\n")
for i in sizes:
	print("\nBlock Size: " + str(i))
	call(["./read_ram_rand", "records.dat", str(i), "10"])
