#ifndef TENSAODC
#define TENSAODC

/**
 * Modelo basico componentes de fonte DC
 */
#include "dc.cpp"

/* Necessario para nao precisar escrever std:: */
using namespace std;

class TensaoDC : public Dc
{
    public:
        /**
         * Construtor
         */
        TensaoDC(string n, int a, int b, double v) : Dc(n, a, b, v)
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
            //condutancia[getNoA()][pos] += 1;
            //condutancia[getNoB()][pos] += -1;
            //condutancia[pos][getNoA()] += -1;
            //condutancia[pos][getNoB()] += 1;

            correntes[L[pos]] += -1*getValor();
            correntes[L[0]] += getValor();
        }
};

#endif
