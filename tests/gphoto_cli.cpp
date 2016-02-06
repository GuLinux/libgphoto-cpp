#include <iostream>
#include "camera.h"
#include "driver.h"
#include "widgets.h"
#include <sstream>
#include <iomanip>
#include <map>
#include "camerafile.h"
using namespace std;
using namespace GPhoto;

string choices2str(const Widget::MenuValue::Choices &l) {
  stringstream ss;
  string sep;
  for(auto c: l) {
    ss << sep << c;
    sep=", ";
  }
  return ss.str();
}

string value2string(const WidgetPtr &widget) {
  stringstream ss;
  switch(widget->type()) {
    case Widget::Toggle:
      ss << boolalpha << *widget->get<Widget::BoolValue>();
      break;
    case Widget::Widget::Date:
      ss << *widget->get<Widget::StringValue>();
      break;
    case Widget::Widget::String:
      ss << widget->get<Widget::StringValue>()->get();
      break;
    case Widget::Menu:
      ss << *widget->get<Widget::MenuValue>() << ", choices: " << choices2str(widget->get<Widget::MenuValue>()->choices());
      break;
    case Widget::Range:
      ss << *widget->get<Widget::RangeValue>();
    default:
      break;
  }
  return ss.str();
}

int main(int argc, char **argv) {
  GPhoto::Driver driver(make_shared<Logger>([](const string &message, Logger::Level level){
    static map<Logger::Level, string> levels {
      {Logger::DEBUG, "DEBUG"},
      {Logger::ERROR, "ERROR"},
      {Logger::INFO, "INFO"},
      {Logger::TRACE, "TRACE"},
      {Logger::WARNING, "WARNING"},
    };
    if(level == Logger::TRACE)
      return;
    cerr << "[" << setfill(' ') << setw(8) << levels[level] << "] " << message << endl;
  }));
  auto camera = driver.autodetect();
  if(!camera) {
    cerr << "Error finding camera" << endl;
    return 1;
  }
  cout << "Found camera: " << camera << endl;
  auto settings = camera->settings();
  cout << "Widgets: " << endl;
  for(auto setting: camera->settings()->all_children()) {
    cout << "** " << setting << ", value: " << value2string(setting) << endl;
  }
  
  auto file = camera->shoot_preset();
  file.wait();
  CameraFilePtr cf = file.get();
  cerr << *cf << endl;
  
  return 0;
};