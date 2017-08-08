#include <stdio.h>
#include <stdlib.h>

#include "Structs.h"
#include <gtk/gtk.h>///Debug only

ListProba* new_list(unsigned long length)
{//Gestion des erreurs à faire ici.
	if(length == 0)
		return NULL;

	ListProba *l = NULL;
	l = (ListProba*)malloc(sizeof(ListProba));
	l->proba = 0;
	l->next = NULL;
	if(length <= 1)
		return l;
	else
	{
		length--;
		while(length > 0)
		{
			l->next = (ListProba*)malloc(sizeof(ListProba));
			l->next->proba = 0;
			l->next->next = NULL;
		}
		return l;
	}
}

ListProba* list_add(ListProba *l, long double proba)//Gérer les erreurs ici.
{//If the list is empty then it is created.
	ListProba *lTmp = l, *lReturn = l;

	if(lReturn == NULL)
	{
		lReturn = (ListProba*)malloc(sizeof(ListProba));
		if(lReturn == NULL)
			g_print("Impossible d'allouer de la mémoire.\n");

		lReturn->proba = proba;
		lReturn->next = NULL;
	}
	else
	{
		while(lTmp->next != NULL)
			lTmp = lTmp->next;
	
		lTmp->next = (ListProba*)malloc(sizeof(ListProba));
		if(lTmp->next == NULL)
			g_print("Impossible d'allouer de la mémoire.\n");

		lTmp->next->proba = proba;
		lTmp->next->next = NULL;
	}
	return lReturn;
}

unsigned long list_count(ListProba *l)
{
	unsigned long count = 0;
	ListProba *lTmp = l;
	while(lTmp->next != NULL)
	{
		count++;
		lTmp = lTmp->next;
	}
	return count;
}

long double mean(ListProba *l)
{
	long count = 0;
	long double m = 0;
	ListProba *lTmp = l;
	while(lTmp != NULL)
	{
		m += lTmp->proba;
		count++;
		lTmp = lTmp->next;
	}
	return m / (long double)count;
}

void list_del_last(ListProba *l)
{
	ListProba *lTmp = l;
	if(lTmp->next == NULL)
	{
		free(lTmp);
		l = NULL;
	}
	else
	{
		while(lTmp->next != NULL)
			lTmp = lTmp->next;

		free(lTmp);
	}
}

ListProba* list_del_first(ListProba *l)
{
	ListProba *lFirst = l;
	if(l != NULL)
	{
		l = l->next;
		free(lFirst);
		return l;
	}
}

void list_del_all(ListProba *l)
{//L'addresse de l pointe vers nulle part après l'appel de cette fonction !
	while(l != NULL)
		l = list_del_first(l);
}
