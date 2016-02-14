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
 * 
 */

#include "exposure.h"
#include <algorithm>
#include "widgets/widgets.h"

using namespace GPhoto;
using namespace std;

class Exposure::Private {
public:
  Private(const WidgetPtr& widget, Exposure* q);
  const WidgetPtr widget;
  list<Value> values;
private:
  Exposure *q;
};

Exposure::Private::Private(const WidgetPtr& widget, Exposure* q) : widget{widget}, q(q)
{
}

Exposure::Exposure(const WidgetPtr& widget)
 : dptr(widget, this)
{
  auto choices = widget->get<Widget::MenuValue>()->choices();
  transform(begin(choices), end(choices), back_inserter(d->values), [](const Widget::MenuValue::Choice &c){ return Value{c.text}; });
}

Exposure::~Exposure()
{
}

list< Exposure::Value > Exposure::values() const
{
  return d->values;
}

bool Exposure::Value::bulb() const
{

}

std::chrono::duration< double > Exposure::Value::duration() const
{

}


