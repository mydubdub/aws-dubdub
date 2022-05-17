using namespace std;
int bufferP[100000];

int getPwave(int dataLPF[], int rPeaks[], int q, char fileNamePWaves[], char fileNamePs[], char fileNameRLPFs[], char fileNameRLPFps[], int isoAvg, int sR) {

	int rConstant = 10; //(int) (0.07*s); //18 13 11; //1/sampling; 10
	int pqD = 40; // (int) (0.21*s); //52; //0.21/(1/sampling); 40
	int rSample = 20; //(int) (0.044*s); //11 18 22; // The distance from the peak to the Q wave; 20
	int r = 0; int w = 0;
	int iso = 0;
	int pPeak = 0;
	int pCount = 0;

	int pr = 0;
	int P = 0;
	int R = 0;

	bool dFlag = false;
	bool iFlag = false;
	FILE* trackPWave = fopen(fileNamePWaves, "w");
	FILE* trackP = fopen(fileNamePs, "w");
	FILE* trackRLPF = fopen(fileNameRLPFs, "w");
	FILE* trackRLPFp = fopen(fileNameRLPFps, "w");
	
	fprintf(trackRLPFp, "R-peaks Detected\n");
	fprintf(trackP, "P-waves Detected\n");
	for (int i = 0; i < q; i++) {
		
		r = rPeaks[i] - rConstant;
		// To fine tune the R peak sample
		
		if (dataLPF[r] - dataLPF[r-1] <= 0){
			// heading left-downwards and have to find the turning up point
			//cout << "Right Here" << endl;
			
			for (int qs = 0; qs >= -rSample; qs--){
				//cout << "Up" << endl;
				
				if ((dataLPF[r+qs] - dataLPF[r+qs-1] > 0)){ //&& (dataLPF[r+qs] > (isoAvg))){
				// R peak is in the correct position
				//Isoelectric line is r
				//cout << "Here" << endl;
				
					if (iFlag == false){
						// r+qs is the peak in x axis
						iso = r+qs - rSample;
						iFlag = true;
						
						fprintf(trackRLPFp, "%i\t", ((r+qs)*1000)/sR);
						fprintf(trackRLPFp, "%i\n", dataLPF[r+qs]);
						//fprintf(trackP, "%i\t", ((r+qs)*1000)/sR);				
						//cout << (r+qs) << "\tUp-down\t" << dataLPF[r+qs] << endl;
						R = ((r+qs)*1000)/sR;
					}
				} else {
					fprintf(trackRLPF, "%i\t", r+qs);
					fprintf(trackRLPF, "%i\n", dataLPF[r+qs]);
				}
			}
			iFlag = false;	
		} else {
			//cout << "Down" << endl;
			
			for (int qs = 0; qs >= -rSample; qs--){

				if ((dataLPF[r+qs] - dataLPF[r+qs-1] > 0) && (dataLPF[r+qs] > (isoAvg+(isoAvg >> 1)))){
				// R peak is in the correct position
				//Isoelectric line is r
				//cout << "Here" << endl;
				
					if (iFlag == false){
						iso = r+qs - rSample; 
						iFlag = true;
						
						fprintf(trackRLPFp, "%i\t", ((r+qs)*1000)/sR);
						fprintf(trackRLPFp, "%i\n", dataLPF[r+qs]);
						//fprintf(trackP, "%i\t", ((r+qs)*1000)/sR);
						//cout << (r+qs) << "\tUp-down\t" << dataLPF[r+qs] << endl;
						R = ((r+qs)*1000)/sR;
					}
				} else if ((dataLPF[r+qs] - dataLPF[r+qs-1] < 0) && (dataLPF[r+qs] < (isoAvg - (isoAvg >> 1)) )){ //&& (dataLPF[r+qs] < (isoAvg >> 1))){ 
				
					if (iFlag == false){
						iso = r+qs - rSample;
						iFlag = true;
						
						fprintf(trackRLPFp, "%i\t", ((r+qs)*1000)/sR);
						fprintf(trackRLPFp, "%i\n", dataLPF[r+qs]);
						//fprintf(trackP, "%i\t", ((r+qs)*1000)/sR);
						//cout << (r+qs) << "\tDown-Up\t" << dataLPF[r+qs] << endl;
						// R = ((r+qs)*1000)/sR;
					}
				} else {
					fprintf(trackRLPF, "%i\t", r+qs);
					fprintf(trackRLPF, "%i\n", dataLPF[r+qs]);
				}
			}
			iFlag = false;	
		}

	// Find the turning point of the curve of PQ duration 0.21 - 0.21 sec //45
		for (int pq = 0; pq >= -pqD; pq--){
		
			if (dataLPF[iso+pq] - dataLPF[iso+pq-1] < 0){
			// It means the curve is going upwards
			
				fprintf(trackPWave, "%i\t", iso+pq);
				fprintf(trackPWave, "%i\n", dataLPF[iso+pq]);
			} else {
			// Curve going downwards and that is the peak
				if ((dataLPF[iso+pq] - dataLPF[iso] > 2)  && (dFlag == false) && ((dataLPF[iso+pq] - dataLPF[iso]) > 30 ) && ((dataLPF[iso+pq] - dataLPF[iso]) < (isoAvg+(isoAvg >> 1))) && (dataLPF[iso+pq] < dataLPF[iso+rSample]) ){
					pPeak = dataLPF[iso+pq];
					bufferP[w] = iso+pq;
					fprintf(trackP, "%i\t", ((iso+pq)*1000)/sR);
					fprintf(trackP, "%i\n", dataLPF[iso+pq]);
					// P = ((iso+pq)*1000)/sR;
					pCount++;
					dFlag = true;
					w++;
				}
			}
		}
		dFlag = false;
		// pr = R - P;
		// if (pr!=0){
		// fprintf(trackP, "%i\t", pr);
		// if(pr>200){
		// 	fprintf(trackP, "Prolonged PR-Interval (PR>200ms)\n");
		// }else if(pr<120){
		// 	fprintf(trackP, "Short PR-Interval (PR<120ms)\n");
		// }else if(120<pr<120){
		// 	fprintf(trackP, "Normal PR-Interval\n");
		// }
		// }
	}
	return pCount;
}