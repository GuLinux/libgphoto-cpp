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
#include <map>
#include "widgets/widgets.h"
#include "backend/exceptions.h"
#include "c++/containers_streams.h"
#include <cmath>
using namespace GPhotoCPP;
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
  d->values = GuLinux::make_stream(widget->get<Widget::MenuValue>()->choices())
    .transform<list<Value>>([](const Widget::MenuValue::Choice &c){ return Value{c.text}; });
}

Exposure::~Exposure()
{
}

Exposure::Values Exposure::values() const
{
  return d->values;
}

bool Exposure::Value::bulb() const
{
  string s;
  transform(begin(text), end(text), back_inserter(s), ::tolower);
  return s.find("bulb") != string::npos;
}

milliseconds Exposure::Value::duration() const
{
  if(bulb())
    return milliseconds{-1};
  stringstream ss{text};
  if(text.find('/') != string::npos) {
    double num, den;
    char sep;
    ss >> num >> sep >> den;
    return milliseconds{num*1000./den};
  };
  double d;
  ss >> d;
  return milliseconds{d*1000.};
}

Exposure::Value Exposure::value() const
{
  return Value{ *d->widget->get<Widget::MenuValue>() };
}

void Exposure::set(const Exposure::Value& value)
{
  d->widget->get<Widget::MenuValue>()->set(value.text);
}

void Exposure::set_bulb()
{
  static map<string, int> ordering{{"bulb", 10}, {"Bulb", 9}};
  vector<Value> bulb_choices = GuLinux::make_stream(d->values)
    .transform<vector<Value>>(GuLinux::identity<Value>{})
    .filter([&](const Value &v){ return v.bulb(); })
    .sorted([&](const Value &v1, const Value &v2){ return ordering[v1.text] > ordering[v2.text]; } );

  if(bulb_choices.size() == 0)
    throw ValueError{"Unable to find bulb exposure"};
  set(bulb_choices[0]);
}


void Exposure::set(const milliseconds& duration, double tolerance)
{
  auto difference = [](const milliseconds &a, const milliseconds &b) -> double { return abs((a-b).count()); };
  
  vector<Value> values = GuLinux::make_stream(d->values)
    .transform<vector<Value>>(GuLinux::identity<Value>{})
    .remove([](const Value &v){ return v.bulb(); })
    .sorted([&](const Value &a, const Value &b) { return difference(a.duration(), duration) < difference(b.duration(), duration); });
    
  auto tolerance_value = duration.count()* tolerance;
  if(difference(values[0].duration(), duration) > tolerance_value) {
    stringstream ss;
    ss << "Unable to find a widget for exposure of " << duration.count() << " milliseconds (" << tolerance_value << "ms of tolerance)";
    throw ValueError{ss.str()};
  }
  set(values[0]);
}

