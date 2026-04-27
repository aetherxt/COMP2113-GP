cxx = g++
flags = -pedantic-errors -std=c++11 -Wall
target = game

OBJECTS  = main.o menu.o player.o

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET)

main.o: main.cpp menu.h player.h
	$(CXX) $(CXXFLAGS) -c main.cpp

menu.o: menu.cpp menu.h player.h
	$(CXX) $(CXXFLAGS) -c menu.cpp

player.o: player.cpp player.h
	$(CXX) $(CXXFLAGS) -c player.cpp

clean:
	rm -f $(TARGET) $(OBJECTS)

.PHONY: all clean