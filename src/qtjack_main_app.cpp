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

#include <QtWidgets/QApplication>
#include <signal.h>
#include <unistd.h>
#include "qtjack_mainwindow.hpp"

void catchUnixSignal(int quitSignal) {
  auto handler = [](int sig) -> void {
    printf("quit application. received signal(%d)\n", sig);
    QCoreApplication::quit();
  };

  sigset_t blocking_mask;
  sigemptyset(&blocking_mask);

  sigaddset(&blocking_mask, quitSignal);
  struct sigaction sa;
  sa.sa_handler = handler;
  sa.sa_mask = blocking_mask;
  sa.sa_flags = 0;

  sigaction(quitSignal, &sa, nullptr);
  
}

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  catchUnixSignal(SIGINT);

  QtJackMainWindow main_window;
  main_window.show();
  
  return app.exec();
}
