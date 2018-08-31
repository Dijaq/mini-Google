#include <time.h> 
#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <set>
#include <functional>
using namespace std;




class Edge;

class Node
{
private:
	vector<Edge> edges;

	friend class Edge;
	friend class SuffixTree;
	friend class MiniGoogle;
};

class Edge
{
private:
	string suffixString;
	Node *startNode;
	Node *endNode;
	vector<int> indexes; // dbindex of each text 
	vector<int> coincidences; // How many coincidences of the suffix for each text

public:
	Edge(Node *startNode, string suffixString, int index)
	{
		this->suffixString = suffixString;
		this->startNode = startNode;
		this->indexes.push_back(index);
		this->coincidences.push_back(1);
		this->endNode = NULL;
	}

	//Constructor in order to create a new node after an split because we need to store all the before coincidences
	Edge(Node *startNode, string suffixString, Edge edge)
	{
		this->suffixString = suffixString;
		this->startNode = startNode;
		this->indexes = edge.indexes;
		this->coincidences = edge.coincidences;
		this->endNode = NULL;
	}

	//Adding a new text index for a given suffix (when the suffix exist in the tree)
	void putOtherIndex(int index)
	{
		this->indexes.push_back(index);
		this->coincidences.push_back(1);
	}

	//Precaution: This part its a trouble to parallelize; because it has a dependence from the last one
	//inserted index.
	void addCantCoicidencia()
	{
		this->coincidences[coincidences.size() - 1] += 1;
	}
	friend class Node;
	friend class SuffixTree;
	friend class MiniGoogle;
};

class Word
{

public:
	Word() {}

	vector<string> getWords(string text)
	{
		vector<string> words;

		/*int posSpace;
		string word;

		while (text.size() >= 1)
		{
			posSpace = text.find(" ");

			if (posSpace == -1)
			{
				words.push_back(text);
				break;
			}

			word = text.substr(0, posSpace);

			if (word.size() > 0)
				words.push_back(word);

			text = text.substr(posSpace + 1);
		}*/

		istringstream isstream(text);
		string tempStr;

		while (isstream >> tempStr)
		{
			//isstream >> tempStr;
			words.push_back(tempStr);
		}
		return words;
	}

	//Getting the word suffixes and set up their indexes.
	vector<string> getSuffix(string word)
	{
		vector<string> suffix;

		int i = word.size() - 1;

		for (; i >= 0; i--)
		{
			suffix.push_back(word.substr(i));
		}
		return suffix;
	}

	friend class SuffixTree;
	friend class MiniGoogle;
};

class SuffixTree
{
private:
	Node *root;

public:
	SuffixTree()
	{
		this->root = new Node();
	}


	//{{'a','b'}}
	//Start point to add the words inside the tree. First we can read the text and separate each word, 
	//after that for each word we can extract the suffix in order to add them in the tree. Also we must have
	//like parameter the index (dbindex) of the text.
	void execution(string text, int index)
	{
		Word w;
		vector<string> words = w.getWords(text);
		vector<string> suffix;
		int i = 0;
		int wordsSize = words.size();
		int suffixSize;
		for (; i<wordsSize; i++)
		{
			//cout<<"----------------"<<endl;
			//cout<<"-"<<words[i].word<<"-"<<words[i].index<<"-"<<endl;

			suffix = w.getSuffix(words[i]);

			suffixSize = suffix.size();
			for (int j = 0; j<suffixSize; j++)
			{
				//cout<<"-"<<suffix[j].word<<"-"<<suffix[j].index<<"-"<<endl;
				addSuffix(suffix[j], index);
				//print();
				//return;
			}
			suffix.clear();
		}
		
		words.clear();
	}

