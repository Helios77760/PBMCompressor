#include <stdlib.h>
#include <stdio.h>
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

/*void vector_init(vector* v)
{
	v->items = malloc(VECTORBASECAPACITY * sizeof(void*));
	v->length = 0;
	v->capacity = VECTORBASECAPACITY;
}
void vector_push(vector* v, void* e)
{
	if(v->capacity == v->length)
	{
		vector_resize(v, v->capacity*2);
	}
	v->items[v->length++] = e;
}
void vector_pop(vector* v, char toFree)
{
	if(toFree)
	{
		free(v->items[v->length-1]);
	}
	v->items[--(v->length)] = NULL;
}
void* vector_get(vector *v, unsigned int pos)
{
	if(v->length > pos)
	{
		return v->items[pos];
	}
	return NULL;
}
void vector_destroy(vector* v)
{
	free(v->items);
}
void vector_set(vector* v, unsigned int pos, void* val)
{
	if(pos < v->length)
	{
		v->items[pos] = val;
	}
}

void vector_resize(vector* v, unsigned int size)
{
	void** items = realloc(v->items, size);
	if(items)
	{
		v->items = items;
		v->capacity = size;
	}
}

void** vector_toarray(vector* v)
{
	void** arr = malloc(v->length * sizeof(void*));
	memcpy(arr, v->items, v->length * sizeof(void*));
	return arr;
}

void vector_concat(vector* v, /*void***//*point** arr, unsigned int arrayLength)
{
	if(arr)
	{
		if(v->capacity < v->length + arrayLength)
		{
			vector_resize(v, v->length+(arrayLength*2));
		}
		memcpy((v->items)+(v->length), arr, arrayLength*sizeof(void*));
		v->length += arrayLength;
	}
}*/

point* new_point(unsigned int x, unsigned int y)
{
	point* p = malloc(sizeof(point));
	p->x = x;
	p->y = y;
	p->next = NULL;
	return p;
}