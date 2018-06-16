/*
* Testando o código do Igor
*/

//#define __USE_MINGW_ANSI_STDIO 0

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include "parser.cpp"
#include "factory.cpp"

using namespace std;

int main(){

	string fileName,type;
  bool repeat;
	int nos = 0;
  ifstream myNet;
  bool linear = true;

	vector<vector<string>> listOfElements;

	fileName = "capacitor.net";
	myNet.open(fileName);

	Parser* elementsList = new Parser(myNet);

	Factory* components = new Factory(0);

	listOfElements = elementsList -> getElements();

	components->setTempoFinal(10);
	components->setPasso(0.8);
	components->setTeta(0.5);

	components -> setup(elementsList-> getElements());
	vector<string> nodes;
	nodes = components -> getAllNodes();
	nos = nodes.size();
	int numeroComponentes = components -> getComponents().size();

	vector<Components*> listaDeComponetesAnterior(numeroComponentes);
	vector<double> resultado(nos);
	vector<double> resultadoAnterior(nos);

	ofstream outfile ("resultados.tab");
	outfile << "t";
	for(int n = 1; n < nos; n++) {
			outfile << " " << nodes[n];
	}
	outfile << endl;

	for (double t = components->getTempo(); t <= components->getTempoFinal()+10e-15; t+= components->getPasso()) {
		vector<vector<double>> condutancia(nos,vector<double>(nos));
		vector<double> correntes(nos);
		components->setTempo(t);
		components->setup(elementsList->getElements());
		for (int i = 0; i < numeroComponentes; i++) {

			if(components->getComponents()[i]->getNome().substr(0,1) == "C"){
				if(t==0){
					components->getComponents()[i]->setCorrente(0);
				} else {
					components->getComponents()[i]->setCorrente(listaDeComponetesAnterior[i]->getCorrente());
				}
			}
			components -> getComponents()[i]->estampar(condutancia,correntes,nodes,resultado);
		}
		resultadoAnterior = resultado;
		resultado = gauss(condutancia, correntes, components->getNodesSize());

		for (int i=0;i<numeroComponentes; i++){
			if (components->getComponents()[i]->getNome().substr(0,1) == "C"){
				int noA = components->getComponents()[i]->getNoA();
				int noB = components->getComponents()[i]->getNoB();

				double tensaoRamo = resultado[noA] - resultado[noB];

				if(noA == 0){tensaoRamo = -1*resultado[noB];}
				if(noB == 0){tensaoRamo = resultado[noA];}

				double correnteResistor = (components->getComponents()[i]->getCapacitancia() / ((components->getPasso()) * components->getComponents()[i]->getTeta())) * tensaoRamo;

				components->getComponents()[i]->setCorrente(correnteResistor - components->getComponents()[i]->getCorrente());
			}
		}
		listaDeComponetesAnterior = components->getComponents();
		outfile << t;
		for(int x = 1; x < (int) resultado.size(); x++) {
				outfile << " " << resultado[x];
		}
		outfile << endl;
	}
	outfile.close();
	return 0;
};
