double power(double x, int n){
	if (x == 0) return 0;
	if (n < 0) return power(1.0/x, -n);
	if (n == 0) return 1.0;
	if (n % 2 == 0) return power(x, n/2)*power(x, n/2); // power n is odd.
	else return x*power(x, n/2)*power(x, n/2); // power n is even.
}
