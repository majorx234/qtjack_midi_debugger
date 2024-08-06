#include <cstdint>
#include <cstring>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "portsmf/allegro.h"
#include "midimsg_store.hpp"
#include <midimsg_store.hpp>

MidiMsgStore::MidiMsgStore() {}
MidiMsgStore::~MidiMsgStore() {}
void MidiMsgStore::add_midi_msg(QtJack::MidiMsg msg){
  midi_msg_list << msg;
}

void MidiMsgStore::reset() {
  midi_msg_list.clear();
}

void MidiMsgStore::save_as_midi_file(std::string path) {
  // sort after timestamp
  std::sort (midi_msg_list.begin(),
             midi_msg_list.end(),
             [](QtJack::MidiMsg &msg1,QtJack::MidiMsg &msg2){return (msg1.timestamp < msg2.timestamp);} );
  QtJack::MidiMsg first = midi_msg_list.first();
  // ToDo: fix time stamps of midievents in new midifile

  Alg_seq_ptr seq = new Alg_seq;

}

void MidiMsgStore::save_as_csv_file(std::string path) {
  // sort after timestamp
  std::sort (midi_msg_list.begin(),
             midi_msg_list.end(),
             [](QtJack::MidiMsg &msg1,QtJack::MidiMsg &msg2){return (msg1.timestamp < msg2.timestamp);} );
  QtJack::MidiMsg first = midi_msg_list.first();
  // ToDo: fix time stamps of midievents in new midifile
  std::ofstream midi_event_csv_file;
  midi_event_csv_file.open(path);
  for (auto midi_event : midi_msg_list) {
    uint8_t status_byte = midi_event.midiData[0] >> 4;
    uint8_t channel = midi_event.midiData[0] & 0x0f;
    uint8_t param1 = midi_event.midiData[1] & 0x7f;
    uint8_t param2 = midi_event.midiData[2] & 0x7f;
    int norm_time = midi_event.timestamp - first.timestamp;
    midi_event_csv_file << norm_time << "," << status_byte << "," << channel << "," << param1 << "," << param2 << std::endl;
  }
  midi_event_csv_file.close();
}
