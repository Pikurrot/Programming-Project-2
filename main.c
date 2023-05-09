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
int main(int argc, char **argv)
{
	return 0;
}
