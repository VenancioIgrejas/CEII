#ifndef CAPACITOR
#define CAPACITOR
/**
 * Modelo basico de componentes
 */
#include "components.cpp"

/* Necessario para nao precisar escrever std:: */
using namespace std;

class Capacitor : public Components
{
    public:
        /**
         * Construtor
         */
        Capacitor(string n, int a, int b, double v,double o) : Components(n, a, b)
        {
            setCapacitancia(v);
        }

        /**
         * Define a corrent no capacitor
         * @param v valor da corrente
         */
        void setPasso(double p)
        {
            passo = p;
        }

        /**
         * Retorna a corrente do  capacitor
         */
        double getPasso()
        {
            return passo;
        }
        /**
        * Retorna o valor de teta.
        Ex(teta = 0.5 é Metodo trapézio)
        */
        double getTeta()
        {
            return teta;
        }

        /**
         * Define a corrent no indutor
         * @param v valor da corrente
         */
        void setTeta(double o)
        {
          /** evitando ter um valor de
          * teta muito proximo de 1, pois pode gerar erro
          */
            if (o < 0.001){
              teta = 0.001;
            }else{
              teta = o;
            }
        }


        /**
         * Define a corrent no capacitor
         * @param v valor da corrente
         */
        void setCorrente(double v)
        {
            corrente = v;
        }

        /**
         * Retorna a corrente do  capacitor
         */
        double getCorrente()
        {
            return corrente;
        }

        /**
         * Define a capacitancia do capacitor
         * @param v valor da capacitancia
         */
        void setCapacitancia(double v)
        {
            capacitancia = v;
        }

        /**
         * Retorna a capacitancia do  capacitor
         */
        double getCapacitancia()
        {
            return capacitancia;
        }

        /**
         * Estanpa da matriz nodal modificada para resistor nao linear
         * @param condutancia matriz de condutancia
         * @param correntes   matriz de correntes
         * @param nodes        matris de nos
         */
        void estampar(vector<vector<double> >& condutancia,
            vector<double>& correntes,
            vector<string> nodes,
            vector<double> resultado)
        {
                /**
                 * Dividido em duas etapas de tempo, para o instante inicial consideramos as tensoes
                 * e correntes zeradas, no caso do capacitor, ele possui uma condutancia infinita
                 * para todo os outros casos consideramos a estampa normal do modelo de trapezio
                 */
                if (passo != 0) {
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

                condutancia[getNoA()][getNoA()] += getCapacitancia()/(teta*passo);
                condutancia[getNoB()][getNoB()] += getCapacitancia()/(teta*passo);
                condutancia[getNoA()][getNoB()] += (-1*getCapacitancia())/(teta*passo);
                condutancia[getNoB()][getNoA()] += (-1*getCapacitancia())/(teta*passo);

                correntes[getNoA()] += ((getCapacitancia()/(teta*passo)) * tensaoRamo) + ((1-teta)/teta)*getCorrente(); /*getCorrente vale j no instante anterior*/
                correntes[getNoB()] += ((-1 * getCapacitancia()/(teta*passo)) * tensaoRamo) - ((1-teta)/teta)*getCorrente(); /*getCorrente vale j no instante anterior*/
                /**
                 * Define a nova corrente usando tensao no instante e a corrente no instante anterior
                 * esse nova corrente agora passa a ser a corrente na fonte de corrente para o instante
                 * atual
                 */
                setCorrente(((getCapacitancia()/(teta*passo)) * tensaoRamo) + ((1-teta)/teta)*getCorrente());
            } else {
                condutancia[getNoA()][getNoA()] += 10e-9;
                condutancia[getNoB()][getNoB()] += 10e-9;
                condutancia[getNoA()][getNoB()] += -10e-9;
                condutancia[getNoB()][getNoA()] += -10e-9;
            }

        }

    private:
        /**
         * Resistencia associada ao modelo do capacitor
         * no trapezio
         */
        double passo;
        /**
         * Corrente da fonte de corrente
         * associada ao modelo do capacitor no trapezio
         */
        double corrente;
        /**
         * valor da capacitancia do capacitor
         */
        double capacitancia;

        double teta;

};

#endif
