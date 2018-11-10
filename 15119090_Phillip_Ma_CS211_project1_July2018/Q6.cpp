bool divisible_9(int n, int &s){
	s = 0;
	for( ; n != 0 ; n /= 10) s += (n % 10);
	if(s <= 9){
		if(s == 9) return true;
		else return false;
	}
	n = s;
	return divisible_9(n, s);
}
