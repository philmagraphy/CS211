int bounding_rect(int n, int x[], int y[], int u[], int v[]){
	int xmax = x[0], xmin = x[0], ymax = y[0], ymin = y[0];
	for(int i = 1; i < n; i++){ // start from i = 1 since min/max values are already set to 1st array elements.
		if(x[i] > xmax) xmax = x[i];
		if(x[i] < xmin) xmin = x[i];
		if(y[i] > ymax) ymax = y[i];
		if(y[i] < ymin) ymin = y[i];
	}
	u[0] = xmin, u[3] = xmin; // bounding rectangle, x-coordinates:
	u[1] = xmax, u[2] = xmax; // u[] = {xmin, xmax, xmax, xmin}
	v[0] = ymin, v[1] = ymin; // bounding rectangle, y-coordinates:
	v[2] = ymax, v[3] = ymax; // v[] = {ymin, ymin, ymax, ymax}
	return (u[2] - u[0])*(v[2] - v[0]); // same as delta-x * delta-y.
}
