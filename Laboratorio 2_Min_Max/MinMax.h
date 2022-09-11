#ifndef MINMAX_H
#define MINMAX_H

#include <iostream>
#include <vector>
#include "Tablero.h"

using namespace std;

struct Node;

struct Node {
	Tablero tablero;
	int estado,  height;
	int idxCadena;
	Node *back; /// padre
	vector<Node *> nodes;
	
	Node(int height, Node *back) {
		this->height = height
			this->back = back
			estado = 0;
		idxCadena = -1;
	}
	
	~Node () {
		while ( !nodes.empty () ) {
			Node *node = nodes.back ();
			delete node;
			nodes.pop_back ();
		}
	}
};


struct MinMax{
	int maxHeight;
	Node *root;
	
	MinMax(int maxHeight) {
		this->maxHeight = maxHeight;
		root = new Node(0, nullptr)
			createTree(root, 1, maxHeight);
	}
	
	createTree(Node *actual, int height, int maxHeight) {
		if(maxHeight == 0) return;
		
		for(int i = 0; i < 10 - height; ++i) {
			Node *node  = new Node(height, maxHeight);
			actual->nodes.push_back(node);
			createTree(node, height + 1, maxHeight - 1)
		}
	}
	
	void generalArbol(Node *actual, int turno) {
		
	}
}

#endif
