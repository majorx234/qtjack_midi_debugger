/*
 * This file is part of the qtjack_midi_debugger distribution (https://github.com/majorx234/qtjack_midi_debugger ).
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
#include <QLabel>
#include <QLineEdit>

#include "ui_mainwindow.h"
#include "messagehistory.hpp"

// QtJack includes
#include <Client>
#include <Processor>
#include <RingBuffer>
#include <MidiMsg>
#include <midievent.h>

namespace Ui {
class MainWindow;
}

class QtJackMainWindow : public QMainWindow, public QtJack::Processor  {
  Q_OBJECT

 public:
  explicit QtJackMainWindow(QWidget *parent = nullptr);
  ~QtJackMainWindow();
  void msg_history_cb(std::string str);
  void process(int samples) override;

 signals:
  void closed();
  void midiMsgEvent(QtJack::MidiMsg);
  void midiEventEvent(QtJack::MidiEvent);

 protected:
  void closeEvent(QCloseEvent *event) override;
  void resizeEvent(QResizeEvent* event) override;

 private slots:
  void close();
  void test();
  void toggleStart();
  void toggleStop();
  void toggleCheckMouseClicks();
  void processMidiMsg(QtJack::MidiMsg new_msg);
  void processMidiEvent(QtJack::MidiEvent new_event);
  void sendMidiMsg();
  void sendSysExMidiMsg(QByteArray midi_bytes);

 private:
  void setupJackClient();
  void initActionsConnections();
  Ui::MainWindow *mainwindow_ui_ = nullptr;
  MessageHistory *message_history_ = nullptr;
  QLabel* filter_label_;
  QLineEdit* filter_in_;
  QtJack::Client _client;
  QtJack::MidiPort _midi_in;
  QtJack::MidiPort _midi_out;
  QtJack::MidiBuffer* _midi_in_buffer;
  QtJack::MidiMsgRingBuffer _midi_msg_out_buffer;
  QtJack::MidiRingBuffer _midi_sys_ex_out_buffer;
  std::atomic_bool started;
  unsigned int _sample_rate;
  int last_frame;
  int last_frame_time;
  bool check_mouse_clicks;
};

#endif // QTJACK_MAIN_WINDOW_HPP
