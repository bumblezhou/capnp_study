#include "test.capnp.h"
#include <kj/debug.h>
#include <capnp/ez-rpc.h>
#include <capnp/message.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

class SimpleImpl final : public TestInterface::TestInterface::Server
{
public:
  kj::Promise<void> add(AddContext context)
  {
    auto params = context.getParams().getParams();
    auto result = params[0] + params[1];
    std::cout << "=========================" << std::endl;
    std::cout << "params[0]:" << params[0] << std::endl;
    std::cout << "params[1]:" << params[1] << std::endl;
    std::cout << "result:" << result << std::endl;
    std::cout << "=========================" << std::endl;
    context.getResults().setValue(result);
    return kj::READY_NOW;
  }

  kj::Promise<void> split(SplitContext context)
  {
    auto param = context.getParams().getParam();
    std::string parsed_param(param.cStr());
    std::vector<std::string> raw_result;
    if (parsed_param.size() > 0)
    {
      if (parsed_param.find(';') != std::string::npos)
      {
        std::istringstream f(parsed_param);
        std::string s;
        while (std::getline(f, s, ';'))
        {
          raw_result.push_back(s);
        }
      }
    }

    auto result = context.getResults().initResult(raw_result.size());

    std::cout << "=========================" << std::endl;
    std::cout << "parsed_param:" << parsed_param << std::endl;
    std::cout << "raw_result:" << raw_result.size() << std::endl;
    int i = 0;
    for (auto &s : raw_result)
    {
      result.set(i, s);
      i++;
      // std::cout << s << std::endl;
    }
    std::cout << "=========================" << std::endl;
    return kj::READY_NOW;
  }
};

int main(int argc, const char *argv[])
{
  if (argc != 2)
  {
    std::cerr << "usage: " << argv[0] << " ADDRESS[:PORT]\n"
                                         "Runs the server bound to the given address/port.\n"
                                         "ADDRESS may be '*' to bind to all local addresses.\n"
                                         ":PORT may be omitted to choose a port automatically."
              << std::endl;
    return 1;
  }

  // Set up a server.
  capnp::EzRpcServer server(kj::heap<SimpleImpl>(), argv[1]);

  // Write the port number to stdout, in case it was chosen automatically.
  auto &waitScope = server.getWaitScope();
  uint port = server.getPort().wait(waitScope);
  if (port == 0)
  {
    // The address format "unix:/path/to/socket" opens a unix domain socket,
    // in which case the port will be zero.
    std::cout << "Listening on Unix socket..." << std::endl;
  }
  else
  {
    std::cout << "Listening on port " << port << "..." << std::endl;
  }

  // Run forever, accepting connections and handling requests.
  kj::NEVER_DONE.wait(waitScope);
}