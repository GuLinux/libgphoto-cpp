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

#include "widgets.h"
#include "widget_p.h"

using namespace GPhoto;
using namespace std;

Widget::MenuValue::MenuValue(Widget* widget): StringValue(widget)
{
  int choices = (*widget->d->gphoto)(GP2_RUN(this, &widget) { return gp_widget_count_choices(widget->d->widget); });
  for(int i=0; i<choices; i++) {
    const char *choice;
    widget->d->gphoto << GP2_RUN(this, &widget, &choice, &i) { return gp_widget_get_choice(widget->d->widget, i, &choice);};
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
