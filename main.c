#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// unfreeze to avoid warnings:
// #define SMALL

#ifdef SMALL
#include "small.h"
#elif defined MEDIUM
#include "medium.h"
#elif defined LARGE
#include "large.h"
#endif

// graph functions declaration
void addCity();
int removeCity();
void resetVisited();
int routeSearch(struct RoadMap **first, struct RoadMap **last, int source_id, int dest_id);
void addToRoadMap(struct RoadMap **first, struct RoadMap **last, int city_id, int total_cost);
void printRoadMap(struct RoadMap *first, struct RoadMap *last);
void deleteAllRoadMap(struct RoadMap **first, struct RoadMap **last);

// tree functions and structures declaration
struct QueueNode // queue to assist the BFS algorithm
{
	struct FamilyTreeNode *data;
	struct QueueNode *next;
};

struct CityIdNode // queue to keep track of the order of destination cities that must be visited
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

struct RoadMap *first_total = NULL;
struct RoadMap *last_total = NULL;

int final_cost = 0;

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
	// remove the first element of a city queue used in main() to create partial Roadmap
	int id = -1;

	if (first_city == NULL)
	{
		// returns -1 if queue empty
		return id;
	}

	// otherwise it removes the first city and returns its id
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

	// if there is a direct connection, it's chosen
	if (adjacency_matrix[source_id][dest_id] != 0)
	{
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
		if (cost > 0 && visited[i] == 0 && (min_cost == -1 || cost < min_cost)) // there is a connection & it's not already visited & its cost is the lowest
		{
			min_cost = cost;
			min_cost_id = i;
		}
	}

	if (min_cost == -1)
	{
		printf("Cities %s and %s can't be connected using this heuristic.\n", citiesInfo[source_id].city_name, citiesInfo[dest_id].city_name);
		return total_cost;
	}

	total_cost += min_cost;
	addToRoadMap(first, last, min_cost_id, total_cost); // add to the roadmap the chosen city

	return routeSearch(first, last, min_cost_id, dest_id);
}

// Data structure functions
void addToRoadMap(struct RoadMap **first, struct RoadMap **last, int city_id, int total_cost)
{
	// add a new city to both the given queue and the total queue
	struct RoadMap *link = malloc(sizeof(struct RoadMap)); // both partial and total queue would share this node
	link->city_id = city_id;
	link->total_cost = total_cost;
	link->next = NULL;
	visited[city_id] = 1;

	if (first_total == NULL)
	{
		// the first city of all is connected to the total queue
		first_total = last_total = link;
	}

	if (*first == NULL)
	{
		// the first city (of all or just of a trip) is connected to the partial queue
		*first = *last = link;
		return; // by exiting the function here, we avoid connecting the first city of a trip to the total queue, so that it's not repeated with the last city of the previous trip
	}

	// for any other city that is not the first one, connect it to both queues
	(*last)->next = link;
	last_total->next = link;
	*last = link;
	last_total = link;
}

void printRoadMap(struct RoadMap *first, struct RoadMap *last)
{
	// will print the current status of the stack
	struct RoadMap *current = first;

	while (current != NULL)
	{
		printf("%s", citiesInfo[current->city_id].city_name);
		if (current != last)
		{
			printf("-");
		}
		current = current->next;
	}
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
	// add a new element to the assisting queue
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
	// remove the first element of the assisting queue
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
		// if the node has no more children, ends recursivity
		return;
	}

	// create all nodes for mothers' side with recursivity
	struct CivilRegistry mother_parents_city = citiesInfo[my_city.mother_parents_city_id];
	root->mother_parents = newNode(mother_parents_city.mother_name, mother_parents_city.father_name, mother_parents_city.city_id);
	createDFS(root->mother_parents);

	// create all nodes for fathers' side with recursivity
	struct CivilRegistry father_parents_city = citiesInfo[my_city.father_parents_city_id];
	root->father_parents = newNode(father_parents_city.mother_name, father_parents_city.father_name, father_parents_city.city_id);
	createDFS(root->father_parents);
}

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
	// initialize a queue to assist with finding ancestors
	struct QueueNode *queue_first = NULL;
	struct QueueNode *queue_last = NULL;
	// initialize a node to build the family tree
	struct FamilyTreeNode *currentNode;
	// initialize empty civil registry structures that will point to cities
	struct CivilRegistry my_city, mother_parents_city, father_parents_city;
	queuePush(&queue_first, &queue_last, root); // add to the queue the root parents

	while (queue_first != NULL)
	{
		currentNode = queue_first->data; // add first city of the queue in the tree
		queuePop(&queue_first, &queue_last);
		addCity(currentNode->city_id); // we save the city in another queue to later create the route map

		my_city = citiesInfo[currentNode->city_id]; // take the city information of the current node from the Civil Registry

		if (my_city.mother_parents_city_id == -1)
		{
			// if the node has no more children, skip the part of adding them to the queue
			continue;
		}

		// save node for mothers' side
		mother_parents_city = citiesInfo[my_city.mother_parents_city_id];																	  // save the id of the current node's mother's city
		currentNode->mother_parents = newNode(mother_parents_city.mother_name, mother_parents_city.father_name, mother_parents_city.city_id); // add a node for the mother's parents
		queuePush(&queue_first, &queue_last, currentNode->mother_parents);																	  // add it to the assisting queue so that it can be added as a root in a future while iteration

		// save node for fathers' side (same as mothers' side)
		father_parents_city = citiesInfo[my_city.father_parents_city_id];
		currentNode->father_parents = newNode(father_parents_city.mother_name, father_parents_city.father_name, father_parents_city.city_id);
		queuePush(&queue_first, &queue_last, currentNode->father_parents);
	}
}

