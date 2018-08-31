#include <iostream>
#include <fstream>
#include <vector>
#include <ctype.h>
#include <string.h>
#include <locale.h>
#include <stdlib.h>
#include <stdio.h>
#include "tools.hpp"

using namespace std;

/*
bool esdeseado(char letra){
	bool bandera=false;
	enie=(char)(-15);//asigno la ñ
    const char Caracteresdeseados[] = { 
    'a','b','c','d','e','f','g','h','i','j',
    'k','l','m','n','o','p','q','r','s','t',
    'v','u','w','x','y','z','0','1','2','3',
    '4','5','6','7','8','9',' ',enie};
    int longitud=strlen(Caracteresdeseados);

    for (int j = 0; j < longitud; j++){
            if (letra == Caracteresdeseados[j]){
            	bandera=true;
            	continue;
            		//cout << j << endl;
            		//system("PAUSE()"); 
            } 
        }
    return	bandera;		
}
*/
string DelCharIndeseados(string s){
    int Switch;
    string resultado="";
    int longitud=s.length();	
    for (int i = 0; i < longitud; i++){
    	if(esdeseado(s[i])){
    		resultado=resultado+s[i];
    	}
    }
    return resultado;
}

/*
string Lower(string s){
	int len=s.length();
	//cout<<'ñ'<<" - "<<'ó'<<endl;
	for (int i = 0; i < len; ++i)
	{
		//TOlower
		//cout << (char)-15 << s[i] <<(int)s[i] << endl;
        //	system("PAUSE()"); 
		if(s[i]>='A' && s[i] <='Z'){
			s[i]=(s[i] + 32);
		}
		//Quitar tildes
		//cout << (int)s[i]<< endl;system("PAUSE()");
		if( (int)s[i] == -31 or (int)s[i]== -63) s[i] = 'a'; 
		if(	(int)s[i] == -55 or (int)s[i]== -23) s[i] = 'e'; 
		if( (int)s[i] == -19 or (int)s[i]== -51) s[i] = 'i'; 
		if( (int)s[i] == -13 or (int)s[i]== -45 ) s[i] = 'o'; 
		if( (int)s[i] == -38 or (int)s[i]== -6 ) s[i] = 'u'; 
		//if( (int)s[i] == -15) s[i] = 'n'; 
		//system("PAUSE()");
		if(s[i] == '\n'){ s[i] = ' ';};
	}

	return s;
}
*/
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

int main (){

	setlocale(LC_ALL, "spanish");	
	setlocale(LC_CTYPE, "Spanish");
	
	string url;
	int cont=10000;
//-------------------lectura de data 
	string frase;
	int i=0;
	vector <string> textInFile(999999);
	ifstream ficheroEntrada;

	for(int k = 0;cont < 485000;k++){
		//ficheroEntrada.open ("../../raw.es/spanishText_10000_15000");
		url="../../raw.es/spanishText_"+to_string(cont)+'_'+to_string(cont+5000);
		//cout<<"["<<k<<"] "<<" url: "<<url<<endl;
		cont=cont+5000;

		ficheroEntrada.open (url);		
		if(ficheroEntrada.fail()){
			cout<<"["<<k<<"] "<<" url: "<<url<<" <- Not Found!"<<endl;
			//cout<<"no se pudo abrir el archivo"<<endl;
			continue;
		} else {
			cout<<"["<<k<<"] "<<" url: "<<url<<endl;
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
				textInFile[i] = textInFile[i] + frase+"\n";
				continue;		
			}
		}
	ficheroEntrada.close();
	}
//----------------------------------------------------------------
	ofstream archivo;
	archivo.open("raw.txt", ios::out);//abriendo el archivo
	if(archivo.fail()){
		cout<<"no se pudo abrir el archivo";
		exit(1);
	}
 //___________________________________________________________
	int posinit,posend,posid,postittle,posnfil,posproce,posdbindex;
	int a=i;
	
	for (int i = 0; i < a; ++i)
	{
		string sstart,ssend,ssid,ssttitle,ssnfil,ssprocesed,ssdbindex,mensaje;
		sstart='<',ssend='>',ssid='=',ssttitle='=',
		ssnfil='=',ssprocesed='=',ssdbindex='=';
		
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

		//cout<<textInFile[i].substr(posend+1)<<endl;

		//mensaje="|"+DelCharIndeseados(Lower(ssid))+"|"+DelCharIndeseados(Lower(ssttitle))+"|"+
		//DelCharIndeseados(Lower(ssnfil))+"|"+DelCharIndeseados(Lower(ssdbindex))+"|"+
		//DelCharIndeseados(Lower(textInFile[i].substr(posend+1)))+"\n";

		mensaje = "|" + DelCharIndeseados(Lower(ssdbindex))+
				  "|" + DelCharIndeseados(Lower(ssttitle)) +
				  //"|" + DelCharIndeseados(Lower(textInFile[i].substr(posend+1)))+
				  "|" + DelMin(Lower(textInFile[i].substr(posend+1)))+
				  "|";
		
		archivo << mensaje+"\n";
		printf("\rParcing <- %3.1f", 100*(float)i/((float)a));
	}
	
	archivo.close();
//---------------------------------------------------------------- 
	ficheroEntrada.close();
	return 0;
}