#include <iostream>
#include <fstream>
#include <vector>
#include <ctype.h>
#include <string.h>
#include <locale.h>
#include <stdlib.h>
#include <stdio.h>
#include <map>

using namespace std;

struct Articulo {
  string titulo;
  string contenido;
} ;
map <int, Articulo> mapContent;

string recorrer(string s,int  pos){
	string resultado;
	//cout<<"pos recorrer"<<pos<<endl;
	s=s.substr(pos);int i;
	//cout<<"s "<<s<<endl;
	//system("PAUSE()");
	for(i=1;i<s.length();i++){
		if(s[i] == '"'){
			break;
		} 
	}	
	//cout<<"i "<<i<<endl;
	resultado=s.substr(0,i);
	//cout<<"resultado recorrer "<<resultado<<endl;
	//system("PAUSE()");
	return resultado;
}


void cargarTexto(){
	setlocale(LC_ALL, "spanish");	
	setlocale(LC_CTYPE, "Spanish");
	 
	string url;
	int cont=10000;
//-------------------lectura de data 
	string frase;
	int i=0;
	vector <string> textInFile(999999);
	ifstream ficheroEntrada;

	for(int k = 0;cont < 340000;k++){

		//ficheroEntrada.open ("../../raw.es/spanishText_10000_15000");
		url="../../raw.es/spanishText_"+to_string(cont)+'_'+to_string(cont+5000);
		cout<<"["<<k<<"] "<<" url: "<<url<<endl;
		cont=cont+5000;

		ficheroEntrada.open (url);
		if(ficheroEntrada.fail()){
			cout<<"no se pudo abrir el archivo"<<endl;
			continue;
		}
		while(!ficheroEntrada.eof()){  

			getline(ficheroEntrada, frase);
			if(frase =="</doc>"){	
				continue;
			}
			if(frase =="ENDOFARTICLE."){
				i++;	
				continue;
			}
		//si no es cabecera ni fin asignarle a una variable.
			if(frase !="ENDOFARTICLE." && frase !="</doc>"){
				textInFile[i] = textInFile[i] + frase;
				if(frase.length() != 0)
					textInFile[i]+= "\n";
				continue;		
			}
		}
	ficheroEntrada.close();
	}
//----------------------------------------------------------------

 //___________________________________________________________
	int posinit,posend,posid,postittle,posnfil,posproce,posdbindex;
	int a=i;
	
	for (int i = 0; i < a; ++i)
	{
		string sstart,ssend,ssid,ssttitle,ssnfil,ssprocesed,ssdbindex,mensaje;
		sstart='<doc',ssend='">',ssid='id=',ssttitle='title=',
		ssnfil='nonfiltered=',ssprocesed='=',ssdbindex='dbindex=';

		posinit=textInFile[i].find(sstart);

		posid=textInFile[i].find(ssid,posinit);
 		ssid=recorrer(textInFile[i],posid+2);

		postittle=textInFile[i].find(ssttitle,posid+2);
		ssttitle=recorrer(textInFile[i],postittle+2);	

		posnfil=textInFile[i].find(ssnfil,postittle+2);
		ssnfil=recorrer(textInFile[i],posnfil+2);
		
		posproce=textInFile[i].find(ssprocesed,posnfil+2);
		ssprocesed=recorrer(textInFile[i],posproce+2);

		posdbindex=textInFile[i].find(ssdbindex,posproce+2);
		ssdbindex=recorrer(textInFile[i],posdbindex+2);	

		posend=textInFile[i].find(ssend);

		//cout << "dbIndex" << ssdbindex << endl;
		//cout << "Titulo:" << ssttitle << endl;
		//cout <<"Contenido:"<<textInFile[i].substr(posend+2)<< endl;

		Articulo art;
		art.titulo = ssttitle;
		art.contenido = textInFile[i].substr(posend+2);
		mapContent.insert(pair <int, Articulo> (stoi(ssdbindex),art));

	}
}

int main() {

	cargarTexto();
	int dbI;

	while(1)
	{
		cout << "Ingrese DbIndex" << endl;
		cin >> dbI;
		cout << mapContent.find(dbI)->second.titulo << mapContent.find(dbI)->second.contenido <<endl;
		cout << "" << endl;
	}
	
	return 0;

}