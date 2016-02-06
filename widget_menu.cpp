#include "widgets.h"
#include "widget_p.h"

using namespace GPhoto;
using namespace std;

Widget::MenuValue::MenuValue(Widget* widget): Value< string, char* >(widget, [](const string &s) -> char*{ return s.c_str(); })
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
