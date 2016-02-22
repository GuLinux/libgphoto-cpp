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
  auto choices = widget->get<Widget::MenuValue>()->choices();
  transform(begin(choices), end(choices), back_inserter(d->values), [](const Widget::MenuValue::Choice &c){ return Value{c.text}; });
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
  vector<Value> bulb_choices;
  copy_if(begin(d->values), end(d->values), back_inserter(bulb_choices), [&](const Value &v){ return v.bulb(); });
  if(bulb_choices.size() == 0)
    throw ValueError{"Unable to find bulb exposure"};
  static map<string, int> ordering{{"bulb", 10}, {"Bulb", 9}};
  sort(begin(bulb_choices), end(bulb_choices), [&](const Value &v1, const Value &v2){ return ordering[v1.text] > ordering[v2.text]; });
  set(bulb_choices[0]);
}


void Exposure::set(const milliseconds& duration, double tolerance)
{
  vector<Value> values;
  copy_if(begin(d->values), end(d->values), back_inserter(values), [](const Value &v){ return !v.bulb(); });
  auto difference = [](const milliseconds &a, const milliseconds &b) -> double { return abs(a.count()-b.count()); };
  sort(begin(values), end(values), [&](const Value &a, const Value &b) { return difference(a.duration(), duration) < difference(b.duration(), duration); });
  auto tolerance_value = duration.count()* tolerance;
  if(difference(values[0].duration(), duration) > tolerance_value) {
    stringstream ss;
    ss << "Unable to find a widget for exposure of " << duration.count() << " milliseconds (" << tolerance_value << "ms of tolerance)";
    throw ValueError{ss.str()};
  }
  set(values[0]);
}

