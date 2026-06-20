#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <limits>
#include <sstream>
using namespace std;

// Graph class represents a directed weighted graph and algorithms
//basically stores adjacency list

class Graph {
private:
	
    int total; // total number of nodes in the graph
    vector<vector<pair<int,int>>> link; 
    // adjacency list: each node has a vector of pairs {neighbor, weight}
    //link[0], link[1] are nodes and they have lists within themselves of edges going out of them

public:

    // Constructor initializes the graph with n nodes
    Graph(int n) {
        total = n;
        //create n empty vectors, one per node. After this, link[i] is ready for pushes
        link.resize(n); // allocate space for adjacency list
        
    }

    // Add an edge from 'from' node to 'to' node with weight 'w'
    void addedge(int from, int to, int w) {
        if(from<0 || to<0 || from>=total || to>=total){
            cout << "Invalid node number!\n"; // check for invalid input
            return;
        }
        link[from].push_back({to, w}); // add the directed edge
    }

    // BFS traversal from start node
    void runbfs(int start, const vector<string> &names) {
        if(start<0 || start>=total){ 
            cout<<"Invalid start node!\n"; 
            return; 
        }

        // we use a visited array to keep track of nodes we've seen
        vector<int> visited(total, 0);
        queue<int> q; // BFS uses a queue(to push neighbours)

        // mark the start node visited and enqueue it
        visited[start] = 1; 
        q.push(start);      

        cout << "BFS: ";

        // run the BFS loop
        while(!q.empty()) {
            int now = q.front(); 
            q.pop();

            // print current node
            cout << names[now] << " "; 

            // check all neighbors of the current node
            for(auto &nbr : link[now]) {
            	//extract the index of the neighbour
                int ne = nbr.first; 
                //if neighbour is unvisited
                if(!visited[ne]) {
                	
                	//mark as visited 
                	//then enqueue
                	
                    visited[ne] = 1; 
                    q.push(ne);     
                }
            }
        }
        cout << "\n";
    }

    // DFS traversal starting from start node
    //explores as far as possible along each branch before backtracking.
    // Recursive helper for DFS
    void rundfshelp(int now, vector<int> &visited, const vector<string> &names) {
        // mark the node as visited
        visited[now] = 1;
        cout << names[now] << " "; // print current node

        // visit all neighbors recursively
        for(auto &nbr : link[now]) {
            int ne = nbr.first;
            if(!visited[ne]) {
                rundfshelp(ne, visited, names);
            }
        }
    }

    
    void rundfs(int start, const vector<string> &names) {
        if(start<0 || start>=total){ 
            cout<<"Invalid start node!\n"; 
            return; 
        }

        vector<int> visited(total, 0); // visited array
        cout << "DFS: ";
        rundfshelp(start, visited, names); // start recursive DFS
        cout << "\n";
    }

    //finds shortest distances from start to all nodes in graphs with nonnegative weights using a priority queue (min-heap).
    void rundijkstra(int start, const vector<string> &names) {
        if(start<0 || start>=total){ 
            cout<<"Invalid start node!\n"; 
            return; 
        }

        // initialize distances to maximum
        vector<int> dist(total, numeric_limits<int>::max());
        // to store the path, stores the predecessor to reconstruct paths.
        //parent init -1 meaning no parent yet.
        vector<int> parent(total, -1); 

        // priority queue to pick node with smallest distance
        priority_queue<pair<int,int>,
                       vector<pair<int,int>>,
                       greater<pair<int,int>>> pq;
        //start from source with distance 0.

        dist[start] = 0; 
        pq.push({0, start}); 

        // main loop of Dijkstra
        while(!pq.empty()) {
        	//returns smallest distance entry.
            auto top = pq.top();
            pq.pop();
            int cost = top.first;
            int now = top.second;

            // skip if a shorter path has already been found
            //basically skip stale entries
			// (because we might have pushed multiple entries for same node with different distances)
            if(cost > dist[now]) continue;

            // check all neighbors
            for(auto &nbr : link[now]) {
                int ne = nbr.first;
                int w = nbr.second;

                // if going through 'now' improves distance, update
                //or each neighbor ne with weight w, attempt relaxation

                if(dist[now] + w < dist[ne]) { //found better path
                    dist[ne] = dist[now] + w;
                    parent[ne] = now; // track the path
                    pq.push({dist[ne], ne});
                }
            }
        }

        // display shortest distance and path to each node
        cout << "Shortest distances from " << names[start] << ":\n";
        for(int i=0;i<total;i++){
            cout << "to " << names[i] << " = " << dist[i];

            // reconstruct path
            if(dist[i] != numeric_limits<int>::max()){
                cout << " ; path: ";
                vector<int> path;
                for(int v=i; v!=-1; v=parent[v]) path.push_back(v); // backtrack
                for(int j=path.size()-1;j>=0;j--){
                    cout << names[path[j]];
                    if(j>0) cout << " -> ";
                }
            }
            cout << "\n";
        }
    }

