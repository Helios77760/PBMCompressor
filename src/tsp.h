#ifndef PBM_COMPRESSOR_TSP_C_H
#define PBM_COMPRESSOR_TSP_C_H

#include "structures.h"

errorStatus updateDeplacements(path* p, unsigned long long int index_a, unsigned long long int index_b);
errorStatus generateBestPath(analizedFile* file, path* p);
unsigned long long int calculatePathLength(path* p, pathLengthCalcMethod);
void exchangePath(point* pointList, unsigned long long int index_a, unsigned long long int index_b);
unsigned long long int acceptanceProbability(unsigned long long int pathlength, unsigned long long int newpathlength, long double temp);
errorStatus randomPath(analizedFile* file);
errorStatus identifyFirstAndLast(analizedFile* file);
errorStatus getOccurences(path* p);
errorStatus fileToPath(analizedFile* file, pathLengthCalcMethod method, path* p);
#endif /*PBM_COMPRESSOR_TSP_C_H*/
