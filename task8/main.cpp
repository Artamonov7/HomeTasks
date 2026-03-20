#include "graph.h"
#include <cstring>

void test_basic() {
    std::cout << "\ntest 1: basic operations" << std::endl;
    graph g;
    
    try {
        g.add_vertex(1);
        g.add_vertex(2);
        g.add_vertex(3);
        g.add_edge(1, 2);
        g.add_edge(2, 3);
        g.add_both(1, 3);
        
        std::cout << g;
        
        g.print_edges(1);
        g.print_edges(2);
        
        std::cout << "3 -> 1 reachable? " << (g.reachable(3, 1) ? "yes" : "no") << std::endl;
        std::cout << "1 -> 3 reachable? " << (g.reachable(1, 3) ? "yes" : "no") << std::endl;
        
        g.find_scc();
    }
    catch (graph::error& e) {
        std::cerr << "error at line " << e.get_line() << ": " << e.what() << std::endl;
    }
}

void test_operators() {
    std::cout << "\ntest 2: operators" << std::endl;
    graph g1, g2;
    
    try {
        g1.add_vertex(1);
        g1.add_vertex(2);
        g1.add_edge(1, 2);
        
        g2.add_vertex(2);
        g2.add_vertex(3);
        g2.add_edge(2, 3);
        
        graph g3 = g1 + g2;
        std::cout << "union:" << std::endl << g3;
        
        g3 + 4;
        int edge[2] = {3, 4};
        g3 + edge;
        std::cout << "after adding:" << std::endl << g3;
        
        g3 - 2;
        std::cout << "after removing vertex 2:" << std::endl << g3;
        
        // удаление вершины 3 через метод remove_vertex
        g3.remove_vertex(3);
        std::cout << "after removing vertex 3:" << std::endl << g3;
    }
    catch (graph::error& e) {
        std::cerr << "error at line " << e.get_line() << ": " << e.what() << std::endl;
    }
}

void test_scc() {
    std::cout << "\test 3: strongly connected components" << std::endl;
    graph g;
    
    try {
        g.add_vertex(1);
        g.add_vertex(2);
        g.add_vertex(3);
        g.add_vertex(4);
        g.add_vertex(5);
        g.add_vertex(6);
        
        g.add_both(1, 2);
        g.add_both(2, 3);
        g.add_both(3, 1);
        g.add_both(4, 5);
        g.add_edge(5, 4);
        
        std::cout << g;
        g.find_scc();
    }
    catch (graph::error& e) {
        std::cerr << "error at line " << e.get_line() << ": " << e.what() << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc > 1 && strcmp(argv[1], "test") == 0) {
        test_basic();
        test_operators();
        test_scc();
    } else {
        graph g;
        int choice, v, v1, v2;
        
        while (true) {
            std::cout << "\nmenu" << std::endl;
            std::cout << "1. add vertex" << std::endl;
            std::cout << "2. add edge" << std::endl;
            std::cout << "3. add both directions" << std::endl;
            std::cout << "4. remove vertex" << std::endl;
            std::cout << "5. remove edge" << std::endl;
            std::cout << "6. print graph" << std::endl;
            std::cout << "7. print edges from vertex" << std::endl;
            std::cout << "8. check reachability" << std::endl;
            std::cout << "9. find SCC" << std::endl;
            std::cout << "0. exit" << std::endl;
            std::cout << "choice: ";
            std::cin >> choice;
            
            try {
                switch (choice) {
                    case 1:
                        std::cout << "vertex: ";
                        std::cin >> v;
                        g.add_vertex(v);
                        break;
                    case 2:
                        std::cout << "from to: ";
                        std::cin >> v1 >> v2;
                        g.add_edge(v1, v2);
                        break;
                    case 3:
                        std::cout << "between: ";
                        std::cin >> v1 >> v2;
                        g.add_both(v1, v2);
                        break;
                    case 4:
                        std::cout << "vertex: ";
                        std::cin >> v;
                        g.remove_vertex(v);
                        break;
                    case 5:
                        std::cout << "from to: ";
                        std::cin >> v1 >> v2;
                        g.remove_edge(v1, v2);
                        break;
                    case 6:
                        std::cout << g;
                        break;
                    case 7:
                        std::cout << "vertex: ";
                        std::cin >> v;
                        g.print_edges(v);
                        break;
                    case 8:
                        std::cout << "from to: ";
                        std::cin >> v1 >> v2;
                        std::cout << (g.reachable(v1, v2) ? "reachable" : "not reachable")
                                 << std::endl;
                        break;
                    case 9:
                        g.find_scc();
                        break;
                    case 0:
                        return 0;
                }
            }
            catch (graph::error& e) {
                std::cerr << "error at line " << e.get_line() << ": " 
                        << e.what() << std::endl;
            }
        }
    }
    
    return 0;
}
