#ifndef JACKMIDI_HPP 
#define JACKMIDI_HPP

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <math.h>
#include <stdint.h>
#include <assert.h>
#include <functional>
#include <thread>

#include <jack/jack.h>
#include <jack/midiport.h>

typedef union convert_8BytesToInt64{
  int64_t integer;
  unsigned char byte[8];
} convert_8BytesToInt64;

typedef union convert_4BytesToInt32{
  int32_t integer;
  unsigned char byte[4];
} convert_4BytesToInt32;

struct MidiData{
  int midiCommand;
  int midiChannel;
  int midiNummber;
  int midiValue;
};

class JackMidi {
 public:
  JackMidi(std::function<void(std::string)>);
  ~JackMidi();
  int run();
 private:
  int process(jack_nframes_t nframes, void *input_port_raw);
  int64_t rcf_midi_msgs_count;
  jack_client_t *client;
  jack_port_t *input_port;
  bool active;
  std::thread jackMidiThread;
  std::function<void(std::string)> msg_cb_;
};

#endif // JACKMIDI_HPP
