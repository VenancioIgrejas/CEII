/**
 * Inclui:
 *  - cin
 *  - cout
 */
#include <iostream>
/**
 * Inclui:
 *  - system
 */
#include <cstdlib>
 /**
 * Inclui:
 *  - ifstream
 */
#include <fstream>
/**
 * Inclui:
 *  - Vector
 */
#include <vector>
#include <iomanip>
/**
 * Classe parceadora de Net List
 */
#include "parser.cpp"
/**
 * Fabrica construtora dos modelos dos elementos
 */
#include "factory.cpp"

/* Necessario para nao precisar escrever std:: */
using namespace std;

int main()
{
    string fileName;
    bool repeat;
    ifstream myNet;
    bool linear = true;

    do {
        //system("cls"); /*Limpa o console do Windows*/
        system("clear"); /*Limpa o console do Linux*/

        if (repeat == true) {
            cout << "Arquivo " << fileName << " nao foi encontrado, tente novamente.\n";
            cout << "Obs. O Arquivo deve estar na mesma pasta do executavel.\n\n";
        }
        repeat = true;

        cout << "EEL525 - Circutos Eletricos II\n";
        cout << "UFRJ/Poli/DEL - Departamento de Engenharia Eletronica\n";
        cout << "Desenvolvido por: Eduardo Pires, Igor Abreu e Jonas Degrave\n";
        cout << "Programa de analise no dominio do tempo de elementos lineares e nao lineares\n";
        cout << "Nome do arquivo contendo a NetList: [ex. minha.net] ";
        cin >> fileName;
        /* Abrindo a net list*/
        myNet.open(fileName);
        if (myNet.is_open()) {
            repeat = false;
        }
    } while (repeat == true);

    /**
     * Lista de elementos a serem construidos
     * @param  myNet Arquivo da netlist
     * @return       Vetor de vetores devidamente separados com os componentes
     */
    Parser* elementsList = new Parser(myNet);
    /**
     * inicializa a fabrica de componentes com a constante tempo
     * inicialmente zerada
     */
    Factory* components = new Factory(0);
    /**
     * Incializa a construcao dos objetos de componentes
     * e pega a matriz de objetos
     */
    components->setup(elementsList->getElements());
    /**
     * Logica para pegar o numero de nos
     */
    int nos = 0;
    vector<string> nodes;
    nodes = components->getAllNodes();
    nos = nodes.size();
    int numeroComponentes = components->getComponents().size();
    components->operacional(nos);
    int neq = components->getNumEq();
    vector <int> L = components->getL();
    vector <int> C = components->getC();

    int numEq = components->getNumEq();

    cout << "\nVariaveis e apontadores:" << endl;
    for (int i = 0; i < nos; i++) {
        cout << i << " -> " << nodes[i] << " (L=" << L[i] << ", C=" << C[i] << ")" << endl;
    }
    cout << "TODOS OS NOS SAO: " << nos << endl;
    cout << "O NUMERO DE NOS EH: " << ((components->getNodesSize())-1) << endl;
    cout << "O NUMERO DE EQS EH: " << neq << endl;

    vector<Components*> listaDeComponetesAnterior(numeroComponentes);
    vector<double> resultado(nos);
    vector<double> resultadoAnterior(nos);

    ofstream outfile ("resultados.tab");
    outfile << "t";
    for(int n = 1; n < nos; n++) {
        if (C[n] != 0) {
            //outfile << " " << nodes[n];
            outfile << "  " << nodes[n];
        }
    }
    outfile << endl;
    for (double t = components->getTempo(); t <= components->getTempoFinal() + 10e-15; t += components->getPasso()) { // 10e-15 para comparacao com double
        /**
         * Criando vetor de condutancai e correntes
         * de acordo com o numero de nos no netlist
         */
        vector<vector<double> > condutancia(nos, vector<double>(nos));
        vector<double> correntes(nos);
        components->setTempo(t);
        /**
         * Criar a lista de elementos.
         */
        components->setup(elementsList->getElements());
        /**
         * Verificar se alguns dos componentes e um capacitor
         * para definir uma corrente inicial.
         */


        for (int i = 0; i < numeroComponentes; i++) {

          if (components->getComponents()[i]->getNome().substr(0,1) == "L"){
            components->getComponents()[i]->setTeta(components->getTeta());
          }
            /**
             * Verificar se existe algum componente nao linear
             */
            if (! components->getComponents()[i]->isLinear()) {
                linear = false;
            }
            /**
             * Caso especifico para quando o componente e um capacitor
             * e temos que definir a corrente que passa pelo capacitor
             */
            if (components->getComponents()[i]->getNome().substr(0,1) == "C") {
                components->getComponents()[i]->setTeta(components->getTeta());
                if (t == 0) {
                    /**
                     * Corrente para quando o instante de tempo e zero
                     */
                    components->getComponents()[i]->setCorrente(0);
                } else {
                    /**
                     * Pega a corrente que passa pelo capacitor em um instante de tempo anterior e define como
                     * corrente a ser utilizada no lugar de j0
                     */
                    components->getComponents()[i]->setCorrente(listaDeComponetesAnterior[i]->getCorrente());
                }
            }
            components->getComponents()[i]->estampar(condutancia, correntes, nodes, L, C, resultado);
        }

        resultadoAnterior = resultado;
        resultado = gauss(condutancia, correntes, neq);
        /**
         * Teste de adicionar a corrente apos o calculo
         */
        for (int i = 0; i < numeroComponentes; i++) {
            if (components->getComponents()[i]->getNome().substr(0,1) == "C") {
                int noA = components->getComponents()[i]->getNoA();
                int noB = components->getComponents()[i]->getNoB();
                /**
                 * Pega a tensao nodal para a matriz de resultados atuais e estampas atuais
                 */
                double tensaoRamo = resultado[C[noA]] - resultado[C[noB]];
                /**
                 * Ignorar a tensao no no 0
                 */
                if (noA == 0) {
                    tensaoRamo = -1*resultado[C[noB]];
                }
                if (noB == 0) {
                    tensaoRamo = resultado[C[noA]];
                }
                /**
                 * Pega a corrente passando no resistor no instante de tempo atual
                 */
                double correnteResistor = ((components->getComponents()[i]->getCapacitancia()/(components->getPasso()*components->getTeta())) * tensaoRamo)
                                        + ((1-components->getTeta())/components->getTeta())
                                        *components->getComponents()[i]->getCorrente();
                /**
                 * Pega a corrente no resistor e subtrai pela corrente na fonte de corrente no modelo
                 * do trapezio
                 */
                components->getComponents()[i]->setCorrente(
                    correnteResistor -
                    components->getComponents()[i]->getCorrente()
                );
            }
        }
        /**
         * Caso a netlist possua elementos nao lineares devemos fazer newton raphson
         * newton raphson e so feito no instante de tempo zero
         */
        if (linear == false) {
            bool converge = false;
            for (int n = 1; n <= 20; n++) {
                for (int i = 0; i < numeroComponentes; i++) {
                    if (components->getComponents()[i]->getNome().substr(0,1) == "$" ||
                        components->getComponents()[i]->getNome().substr(0,1) == "N" ||
                        components->getComponents()[i]->getNome().substr(0,1) == "D" ){
                        /*Desestampa e reestampa componentes nao lineares*/
                        components->getComponents()[i]->desestampar(condutancia, correntes,L, C, resultadoAnterior);
                        components->getComponents()[i]->estampar(condutancia, correntes, nodes, L, C, resultado);
                        std::cout << "passou aqui : " << n << '\n';
                    }
                }
                resultadoAnterior = resultado;
                resultado = gauss(condutancia, correntes, neq);//components->getNodesSize());

                converge = comparar(resultadoAnterior, resultado);
                if (converge == true) {
                    break;
                }
            }
            std::cout << "tempo: "<< t << '\n';
        }

        /**
         * Salva a lista de componentes no instante anterior
         */
        listaDeComponetesAnterior = components->getComponents();

        outfile << t;
        for(int x = 1; x < nos; x++) {//(int) resultado.size()+1; x++) {
            if (C[x] !=0)
            {
              outfile << "  " << resultado[C[x]];
            }//outfile << " " << resultado[x];
        }
        outfile << endl;
    }
    outfile.close();
}
