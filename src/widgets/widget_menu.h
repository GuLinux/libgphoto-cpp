/*
 * libgphoto++ - modern c++ wrapper library for gphoto2
 * Copyright (C) 2016 Marco Gulino <marco AT gulinux.net>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef GPHOTO_WIDGET_MENU_H
#define GPHOTO_WIDGET_MENU_H
#include "widget_string.h"
#include <vector>
#include <ostream>

namespace GPhotoCPP {
class Widget::MenuValue : public Widget::StringValue {
public:
  struct Choice {
    std::string text;
    bool operator==(const Choice &o) const;
    bool operator==(const std::string &s) const;
    operator std::string() const;
  };
  typedef std::vector<Choice> Choices;
  Choices choices() const;
  void set(int choice_index);
  void set(const std::string &choice_text);
  void set(const Choice &choice);
  Choices filter_choices(const std::string &text, bool case_sensitive=false, bool partial=false) const;
private:
  friend class Widget;
  MenuValue(Widget* widget);
  Choices _choices;
};
}

std::ostream &operator<<(std::ostream &s, const GPhotoCPP::Widget::MenuValue::Choice &c);
std::ostream &operator<<(std::ostream &s, const std::vector<GPhotoCPP::Widget::MenuValue::Choice> &c);
std::ostream &operator<<(std::ostream &o, const GPhotoCPP::Widget::MenuValue &w);
inline std::ostream &operator<<(std::ostream &o, const std::shared_ptr<GPhotoCPP::Widget::MenuValue> &w) { return o << *w; }
#endif
