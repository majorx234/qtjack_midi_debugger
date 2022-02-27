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

#include "mainwindow.hpp"

MainWindow::MainWindow(QWidget *parent)
  :QMainWindow(parent)
  ,mainwindow_ui(new Ui::MainWindow) 
//  ,mainwindow_console(new Console)
{

//  setCentralWidget(m_console);   

  mainwindow_ui->setupUi(this);
  mainwindow_ui->actionStart->setEnabled(true);
  mainwindow_ui->actionStop->setEnabled(false);
  mainwindow_ui->actionQuit->setEnabled(true);
  mainwindow_ui->actionConfigure->setEnabled(true);
}

MainWindow::~MainWindow()
{
  delete mainwindow_ui;
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

void MainWindow::initActionsConnections()
{
  connect(mainwindow_ui->actionQuit, &QAction::triggered, this, &MainWindow::close);
}