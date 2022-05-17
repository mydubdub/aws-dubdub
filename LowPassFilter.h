using namespace std;

int LowPassFilter(int data) {
	
	static int y1 = 0, y2 = 0, x[26], n = 12;
	int y0;
	x[n] = x[n + 13] = data;
	y0 = (y1 << 1) - y2 + x[n] - (x[n + 6] << 1) + x[n + 12];
	y2 = y1;
	y1 = y0;
	y0 >>= 5;
	
	if(--n < 0)
		n = 12;
	return y0;
}