/*
 *     Driver type: GPhoto Camera INDI Driver
 * 
 *     Copyright (C) 2016 Marco Gulino (marco AT gulinux.net)
 * 
 *     This library is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU Lesser General Public License as published
 *     by the Free Software Foundation; either version 2.1 of the License, or
 *     (at your option) any later version.
 * 
 *     This library is distributed in the hope that it will be useful, but
 *     WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 *     or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 *     License for more details.
 * 
 *     You should have received a copy of the GNU Lesser General Public License
 *     along with this library; if not, write to the Free Software Foundation,
 *     Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 * 
 */

#ifndef GPHOTO_WIDGET_MENU_H
#define GPHOTO_WIDGET_MENU_H
#include "widget.h"
#include <vector>
#include <ostream>

namespace GPhoto {
class Widget::MenuValue : public Widget::Value<std::string, char*, char*>{
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
