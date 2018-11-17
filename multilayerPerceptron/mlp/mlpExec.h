#include <stdio.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <unistd.h>

using namespace std;

void definirTopologia();
double propagar( vector<double> entrada);
void imprimirTopologia();
double calcularErro(double saidaEsperada, double saidaObtida);
double gradienteSaida(double saidaEsperada, double saidaObtida);
double gradienteCamadasOcultas(double somatorioGradientePesoCamadaPosterior, double derivadaSaida);
double funcaoAtivacao(double valor);
double derivadaFuncaoAtivacao(double valor);


//variaveis globais
bool ativacao = 0;
double bias, soma, erroRede, erro, limiar, taxaAprendizagem;
int tamEntrada, maximoIteracoes, ciclos, tamanhoEntrada, qntCamadas;

vector< vector< vector<double> > > rna;
vector< vector<double> > matrizTreinamento;

/*
int main(){
	
	definirTopologia();
	imprimirTopologia();
	
	double leitura;
	vector<double> entrada;
	scanf("%d", &tamanhoEntrada);
	for(int z=0; z<tamanhoEntrada; z++){
		entrada.clear();
		cout<<"Entrada: ";
		for(unsigned i=0; i<rna[1].front().size() -2; i++){
			scanf("%lf", &leitura);
			leitura /= 1000;
			entrada.push_back(leitura);
			cout<<leitura<<"  ";
		}
		entrada.push_back(leitura);
		cout<<"\tSaida: "<<(propagar(entrada))<<endl;	//> 0.5?1:0
	}
}
*/

void definirTopologia(){
	
	/* modelo de arquivo de entrada:
	 * tamanho da entrada | qnt de camadas | qnt de neuronios em cada camada
	 * peso de todos os neuronios
	 */ 
	vector <int> qntNeuroniosPorCamada;
	//lendo tamanho da entrada e a quantidade de camadas e o bias
	scanf("%d %d %lf", &tamEntrada, &qntCamadas, &bias);
	qntNeuroniosPorCamada.push_back(tamEntrada);
	
	int qnt;
	//lendo a quantidade de neuronios por camada:
	for(int i=1; i<qntCamadas+1; i++){
		scanf("%d", &qnt);
		qntNeuroniosPorCamada.push_back(qnt);
	}
	
	//estruturas auxiliares:
	vector<double> neuronio;
	vector < vector<double> > camada;
	double peso;
	rna.push_back(camada);
	
	//lendo o bias e pesos de cada neuronio em ordem.
	for(int k=1; k<qntCamadas+1; k++){
		camada.clear();
		for(int i=0; i<qntNeuroniosPorCamada[k]; i++){ 
			neuronio.clear();
			for(int j=0; j<qntNeuroniosPorCamada[k-1]+1; j++){
				scanf("%lf", &peso);
				neuronio.push_back(peso);
			}
			neuronio.push_back(0); //o y de cada neuronio fica na ultima posicao
			camada.push_back(neuronio);
		}
		rna.push_back(camada);
	}
}





/*-----------PROPAGACAO DA ENTRADA--------------*/
double propagar( vector<double> entrada){
	
	/*--------------------COPIANDO A ENTRADA PARA A CAMADA 0 DA RNA----------------------*/
	vector<double> neuronio;
	rna[0].clear();
	for(unsigned i=0;i<entrada.size(); i++){
		neuronio.push_back(entrada[i]);
		rna[0].push_back(neuronio);
		neuronio.clear();
	}

	
	/*--------------------PROPAGANDO A ENTRADA----------------------*/
	for(unsigned k=1; k<rna.size(); k++){
		for(unsigned i=0; i<rna[k].size(); i++){
			soma = bias * rna[k][i][0];
			for(unsigned j=1; j<(rna[k][i].size()-1); j++){
				soma += (rna[k][i][j]) * (rna[k-1][j-1].back());
			}
			//armazenando o resultado no ultimo campo do neuronio
			rna[k][i].back() = funcaoAtivacao(soma);
		}
	}
																
	return rna.back().back().back();
}





/*--------------Funcao para imprimir a topologia da rna com os pesos----------------
 * cada camada e uma matriz. 
 * as colunas sao as entradas e as linhas sao os neuronios*/
void imprimirTopologia(){
	/*--------------------IMPRIMINDO PARAMETROS INICIAIS---------------------*/
	printf("\nTamanho da entrada: %d\nQuantidade de camadas: %d | bias: %lf\n", tamEntrada, qntCamadas, bias);
	printf("Taxa de aprendizagem: %lf\nNumero maximo de iteracoes: %d\nlimiar: %lf\n", taxaAprendizagem, maximoIteracoes, limiar);
	
	/*--------------------IMPRIMINDO A ENTRADA----------------------*/
	printf("Ultima Entrada: ");
	for(unsigned i=0; i<rna[0].size(); i++){
		printf("%lf ", rna[0][i].back());
	}
	puts("\n");
	/*--------------------IMPRIMINDO A RNA----------------------*/
	for(unsigned k=1; k<rna.size(); k++){
		//printf("Camada %d, qnt de neuronios %d: \n", k, (int)rna[k].size());
		//puts("Wbias - p1 - p2 - ... - pn - f(y)");
		for(unsigned i=0; i<rna[k].size(); i++){
			for(unsigned j=0; j<rna[k][i].size()-1; j++){
				printf(" %lf \t", rna[k][i][j]);
			}
			puts("");
		}
		puts("");
	}
}

double calcularErro(double saidaEsperada, double saidaObtida){
	//erro da rna erro =( desejado - obtido) elevado a 2 dividido por 2 
	return pow(saidaEsperada - saidaObtida, 2)/2;
}

double gradienteSaida(double saidaEsperada, double saidaObtida){
	// delta = (desejado - obtido)*derivada da função de ativação do somatório dos pesos pelas entradas
	return (saidaEsperada - saidaObtida)*derivadaFuncaoAtivacao(saidaObtida);
}

double gradienteCamadasOcultas(double somatorioGradientePesoCamadaPosterior, double derivadaSaida){
	return somatorioGradientePesoCamadaPosterior * derivadaSaida;
}

double funcaoAtivacao(double valor){ 
	if(ativacao)
		return 1/(1 + exp(-valor));
	
	return valor>=0?1:0; 
}

double derivadaFuncaoAtivacao(double valor){
	if(ativacao){
		return valor;
	}
	return 1;
}

