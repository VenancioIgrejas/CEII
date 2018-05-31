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

	fileName = "simples.net";
	myNet.open(fileName);

	Parser* elementsList = new Parser(myNet);

	Factory* components = new Factory(0);

	listOfElements = elementsList -> getElements();

	components->setTempoFinal(10);
	components->setPasso(1);
	components->setTeta(0.5);
	
	components -> setup(elementsList-> getElements());
	vector<string> nodes;
	nodes = components -> getAllNodes();
	nos = nodes.size();
	int numeroComponentes = components -> getComponents().size();

	vector<Components*> listaDeComponetesAnterior(numeroComponentes);
	vector<double> resultado(nos);
	vector<double> resultadoAnterior(nos);

	for (double t = components->getTempo(); t <= components->getTempoFinal()+10e-15; t+= components->getPasso()) {
		vector<vector<double>> condutancia(nos,vector<double>(nos));
		vector<double> correntes(nos);
		components->setTempo(t);
		components->setup(elementsList->getElements());
		for (int i = 0; i < numeroComponentes; i++) {
			components -> getComponents()[i]->estampar(condutancia,correntes,nodes,resultado);
		}
		resultadoAnterior = resultado;
		resultado = gauss(condutancia, correntes, components->getNodesSize());

		listaDeComponetesAnterior = components->getComponents();

		for (int x = 1; x < (int) resultado.size(); x++) {
			cout << " "<< resultado[x];
		}
		cout << endl;
		}
	return 0;
};