	//This function search return the correct node where we can insert the word suffix.
	//Also it returns the edge position where the suffix has a coincidence (if it is -1 there is not a coincidence).
	//On the other side, it returns the coincidence string with respect to the position to insert.
	Node * search(string &suffix, string &parteCoinciden, Node *node, int &edgePos, int index)
	{
		if (node->edges.size() == 0)
			return node;
		edgePos = -1;

		parteCoinciden = "";

		string substring1, substring2;
		for (int i = 0; i<node->edges.size(); i++)
		{
			substring1 = suffix.substr(0, 1);
			substring2 = node->edges[i].suffixString.substr(0, 1);

			if (substring1 == substring2)
			{
				//----------Agregar coicidencias
				//if the suffix is from a different text (push_back)
				if (node->edges[i].indexes[node->edges[i].indexes.size() - 1] != index)
				{
					node->edges[i].putOtherIndex(index);
				}
				else //If the suffix is from the same text, add coincidence(+1).
				{
					node->edges[i].addCantCoicidencia();
				}
				//---------------Fin Agregar coicidencias
				parteCoinciden += substring1;
				suffix = suffix.substr(1);
				edgePos = i;

				int j = 0, k = 1;
				for (; j<suffix.size() && k<node->edges[i].suffixString.size(); j++, k++)
				{
					substring1 = suffix.substr(j, 1);
					substring2 = node->edges[i].suffixString.substr(k, 1);


					edgePos = i;

					if (substring1 != substring2)
					{
						break;
					}
					parteCoinciden += substring1;

				}
				suffix = suffix.substr(j);

				if ((node->edges[i].endNode) != NULL && suffix.size()>0)
				{
					return search(suffix, parteCoinciden, node->edges[i].endNode, edgePos, index);
				}

				return node;
			}

		}

		return node;
	}

	// Recursive search: it searches the correct node from to root.
	Node * search(string &suffix, string &parteCoinciden, int &edgePos, int index)
	{
		return search(suffix, parteCoinciden, root, edgePos, index);
	}


	//Search: the same idea like the search in the tree; however in this case we store all the coincidence characters without
	//remove anything for each recursive call.
	Node * search2(string &suffix, string &parteCoinciden, Node *node, int &edgePos)
	{
		edgePos = -1;
		if (node->edges.size() == 0)
			return node;

		string substring1, substring2;
		for (int i = 0; i<node->edges.size(); i++)
		{
			substring1 = suffix.substr(0, 1);
			substring2 = node->edges[i].suffixString.substr(0, 1);
			if (substring1 == substring2)
			{
				parteCoinciden += substring1;
				suffix = suffix.substr(1);
				edgePos = i;
				int j = 0, k = 1;
				for (; j<suffix.size() && k<node->edges[i].suffixString.size(); j++, k++)
				{
					substring1 = suffix.substr(j, 1);
					substring2 = node->edges[i].suffixString.substr(k, 1);


					edgePos = i;

					if (substring1 != substring2)
					{
						break;
					}
					parteCoinciden += substring1;

				}
				suffix = suffix.substr(j);


				if ((node->edges[i].endNode) != NULL && suffix.size()>0)
				{
					return search2(suffix, parteCoinciden, node->edges[i].endNode, edgePos);
				}

				return node;
			}

		}

		return node;
	}

	// Recursive search: it searches the text where a given word exists after the insertion of the data.
	Node * search2(string &suffix, string &parteCoinciden, int &edgePos)
	{
		parteCoinciden = "";
		return search2(suffix, parteCoinciden, root, edgePos);
	}

	// Function to add all the suffix in the tree.
	void addSuffix(string suffix, int index)
	{
		//In the case we use the $ sign, we must consider the next line
		suffix += "$";
		int edgePos = -1; //Edge position
		string parteCoinciden = "";
		//The search give us inside suffix the remained part to add in the tree.
		Node *node = search(suffix, parteCoinciden, edgePos, index);

		if (edgePos == -1) // If not exist the appropiate edge, we add a new element to the node.
		{
			node->edges.push_back(Edge(node, suffix, index));
		}
		else
		{
			string coinciden = parteCoinciden;
			string noCoinciden = suffix;
			string noCoincidenEdge = node->edges[edgePos].suffixString.substr(coinciden.size());
			//cout << "coincide:" << coinciden <<"- restoSuf:" << noCoinciden << "- resto edge:" << noCoincidenEdge << "-" << endl;

			node->edges[edgePos].suffixString = coinciden;

			//When we need to add a new node and when there is nothing to add from the suffix word.
			if (node->edges[edgePos].endNode == NULL && (noCoinciden.size()>0 || noCoincidenEdge.size()>0))
			{
				Node * newNode = new Node();
				node->edges[edgePos].endNode = newNode;

				if (noCoinciden.size()>0)
					newNode->edges.push_back(Edge(newNode, noCoinciden, index));

				if (noCoincidenEdge.size()>0)
					newNode->edges.push_back(Edge(newNode, noCoincidenEdge, node->edges[edgePos]));
			}
			else
			{
				if (noCoinciden.size()>0)
					node->edges.push_back(Edge(node, noCoinciden, index));
			}

			//In case all the suffix coincide in the tree (we must not to add again).
			/*if (noCoinciden.size() == 0)
			{
				//if the suffix is from a different text (push_back)

				if (node->edges[edgePos].indexes[node->edges[edgePos].indexes.size() - 1] != index)
				{
					node->edges[edgePos].putOtherIndex(index);
				}
				else //If the suffix is from the same text, add coincidence(+1).
				{
					node->edges[edgePos].addCantCoicidencia();
				}
			}*/
		}



		//system("pause");
	}

