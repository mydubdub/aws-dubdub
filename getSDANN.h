using namespace std;

float getSDANN(float miu, int N, int bufferRR[]) {

	float sd = 0;
	float var = 0;

	//cout << "miu = " << miu << endl;				// N for SDANN
	//cout << "sample = " << bufferRR[0] << endl;	// N for SDANN

	for(int i = 0; i < N; i++ ) {
		var += (bufferRR[i] - miu) * (bufferRR[i] - miu);
	}
	var /= N;
	sd = sqrt(var);
	
	//cout << "variance = " << var << endl;			// N for SDANN
	//cout << "sdANN = " << sd << endl;				// N for SDANN

	return sd;
}