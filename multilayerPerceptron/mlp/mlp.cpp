#include <stdio.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <unistd.h>

using namespace std;

void definirTopologia();
double propagar( vector<double> entrada);
void imprimirTopologia();
void backpropagation();
double calcularErro(double saidaEsperada, double saidaObtida);
double gradienteSaida(double saidaEsperada, double saidaObtida);
double gradienteCamadasOcultas(double somatorioGradientePesoCamadaPosterior, double derivadaSaida);
void atualizarPesos(double saidaEsperada, double saidaObtida);
double funcaoAtivacao(double valor);
double derivadaFuncaoAtivacao(double valor);
void popularMatrizTreinamento();

//variaveis globais
bool ativacao;
double bias, soma, erroRede, erro, limiar, taxaAprendizagem;
int tamEntrada, maximoIteracoes, ciclos, tamanhoEntrada, qntCamadas;

vector< vector< vector<double> > > rna;
vector< vector<double> > matrizTreinamento;

int main(){
	
	definirTopologia();
	popularMatrizTreinamento();
	//sleep(1);
	backpropagation();
	imprimirTopologia();

}


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
	
	//lendo o restante das informacoes
	int aux;
	scanf("%lf %d %lf %d", &taxaAprendizagem, &maximoIteracoes, &limiar, &aux);
	ativacao = aux-1;
	
	//imprimindo a topologia lida:
	//imprimirTopologia();
	
}


void popularMatrizTreinamento(){
	int entrada, bit, contador=0;
	vector <double> linha;
	
	//lendo as linhas de treinamento:
	while(scanf("%d", &entrada) != EOF){
		cout<<"Entrada "<<++contador<<": ";
		bit = entrada/100;
								cout<<bit<<"  ";
		linha.push_back(bit);
		bit = entrada % 100;
		bit /= 10;
								cout<<bit<<"  ";
		linha.push_back(bit);
		bit = entrada % 10; 
								cout<<bit<<"  ";
		linha.push_back(bit);
		for(int j=0; j<tamEntrada/3 -1; j++){
			scanf("%d", &entrada);
			bit = entrada/100;
									cout<<bit<<"  ";
			linha.push_back(bit);
			bit = entrada % 100;
			bit /= 10;
									cout<<bit<<"  ";
			linha.push_back(bit);
			bit = entrada % 10; 
									cout<<bit<<"  ";
			linha.push_back(bit);
		}
		scanf("%d", &entrada);
														cout<<" Saida: "<<entrada<<endl;
		linha.push_back(entrada);
		matrizTreinamento.push_back(linha);
		linha.clear();
	}
}





/*-----------PROPAGACAO DA ENTRADA--------------*/
double propagar( vector<double> entrada){
	
	/*--------------------COPIANDO A ENTRADA PARA A CAMADA 0 DA RNA----------------------*/
	vector<double> neuronio;
	rna[0].clear();
	for(unsigned i=0;i<entrada.size() -1; i++){
		neuronio.push_back(entrada[i]);
		rna[0].push_back(neuronio);
		neuronio.clear();
	}

	
	/*--------------------PROPAGANDO A ENTRADA----------------------*/
	for(unsigned k=1; k<rna.size(); k++){
		for(unsigned i=0; i<rna[k].size(); i++){
			soma = bias * rna[k][i][0];
																			//cout<<soma<<endl;
			for(unsigned j=1; j<(rna[k][i].size()-1); j++){
				soma += (rna[k][i][j]) * (rna[k-1][j-1].back());
																			//cout<<rna[k-1][j-1].back()<<" x "<<rna[k][i][j]<<"   ";
			}
			//armazenando o resultado no ultimo campo do neuronio
			rna[k][i].back() = funcaoAtivacao(soma);
																			//cout<<rna[k][i].back()<<"Soma: "<<soma<<" funcao ativacao: "<<funcaoAtivacao(soma)<<endl;
		}
	}
																			//cout<<rna.back().back().back()<<endl;
	return rna.back().back().back();
}

void backpropagation(){
	ciclos = 0;
	erroRede = 1;
	double  maiorErro, saida;
	int count = 0;
						
	while(ciclos++ < maximoIteracoes && count <2){
		maiorErro=0;
																		
		for(unsigned i = 0; i<matrizTreinamento.size(); i++){
			saida = propagar(matrizTreinamento[i]);
																		//cout<<" saida: "<<saida<<" saida esperada: "<<matrizTreinamento[i].back()<<endl;
			erro = calcularErro(matrizTreinamento[i].back(), saida);
			//if(!(abs(matrizTreinamento[i].back() - saida) < limiar))
				atualizarPesos(matrizTreinamento[i].back(), saida);															
																			
			if(abs(erro) > maiorErro){
				maiorErro = abs(erro);
			}
			
		}
			
		erroRede = maiorErro;
		//printf("Maior erro: %lf | ErroRede: %lf \n", maiorErro, erroRede);
		if(erroRede < limiar)
			count++;
	}
	if(ciclos >= maximoIteracoes){
		printf("Fim do treinamento numero maximo de ciclos atingido %d ciclos, erro: %lf\n", ciclos-1, erroRede);
	}else{
		printf("Fim do treinamento erro minimizado em %lf , ciclos: %d\n", erroRede, ciclos-1);
	}
	//puts("Topologia apos treinamento: ");
}

void atualizarPesos(double saidaEsperada, double saidaObtida){
	erro = calcularErro(saidaEsperada, saidaObtida);
	
	/*--------------------camada de saida----------------------*/
	//bias

	rna.back()[0][0] = rna.back()[0][0] + taxaAprendizagem*gradienteSaida(saidaEsperada, saidaObtida)*bias;
	for(unsigned j=1; j<(rna.back()[0].size())-1; j++){
			rna.back()[0][j] = rna.back()[0][j] + taxaAprendizagem*gradienteSaida(saidaEsperada, saidaObtida)*rna[rna.size()-2][j-1].back();
	}
	
	vector < vector <double> > gradientes;
	vector <double> camada;
	camada.push_back(gradienteSaida(saidaEsperada, saidaObtida));
	gradientes.push_back(camada);
																			//cout<<"Gradiente camada de saida: "<<camada.back()<<endl;
	/*--------------------camadas intermediarias----------------------*/
	for(unsigned k=rna.size()-2; k>0; k--){
		camada.clear();
		for(unsigned i=0; i<rna[k].size(); i++){
			//calculo do gradiente desta camada intermediaria
			double auxSum = 0;
			for(unsigned z = 0; z < gradientes.back().size(); z++){
				
				auxSum += gradientes.back()[z] * rna[k+1][z][i+1];//  				pagina 104 e  105 equação 5.26 primeiro fator
			}
			camada.push_back(gradienteCamadasOcultas(auxSum, derivadaFuncaoAtivacao(rna[k][i].back())));
			
			rna[k][i][0] = rna[k][i][0] + taxaAprendizagem*1*bias*rna[k][i][0];
			
			for(unsigned j=1; j<(rna[k][i].size()-1); j++){
																										//cout<<"Antigo: "<<rna[k][i][j]<<" Novo: ";
				rna[k][i][j] = rna[k][i][j] + taxaAprendizagem*camada[i]*rna[k-1][j-1].back();
																										//cout<<rna[k][i][j]<<endl;
			}
			
		}
		gradientes.push_back(camada);
	}
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
