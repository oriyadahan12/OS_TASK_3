CXX = g++
CXXFLAGS = -std=c++11 -Wall -pg  # Add -pg for profiling

TARGET_MATRIX = matrix
TARGET_LIST = list


all: $(TARGET_MATRIX) $(TARGET_LIST) 

$(TARGET_MATRIX): matrix.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET_MATRIX) matrix.cpp

$(TARGET_LIST): list.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET_LIST) list.cpp

clean:
	rm -f $(TARGET_MATRIX) $(TARGET_LIST) gmon.out
