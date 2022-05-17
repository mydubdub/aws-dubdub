using namespace std;

int getQwave(int dataLPF[], int Rx[], int q, int sR, char fileNameQS[], float DW[], int buffS[], int buffQ[]){
	
	int left = 0, right = 0, istop = 0, countS = 0, countQ = 0, xS, xQ;
	float distance, weight, minDW, maxS, maxQ;
	FILE* trackQS = fopen(fileNameQS, "w");
	const double pi = 3.14159265358979323846;
	buffQ[0] = 0;
	fprintf(trackQS, "Q-wave(ms)	S-wave(ms)	QS-width(ms)	Abnormal or Normal\n");
	for(int j = 0; j < q; j++){
		
		minDW = 1000000;
		maxS = 0;
		maxQ = 0;
		xS = 0;
		xQ = 0;
		
		left = Rx[j];
		right = Rx[j+1];

		for(int i = left+1; i < right; i++){
			
			// distance sample point i from RR line
			distance = abs( ((dataLPF[right]-dataLPF[left])*i) - ((right-left)*dataLPF[i]) + (right*dataLPF[left]) - (left*dataLPF[right]) ) / sqrt( pow((dataLPF[right]-dataLPF[left]),2) + pow((right-left),2) );
		
			weight = cos(2*pi*((i-left)/(right-left)));
		
			DW[i] = distance*weight;
			
			if(DW[i] < minDW){
				minDW = DW[i];
				istop = i;
			}
		}
		
		// max DW between stop and Rx[j+1] is Q[j+1]
		for(int iQ = right; iQ > istop; iQ--){
			if(DW[iQ] > maxQ){
				maxQ = DW[iQ];
				xQ = iQ;
			}
		}
		buffQ[j+1] = (((xQ-16)*1000)/sR); //FINE TUNING -16
		//countQ++;
		
		// max DW between Rx[j] and stop is S[j]
		for(int iS = left+1; iS < istop; iS++){
			if(DW[iS] > maxS){
				maxS = DW[iS];
				xS = iS;
			}
		}
		buffS[j] = ((xS*1000)/sR);
		countS++;

		 fprintf(trackQS, "%i\t", buffQ[j]);
		 fprintf(trackQS, "%i\t", buffS[j]);
		
		 int tempWidth = (buffS[j])-(buffQ[j]);
		 if(tempWidth <= 0 || buffS[j] == 0){
		 	tempWidth = 0;
		 }
		
		if(tempWidth != 0){
			
			//cout << buffQ[j] << "\t" << buffS[j] << "\t" << tempWidth << endl;
		
			fprintf(trackQS, "%i\t", tempWidth);
			
			if(tempWidth > 120){ //real deal *sR
			// if(tempWidth > 40){ //only for testing on cu04
				fprintf(trackQS, "Large Width (QS > 120ms)"); // FLAG original buffQ[j]
				countQ++;
			}else if (tempWidth < 80){
				fprintf(trackQS, "Small Width (QS < 80ms)");
				countQ++;
			}else if (80 <= tempWidth <= 120){
				fprintf(trackQS, "Normal Width");
				countQ++;
			}		
		}
		fprintf(trackQS, "\n");
	}
	return countQ;
}