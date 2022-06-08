#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;
//klasa Graph
class Graph {
    int nVertices;
    int nArrows;
public:
    Graph(int v, int a) {
        nVertices = v;
        nArrows = a;
        vector<int> temp;
        for (int i = 0; i < v; i++)
            adjList.push_back(temp);
    }
    ~Graph() = default;
    vector<vector<int>> adjList;
};
int main() {
    //odczyt z pliku
    string filename;
    cout << "Podaj nazwe pliku:";
    cin >> filename;
    cout << "\n";
    fstream file("path_to_directory" + filename + ".txt", fstream::in);

    if (!file.good())
    {
        cout << "Nie mozna otworzyc";
    }
    else
    {
        cout << "WCZYTANY GRAF:" << endl;
        int v, a, vertex, next;
        file >> v;
        cout << v << endl;
        file >> a;
        cout << a << endl;
        Graph G(v, a);                  //utworzenie obiektu klasy Graph
        for (int numA = 0; numA < a; numA++) {
            file >> vertex;
            file >> next;
            int index = vertex - 1;
            G.adjList[index].push_back(next);
            cout << vertex << " " << next << endl;
            
        }
        cout << "\n";
        
        file.close();
        
        //sprawdzenie czy graf jest 1-grafem
        bool disjoint, same;
        bool adjoint = true;
        for (int x = 0; x < G.adjList.size(); x++) {
            for (int n = 0; n < G.adjList[x].size(); n++) {
                for (int k = n + 1; k < G.adjList[x].size(); k++) {
                    if (G.adjList[x][n] == G.adjList[x][k]) {        
                        adjoint = false;
                        break;
                    }
                }
            }
            //sprawdzenie warunku na sprzężenie grafu 𝑁 +(𝑥) ∩ 𝑁 +(𝑦) ≠ Ø ⇒ 𝑁 +(𝑥) = 𝑁 +(𝑦) 
            for (int y = 0; y < G.adjList.size(); y++) {
                if (x == y) continue;
                disjoint = true;
                same = false;
                for (int nextX = 0; nextX < G.adjList[x].size(); nextX++) {
                    for (int nextY = 0; nextY < G.adjList[y].size(); nextY++)
                        if (G.adjList[x][nextX] ==
                            G.adjList[y][nextY]) {      
                            disjoint = false;                       
                            same = equal(G.adjList[x].begin(), G.adjList[x].end(), G.adjList[y].begin());   
                            break;
                        }
                    if (!disjoint) break;
                }
                if (!disjoint && !same) {           
                    adjoint = false;
                    break;
                }
            }
            if (!adjoint) break;
        }
        if (adjoint) cout << "Graf jest grafem sprzezonym" << endl;
        else cout << "Graf nie jest grafem sprzezonym" << endl;
        //sprawdzenie czy graf jest grafem liniowym (jesli jest grafem sprzezonym)
        if (adjoint) {
            bool lineGraph = true;
            int v1, v2;
            for (int x = 0; x < G.adjList.size(); x++) {     
                if ((G.adjList[x].size() > 1) && lineGraph) {
                    for (int next1 = 0; next1 < G.adjList[x].size(); next1++) {      
                        for (int next2 = 0; next2 < G.adjList[x].size(); next2++) {  
                            if ((next1 != next2) && lineGraph) {
                                v1 = G.adjList[x][next1];
                                v2 = G.adjList[x][next2];
                                for (int n1 = 0; n1 < G.adjList[v1 - 1].size(); n1++) {  
                                    for (int n2 = 0; n2 < G.adjList[v2 - 1].size(); n2++) {  
                                        if (!G.adjList[v1 - 1].empty() && !G.adjList[v2 - 1].empty()) {
                                            //sprawdzenie czy graf nie zawiera struktury I albo II jako podgrafu
                                            if (G.adjList[v1 - 1][n1] == G.adjList[v2 - 1][n2]) {
                                                lineGraph = false;
                                             
                                                break;
                                            }
                                            //sprawdzenie czy graf nie zawiera struktury III jako podgrafu
                                            if (((x + 1) == v1) && (n1 != n2))     
                                                if (G.adjList[v2 - 1][n1] == v2 && G.adjList[v2 - 1][n2] == (x + 1)) {    
                                                    lineGraph = false;
                                                    break;
                                                }
                                        }
                                    }
                                    if (!lineGraph) break;
                                }
                            }
                        }
                    }
                }
            }
            if (lineGraph) cout << "Graf jest grafem liniowym" << endl;
            else cout << "Graf nie jest grafem liniowym" << endl;
        }
        //zmiana wierzchołków z G na łuki w H
        if (adjoint) {
            vector<vector<int>> arrowsH;
            vector<int> temp;
            int x, y, changed, maxV;
            for (int i = 0, j = 1; i < G.adjList.size(); i++, j++) {      
                temp.push_back(j); 
                //cout << "p" << j;
                j++;  
                //cout <<"k" << j;
                temp.push_back(j);
                arrowsH.push_back(temp);
                temp.clear();
            }
            //utworzenie połączeń w łukach
            for (int i = 0; i < G.adjList.size(); i++) {         
                if (!G.adjList[i].empty()) {                     
                    for (int j = 0; j < G.adjList[i].size(); j++) {
                        x = i + 1;
                        y = G.adjList[i][j];
                        changed = arrowsH[y - 1][0];            
                        arrowsH[y - 1][0] = arrowsH[x - 1][1];    
                        for (int n = 0; n < arrowsH.size(); n++) {              
                            if (arrowsH[n][0] == changed)
                                arrowsH[n][0] = arrowsH[y - 1][0];
                            if (arrowsH[n][1] == changed)
                                arrowsH[n][1] = arrowsH[y - 1][0];
                            
                        }
                    }
                }
            }
            //przeindeksowanie wierzchołków
            
            for (int vertexNum = 1; vertexNum < (2 * arrowsH.size()); vertexNum++) {     
                bool inOrder = false;
                for (int arrow = 0; arrow < arrowsH.size(); arrow++)     
                    
                    if (arrowsH[arrow][0] == vertexNum || arrowsH[arrow][1] == vertexNum) {
                        inOrder = true;
                        break;
                    }
                if (!inOrder) {     
                    int index = vertexNum;  
                    
                    do {
                        vertexNum++;
                        for (int arrow = 0; arrow < arrowsH.size(); arrow++) {
                            for (int i = 0; i < 2; i++)
                                
                                if (arrowsH[arrow][i] == vertexNum) {
                                    arrowsH[arrow][i] = index;         
                                    maxV = arrowsH[arrow][i];
                                    
                                    inOrder = true;
                                    
                                }
                        }
                    } while (!inOrder && vertexNum < (2 * arrowsH.size()));
                    vertexNum = index;
                }

            }
            //stworzenie obiektu H klasy Graph
            Graph H(maxV, arrowsH.size());
            for (int i = 0; i < arrowsH.size(); i++) {
                H.adjList[arrowsH[i][0] - 1].push_back(arrowsH[i][1]); 
                sort(H.adjList[arrowsH[i][0] - 1].begin(), H.adjList[arrowsH[i][0] - 1].end());
                
            }
            //zapis do pliku
            fstream outfile("path_to_directory" + filename + "_out.txt", fstream::out);
            if (!outfile.good())
            {
                cout << "nie udalo sie zapisac pliku";
            }
            else
            {
                cout << "\n";
                cout << "GRAF ORYGINALNY H:" << endl;
                outfile << H.adjList.size() << endl;
                cout<< H.adjList.size() << endl;
                outfile << arrowsH.size() << endl;
                cout << arrowsH.size() << endl;
                for (int i = 0; i < H.adjList.size(); i++)
                    for (int j = 0; j < H.adjList[i].size(); j++) {
                        outfile << i + 1 << " " << H.adjList[i][j] << endl;
                        cout << i + 1 << " " << H.adjList[i][j] << endl;
                        
                    }
                outfile.close();
                return 0;
            }
        }
    }
}