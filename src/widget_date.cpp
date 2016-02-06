#include "widgets.h"
#include "widget_p.h"

using namespace GPhoto;
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
