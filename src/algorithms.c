#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "algorithms.h"

#define INF 1e9
#define EARTH_RADIUS 6371.0

double haversine(double lat1, double lon1, double lat2, double lon2)
{
    double dLat = (lat2 - lat1) * (M_PI / 180.0);
    double dLon = (lon2 - lon1) * (M_PI / 180.0);
    lat1 = lat1 * (M_PI / 180.0);
    lat2 = lat2 * (M_PI / 180.0);
    double a = (sin(dLat / 2) * sin(dLat / 2))+(sin(dLon / 2) * sin(dLon / 2) * cos(lat1) * cos(lat2));
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    return EARTH_RADIUS * c;
}

double* dijkstra(double **graph, int source, int numNodes)
{
    double *distance = (double*) malloc(numNodes * sizeof(double));
    int *visited_check = (int*) malloc(numNodes * sizeof(int));
    
    for (int i = 0; i < numNodes; i++)
    {
        distance[i] = INF;
        visited_check[i] = 0;
    }
    distance[source] = 0;

    for (int count = 0; count < numNodes - 1; count++)
    {
        int u=-1;
        double min = INF;
        for (int i = 0; i < numNodes; i++)
        {
            if (!visited_check[i] && distance[i] < min)
            {
                min = distance[i];
                u = i;
            }
        }
        if (u == -1) 
        break;
        visited_check[u] = 1;

        for (int v = 0; v < numNodes; v++) 
        {
            if (!visited_check[v] && graph[u][v] < INF && distance[u] + graph[u][v] < distance[v])
            {
                distance[v] = distance[u] + graph[u][v];
            }
        }
    }

    free(visited_check);
    return distance;
}

int dijkstraFindBestWarehouse(Warehouse warehouses[], int count, double productLat, double productLon) {
    int numNodes = count + 1;
    double costFactor = 10.0;

    double **graph = (double**) malloc(numNodes * sizeof(double*));
    for (int i = 0; i < numNodes; i++)
    {
        graph[i] = (double*) malloc(numNodes * sizeof(double));
    }

    for (int i = 0; i < numNodes; i++)
    {
        for (int j = 0; j < numNodes; j++)
        {
            graph[i][j] = INF;
        }
    }

    for (int i = 1; i < numNodes; i++)
    {
        double directDistance = haversine(productLat, productLon, warehouses[i - 1].latitude, warehouses[i - 1].longitude);
        graph[0][i] = directDistance + costFactor * warehouses[i - 1].costPerUnit;
        graph[i][0] = graph[0][i];
    }

    for (int i = 1; i < numNodes; i++)
    {
        for (int j = 1; j < numNodes; j++)
        {
            if (i != j)
            {
                graph[i][j] = haversine(warehouses[i - 1].latitude, warehouses[i - 1].longitude,warehouses[j - 1].latitude, warehouses[j - 1].longitude);
            }
        }
    }

    double *dist = dijkstra(graph, 0, numNodes);

    double bestDistance = INF;
    int bestNode = -1;
    for (int i = 1; i < numNodes; i++)
    {
        if (dist[i] < bestDistance)
        {
            bestDistance = dist[i];
            bestNode = i;
        }
    }

    for (int i = 0; i < numNodes; i++)
    {
        free(graph[i]);
    }
    free(graph);
    free(dist);

    return (bestNode == -1) ? -1 : (bestNode - 1);
}
