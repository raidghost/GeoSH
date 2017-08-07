#include <stdio.h>
#include <stdlib.h>

#include "Structs.h"

ListProba* new_list(unsigned long length)
{//Gestion des erreurs à faire ici.
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

int list_add(ListProba *l, long double proba)
{
	ListProba *lTmp = l;
	while(lTmp->next != NULL)
		lTmp = lTmp->next;
	
	lTmp->next = (ListProba*)malloc(sizeof(ListProba));
	lTmp->next->proba = proba;
	lTmp->next->next = NULL;
}

long double mean(const ListProba *l)
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
