#ifndef TRANSFORMADOR
#define TRANSFORMADOR
/**
 * Modelo basico componentes de 4 terminais
 */
#include "components4t.cpp"

/* Necessario para nao precisar escrever std:: */
using namespace std;

class Transformador : public Components4t
{

    public:
        /**
         * Construtor
         */
        Transformador(string n, int a, int b, int c, int d, double param) : Components4t(n, a, b, c, d)
        {
            setN(param);
        }

        /**
         * Define n do transformador
         * @param n valor de N
         */
        void setN(double param)
        {
            N = param;
        }

        /**
         * Retorna n do transformador
         */
        double getN()
        {
            return N;
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

            condutancia[L[getNoA()]][C[pos]] += -1*getN();
            condutancia[L[getNoB()]][C[pos]] += getN();
            condutancia[L[getNoC()]][C[pos]] += 1;
            condutancia[L[getNoD()]][C[pos]] += -1;
            condutancia[L[pos]][C[getNoA()]] += getN();
            condutancia[L[pos]][C[getNoB()]] += -1*getN();
            condutancia[L[pos]][C[getNoC()]] += -1;
            condutancia[L[pos]][C[getNoD()]] += 1;
        }

    private:
        /**
         * Valor n do transformador
         */
        double N;
};

#endif
