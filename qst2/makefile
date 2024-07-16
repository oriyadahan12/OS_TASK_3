CXX = g++
CXXFLAGS = -std=c++11 -Wall -pg  # Add -pg for profiling

TARGET_DEQUE = dequeq2
TARGET_LIST = listq2
TARGET_MATRIX = adj_matrix
TARGET_Q2 = q2

all: $(TARGET_DEQUE) $(TARGET_LIST) $(TARGET_MATRIX) $(TARGET_Q2)

$(TARGET_DEQUE): dequeq2.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET_DEQUE) dequeq2.cpp

$(TARGET_LIST): listq2.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET_LIST) listq2.cpp

$(TARGET_MATRIX): adj_matrix.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET_MATRIX) adj_matrix.cpp

$(TARGET_Q2): q2.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET_Q2) q2.cpp

clean:
	rm -f $(TARGET_DEQUE) $(TARGET_LIST) $(TARGET_MATRIX) $(TARGET_Q2) gmon.out
