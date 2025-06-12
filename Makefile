CXX = g++
CXXFLAGS = -I./sqlite/include
LDFLAGS = -L./sqlite/src -lsqlite3

TARGET = programa
SRC = main.cpp

$(TARGET): $(SRC)
	$(CXX) $(SRC) $(CXXFLAGS) $(LDFLAGS) -o $(TARGET)

clean:
	rm -f $(TARGET).exe

.PHONY: clean 