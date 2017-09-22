#include <iostream>
#include <fstream>
#include <eigen3/Eigen/Sparse>
#include <eigen3/Eigen/Eigenvalues>
#include <math.h>
#include "TGraph.h"
#include "TFile.h"


using namespace Eigen;
using namespace std;

double potential(double l, double r)
{
  //just for testing, eventually want implementation to  insert whatever potentials I want

  //for now just using Wood-Saxon for 208Pb case
  double mass = 208;
  double R = 1.27*pow(mass,0.33333); //fm
  double A = 0.67; //fm
  double V = 44; //MeV

  return -V/( 1 + exp( (r-R)/A ) ) +20.711666*l*(l+1)/pow(r,2.); //from notes 20.7 this is the conversion factor for hbar^2/2m

  //coulomb potential below
  // double k = 14.3997; // eV*Angstrom/e^2
  // double Z = 1;

  // return -k*Z/r + l*(l+1)/pow(r,2.)*3.809982; //3.8 is the conversion factor for hbar^2/2m for ev/(mass of electron)
  
}

int main()
{
  //MatrixXd m(2,2);
  // m(0,0) = 3;
  // m(1,0) = 2.5;
  // m(0,1) = -1;
  // m(1,1) = m(1,0) + m(0,1);
  // std::cout << m << std::endl;

  ofstream wf("wfs.txt");
  
  int N=500;
  double h = 0.05;

  wf << "N = " << N << "\n";
  
  MatrixXd H(N,N);
  
  // for(int i=0;i<N;i++)
  //   {
  //     cout << "V(" << i*h << ") = " <<  potential(i*h) << endl;
  //   }

  double hbar2_2m = 20.7116666;
  //hbar2_2m = 3.809982;
  
  //just start with L=0 case
  int L=0;
  double x[N];
  double y[N];
  double V[N];

  //just for testing
  for(int i=0;i<N;i++)
    {
      V[i] = potential(L,(i+1)*h);
    }
  //int check=1;
  //check increments if no eigenvalue < 0 is found. If no eigenvalue < 0 is found the loop should terminate
  //doesn't work
  
  for(int L=0;L<1;L++)
    {
      
  //boundary condition at beginning

  if ( L % 2== 0 )
    {
      H(0,0) = hbar2_2m*3/pow(h,2.)+potential(L,h);
    }
  else
    {
      H(0,0) = hbar2_2m/pow(h,2.)+potential(L,h);
    }

  H(0,1) = -hbar2_2m/pow(h,2.);

  cout << "building Hamiltonian for L = " << L << "..." << endl;
  for(int i=1;i<(N-1);i++)
    {
      H(i,i-1) = -hbar2_2m/pow(h,2.);
      H(i,i) = hbar2_2m*2/pow(h,2.) + potential(L,(i+1)*h);
      H(i,i+1) = -hbar2_2m/pow(h,2.);
    }

  //boundary condition at end
  H(N-1,N-1) = potential(L,N*h);
  H(N-2,N-1) = 0.;

  
  cout << "finding eigen-values(vectors)..." << endl;
  
  EigenSolver<MatrixXd> Hamiltonian(H);
  MatrixXcd wavefunctions = Hamiltonian.eigenvectors();
  
 
  cout << "The bound state energies of H for L = " << L << " are:" << endl;
  for(int i=0;i<N;i++)
    {
      x[i] = (i+1)*h;
      //check++;
      if(real(Hamiltonian.eigenvalues()[i]) < 0.) //I guess eigenvalues are type complex<double>
	{
	  //check=1.;
	  wf << "Quantum #s: " << "E = " << real(Hamiltonian.eigenvalues()[i]) << " L = " << L << "\n";
	  wf << "r \t r*psi(r)\n";
	  cout << real(Hamiltonian.eigenvalues()[i]) << endl;
	  //cout << "writing wavefunction [u(r)] to file..." << endl;
	    for(int j=0;j<N;j++)
	      {
		y[j] = real(wavefunctions(j,i));
		wf << (j+1)*h << "\t" << y[j] << "\n";
	      }
	  
	}
    }

    }
  TFile * outFile = new TFile("WF.root","RECREATE");
  TGraph * gr1 = new TGraph(N,x,V);
  gr1->Write();
  
}
