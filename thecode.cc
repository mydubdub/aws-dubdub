// QRS Algorithm and Peak Detection with Tracing
// Modified by SamAndrew 
// 2020

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>     /* atoi */
#include <math.h>

#include "LowPassFilter.h"
#include "Derivative.h"
#include "MovingWindowIntegral.h"
#include "getPwave.h"
#include "getPwaveAvg.h"
#include "getQwave.h"
#include "getSDANN.h"
#include "getNumOfPeak.h"
#include "getRMSSD.h"

using namespace std;

int LowPassFilter(int data);
int Derivative(int data);
int MovingWindowIntegral(int data);

int getNumOfPeak(int sample[], int k, char fileNamePeaks[], char fileNamePeakTracks[], int ymax);
int getPwave(int dataLPF[], int rPeaks[], int q, char fileNamePWaves[], char fileNamePs[], char fileNameRLPFs[], char fileNameRLPFps[], int isoAvg);
int getPwaveAvg(int dataLPF[], int numOfSample);
int getQwave(int dataLPF[], int Rx[], int q, int sR, char fileNameQS[],  float DW[], int buffS[], int buffQ[]);
float getSDANN(float miu, int N, int bufferRR[]);
float getRMSSD(int N, int bufferRR[]);

// int bufferR[100000];  //y-value or all valid RR peaks (no x-value)
int bufferRRV[100000];
// int bufferP[100000];
float distanceWeight[100000];
int bufferS[100000];
int bufferQ[100000];

