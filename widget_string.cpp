#include "widgets.h"
#include "widget_p.h"

using namespace GPhoto;
using namespace std;

Widget::StringValue::StringValue(Widget* widget): Value< std::string, char* >(widget, [](string &s) -> char*{
  char *c = new char[s.size()+1];
  copy(begin(s), end(s), c);
  c[s.size()] = 0;
  return c;
})
{

}
