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

#include "widgets.h"
#include "widget_p.h"

using namespace GPhotoCPP;
using namespace std;

DPTR_CLASS(Widget::DateValue) {
public:
};

Widget::DateValue::~DateValue()
{

}


Widget::DateValue::DateValue(Widget* widget)
  : Value< chrono::system_clock::time_point,int,shared_ptr<int> >(widget, 
      [=](chrono::system_clock::time_point &s) -> shared_ptr<int>{ auto i = make_shared<int>(); *i=chrono::system_clock::to_time_t(s); return i; },
      [](int i) -> chrono::system_clock::time_point{ return chrono::system_clock::from_time_t(i); }
  ), dptr()
{

}


ostream& operator<<(ostream& o, const Widget::DateValue& w)
{
  time_t t = chrono::system_clock::to_time_t(w);
  return o << ctime(&t);
}
