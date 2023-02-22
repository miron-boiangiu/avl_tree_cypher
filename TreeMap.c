#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "TreeMap.h"

#define MAX(a, b) (((a) >= (b))?(a):(b))

// Trebuie sa fac asta deoarece vmchecker nu imi permite sa dau upload unui header.
int count_apparitions(TreeNode* root);
TreeNode* node_with_max_apparitions(TTree* tree);
int get_node_level(TreeNode* node);
void repair_parents_children(TTree* tree);



/* Creeaza un arbore cu o serie de metode asociate
 *
 * return: arborele creat
 */
TTree* createTree(void* (*createElement)(void*),
				  void (*destroyElement)(void*),
				  void* (*createInfo)(void*),
				  void (*destroyInfo)(void*),
				  int compare(void*, void*)) {
	
	TTree* new_tree = calloc(1, sizeof(TTree));
	new_tree->compare = compare;
	new_tree->createElement=createElement;
	new_tree->destroyElement=destroyElement;
	new_tree->createInfo=createInfo;
	new_tree->destroyInfo=destroyInfo;
	new_tree->root = NULL;
	new_tree->size = 0;

	return new_tree;
}


/* Verifica daca un arbore este gol (vid)
 * 		1 - daca arborele este gol
 * 		0 - in caz contrar
 */
int isEmpty(TTree* tree) {
	if(tree->root == NULL)
		return 1;
	return 0;
}


/* Cauta un anumit element in interiorul unui arbore
 *
 * tree: structura cu metodele asociate arborelui
 *   !folosit pentru a putea compara elementul curent cu cel cautat
 *
 * x: radacina arborelui curent (in care se face cautarea)
 * elem: elementul ce trebuie cautat
 */
TreeNode* search(TTree* tree, TreeNode* x, void* elem) {
	TreeNode* found_node = NULL;
	TreeNode* iterator_node = x;
	//We go through the tree and stop once we find the desired value or once we hit a dead-end, then return either.
	while(iterator_node != NULL){
		if(tree->compare(iterator_node->elem, elem) == 0){
			found_node = iterator_node;
			break;
		}
		else if(tree->compare(iterator_node->elem, elem) < 0){
			iterator_node=iterator_node->right;
		}
		else{
			iterator_node=iterator_node->left;
		}
	}
	return found_node;
}


/* Gaseste nodul cu elementul minim dintr-un arbore
 * avand radacina in x
 */
TreeNode* minimum(TreeNode* x) {
	if(x == NULL)
		return x;
	
	//We go the furthest left we can.
	TreeNode* minimum = x;
	while(minimum->left!=NULL){
		minimum = minimum->left;
	}
	return minimum;
}

/* Gaseste nodul cu elementul maxim dintr-un arbore
 * avand radacina in x
 */
TreeNode* maximum(TreeNode* x) {
	//We go the furthest right we can.
	TreeNode* maximum = x;
	while(maximum->right!=NULL){
		maximum = maximum->right;
	}
	return maximum;
}


/* Functie pentru gasirea succesorului unui nod
 * (succesorul in inordine)
 */
TreeNode* successor(TreeNode* x) {
	TreeNode* child = x;
	if(child == NULL){
		return NULL;
	}
	//If it has a right-child, return the minimum of the right subtree.
	if(child->right != NULL){
		return minimum(child->right);
	}
	//Otherwise, go up until you went right.
	while(child!= NULL)
	{
		if(child == NULL)
			return NULL;

		if(child->parent == NULL)
			return NULL;

		if(child->parent->left == child)
			return child->parent;

		child=child->parent;

	}
	return NULL;
}


/* Functie pentru gasirea predecesorului unui nod
 * (predecesorul in inordine)
 */
TreeNode* predecessor(TreeNode* x) {
	TreeNode* child = x;

	if(child == NULL){
		return NULL;
	}
	//If it has a left-child, return the maximum of the left subtree.

	if(child->left != NULL){
		return maximum(child->left);
	}
	//Otherwise, go up until you went left.
	while(child!= NULL)
	{
		if(child == NULL)
			return NULL;

		if(child->parent == NULL)
			return NULL;

		if(child->parent->right == child)
			return child->parent;

		child=child->parent;

	}
	return NULL;
}


/* Actualizeaza inaltimea unui nod din arbore
 */
