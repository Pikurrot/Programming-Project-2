// PREGUNTES CLASSSE 11/05
// 1. same city loop
// 2. SAME CITY LOOP visited vector for not repeating cities (NOT SAVE COST,
// save binary vector with position 0 or 1 if visited!) (or maybe save vector
// with positions? my idea)
// 3. #ifdef compilation include large.h
//  dijkstra algorithm is more exhaustive, but doesn't work with medium and
//  large, we have to use HEURISTIC
//  ----------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "small.h"

int RouteSearch(struct RoadMap *top, int source_id, int dest_id);
void addToRoadMap(struct RoadMap *top, int city_id, int total_cost);
void printRoadMap(struct RoadMap *top);
void deleteAllRoadMap(struct RoadMap *top);

// A. World journey

// Path searching
// must create partial stack before calling RouteSearch() (top), add source city to the stack at the beggining
int RouteSearch(struct RoadMap *top, int source_id, int dest_id)
{
	int i, min_cost, cost, min_cost_id;
	int *connections;

	// if there is a direct connection, it's chosen; else it chooses connection to city with lowest cost from current
	if (adjacency_matrix[source_id][dest_id] != 0)
	{

		return adjacency_matrix[source_id][dest_id];
	}

	// search the city with minimum cost
	connections = adjacency_matrix[source_id];
	min_cost = connections[0];
	min_cost_id = 0;

	for (i = 1; i < NUMBER_CITIES; i++)
	{
		cost = connections[i];
		if (min_cost == 0 || (cost > 0 && cost < min_cost))
		{
			min_cost = cost;
			min_cost_id = i;
		}
	}

	addToRoadMap(top, min_cost_id, min_cost + top->total_cost);

	return RouteSearch(top, min_cost_id, dest_id);
}

void addToRoadMap(struct RoadMap *top, int city_id, int total_cost)
{
	// add a new city to the stack
	struct RoadMap *link = (struct RoadMap *)malloc(sizeof(struct RoadMap));
	link->city_id = city_id;
	link->total_cost = total_cost;

	if (top == NULL)
	{
		top = link;
		top->next = NULL;
		return;
	}

	link->next = top;
	top = link;
}

void printRoadMap(struct RoadMap *top)
{
	// will print the current status of the stack
	struct RoadMap *current = top;

	while (current != NULL)
	{
		// search for the name of the city in CivilRegistry
		// ...
		// printf("%d ", current->data);
		// current = current->next;
	}

	printf("\n");
}

void deleteAllRoadMap(struct RoadMap *top)
{
	// will delete only once all the cities from the stack (before progam finishes its execution)
	// if (top==NULL)
	// {
	//    	printf("nothing to remove");
	//    	return;
	//    }

	// // remove the one element
	//   	if (top->next==NULL)
	// {
	// 	printf("removing data %d\n", top->data);
	// 	free (top);
	//        top = NULL;
	//        printf("List is empty now");
	//        return;
	// }
	// struct Node * current = top;
	//     while (current != NULL)
	//     {
	//         free(current);
	//         top = current = current->next;
	//     }
	// 	printf("List empty");
} // IGUAL AQUEST ÑULTIM ES POT MILLORAR

int main(int argc, char **argv)
{
	int option;

	// Check that argument is correct
	if (argc != 2)
	{
		printf("Invalid number of arguments");
		return -1;
	}

	option = atoi(argv[0]);

	if (option < 1 || option > 3)
	{
		printf("Yor must enter an integer (from 1-3)");
		return -1;
	}

	// Treat every option
	switch (option)
	{
	case 1:
		// code here
		break;
	case 2:
		break;
	case 3:
		break;
	}

	return 0;
}
