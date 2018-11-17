#include <iostream>
#include <stdio.h>
#include <vector>
#include <list>
#include <string>
#include <sstream>

using namespace std;

void lerPicosEntrada();

vector <double>		picosEntrada;
list   <double>		tensoesLidas;

int main(){
	
	lerPicosEntrada();
	double tempo, tensao;
	//caso o primeiro pico esteja nos 7 primeiros nao encontra
	scanf("%lf", &tensao);
	tensoesLidas.push_back(tensao);
	for(int i=0; i<5; i++){
		scanf("%lf %lf", &tempo, &tensao);
		tensoesLidas.push_back(tensao);
	}
	
	while(scanf("%lf %lf", &tempo, &tensao) != EOF && !picosEntrada.empty()){
		tensoesLidas.push_back(tensao);
		if(tempo == picosEntrada.front()){
			stringstream durante, depois;
			for(int i=0; i<3; i++){
				cout<<tensoesLidas.front()<<"\t";
				tensoesLidas.pop_front();
			}
			for(int i=0; i<4; i++){
				cout<<tensoesLidas.front()<<"\t";
				durante << tensoesLidas.front();
				durante << "\t";
				tensoesLidas.pop_front();
			}
			cout<<"0"<<endl;
			for(int i=0; i<3; i++){
				scanf("%lf %lf", &tempo, &tensao);
				durante << tensao;
				durante << "\t";
				depois  << tensao;
				depois << "\t";
			}
			cout<<durante.str()<<"1\n";
			for(int i=0; i<4; i++){
				scanf("%lf %lf", &tempo, &tensao);
				tensoesLidas.push_back(tensao);
				depois << tensao;
				depois << "\t";
			}
			
			cout<<depois.str()<<"0\n";
			for(int i=0; i<3; i++){
				scanf("%lf %lf", &tempo, &tensao);
				tensoesLidas.push_back(tensao);
			}
			picosEntrada.erase(picosEntrada.begin());
		}
		tensoesLidas.pop_front();
	}
}

void lerPicosEntrada(){
	double leitura;
	char fim;
	scanf("%lf", &leitura);
	picosEntrada.push_back(leitura);
	while(scanf("%c", &fim), fim == '\n'){
		scanf("%lf", &leitura);
		picosEntrada.push_back(leitura);
	}
	picosEntrada.erase(picosEntrada.end() -1 );
}
