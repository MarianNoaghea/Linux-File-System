#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file.h"

/*comentariile sunt puse in ordinea: DE JOS IN SUS*/
/*si functiile au fost implementate to cam in aceeasi ordine*/

int comand(char *s)
{
	/*functie care returneaza un cod pentru comenzile*/
	/*citite de la tastatura*/
	char s1[] = "touch", s2[] = "mkdir", s3[] = "ls\n", s4[] = "pwd\n",
	s5[] = "cd", s6[] = "tree\n", s7[] = "rm", s8[] = "rmdir";
	int actiune = -1;

	if (strcmp(s, s1) == 0)
		actiune = 1;
	if (strcmp(s, s2) == 0)
		actiune = 2;
	if (strcmp(s, s3) == 0)
		actiune = 3;
	if (strcmp(s, s4) == 0)
		actiune = 4;
	if (strcmp(s, s5) == 0)
		actiune = 5;
	if (strcmp(s, s6) == 0)
		actiune = 6;
	if (strcmp(s, s7) == 0)
		actiune = 7;
	if (strcmp(s, s8) == 0)
		actiune = 8;
	return actiune;
}
void freeall(Directory **dir_urm)
{
	/*aceasta functie urma sa stearga recursiv ierarhia de directoare*/
	/*pe acelasi principiu ca si tree*/
	list_files *cap_f = (*dir_urm)->list_f;
	list_files *tmp_f;
	list_dirs *tmp_d;

	while (cap_f != NULL) {
		tmp_f = cap_f;
		cap_f = cap_f->next;
		free(tmp_f);
	}
	list_dirs *cap_d = (*dir_urm)->list_d;

	while (cap_d) {
		tmp_d = cap_d;
		cap_d = cap_d->next;
		freeall(&(tmp_d->dir));
		free(tmp_d);
	}
}
list_dirs *find_dir(Directory *director, char file_name[30])
{
	/*acest find_dir avea ca scop realizarea eliberarii complete de*/
	/*memorie, acesta are ca scop salvarea adresei nodului din lista*/
	/*la care se gaseste directorul ce urmeaza sa fie sters(daca exista)*/
	/*de aici urma ca in functia de mai sus(free all) sa se elibereze*/
	/*recursiv memoriea, din cauza unor motive momentan necunoscute daca*/
	/*se incearca accesarea acelei zone de memorie programul crapa*/
	/*si incepe sa influenteze si celelalte functii, de exemplu functia*/
	/*rmdir nu mai distinge stringurile pe care le cauta in lista de*/
	/*directoare*/
	char *copie;
	int ok = 1, n = strlen(file_name);
	list_dirs *cap = (director)->list_d, *prev;

	copie = malloc(n * sizeof(char));
	if (cap != NULL) {
		strcpy(copie, cap->dir->name);
		strcat(copie, "\n");
	}
	if (cap == NULL && strcmp(copie, file_name) != 0)
		ok = 0;
	if (ok == 1) {
		if (cap != NULL && strcmp(copie, file_name) == 0)
			return cap;
		while (cap != NULL && strcmp(copie, file_name) != 0) {
			cap = cap->next;
			if (cap != NULL) {
				strcpy(copie, cap->dir->name);
				strcat(copie, "\n");
			}
		}
}
return cap;
}
void rmdir(Directory **director, char file_name[30])
{
	/*pe acelasi principiu ca si rm functioneaza si rmdir*/
	/*doar ca pentru numele directorului a primit si "\n" la*/
	/*final pentru a se putea compara cele 2 stringuri*/
	char *copie;
	int ok = 1, n = strlen(file_name), i;
	list_dirs *cap = (*director)->list_d, *prev;

	copie = malloc(n * sizeof(char));
	if (cap != NULL) {
		strcpy(copie, cap->dir->name);
		strcat(copie, "\n");
	}
	if (cap == NULL && strcmp(copie, file_name) != 0)
		ok = 0;/*in caz ca nu se gaseste niciun director*/
		/*cu numele cautat se renunt la inaintarea in cautare*/
		/*deoarece urmatoarele "potentiale" stringuri*/
		/*de fapt nu exista si asta va face programul*/
		/*sa dea seg fault la apelarea strcpy*/
	if (ok == 1) {
		if (cap != NULL && strcmp(copie, file_name) == 0) {
			(*director)->list_d = cap->next;
			free(cap);
			return;
		}
		while (cap != NULL && strcmp(copie, file_name) != 0) {
			prev = cap;
			cap = cap->next;
			if (cap != NULL) {
				strcpy(copie, cap->dir->name);
				strcat(copie, "\n");
			}
		}

		if (cap == NULL) {
			printf("Cannot remove '");
			int n = strlen(file_name);

			for (i = 0; i < n - 1; i++)
				printf("%c", file_name[i]);
			printf("': No such directory!\n");
			return;
		}
		prev->next = cap->next;
		free(cap);
	}
}
int compara(char s1[30], char s2[30])
{
	int k = 0, n = strlen(s1), i = 0;

	for (i = 0; i <= n + 1; i++)
		if (s1[i] == s2[i])
			k++;
	if (k == n)
		return 1;
	else
		return 0;
}
void rm(Directory **director, char file_name[30])
{
	char *copie;
	int i, n = strlen(file_name);
	list_files *cap = (*director)->list_f, *prev;

	copie = malloc(n * sizeof(char));
	for (i = 0; i < n; i++)/*se face o copie ce nu*/
		if ((int) file_name[i] != 10)/*contine "\n"*/
			copie[i] = file_name[i];
	/*se parcurge lista si se vede unde se gaseste cuvantul*/
	if (cap != NULL && strcmp(cap->file->name, copie) == 0) {
		(*director)->list_f = cap->next;
		free(cap);
		return;
	}
	while (cap != NULL && strcmp(cap->file->name, copie) != 0) {
		prev = cap;
		cap = cap->next;
	}
	if (cap == NULL) {
		printf("Cannot remove '");
		int n = strlen(file_name);

		for (i = 0; i < n; i++)
			if ((int) file_name[i] != 10)
				printf("%c", file_name[i]);
		printf("': No such file!\n");
		return;
	}
	prev->next = cap->next;/*se scurtcircuiteaza lista*/
	free(cap);
}
void cd(Directory **director_curent, char director_name[30])
{
	list_dirs *nod = (*director_curent)->list_d;
	int ok = 0, i;

	if (strcmp(director_name, "..\n") == 0) {
		if ((*director_curent)->parentDir != NULL)
			(*director_curent) = (*director_curent)->parentDir;
		else
			printf("te aflii in root!\n");/*checkerul nu verifica*/
			/*acest mesaj totusi mi se pare util*/
		return;
	}
	while (nod) {
		char copie[30];

		strcpy(copie, nod->dir->name);
		strcat(copie, "\n");
		if (strcmp(copie, director_name) == 0) {
			(*director_curent) = nod->dir;
			ok = 1;/*daca s-a gasit un director cu numele cautat*/
		}
		nod = nod->next;
	}
	if (ok == 0) {
		printf("Cannot move to '");
		int n = strlen(director_name);

		for (i = 0; i < n; i++)
			if ((int) director_name[i] != 10)
				printf("%c", director_name[i]);
		printf("': No such directory!\n");
	}
}
void pwd(Directory *director_curent)
{
	char m[20][30];
	int l = 0, i;
	Directory *copie_director_curent = director_curent;

	while (copie_director_curent->parentDir != NULL) {
		strcpy(m[l++], copie_director_curent->name);
		copie_director_curent = copie_director_curent->parentDir;
	}
	if (director_curent->parentDir == NULL)
		printf("/");/*fisierul root*/
	else
		for (i = l - 1; i >= 0; i--)
			printf("/%s", m[i]);/*apoi se afiseaza*/
			/*in ordine descrescatoare*/
	printf("\n");
}
void afisare_d(list_dirs *lista)
{/*se parcurge lista si se afiseaza elementele ei*/
	list_dirs *nod = lista;

	while (nod) {
		printf("%s ", nod->dir->name);
		nod = nod->next;
	}
}
void afisare_f(list_files *lista)
{/*se parcurge si se afiseaza*/
	list_files *nod = lista;
	int i;

	while (nod) {
		int n = strlen(nod->file->name);

		for (i = 0; i < n; i++)/*se afiseaza numele fisierului*/
			if ((int) nod->file->name[i] != 10)
				printf("%c", nod->file->name[i]);
		printf(" ");
		nod = nod->next;
	}
}
void tree(Directory *director_curent, int k, int k2)
{
	if (k == 0)
		printf("%s", director_curent->name);
	k++;/*numarul de taburi de afisat pe masura*/
	/*se ne adancim in ierarhie*/
	afisare_f_tree(director_curent->list_f, k);
	afisare_d_tree(director_curent->list_d, k, k2);
	list_dirs *nod = director_curent->list_d;

	if (k2 == 0)
		printf("\n");
}
void afisare_d_tree(list_dirs *lista, int k, int k2)
{/*list_dirs *cap_d =find_dir((director_curent),matrice_cuvinte[1]);*/
		/*if(cap_d!=NULL)//aici ar fi trebuit sa se caute adresa*/
		/*freeall(&(cap_d->dir));//cuvantului cautat apoi cu functia*/
		/*recursiva sa se elibereze memoria din fiecare director*/
		/*dar din motive care inca nu imi sunt clare functia rmdir*/
		/*"crapa" daca se incearca sa se apeleze lista directorului*/
		/*curent intr-o alta functie*/
	list_dirs *nod = lista;
	int i;

	while (nod) {
		printf("\n");
		for (i = 1; i <= k; i++)
			printf("    ");
		int n = strlen(nod->dir->name);

		for (i = 0; i < n; i++)
			if ((int) nod->dir->name[i] != 10)
				printf("%c", nod->dir->name[i]);
		Directory *nou = malloc(sizeof(Directory));

		if (nod != NULL)
			nou = nod->dir;
		k2++; /*numarul de directoare afisate din dir curent*/
		tree(nou, k, k2);/*afisare recursiva*/

		nod = nod->next;
	}
}
void afisare_f_tree(list_files *lista, int k)
{
	list_files *nod = lista;
	int i;

	while (nod) {
		printf("\n");
		for (i = 1; i <= k; i++)
			printf("    ");
		int n = strlen(nod->file->name);

		for (i = 0; i < n; i++)/*excludem din afisare "\n"*/
			if ((int) nod->file->name[i] != 10)
				printf("%c", nod->file->name[i]);
		nod = nod->next;
	}
}

