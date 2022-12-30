#include "test.capnp.h"
#include <kj/debug.h>
#include <capnp/ez-rpc.h>
#include <capnp/message.h>
#include <iostream>

int main(int argc, const char* argv[]) {
  if (argc != 2) {
    std::cerr << "usage: " << argv[0] << " HOST:PORT\n"
        "Connects to the TestInterface server at the given address and "
        "does some RPCs." << std::endl;
    return 1;
  }

  capnp::EzRpcClient client(argv[1]);
  TestInterface::Client client_inst = client.getMain<TestInterface>();

  // Keep an eye on `waitScope`.  Whenever you see it used is a place where we
  // stop and wait for the server to respond.  If a line of code does not use
  // `waitScope`, then it does not block!
  auto& waitScope = client.getWaitScope();
  
  {
    std::cout << "Executing a add ... " << std::endl;
    std::cout.flush();

    auto add_request = client_inst.addRequest();
    auto add_params = add_request.initParams(2);
    add_params.set(0, 23.00);
    add_params.set(1, 32.00);
    auto add_promise = add_request.send();
    auto add_response = add_promise.wait(waitScope);
    KJ_ASSERT(add_response.getValue() == 55.00);

    std::cout << "add result:" << add_response.getValue() << std::endl;

    std::string input_string("Jim;Jack;Jason;John;Jackin");
    std::cout << "Executing a split ... " << std::endl;
    std::cout << "Input string:" << input_string << std::endl;
    std::cout.flush();

    auto split_request = client_inst.splitRequest();
    split_request.setParam(input_string);

    auto split_promise = split_request.send();
    auto split_response = split_promise.wait(waitScope);
    KJ_ASSERT(split_response.getResult().size() == 5);
    KJ_ASSERT(std::string(split_response.getResult()[0].cStr()) == "Jim");
    KJ_ASSERT(std::string(split_response.getResult()[1].cStr()) == "Jack");
    KJ_ASSERT(std::string(split_response.getResult()[2].cStr()) == "Jason");
    KJ_ASSERT(std::string(split_response.getResult()[3].cStr()) == "John");
    KJ_ASSERT(std::string(split_response.getResult()[4].cStr()) == "Jackin");

    std::cout << "split result:" << split_response.getResult().size() << std::endl;
    std::cout << "split result[0]:" << split_response.getResult()[0].cStr() << std::endl;
    std::cout << "split result[1]:" << split_response.getResult()[1].cStr() << std::endl;
    std::cout << "split result[2]:" << split_response.getResult()[2].cStr() << std::endl;
    std::cout << "split result[3]:" << split_response.getResult()[3].cStr() << std::endl;
    std::cout << "split result[4]:" << split_response.getResult()[4].cStr() << std::endl;
  }
}