	void print(Node *node, int profundidad)
	{
		string aux = "";
		string indexs;
		for (int i = 0; i<profundidad; i++)
			aux += "\t";
		for (int i = 0; i<node->edges.size(); i++)
		{
			indexs = "";
			for (int j = 0; j <node->edges[i].indexes.size(); j++)
				indexs += "-" + to_string(node->edges[i].indexes[j]);
			indexs += "-";

			cout << aux << "suffix: -" << node->edges[i].suffixString << "- \t indexs: " << indexs << endl;
			if (node->edges[i].endNode != NULL)
			{
				print(node->edges[i].endNode, profundidad + 1);
			}
		}
	}

	void print()
	{
		cout << "--------- Impresion del Arbol ---------" << endl;
		print(root, 2);
		cout << endl;
	}

	friend class MiniGoogle;
};


class MiniGoogle
{
private:
	int cantTree; // Number of trees to use
	SuffixTree *trees; // Vector that store all the trees
							  //SuffixTree tress;
	map<int, string> documentTitles; // Map that stores index, titles.
	int idTree;

public:
	MiniGoogle(int cantTree)
	{
		this->cantTree = cantTree;
		this->idTree = 0;
		trees = new SuffixTree[cantTree];
		for (int i = 0; i < cantTree; i++)
		{
			trees[i] = SuffixTree();
		}

	}

	//Inserting all the documents. posTree: tree which have the document, id: document id, dbindex(using it), title, content
	void putDocument(int posTree, int id, int dbindex, string title, string content)
	{
		trees[posTree].execution(title, dbindex);
		trees[posTree].execution(content, dbindex);
		documentTitles[dbindex] = title;
	}

	void putDocument(int id, int dbindex, string title, string content)
	{
		putDocument(idTree, id, dbindex, title, content);
		idTree++;
		if (cantTree <= idTree)
			idTree = 0;
		//print();
	}

	void buscarIndicesHojas(Edge edge, map<int, int> &result)
	{
		//if (edge.endNode == NULL)//Significa q llego a una arista hoja
		{
			for (int j = 0; j < edge.indexes.size(); j++)
			{
				/*if (result.find(edge.indexes[j]) != result.end()) //si lo contiene
				{
					result[edge.indexes[j]] = result.find(edge.indexes[j])->second + edge.coincidences[j];
				}
				else
				{
					result[edge.indexes[j]] = edge.coincidences[j];
				}*/
				map<int, int>::iterator it = result.find(edge.indexes[j]);
				if (it != result.end())
					(*it).second = (*it).second + edge.coincidences[j];
				else
					result[edge.indexes[j]] = edge.coincidences[j];
			}
		}
		/*else
		{
			for (int j = 0; j < edge.endNode->edges.size(); j++)
			{
				buscarIndicesHojas(edge.endNode->edges[j], result);
			}
		}*/
	}

	template<typename K, typename V>
	std::multimap<V, K> invertMap(std::map<K, V> const &map)
	{
		std::multimap<V, K> multimap;

		for (auto const &pair : map) {
			multimap.insert(std::make_pair(pair.second, pair.first));
		}

		return multimap;
	}

	//Function to return a map with the keys and values; where the key is the index of the text and the value is the
	//coincidence number in the tree.
	map<int, int> search(string texto)
	{
		map<int, int> result;//key, value
		for (int i = 0; i < cantTree; i++)
		{
			int edgePos = -1;
			string parteCoinciden = "";
			string suffix = texto;
			Node *node = trees[i].search2(suffix, parteCoinciden, edgePos);


			//cout << "suff:" << suffix << " coicide:" << parteCoinciden << endl;

			if (edgePos != -1 && suffix.size() == 0)
			{
				buscarIndicesHojas(node->edges[edgePos], result);
			}
		}

		return result;
	}

	void print()
	{
		cout << "================================ Imprimiendo Arboles ================================" << endl;
		for (int i = 0; i<cantTree; i++)
		{
			cout << "--------------- Arbol " << to_string(i) << "----------------" << endl;
			trees[i].print();
		}
		cout << endl;
	}

