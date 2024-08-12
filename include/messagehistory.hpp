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

#ifndef MESSAGE_HISTORY_HPP_
#define MESSAGE_HISTORY_HPP_

#include <chrono>
#include <QPlainTextEdit>
#include <QList>

class MessageHistory : public QPlainTextEdit
{
  Q_OBJECT
signals:
  void getData(const QByteArray &data);
  void onclick();
 public:
  explicit MessageHistory(QWidget *parent = nullptr);
  void setLocalEchoEnabled(bool set);
 public slots:
  void addMessage(const QString msg);
  int addFilter(const QString filter);
  QList<int> addFilterList(const QString filter);
  void clearAllFilter();
  bool deleteFilter(int filter_index);

 private:
  bool eventFilter(QObject *object, QEvent *event);
  QList<QString> history;
  QMap<int, QString> filter_map;
  int filter_count;
  bool record_in_history;
  std::chrono::steady_clock::time_point begin;
};

#endif // MESSAGE_HISTORY_HPP_
