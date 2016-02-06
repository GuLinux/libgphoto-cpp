#include "widgets.h"
#include "widget_p.h"

using namespace GPhoto;
using namespace std;

Widget::BoolValue::BoolValue(Widget* widget): Value< bool, int, shared_ptr<int>>(widget, [](bool &v) { auto vi = make_shared<int>(v?1:0); return vi; }, [](int v){ return v!=0; })
{
}
