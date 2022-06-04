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

#include <string>
#include <sstream>

#include <QtWidgets>
#include <QString>

#include "messagehistory.hpp"

#include "qtjack_mainwindow.hpp"

QtJackMainWindow::QtJackMainWindow(QWidget *parent)
  : QMainWindow(parent)
  , Processor(_client)
  , mainwindow_ui_(new Ui::MainWindow)
  , message_history_(new MessageHistory(this))
  , started(false)
  , _sample_rate(48000)
{
  Q_INIT_RESOURCE(qt_jack_midi_debugger);
  mainwindow_ui_->setupUi(this);
  setCentralWidget(message_history_);
  mainwindow_ui_->actionStart->setEnabled(true);
  mainwindow_ui_->actionStop->setEnabled(false);
  mainwindow_ui_->actionQuit->setEnabled(true);
  mainwindow_ui_->actionConfigure->setEnabled(true);
  initActionsConnections();
  setupJackClient();
}

void QtJackMainWindow::setupJackClient() {
  // Connect to JACK server
  _client.connectToServer("qtjack_midi_debugger");
  _midi_in = _client.registerMidiInPort("in");
  _sample_rate = _client.sampleRate();
  _client.setMainProcessor(this);
  _client.activate();
}

QtJackMainWindow::~QtJackMainWindow()
{
  delete mainwindow_ui_;
  Q_CLEANUP_RESOURCE(qt_jack_midi_debugger);
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

void QtJackMainWindow::toogleStart() {
  started = true;
  mainwindow_ui_->actionStart->setEnabled(false);
  mainwindow_ui_->actionStop->setEnabled(true);
}

void QtJackMainWindow::toogleStop() {
  started = false;
  mainwindow_ui_->actionStart->setEnabled(true);
  mainwindow_ui_->actionStop->setEnabled(false);
}

void QtJackMainWindow::processMidiMsg(QtJack::MidiMsg new_msg) {
  int type = new_msg.midiData[0] & 0xf0;
  int channel = (new_msg.midiData[0] & 0x0f) + 1;
  int index = new_msg.midiData[1];
  int value = new_msg.midiData[2];
  uint32_t time = new_msg.timestamp;
  double time_in_ms = (time * 1000.0) / _sample_rate;

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

void QtJackMainWindow::initActionsConnections()
{
  connect(mainwindow_ui_->actionQuit, &QAction::triggered, this, &QtJackMainWindow::close);
  connect(mainwindow_ui_->actionStart, &QAction::triggered, this, &QtJackMainWindow::toogleStart);
  connect(mainwindow_ui_->actionStop, &QAction::triggered, this, &QtJackMainWindow::toogleStop);
  connect(mainwindow_ui_->actionClear, &QAction::triggered, message_history_,  &MessageHistory::clear);
  connect(this, &QtJackMainWindow::midiMsgEvent,
          this, &QtJackMainWindow::processMidiMsg);
}

void QtJackMainWindow::process(int samples) {
  int event_count = _midi_in.buffer(samples).numberOfEvents();
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
      }
    }
  }
}
