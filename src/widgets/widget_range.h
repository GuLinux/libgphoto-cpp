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

#ifndef WIDGET_RANGE_H
#define WIDGET_RANGE_H

#include "widget.h"
namespace GPhotoCPP {
class Widget::RangeValue : public Widget::Value<float, float, float*> {
public:
  struct Range {
    float min, max, increment;
    bool operator==(const Range &o) const;
  };
  Range range() const;
  operator Range() const;
private:
  friend class Widget;
  RangeValue(Widget *widget);
  Range _range;
};
}

std::ostream &operator<<(std::ostream &o, const GPhotoCPP::Widget::RangeValue &w);
inline std::ostream &operator<<(std::ostream &o, const std::shared_ptr<GPhotoCPP::Widget::RangeValue> &w) { return o << *w; }

#endif