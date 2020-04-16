CXX=g++

%.o : %.cpp
	$(CXX) $< -c -o $@

ttcp_block: ttcp_block.o common.o
	$(CXX) $^ -o $@
