''' Warning: It will take awfully long time to run this script. So we tested 
it in parts. So uncomment the required parts '''


import subprocess as s

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

write_blocks_seq = []
write_lines = []
read_blocks_seq = []
read_ram_seq = []
read_ram_rand = []
read_blocks_rand = []
write_blocks_rand = []
write_ram_rand = []
x = ""
attempt = [1,2,3,4,5]
time = 0.0

"""
print("===Testing write_lines===")
for i in attempt:
	x = s.check_output(["./write_lines", "edges.csv"])
	print("\nAttempt " + str(i) + ": " + x.strip())
	time += float(x.split()[2])
print("Average speed is " + str(time / len(attempt)) + " MBPS")
write_lines.append(time / len(attempt))	

for i in sizes:
	print("===Testing write_blocks_seq===")
	print("\nBlock Size: " + str(i))
	time = 0.0
	for j in attempt:
		x = s.check_output(["./write_blocks_seq", "edges.csv", str(i)])
		print("Attempt " + str(j) + ": " + x.strip())
		time += float(x.split()[2])
	print("\n Average speed is " + str(time / len(attempt)) + " MBPS")
	write_blocks_seq.append(time / len(attempt))
	
	print("===Testing read_blocks_seq===")
	time = 0.0
	print("\nBlock Size: " + str(i))
	for j in attempt:
		s.check_output(["./write_blocks_seq", "edges.csv", str(i)])
		x = s.check_output(["./read_blocks_seq", "records.dat", str(i)])
		print("Attempt " + str(j) + ": " + x.strip())
		time += float(x.split()[6])
	print("\n Average speed is " + str(time / len(attempt)) + " MBPS")
	read_blocks_seq.append(time / len(attempt))

	print("===Testing read_ram_seq===")
	time = 0.0
	print("\nBlock Size: " + str(i))
	for j in attempt:
		s.check_output(["./write_blocks_seq", "edges.csv", str(i)])
		x = s.check_output(["./read_ram_seq", "records.dat", str(i)])
		print("Attempt " + str(j) + ": " + x.strip())
		time += float(x.split()[6])
	print("\n Average speed is " + str(time / len(attempt)) + " MBPS")
	read_ram_seq.append(time / len(attempt))	

	print("===Testing read_blocks_rand===")
	time = 0.0
	print("\nBlock Size: " + str(i))
	for j in attempt:
		s.check_output(["./write_blocks_seq", "edges.csv", str(i)])
		x = s.check_output(["./read_blocks_rand", "records.dat", str(i)])
		print("Attempt " + str(j) + ": " + x.strip())
		time += float(x.split()[6])
	print("\n Average speed is " + str(time / len(attempt)) + " MBPS")
	read_blocks_rand.append(time / len(attempt))	

	print("===Testing read_ram_rand===")
	time = 0.0
	print("\nBlock Size: " + str(i))
	for j in attempt:
		s.check_output(["./write_blocks_seq", "edges.csv", str(i)])
		x = s.check_output(["./read_ram_rand", "records.dat", str(i)])
		print("Attempt " + str(j) + ": " + x.strip())
		time += float(x.split()[6])
	print("\n Average speed is " + str(time / len(attempt)) + " MBPS")
	read_ram_rand.append(time / len(attempt))
	

	print("===Testing write_blocks_rand===")
	
	time = 0.0
	for j in attempt:
		s.check_output(["./write_blocks_seq", "edges.dat", "1024"])
		x = s.check_output(["./write_blocks_rand", "100000"])
		print("Attempt " + str(j) + ": " + x.strip())
		time += float(x.split()[2])
	print("\n Average speed is " + str(time / len(attempt)) + " MBPS")
	write_blocks_rand.append(time / len(attempt))
	
	print("===Testing write_ram_rand===")
	time = 0.0
	for j in attempt:
		s.check_output(["./write_blocks_seq", "edges.csv", "1024"])
		x = s.check_output(["./write_ram_rand", "100000"])
		print("Attempt " + str(j) + ": " + x.strip())
		time += float(x.split()[2])
	print("\n Average speed is " + str(time / len(attempt)) + " MBPS")
	write_ram_rand.append(time / len(attempt))

"""
print("write_blocks_seq: " + str(write_blocks_seq))
print("write_lines: " + str(write_lines))
print("read_blocks_seq: " + str(read_blocks_seq))
print("read_ram_seq: " + str(read_ram_seq))
print("read_blocks_rand: " + str(read_blocks_rand))
print("read_ram_rand: " + str(read_ram_rand))
print("write_blocks_rand: " + str(write_blocks_rand))
print("write_ram_rand: " + str(write_ram_rand))