CXX = g++
CXXFLAGS = -Wall -g
TARGET = graph

all: test

$(TARGET): graph.o main.o
	$(CXX) $(CXXFLAGS) -o $(TARGET) graph.o main.o

graph.o: graph.cpp graph.h
	$(CXX) $(CXXFLAGS) -c graph.cpp

main.o: main.cpp graph.h
	$(CXX) $(CXXFLAGS) -c main.cpp

test: $(TARGET)
	./$(TARGET) test

clean:
	rm -f $(TARGET) *.o
