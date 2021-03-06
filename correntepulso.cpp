#ifndef CORRENTEPULSO
#define CORRENTEPULSO

/**
 * Modelo basico componentes de fontes de pulso
 */
#include "pulso.cpp"

/* Necessario para nao precisar escrever std:: */
using namespace std;

class CorrentePulso : public Pulso
{

    public:
        /**
         * Construtor
         */
        CorrentePulso(string n, int a, int b,
            double a1, double a2, double delay,
            double tSub, double tDes, double tOn,
            double per, double cic, double t, double p) :
            Pulso(n, a, b, a1, a2, delay, tSub, tDes, tOn, per, cic, t, p)
        {
        }

        /**
         * Estanpa da matriz nodal modificada fonte de corrente
         * @param condutancia matriz de condutancia
         * @param correntes   matriz de correntes
         * @param nodes        matris de nos
         */
        void estampar(vector<vector<double> >& condutancia,
            vector<double>& correntes,
            vector<string> nodes,
            vector<int> L,
            vector<int> C,
            vector<double> resultado)
        {
            correntes[L[getNoA()]] += -1*getValor();
            correntes[L[getNoB()]] += getValor();
        }
};

#endif
