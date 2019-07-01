#include<bits/stdc++.h> 
using namespace std; 

 
struct Node{ 
	float data;
	int degree, i;
	Node *child, *sibling, *parent; 
}; 


Node* newNode(float key, int i){ 
	Node *temp = new Node; 
	temp->data = key; 
	temp->degree = 0;
	temp->i = i;
	temp->child = temp->parent = temp->sibling = NULL; 
	return temp; 
} 

Node* mergeBinomialTrees(Node *b1, Node *b2){ 
	if (b1->data > b2->data) 
		swap(b1, b2); 

	b2->parent = b1; 
	b2->sibling = b1->child; 
	b1->child = b2; 
	b1->degree++; 

	return b1; 
} 

list<Node*> unionBionomialHeap(list<Node*> l1, list<Node*> l2){ 
	list<Node*> newList; 
	list<Node*>::iterator it = l1.begin(); 
	list<Node*>::iterator ot = l2.begin(); 
	while (it!=l1.end() && ot!=l2.end()){ 
		if((*it)->degree <= (*ot)->degree){ 
			newList.push_back(*it); 
			it++; 
		} 
		else{ 
			newList.push_back(*ot); 
			ot++; 
		} 
	} 

	while(it != l1.end()){ 
		newList.push_back(*it); 
		it++; 
	} 

	while(ot!=l2.end()){ 
		newList.push_back(*ot); 
		ot++; 
	} 

	return newList; 
} 


list<Node*> adjust(list<Node*> heap){ 
	if(heap.size() <= 1) 
		return heap; 
	list<Node*> newheap; 
	list<Node*>::iterator it1,it2,it3; 
	it1 = it2 = it3 = heap.begin(); 

	if(heap.size() == 2){ 
		it2 = it1; 
		it2++; 
		it3 = heap.end(); 
	} 
	else{ 
		it2++; 
		it3=it2; 
		it3++; 
	} 
	while(it1 != heap.end()){ 
		if(it2 == heap.end()) 
			it1++; 

		else if((*it1)->degree < (*it2)->degree){ 
			it1++; 
			it2++; 
			if(it3!=heap.end()) 
				it3++; 
		} 

		else if(it3!=heap.end() && (*it1)->degree == (*it2)->degree && (*it1)->degree == (*it3)->degree){ 
			it1++; 
			it2++; 
			it3++; 
		} 

		else if((*it1)->degree == (*it2)->degree){ 
			Node *temp; 
			*it1 = mergeBinomialTrees(*it1,*it2); 
			it2 = heap.erase(it2); 
			if(it3 != heap.end()) 
				it3++; 
		} 
	} 
	return heap; 
} 

list<Node*> insertATreeInHeap(list<Node*> heap, Node *tree){ 
	list<Node*> temp; 
	temp.push_back(tree);  
	temp = unionBionomialHeap(heap,temp); 
	return adjust(temp); 
} 

list<Node*> removeMinFromTreeReturnBHeap(Node *tree){ 
	list<Node*> heap; 
	Node *temp = tree->child; 
	Node *lo; 

	while(temp){ 
		lo = temp; 
		temp = temp->sibling; 
		lo->sibling = NULL; 
		heap.push_front(lo); 
	} 
	return heap; 
} 

list<Node*> insert(list<Node*> _head, float key, int i){ 
	Node *temp = newNode(key, i); 
	return insertATreeInHeap(_head,temp); 
} 

Node* getMin(list<Node*> heap){ 
	list<Node*>::iterator it = heap.begin(); 
	Node *temp = *it; 
	while(it != heap.end()){ 
		if((*it)->data < temp->data) 
			temp = *it; 
		it++; 
	} 
	return temp; 
} 

list<Node*> extractMin(list<Node*> heap){ 
	list<Node*> newheap,lo; 
	Node *temp; 

	temp = getMin(heap); 
	list<Node*>::iterator it; 
	it = heap.begin(); 
	while (it != heap.end()){ 
		if (*it != temp){ 
			newheap.push_back(*it); 
		} 
		it++; 
	} 
	lo = removeMinFromTreeReturnBHeap(temp); 
	newheap = unionBionomialHeap(newheap,lo); 
	newheap = adjust(newheap); 
	return newheap; 
} 

/*
void decrease_key(list<Node*> h, int v, float peso){
    
    while(v > 0 && (h->nos[v]).valor < (h->nos[parent(v)]).valor){
        swap(h, v, parent(v));
        v = parent(v);
    }
}
*/

void printTree(Node *temp, int i){
	while (temp){ 
		if(temp->i == i){
			cout<< temp->data << endl;
		}
       	//cout << temp->data << " "; 
       	printTree(temp->child, i); 
       	temp = temp->sibling; 
    }  
} 

void printar(list<Node*> heap, int i){
	list<Node*>::iterator it;
	it = heap.begin();
	while(it != heap.end()){
		printTree(*it, i);
		it++;
	}
}

int main(){  
	list<Node*> heap; 
 
	heap = insert(heap, 20.0, 0); 
	heap = insert(heap, 30.0, 1); 
	heap = insert(heap, 10.0, 2);

	printar(heap, 2);

	Node *temp = getMin(heap); 
	cout << "Minimum element of heap "<< temp->data << "\n"; 

	heap = extractMin(heap);  

	return 0; 
} 
