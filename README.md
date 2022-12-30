# capnp_study
A simple project to demo how to use capnp.

1. Install capnp:
  ```bash
  sudo apt update
  sudo apt install autoconf automake libtool
  
  git clone https://github.com/capnproto/capnproto.git
  cd capnproto/c++
  autoreconf -i
  ./configure
  make -j6 check
  sudo make install
  ```
2. Build this project
  ```bash
  cd capnp_study
  sh ./build.sh
  ```
  
3. Run and test this project
  ```bash
  cd capnp_study
  ./test_server localhost:1024
  ./test_client localhost:1024
  ```
