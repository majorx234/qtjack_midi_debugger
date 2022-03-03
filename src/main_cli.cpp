#include <string>
#include <iostream>
#include <chrono>
#include "jackmidi.hpp"


void print_jack_event(std::string msg) {
  std::cout << "test" << std::endl;
}

int main(int narg, char **args) {
  std::function<void(std::string)> jack_cb = [](std::string in) {std::cout << in << std::endl;};
  JackMidi* client = new JackMidi(jack_cb);
  int i = 0;
  while (i < 100000) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    i++;
  }
  print_jack_event("end");
  delete client;
  return 0;
}
