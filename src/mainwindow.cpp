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
#include <QtWidgets>
#include <QString>

#include "messagehistory.hpp"

#include "mainwindow.hpp"


MainWindow::MainWindow(QWidget *parent)
  :QMainWindow(parent)
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

  std::function<void(std::string)> msg_history_cb = std::bind(&MainWindow::msg_history_cb,
                                                               this, std::placeholders::_1);
  jack_midi_interface_ = new JackMidi(msg_history_cb);
  // just for testing
  message_history_->setEnabled(true);
}

MainWindow::~MainWindow()
{
  delete jack_midi_interface_;
  delete mainwindow_ui_;
  Q_CLEANUP_RESOURCE(qt_jack_midi_debugger);
}

void MainWindow::closeEvent(QCloseEvent *event) 
{
   QMainWindow::closeEvent(event);
     if (event->isAccepted())
        emit closed();
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
  QMainWindow::resizeEvent(event);
}

void MainWindow::close() {
  QMainWindow::close();    
}

void MainWindow::test() {
  message_history_->addMessage(QString("Hello Jack"));
}

void MainWindow::msg_history_cb(std::string str) {
  message_history_->addMessage(QString::fromStdString(str));
}

void MainWindow::initActionsConnections()
{
  connect(mainwindow_ui_->actionQuit, &QAction::triggered, this, &MainWindow::close);
  connect(mainwindow_ui_->actionStart, &QAction::triggered, this, &MainWindow::test);
}
