#include <fstream>
#include <sstream>
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
  int level = 0;
  double Lold;
  //int check = 0;
  wfFile >> temp; //do this here so I can get to eof in time
  
  do {

    double *x = new double[N];
    double *y = new double[N];
  
    wfFile  >> temp >> temp >> temp;
    wfFile >> E >>  temp >> temp >> L;
    wfFile >> temp >> temp;

    for(int i=0;i<N;i++) {
	wfFile >> x[i] >> y[i];
	//cout << x[i] << " " <<  y[i]  << endl;
	// if(y[i] == 0) {
	//   check++;
	// }

	y[i] = y[i]*50 +E;
      }

    wfFile >> temp;
    // if(check>N/2;)
    //   break;

    level++;

    if(L!=Lold)
      level=0;
    
    TGraph * gr = new TGraph(N,x,y);
    ostringstream name;
    name << "WF_L=" << L << "_lvl=" << level;
    gr->SetName(name.str().c_str());
    gr->Write();
    
    delete x;
    delete y;
    delete gr;

    Lold = L;

    }  while(!wfFile.eof());
  

}
