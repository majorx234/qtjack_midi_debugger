/* 
 * This file is part of the rtaudio_example distribution (https://github.com/majorx234/qt_jack_midi_debugger ).
 * Copyright (c) 2021 Majorx234
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

void msg_callback( double deltatime, std::vector< unsigned char > *message, void *userData )
{
  QtJackMainWindow* main_window =  (QtJackMainWindow*)userData;
  std::stringstream midi_msg;
  unsigned int nBytes = message->size();
  for ( unsigned int i=0; i<nBytes; i++ )
    midi_msg << "Byte " << i << " = " << (int)message->at(i) << ", ";
  if ( nBytes > 0 )
    midi_msg << "stamp = " << deltatime; // << std::endl;
  main_window->msg_history_cb(midi_msg.str());
}

QtJackMainWindow::QtJackMainWindow(QWidget *parent)
  :QMainWindow(parent)
  ,Processor(_client)
  ,mainwindow_ui_(new Ui::MainWindow) 
  ,message_history_(new MessageHistory)
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
  //std::function<void(std::string)> msg_history_cb = std::bind(&MainWindow::msg_history_cb,
  //                                                             this, std::placeholders::_1);
}

void QtJackMainWindow::setupJackClient() {
    // Connect to JACK server
    _client.connectToServer("qtjack_midi_debugger");
    _client.registerMidiInPort("in");
    _client.setMainProcessor(this);
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

void QtJackMainWindow::msg_history_cb(std::string str) {
  message_history_->addMessage(QString::fromStdString(str));
}

void QtJackMainWindow::initActionsConnections()
{
  connect(mainwindow_ui_->actionQuit, &QAction::triggered, this, &QtJackMainWindow::close);
  connect(mainwindow_ui_->actionStart, &QAction::triggered, this, &QtJackMainWindow::test);
}

void QtJackMainWindow::process(int samples) {
    // Just shift samples from the ringbuffers to the outputs buffers.
    int event_count = _midi_in.buffer(samples).numberOfEvents();
    printf("number events: %d/n");
}