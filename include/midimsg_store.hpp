#ifndef MIDIMSG_STORE_HPP
#define MIDIMSG_STORE_HPP

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
  QList<QtJack::MidiMsg> midi_msg_list;
};

#endif // MIDIMSG_STORE_HPP
