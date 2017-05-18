#ifndef PBM_COMPRESSOR_STRUCTURES_H
#define PBM_COMPRESSOR_STRUCTURES_H
#include <math.h>
#define NUMLENGTH(x) (floor(log10(x))+1)

typedef struct _point
{
	unsigned long long int x;
	unsigned long long int y;
	struct _point* next;
} point;

typedef struct _deplacement
{
	long long int value;
	unsigned long long int occurences;
} deplacement;



typedef char byte;

typedef enum {NO_ERROR=0, INVALID_FILE=1, ALLOC_ERROR=2} errorStatus;

typedef enum {DISTANCE_EUCLIDIAN, DISTANCE_REAL, SPACE_OPTIMIZING} pathLengthCalcMethod;

typedef enum {NOT_ANALYZED=0, FILE_READ=1, ENDINGS_IDENTIFIED=2, PATH_GENERATED=3, PATH_OPTIMIZED=4} analysisStatus;

typedef struct _path
{
	point* pointList;
	pathLengthCalcMethod method;
	deplacement* depList;
	unsigned long long int nbDeplacements;
	unsigned long long int pathLength;
}path ;

typedef struct _analizedFile
{
    errorStatus error;
    unsigned long long int numberOfPoints;
    unsigned long long int width;
    unsigned long long int height;
	byte originIsBlack;
	byte endIsBlack;
    point* points;
	analysisStatus status;

} analizedFile;

byte BinarySearch(deplacement* dep, long long int value, unsigned long long int length,  unsigned long long int* index);
byte SimpleSearch(deplacement* dep, long long int value, unsigned long long int length, unsigned long long int* index);
int comparDep(const void* a, const void* b);


#endif /*PBM_COMPRESSOR_STRUCTURES_H*/
