#include <stdio.h>
#include <string.h>
#include <limits.h>

#define MAX 20
#define INF INT_MAX

char routers[MAX][10];
int cost[MAX][MAX];
int n = 0;

// Return the index of a router, adding it if it is new
int getRouter(char name[]) {
    for (int i = 0; i < n; i++) {
        if (strcmp(routers[i], name) == 0)
            return i;
    }
    strcpy(routers[n], name);
    return n++;
}

// Add a bidirectional link
void addLink(char router1[], char router2[], int c) {
    int u = getRouter(router1);
    int v = getRouter(router2);
    cost[u][v] = c;
    cost[v][u] = c;
}

// Dijkstra's shortest-path algorithm
void dijkstra(int source, int distance[], int previous[]) {
    int visited[MAX] = {0};

    for (int i = 0; i < n; i++) {
        distance[i] = INF;
        previous[i] = -1;
    }
    distance[source] = 0;

    for (int count = 0; count < n; count++) {
        int current = -1;
        int minDistance = INF;

        // Select unvisited router with minimum distance
        for (int i = 0; i < n; i++) {
            if (!visited[i] && distance[i] < minDistance) {
                minDistance = distance[i];
                current = i;
            }
        }

        if (current == -1)
            break;

        visited[current] = 1;

        // Update neighboring routers
        for (int neighbor = 0; neighbor < n; neighbor++) {
            if (cost[current][neighbor] > 0 && !visited[neighbor]) {
                int newDistance = distance[current] + cost[current][neighbor];
                if (newDistance < distance[neighbor]) {
                    distance[neighbor] = newDistance;
                    previous[neighbor] = current;
                }
            }
        }
    }
}

// Print the path from source to destination
void printPath(int destination, int previous[]) {
    if (previous[destination] != -1) {
        printPath(previous[destination], previous);
        printf(" -> ");
    }
    printf("%s", routers[destination]);
}

// Display routing table
void displayRoutingTable(int source) {
    int distance[MAX], previous[MAX];
    dijkstra(source, distance, previous);

    printf("\nRouting Table for Router %s\n", routers[source]);
    printf("---------------------------------------------\n");
    printf("Destination\tCost\tShortest Path\n");
    printf("---------------------------------------------\n");

    for (int i = 0; i < n; i++) {
        if (i == source) {
            printf("%s\t\t0\t%s\n", routers[i], routers[source]);
        } else if (distance[i] == INF) {
            printf("%s\t\tINF\tNo Path\n", routers[i]);
        } else {
            printf("%s\t\t%d\t", routers[i], distance[i]);
            printPath(i, previous);
            printf("\n");
        }
    }
    printf("---------------------------------------------\n");
}

int main() {
    int numberOfLinks, c;
    char router1[10], router2[10], source[10];

    printf("Enter number of links: ");
    scanf("%d", &numberOfLinks);

    for (int i = 1; i <= numberOfLinks; i++) {
        printf("Enter link %d (Router1 Router2 Cost): ", i);
        scanf("%s %s %d", router1, router2, &c);
        addLink(router1, router2, c);
    }

    printf("Enter source router: ");
    scanf("%s", source);

    int s = -1;
    for (int i = 0; i < n; i++) {
        if (strcmp(routers[i], source) == 0)
            s = i;
    }

    if (s == -1)
        printf("Invalid source router.\n");
    else
        displayRoutingTable(s);

    return 0;
}
