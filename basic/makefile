CXX=g++
CXX_COMPILE=$(CXX) $^ -o $@

TARGETS=reuse_addr nodelay_client nodelay_server rst_client rst_server epoll_server


all : $(TARGETS)

clean: $(TARGETS)
	rm $(TARGETS)

reuse_addr: reuse_addr.cpp
	$(CXX_COMPILE)

nodelay_client: nodelay_client.cpp
	$(CXX_COMPILE)

nodelay_server : nodelay_server.cpp
	$(CXX_COMPILE)

rst_client : rst_client.cpp
	$(CXX_COMPILE)

rst_server : rst_server.cpp
	$(CXX_COMPILE)

epoll_server: epoll_server.cpp
	$(CXX_COMPILE)
