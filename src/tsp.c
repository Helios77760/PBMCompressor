#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include "tsp.h"
#include "mersenneTwister.h"

#define abs(x) (((x)>=0) ? (x) : (-x))
#define DEPLISTINIT 20


errorStatus getOccurences(path* p)
{
	deplacement* dep = calloc(DEPLISTINIT ,sizeof(deplacement));
	unsigned long long int capacity = DEPLISTINIT;
	if(dep)
	{
		point* node = p->pointList;
		if(node->next)
		{
			dep[0].value = node->next->x - node->x;
			dep[0].occurences = 1;
			p->nbDeplacements = 1;
			long long int value = node->next->y - node->y;
			if(dep[0].value == value)
			{
				dep[0].occurences++;
			}else
			{
				if(value > dep[0].value)
				{
					dep[1].value = value;
					dep[1].occurences = 1;
				}else
				{
					dep[1].value = dep[0].value;
					dep[1].occurences = dep[0].occurences;
					dep[0].value = value;
					dep[0].occurences = 1;
				}
				p->nbDeplacements++;
			}
			node = node->next;
			if(p->nbDeplacements > 1)
				printf("%lld , %lld", dep[0].value, dep[1].value);
			else
				printf("%lld", dep[0].value);
		}
		byte found = 0;
		long long int value;
		unsigned long long int index=0;
		while(node->next)
		{
			value = node->next->x - node->x;
			found = BinarySearch(dep, value, p->nbDeplacements, &index);
			if(found)
			{
				dep[index].occurences++;
			}else
			{
				if(p->nbDeplacements == capacity)
				{

					deplacement* newList =  realloc(dep, sizeof(deplacement) * (capacity + DEPLISTINIT));
					capacity += DEPLISTINIT;
					if(!newList) {
						free(dep);
						p->nbDeplacements=0;
						return ALLOC_ERROR;
					}
					dep = newList;

				}
				if(value < dep[index].value)
				{
					memmove(dep + index + 1, dep+index, (p->nbDeplacements-index)*sizeof(deplacement));
					dep[index].value = value;
					dep[index].occurences = 1;
				}
				else
				{
					memmove(dep + index + 2, dep+index+1, (p->nbDeplacements-index-1)*sizeof(deplacement));
					dep[index+1].value = value;
					dep[index+1].occurences = 1;
				}
				/*if(index == p->nbDeplacements-1)
				{
					dep[index+1].value = value;
					dep[index+1].occurences=1;
				} else
				{
					memmove(dep + index + 1, dep+index, (p->nbDeplacements-index)*sizeof(deplacement));
					dep[index].value = value;
					dep[index].occurences = 1;
				}*/
				p->nbDeplacements++;
			}
			value = node->next->y - node->y;
			found = BinarySearch(dep, value, p->nbDeplacements, &index);
			if(found)
			{
				dep[index].occurences++;
			}else
			{
				if(p->nbDeplacements == capacity)
				{

					deplacement* newList =  realloc(dep, sizeof(deplacement) * (capacity + DEPLISTINIT));
					capacity += DEPLISTINIT;
					if(!newList) {
						free(dep);
						p->nbDeplacements=0;
						return ALLOC_ERROR;
					}
					dep = newList;

				}
				if(value < dep[index].value)
				{
					memmove(dep + index + 1, dep+index, (p->nbDeplacements-index)*sizeof(deplacement));
					dep[index].value = value;
					dep[index].occurences = 1;
				}
				else
				{
					memmove(dep + index + 2, dep+index+1, (p->nbDeplacements-index-1)*sizeof(deplacement));
					dep[index+1].value = value;
					dep[index+1].occurences = 1;
				}
				p->nbDeplacements++;
			}
			node = node->next;
		}
		deplacement* list = realloc(dep, sizeof(deplacement)*p->nbDeplacements);
		if(!list) {
			free(dep);
			p->nbDeplacements=0;
			return ALLOC_ERROR;
		}
		p->depList = list;
		return NO_ERROR;
	}

	return ALLOC_ERROR;
}