void updateHeight(TreeNode* x) {

	int leftHeight = 0;
	int rightHeight = 0;
	//This updates the height of a node according to the heights of its subtrees.
	if (x != NULL) {
		if (x->left != NULL)  leftHeight  = x->left->height;
		if (x->right != NULL) rightHeight = x->right->height;
		x->height = MAX(leftHeight, rightHeight) + 1;
	}
}


/* Functie ce primeste adresa unui arbore si
 * a unui nod x si realizeaza o rotatie la stanga
 * a subarborelui care are varful in x
 *
 *   (x)	 Rotatie    (y)
 *	 / \     stanga     / \
 *  a  (y)    ---->   (x)  c
 *	   / \			  / \
 *    b   c          a   b
 */
void avlRotateLeft(TTree* tree, TreeNode* x) {
	if (tree == NULL || x == NULL) return;
	TreeNode* y = x->right;
	TreeNode* a = x->left;
	TreeNode* b = y->left;
	TreeNode* g = y->right;
	if(x->parent == NULL){
		tree->root = y;
	}
	else if(x->parent != NULL){
		if( (tree->compare)(x->elem, x->parent->elem) < 0){
			x->parent->left = y;
		}
		else{
			x->parent->right = y;
		}
	}
	//Acum, schimbam legaturile care pleaca din X si din Y.
	//Intai parintii:
	y->parent = x->parent;
	x->parent = y;
	//Acum copiii:
	x->left = a;
	x->right = b;
	y->left = x;
	y->right = g;
	
	//Acum, schimbam parintii copiilor
	if(a != NULL){
		a->parent = x;
	}
	if(b != NULL){
		b->parent = x;
	}
	if(g != NULL){
		g->parent = y;
	}
	//Acum, calculam inaltimi si le updatam.

	if(a != NULL){
		updateHeight(a);
	}
	if(b != NULL){
		updateHeight(b);
	}
	if(g != NULL){
		updateHeight(g);
	}
	updateHeight(x);
	updateHeight(y);
	//Le updatam pana in varf.
	TreeNode* parent = y->parent;
	while(parent != NULL){
		updateHeight(parent);
		parent = parent->parent;
	}
}


/* Functie ce primeste adresa unui arbore si
 * a unui nod x si realizeaza o rotatie la dreapta
 * a subarborelui care are varful in y
 *
 *     (y)	 Rotatie    (x)
 *	   / \   dreapta    / \
 *   (x)  c   ---->    a  (y)
 *	 / \			      / \
 *  a   b                b   c
 */
void avlRotateRight(TTree* tree, TreeNode* y) {
	if (tree == NULL || y == NULL) return;
	TreeNode* x = y->left;
	TreeNode* a = x->left;
	TreeNode* b = x->right;
	TreeNode* g = y->right;
	//Schimbam copilul parintelui lui y, ca sa scapam de-o grija.
	if(y->parent == NULL){
		tree->root = x;	}
	else if(y->parent != NULL){
		if((tree->compare)(y->elem, y->parent->elem) < 0){
			y->parent->left = x;
		}
		else{
			y->parent->right = x;
		}
	}
	//Acum, schimbam legaturile care pleaca din X si din Y.
	//Intai parintii:
	x->parent = y->parent;
	y->parent = x;
	//Acum copiii:
	x->left = a;
	x->right = y;
	y->left = b;
	y->right = g;
	
	//Acum, schimbam parintii copiilor
	//Acum, calculam inaltimi si le updatam.
	if(a){
		a->parent = x;
		updateHeight(a);
	}
	if(b){
		b->parent = y;
		updateHeight(b);
	}
	if(g){
		g->parent = y;
		updateHeight(g);
	}
	updateHeight(y);
	updateHeight(x);
	TreeNode* parent = x->parent;
	while(parent!=NULL){
		updateHeight(parent);
		parent = parent->parent;
	}
}


/* Calculeaza factorul de echilibrare pentru un nod x
 * (AVL balance factor)
*/
int avlGetBalance(TreeNode *x) {
	int l = 0;
	int r = 0;
	if(x->left)
		l = x->left->height;
	if(x->right)
		r = x->right->height;

	return l-r;
}


/* Functie pentru reechilibrarea unui arbore AVL
 * in urma unei inserari prin rotatii simple sau duble
 * si recalcularea inaltimii fiecarui nod intalnit parcurgand
 * arborele de jos in sus, spre radacina
 *
 */
