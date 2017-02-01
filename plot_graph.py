import matplotlib.pyplot as plt

KB = 1024
MB = 1024 * 1024
sizes = [
    512,
    1 * KB,
    4 * KB,
    8 * KB,
    16 * KB,
    32 * KB,
    1 * MB,
    2 * MB,
    4 * MB
]
rates = [
1266.595,
1325.547,
1436.329,
1621.138,
1584.034,
1450.422,
1557.741,
1684.971,
1251.363
]

rates2 = [
3563.512,
3144.268,
3091.049,
3576.690,
3359.486,
2911.598,
3520.336,
3354.226,
3519.494
]

plt.title("Data rate for read_blocks_seq vs read_ram_seq")
plt.xlabel("Size in Bytes")
plt.ylabel("Data Rate in MBPS")
plt.plot(sizes, rates)
plt.plot(sizes, rates2)
plt.show()