	void printResult2(map<int, int> result, string texto)
	{
		cout << "\n\n================================ Resultado de busqueda (" << texto << ")================================\n" << endl;
		if (result.size() == 0)
			cout << "No hay coicidencia" << endl;

		int i = 0;
		std::multimap<int, int>::reverse_iterator it;
		for (it = result.rbegin(); it != result.rend(); ++it) {
			printf("[%2d] dbIndex: %8d | cnds: %8d \n", i + 1, it->first, it->second);
			//cout<<"titulo: " << resultTitles[i]<<endl;
			i++;
		}
	}

	void printResult(multimap<int, int> result, string texto)//, vector<string> resultTitles)
	{
		cout << "================================ Resultado de busqueda (" << texto << ")================================" << endl;
		if (result.size() == 0)
			cout << "No hay coicidencia" << endl;

		int i = 0;
		std::multimap<int, int>::reverse_iterator rit;
		//for (auto const &pair: result) 

		for (rit = result.rbegin(); rit != result.rend(); ++rit)
		{
			//resultTitles.push_back(documentTitles[rit->second]);
			std::cout << "dbIndex: " << rit->second << "\t coincidencias: " << rit->first << "\n";
			//std::cout <<"dbIndex: "<< rit->second << " \t titulo: " << resultTitles[i] << "\t coincidencias: " << rit->first << "\n";
			i++;
		}
	}

	//Function that return a map with the text index and the number of the coincidence
	void find(string text)
	{
		/*
		map<int, int> result = search(texto);

		vector<string> resultTitles;

		printResult(result, texto);
		*/

		//New --------------------------------------------------------------->
		cout << text << endl;
		Word word;
		vector<string> words = word.getWords(text);
		//vector<multimap<int,int>> mapsResults;
		cout << "size = " << words.size() << endl;

		if (words.size() == 1) {
			vector<map<int, int>> maps;

			for (string t : words)
			{
				maps.push_back(search(t));
			}

			int key, value; //key: dbindex, value: nro coincidences
			map<int, int> result;

			for (int i = 0; i<maps.size(); i++)
			{
				for (map<int, int>::const_iterator it = maps[i].begin(); it != maps[i].end(); ++it)
				{
					vector<int> coincidences;
					key = it->first;
					value = it->second;
					coincidences.push_back(value); //all the coincidences for the key.

					map<int, int>::iterator itAux;
					for (int k = i + 1; k<maps.size(); k++)
					{
						itAux = (maps[k]).find(key);

						if (itAux != maps[k].end())//si existe
						{
							value = itAux->second;
							coincidences.push_back(value);
							maps[k].erase(itAux);
						}
						else
						{
							coincidences.push_back(0);
						}
					}

					int min = 1000000000;
					for (int j = 0; j<coincidences.size(); j++)
					{
						if (coincidences[j]<min)
							min = coincidences[j];
					}
					result[key] = min;
				}

			}
			printResult2(result, text);
			/*
			multimap<int,int> multimap = invertMap(result);
			vector<string> resultTitles;


			std::multimap<int,int>::reverse_iterator rit;
			for (rit=multimap.rbegin(); rit!=multimap.rend(); ++rit)
			{
			//std::cout << rit->first << " => " << rit->second << '\n';
			resultTitles.push_back(documentTitles[rit->second]);
			}

			printResult(multimap, text);
			*/
		}
		else {


			vector<map<int, int>> maps;

			for (string t : words)
			{
				maps.push_back(search(t));
			}

			int key, value; //key: dbindex, value: nro coincidences
			map<int, int> result;

			for (int i = 0; i<maps.size(); i++)
			{
				for (map<int, int>::const_iterator it = maps[i].begin(); it != maps[i].end(); ++it)
				{
					vector<int> coincidences;
					key = it->first;
					value = it->second;
					coincidences.push_back(value); //all the coincidences for the key.

					map<int, int>::iterator itAux;
					for (int k = i + 1; k<maps.size(); k++)
					{
						itAux = (maps[k]).find(key);

						if (itAux != maps[k].end())//si existe
						{
							value = itAux->second;
							coincidences.push_back(value);
							maps[k].erase(itAux);
						}
						else
						{
							coincidences.push_back(0);
						}
					}

					int min = 1000000000;
					for (int j = 0; j<coincidences.size(); j++)
					{
						if (coincidences[j]<min)
							min = coincidences[j];
					}
					result[key] = min;
				}

			}

			multimap<int, int> multimap = invertMap(result);
			vector<string> resultTitles;


			std::multimap<int, int>::reverse_iterator rit;
			for (rit = multimap.rbegin(); rit != multimap.rend(); ++rit)
			{
				//std::cout << rit->first << " => " << rit->second << '\n';
				resultTitles.push_back(documentTitles[rit->second]);
			}

			printResult(multimap, text);
			//New <---------------------------------------------------------------

			
		}
	}