void avlFixUp(TTree* tree, TreeNode* y) {
	if(y == NULL) return;
	TreeNode* nod_parcurgere = y->parent;
	TreeNode* nod_copil_nod_parcurgere = y;
	while( nod_parcurgere != NULL && (nod_parcurgere->parent != NULL)){
		//Vedem daca ne aflam pe oricare din cazuri:		
		//Updatam si inaltimile pe masura ce urcam.
		updateHeight(nod_parcurgere);

		int parent_balance = avlGetBalance(nod_parcurgere->parent);
		if(abs(parent_balance) > 1){
			//Hopa, ne aflam intr-un caz de dezechilibru. Hai sa vedem pe care caz, adica pe unde am urcat sa ajungem la cazul de dezechilibru.
			int mutare_parinte = 0; //0 stanga, 1 dreapta
			int mutare_pivot = 0;

			if(nod_parcurgere->parent->right == nod_parcurgere){
				mutare_parinte = 1;
			}
			if(nod_parcurgere->right == nod_copil_nod_parcurgere){
				mutare_pivot = 1;
			}
			if(mutare_parinte && mutare_pivot){ //Daca ma aflu pe cazul RR:
				avlRotateLeft(tree, nod_parcurgere->parent);
				break;
			}
			else if(!mutare_parinte && !mutare_pivot){ //Daca ma aflu pe cazul LL:			
				avlRotateRight(tree, nod_parcurgere->parent);
				break;
			}
			else if(mutare_parinte && !mutare_pivot){ //Daca ma aflu pe cazul RL:
				TreeNode* fost_parinte = nod_parcurgere->parent;
				avlRotateRight(tree, nod_parcurgere);
				avlRotateLeft(tree, fost_parinte);
				break;
			}
			else if(!mutare_parinte && mutare_pivot){ //Daca ma aflu pe cazul LR:
				TreeNode* fost_parinte = nod_parcurgere->parent;
				avlRotateLeft(tree, nod_parcurgere);
				avlRotateRight(tree, fost_parinte);
				break;
			}
		}

		nod_copil_nod_parcurgere = nod_parcurgere;
		nod_parcurgere = nod_parcurgere->parent;
	}
	if(nod_parcurgere != NULL){
		updateHeight(nod_parcurgere);
	}
}


/* Functie pentru crearea unui nod
 *
 * value: valoarea/cheia din cadrul arborelui
 * info: informatia/valoarea din dictionar
 */
TreeNode* createTreeNode(TTree *tree, void* value, void* info) {

	if (tree == NULL)
		return NULL;

	// Alocarea memoriei
	TreeNode* node = (TreeNode*) malloc(sizeof(TreeNode));

	// Setarea elementului si a informatiei
	node->elem = tree->createElement(value);
	node->info = tree->createInfo(info);

	// Initializarea legaturilor din cadrul arborelui
	node->parent = node->right = node->left = NULL;

	// Initializarea legaturilor din cadrul listei dublu inlantuite
	node->next = node->prev = node->end = NULL;


	// Inaltimea unui nod nou este 1
	// Inaltimea lui NULL este 0
	node->height = 1;

	return node;
}
void avlFixSuccessor(TreeNode* root){
	if(root == NULL){
		return;
	}
	//printf("Now fixing: %ld\n", *(long*)(root->elem));
	//First, the predecessor.
	root->prev = predecessor(root);
	root->end = root;
	if(root->prev != NULL){
		while(root->prev->next != NULL && root->prev->next->elem == root->prev->elem){
			root->prev = root->prev->next;
		}
	}
	if(root->next == NULL){
		root->next = successor(root);
	}
	else if(root->next != successor(root)){
		//printf("Fixing!\n");
		TreeNode* iterator = root;
		TreeNode* successor_node = successor(root);
		//if(successor_node != NULL)
		//printf("Its successor: %ld\n", *(long*)(successor_node->elem));
		while(iterator->next != NULL && iterator->next->elem == iterator->elem){
			iterator = iterator->next;
		}
		root->end = iterator;
		iterator->next = successor_node;
	}
	avlFixSuccessor(root->left);
	avlFixSuccessor(root->right);
	//Now, the successor.
}

