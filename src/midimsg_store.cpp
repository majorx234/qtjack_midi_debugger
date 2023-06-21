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
    midi_event_csv_file << midi_event.length;
    //ToDo; write Midi Meessage in CSV
  }
  midi_event_csv_file.close();
}
