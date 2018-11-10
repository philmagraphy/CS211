bool divisible_11(int n, int &s){
	int i = 0, s1 = 0, s2 = 0; // i counts number of digits in n. s1 = odd sum, s2 = even sum. If s1, s2 can be arbitrary, this and first loop not needed.
	for(int m = n; m != 0; m /= 10, i++); // There is definitely a more elegant way to write this function.
	if(i % 2 != 0){ // n has an odd number of digits, so singles-digit is odd.
		for( ; n != 0; n /= 100){
			s1 += n % 10;
			s2 += (n / 10) % 10;
		}
	}
	else { // n has an even number of digits, so singles-digit is even.
		for( ; n != 0; n /= 100){
			s1 += (n / 10) % 10;
			s2 += n % 10;
		}
	}
	s = abs(s1 - s2);
	if(s == 0) return true;
	else if(s < 11) return false;
	n = s;
	return divisible_11(n, s);
}
