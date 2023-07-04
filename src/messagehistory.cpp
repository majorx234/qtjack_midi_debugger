/* 
 * This file is part of the rtaudio_example distribution (https://github.com/majorx234/qtjack_midi_debugger ).
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

#include "messagehistory.hpp"

#include <iostream>
#include <sstream>

#include <QScrollBar>
#include <QEvent>
#include <QObject>


MessageHistory::MessageHistory(QWidget *parent) 
  : QPlainTextEdit(parent)
  , record_in_history(false)
  , begin(std::chrono::steady_clock::now())
{
    document()->setMaximumBlockCount(1000);
    QPalette p = palette();
    p.setColor(QPalette::Base, Qt::black);
    p.setColor(QPalette::Text, Qt::green);
    setPalette(p);
    this->viewport()->installEventFilter(this);
}

void MessageHistory::addMessage(const QString msg)
{
    std::chrono::steady_clock::time_point msg_time = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(msg_time - begin).count();
                    
    moveCursor(QTextCursor::End);
    QString format_msg = QString("%1 - %2").arg(QString::number(duration).rightJustified(8, '0'), msg);
    appendPlainText(format_msg);
    if(record_in_history)
      history.append(msg);
    QScrollBar *bar = verticalScrollBar();
    bar->setValue(bar->maximum());
}

bool MessageHistory::eventFilter(QObject *object, QEvent *event)
{
  if (event->type() == QEvent::MouseButtonPress) {
    QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
    emit onclick();
    qDebug("Mouse button pressed");
    return true;
  } else {
    // standard event processing
    return QObject::eventFilter(object, event);
  }
}
