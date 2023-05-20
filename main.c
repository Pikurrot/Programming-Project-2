// PREGUNTES CLASSSE 11/05
// 1. same city loop
// 2. SAME CITY LOOP visited vector for not repeating cities (NOT SAVE COST,
// save binary vector with position 0 or 1 if visited!) (or maybe save vector
// with positions? my idea)
// 3. #ifdef compilation include large.h
//  dijkstra algorithm is more exhaustive, but doesn't work with medium and
//  large, we have to use HEURISTIC
//--- 5/18
// 4. ifdef
// hay q descubrir la cosa especial q se define en la compilacion, cuando se define el gcc para usar el ifdef
// delivery SUNDAY 28/05
//  ----------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define SMALL // unfreeze to avoid warnings

#ifdef SMALL
#include "small.h"
#elif defined MEDIUM
#include "medium.h"
#elif defined LARGE
#include "large.h"
#endif

// graph functions
void addCity();
int removeCity();
void resetVisited();
int routeSearch(struct RoadMap **first, struct RoadMap **last, int source_id, int dest_id);
void addToRoadMap(struct RoadMap **first, struct RoadMap **last, int city_id, int total_cost);
void printRoadMap(struct RoadMap *first, struct RoadMap *last);
void deleteAllRoadMap(struct RoadMap **first, struct RoadMap **last);

// tree functions
struct QueueNode
{
	struct FamilyTreeNode *data;
	struct QueueNode *next;
};

struct CityIdNode
{
	int id;
	struct CityIdNode *next;
};

struct FamilyTreeNode *newNode(char *motherName, char *fatherName, int city_id);
void queuePush(struct QueueNode **first, struct QueueNode **last, struct FamilyTreeNode *data);
void queuePop(struct QueueNode **first, struct QueueNode **last);
void createDFS(struct FamilyTreeNode *root);
void printDFS(struct FamilyTreeNode *root, int level);
void createBFS(struct FamilyTreeNode *root);
void printBFS(struct FamilyTreeNode *root);

int visited[NUMBER_CITIES];
struct CityIdNode *first_city;
struct CityIdNode *last_city;

// --------------- Graph functions ---------------
void addCity(int data)
{
	// add a new element to the queue
	struct CityIdNode *new_city = malloc(sizeof(struct CityIdNode));
	new_city->id = data;
	new_city->next = NULL;

	if (first_city == NULL)
	{
		first_city = last_city = new_city;
		return;
	}

	last_city->next = new_city;
	last_city = new_city;
}

int removeCity()
{
	// remove the first element
	int id = -1;

	if (first_city == NULL)
	{
		return id;
	}

	if (first_city->next == NULL)
	{
		id = first_city->id;
		free(first_city);
		first_city = last_city = NULL;
		return id;
	}

	struct CityIdNode *temp = first_city;
	id = first_city->id;
	first_city = first_city->next;
	free(temp);
	return id;
}

void resetVisited()
{
	// initialize the visited array with 0
	for (int i = 0; i < NUMBER_CITIES; i++)
	{
		visited[i] = 0;
	}
}

int routeSearch(struct RoadMap **first, struct RoadMap **last, int source_id, int dest_id)
{
	// returns the total cost of the calculated route. Meanwhile, adds the cities it visits to the queue
	int i, min_cost, cost, min_cost_id, total_cost;
	int *connections;
	total_cost = (*last)->total_cost;

	// if there is a direct connection, it's chosen; else it chooses connection to city with lowest cost from current
	if (adjacency_matrix[source_id][dest_id] != 0)
	{
		// direct connection
		total_cost += adjacency_matrix[source_id][dest_id];
		addToRoadMap(first, last, dest_id, total_cost);
		return total_cost;
	}

	// search the city with minimum cost
	connections = adjacency_matrix[source_id];
	min_cost = -1;
	min_cost_id = -1;

	for (i = 0; i < NUMBER_CITIES; i++)
	{
		cost = connections[i];
		if (cost > 0 && visited[i] == 0 && (min_cost == -1 || cost < min_cost))
		{
			min_cost = cost;
			min_cost_id = i;
		}
	}
	total_cost += min_cost;
	addToRoadMap(first, last, min_cost_id, total_cost);

	return routeSearch(first, last, min_cost_id, dest_id);
}

