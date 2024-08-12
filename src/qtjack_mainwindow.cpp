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

#include <cstdio>
#include <midievent.h>
#include <qaction.h>
#include <ringbuffer.h>
#include <string>
#include <sstream>
#include <cstring>

#include <QtWidgets>
#include <QString>
#include <QByteArray>

#include "messagehistory.hpp"

#include "qtjack_mainwindow.hpp"

QtJackMainWindow::QtJackMainWindow(QWidget *parent)
  : QMainWindow(parent)
  , Processor(_client)
  , mainwindow_ui_(new Ui::MainWindow)
  , message_history_(new MessageHistory(this))
  , started(false)
  , _sample_rate(48000)
  , check_mouse_clicks(false)
{
  Q_INIT_RESOURCE(qtjack_midi_debugger);
  mainwindow_ui_->setupUi(this);
  mainwindow_ui_->centralVerticalLayout->addWidget(message_history_);
  //setCentralWidget();
  mainwindow_ui_->actionStart->setEnabled(true);
  mainwindow_ui_->actionStop->setEnabled(false);
  mainwindow_ui_->actionQuit->setEnabled(true);
  mainwindow_ui_->actionConfigure->setEnabled(true);

  filter_label_ = new QLabel("Filter:", this);
  mainwindow_ui_->mainToolBar->addWidget(filter_label_);
  filter_in_ = new QLineEdit(this);
  mainwindow_ui_->mainToolBar->addWidget(filter_in_);
  initActionsConnections();
  setupJackClient();
}

void QtJackMainWindow::setupJackClient() {
  // Connect to JACK server
  bool connected = _client.connectToServer("qtjack_midi_debugger");
  if(connected) {
    _midi_in = _client.registerMidiInPort("in");
    _midi_out = _client.registerMidiOutPort("out");
    _sample_rate = _client.sampleRate();
    _client.setMainProcessor(this);
    _client.activate();
    _midi_msg_out_buffer = QtJack::MidiMsgRingBuffer(1024);
    _midi_sys_ex_out_buffer = QtJack::MidiRingBuffer(1024);
  }
  else {
    message_history_->addMessage(QString("Connection to jack server failed"));
    printf("Connection to jack server failed\n");
    emit closed();
  }
}

QtJackMainWindow::~QtJackMainWindow()
{
  delete mainwindow_ui_;
  Q_CLEANUP_RESOURCE(qtjack_midi_debugger);
}

void QtJackMainWindow::closeEvent(QCloseEvent *event) 
{
  QMainWindow::closeEvent(event);
  if (event->isAccepted())
    emit closed();
}

void QtJackMainWindow::resizeEvent(QResizeEvent* event)
{
  QMainWindow::resizeEvent(event);
}

void QtJackMainWindow::close() {
  QMainWindow::close();
}

void QtJackMainWindow::test() {
  message_history_->addMessage(QString("Hello Jack"));
}

void QtJackMainWindow::toggleStart() {
  started = true;
  mainwindow_ui_->actionStart->setEnabled(false);
  mainwindow_ui_->actionStop->setEnabled(true);
}

void QtJackMainWindow::toggleStop() {
  started = false;
  mainwindow_ui_->actionStart->setEnabled(true);
  mainwindow_ui_->actionStop->setEnabled(false);
}

void QtJackMainWindow::toggleCheckMouseClicks() {
  if(check_mouse_clicks) {
    mainwindow_ui_->actionCheckMouseClick->setIcon(QIcon(":/images/toggle_mouse_on.png"));
    message_history_->blockSignals(false);
    check_mouse_clicks = false;
  } else {
    mainwindow_ui_->actionCheckMouseClick->setIcon(QIcon(":/images/toggle_mouse_off.png"));
    message_history_->blockSignals(true);
    check_mouse_clicks = true;
  }
}

void QtJackMainWindow::sendMidiMsg() {
  QString midi_msg_to_send = mainwindow_ui_->sendInput->text();
  QByteArray midi_msg_to_send_bytes = QByteArray::fromHex(midi_msg_to_send.toLatin1());
  int t1 = _client.getJackTime();
  if (midi_msg_to_send_bytes.size() == 3) {
    QtJack::MidiMsg note_msg;

    // e.g.: 0x91,0x69,0x3f  -> 91693f
    note_msg.midiData[0] = midi_msg_to_send_bytes[0]; //0x91;
    note_msg.midiData[1] = midi_msg_to_send_bytes[1]; //0x69;
    note_msg.midiData[2] = midi_msg_to_send_bytes[2]; //0x3f;
    note_msg.length = 3;
    note_msg.timestamp = t1;

    int space = _midi_msg_out_buffer.numberOfElementsCanBeWritten();
    if (space > 1) {
      int written1 = _midi_msg_out_buffer.write(&note_msg, 1);
      // ToDo check if fail
      // if(!written1)
    }
  } else {
     sendSysExMidiMsg(midi_msg_to_send_bytes);
  }
}

