#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

string recorrer(string s,int  pos){
    string resultado;
    s=s.substr(pos+1);int i;
    for(i=1;i<s.length();i++){
        if(s[i] == '|'){
            break;
        } 
    }   
    resultado=s.substr(0,i);

    return resultado;
}

int main()
{


//--------------Lectura de data------------------------
    string frase;
    string doc1 = "</doc>";
    string endofarticle = "ENDOFARTICLE.";
    int i=0;
    vector <string> contenido(999999);
    vector <int> id(999999);
    vector <int> dbindex(999999);
    vector <string> title(999999);
    vector <string> nonfiltered(999999);
    vector <string> textInFile(999999);
    int posend,posid,postittle,posnfil,posdbindex;
    string ssid,ssttitle,ssnfil,ssdbindex,sscontenido;

    ifstream ficheroEntrada;
    ficheroEntrada.open ("raw.txt");
    int k=0;
    while(!ficheroEntrada.eof()){  

        getline(ficheroEntrada, frase);
        if(frase.length() == 0)
            continue;

        k++;
              
        if(k<20)cout <<"TEXTO" <<endl;
        textInFile[i] = textInFile[i] + frase;
        string sstart="|";

        /*
        posid=textInFile[i].find(sstart);
        id[i]=stoi(recorrer(textInFile[i],posid));

        postittle=textInFile[i].find(sstart,posid+1);
        title[i]=recorrer(textInFile[i],postittle);  

        posnfil=textInFile[i].find(sstart,postittle+1);
        nonfiltered[i]=recorrer(textInFile[i],posnfil);

        posdbindex=textInFile[i].find(sstart,posnfil+1);
        dbindex[i]=stoi(recorrer(textInFile[i],posdbindex)); 

        posend=textInFile[i].find(sstart,posdbindex+1);
        contenido[i]=textInFile[i].substr(posend+1);

        cout<<i<<" mensaje: "<<id[i]<<"|"<<title[i]<<"|"<<nonfiltered[i]<<"|"<<dbindex[i] << endl;

        */

        posdbindex      =   textInFile[i].find(sstart);
        dbindex[i]      =   stoi(recorrer(textInFile[i],posdbindex));

        postittle       =   textInFile[i].find(sstart,posdbindex+1);
        title[i]        =   recorrer(textInFile[i],postittle);

        posend          =   textInFile[i].find(sstart,postittle+1);
        contenido[i]    =   textInFile[i].substr(posend);

        cout<<"|"<<dbindex[i]<<"|"<<title[i]<<"|"<<endl;//<<contenido[i] << endl;
        i++;                 
             
     }    
}