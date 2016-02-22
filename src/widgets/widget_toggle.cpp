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

Widget::ToggleValue::ToggleValue(Widget* widget): Value< bool, int, shared_ptr<int>>(widget, [](bool &v) { auto vi = make_shared<int>(v?1:0); return vi; }, [](int v){ return v!=0; })
{
}

ostream& operator<<(ostream& o, const Widget::ToggleValue& w)
{
  return o << boolalpha << w.get();
}
