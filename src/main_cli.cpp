#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include <functional>
#include "RtMidi.h"

void print_jack_event(std::string msg) {
  std::cout << msg<< std::endl;
}


void mycallback( double deltatime, std::vector< unsigned char > *message, void *userData )
{
  unsigned int nBytes = message->size();
  for ( unsigned int i=0; i<nBytes; i++ )
    std::cout << "Byte " << i << " = " << (int)message->at(i) << ", ";
  if ( nBytes > 0 )
    std::cout << "stamp = " << deltatime << std::endl;
}

int main(int narg, char **args) {
  std::function<void(std::string)> jack_cb = [](std::string in) {std::cout << in << std::endl;};
  print_jack_event("creat jack obj");

  RtMidiIn *midiin = new RtMidiIn();
  unsigned int nPorts = midiin->getPortCount();
  if ( nPorts == 0 ) {
    std::cout << "No ports available!\n";
    delete midiin;
    return 0;
  }
  midiin->openPort( 0 );

  midiin->setCallback( &mycallback );
  midiin->ignoreTypes( false, false, false );

  int i = 0;
  print_jack_event("start");
  while (i < 20000) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    i++;
  }
  print_jack_event("end");
  delete midiin;
  return 0;
}
