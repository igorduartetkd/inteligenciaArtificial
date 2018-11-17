#include <iostream>
#include <stdio.h>

using namespace std;

int main(){
	
	double tempo, tensao, descarte;
	
	//obs. excluir a primeira linha pois contem 0
	while(scanf("%lf", &tempo) != EOF){
		scanf("%lf %lf", &tensao, &descarte);
		cout<<tempo<<"\t";
		
		if(tensao < 0){
			cout<<" 0";
		}else if(tensao < 0.2){
			cout<<" 1";
		}else if(tensao < 0.5){
			cout<<" 10";
		}else if(tensao < 0.7){
			cout<<" 11";
		}else if(tensao < 0.9){
			cout<<" 100";
		}else if(tensao < 1){
			cout<<" 101";
		}else if(tensao < 1.2){
			cout<<" 110";
		}else{
			cout<<" 111";
		}
		puts("");
	}
}
