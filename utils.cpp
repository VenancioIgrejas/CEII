#ifndef UTILS
#define UTILS
/**
 * Inclui:
 *  - istringstream
 */
#include <sstream>
 /**
 * Inclui:
 *  - vector
 */
#include <vector>
/**
 * Tratamento de excecoes
 */
#include <stdexcept>
/**
 * Inclui
 *  - fabs
 */
#include <math.h>
/**
 * Inclui
 *  - sort
 */
#include <algorithm>

#define TOLG 1e-9

/* Necessario para nao precisar escrever std:: */
using namespace std;

/**
 * Transformar uma string em elementos de um array utilizando um caracter delimitador
 * Ex. explode("Hello World", ' ')
 * Retorna:
 *     vector[0] = "Hello"
 *     vector[1] = "World"
 */
const vector<string> explode(const string& phrase, char delim)
{
    vector<string> result;
    istringstream iss(phrase);

    for (string token; getline(iss, token, delim); )
    {
        result.push_back(move(token));
    }

    return result;
}

/**
 * Retorna as tensoes nodais quando voce passa a matriz de condutancia
 * e a matriz de corrente, aplica a eliminacao de gauss para transformar a matriz
 * de condutividade em uma matriz identidade.
 */
vector<double> gauss(vector<vector<double> > condutancia, vector<double> correntes, int neq)
{
    vector<vector<double>> Yn(neq+1, vector<double>(neq+2));
    vector<double> resultado(neq+1);
    int c = 0;
    for (int l = 0; l <= neq; l++) {
        for (c = 0; c <= neq; c++) {
            Yn[l][c] = condutancia[l][c];
        }
        Yn[l][c] = correntes[l];
    }
    int i,j,l, a;
    double t, p;

     /*cout << "\nA MATRIZ NO INICIO EH: " << endl;
     for (int l = 1; l <= neq; l++) {
         for (c = 1; c <= neq+1; c++) {
             cout << Yn[l][c] << " ";
         }
         cout << "\n";
     }
     cout << "\n" << endl;*/

    for (i=1; i<=neq; i++) {
        t=0.0;
        a=i;
        for (l=i; l<=neq; l++) {
            if (fabs(Yn[l][i])>fabs(t)) {
                a=l;
                t=Yn[l][i];

            }
        }
        if (i!=a) {
            for (l=1; l<=neq+1; l++) {
                p=Yn[i][l];
                Yn[i][l]=Yn[a][l];
                Yn[a][l]=p;
            }
        }


        if (fabs(t)<TOLG) {
            throw invalid_argument("Sistema Singular");
        }
        for (j=neq+1; j>0; j--) {  /* Basta j>i em vez de j>0 */
            Yn[i][j] /= t;
            p=Yn[i][j];
            if (p!=0)
            for (l=1; l<=neq; l++) {
                if (l!=i)
                    Yn[l][j]-=Yn[l][i]*p;
            }
        }

        /* cout << "\nA MATRIZ NO FIM EH: " << endl;
         for (int l = 1; l <= neq; l++) {
             for (c = 1; c <= neq+1; c++) {
                 cout << Yn[l][c] << " ";
             }
             cout << "\n";
         }
         cout << "\n" << endl;*/

    }
    for(l = 0; l<=neq; l++ ) {
        resultado[l] = Yn[l][neq+1];
    }
   return resultado;
}

/**
 * Compara 2 vetores e verifica se eles estao proximos por uma margem de erro
 * @param  vetor1
 * @param  vetor2
 * @return        True se igual False  se diferente
 */
bool comparar(vector<double> vetor1, vector<double> vetor2)
{
    double x = 1;//10e-2;
    double erro;
    int nosIguais = 0;
    double limite = 10e-10; //Esse limete pode ser modificado para aumentar a exatidao da comparacao
    int sizeOne = vetor1.size();
    int sizeTwo = vetor2.size();

    if (sizeOne != sizeTwo) {
        throw invalid_argument("Matrizes de condutancia e corrente nao tem o msm tamanho");
    }

    for(int i = 0; i < sizeOne; i++) {

      if (fabs(vetor2[i]) > x)
      {
        erro = x*(fabs((vetor2[i]-vetor1[i])/vetor2[i]));
      }else{
        erro = fabs(vetor2[i] - vetor1[i]);
      }

      if (erro < limite){
        nosIguais++;
      }
        // double lLimit = fabs(vetor1[i]) - (fabs(vetor1[i]) * limite);
        // double uLimit = fabs(vetor1[i]) + (fabs(vetor1[i]) * limite);
        // if (fabs(vetor2[i]) <= uLimit && fabs(vetor2[i]) >= lLimit) {
        //     nosIguais++;
        // }
    }
    if (nosIguais == sizeOne) {
        return true;
    }
    return false;
}

/**
 * Retirada do stack overflow para comparar numeros no formato de string
 */
bool is_not_digit(char c)
{
    return ! isdigit(c);
}

/**
 * Retirada do stack overflow para ordenar corretamente os nos
 * de acordo com suas posicoes
 */
bool numeric_string_compare(const string& s1, const string& s2)
{
    string::const_iterator it1 = s1.begin(), it2 = s2.begin();

    if (isdigit(s1[0]) && isdigit(s2[0])) {
        int n1, n2;
        stringstream ss(s1);
        ss >> n1;
        ss.clear();
        ss.str(s2);
        ss >> n2;

        if (n1 != n2) return n1 < n2;

        it1 = find_if(s1.begin(), s1.end(), is_not_digit);
        it2 = find_if(s2.begin(), s2.end(), is_not_digit);
    }

    return lexicographical_compare(it1, s1.end(), it2, s2.end());
}
#endif
