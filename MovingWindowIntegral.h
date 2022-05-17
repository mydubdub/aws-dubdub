using namespace std;

int MovingWindowIntegral(int data) {

	static int x[32], ptr = 0;
	static long sum = 0;
	long ly;
	int y;

	if(++ptr == 32) {
		ptr = 0;
	}
	sum -= x[ptr];
	sum += data;
	x[ptr] = data;
	ly = sum >> 5;
	
	if(ly > 32400){	
		/*check for register overflow*/
		y = 32400;
	} else {
		y = (int) ly;
	}
	return y;
}