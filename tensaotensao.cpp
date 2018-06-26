#ifndef TENSAOTENSAO
#define TENSAOTENSAO
/**
 * Modelo basico componentes de fontes controladas
 */
#include "fontescontroladas.cpp"

/* Necessario para nao precisar escrever std:: */
using namespace std;

class TensaoTensao : public FontesControladas
{
    public:
        /**
         * Construtor
         */
        TensaoTensao(string n, int a, int b, int c, int d, double g) : FontesControladas(n, a, b, c, d, g)
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
         * Estanpa da matriz nodal modificada fonte de tensao controlada
         * por tensao
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
            condutancia[L[pos]][C[getNoA()]] += -1;
            condutancia[L[pos]][C[getNoC()]] += getGanho();
            condutancia[L[0]][C[getNoD()]] += getGanho();
            condutancia[L[pos]][C[getNoD()]] += -1*getGanho();
            condutancia[L[0]][C[getNoC()]] += -1*getGanho();

            //condutancia[getNoA()][pos] += 1;
            //condutancia[getNoB()][pos] += -1;
            //condutancia[pos][getNoA()] += -1;
            //condutancia[pos][getNoB()] += 1;
            //condutancia[pos][getNoC()] += getGanho();
            //condutancia[pos][getNoD()] += -1*getGanho();
        }
};

#endif