void touch(Directory *director_curent, char matrice_cuvinte[3][30])
{
	File *fis = malloc(sizeof(File));

	fis->size = strlen(matrice_cuvinte[2]) * 4;
	fis->name = malloc((fis->size + 1) * sizeof(char));
	strncpy(fis->name, matrice_cuvinte[1], strlen(matrice_cuvinte[1]));
	fis->data = malloc((strlen(matrice_cuvinte[2])) * sizeof(char));
	strcpy(fis->data, matrice_cuvinte[2]);
	fis->dir = director_curent;
	list_files *cap = director_curent->list_f;
	list_files *nod = malloc(sizeof(list_files));

	nod->next = NULL;
	nod->file = fis;
	if (cap == NULL) {/*daca lista e goala*/
		director_curent->list_f = nod;
		return;
	}
	if (cap->next == NULL) {/*daca are un element*/
		if (strcmp(nod->file->name, cap->file->name) > 0) {
			nod->next = NULL; /*il pun la dreapta(push back)*/
			cap->next = nod;
		} else {/*se alege ordinea de pus in lista*/
			cap->next = NULL; /*il pun la stanga(push front)*/
			nod->next = cap;
			director_curent->list_f = nod;
			return;
		}
	}

	list_files *prev;

	while (cap->next != NULL &&/*daca lista are mai mult de un element*/ 
		strcmp(nod->file->name, cap->file->name) > 0) {
		prev = cap;/*a b | d e f */
		cap = cap->next;
	}
	if (cap->next == NULL) {/*daca s-a ajuns la ultimul element*/
		if (strcmp(nod->file->name, cap->file->name) > 0) {
			cap->next = nod; /*push back*/
			nod->next = NULL;
			return;
		}
			nod->next = cap; /*push in fata lui cap*/
			cap->next = NULL;
			prev->next = nod;
			return;
	}
	if (cap == director_curent->list_f) {/*daca lista e goala*/
		nod->next = cap;
		director_curent->list_f = nod;
		return;
	}
	nod->next = cap;
	prev->next = nod;
}
void mkdir(Directory *director_curent, char matrice_cuvinte[3][30])
{
	char *copie; /*cu acelasi principiu ca si touch functioneaza*/
	int n = strlen(matrice_cuvinte[1]), i;/*si acesta*/

	copie = malloc(n * sizeof(char));
	for (i = 0; i < n; i++)
		if ((int)(matrice_cuvinte[1][i]) != 10)
			copie[i] = matrice_cuvinte[1][i];
	Directory *dir = malloc(sizeof(Directory));

	dir->parentDir = director_curent;
	dir->name = malloc(strlen(copie) * sizeof(char));
	strcpy(dir->name, copie);
	dir->list_d = NULL;
	list_dirs *cap = director_curent->list_d;
	list_dirs *nod = malloc(sizeof(list_dirs));

	nod->next = NULL;
	nod->dir = dir;
	if (cap == NULL) {/*de aici in jos se pun elementele in ordine*/
		director_curent->list_d = nod;/*alfabetica in lista*/
		return;
	}
	if (cap->next == NULL) {
		if (strcmp(nod->dir->name, cap->dir->name) > 0) {
			nod->next = NULL;/*push in fata lui cap*/
			cap->next = nod;
			return;
		}
		{
			cap->next = NULL;/*push back*/
			nod->next = cap;
			director_curent->list_d = nod;
			return;
		}
	}

	list_dirs *prev;

	while (cap->next != NULL &&
		strcmp(nod->dir->name, cap->dir->name) > 0) {
		prev = cap;
		cap = cap->next;
	}
	if (cap->next == NULL) {
		if (strcmp(nod->dir->name, cap->dir->name) > 0) {
			cap->next = nod;
			nod->next = NULL;
			return;
		}
		{
			nod->next = cap; ////
			cap->next = NULL;
			prev->next = nod;
		}
	}
	if (cap == director_curent->list_d) {
		nod->next = cap;
		director_curent->list_d = nod;
		return;
	}
	nod->next = cap;
	prev->next = nod;
}
int main(void)
{
	Directory *root = malloc(sizeof(Directory));
	Directory *director_curent, *list_d, *list_f;
	int inceput = 0, actiune = -1;
	char matrice_cuvinte[3][30], comanda[100], copie[100],
	intermediar[30], fin[] = "delete fs\n", start[] = "create fs\n", *p;
	do {
		fgets(comanda, 100, stdin);
		if (strcmp(comanda, start) == 0) {
			inceput = 1;
			root->name = malloc(sizeof(char));
			strcpy(root->name, "/");
			root->parentDir = NULL;
			director_curent = root;
		}
		strcpy(copie, comanda);
		int l = 0;

		/*aici despart cuvintele din stringul citit de la tastatura*/
		for (p = strtok(copie, " "); p; p = strtok(NULL, " "))
			strcpy(matrice_cuvinte[l++], p);
		actiune = comand(matrice_cuvinte[0]);
		if (actiune == 1)/*fiecare cuvant primeste un cod*/
			touch(director_curent, matrice_cuvinte);
		if (actiune == 2)
			mkdir(director_curent, matrice_cuvinte);
		if (actiune == 3) {
			afisare_f(director_curent->list_f);
			afisare_d(director_curent->list_d);
			printf("\n");
		}
		if (actiune == 4)
			pwd(director_curent);
		if (actiune == 5) {
			cd(&director_curent, matrice_cuvinte[1]);
			/*printf("%s",matrice_cuvinte[1]);*/
		}
		if (actiune == 6) {
			int k = 0, k2 = 0;

			tree(director_curent, k, k2);
			}
		if (actiune == 7)
			rm(&(director_curent), matrice_cuvinte[1]);
		if (actiune == 8) {
	/*list_dirs *cap_d =find_dir((director_curent),matrice_cuvinte[1]);*/
		/*if(cap_d!=NULL)//aici ar fi trebuit sa se caute adresa*/
		/*freeall(&(cap_d->dir));//cuvantului cautat apoi cu functia*/
		/*recursiva sa se elibereze memoria din fiecare director*/
		/*dar din motive care inca nu imi sunt clare functia rmdir*/
		/*"crapa" daca se incearca sa se apeleze lista directorului*/
		/*curent intr-o alta functie*/
			rmdir(&(director_curent), matrice_cuvinte[1]);
		}
	} while (strcmp(comanda, fin));
	return 0;
}
