#ifndef DIODO
#define DIODO

#include "components.cpp"
#include <math.h>

using namespace std;

class Diodo: public Components
{
  public:
    Diodo(string n, int a, int b, double i,double v) : Components(n, a, b)
    {
        setLinearidade(false);
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

    double getResistencia(double v)
    {
      if (v < 0.8)
      {
        double resistenciaNL = getTensaoVT()/(getCorrenteS()*exp(v/getTensaoVT()));
        return resistenciaNL;
      }else{
        /**
        * na parte linear, a derivada no ponto 0.8 é a condutancia da parte linear
        *do diodo
        */
        double resistencia = getTensaoVT()/(getCorrenteS()*exp(0.8));
        return resistencia;
      }
    }

    double getCorrente(double v)
    {

      double corrente1 = getCorrenteS()*(exp(v/getTensaoVT())-1);
      double corrente2 = v/getResistencia(v);

      return corrente1 - corrente2;
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
        vector <int> L,
        vector <int> C,
        vector<double> resultado)
    {
        /**
         * Pega a tensao no ramo no instante de tempo anterior
         */
        double tensaoRamo = resultado[C[getNoA()]] - resultado[C[getNoB()]];
        /**
         * Descarta o no Zero uma vez que ele e linearmente dependente
         */
        if (C[getNoA()] == 0) {
            tensaoRamo = -1*resultado[C[getNoB()]];
        }
        if (C[getNoB()] == 0) {
            tensaoRamo = resultado[C[getNoA()]];
        }
        condutancia[L[getNoA()]][C[getNoA()]] += 1/getResistencia(tensaoRamo);
        condutancia[L[getNoB()]][C[getNoB()]] += 1/getResistencia(tensaoRamo);
        condutancia[L[getNoA()]][C[getNoB()]] += -1/getResistencia(tensaoRamo);
        condutancia[L[getNoB()]][C[getNoA()]] += -1/getResistencia(tensaoRamo);

        if (tensaoRamo < 0.8)
        {
          correntes[L[getNoA()]] += -1*getCorrente(tensaoRamo);
          correntes[L[getNoB()]] += getCorrente(tensaoRamo);
        }
      }

    void desestampar(vector<vector<double> >& condutancia,
        vector<double>& correntes,
        vector<double> resultado)
        {
          /**
           * Pega a tensao no ramo no instante de tempo anterior
           */
          double tensaoRamo = resultado[getNoA()] - resultado[getNoB()];
          /**
           * Descarta o no Zero uma vez que ele e linearmente dependente
           */
          if (getNoA() == 0) {
              tensaoRamo = -1*resultado[getNoB()];
          }
          if (getNoB() == 0) {
              tensaoRamo = resultado[getNoA()];
          }

          condutancia[getNoA()][getNoA()] += -1/getResistencia(tensaoRamo);
          condutancia[getNoB()][getNoB()] += -1/getResistencia(tensaoRamo);
          condutancia[getNoA()][getNoB()] += 1/getResistencia(tensaoRamo);
          condutancia[getNoB()][getNoA()] += 1/getResistencia(tensaoRamo);

          if (tensaoRamo < 0.8)
          {
            correntes[getNoA()] += getCorrente(tensaoRamo);
            correntes[getNoB()] += -1*getCorrente(tensaoRamo);
          }

        }

  private:

    double correnteS;

    double tensaoVT;

};

#endif