	friend class Node;
	friend class SuffixTree;

};

string recorrer(string s, int  pos) {
	string resultado;

	s = s.substr(pos + 1);int i;
	for (i = 1;i<s.length();i++) {
		if (s[i] == '|') {
			break;
		}
	}
	resultado = s.substr(0, i);

	return resultado;
}

int main()
{

	MiniGoogle *mg = new MiniGoogle(1);


	//--------------Lectura de data------------------------
	string frase;
	int i = 0;


	string textInFile="";
	int dbindex;
	string title;
	string contenido;

	int posend, posid, postittle, posnfil, posdbindex;
	string ssid, ssttitle, ssnfil, ssdbindex, sscontenido;
	//string nonfiltered;

	ifstream ficheroEntrada;
	ficheroEntrada.open("raw2.txt");
	
	clock_t t2;
	t2 = clock();

	if (ficheroEntrada.is_open())
	{
		cout << "Lectura correcta\n" << endl;
		cout << "================\n\n" << endl;

		while (!ficheroEntrada.eof())
		{
			//READ
			getline(ficheroEntrada, frase);
			if (frase.length() == 0)
				continue;

			//======================Number of text to read============================================
			/*if(i > 100){
			    break;
			}*/
			//si no es cabecera ni fin asignarle  variables.

			textInFile = textInFile + frase;
			string sstart = "|";

			posdbindex = textInFile.find(sstart);
			dbindex = stoi(recorrer(textInFile, posdbindex));


			postittle = textInFile.find(sstart, posdbindex + 1);
			title = recorrer(textInFile, postittle);

			posend = textInFile.find(sstart, postittle + 1);
			contenido = recorrer(textInFile, posend + 1);

			mg->putDocument(0, dbindex, title, contenido);

			textInFile = "";
			i++;

			printf("\rSuffix-Tree <- %3.2f %%", 100 * (float)i / 259409.0);
		}
		/*
		while (!ficheroEntrada.eof()) {
			//READ
			getline(ficheroEntrada, frase);
			if (frase == "</doc>") {
				continue;
			}

			//======================Number of text to read============================================
			if (i > 100) {
				break;
			}

			if (frase == "ENDOFARTICLE.") {
				i++;
				textInFile = "";
				continue;
			}
			//si no es cabecera ni fin asignarle  variables.
			if (frase != "ENDOFARTICLE." && frase != "</doc>") {
				//buscar posiciones y dividir campos
				textInFile = textInFile + frase;
				string sstart = "|";

				posdbindex = textInFile.find(sstart);
				dbindex = stoi(recorrer(textInFile, posdbindex));

				postittle = textInFile.find(sstart, posdbindex + 1);
				title = recorrer(textInFile, postittle);

				posnfil = textInFile.find(sstart, postittle + 1);
				nonfiltered = recorrer(textInFile, posnfil);

				posdbindex = textInFile.find(sstart, posnfil + 1);
				dbindex = stoi(recorrer(textInFile, posdbindex));

				posend = textInFile.find(sstart, posdbindex + 1);
				contenido = textInFile.substr(posend + 1);
				
				mg->putDocument(0, dbindex, title, contenido);
				continue;
			}
		}*/
	}
	else
	{
		cout << "¡Error! El archivo no pudo ser abierto." << endl;
	}
	ficheroEntrada.close();
	
	t2 = clock() - t2;
	printf("\n<Tiempo Lectura (%.8f seconds)======\n", ((float)t2) / CLOCKS_PER_SEC);


	cout << "\n\nBusqueda:\n" << endl;
	cout << "=========\n" << endl;
	string word;
	clock_t t;

	int ik = 0;

	while (true) {
		cout << "Query: ";

		//cin>>word;
		getline(cin, word);
		t = clock();
		mg->find(word);

		t = clock() - t;

		printf("\n<======= (%.8f seconds)======\n", ((float)t) / CLOCKS_PER_SEC);
		cout << "\n================================ Resultado de busqueda (" << word << ")================================\n" << endl;
		printf("It took me %d clicks (%f seconds).\n", t, ((float)t) / CLOCKS_PER_SEC);

		if(ik>=2)
			break;
		ik++;
	}


	system("pause"); 
	return 0;
}
