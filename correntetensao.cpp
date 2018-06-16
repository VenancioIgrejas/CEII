#ifndef CORRENTETENSAO
#define CORRENTETENSAO
/**
 * Modelo basico componentes de fontes controladas
 */
#include "fontescontroladas.cpp"

/* Necessario para nao precisar escrever std:: */
using namespace std;

class CorrenteTensao : public FontesControladas
{
    public:
        /**
         * Construtor
         */
        CorrenteTensao(string n, int a, int b, int c, int d, double g) : FontesControladas(n, a, b, c, d, g)
        {
        }

        /**
         * Estanpa da matriz nodal modificada para fonte de corrente controlada por tensao
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
            condutancia[L[getNoA()]][C[getNoC()]] += getGanho();
            condutancia[L[getNoA()]][C[getNoD()]] += -1*getGanho();
            condutancia[L[getNoB()]][C[getNoC()]] += -1*getGanho();
            condutancia[L[getNoB()]][C[getNoD()]] += getGanho();
        }
};

#endif
