#include <stdio.h>
#include <iostream>
#include <vector>
#include <unistd.h>
#include "mlpExec.h"

using namespace std;

int main(){
	definirTopologia();
	imprimirTopologia();
	
	double instanteLido, ultimoTempo = 0, tempoBatida;
	int medida, bit, bpm=0, minutos=0, aux;
	vector<double> entrada;
	vector<double> tempo;
	
	for(int z=0; z<tamEntrada/3 -1; z++){
		scanf("%lf %d", &instanteLido, &medida);
		tempo.push_back(instanteLido);
			bit = medida/100;
			entrada.push_back(bit);
			bit = medida % 100;
			bit /= 10;
			entrada.push_back(bit);
			bit = medida % 10; 
			entrada.push_back(bit);
	}
	
	while(scanf("%lf %d", &instanteLido, &medida) != EOF){
		
		//cout<<"Tempo: "<<instanteLido<<" Medida: "<<medida<<endl;
		tempo.push_back(instanteLido);
		
		bit = medida/100;
		entrada.push_back(bit);
		bit = medida % 100;
		bit /= 10;
		entrada.push_back(bit);
		bit = medida % 10; 
		entrada.push_back(bit);
		
		aux = instanteLido/60;
		if(aux > minutos){
			minutos++;
			cout<<minutos<<"º minuto "<<bpm<<" batimentos"<<endl;
			bpm = 0;
		}
		
		if(propagar(entrada)){
			tempoBatida = tempo[tempo.size()/2];
			if(tempoBatida - ultimoTempo > 0.25){
				bpm++;
				//cout<<" e "<<ultimoTempo<<" | periodo: ";
				//cout<<tempoBatida-ultimoTempo<<endl;
				//cout<<"Batida entre: "<<tempoBatida;
			}
			ultimoTempo = tempoBatida;
		}
		tempo.erase(tempo.begin());
		for(int i=0; i<3; i++)
			entrada.erase(entrada.begin());
	}
	printf("\n%d batimentos por minuto\n", bpm);
}
