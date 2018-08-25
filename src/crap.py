n, r, k = [int(x) for x in input().strip().split(" ")]
a = [int(x) for x in input().strip().split(" ")]

i = 0
j = 0
s = 0
res = []

print("n {} r {} k {}".format(n, r, k))
for i in range(r):
	s += a[i]

for i in range(n):
	if i + r < n:
		s += a[i + r]
	if i > r:
		s -= a[i - r - 1]
	res.append(s)

#print("\n".join(map(str, res)))
print("{}".format(min(res)));
