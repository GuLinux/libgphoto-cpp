#include "widget.h"
#include "widget_string.h"
#include "gphoto_wrapper.h"
#include <gphoto2/gphoto2.h>
#include <map>
using namespace GPhoto;
using namespace std;


class Widget::Private {
public:
  Private(CameraWidget* widget, const GPhoto::GPhotoWrapperPtr& gphoto, const Logger::ptr &log, Widget* q);
  CameraWidget* widget;
  GPhotoWrapperPtr gphoto;
  std::string name;
  std::string label;
  int id;
  Type type;
  Logger::ptr log;
  
  WidgetPtr find_by(function<int(::CameraWidget*&)> run, const string &field_name) const;
  
  
private:
  Widget *q;
};
