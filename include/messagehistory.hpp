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

#ifndef MESSAGE_HISTORY_HPP_
#define MESSAGE_HISTORY_HPP_

#include <QPlainTextEdit>

class MessageHistory : public QPlainTextEdit
{
  Q_OBJECT
signals:
    void getData(const QByteArray &data);
public:
    explicit MessageHistory(QWidget *parent = nullptr);
    void addMessage(const QString msg);
    void setLocalEchoEnabled(bool set); 
};

#endif // MESSAGE_HISTORY_HPP_
