#ifndef QTJACK_MAIN_WINDOW_HPP 
#define QTJACK_MAIN_WINDOW_HPP

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"
#include "messagehistory.hpp"
#include "RtMidi.h"

// QtJack includes
#include <Client>
#include <Processor>
#include <RingBuffer>

namespace Ui {
class MainWindow;
}

class QtJackMainWindow : public QMainWindow, public QtJack::Processor  {
  Q_OBJECT

 public:
  explicit QtJackMainWindow(QWidget *parent = nullptr);
  ~QtJackMainWindow();
  void msg_history_cb(std::string str);
 
 signals:
  void closed();  

 protected:
  void closeEvent(QCloseEvent *event) override;
  void resizeEvent(QResizeEvent* event) override;

 private slots:
  void close();
  void test();

 private:
  void initActionsConnections();
  Ui::MainWindow *mainwindow_ui_ = nullptr;
  MessageHistory *message_history_ = nullptr;
  QtJack::Client _client;
  QtJack::MidiPort in;
};        

#endif // QTJACK_MAIN_WINDOW_HPP
