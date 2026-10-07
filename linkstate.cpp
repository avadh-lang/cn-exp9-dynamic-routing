#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <string>
#include <climits>
#include <algorithm>
using namespace std;

struct Edge {
    string router;
    int cost;
};

// Link State Database: every router's list of links
map<string, vector<Edge>> graph;
vector<string> order;

// Add a bidirectional link
void addLink(string router1, string router2, int cost) {
    if (graph.find(router1) == graph.end()) order.push_back(router1);
    if (graph.find(router2) == graph.end()) order.push_back(router2);

    graph[router1].push_back({router2, cost});
    graph[router2].push_back({router1, cost});
}

// Construct shortest path
string getPath(string destination, map<string, string> &previous) {
    vector<string> path;
    string current = destination;

    while (current != "") {
        path.push_back(current);
        current = previous[current];
    }
    reverse(path.begin(), path.end());

    string result = path[0];
    for (size_t i = 1; i < path.size(); i++)
        result += " -> " + path[i];
    return result;
}

// Display routing table
void displayRoutingTable(string source, map<string, int> &distance,
                         map<string, string> &previous) {
    cout << "\nLink State Routing Table for Router " << source << endl;
    cout << "---------------------------------------------" << endl;
    cout << "Destination\tCost\tShortest Path" << endl;
    cout << "---------------------------------------------" << endl;

    for (string router : order) {
        if (router == source)
            cout << router << "\t\t0\t" << source << endl;
        else if (distance[router] == INT_MAX)
            cout << router << "\t\tINF\tNo Path" << endl;
        else
            cout << router << "\t\t" << distance[router] << "\t"
                 << getPath(router, previous) << endl;
    }
    cout << "---------------------------------------------" << endl;
}

// Link State Routing using Dijkstra's algorithm
void linkStateRouting(string source) {
    map<string, int> distance;
    map<string, string> previous;
    set<string> visited;

    // Initialize distances
    for (string router : order) {
        distance[router] = INT_MAX;
        previous[router] = "";
    }
    distance[source] = 0;

    while (visited.size() < graph.size()) {
        string current = "";
        int minDistance = INT_MAX;

        // Select unvisited router with minimum distance
        for (string router : order) {
            if (!visited.count(router) && distance[router] < minDistance) {
                minDistance = distance[router];
                current = router;
            }
        }

        if (current == "")
            break;

        visited.insert(current);

        // Update neighboring routers
        for (Edge edge : graph[current]) {
            int newDistance = distance[current] + edge.cost;
            if (newDistance < distance[edge.router]) {
                distance[edge.router] = newDistance;
                previous[edge.router] = current;
            }
        }
    }

    displayRoutingTable(source, distance, previous);
}

int main() {
    int numberOfLinks;
    cout << "Enter number of links: ";
    cin >> numberOfLinks;

    for (int i = 1; i <= numberOfLinks; i++) {
        string router1, router2;
        int cost;
        cout << "Enter link " << i << " (Router1 Router2 Cost): ";
        cin >> router1 >> router2 >> cost;
        addLink(router1, router2, cost);
    }

    string source;
    cout << "Enter source router: ";
    cin >> source;

    if (graph.find(source) == graph.end())
        cout << "Invalid source router." << endl;
    else
        linkStateRouting(source);

    return 0;
}