/* Inserarea unul nou nod in cadrul multi-dictionarului
 * ! In urma adaugarii arborele trebuie sa fie echilibrat
 *
 */
void insert(TTree* tree, void* elem, void* info) {
		TreeNode* parent = NULL;
		TreeNode* node = tree->root;
		if(node == NULL){ //If this is the first node we insert:
			TreeNode* new_node= createTreeNode(tree, elem, info);
			new_node->end = new_node;
			(tree->size)++;
			tree->root = new_node;
			return;
		}
		
		int stanga_dreapta = 0; //This is used so we know whether we went left or right while going through the tree.
		while(node!=NULL){
			if(tree->compare(elem, node->elem) < 0){
				parent = node;
				node = node->left;
				stanga_dreapta = 0;
			}
			else if(tree->compare(elem, node->elem) == 0){ //Found an identical node!
				TreeNode* new_node = calloc(1, sizeof(TreeNode));
				memcpy(new_node, node, sizeof(TreeNode));
				new_node->info = tree->createInfo(info);
				//Now, let's go to the list and see where the last node in the list is. Also, update the end variable of all the nodes.
				TreeNode* iterator_node = node;
				iterator_node->end = new_node;
				while(iterator_node->next != NULL && iterator_node->next->elem == iterator_node->elem){
					iterator_node = iterator_node->next;
					iterator_node->end = new_node;
				}
				iterator_node->next = new_node;
				new_node->prev = iterator_node;
				new_node->next = successor(node);
				(tree->size)++;
				avlFixSuccessor(tree->root); //This fixes the list structure (prev, next pointers).
				repair_parents_children(tree); //This makes sure the duplicates have the same pointers to parents and children as the main node.
				return;
			}
			else{
				parent = node;
				node = node->right;
				stanga_dreapta = 1;
			}
		}
		//Acum, parinte e nodul de care trebuie sa leg noul nod.
		TreeNode* new_node= createTreeNode(tree, elem, info);
		new_node->next = NULL;
		new_node->end = new_node;
		new_node->prev = NULL;
		if(!stanga_dreapta){
			parent->left = new_node;
		}
		else{
			parent->right = new_node;
		}
		updateHeight(parent);
		new_node->parent = parent;
		//Acum, ar fi cazul sa urc in arbore si sa updatez inaltimile.
		TreeNode* nod_parcurgere = parent;
		while(nod_parcurgere != NULL){
			updateHeight(nod_parcurgere);
			nod_parcurgere = nod_parcurgere->parent;
		}
		//Acum, avem un nou nod. Extraordinar. Acum, trebuie sa urc in arbore si sa verific daca nu e echilibru undeva.
		avlFixUp(tree, new_node);
		avlFixSuccessor(tree->root);
		repair_parents_children(tree);
		(tree->size)++;
}


/* Eliminarea unui nod dintr-un arbore
 *
 * ! tree trebuie folosit pentru eliberarea
 *   campurilor `elem` si `info`
 * */
void destroyTreeNode(TTree *tree, TreeNode* node){

	// Verificarea argumentelor functiei
	if(tree == NULL || node == NULL) return;

	// Folosirea metodelor arborelui
	// pentru de-alocarea campurilor nodului
	tree->destroyElement(node->elem);
	tree->destroyInfo(node->info);

	// Eliberarea memoriei nodului
	free(node);
}


/* Eliminarea unui nod din arbore
 *
 * elem: cheia nodului ce trebuie sters
 * 	! In cazul in care exista chei duplicate
 *	  se va sterge ultimul nod din lista de duplicate
 */
