#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>

class graph {
    int **matrix;
    int *vertices;
    int capacity;   // Вместимость
    int size;       // Число вершин

    int find_index(int v) const;
    void resize();
    void clear();
    void copy(const graph& other);

public:
    class error : public std::exception {
    private:
        const char *msg;
        int line;
    public:
        error(const char *message, int line_num) : msg(message), line(line_num){}
        const char *what() const noexcept override { 
            return msg;
        }
        int get_line() const {
            return line;
        }
    };

    graph();
    graph(const graph& other);
    ~graph();

    graph& operator=(const graph& other);
    graph operator+(const graph& other) const;
    graph operator-(const graph& other) const;
    graph& operator+(int vertex);
    graph& operator+(const int edge[2]);
    graph& operator-(int vertex);
    graph& operator-(const int edge[2]);
    friend std::ostream& operator<<(std::ostream& os, const graph& g);

    void add_vertex(int v);
    void add_edge(int from, int to);
    void add_both(int v1, int v2);
    void remove_vertex(int v);
    void remove_edge(int from, int to);
    void remove_both(int v1, int v2);

    void print_edges(int v, std::ostream& os = std::cout) const;
    bool reachable(int from, int to) const;
    void find_scc() const;
    int get_size() const {
        return size;
    }
    bool has_vertex(int v) const;
    bool has_edge(int from, int to) const;
    int get_vertex(int index) const { 
        return vertices[index]; 
    }
};

#endif
