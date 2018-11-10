double sqrt_Babylonian(double a){
	double guess0 = 0, guess1 = 1; // guess0 = last guess, guess1 = current guess; initial guess is always 1.
	for( ; abs(guess1 - guess0) > 0.0001; ){ // I just like for-loops.
		guess0 = guess1;
		guess1 = (guess1 + (a/guess1))/2;
		cout << fixed << showpoint;
		cout << setprecision(4);
		cout << guess1 << endl;
	}
	return guess1;
}