    // Display all nodes
    void displayNodes(const vector<string> &names) {
        cout << "Nodes: ";
        for(int i=0;i<total;i++) cout << names[i] << " ";
        cout << "\n";
    }

    // Display all edges
    void displayEdges(const vector<string> &names) {
        cout << "Edges (from -> to : weight):\n";
        for(int i=0;i<total;i++){
            for(auto &nbr : link[i]){
                cout << names[i] << " -> " << names[nbr.first] << " : " << nbr.second << "\n";
            }
        }
    }

    // Check if the graph is fully connected
    //This checks if all nodes are reachable starting from node 0 using BFS.
    bool isConnected() {
        if(total == 0) return true;

        vector<int> visited(total, 0);
        queue<int> q;

        visited[0] = 1; // start from node 0
        q.push(0);
        int count = 1;

        // BFS to count reachable nodes
        while(!q.empty()) {
            int now = q.front(); q.pop();
            for(auto &nbr : link[now]) {
                int ne = nbr.first;
                if(!visited[ne]) {
                    visited[ne] = 1;
                    q.push(ne);
                    count++;
                }
            }
        }

        return count == total; // true if all nodes are reachable
    }

    // Recursive helper to detect cycle using DFS
    bool detectCycleDFS(int node, vector<int>& visited, vector<int>& recStack) {
        visited[node] = 1;
        //mark visited and push node to recursion stack.
        recStack[node] = 1; 

        // check neighbors
        //If not visited, recursively check subtree. If recursion returns true then cycle found.
        for (auto &nbr : link[node]) {
            int ne = nbr.first;
            if (!visited[ne] && detectCycleDFS(ne, visited, recStack))
                return true;
            else if (recStack[ne])
                return true; // cycle detected
        }

        recStack[node] = 0; // remove from recursion stack
        return false;
    }

    // Check if graph contains any cycle
    bool hasCycle() {
        vector<int> visited(total, 0);
        vector<int> recStack(total, 0);

        // check each node
        for (int i = 0; i < total; i++) {
            if (!visited[i]) {
                if (detectCycleDFS(i, visited, recStack))
                    return true;
            }
        }
        return false;
    }

}; // end Graph class

