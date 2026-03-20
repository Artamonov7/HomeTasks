#include "graph.h"

graph::graph() : matrix(nullptr), vertices(nullptr), capacity(10), size(0) {
    vertices = new int[capacity];
    matrix = new int*[capacity];
    int i, j;
    for (i = 0; i < capacity; i++){
        matrix[i] = new int[capacity];
        for (j = 0; j < capacity; j++)
            matrix[i][j] = 0;
    }
}

graph::graph(const graph& other) : matrix(nullptr), vertices(nullptr), capacity(0), size(0) {
    copy(other);
}

void graph::copy(const graph& other) {
    int i, j;
    capacity = other.capacity;
    size = other.size;

    vertices = new int[capacity];
    for (i = 0; i < size; i++)
        vertices[i] = other.vertices[i];
    
    matrix = new int*[capacity];
    for (i = 0; i < capacity; i++){
        matrix[i] = new int[capacity];
        for (j = 0; j < capacity; j++)
            if (i < size && j < size)
                matrix[i][j] = other.matrix[i][j];
            else
                matrix[i][j] = 0;
    }
}

graph::~graph() {
    clear();
}

void graph::clear() {
    int i;
    if (matrix) {
        for (i = 0; i < capacity; i++)
            delete[] matrix[i];
        delete[] matrix;
        matrix = nullptr;
    }
    delete[] vertices;
    vertices = nullptr;
    size = 0;
    capacity = 0;
}

graph& graph::operator=(const graph& other) {
    if (this == &other)
        return *this;
    clear();
    copy(other);
    return *this;
}

int graph::find_index(int v) const {
    int i;
    for (i = 0; i < size; i++)
        if (vertices[i] == v)
            return i;
    return -1;
}

void graph::resize() {
    int i, j;
    int new_cap = capacity * 2;
    int **new_matrix = new int*[new_cap];
    for (i = 0; i < new_cap; i++) {
        new_matrix[i] = new int[new_cap];
        for (j = 0; j < new_cap; j++)
            new_matrix[i][j] = 0;
    }

    for (i = 0; i < size; i++)
        for (j = 0; j < size; j++)
            new_matrix[i][j] = matrix[i][j];
    
    for (i = 0; i < capacity; i++)
        delete[] matrix[i];
    delete[] matrix;

    int *new_vertices = new int[new_cap];
    for (i = 0; i < size; i++)
        new_vertices[i] = vertices[i];
    delete[] vertices;

    matrix = new_matrix;
    vertices = new_vertices;
    capacity = new_cap;
}

void graph::add_vertex(int v) {
    if (has_vertex(v))
        throw error("vertex already exists", __LINE__);
    if (size >= capacity)
        resize();

    vertices[size++] = v;
}

void graph::add_edge(int from, int to) {
    int from_idx = find_index(from);
    int to_idx = find_index(to);
    
    if (from_idx == -1)
        throw error("source vertex not found", __LINE__);
    if (to_idx == -1)
        throw error("target vertex not found", __LINE__);

    matrix[from_idx][to_idx] = 1;
}

void graph::add_both(int v1, int v2) {
    add_edge(v1, v2);
    add_edge(v2, v1);
}

void graph::remove_vertex(int v) {
    int i, j;
    int idx = find_index(v);
    if (idx == -1)
        throw error("vertex not found", __LINE__);
    
    for (i = idx; i < size - 1; i++)
        vertices[i] = vertices[i + 1];

    for (i = idx; i < size - 1; i++)
        for (j = 0; j < size; j++)
            matrix[i][j] = matrix[i + 1][j];
    
    for (i = 0; i < size; i++)
        for (j = idx; j < size - 1; j++)
            matrix[i][j] = matrix[i][j + 1];

    for (i = size - 1; i < capacity; i++)
        for (j = 0; j < capacity; j++)
            matrix[i][j] = 0;
    
    for (i = 0; i < capacity; i++)
        matrix[i][size - 1] = 0;

    size--;
}

void graph::remove_edge(int from, int to) {
    int from_idx = find_index(from);
    int to_idx = find_index(to);

    if (from_idx == -1 || to_idx == -1)
        throw error("vertex not found", __LINE__);
    if (!matrix[from_idx][to_idx])
        throw error("edge doesnt exist", __LINE__);
    
    matrix[from_idx][to_idx] = 0;
}

void graph::remove_both(int v1, int v2) {
    remove_edge(v1, v2);
    remove_edge(v2, v1);
}

bool graph::has_vertex(int v) const {
    return find_index(v) != -1;
}

bool graph::has_edge(int from, int to) const {
    int from_idx = find_index(from);
    int to_idx = find_index(to);

    if (from_idx == -1 || to_idx == -1)
        return false;

    return matrix[from_idx][to_idx] == 1;
}

void graph::print_edges(int v, std::ostream& os) const {
    int i;
    int idx = find_index(v);
    if (idx == -1)
        throw error("vertex not found", __LINE__);

    os << v << " -> ";
    bool first = true;
    for (i = 0; i < size; i++){
        if (matrix[idx][i]) {
            if (!first) os << ", ";
            os << vertices[i];
            first = false;
        }
    }

    if (first) os << "none";
    os << std::endl;
}

struct my_queue_node {
    int data;
    my_queue_node *next;
};

class my_queue {
    my_queue_node *head;
    my_queue_node *tail;
public:
    my_queue() : head(nullptr), tail(nullptr) {}
    ~my_queue() {
        while (head){
            my_queue_node *tmp = head;
            head = head->next;
            delete tmp;
        }
    }