// Data structure functions
void addToRoadMap(struct RoadMap **first, struct RoadMap **last, int city_id, int total_cost)
{
	// add a new city to the queue
	struct RoadMap *link = malloc(sizeof(struct RoadMap));
	link->city_id = city_id;
	link->total_cost = total_cost;
	link->next = NULL;
	visited[city_id] = 1;

	if (*first == NULL)
	{
		*first = *last = link;
		return;
	}

	(*last)->next = link;
	*last = link;
}

void printRoadMap(struct RoadMap *first, struct RoadMap *last)
{
	// will print the current status of the stack
	struct RoadMap *current = first;

	printf("\n");

	while (current != NULL)
	{
		printf("%s-", citiesInfo[current->city_id].city_name);
		current = current->next;
	}

	printf("\n");
}

void deleteAllRoadMap(struct RoadMap **first, struct RoadMap **last)
{
	// will delete only once all the cities from the stack (before progam finishes its execution)
	if (*first == NULL)
	{
		return;
	}

	// remove the one element
	if ((*first)->next == NULL)
	{
		free(*first);
		*first = *last = NULL;
		return;
	}

	while (*first != NULL)
	{
		struct RoadMap *temp = *first;
		*first = (*first)->next;
		free(temp);
	}
}

// --------------- Tree functions ---------------

struct FamilyTreeNode *newNode(char *motherName, char *fatherName, int city_id)
{
	struct FamilyTreeNode *link = malloc(sizeof(struct FamilyTreeNode));
	strcpy(link->motherName, motherName);
	strcpy(link->fatherName, fatherName);
	link->city_id = city_id;
	link->mother_parents = link->father_parents = NULL;
	return link;
}

void queuePush(struct QueueNode **first, struct QueueNode **last, struct FamilyTreeNode *data)
{
	// add a new element to the queue
	struct QueueNode *link = malloc(sizeof(struct QueueNode));
	link->data = data;
	link->next = NULL;

	if (*first == NULL)
	{
		*first = *last = link;
		return;
	}

	(*last)->next = link;
	*last = link;
}

void queuePop(struct QueueNode **first, struct QueueNode **last)
{
	// remove the first element
	if (*first == NULL)
	{
		return;
	}

	if ((*first)->next == NULL)
	{
		free(*first);
		*first = *last = NULL;
		return;
	}

	struct QueueNode *temp = *first;
	*first = (*first)->next;
	free(temp);
}

// DFS
void createDFS(struct FamilyTreeNode *root)
{
	// create tree using DFS
	struct CivilRegistry my_city = citiesInfo[root->city_id];
	addCity(root->city_id);

	if (my_city.mother_parents_city_id == -1)
	{
		return;
	}

	// create all nodes for mothers' side
	struct CivilRegistry mother_parents_city = citiesInfo[my_city.mother_parents_city_id];
	root->mother_parents = newNode(mother_parents_city.mother_name, mother_parents_city.father_name, mother_parents_city.city_id);
	createDFS(root->mother_parents);

	// create all nodes for fathers' side
	struct CivilRegistry father_parents_city = citiesInfo[my_city.father_parents_city_id];
	root->father_parents = newNode(father_parents_city.mother_name, father_parents_city.father_name, father_parents_city.city_id);
	createDFS(root->father_parents);
}

// BEFORE in main: printf("DFS -> Names:\n");
// int level = 0;
void printDFS(struct FamilyTreeNode *root, int level)
{
	// given a tree print its nodes using DFS

	if (root == NULL)
		return;

	for (int i = 0; i < level; i++)
	{
		printf("->");
	}

	if (level > 0)
	{
		printf(" ");
	}

	printf("%s and %s (%s)\n", root->motherName, root->fatherName, citiesInfo[root->city_id].city_name);
	printDFS(root->mother_parents, level + 1);
	printDFS(root->father_parents, level + 1);
}