errorStatus generateBestPath(analizedFile* file, unsigned long long int height, unsigned long long int width)
{

}
unsigned long long int calculatePathLength(point* pointList, pathLengthCalcMethod method)
{
	unsigned long long int pathLength = 0;
	if(method == DISTANCE_EUCLIDIAN)
	{
		point* node = pointList;
		while(node->next)
		{
			pathLength+= abs(node->x - node->next->x);
			pathLength+= abs(node->y - node->next->y);
			node = node->next;
		}
	}else if(method == DISTANCE_REAL)
	{
		point* node = pointList;
		long double doublepath=0;
		while(node->next)
		{
			doublepath+= sqrt(abs(node->x - node->next->x) + abs(node->y - node->next->y));
			node = node->next;
		}
		pathLength = (unsigned long long int)doublepath;
	}else if(method == SPACE_OPTIMIZING)
	{

	}
	return pathLength;

}
void exchangePath(point* pointList, unsigned long long int index_a, unsigned long long int index_b)
{
	point* temp = pointList[index_a].next;
	pointList[index_a].next = pointList[index_b].next;
	pointList[index_b].next = temp;
}
unsigned long long int acceptanceProbability(unsigned long long int pathlength_a, unsigned long long int pathlength_b, long double temp)
{

}
errorStatus randomPath(analizedFile* file)
{
	unsigned long long int* indexList = malloc((file->numberOfPoints-2)* sizeof(unsigned long long int));
	if(indexList)
	{
		//On ommet le premier et le dernier car ce sont les points initiaux et finaux
		unsigned long long int limit = file->numberOfPoints-1;
		for(unsigned long long int i=1; i < limit; ++i) {
			indexList[i-1] = i; //On numérote à partir de l'index 0
		}
		unsigned long long int nextIndex;
		unsigned long long int pos;
		limit--; //Car la liste ne commence pas à l'index 1
		point* node = file->points;
		for(unsigned long long int i=0; i<limit;++i)
		{
			pos = genrand64_int64()%(limit-i);
			nextIndex = indexList[pos];
			node->next = file->points + nextIndex;
			node = node->next;
			indexList[pos]=indexList[limit-i-1];
		}
		node->next = file->points + file->numberOfPoints-1;
		free(indexList);
		file->status = PATH_GENERATED;
		return NO_ERROR;
	}else
	{
		return ALLOC_ERROR;
	}

}

errorStatus identifyFirstAndLast(analizedFile* file)
{
	if(file && file->numberOfPoints>0)
	{
		file->originIsBlack = (byte)(((file->points[0].x == 0) && (file->points[0].y == 0)) ? 1 : 0);
		file->endIsBlack = (byte)(((file->points[file->numberOfPoints-1].x == file->width-1) && (file->points[file->numberOfPoints-1].y == file->height-1)) ? 1 : 0);
		if(!(file->originIsBlack && file->endIsBlack))
		{
			unsigned long long int size = file->numberOfPoints + 2 - file->originIsBlack - file->endIsBlack;
			point* newList = realloc(file->points, size * sizeof(point));
			if(newList)
			{
				if(!(file->originIsBlack))
				{
					memmove(newList+1, newList, file->numberOfPoints * sizeof(point));
					newList[0].x = 0;
					newList[0].y = 0;
					newList[0].next = NULL;
				}
				if(!(file->endIsBlack))
				{
					newList[size-1].x = file->width-1;
					newList[size-1].y = file->height-1;
					newList[size-1].next = NULL;
				}
				file->numberOfPoints = size;
				file->points = newList;
				file->status = ENDINGS_IDENTIFIED;
				return NO_ERROR;
			}
			return ALLOC_ERROR;
		}
		file->status = ENDINGS_IDENTIFIED;
		return NO_ERROR;
	}
	return INVALID_FILE;
}

errorStatus fileToPath(analizedFile* file, pathLengthCalcMethod method, path* p)
{
	p->pointList = NULL;
	p->depList = NULL;
	p->method = method;
	p->pathLength = ULLONG_MAX;
	if(file->status >= FILE_READ) {
		p->pointList = file->points;
		if(file->status >= PATH_GENERATED)
		{
			return getOccurences(p);
		}
	}
	return NO_ERROR;
}