void avlFixUpDelete(TTree* tree, TreeNode* y) {
	if(y == NULL) return;
	updateHeight(y);
	TreeNode* nod_parcurgere = y;
	while(nod_parcurgere != NULL){
		//Vedem daca ne aflam pe oricare din cazuri:
		int changed = 0;
		//Updatam si inaltimile pe masura ce urcam.
		updateHeight(nod_parcurgere);
		//printf("Nodul de parcurgere e acum la: %ld \n", *(nod_parcurgere->elem));

		//Verificam daca parintele se afla in dezechilibru:
		int balance = avlGetBalance(nod_parcurgere);
		if(abs(balance) > 1 && nod_parcurgere->height > 2){
			//Hopa, ne aflam intr-un caz de dezechilibru. Hai sa vedem pe care caz, adica pe unde am urcat sa ajungem la cazul de dezechilibru.
			int mutare_parinte = 0; //0 stanga, 1 dreapta
			int mutare_pivot = 0;
			TreeNode* z = nod_parcurgere;
			TreeNode* y = NULL;
			if(z->left && z->right){
				if(z->left->height < z->right->height){
					y = z->right;
					mutare_parinte = 1;
				}
				else{
					y = z->left;
					mutare_parinte = 0;
				}
			}
			else if(z->right){
				y = z->right;
				mutare_parinte = 1;
			}
			else{
				y = z->left;
				mutare_parinte = 0;
			}
			if(y->left && y->right){
				if(y->left->height < y->right->height){
					mutare_pivot = 1;
				}
				else{
					mutare_pivot = 0;
				}
			}
			else if(y->right){
				mutare_pivot = 1;
			}
			else{
				mutare_pivot = 0;
			}
			if(mutare_parinte && mutare_pivot){ //Daca ma aflu pe cazul RR:
				avlRotateLeft(tree, z);
			}
			else if(!mutare_parinte && !mutare_pivot){ //Daca ma aflu pe cazul LL:			
				avlRotateRight(tree, z);			}
			else if(mutare_parinte && !mutare_pivot){ //Daca ma aflu pe cazul RL:
				avlRotateRight(tree, y);
				avlRotateLeft(tree, z);
			}
			else if(!mutare_parinte && mutare_pivot){ //Daca ma aflu pe cazul LR:
				avlRotateLeft(tree, y);
				avlRotateRight(tree, z);
			}
			changed = 1;
		}
		if(!changed)
			nod_parcurgere = nod_parcurgere->parent;
	}
	if(nod_parcurgere != NULL){
		updateHeight(nod_parcurgere);
	}
}

/*
Auxiliara care taie legaturile intre nodul care trebuie sters si tine minte adresa
acestuia intr-un vector extern, dar si de unde trebuie sa plecam pentru a repara
structura de AVL.
*/
TreeNode* delete_aux(TTree* tree,TreeNode* root, void* value, TreeNode** up_fix,
							 int* current_size, int* delete_size, TreeNode*** nodes_to_be_deleted) {
	if(root == NULL)
		return NULL;

	if(tree->compare(root->elem,value) > 0){
		root->left = delete_aux(tree, root->left, value, up_fix, current_size, delete_size, nodes_to_be_deleted);
		if(*up_fix == NULL)
			*up_fix = root;
	}
	else if(tree->compare(root->elem,value) < 0){
		root->right = delete_aux(tree, root->right, value, up_fix, current_size, delete_size, nodes_to_be_deleted);
		if(*up_fix == NULL)
			*up_fix = root;
	}
	else{
		if(root->next != NULL && root->next->elem == root->elem){
			TreeNode* iterator_node = root->next;
			TreeNode* last_node = root;
			while(iterator_node->next != NULL && iterator_node->next->elem == iterator_node->elem){
				last_node = iterator_node;
				iterator_node = iterator_node->next;
			}
			tree->destroyInfo(iterator_node->info);
			free(iterator_node);
			
			last_node->next = NULL;
		}
		else if(root->left != NULL && root->right != NULL){
			TreeNode* tmp = minimum(root->right);
			if(root->info != 0){
				tree->destroyElement(root->elem);
				tree->destroyInfo(root->info);
			}
			root->elem = tmp->elem;
			root->info = tmp->info;
			root->next = tmp->next;
			tmp->next = NULL;
			tmp->info = 0; //Deoarece am "furat" pointer-ul elem si info de la nodul tmp, il marcam
			//folosind acest pointer "0" ca trebuind sa fie eliberata doar strucura de nod, nu si
			//elem si info ale lui, caci ne trebuie pentru nodul acesta.
			root->right = delete_aux(tree, root->right, tmp->elem, up_fix, current_size, delete_size, nodes_to_be_deleted);
			if(*up_fix == NULL)
				*up_fix = root;
		}
		else{
			TreeNode* tmp = root;
			if(root->left!= NULL){
				root = root->left;
				root->parent = tmp->parent;
			}
			else if(root->right!= NULL){
				root=root->right;
				root->parent = tmp->parent;
			}
			else{
				root = NULL;
			}
			if(tmp->info == 0){ //Daca e marcat, elibereaza doar structura de nod.
				free(tmp);
			}
			else{
				if(*current_size == *delete_size){ //Altfel, trebuie sters tot. Adauga-l in vectorul de stergeri!
					*delete_size = (*delete_size) * 2;
					(*nodes_to_be_deleted) = realloc((*nodes_to_be_deleted), *delete_size * sizeof(TreeNode*));
				}
				(*nodes_to_be_deleted)[*current_size] = tmp;
				(*current_size)++;
			}
		}
	}
	return root;
}

