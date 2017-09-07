#ifndef potential_
#define potential_

using namespace std;



class potential
{
 public:
  potential(int type); //WS = 0, Coulomb = 1, HO = 2;
  double V(double r);

 private:
  double V0;
  double A0;
  double R0;

}


  
