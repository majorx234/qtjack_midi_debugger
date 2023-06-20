#include <algorithm>
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
  Qtjack::MidiMsg first = midi_msg_list.first();
  // ToDo: fix time stamps of midievents in new midifile
}
