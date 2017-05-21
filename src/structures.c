#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "structures.h"

const char* errorMessage[] = {"\nPas d'erreur\n", "\nFichier invalide\n", "\nErreur d'allocation\n", "", "", "", ""};

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

/*huffmann* deplacementsToHuffmannTree(deplacement* dep, unsigned long long int nbDep)
{
	huffmann** leafs = malloc(nbDep* sizeof(huffmann*));
	if(leafs)
	{
		huffmann* tree = NULL;
		huffmann** nodeList = malloc(nbDep * sizeof(huffmann*));
		unsigned long long int capacity= nbDep;
		if(nodeList){
			for(unsigned long long int i=0; i <nbDep; i++)
			{
				leafs[i]=malloc(sizeof(huffmann));
				if(leafs[i])
				{
					leafs[i]->occurences = dep[i].occurences;
					leafs[i]->value = dep[i].value;
					leafs[i]->left=NULL;
					leafs[i]->right=NULL;
				}
			}
			unsigned long long int leavesLeft = nbDep;
			unsigned long long nbrRootNodes = 0;
			qsort(leafs, nbDep,sizeof(huffmann*), comparHuffmann);
			while(leavesLeft)
			{
				if(nbrRootNodes)
				{
					if(nbrRootNodes > 1)
					{
						qsort(nodeList, nbrRootNodes, sizeof(huffmann*), comparHuffmannPointers);
					}
					huffmann* min = NULL;
					huffmann* min2 = NULL;
					if(leafs[leavesLeft-1]->occurences <= nodeList[nbrRootNodes-1]->occurences)
					{
						min = leafs[leavesLeft -1];
						if(leavesLeft > 1)
						{
							if(leafs[leavesLeft-2]->occurences <= nodeList[nbrRootNodes-1]->occurences)
							{
								min2 = leafs[leavesLeft-2];
								leavesLeft-=2;
							}else
							{
								min2 = nodeList[nbrRootNodes-1];
								leavesLeft--;
								nbrRootNodes--;
							}
						}else
						{
							min2 = nodeList[nbrRootNodes-1];
							leavesLeft--;
							nbrRootNodes--;
						}
					}else {
						min = nodeList[nbrRootNodes - 1];
						if (nbrRootNodes > 1) {
							if (leafs[leavesLeft - 1]->occurences <= nodeList[nbrRootNodes - 2]->occurences) {
								min2 = leafs[leavesLeft - 1];
								leavesLeft--;
								nbrRootNodes--;
							} else {
								min2 = nodeList[nbrRootNodes - 2];
								nbrRootNodes-=2;
							}
						} else {
							min2 = leafs[leavesLeft - 1];
							leavesLeft--;
							nbrRootNodes--;
						}
					}
					if(nbrRootNodes==capacity)
					{
						huffmann** temp = realloc(nodeList, capacity*2*sizeof(huffmann*));
						if(temp)
						{
							nodeList = temp;
							capacity*=2;
						}
					}
					huffmann* temp = malloc(sizeof(huffmann));
					if(temp)
					{
						nodeList[nbrRootNodes] = temp;
						nodeList[nbrRootNodes]->occurences = min->occurences + min2->occurences;
						nodeList[nbrRootNodes]->right = min;
						nodeList[nbrRootNodes++]->left = min2;
					}
				}else
				{
					if(leavesLeft >= 2)
					{
						if(nbrRootNodes==capacity)
						{
							huffmann** temp = realloc(nodeList, capacity*2*sizeof(huffmann*));
							if(temp)
							{
								nodeList = temp;
								capacity*=2;
							}
						}
						nodeList[nbrRootNodes] = malloc(sizeof(huffmann));
						if(nodeList[nbrRootNodes])
						{
							nodeList[nbrRootNodes]->right = leafs[leavesLeft-1];
							nodeList[nbrRootNodes]->left = leafs[leavesLeft-2];
							nodeList[nbrRootNodes++]->occurences = leafs[leavesLeft-1]->occurences + leafs[leavesLeft-2]->occurences;
							leavesLeft -=2;
						}
					}

				}
			}
			while(nbrRootNodes>1)
			{
				qsort(nodeList, nbrRootNodes,sizeof(huffmann*), comparHuffmannPointers);
				huffmann* min = nodeList[nbrRootNodes-1];
				huffmann* min2 = nodeList[nbrRootNodes-2];
				huffmann* temp = malloc(sizeof(huffmann));
				if(temp)
				{
					nodeList[nbrRootNodes-2] = temp;
					nodeList[nbrRootNodes-2]->occurences = min->occurences + min2->occurences;
					nodeList[nbrRootNodes-2]->right = min;
					nodeList[nbrRootNodes-2]->left = min2;
					nbrRootNodes--;
				}
			}
			tree = nodeList[0];
			free(nodeList);
			free(leafs);
		}else
		{
			free(leafs);
			tree = NULL;
		}
		return tree;
	}
	return NULL;
}*/

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

void exploreHuffmann(huffmann* tree, char* code)
{
	const unsigned long long int size = strlen(code)+2;
	char cpy[size];
	if(tree->left)
	{
		strcpy(cpy, code);
		exploreHuffmann(tree->left, strcat(cpy, "0"));
	}
	if(tree->right)
	{
		strcpy(cpy, code);
		exploreHuffmann(tree->right, strcat(cpy, "1"));
	}
	if(!(tree->right) && !(tree->left))
	{
		printf("Val : %lld , Occ : %llu, Code : %s\n", tree->value, tree->occurences, code);
	}
}