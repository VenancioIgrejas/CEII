#ifndef TENSAOCORRENTE
#define TENSAOCORRENTE
/**
 * Modelo basico componentes de fontes controladas
 */
#include "fontescontroladas.cpp"

/* Necessario para nao precisar escrever std:: */
using namespace std;

class TensaoCorrente : public FontesControladas
{
    public:
        /**
         * Construtor
         */
        TensaoCorrente(string n, int a, int b, int c, int d, double g) : FontesControladas(n, a, b, c, d, g)
        {
        }

        /**
         * Retorna corrente que auxiliar
         * para analise nodal modificada
         */
        string getAuxNode()
        {
            return "jx" + getNome();
        }

        /**
         * Retorna corrente que auxiliar
         * para analise nodal modificada
         */
        string getAuxNode2()
        {
            return "jy" + getNome();
        }

        /**
         * Estanpa da matriz nodal modificada fonte de tensao
         * controlada por corrente
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
            vector<string>::iterator it2;

            it = find(nodes.begin(), nodes.end(), getAuxNode());
            it2 = find(nodes.begin(), nodes.end(), getAuxNode2());
            auto pos = it - nodes.begin();
            auto pos2 = it2 - nodes.begin();

            condutancia[L[0]][C[getNoA()]] += 1;
            condutancia[L[pos2]][C[getNoB()]] += 1;
            condutancia[L[0]][C[getNoB()]] += -1;
            condutancia[L[pos2]][C[getNoA()]] += -1;

            condutancia[L[pos2]][C[pos]] += getGanho();
            condutancia[L[0]][C[0]] += getGanho();
            condutancia[L[pos2]][C[0]] += -1*getGanho();
            condutancia[L[0]][C[pos]] += -1*getGanho();

            condutancia[L[getNoC()]][C[pos]] += 1;
            condutancia[L[getNoD()]][C[0]] += 1;
            condutancia[L[getNoC()]][C[0]] += -1;
            condutancia[L[getNoD()]][C[pos]] += -1;

            /*condutancia[L[getNoA()]][C[pos2]] += 1;
            condutancia[L[getNoB()]][C[pos2]] += -1;
            condutancia[L[getNoC()]][C[pos]] += 1;
            condutancia[L[getNoD()]][C[pos]] += -1;
            condutancia[L[pos]][C[getNoC()]] += -1;
            condutancia[L[pos]][C[getNoD()]] += 1;
            condutancia[L[pos2]][C[getNoA()]] += -1;
            condutancia[L[pos2]][C[getNoB()]] += 1;
            condutancia[L[pos2]][C[pos]] += getGanho();*/
        }
};

#endif
