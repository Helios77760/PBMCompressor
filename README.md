# PBM Compressor

**PBM Compressor** est un défi que je me suis lancé visant à réaliser le projet [2016-L2J](http://www.ens.math-info.univ-paris5.fr/projets-informatiques/doku.php?id=projets:licence2:2016-2017) de l'Université Paris-Descartes.

Bien que ce projet soit à l'origine un projet de groupe, j'ai décidé de le réaliser juste par pure passion et défi.

La solution que je propose n'est surement pas la meilleure et je n'ai pas regarder la solution du groupe qui devait faire le projet.
 
## Enoncé

L'objectif est de compresser des images binaires « sparses », c'est-à-dire constituées de quelques pixels noirs éparpillés dans un fond blanc (de l'ordre de 1 à 5 % des pixels). Pour cela, on propose de calculer le plus court chemin reliant tous les pixels noirs et de coder les déplacements successifs.

Le calcul du plus court chemin est un problème bien connu en IA sous le nom de « problème du voyageur de commerce » (PVC).

Vous implémenterez deux programmes : un codeur et un décodeur. 
* Le codeur charge une image binaire ppm, crée une liste des pixels noirs ordonnés selon le plus court chemin, fait un codage entropique des déplacements et écrit un fichier avec la liste des déplacements et le dictionnaire de codage.
* Le décodeur place des noirs dans une matrice de blancs à partir de la liste des déplacements et écrit cette matrice sous forme d'un fichier ppm.

## Interprétation du sujet

Le sujet laisse peu de place à l'imagination quant à la manière de réaliser l'application. J'ai cependant décidé de spécifier quelques parties de l'énoncé :
* Le type de fichier est le PPM. PPM désigne les fichiers Portable Pixmap. Au vu de l'énoncé, il s'agit de fichiers en noir et blanc. Nous sommes donc en présence de fichier PBM. Il en existe une version ASCII et une autre binaire. On suppose ici que le format est binaire car sinon la compression serait au minimum d'environ 12.5% (Divisé par 8). Une version 2 pourra lire les fichiers PBM ASCII.
* Le problème du voyageur de commerce est utilisé pour réduire la taille du chemin parcouru. Dans le cas de la compression, cette réduction n'a pas de sens. Je propose donc d'utiliser cet algorithme avec une évaluation non pas de distance, mais d'espace.
* Le projet demande 2 programmes, le compresseur et le décompresseur. Ici, un seul programme sera réalisé mais permettra la compression ou la décompession selon les paramètres.

## Structure du fichier compressé
Le fichier compressé est composé comme suit :

+ 1 octet de contrôle
+ M entrées de dictionnaire
+ N octets de données

#### Octet de contrôle
L'octet de contrôle est composé comme suit :

<p align=center><b>XXXXYZAB</b></p>

+ XXXX est la taille en demi-octets des entrées du dictionnaire. 0000 = 1, 1111 = 16
+ Y vaut 1 si le point (0,0) est noir
+ Z vaut 1 si le point (largeur, hauteur) est noir
+ A vaut 1 si 0 est présent dans les entrées du dictionnaire
+ B n'est pas défini pour le moment

En précisant si les points d'origine et de fin sont noir, on peut déduire la taille de l'image en faisant le chemin de l'origine à la fin.
Ceci permet d'ommettre des champs potentiellement grands et on n'ajoute que très peu à la section données.

#### Dictionnaire
Les entrées du dictionnaire font XXXX+1 octets et sont ordonnées par ordre de taille croissante dans le code de Huffmann. Si le bit de contrôle A est mis à 0, la première entrée valant 0 mettra fin au dictionnaire. Sinon, ce sera la deuxième.

#### Données

Les données sont codées dans le code de Huffmann par paire indiquant les déplacements dans l'ordre.