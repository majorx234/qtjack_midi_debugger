#ifndef MIDI_MSG_HPP
#define MIDI_MSG_HPP

#include <QList>
#include <MidiMsg>

class MidiMsgStore {
 public:
  MidiMsgStore();
  ~MidiMsgStore();

  void add_midi_msg(QtJack::MidiMsg msg);
  void reset();
  void save_as_midi_file(std::string path);
 private:
  Qlist<QtJack::MidiMsg> midi_msg_list;
}

#endif // MIDI_MSG_HPP
