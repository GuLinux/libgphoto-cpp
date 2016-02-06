/*
 * libgphoto++ - modern c++ wrapper library for gphoto2
 * Copyright (C) 2016 Marco Gulino <marco AT gulinux.net>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef GPHOTO_WIDGET_MENU_H
#define GPHOTO_WIDGET_MENU_H
#include "widget_string.h"
#include <vector>
#include <ostream>

namespace GPhoto {
class Widget::MenuValue : public Widget::StringValue {
public:
  struct Choice {
    std::string text;
    bool operator==(const Choice &o) const;
    bool operator==(const std::string &s) const;
  };
  typedef std::vector<Choice> Choices;
  Choices choices() const;
private:
  friend class Widget;
  MenuValue(Widget* widget);
  Choices _choices;
};
}

std::ostream &operator<<(std::ostream &s, const GPhoto::Widget::MenuValue::Choice &c);

#endif