    void push(int val) {
        my_queue_node *node = new my_queue_node;
        node->data = val;
        node->next = nullptr;
        if (!tail){
            tail = node;
            head = node;
        }
        else {
            tail->next = node;
            tail = node;
        }
    }    
    int pop() {
        if (!head)
            return -1;
        my_queue_node *tmp = head;
        int val = head->data;
        head = head->next;
        if (!head)
            tail = nullptr;
        delete tmp;
        return val;
    }

    bool empty() const {
        return head == nullptr;
    }
};

struct my_stack_node {
    int vertex;
    bool processed;
    my_stack_node *next;
};

class my_stack {
    my_stack_node *top;
public:
    my_stack() : top(nullptr) {}
    ~my_stack() {
        while (top) {
            my_stack_node *tmp = top;
            top = top->next;
            delete tmp;
        }
    }

    void push(int v, bool proc = false) {
       my_stack_node *node = new my_stack_node;
       node->vertex = v;
       node->processed = proc;
       node->next = top;
       top = node;
    }
    void pop() {
        if (top){
            my_stack_node *tmp = top;
            top = top->next;
            delete tmp;
        }
    }
    my_stack_node *peek() {
        return top;
    }
    bool empty() const {
        return top == nullptr;
    }
};

bool graph::reachable(int from, int to) const {
    int from_idx = find_index(from);
    int to_idx = find_index(to);

    if (from_idx == -1 || to_idx == -1) 
        return false;
    if (from_idx == to_idx) 
        return true;
    
    bool *visited = new bool[size]();
    my_queue q;

    visited[from_idx] = true;
    q.push(from_idx);

    while (!q.empty()) {
        int i;
        int cur = q.pop();
        for (i = 0; i < size; i++){
            if (matrix[cur][i] && !visited[i]){
                if (i == to_idx){
                    delete[] visited;
                    return true;
                }
                visited[i] = true;
                q.push(i);
            }
        }
    }
    delete[] visited;
    return false;
}

void graph::find_scc() const {
    int i, j;
    if (size == 0) {
        std::cout << "graph is empty" << std::endl;
        return;
    }
    bool *visited = new bool[size]();
    int *order = new int[size];
    int order_count = 0;
    
    for (i = 0; i < size; i++){
        if (!visited[i]){
            my_stack st;
            st.push(i, false);
            while (!st.empty()){
                my_stack_node *cur = st.peek();
                int v = cur->vertex;
                bool done = cur->processed;
                st.pop();
                if (done){
                    order[order_count++] = v;
                    continue;
                }
                if (visited[v])
                    continue;
                visited[v] = true;
                st.push(v, true);

                for (j = 0; j < size; j++){
                    if (matrix[v][j] && !visited[j])
                        st.push(j, false);
                }
            }
        }
    }

    int** trans = new int*[size];
    for (i = 0; i < size; i++) {
        trans[i] = new int[size];
        for (j = 0; j < size; j++) {
            trans[i][j] = matrix[j][i];
        }
    }

    for (i = 0; i < size; i++)
        visited[i] = false;
    
    std::cout << "strongly connected components:" << std::endl;
    int comp_num = 1;

    for (i = order_count - 1; i >= 0; i--){
        int v = order[i];
        if (!visited[v]){
            std::cout << "component " << comp_num++ << ": ";
            my_stack st;
            st.push(v, false);
            visited[v] = true;

            while (!st.empty()){
                my_stack_node* cur = st.peek();
                int cur_v = cur->vertex;
                st.pop();
                std::cout << vertices[cur_v] << " ";

                for (j = 0; j < size; j++) {
                    if (trans[cur_v][j] && !visited[j]) {
                        visited[j] = true;
                        st.push(j, false);
                    }
                }
            }
            std::cout << std::endl;
        }
    }

    for (i = 0; i < size; i++)
        delete[] trans[i];

    delete[] trans;
    delete[] visited;
    delete[] order;
}

graph graph::operator+(const graph& other) const {
    graph res = *this;
    
    for (int i = 0; i < other.size; i++)
        if (!res.has_vertex(other.vertices[i]))
            res.add_vertex(other.vertices[i]);

    for (int i = 0; i < other.size; i++)
        for (int j = 0; j < other.size; j++)
            if (other.matrix[i][j])
                res.add_edge(other.vertices[i], other.vertices[j]);
    return res;
}

graph& graph::operator+(int vertex) {
    add_vertex(vertex);
    return *this;
}

graph& graph::operator+(const int edge[2]) {
    add_edge(edge[0], edge[1]);
    return *this;
}

graph graph::operator-(const graph& other) const {
    graph res = *this;

    for (int i = 0; i < other.size; i++)
        if (res.has_vertex(other.vertices[i]))
            res.remove_vertex(other.vertices[i]);
    return res;
}

graph& graph::operator-(int vertex) {
    remove_vertex(vertex);
    return *this;
}

graph& graph::operator-(const int edge[2]) {
    remove_edge(edge[0], edge[1]);
    return *this;
}

std::ostream& operator<<(std::ostream& os, const graph& g) {
    os << "graph (" << g.size << " vertices):" << std::endl;

    for (int i = 0; i < g.size; i++) {
        os << g.vertices[i] << " -> ";
        bool first = true;
        for (int j = 0; j < g.size; j++) {
            if (g.matrix[i][j]) {
                if (!first) os << ", ";
                os << g.vertices[j];
                first = false;
            }
        }
        if (first) os << "none";
        os << std::endl;
    }

    return os;
}
