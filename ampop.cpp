#ifndef AMPOP
#define AMPOP
/**
 * Modelo basico componentes de 4 terminais
 */
#include "components4t.cpp"

/* Necessario para nao precisar escrever std:: */
using namespace std;

class AmpOp : public Components4t
{

    public:
        /**
         * Construtor
         */
        AmpOp(string n, int a, int b, int c, int d) : Components4t(n, a, b, c, d)
        {
        }

        /**
         * Retorna corrente que auxiliar
         * para analise nodal modificada
         */
        //string getAuxNode()
        //{
        //    return "j" + getNome();
        //}

        /**
         * Estanpa da matriz nodal modificada amp op
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
          //  vector<string>::iterator it;
          //  it = find(nodes.begin(), nodes.end(), getAuxNode());
            //auto pos = it - nodes.begin();

           // condutancia[getNoA()][pos] += 1;
           // condutancia[getNoB()][pos] += -1;
          //  condutancia[pos][getNoC()] += 1;
         //   condutancia[pos][getNoD()] += -1;
        }
};

#endif