void QtJackMainWindow::sendSysExMidiMsg(QByteArray midi_bytes) {
  int t1 = _client.getJackTime();
  QtJack::MidiEvent sys_ex_msg;
  // examples
  //  F0 7F 7F 06 02 F7 -> F07F7F0602F7
  // f00021420000480101f7
  int space = _midi_sys_ex_out_buffer.numberOfElementsCanBeWritten();
  // space is amount of bytes here
  size_t midi_bytes_size = midi_bytes.size();
  if (space > midi_bytes_size) {
    int written1 = _midi_sys_ex_out_buffer.write(reinterpret_cast<unsigned char *>(&midi_bytes_size), sizeof(midi_bytes_size));
    int written2 = _midi_sys_ex_out_buffer.write(reinterpret_cast<unsigned char *>(&t1), sizeof(t1));
    int written3 = _midi_sys_ex_out_buffer.write(reinterpret_cast<unsigned char *>(midi_bytes.data()), midi_bytes.size());
    // ToDo check if fail
    // if(!written1)
  }
}

void QtJackMainWindow::processMidiMsg(QtJack::MidiMsg new_msg) {
  int type = new_msg.midiData[0] & 0xf0;
  int channel = (new_msg.midiData[0] & 0x0f) + 1;
  int index = new_msg.midiData[1];
  int value = new_msg.midiData[2];
  uint32_t time = new_msg.timestamp;
  int time_in_ms = (int)((time * 1000.0) / _sample_rate);

  QString msg_string = "Time: " + QString::number(time_in_ms).rightJustified(10, '0') +" Channel: " + QString::number(channel);
  switch(type) {
    case 0x80:
      msg_string += " Note Off " + QString::number(index) + " Value: " + QString::number(value);
    break;
    case 0x90:
      msg_string += " Note On " + QString::number(index) + " Value: " + QString::number(value);
      break;
    case 0xB0:
      msg_string += " Controller " + QString::number(index) + " Value: " + QString::number(value);
      break;
    case 0xC0:
      msg_string += " Program: " + QString::number(index);
      break;
    case 0xD0:
      msg_string += " Aftertouch: " + QString::number(index);
      break;
    case 0xE0:
      msg_string += " Pitchbend: " + QString::number(index + (value<<7) - 8192);
      break;
  }
  message_history_->addMessage(msg_string);
}

void QtJackMainWindow::processMidiEvent(QtJack::MidiEvent new_event) {
  message_history_->addMessage("SysEx: " + QString(QByteArray(reinterpret_cast<const char *>(new_event.buffer), new_event.size).toHex()));
}

void QtJackMainWindow::initActionsConnections()
{
  connect(mainwindow_ui_->actionQuit, &QAction::triggered, this, &QtJackMainWindow::close);
  connect(mainwindow_ui_->actionStart, &QAction::triggered, this, &QtJackMainWindow::toggleStart);
  connect(mainwindow_ui_->actionStop, &QAction::triggered, this, &QtJackMainWindow::toggleStop);
  connect(mainwindow_ui_->actionClear, &QAction::triggered, message_history_,  &MessageHistory::clear);
  connect(mainwindow_ui_->actionCheckMouseClick, &QAction::triggered, this, &QtJackMainWindow::toggleCheckMouseClicks);
  connect(message_history_, &MessageHistory::onclick,
          [&](){this->message_history_->addMessage("mouse click"); });
  connect(this, &QtJackMainWindow::midiMsgEvent,
          this, &QtJackMainWindow::processMidiMsg);
  connect(this, &QtJackMainWindow::midiEventEvent,
          this, &QtJackMainWindow::processMidiEvent);
  connect(this->mainwindow_ui_->sendButton, &QPushButton::clicked,
          this, &QtJackMainWindow::sendMidiMsg );
}

