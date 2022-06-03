/* 
 * This file is part of the rtaudio_example distribution (https://github.com/majorx234/qt_jack_midi_debugger ).
 * Copyright (c) 2021-2022 Majorx234 <majorx234@googlemail.com>
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef QTJACK_MAIN_WINDOW_HPP 
#define QTJACK_MAIN_WINDOW_HPP

#include <atomic>

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"
#include "messagehistory.hpp"

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
  unsigned int _sample_rate;
};        

#endif // QTJACK_MAIN_WINDOW_HPP
