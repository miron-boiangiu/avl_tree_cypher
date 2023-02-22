#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<ctype.h>

#include "Cipher.h"

// Trebuie sa fac asta deoarece vmchecker nu imi permite sa dau upload unui header.
int count_apparitions(TreeNode* root);
TreeNode* node_with_max_apparitions(TTree* tree);
int get_node_level(TreeNode* node);
void repair_parents_children(TTree* tree);

void remove_char(char* string, int pos){
	//Sterg un caracter din string.
	int len = strlen(string);
	for(int i = pos; i<len; i++){
		string[i] = string[i+1];
	}
}

void clean_word(char* string){
	//Sterg toate caracterele nedorite din string.
	int len = strlen(string);
	for(int i = 0; i<len; i++){
		if(!( ('a'<=string[i] && string[i]<='z') || ('A' <=string[i] && string[i] <= 'Z'))){
			remove_char(string, i);
			len--;
		}
	}
}
/* Construieste un multi-dictionar pe baza unui fisier text
 * Cheia (elem) unui nod va fi reprezentata de un cuvant din text
 * iar valoarea (info) va fi indexul de inceput al acelui cuvant
 * ignorand caracterele separatoare (",.? \n\r") - i.e. numarul
 * de caractere A-Z precedente
 *
 * E.g: THIS IS AN EXAMPLE
 *
 * 			IS (4)
 *		    / \
 * 	  (6) AN  THIS (0)
 *         \
 *	 	 EXAMPLE (8)
 * */
void buildTreeFromFile(char* fileName, TTree* tree) {

	// Verificarea argumentelor
	if(fileName == NULL || tree == NULL)
		return;

	FILE* fin = fopen(fileName, "r");
	if(!fin)
		return;
	int pos = 0;
	char* word = calloc(BUFLEN, 1);
	while(fscanf(fin, "%1023s", word)){
		//Citesc cuvantul, il "curat" si il adaug in arbore.
		if(word[strlen(word)-1] == '\n'){
			word[strlen(word)-1] = 0;
		}
		if(!strcmp(word, "")){
			break;
		}
		char* word_cleaned = strdup(word);
		clean_word(word_cleaned);
		insert(tree, word_cleaned, &pos);
		pos+=strlen(word_cleaned);
		free(word_cleaned);
		if(feof(fin)){
			break;
		}
		
	}
	fclose(fin);
	free(word);

}


/* Functie pentru afisarea unei chei de criptare
 * O cheie este reprezentata print-o serie de offset-uri
 *
 * E.g: key = [1, 2, 3, 4]
 * input text  = A    A    A    A
 *			   +1|  +2|  +3|  +4|
 * 				 V    V    V    V
 * output text = B    C    D    E
 *
 */
void printKey(char *fileName, Range *key) {

	FILE *f = fopen(fileName, "w");

	if (key == NULL) {
		fprintf(f, "No key provided!\n");
		fclose(f);
		return;
	}

	fprintf(f, "Decryption key of length %d is:\n", key->size);

	// Afiseaza fiecare offset din cheie % 26 (lungimea
	// alfabetului A-Z)
	for (int i = 0; i < key->size; i++) {
		fprintf(f, "%d ", key->index[i] % 26);

		// Rand nou pentru lizibilitate
		if ((i + 1) % 10 == 0)
			fprintf(f, "\n");
	}

	fclose(f);
}


/* Returneaza cheia obtinuta de parcurgerea in ordine
 * crescatoare a nodurilor arborelui (parcurgand si listele
 * de duplicate)
 */
Range* inorderKeyQuery(TTree* tree) {
	Range* range= calloc(1, sizeof(Range));
	TreeNode* iterator = minimum(tree->root);
	range->capacity = tree->size;
	range->size = 0;
	range->index = calloc(range->capacity, sizeof(int));

	while(iterator != NULL){
		(range->index)[range->size] = *(int*)(iterator->info) % 26;
		(range->size)++;
		iterator = iterator->next;
	}
	return range;
}



