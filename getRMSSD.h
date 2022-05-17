using namespace std;

float getRMSSD(int N, int bufferRR[]) {

	float rmssd = 0;
	float var = 0;

	//cout << "sample = " << bufferRR[0] << endl;	// N for SDANN

	for(int i = 0; i < N; i++ ) {
		var += ((bufferRR[i] - bufferRR[i+1]) * (bufferRR[i] - bufferRR[i+1]));
	}
	var /= (N - 1);
	rmssd = sqrt(var);
	
	//cout << "root mean = " << var << endl;		// N for SDANN
	//cout << "sdANN = " << sd << endl;				// N for SDANN

	return rmssd;
}