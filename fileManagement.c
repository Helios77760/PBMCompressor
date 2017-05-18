#define _LARGEFILE_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "fileManagement.h"
#include "pointManagement.h"



analizedFile readFile(char* filepath)
{
    analizedFile result = {NO_ERROR, 0, 0,0,0,0,NULL, NOT_ANALYZED};
	FILE* file = fopen(filepath, "rb");
	if(!file)
	{
		puts("Le fichier n'existe pas ou est impossible à ouvrir\n");
        result.error= INVALID_FILE;
		return result;
	}
	byte buffer[512] = {0};
	if(fread(buffer,sizeof(byte),512,file))
	{
		puts("Fichier accédé");
		if(buffer[0] == 'P' && buffer[1] == '4')
		{
			puts("Le fichier est un pbm");
			//buffer[2] est un espace
			long offset = 3;
			unsigned long long int width = (unsigned long long int)atoll(buffer+offset);
			if(width == 0) {
				puts("Largeur invalide ou inexistante");
                result.error = INVALID_FILE;
				fclose(file);
				return result;
			}
			//ensuite vient un espace
			offset += NUMLENGTH(width)+1;

			unsigned long long int height = (unsigned long long int)atoll(buffer+offset);
			if(height == 0)
			{
				puts("Hauteur invalide ou inexistante");
                result.error = INVALID_FILE;
				fclose(file);
                return result;
			}
			//ensuite vient un espace
			offset += NUMLENGTH(height) + 1;

			printf("Largeur : %llu , Hauteur : %llu\n", width, height);

			//On commence à lire les points
			point* pointList = calloc(10, sizeof(point));
			if(!pointList)
            {
                result.error=ALLOC_ERROR;
				fclose(file);
                return result;
            }

			unsigned long long int pointListCapacity = 10;
			unsigned long long int pointListLength = 0;
			point bytePoints[8];
			unsigned long long int byteWidth = (unsigned long long int)ceil(width/8.0);
			unsigned long long int currentRow=0, currentColumn=0,arrayLength=0;
			do {
				for (; (currentRow < height) && (offset < 512); offset++) {
					arrayLength = pointsfrombyte(buffer[offset], bytePoints, currentColumn++, currentRow);
					if(pointListLength + arrayLength > pointListCapacity)
					{
						pointList = realloc(pointList, (pointListCapacity+arrayLength)*2* sizeof(point));
						pointListCapacity += pointListCapacity+2*arrayLength;
					}
					if(!pointList)
                    {
                        result.error=ALLOC_ERROR;
						fclose(file);
                        return result;
                    }
					for(int i = 0; i < arrayLength && bytePoints[i].x < width; i++)
					{
						pointList[pointListLength++] = bytePoints[i];
					}
					if (currentColumn == byteWidth) {
						currentColumn = 0;
						currentRow++;
					}
				}
				if(currentRow < height && !feof(file))
				{
					offset=0;
					fread(buffer,sizeof(byte),512,file);
				}
			}while(currentRow < height);

			point* array = malloc(pointListLength* sizeof(point));
			if(array)
			{
				for(unsigned long long int i = 0; i < pointListLength ; ++i)
				{
					array[i].x = pointList[i].x;
					array[i].y = pointList[i].y;
					array[i].next = pointList[i].next;
				}
				free(pointList);
                result.error=NO_ERROR;
                result.numberOfPoints = pointListLength;
                result.height = height;
                result.width = width;
                result.points = array;
				result.status = FILE_READ;
				fclose(file);
				return result;
			}
            result.error=ALLOC_ERROR;
			fclose(file);
            return result;

		}else
		{
			puts("Le fichier n'est pas un PBM !");
            result.error=INVALID_FILE;
			fclose(file);
            return result;
		}
	}else
	{
		fclose(file);
		puts("Erreur de lecture ou fichier vide");
        result.error=INVALID_FILE;
        return result;
	}

}
