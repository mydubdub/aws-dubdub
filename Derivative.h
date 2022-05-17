using namespace std;

int Derivative(int data) {

	int y, i;
	static int x_derv[4];

	/*y = 1/8 (2x( nT) + x( nT - T) - x( nT - 3T) - 2x(nT - x(4T))*/
	
	y = (data << 1) + x_derv[3] - x_derv[1] - ( x_derv[0] << 1);
	y >>= 3;

	for (i = 0; i < 3; i++) {
		x_derv[i] = x_derv[i + 1];
		x_derv[3] = data;
	}
	return y;
}