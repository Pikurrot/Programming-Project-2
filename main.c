#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "small.h"

int RouteSearch(struct RoadMap *top, int source_id, int dest_id);
void addToRoadMap(struct RoadMap *top, int city_id, int total_cost);
void printRoadMap(struct RoadMap *top);
void deleteAllRoadMap(struct RoadMap *top);
int main(int argc, char **argv)
{
	return 0;
}
