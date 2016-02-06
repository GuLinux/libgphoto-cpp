#include "widgets.h"
#include "widget_p.h"
using namespace GPhoto;
using namespace std;

Widget::RangeValue::RangeValue(Widget* widget): Value< float, float, float* >(widget, [](float &f) {return &f; })
{
  widget->d->gphoto << GP2_RUN(this, widget) { return gp_widget_get_range(widget->d->widget, &_range.min, &_range.max, &_range.increment); };
}


Widget::RangeValue::Range Widget::RangeValue::range() const
{
  return *this;
}


bool Widget::RangeValue::Range::operator==(const Widget::RangeValue::Range& o) const
{
  return o.min == min && o.max == max && o.increment == increment;
}

Widget::RangeValue::operator Range() const
{
  return _range;
}