void printBFS(struct FamilyTreeNode *root)
{
	// given a tree, print its nodes using BFS (the implementation is basically the same than in createBFS())
	struct QueueNode *queue_first = NULL;
	struct QueueNode *queue_last = NULL;
	struct FamilyTreeNode *currentNode;
	int level = 0, nodes_current_level = 1, nodes_next_level = 0; // variables to keep track of the level of the tree

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
			// save node for mothers' side
			queuePush(&queue_first, &queue_last, currentNode->mother_parents);
			nodes_next_level++;
		}

		if (currentNode->father_parents != NULL)
		{
			// save node for mothers' side
			queuePush(&queue_first, &queue_last, currentNode->father_parents);
			nodes_next_level++;
		}

		if (nodes_current_level == 0)
		{
			// update the level
			nodes_current_level = nodes_next_level;
			nodes_next_level = 0;
			level++;
		}
	}
}

void deleteTree(struct FamilyTreeNode *root)
{
	// remove all nodes from the tree
	if (root == NULL)
	{
		return;
	}

	deleteTree(root->mother_parents);
	deleteTree(root->father_parents);
	free(root);
}

//----------------------------------------------------------main
int main()
{
	// use -DSMALL / -DMEDIUM / -DLARGE to compile with different datasets

	resetVisited();
	printf("Ancestors' tree:\n\n");
	printf("BFS -> Names:\n");
	struct FamilyTreeNode *root = newNode("Maria", "Jordi", 0); // define the root from the parents of Barcelona
	createBFS(root);
	// here the cities' id queue is full
	printBFS(root);

	printf("\nPartial road map:\n");
	// initializes empty first and last values of RoadMap
	struct RoadMap *first = NULL;
	struct RoadMap *last = NULL;
	// city_A and city_B will be the partial source and destination of the road map
	int city_A = removeCity(), city_B;

	while (last_city != NULL) // until the queue is empty
	{
		addToRoadMap(&first, &last, city_A, 0);					  // add the first city to the roadmap
		city_B = removeCity();									  // set destination
		final_cost += routeSearch(&first, &last, city_A, city_B); // build roadmap of the trip and save its cost
		city_A = city_B;										  // set the starting point for the next trip
		printRoadMap(first, last);
		printf(" %d\n", last->total_cost);
		first = last = NULL; // reset partial roadmap
		resetVisited();
	}
	// here the cities' id queue is empty

	printf("\nTotal road map:\n");
	printRoadMap(first_total, last_total);
	printf("\n\nTotal cost: %d\n", final_cost);
	deleteAllRoadMap(&first_total, &last_total);
	deleteTree(root);
	root = NULL;

	// same steps but with DFS
	printf("\n----------------------------------\n");
	printf("DFS -> Names:\n");
	root = newNode("Maria", "Jordi", 0);
	createDFS(root);
	// here the cities' id queue is full again
	printDFS(root, 0);

	printf("\nPartial road map:\n");
	city_A = removeCity();
	final_cost = 0;

	while (last_city != NULL)
	{
		addToRoadMap(&first, &last, city_A, 0);
		city_B = removeCity();
		final_cost += routeSearch(&first, &last, city_A, city_B);
		city_A = city_B;
		printRoadMap(first, last);
		printf(" %d\n", last->total_cost);
		first = last = NULL;
		resetVisited();
	}
	// here the cities' id queue is empty again

	printf("\nTotal road map:\n");
	printRoadMap(first_total, last_total);
	printf("\n\nTotal cost: %d\n", final_cost);
	deleteAllRoadMap(&first_total, &last_total);
	deleteTree(root);
	root = NULL;

	return 0;
}
