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

#ifndef MIDIMSG_STORE_HPP
#define MIDIMSG_STORE_HPP

#include <QList>
#include <MidiMsg>

class MidiMsgStore {
 public:
  MidiMsgStore();
  ~MidiMsgStore();

  void add_midi_msg(QtJack::MidiMsg msg);
  void reset();
  void save_as_midi_file(std::string path);
  void save_as_csv_file(std::string path);
 private:
  QList<QtJack::MidiMsg> midi_msg_list;
};

#endif // MIDIMSG_STORE_HPP