int main() {

    // Ask user for number of nodes
    int n;
    cout << "How many nodes? ";
    while(!(cin >> n) || n<=0){
        cout<<"Invalid input! Enter a positive integer.\n";
        cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore();

    // Create mappings from index to name and name to index
    vector<string> index2name(n);
    unordered_map<string,int> name2index;

    // Ask if user wants to use city names
    cout << "Do you want to name the cities? (y/n): ";
    char choice;
    cin >> choice;
    bool useNames = (choice=='y' || choice=='Y');
    cin.ignore();

    if(useNames){
        cout << "Enter the city names, one per line:\n";
        for(int i=0;i<n;i++){
            string name;
            getline(cin,name);
            if(name.empty()){ 
                cout<<"Empty name not allowed!\n"; 
                i--; 
                continue; 
            }
            if(name2index.count(name)){
                cout << "Duplicate name! Enter a unique name.\n"; 
                i--; 
                continue;
            }
            index2name[i] = name;
            name2index[name] = i;
        }
    } else {
        for(int i=0;i<n;i++){
            index2name[i] = to_string(i);
            name2index[index2name[i]] = i;
        }
    }

    // Create the graph
    Graph g(n);

    // Ask user for number of edges
    int e;
    cout << "How many edges? ";
    while(!(cin>>e) || e<0){
        cout<<"Invalid input! Enter a non-negative integer.\n";
        cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore();

    cout << "Enter each edge: from to weight\n";
    cout << "(If using names, type exact names; otherwise, type numbers)\n";

    for(int i=0;i<e;i++){
        string line, a, b;
        int w;
        getline(cin,line);
        stringstream ss(line);
        ss >> a >> b >> w;

        int ia=-1, ib=-1;
        try{
            if(useNames){
                if(!name2index.count(a) || !name2index.count(b)){
                    cout << "Invalid city name, try again.\n"; 
                    i--; 
                    continue;
                }
                ia = name2index[a]; ib = name2index[b];
            } else {
                ia = stoi(a); ib = stoi(b);
                if(ia<0 || ib<0 || ia>=n || ib>=n){
                    cout << "Invalid node number, try again.\n";
                    i--; 
                    continue;
                }
            }
            if(w<=0){ cout<<"Weight must be positive!\n"; i--; continue; }
        }catch(...){
            cout<<"Invalid input! Try again.\n"; i--; continue;
        }
        g.addedge(ia,ib,w);
    }

    int choiceMenu;

    do {

        // display menu
        cout << "\nGRAPH-BASED PATH FINDER MENU\n";
        cout << "1. Display all nodes\n";
        cout << "2. Display all edges\n";
        cout << "3. Add an edge\n";
        cout << "4. Perform BFS\n";
        cout << "5. Perform DFS\n";
        cout << "6. Dijkstra shortest path from a node\n";
        cout << "7. Check if graph is fully connected\n";
        cout << "8. Check if graph has a cycle\n";
        cout << "9. Exit\n";

        cout << "Enter your choice: ";
        while(!(cin >> choiceMenu)){
            cout<<"Invalid input! Enter a number between 1 and 9.\n";
            cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout<<"Enter your choice: ";
        }
        cin.ignore();

        switch(choiceMenu){
            case 1:
                g.displayNodes(index2name);
                break;
            case 2:
                g.displayEdges(index2name);
                break;
            case 3: {
                cout << "Enter edge (from to weight): ";
                string line, a, b;
                int w;
                getline(cin,line);
                stringstream ss(line);
                ss >> a >> b >> w;

                int ia=-1, ib=-1;
                try{
                    if(useNames){
                        if(!name2index.count(a) || !name2index.count(b)){
                            cout << "Invalid city name!\n"; break;
                        }
                        ia = name2index[a]; ib = name2index[b];
                    } else {
                        ia = stoi(a); ib = stoi(b);
                        if(ia<0 || ib<0 || ia>=n || ib>=n){ cout << "Invalid node number!\n"; break; }
                    }
                    if(w<=0){ cout<<"Weight must be positive!\n"; break; }
                }catch(...){
                    cout<<"Invalid input!\n"; break;
                }
                g.addedge(ia, ib, w);
                break;
            }
            case 4: {
                cout << "Enter start node: ";
                string startName; getline(cin,startName);
                int start=-1;
                try{
                    if(useNames){
                        if(!name2index.count(startName)){ cout<<"Invalid city name!\n"; break; }
                        start=name2index[startName];
                    } else start=stoi(startName);
                    if(start<0 || start>=n){ cout<<"Invalid node number!\n"; break; }
                }catch(...){ cout<<"Invalid input!\n"; break; }
                g.runbfs(start, index2name);
                break;
            }
            case 5: {
                cout << "Enter start node: ";
                string startName; getline(cin,startName);
                int start=-1;
                try{
                    if(useNames){
                        if(!name2index.count(startName)){ cout<<"Invalid city name!\n"; break; }
                        start=name2index[startName];
                    } else start=stoi(startName);
                    if(start<0 || start>=n){ cout<<"Invalid node number!\n"; break; }
                }catch(...){ cout<<"Invalid input!\n"; break; }
                g.rundfs(start, index2name);
                break;
            }
            case 6: {
                cout << "Enter start node: ";
                string startName; getline(cin,startName);
                int start=-1;
                try{
                    if(useNames){
                        if(!name2index.count(startName)){ cout<<"Invalid city name!\n"; break; }
                        start=name2index[startName];
                    } else start=stoi(startName);
                    if(start<0 || start>=n){ cout<<"Invalid node number!\n"; break; }
                }catch(...){ cout<<"Invalid input!\n"; break; }
                g.rundijkstra(start, index2name);
                break;
            }
            case 7:
                if(g.isConnected())
                    cout << "The graph is fully connected!\n";
                else
                    cout << "The graph is NOT fully connected!\n";
                break;
            case 8:
                if(g.hasCycle())
                    cout << "The graph contains a cycle!\n";
                else
                    cout << "The graph does NOT contain a cycle.\n";
                break;
            case 9:
                cout << "Exiting program.\n";
                break;
            default:
                cout << "Invalid choice! Try again.\n";
        }

    } while(choiceMenu != 9);

    return 0;
}