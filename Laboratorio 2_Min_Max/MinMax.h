#ifndef MINMAX_H
#define MINMAX_H

#include <iostream>
#include <vector>
#include <list>
#include <iterator>
#include <map>
#include "Tablero.h"

using namespace std;
//FUNCTIONS TO FIND POSIBILITIES
bool d1=false,d2=false;
map <int,int> v,h;


int posib_nxn (vector<vector<int>> vec, int n);
void check_h (vector<vector<int>> vec, int n,int x, int &cont);
void check_v (vector<vector<int>> vec, int n,int y,int &cont);
void check_d1 (vector<vector<int>> vec, int n,int &cont);
void check_d2 (vector<vector<int>> vec, int n,int &cont);
int posib_nxn (vector<vector<int>> vec, int n){
    list<int> a;
    int total = vec.size()*2+2;
    int cont = 0;
    bool exist = false;
    for (int i=0; i<vec.size(); i++) {
        for (int j=0; j<vec.size(); j++) {
            if (vec[i][j]== 3-n){
                if (h.find(i)==h.end())check_h(vec,n,i,cont);
                if (v.find(j)==v.end())check_v(vec,n,j,cont);
                if (i==j&&!d1) check_d1(vec,n,cont);
                if (i+j==vec.size()-1&&!d2) check_d2(vec,n,cont);
            } 
        }
    }
    return total-cont;
}
void check_h (vector<vector<int>> vec, int n,int x,int &cont){
    list <int>a;
    list <int>::iterator it;
    cout<<"h"<<endl;
    cont++;
    for (int i=0; i<vec.size(); i++) {
        if (vec[x][i]== 3-n){
                vec[x][i]=0;
                h.insert(pair<int,int>(x,i));
                a.push_back(i);
            } 
    }
    if (a.empty()) return;
    for (it = a.begin(); it!=a.end();it++){
        if (v.find(*it)==v.end())check_v(vec,n,*it,cont);
        if (x==*it&&!d1) check_d1(vec,n,cont);
        if (*it+x==vec.size()-1&&!d2) check_d2(vec,n,cont);
    }
}
void check_v (vector<vector<int>> vec, int n,int y,int &cont){
    list <int>a;
    list <int>::iterator it;
    cout<<"v"<<endl;
    cont++;
    for (int i=0; i<vec.size(); i++) {
        if (vec[i][y]== 3-n){
                vec[i][y]=0;
                v.insert(pair<int,int>(i,y));
                a.push_back(i);           
            } 
    }
    if (a.empty()) return;
    for (it = a.begin(); it!=a.end();it++){
        if (h.find(*it)==h.end())check_h(vec,n,*it,cont);
        if (y==*it&&!d1) check_d1(vec,n,cont);
        if (*it+y==vec.size()-1&&!d2) check_d2(vec,n,cont);
    }
}
void check_d1 (vector<vector<int>> vec, int n,int &cont){
    list <int>a;
    list <int>::iterator it;
    cout<<"d1"<<endl;
    cont++;
    d1 = true;
    for (int i = 0; i < vec.size(); i++){
        if(vec[i][i]== 3-n){
            vec[i][i]=0;
            a.push_back(i);
        }
    }
    if (a.empty()) return;
    for (it = a.begin(); it!=a.end();it++){
        if (h.find(*it)==h.end())check_h(vec,n,*it,cont);
        if (v.find(*it)==v.end())check_v(vec,n,*it,cont);
        if (*it*2==vec.size()-1&&!d2) check_d2(vec,n,cont);
    }
    
}
void check_d2 (vector<vector<int>> vec, int n,int &cont){
    list <int>a;
    list <int>::iterator it;
    cout<<"d2"<<endl;
    cont++;
    d2 = true;
    for (int i = 0; i < vec.size(); i++){
        if(vec[i][vec.size()-i]== 3-n){
            vec[i][vec.size()-i]=0;
            a.push_back(i);
        }
    }
    if (a.empty()) return;
    for (it = a.begin(); it!=a.end();it++){
        if (h.find(*it)==h.end())check_h(vec,n,*it,cont);
        if (v.find(vec.size()-*it)==v.end())check_v(vec,n,vec.size()-*it,cont);
        if (*it==vec.size()-(*it+1)&&!d2) check_d2(vec,n,cont);
    }
}

//FOR MINIMAX
struct Node;

struct Node {
	Tablero tablero;
	bool estado;
	int height;	//ESTADO-> 0:MAX, 1:MIN			HEIGHT->0,1...
	int nodeScore;
	int selectedChild;
	int colChanged, rowChanged;	//index of the col and row that has been changed
	Node *back; /// padre
	vector<Node *> nodes;
	
	Node(int height, Node *back, Tablero tablero) {
		this->height = height;
		this->back = back;
		this->tablero = tablero;
		estado = 0;
	}
	
	~Node () {
		while ( !nodes.empty () ) {
			Node *node = nodes.back ();
			delete node;
			nodes.pop_back ();
		}
	}
};


struct MinMaxTree{
	int maxHeight;
	Node *root;
	
	MinMaxTree(int maxHeight, Tablero tableroInicial) {
		this->maxHeight = maxHeight;
		root = new Node(0, nullptr, tableroInicial);
	}
	
	int createTree(Node* actual, int turn){
		//stop condition if the max height is reached
		if(actual->height == maxHeight){
			//find value for leafs
			vector<vector<int>> vect = {{0,0,0},{0,0,0},{0,0,0}};
			for (size_t i = 0; i < 3; i++)
			{
				for (size_t j = 0; j < 3; j++)
				{
					if(actual->tablero.casillas[i][j]==0)
						vect[i][j]=1;
					if(actual->tablero.casillas[i][j]==1)
						vect[i][j]=2;
				}				
			}
			if(turn)//player turn
				return (posib_nxn(vec,turn+1) - posib_nxn(vec,turn));
			else
				return (posib_nxn(vec,turn+1) - posib_nxn(vec,turn + 2));
		}
		//#posibilities
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 3; j++)
			{
				//empty block
				if(actual->tablero.casillas[i][j] == -1){
					//create a new node
					Tablero tempTablero = actual->tablero;
					tempTablero[i][j] = turn;
					Node *newNode = new Node(actual->height + 1, actual, tempTablero);
					newNode->rowChanged = i;
					newNode->colChanged = j;
					newNode->estado = !(actual->estado);

					actual->nodes.push_back(newNode);
				}
			}			
		}
		//iterate over the children
		if(actual->estado)
			actual->nodeScore = 999999;
		else
			actual->nodeScore = -999999;
		for (size_t i = 0; i < actual->nodes.size(); i++)
		{
			int tempValue = createTree(actual->nodes[i], !turn);
			if(actual->estado){
				if(tempValue < actual->nodeScore){
					actual->nodeScore = tempValue;
					actual->selectedChild = i;
				}
			}
			else{
				if(tempValue > actual->nodeScore){
					actual->nodeScore = tempValue;
					actual->selectedChild = i;
				}
			}
		}		
		return actual->nodeScore;
		
	}

	vector<int> returnAnswer(){
		vector<int> answer;
		answer.push_back(root->nodes[root->selectedChild]->rowChanged);
		answer.push_back(root->nodes[root->selectedChild]->colChanged);
		return answer;
	}
	
	
	void generalArbol(Node *actual, int turno) {
		//capturar Tablero.casillas
		//	convertir matriz a vector de vectores
		/*
		Crear nodos hijos de nivel (según espacios en blanco)
		*/
	}
}

#endif
