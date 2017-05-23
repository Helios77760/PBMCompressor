#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "structures.h"

const char* errorMessage[] = {"\nPas d'erreur\n", "\nFichier invalide\n", "\nErreur d'allocation\n", "\nErreur inconnue\n", "", "", ""};

byte BinarySearch(deplacement* dep, long long int value, unsigned long long int length,  unsigned long long int* index)
{
	*index = 0;
	if(dep && length)
	{
		unsigned long long int mid=0, inf=0, sup=length-1;
		while(inf < sup)
		{
			mid = (sup+inf)/ 2;
			if(value <= dep[mid].value)
			{
				sup = mid;
			}else
			{
				inf = mid+1;
			}
		}
		*index = inf;
		return dep[inf].value == value;
	}
	return 0;
}
byte SimpleSearch(deplacement* dep, long long int value, unsigned long long int length, unsigned long long int* index)
{
	*index=0;
	if(dep && length)
	{
		unsigned long long int i=0;
		while(i < length) {
			if (value == dep[i].value) {
				*index = i;
				break;
			} else {
				++i;
			}
		}
		return i < length;
	}
	return 0;
}

int comparDep(const void* a, const void* b)
{
	if(((deplacement*)a)->value < ((deplacement*)b)->value) return -1;
	if(((deplacement*)a)->value == ((deplacement*)b)->value) return 0;
	if(((deplacement*)a)->value > ((deplacement*)b)->value) return 1;
}

int comparOcc(const void* a, const void* b)
{
	if(((deplacement*)a)->occurences < ((deplacement*)b)->occurences) return -1;
	if(((deplacement*)a)->occurences == ((deplacement*)b)->occurences) return 0;
	if(((deplacement*)a)->occurences > ((deplacement*)b)->occurences) return 1;
}

int comparDepDec(const void* a, const void* b)
{
	if(((deplacement*)a)->value > ((deplacement*)b)->value) return -1;
	if(((deplacement*)a)->value == ((deplacement*)b)->value) return 0;
	if(((deplacement*)a)->value < ((deplacement*)b)->value) return 1;
}

int comparHuffmann(const void* a, const void* b)
{
	if(((huffmann*)a)->occurences > ((huffmann*)b)->occurences) return -1;
	if(((huffmann*)a)->occurences == ((huffmann*)b)->occurences) return 0;
	if(((huffmann*)a)->occurences < ((huffmann*)b)->occurences) return 1;
}

int comparHuffmannPointers(const void* a, const void* b)
{
	if((*(huffmann**)a)->occurences > (*(huffmann**)b)->occurences) return -1;
	if((*(huffmann**)a)->occurences == (*(huffmann**)b)->occurences) return 0;
	if((*(huffmann**)a)->occurences < (*(huffmann**)b)->occurences) return 1;
}

huffmann* deplacementsToHuffmannTree(deplacement* dep, unsigned long long int nbDep)
{
	huffmann* tree = NULL;
	huffmann** nodes = malloc(sizeof(huffmann*)*nbDep);
	if(nodes)
	{
		unsigned long long int nodesSize = nbDep;
		for(unsigned long long int i=0; i < nbDep; ++i)
		{
			nodes[i]=malloc(sizeof(huffmann));
			if(nodes[i])
			{
				nodes[i]->value = dep[i].value;
				nodes[i]->occurences = dep[i].occurences;
				nodes[i]->left = NULL;
				nodes[i]->right = NULL;
			}
		}
		huffmann* min = NULL;
		huffmann* min2 = NULL;
		huffmann* temp = NULL;
		while(nodesSize>1)
		{
			qsort(nodes, nodesSize,sizeof(huffmann*), comparHuffmannPointers);
			min = nodes[nodesSize-1];
			min2 = nodes[nodesSize-2];
			temp = malloc(sizeof(huffmann));
			if(temp)
			{
				temp->right = min;
				temp->left = min2;
				temp->occurences = min->occurences + min2->occurences;
				nodes[nodesSize-2] = temp;
				--nodesSize;
			}

		}
		tree = nodes[0];
		free(nodes);
	}

	return tree;
}

void freeHuffmannTree(huffmann* tree)
{
	if(tree->left)
	{
		freeHuffmannTree(tree->left);
	}
	if(tree->right)
	{
		freeHuffmannTree(tree->right);
	}
	free(tree);
}

void exploreHuffmann(huffmann* tree, char* code, unsigned long long int* expectedLength)
{
	const unsigned long long int size = strlen(code)+2;
	char cpy[size];
	if(tree->left)
	{
		strcpy(cpy, code);
		exploreHuffmann(tree->left, strcat(cpy, "0"), expectedLength);
	}
	if(tree->right)
	{
		strcpy(cpy, code);
		exploreHuffmann(tree->right, strcat(cpy, "1"), expectedLength);
	}
	if(!(tree->right) && !(tree->left))
	{
		//printf("Val : %lld , Occ : %llu, Code : %llu\n", tree->value, tree->occurences, (unsigned long long int)strlen(code));
		*expectedLength += tree->occurences*strlen(code);
	}
}