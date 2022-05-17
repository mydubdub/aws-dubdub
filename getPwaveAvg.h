using namespace std;

int getPwaveAvg(int dataLPF[], int numOfSample){

	int avg = 0, sumOfSample = 0;

	for(int i = 0; i < numOfSample; i++){
		sumOfSample = sumOfSample + dataLPF[i];
	}
	
	avg = (sumOfSample/numOfSample);
	cout << "Isoline avg = " << avg << endl;
	
	return avg;
}