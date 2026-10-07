# Dynamic Routing Using Distance Vector Routing Algorithm

graph = {}


# Add a bidirectional link
def add_link(router1, router2, cost):
    if router1 not in graph:
        graph[router1] = {}
    if router2 not in graph:
        graph[router2] = {}

    graph[router1][router2] = cost
    graph[router2][router1] = cost


# Distance Vector Routing (Bellman-Ford exchange)
def distance_vector_routing():
    routers = list(graph.keys())
    distance = {}
    next_hop = {}

    # Initialize routing tables
    for router in routers:
        distance[router] = {}
        next_hop[router] = {}
        for destination in routers:
            if router == destination:
                distance[router][destination] = 0
                next_hop[router][destination] = "-"
            elif destination in graph[router]:
                distance[router][destination] = graph[router][destination]
                next_hop[router][destination] = destination
            else:
                distance[router][destination] = float("inf")
                next_hop[router][destination] = "-"

    # Exchange distance vectors until no entry changes
    updated = True
    while updated:
        updated = False
        for router in routers:
            for neighbor in graph[router]:
                link_cost = graph[router][neighbor]
                for destination in routers:
                    new_distance = link_cost + distance[neighbor][destination]
                    if new_distance < distance[router][destination]:
                        distance[router][destination] = new_distance
                        next_hop[router][destination] = neighbor
                        updated = True

    return distance, next_hop


# Display routing table
def display_routing_table(source):
    distance, next_hop = distance_vector_routing()

    print("\nDistance Vector Routing Table for Router", source)
    print("---------------------------------------------")
    print("Destination\tCost\tNext Hop")
    print("---------------------------------------------")

    for router in graph:
        if router == source:
            print(f"{router}\t\t0\t-")
        elif distance[source][router] == float("inf"):
            print(f"{router}\t\tINF\tNo Path")
        else:
            print(f"{router}\t\t{distance[source][router]}\t{next_hop[source][router]}")

    print("---------------------------------------------")


# Main program
number_of_links = int(input("Enter number of links: "))

for i in range(1, number_of_links + 1):
    data = input(f"Enter link {i} (Router1 Router2 Cost): ").split()
    add_link(data[0], data[1], int(data[2]))

source = input("Enter source router: ")

if source not in graph:
    print("Invalid source router.")
else:
    display_routing_table(source)
