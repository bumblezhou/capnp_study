rm ./test_*
rm ./test.capnp.*

capnpc -oc++ test.capnp

c++ -std=c++14 -Wall test.client.cpp test.capnp.c++ $(pkg-config --cflags --libs capnp-rpc) -o test_client
c++ -std=c++14 -Wall test.server.cpp test.capnp.c++ $(pkg-config --cflags --libs capnp-rpc) -o test_server