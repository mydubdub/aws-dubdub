using namespace std;

int bufferR[100000];

int getNumOfPeak(int sample[], int k, char fileNamePeaks[], char fileNamePeakTracks[], int ymax) {

	// the inputs from main method are bufferData, z (line number), fileNamePeak, fileNamePeakTrack, yMax
	
	//int ymax = 511;
	int z = 0;
	int m = 0;
	int r = 0;
	int sam = 0, sumSam = 0, avgSam = 0;
	int prev = 0, next = 0, delta = 0;
	FILE* peakFile = fopen(fileNamePeaks, "w");
	FILE* trackPeak = fopen(fileNamePeakTracks, "w");
	fprintf(trackPeak, "R-peak trace Location\n");
	for (int j = 2; j <= k; j++) {

		//if (((sample[j] - sample[j-4]) < 5) && ((sample[j] - sample[j+2]) <= 1) && (sample[j+10] > (ymax >> 1)))
		if (((sample[j] - sample[j-4]) < 500) && (sample[j+10] > (ymax >> 1))) {
		
			sam = sample[j];
			
			fprintf(trackPeak, "%i\t", j);
			fprintf(trackPeak, "%i\n", sample[j]);

			if (sam > ymax) {// half the max peak of QRS
			
				next = j;
				delta = (next - prev);
					
				if (delta > 62) {// 240 bpm = 4 beats/sec, 250 Hz = 250 samples/sec, 250/4 = 62
				
					bufferR[r] = j;
					r++;
					z = r;
					//cout << sam << "\t" << j << endl;
					
					fprintf(peakFile, "%i\t", j);
					fprintf(peakFile, "%i\n", sam);
					sumSam = sumSam + sam;
					avgSam = sumSam/r;
				}
				prev = next;
				m++;
			}
		}
	}
	//cout << "Avg. QRS Peak = " << avgSam << endl;
	return z;
}