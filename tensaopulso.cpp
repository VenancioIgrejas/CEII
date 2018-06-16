#ifndef TENSAOPULSO
#define TENSAOPULSO

/**
 * Modelo basico componentes de fontes de pulso
 */
#include "pulso.cpp"

/* Necessario para nao precisar escrever std:: */
using namespace std;

class TensaoPulso : public Pulso
{

    public:
        /**
         * Construtor
         */
        TensaoPulso(string n, int a, int b,
            double a1, double a2, double delay,
            double tSub, double tDes, double tOn,
            double per, double cic, double t, double p) :
            Pulso(n, a, b, a1, a2, delay, tSub, tDes, tOn, per, cic, t, p)
        {
        }

        /**
         * Retorna corrente que auxiliar
         * para analise nodal modificada
         */
        string getAuxNode()
        {
            return "j" + getNome();
        }

        /**
         * Estanpa da matriz nodal modificada fonte de tensao
         * @param condutancia matriz de condutancia
         * @param correntes   matriz de correntes
         * @param nodes       matriz de nos
         */
        void estampar(vector<vector<double> >& condutancia,
            vector<double>& correntes,
            vector<string> nodes,
            vector<int> L,
            vector<int> C,
            vector<double> resultado)
        {
            vector<string>::iterator it;
            it = find(nodes.begin(), nodes.end(), getAuxNode());
            auto pos = it - nodes.begin();
            condutancia[L[0]][C[getNoA()]] += 1;
            condutancia[L[pos]][C[getNoB()]] += 1;
            condutancia[L[0]][C[getNoB()]] += -1;
            condutancia[L[pos]][C[getNoA()]] =+ -1;
            correntes[L[pos]] += -1*getValor();
            correntes[L[0]] += getValor();
            //condutancia[getNoA()][pos] += 1;
            //condutancia[getNoB()][pos] += -1;
            //condutancia[pos][getNoA()] += -1;
            //condutancia[pos][getNoB()] += 1;
            //correntes[pos] += -1*getValor();
        }
};

#endif
