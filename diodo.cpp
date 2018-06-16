#ifdef DIODO
#define DIODO

#include "components.cpp"
#include "resistor.cpp"
#include "correntedc.cpp"

using namespace std;

class Diodo: public Components
{
  public:
    Diodo(string n, int a, int b, double i,double v) : Components(n, a, b)
    {
        setCorrenteS(i);
        setTensaoVT(v);
    }

    void setCorrenteS(double i)
    {
      correnteS = i;
    }

    void setTensaoVT(double v)
    {
      tensaoVT = v;
    }

    double getCorrenteS()
    {
      return correnteS;
    }

    double getTensaoVT()
    {
      return tensaoVT;
    }

    void estampar(vector<vector<double> >& condutancia,
        vector<double>& correntes,
        vector<string> nodes,
        vector<double> resultado)
    {
      resistorDiodo = new Resistor(getNome(),getNoA(),getNoB(),sansdjn);
      resistorDiodo->estampar()
    }

  private:

    Resistor* resistorDiodo;

    double correnteS;

    double tensaoVT
}
