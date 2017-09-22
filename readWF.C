#include <fstream>
#include "TGraph.h"

void readWF()
{
  ifstream wfFile("wfs.txt");

  string temp;
  double L;
  int N;
  wfFile >> temp >> temp >> N;
  double E;
  TFile * outFile = new TFile("WF.root","update");

  
  
  do {

    double *x = new double[N];
    double *y = new double[N];
  
    wfFile >> temp >> temp >> temp >> temp;
    wfFile >> E >>  temp >> temp >> L;
    wfFile >> temp >> temp;

    for(int i=0;i<N;i++)
      {
	wfFile >> x[i] >> y[i];
	//cout << x[i] << " " <<  y[i]  << endl;;
	y[i] = y[i]*50 +E;
      }
    TGraph * gr = new TGraph(N,x,y);

    gr->Write();
    
    delete x;
    delete y;
    delete gr;
    

    }  while(!wfFile.eof());
  

}
