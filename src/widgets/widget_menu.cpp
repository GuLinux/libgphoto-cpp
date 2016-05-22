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
#include <algorithm>

using namespace GPhotoCPP;
using namespace std;


Widget::MenuValue::MenuValue(const WidgetPtr& widget): StringValue(widget)
{
  int choices = (*widget->d->gphoto)(GP2_RUN(this, &widget) { GPRET(gp_widget_count_choices(widget->d->widget)) });
  for(int i=0; i<choices; i++) {
    const char *choice;
    widget->d->gphoto << GP2_RUN(this, &widget, &choice, &i) { GPRET(gp_widget_get_choice(widget->d->widget, i, &choice)) };
    _choices.push_back({string{choice}});
  }
}


bool Widget::MenuValue::Choice::operator==(const Widget::MenuValue::Choice& o) const
{
  return o.text == text;
}
bool Widget::MenuValue::Choice::operator==(const string& s) const
{
  return s == text;
}


Widget::MenuValue::Choices Widget::MenuValue::choices() const
{
  return _choices;
}

ostream& operator<<(ostream& s, const Widget::MenuValue::Choice& c)
{
  return s << c.text;
}

ostream& operator<<(ostream& o, const Widget::MenuValue& w)
{
   return o << w.get() << ", choices: " << w.choices();
}

ostream& operator<<(ostream& s, const vector< Widget::MenuValue::Choice >& c)
{
  string sep;
  for(auto choice: c) {
    s << sep << choice;
    sep = ", ";
  }
  return s;
}


Widget::MenuValue::Choice::operator string() const
{
  return text;
}

void Widget::MenuValue::set(int choice_index)
{
  set(choices()[choice_index]);
}

void Widget::MenuValue::set(const string& choice_text)
{
  Widget::StringValue::set(choice_text);
}


void Widget::MenuValue::set(const Widget::MenuValue::Choice& choice)
{
  set(choice.text);
}


Widget::MenuValue::Choices Widget::MenuValue::filter_choices(const string& text, bool case_sensitive, bool partial) const
{
  Choices filtered;
  auto case_check = [&](const string &s) {
    if(case_sensitive)
      return s;
    string cs;
    transform(begin(s), end(s), back_inserter(cs), ::tolower);
    return cs;
  };
  auto matcher = [&](const string &a, const string &b){
    return partial ? a.find(b) != string::npos : a == b;
  };
 auto filter = [&](const Choice &choice){ return matcher(case_check(choice.text), case_check(text)); };
 copy_if(begin(_choices), end(_choices), back_inserter(filtered), filter);
 return filtered;
}

