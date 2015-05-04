CXX=gcc
CXX_FLAGS=-g
LINK_FLAGS=

CXX_COMPILE=$(CXX) $(CXX_FLAGS) -c $< -o $@
CXX_LINK=$(CXX) $(LINK_FLAGS) $^ -o $@

all : test_get_host_by_name

test_get_host_by_name: test_get_host_by_name.o
	$(CXX_LINK)

%.o : %.cpp
	$(CXX_COMPILE)

