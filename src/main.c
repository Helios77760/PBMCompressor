#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>
#include "fileManagement.h"
#include "mersenneTwister.h"
#include "tsp.h"

extern char* errorMessage[];

void handleError(errorStatus err)
{
	puts(errorMessage[err]);
}

int main(int argc, char* argv[]) {
	init_genrand64((unsigned long long int)time(NULL));
	setlocale(LC_ALL, "fr_FR");
	printf("PBM Image Compressor\n");
	char filepath[255];
	if(argc > 1)
	{
		strcpy(filepath, argv[1]);
	} else
	{
		puts("Chemin vers l'image à compresser : ");
		scanf("%s", filepath);
	}
	analizedFile file = readFile(filepath);
    if(!file.error)
    {

        printf("%lld points trouvés\n", file.numberOfPoints);
		errorStatus err = identifyFirstAndLast(&file);
		if(err == NO_ERROR)
		{
			printf("(0,0) est noir\t\t:\t%.5s\n(%llu,%llu) est noir\t\t:\t%.5s\n", file.originIsBlack ? "true":"false", file.width-1, file.height-1, file.endIsBlack ? "true" : "false");
			err = randomPath(&file);
			if(!err)
			{
				printf("Random path : \n");
				point* node = file.points;
				do
				{
					printf("(%llu,%llu)->", node->x, node->y);
					node = node->next;
				}while(node);
				path p;
				err = fileToPath(&file, SPACE_OPTIMIZING, &p);
				handleError(err);
				if(p.depList)
				{
					printf("Deplacements : %llu \n", p.nbDeplacements);
					//qsort(p.depList, p.nbDeplacements, sizeof(deplacement), comparDep);
					for(unsigned long long int i =0; i < p.nbDeplacements; i++)
					{
						printf("%lld\t\t:\t%llu\n", p.depList[i].value, p.depList[i].occurences);
					}
					huffmann* tree = deplacementsToHuffmannTree(p.depList, p.nbDeplacements);
					if(tree)
					{
						char code[1] = {'\0'};
						exploreHuffmann(tree, code);
						freeHuffmannTree(tree);
					}else
					{
						printf("No Tree\n");
					}
				}
			}else
			{
				handleError(err);
			}

		}else
		{
			handleError(err);
		}

    }else
    {
		handleError(file.error);
    }
	if(file.points)
		free(file.points);
	puts("Programme terminé, appuyez sur entrée pour quitter...");
	scanf("%*c%*c");
	return 0;
}