void delete(TTree* tree, void* elem) {
	TreeNode* up_fix = NULL;
	int delete_size = 10;
	int delete_current = 0;
	//Cream un vector de noduri care trebuie sterse,pentru a nu avea erori de accesari pana reparam structura de arbore.
	TreeNode** nodes_to_be_deleted = calloc(delete_size, sizeof(TreeNode*));
	//Rup legaturile catre nodul ce trebuie sters.
	tree->root = delete_aux(tree, tree->root, elem, &up_fix, &delete_current, &delete_size, &nodes_to_be_deleted);
	(tree->size)--;
	//Repar structura de AVL.
	if(up_fix != NULL)
		avlFixUpDelete(tree, up_fix);
	else
		avlFixUpDelete(tree, tree->root);
	//Repar pointerii prev, next pentru fiecare nod.
	avlFixSuccessor(tree->root);

	//Sterg din memorie nodurile marcate.
	for(int i = 0; i<delete_current; i++){
		destroyTreeNode(tree, nodes_to_be_deleted[i]);
	}
	free(nodes_to_be_deleted);
	//Ma asigur ca fiecare nod duplicat are aceiasi parents si copii ca nodul duplicat principal.
	repair_parents_children(tree);
}

void destroyTreeAux(TTree* tree, TreeNode* root){
	if(root == NULL)
		return;

	TreeNode* start = minimum(root);
	TreeNode* iterator = start;
	TreeNode* next = iterator->next;
	int destroy_elem = 1;
	while(iterator != NULL){
		tree->destroyInfo(iterator->info);
		destroy_elem = 1;
		if(next){
			if(next->elem == iterator->elem){
				destroy_elem = 0;
			}
		}
		if(destroy_elem)
			tree->destroyElement(iterator->elem);
		free(iterator);
		iterator = next;
		
		if(next){
			next = next->next;
		}
	}
}

/* Eliberarea memoriei unui arbore
 */
void destroyTree(TTree* tree){

	/* Se poate folosi lista dublu intalntuita
	 * pentru eliberarea memoriei
	 */
	if (tree == NULL)
		return;
	destroyTreeAux(tree, tree->root);
	free(tree);	
}

int count_apparitions(TreeNode* root){
	//Intorc cate duplicate sunt.
	int count = 1;
	TreeNode* iterator = root;
	while(iterator->next && iterator->next->elem == iterator->elem){
		count++;
		iterator = iterator->next;
	}
	return count;
}

TreeNode* node_with_max_apparitions(TTree* tree){
	//Intorc nodul cu numarul maxim de duplicate.
	if(tree == NULL || tree->root == NULL)
		return NULL;
	TreeNode* maximum_apparitions = minimum(tree->root);
	int max_apparitions = count_apparitions(maximum_apparitions);
	TreeNode* iterator = maximum_apparitions;
	while(iterator != NULL){
		if(count_apparitions(iterator) > max_apparitions){
			maximum_apparitions = iterator;
			max_apparitions = count_apparitions(iterator);
		}
		iterator = iterator->next;
	}
	return maximum_apparitions;
}

int get_node_level(TreeNode* node){
	//Intorc adancimea.
	int level = 0;
	while(node->parent != NULL){
		level++;
		node = node->parent;
	}
	return level;
}

void repair_parents_children(TTree* tree){

	if(!tree)
		return;
	//Repar legaturile de parinti-copii ale duplicatelor.
	TreeNode* last = minimum(tree->root);
	if(!last)
		return;
	TreeNode* iterator = last->next;
	while(iterator){
		if(iterator->elem == last->elem){
			iterator->left = last->left;
			iterator->right = last->right;
			iterator->parent = last->parent;
		}
		last=iterator;
		iterator = iterator->next;
	}

}