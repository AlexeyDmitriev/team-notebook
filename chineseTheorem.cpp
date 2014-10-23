
r_{ij} = binpow(p[i], p[j] - 2, p[j]);
for (int i=0; i<k; ++i) {
	x[i] = a[i];
	for (int j=0; j<i; ++j) {
		x[i] = r[j][i] * (x[i] - x[j]);
 
		x[i] = x[i] % p[i];
		if (x[i] < 0)  x[i] += p[i];
	}
}