int main(int argc, char* argv[])
{
	// ensure proper usage
	//  if (argc != 3)
	//  {
	//  	printf("Usage: ./t200qs <amplifiction> <filename>\n");
	//  	printf("Source filename read-15sp250qs\n");
	//  	printf("rConstant = 11\t");
	//  	printf("pqD = 40\t");
	//  	printf("rSample = 14\n");
	//  	return -1;
	//  }

	//  char fileName[] = "05pfna";
	//  int amp = 10;
	//  int samplingRate = 200;

	//  cout << "\nEnter file Name (without the .csv): ";
	//  cin >> fileName;

	//  cout << "Enter Amplification: ";
	//  cin >> amp;

	//  cout << "Enter Sampling Frequency: ";
	//  cin >> samplingRate;

	// remember filename
	 char* fileName = argv[3];
	 int samplingRate = atoi(argv[2]); //atoi(argv[2]);
	 int amp = atoi(argv[1]); // default 2

	char fileNameRaw[50]; 
        strcpy(fileNameRaw, fileName);
        strcat(fileNameRaw, ".csv");
	
	// low-pass filtered
	char fileNameLPF[50]; 
        strcpy(fileNameLPF, fileName);
        strcat(fileNameLPF, "LPF.csv");
	
	// tompkins graph result
	char fileNameQRS[50]; 
        strcpy(fileNameQRS, fileName);
        strcat(fileNameQRS, "QRS.csv");
	
	// R-peak on tompkins
	char fileNamePeak[50]; 
        strcpy(fileNamePeak, fileName);
        strcat(fileNamePeak, "Peak.csv");

	// R-peak tracking decision on tompkins
	char fileNamePeakTrack[50]; 
        strcpy(fileNamePeakTrack, fileName);
        strcat(fileNamePeakTrack, "PeakTrack.csv");

	// tracing P-Q {?}
	char fileNamePWave[50]; 
        strcpy(fileNamePWave, fileName);
        strcat(fileNamePWave, "PWave.csv");

	// detected P-waves
	char fileNameP[50]; 
        strcpy(fileNameP, fileName);
        strcat(fileNameP, "P.csv");

	// tracing Q-R-S (?)
	char fileNameRLPF[50]; 
        strcpy(fileNameRLPF, fileName);
        strcat(fileNameRLPF, "RLPF.csv");

	// R-peak decision
	char fileNameRLPFp[50]; 
        strcpy(fileNameRLPFp, fileName);
        strcat(fileNameRLPFp, "RLPFp.csv");

	// valid R-R
	char fileNameRR[50]; 
        strcpy(fileNameRR, fileName);
        strcat(fileNameRR, "RR.csv");

	// invalid R-R
	char fileNameinRR[50];
        strcpy(fileNameinRR, fileName);
        strcat(fileNameinRR, "inRR.csv");

	// activity log
	char fileNameLog[50]; 
        strcpy(fileNameLog, fileName);
        strcat(fileNameLog, "log.csv");

	// detected Q-wave & S-wave
	char fileNameQS[50]; 
        strcpy(fileNameQS, fileName);
        strcat(fileNameQS, "QS.csv");

	char fileNamePre[50]; 
        strcpy(fileNamePre, fileName);
        strcat(fileNamePre, "Pre.csv");
		
	// counting how many lines in the .csv file (basically number of samples)
	string line;
	ifstream myfile (fileNameRaw);
	ifstream myfilecheck (fileNameRaw);
	cout << fileNameRaw << endl;
    
    	long l = 0; //File lines
        if (myfilecheck.is_open())
        {
            
            while (getline (myfilecheck,line) )
            {
                l++;
            }
            cout << "\nline = " << (l-1) << endl;
            myfilecheck.close();
        }

	//below aren't further used in this prog.
	int lineV = (int) (l/2)-1;
	cout << "lineV = " << lineV << endl;
	
	int bufferLPF[l];
	int bufferDrv[l];
	int buffer[l];
	int lineInt[l]; //l*2
	int bufferData[l];
	// int baris[l];
	int dataInt = 0;
	int z = 0, y = 0;
	
	FILE* temp = fopen(fileNameLPF, "w");
	FILE* temp1 = fopen(fileNameQRS, "w");
	FILE* tempR = fopen(fileNameRR, "w");
	FILE* fileLog = fopen(fileNameLog, "w");
	FILE* invalidR = fopen(fileNameinRR, "w");
	FILE* preDiagnose = fopen(fileNamePre, "w");
	//FILE* temp2 = fopen(fileNamePeak, "w");

	// cout << "Sampling rate = " << samplingRate << " Hz" << endl;
    	cout << "Ready to load next file..." << endl;
  	if (myfile.is_open()) 
	  {
		cout << "Reading file... ";
		
   		while ( getline (myfile,line) )
		   {
			istringstream (line) >> lineInt[z];
			//cout << z << '\t' << lineInt[z] << endl;
			z++;
   		}

		cout << " completed.\n";
		myfile.close();
  	} else {
		cout << "Unable to open file";
	} 

	for (int j = 0; j < l-1; j++)
	{
	//y = (j * 1) + 3; // second column +3, third column +4
	//bufferData[j] = (lineInt[y]);			
		
		// Modifying & amplifying samples
		// 12 bit = 4096 (+2048 to -2048). offset 2048 so that all data is positive & /4 to make it 10 bit
	//dataInt = (2048 + bufferData[j])/4;	
		//cout << j << '\t' << y << '\t' << lineInt[y] <<'\t' << bufferData[j] << '\t' << dataInt << endl;

		// Implementing the filters and tompkins algorithm
	//bufferLPF[j] = LowPassFilter(dataInt*amp);
		bufferLPF[j] = LowPassFilter(lineInt[j]*4);

		fprintf(temp, "%i\t", (j*1000)/samplingRate); //lineInt[j]
		fprintf(temp, "%i\n", bufferLPF[j]); // save LPF to file

		bufferData[j] = Derivative(bufferLPF[j]); // derivation		
		bufferData[j] = pow(bufferData[j],2); // squaring
		bufferData[j] = MovingWindowIntegral(bufferData[j]);
		
		fprintf(temp1, "%i\t", j);
		fprintf(temp1, "%i\n", bufferData[j]); // save tompkins line to file
	}

	// Peak Detection
	int p = 0, pwave = 0, qwave = 0, yMax = (500*amp)/2, isolineAvg = 0, num = 750;
	int rr1 = 0, rr2 = 0, rr = 0, rrv = 0, pv = 0, rrd=0, rra=0, rrb=0;
	float avg = 0, rri = 0, hr = 0, sdann = 0, rMSSD = 0, AA=0, sum=0;
	z = (z/2)+1;	
	z++;
	//cout << z << endl;
	
	p = getNumOfPeak(bufferData, z*2, fileNamePeak, fileNamePeakTrack, yMax); //z*2
	fprintf(tempR, "R-Peak Detected	 	RR Interval(ms)	Abnormal or Normal\n");
	for (int j = 0; j < (p-1); j++) 
	{
		rr2 = bufferR[j+1];
		rr1 = bufferR[j];
		rr = rr2 + (rr1*(-1));

		rra = (rr2*1000)/samplingRate;
		rrb = (rr1*1000)/samplingRate;
		rrd = rra-rrb;

		if ((rr > (int)((3*samplingRate)/10)) && (rr < 2*samplingRate)) // To limit only 30-200 BPM, eliminate outlier
        {
			bufferRRV[j] = rr;
			
			fprintf(tempR, "%i\t", rra); // save valid R-R to file
			fprintf(tempR, "%i\t", rrb); // original rr
			//cout << "RR = " << rr << endl;
			hr = (60*samplingRate)/rr;
			rri += hr;
			fprintf(tempR, "%i\t", rrd); // original (int)hr

			if(rrd>1200){
			fprintf(tempR, "Big Interval (RR>1200ms)\n");
			}else if (rrd<600){
			fprintf(tempR, "Small Interval (RR<600ms)\n");
			}else{
			fprintf(tempR, "Normal Interval\n");
			};
			
			rrv++;
			
		} else {
			rr1 = rr1 - (0.5*samplingRate);
			rr2 = rr2 + (0.5*samplingRate);
			//cout << "RR1 = " << rr1;
			//cout << "\tRR2 = " << rr2 << endl;
			
			for (rr1; rr1 <= rr2; rr1++) {
				fprintf(invalidR, "%i\t", rr1); // save invalid R-R to file
				fprintf(invalidR, "%i\n", bufferData[rr1]);
			}
		}
		sum += rrd;
	}	AA= sum/rrv;

	fprintf(fileLog, "Filename	Amplification	SamplingRate(Hz)	ANN(bpm)	RR Interval(ms)	SDANN(ms)	rMSSD(ms)	R-peak	ValidRR	P-wave	P-RValid	QS-waves\n"); //title
	fprintf(fileLog, "%s\t", fileName);
	fprintf(fileLog, "%i\t", amp);	
	fprintf(fileLog, "%i\t", samplingRate);	
	
	avg = rri / (rrv-1); // (p-1)
	
	if(avg<40){
		cout << ">>> WARNING\t";
	} else if(avg<60){
		cout << ">>> BRADY\t";
	} else if(avg>100){
		cout << ">>> TACHY\t";
	}
	
	cout << "Average HR = " << (int)avg << " bpm." << endl;
	cout << "ANN = " << avg << " bpm." << endl;	// miu for SDANN
	cout << "RR Interval = " << AA << " ms." << endl;
	fprintf(fileLog, "%f\t", avg);
	fprintf(fileLog, "%f\t", AA);
	
	sdann = getSDANN(avg, rrv, bufferRRV);
	
	if(sdann > 195){
		cout << ">>> HIGH SDANN\t";
	}else if (sdann < 64){
		cout << ">>> LOW SDANN\t";
	}else{
		cout << ">>> NORMAL SDANN\t";
	};
	
	cout << "SDANN = " << sdann << endl;	// N for SDANN https://www.sciencedirect.com/science/article/pii/S0735109797005548
	fprintf(fileLog, "%f\t", sdann);

	if(rMSSD > 58){
	cout << ">>> HIGH rMSSD\t";
	}else if (rMSSD < 14){
		cout << ">>> LOW rMSSD\t";
	}else{
		cout << ">>> NORMAL rMSSD\t";
	};
	
	rMSSD = getRMSSD(rrv, bufferRRV);
	cout << "RMSSD = " << rMSSD << endl;	// N for SDANN
	fprintf(fileLog, "%f\t", rMSSD);
	
	isolineAvg = getPwaveAvg(bufferLPF, num);
	
	cout << "R Peak detected = " << p;		// N for SDANN
	fprintf(fileLog, "%i\t", p);
	
	cout << "\tRR valid = " << rrv << endl;	// N for SDANN
	fprintf(fileLog, "%i\t", rrv);
	
	pwave = getPwave(bufferLPF, bufferR, p, fileNamePWave, fileNameP, fileNameRLPF, fileNameRLPFp, isolineAvg, samplingRate);
	cout << "P Wave detected = " << pwave;
	fprintf(fileLog, "%i\t", pwave);
	
	for (int j = 0; j < (p-1); j++) 
	{
		rr2 = bufferP[j+1];
		rr1 = bufferP[j];
		rr = rr2 + (rr1*(-1));
		
		if ((rr > (int)((3*samplingRate)/10)) && (rr < 2*samplingRate)) // To limit only 30-190 BPM, eliminate outlier
        {
			pv++;
		}
	}
	
	cout << "\tP Wave valid = " << pv;
	fprintf(fileLog, "%i\t", pv);
	cout << "\tP-R valid = " << (rrv-pv) << endl;
	
	qwave = getQwave(bufferLPF, bufferR, p, samplingRate, fileNameQS, distanceWeight, bufferS, bufferQ);
	cout << "Flags detected = " << qwave << "\n" << endl;
	fprintf(fileLog, "%i\n", qwave);

	fprintf(preDiagnose, "HR(bpm)	RR Interval(ms)	SDANN(ms)	rMSSD(ms)	Ratio of P-wave : R-peak\n");

	if(avg<40){
	fprintf(preDiagnose, "WARNING (HR<40)\t");
	} else if(avg<60){
	fprintf(preDiagnose, "BRADY (HR<60)\t");
	} else if(avg>100){
	fprintf(preDiagnose, "TACHY (HR>100)\t");
	}else {
	fprintf(preDiagnose, "NORMAL HR\t");
	};

	if(AA<600){
	fprintf(preDiagnose, "LOW RR INTERVAL (RR<600ms)\t");
	} else if(AA>1200){
	fprintf(preDiagnose, "HIGH RR INTERVAL (RR>1200ms)\t");
	}else {
	fprintf(preDiagnose, "NORMAL RR INTERVAL\t");
	};

	// https://www.sciencedirect.com/science/article/pii/S0735109797005548
	if(sdann>195){
	fprintf(preDiagnose, "HIGH SDANN (SDANN>195)\t");
	}else if (sdann<64){
	fprintf(preDiagnose, "LOW SDANN (SDANN<64)\t");
	}else{
	fprintf(preDiagnose, "NORMAL SDANN\t");
	};

	if(rMSSD>58){
	fprintf(preDiagnose, "HIGH rMSSD (rMSSD>58)\t");
	}else if (rMSSD<14){
	fprintf(preDiagnose, "LOW rMSSD (rMSSD<14)\t");
	}else{
	fprintf(preDiagnose, "NORMAL rMSSD\t");
	};

	fprintf(preDiagnose, "%i", pwave);
	fprintf(preDiagnose, " : %i", p);

	// fprintf(fileLog, "Filename	Amplification	SamplingRate(Hz)	AvgHR(bpm)	SDANN	RMSSD	R-peak	ValidRR	P-wave	ValidP	P-RValid \n"); //title
	// fprintf(fileLog, "%s\t", fileName);
	// fprintf(fileLog, "%i\t", amp);	
	// fprintf(fileLog, "%i\t", samplingRate);	
	
	return 0;
}