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

#ifndef GPHOTO_WIDGET_STRING_H
#define GPHOTO_WIDGET_STRING_H
#include "widget.h"
namespace GPhoto {
class Widget::StringValue : public Widget::Value<std::string, char*, char*>{
private:
  friend class Widget;
  StringValue(Widget* widget);
};
}

std::ostream &operator<<(std::ostream &o, const GPhoto::Widget::StringValue &w);
inline std::ostream &operator<<(std::ostream &o, const std::shared_ptr<GPhoto::Widget::StringValue> &w) { return o << *w; }

#endif