/* Functie pentru extragerea cheii formate din valorile
 * nodurilor de la nivelul ce contine cel mai frecvent cuvant
 * (in cazul in care exista mai multe cuvinte cu numar maxim
 * de aparitii atunci se va considera primul nod dintre acestea conform
 * parcurgerii in inordine a arborelui)
 */
Range* levelKeyQuery(TTree* tree){
	Range* range= calloc(1, sizeof(Range));
	TreeNode* iterator = minimum(tree->root);
	range->capacity = tree->size;
	range->size = 0;
	range->index = calloc(range->capacity, sizeof(int));
	//Vad care e nodul cu numarul maxim de aparitii si apoi adaug toate
	//nodurile care se afla pe acelasi nivel, parcurgand lista.
	TreeNode* max_apparitions = node_with_max_apparitions(tree);
	int max_level = get_node_level(max_apparitions);
	while(iterator != NULL){
		if(get_node_level(iterator) == max_level){
			(range->index)[range->size] = *(int*)(iterator->info) % 26;
			(range->size)++;
		}
		iterator = iterator->next;
	}
	range->capacity = range->size;
	range->index = realloc(range->index, range->size * sizeof(int));
	return range;
}


/* Extragerea cheii din nodurile aflate intr-un anumit
 * domeniu de valori specificat
 */
Range* rangeKeyQuery(TTree* tree, char* q, char* p) {
	Range* range= calloc(1, sizeof(Range));
	TreeNode* iterator = minimum(tree->root);
	range->capacity = tree->size;
	range->size = 0;
	range->index = calloc(range->capacity, sizeof(int));
	//Parcurg lista si adaug toate nodurile care au valoarea cuprinsa intre p si q, pretty self-explanatory.
	while(iterator != NULL){
		if(tree->compare(q, iterator->elem)<0 && tree->compare(iterator->elem, p)<0){
			(range->index)[range->size] = *(int*)(iterator->info) % 26;
			(range->size)++;
		}
		iterator = iterator->next;
	}
	range->capacity = range->size;
	range->index = realloc(range->index, range->size * sizeof(int));
	return range;
}


void encrypt(char *inputFile, char *outputFile, Range *key) {

	FILE * f_in  = fopen(inputFile,  "r");
	FILE * f_out = fopen(outputFile, "w");

	if (f_in == NULL)
		return;

	char *buff = (char*) malloc(BUFLEN+1);
	char c;

	int idx = 0;

	while (fgets(buff, BUFLEN, f_in) != NULL) {

		for (int i = 0; i < strlen(buff); i++) {
			if (buff[i] != ' ' && buff[i] != '\n' && buff[i] != '\r') {
				c = ((toupper(buff[i]) - 'A') + key->index[idx] % 26) % 26 + 'A';
				idx += 1;
			} else
			 	c = buff[i];

			fprintf(f_out, "%c", c);

			if (idx == key->size)
				idx = 0;
		}
	}

	free(buff);
	fclose(f_in);
	fclose(f_out);
}


void decrypt(char *inputFile, char *outputFile, Range *key) {
 	
	FILE * f_in  = fopen(inputFile,  "r");
	FILE * f_out = fopen(outputFile, "w");

	if (f_in == NULL)
		return;

	char *buff = (char*) malloc(BUFLEN+1);
	char c;

	int idx = 0;

	while (fgets(buff, BUFLEN, f_in) != NULL) {

		for (int i = 0; i < strlen(buff); i++) {
			if (buff[i] != ' ' && buff[i] != '\n' && buff[i] != '\r') {
				c = ((toupper(buff[i]) - 'A') - (key->index[idx] % 26) + 26) % 26 + 'A';
				idx += 1;
			} else
			 	c = buff[i];

			fprintf(f_out, "%c", c);

			if (idx == key->size)
				idx = 0;
		}
	}

	free(buff);
	fclose(f_in);
	fclose(f_out);
}