#ifndef QTJACK_MAIN_WINDOW_HPP 
#define QTJACK_MAIN_WINDOW_HPP

#include <atomic>

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"
#include "messagehistory.hpp"
#include "RtMidi.h"

// QtJack includes
#include <Client>
#include <Processor>
#include <RingBuffer>
#include <MidiMsg>

namespace Ui {
class MainWindow;
}

class QtJackMainWindow : public QMainWindow, public QtJack::Processor  {
  Q_OBJECT

 public:
  explicit QtJackMainWindow(QWidget *parent = nullptr);
  ~QtJackMainWindow();
  void msg_history_cb(std::string str);
  void process(int samples);
 
 signals:
  void closed();
  void midiMsgEvent(QtJack::MidiMsg);

 protected:
  void closeEvent(QCloseEvent *event) override;
  void resizeEvent(QResizeEvent* event) override;

 private slots:
  void close();
  void test();
  void toogleStart();
  void toogleStop();
  void processMidiMsg(QtJack::MidiMsg new_msg);

 private:
  void setupJackClient();
  void initActionsConnections();
  Ui::MainWindow *mainwindow_ui_ = nullptr;
  MessageHistory *message_history_ = nullptr;
  QtJack::Client _client;
  QtJack::MidiPort _midi_in;
  QtJack::MidiBuffer* _midi_in_buffer;
  std::atomic_bool started;
};        

#endif // QTJACK_MAIN_WINDOW_HPP
