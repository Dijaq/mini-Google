#include <string.h>
#include <iostream>

using namespace std;

string Lower(string s){
	int len=s.length();
	string result;
	char currentChar;
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
		if( (int)s[i] == -31 or (int)s[i]== -63 or (int)s[i]==-95 or (int)s[i]==-127) s[i] = 'a'; 
		if(	(int)s[i] == -55 or (int)s[i]== -23 or (int)s[i]==-87 or (int)s[i]==-119) s[i] = 'e'; 
		if( (int)s[i] == -19 or (int)s[i]== -51 or (int)s[i]==-83 or (int)s[i]==-115) s[i] = 'i'; 
		if( (int)s[i] == -13 or (int)s[i]== -45 or (int)s[i]==-77 or (int)s[i]==-109) s[i] = 'o'; 
		if( (int)s[i] == -38 or (int)s[i]== -6  or (int)s[i]==-70 or (int)s[i]==-102 ) s[i] = 'u'; 

		currentChar = s[i];
		if( (int)s[i] != -61){
			result += currentChar;
		} 
		//if( (int)s[i] == -15) s[i] = 'n'; 
		//system("PAUSE()");
		cout<<s[i]<<"->"<<(int)s[i]<<endl;
		//if(s[i] == '\n'){ s[i] = ' ';};
	}
	return result;
	return s;
}














