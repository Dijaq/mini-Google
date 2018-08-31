#include <iostream>
#include <fstream>
#include <string>
#include "Suffix-Tree.hpp"

using namespace std;

string readQuery();

int main(){
    string OldQuery="$";
    string Query;
    MiniGoogle *mg = new MiniGoogle(1);
    MakeSuffixTree(mg);

    char modo;

    while (true){
        cout<<"\nModo COnsola(C/c) o Modo App(A/p) :";
        cin >>modo;
        if (modo == 'C' or modo == 'c' or modo == 'A' or modo == 'a'){
            break;
        }
    }


    if (modo == 'C' or modo == 'c'){
        while (true){
            cout<<"\n\nBusqueda"<<endl;
            cout<<"============================================================"<<endl;
            string word;
            cout<<"\nQuery: ";
            getline(cin, word);
            word = Lower(word);
            if (word.size()==0){continue;}

            mg->find(word, true);
                   
            cout<<"\n================================== Final de busqueda ("<< word << ")================================\n"<<endl;     
        }
    } 
    else if (modo == 'A' or modo == 'a'){
        while (true){
            Query = readQuery();
            if (Query != OldQuery && Query.length()!=0){
                //cout<<"Query: "<< Query;
                //cout<<", length: "<< Query.length()<<endl;

                cout<<"\n\nBusqueda"<<endl;
                cout<<"============================================================"<<endl;
                string word;
                //clock_t t;

                //cout<<"\nQuery: ";
                //getline(cin, word);
                word = Query;
                word = Lower(word);
                if (word.size()==0){continue;}
                    //t = clock();
                    //cout << "================================ Resultado de busqueda ("<< word<< ")================================" << endl;    
                mg->find(word, false);
                    //t = clock() - t;
                    //printf ("\n<======= (%.8f seconds)======\n",((float)t)/CLOCKS_PER_SEC);
                   
                cout<<"\n================================== Final de busqueda ("<< word << ")================================\n"<<endl;     
            }


            
            OldQuery = Query;
        }       
    } 


    return 0;
}

string readQuery(){
    string      line;
    ifstream    myfile("comm/log.txt");

    if (myfile.is_open()){
        getline(myfile, line);
        //cout<< line << '\n';
        myfile.close();
        return line;
    }
    else {
        cout<< "Unable to open file!";
        return "$";
    }
}