// BFS
void createBFS(struct FamilyTreeNode *root)
{
	// create tree using BFS
	struct QueueNode *queue_first = NULL;
	struct QueueNode *queue_last = NULL;
	struct FamilyTreeNode *currentNode;
	struct CivilRegistry my_city, mother_parents_city, father_parents_city;
	queuePush(&queue_first, &queue_last, root);

	while (queue_first != NULL)
	{
		currentNode = queue_first->data;
		queuePop(&queue_first, &queue_last);
		addCity(currentNode->city_id);

		my_city = citiesInfo[currentNode->city_id];

		if (my_city.mother_parents_city_id == -1)
		{
			continue;
		}

		// create nodes for mothers' side
		mother_parents_city = citiesInfo[my_city.mother_parents_city_id];
		currentNode->mother_parents = newNode(mother_parents_city.mother_name, mother_parents_city.father_name, mother_parents_city.city_id);
		queuePush(&queue_first, &queue_last, currentNode->mother_parents);

		// create nodes for fathers' side
		father_parents_city = citiesInfo[my_city.father_parents_city_id];
		currentNode->father_parents = newNode(father_parents_city.mother_name, father_parents_city.father_name, father_parents_city.city_id);
		queuePush(&queue_first, &queue_last, currentNode->father_parents);
	}
}

void printBFS(struct FamilyTreeNode *root)
{
	// given a tree, print its nodes using BFS
	struct QueueNode *queue_first = NULL;
	struct QueueNode *queue_last = NULL;
	struct FamilyTreeNode *currentNode;
	int level = 0, nodes_current_level = 1, nodes_next_level = 0;

	queuePush(&queue_first, &queue_last, root);

	while (queue_first != NULL)
	{
		currentNode = queue_first->data;
		queuePop(&queue_first, &queue_last);
		nodes_current_level--;

		for (int i = 0; i < level; i++)
		{
			printf("->");
		}

		if (level > 0)
		{
			printf(" ");
		}

		printf("%s and %s (%s)\n", currentNode->motherName, currentNode->fatherName, citiesInfo[currentNode->city_id].city_name);

		if (currentNode->mother_parents != NULL)
		{
			queuePush(&queue_first, &queue_last, currentNode->mother_parents);
			nodes_next_level++;
		}

		if (currentNode->father_parents != NULL)
		{
			queuePush(&queue_first, &queue_last, currentNode->father_parents);
			nodes_next_level++;
		}

		if (nodes_current_level == 0)
		{
			nodes_current_level = nodes_next_level;
			nodes_next_level = 0;
			level++;
		}
	}
}

//----------------------------------------------------------main
int main()
{
	// use -DSMALL / -DMEDIUM / -DLARGE to compile with different datasets

	resetVisited();
	printf("Ancestors' tree:\n\n");
	printf("BFS -> Names:\n");
	struct FamilyTreeNode *root = newNode("Maria", "Jordi", 0);
	createBFS(root);
	printBFS(root);

	printf("Partial road map:\n");
	struct RoadMap *first = NULL;
	struct RoadMap *last = NULL;
	int city_A = removeCity(), city_B;

	while (last_city != NULL)
	{
		addToRoadMap(&first, &last, city_A, 0);
		city_B = removeCity();
		routeSearch(&first, &last, city_A, city_B);
		city_A = city_B;
		printRoadMap(first, last);
		printf(" %d\n", last->total_cost);
		deleteAllRoadMap(&first, &last);
		first = last = NULL;
		resetVisited();
	}

	// test DFS
	// struct FamilyTreeNode *root = newNode("Maria", "Jordi", 0);
	// createDFS(root);
	// printDFS(root, 0);

	printf("Done\n");

	return 0;
}