void QtJackMainWindow::process(int samples) {
  int event_count = _midi_in.buffer(samples).numberOfEvents();
  // read midi data from jack input
  if(started) {
    for (int i = 0;i<event_count;i++) {
      bool ok  = false;
      QtJack::MidiEvent in_event = _midi_in.buffer(samples).readEvent(i, &ok);
      if(in_event.size == 3) {
        // create MidiMsgs and send via Qt signal
        uint32_t timestamp = in_event.time + _client.getJackTime();
        QtJack::MidiMsg new_msg{{in_event.buffer[0],
                                 in_event.buffer[1],
                                 in_event.buffer[2]},
                                 in_event.size,
                                 timestamp};
        emit midiMsgEvent(new_msg);
      } else {
        midiEventEvent(in_event);
      }
    }
  }
  
  // write data to jack output
  QtJack::MidiBuffer outport_buffer = _midi_out.buffer(samples);
  outport_buffer.clearEventBuffer();
  last_frame_time = _client.getJackTime();
  jack_nframes_t first_added_message = 0;
  bool looped = false;
  int elements = _midi_msg_out_buffer.numberOfElementsAvailableForRead();
  // MidiMsg send
  while (elements && !looped) {
    QtJack::MidiMsg ev;
    int read = _midi_msg_out_buffer.read(&ev, 1);
    if(read < 1) continue;
    if (first_added_message == ev.timestamp) {
      //first element wich was written back for future use
      looped = true;
      // need to write it back:
      int space = _midi_msg_out_buffer.numberOfElementsCanBeWritten();
      if (space) {
        int written = _midi_msg_out_buffer.write(&ev, 1);
      }
      break;
    }
    int t = ev.timestamp + samples - last_frame_time;
    if (t >= static_cast<int>(samples)) { // write back for future use
      if(!first_added_message)
        first_added_message = ev.timestamp;
      int space = _midi_msg_out_buffer.numberOfElementsCanBeWritten();
      if (space) {
        int written = _midi_msg_out_buffer.write(&ev, 1);
        // ToDo check if fail
        // if(!written)
      }
    } else {
      if (t < 0)
        t = 0; // maybe we missed a cirlce because of xrun
      QtJack::MidiData *mididata = outport_buffer.reserveEvent(t, ev.length);
      if (mididata)
        memcpy(mididata, ev.midiData, ev.length);
      // check if midi msgs was sent
      // if(!midi_was_sent)
    }
    elements = _midi_msg_out_buffer.numberOfElementsAvailableForRead();
  }

  // TODO MidiData send (SysEx)
  static unsigned char sys_ex_buffer[16];
  std::memset(sys_ex_buffer,0,16);
  jack_nframes_t first_added_sys_ex_message = 0;
  bool sys_ex_looped = false;
  int sys_ex_elements = _midi_sys_ex_out_buffer.numberOfElementsAvailableForRead();
  while (sys_ex_elements && !sys_ex_looped) {
    QtJack::MidiEvent ev;
    // read size
    int read_size = _midi_sys_ex_out_buffer.read(reinterpret_cast<unsigned char *>(&(ev.size)), sizeof(ev.size));
    if(read_size < 1) continue;
    // read time
    int read_time = _midi_sys_ex_out_buffer.read(reinterpret_cast<unsigned char *>(&(ev.time)), sizeof(ev.time));
    if(read_time < 1) continue;
    int read_data = _midi_sys_ex_out_buffer.read(sys_ex_buffer, ev.size);
    if(read_data < 1) continue;
    if (ev.size < 16) {
      if (first_added_sys_ex_message == ev.time) {
        //first element wich was written back for future use
        sys_ex_looped = true;
        // need to write it back:
        int space = _midi_sys_ex_out_buffer.numberOfElementsCanBeWritten();
        if (space >= ev.size + sizeof(ev.size) + sizeof(ev.time)) {
          int written = _midi_sys_ex_out_buffer.write(reinterpret_cast<unsigned char *>(&(ev.size)), sizeof(ev.size));
          written += _midi_sys_ex_out_buffer.write(reinterpret_cast<unsigned char *>(&(ev.time)), sizeof(ev.time));
          written += _midi_sys_ex_out_buffer.write(sys_ex_buffer, ev.size);
        }
        break;
      }
      int t = ev.time + samples - last_frame_time;
      if (t >= static_cast<int>(samples)) { // write back for future use
        if(!first_added_sys_ex_message)
          first_added_sys_ex_message = ev.time;
        int space = _midi_sys_ex_out_buffer.numberOfElementsCanBeWritten();
        if (space >= ev.size + sizeof(ev.size) + sizeof(ev.time)) {
          int written = _midi_sys_ex_out_buffer.write(reinterpret_cast<unsigned char *>(&(ev.size)), sizeof(ev.size));
          written += _midi_sys_ex_out_buffer.write(reinterpret_cast<unsigned char *>(&(ev.time)), sizeof(ev.time));
          written += _midi_sys_ex_out_buffer.write(sys_ex_buffer, ev.size);
          // ToDo check if fail
          // if(!written)
        }
      } else {
        if (t < 0)
          t = 0; // maybe we missed a cirlce because of xrun
        QtJack::MidiData *mididata = outport_buffer.reserveEvent(t, ev.size);
        if (mididata)
          memcpy(mididata, sys_ex_buffer, ev.size);
        // check if midi msgs was sent
        // if(!midi_was_sent)
      }
      sys_ex_elements = _midi_sys_ex_out_buffer.numberOfElementsAvailableForRead();
    }
  }
}
