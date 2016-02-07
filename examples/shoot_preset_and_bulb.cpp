/*
 * Copyright (C) 2016  Marco Gulino <marco@gulinux.net>
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
#include <iostream>
#include "camera.h"
#include "driver.h"
#include "widgets.h"
#include <sstream>
#include <iomanip>
#include <map>
#include "camerafile.h"
#include "serialshooter.h"
#include "gphotowidgetshooter.h"
#include <list>
#include <algorithm>
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
      ss << *widget->get<Widget::DateValue>();
      break;
    case Widget::Widget::String:
      ss << *widget->get<Widget::StringValue>();
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

bool has_option(const list<string> &args, const string &option) {
  return find(begin(args), end(args), option) != end(args);
};

int main(int argc, char **argv) {
  list<string> args(argc-1);
  copy(argv+1, argv+argc, begin(args));
  auto logger = make_shared<Logger>([](const string &message, Logger::Level level){
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
  });

  
  
  GPhoto::Driver driver(logger);
  GPhoto::CameraPtr camera = driver.autodetect();
  if(!camera) {
    cerr << "Error finding camera" << endl;
    return 1;
  }
  cout << "Found camera: " << camera << endl;
  auto settings = camera->settings();
  
  static multimap<string, string> widget_names{{"shutter", "eosremoterelease"}, {"shutter", "bulb"}, {"customfunc", "customfuncex"}, {"exposure", "shutterspeed"}, {"exposure", "shutterspeed2"}, {"exposure", "eos-shutterspeed"}};
  multimap<string, WidgetPtr> widgets_init;
  transform(begin(widget_names), end(widget_names), inserter(widgets_init, end(widgets_init)), [&](const pair<string,string> &p){ return make_pair(p.first,  settings->child_by_name(p.second));});
  map<string, WidgetPtr> widgets;
  copy_if(begin(widgets_init), end(widgets_init), inserter(widgets, end(widgets)), [](const pair<string,WidgetPtr> &p){ return static_cast<bool>(p.second);});
  cout << "Widgets found:\n";
  for(auto w: widgets_init) { if(w.second) cout << w.first << ": " << w.second->name() << endl; }
  cout << "Selected widgets:\n";
  for(auto w: widgets) { cout << w.first << ": " << w.second->name() << endl; }


  ShooterPtr shooter = make_shared<SerialShooter>("/dev/ttyUSB0", logger);
  if(widgets.count("shutter")) {
    shooter = widgets["shutter"]->name() == "eosremoterelease" ? ShooterPtr{new EOSRemoteReleaseShutter{camera, logger}} : ShooterPtr{new BulbSettingShutter{camera, logger}};
  }
  
  auto mirror_lock = has_option(args, "-m") ? GPhoto::Camera::MirrorLock{chrono::duration<double>(2), shooter} : GPhoto::Camera::MirrorLock{};
  
  cout << "Using mirror lock (option -m): " << boolalpha << static_cast<bool>(mirror_lock) << endl;
  cout << "Widgets: " << endl;
  for(auto setting: camera->settings()->all_children()) {
    cout << "** " << setting << ", value: " << value2string(setting) << endl;
  }
  
  
  if(!widgets.count("exposure")) {
    cout << "Error! unable to find <shutterspeed> settings widget\n";
    return 1;
  }
  
  auto shoot = [&](const string &speed, function<future<CameraFilePtr>()> shoot_f){
    widgets["exposure"]->get<Widget::MenuValue>()->set(speed);
    camera->save_settings();
    auto file = shoot_f();
    file.wait();
    CameraFilePtr cf = file.get();
    cerr << *cf << endl;
  };
 
  shoot("5", [&]{ return camera->shoot_preset(mirror_lock); });
  this_thread::sleep_for(chrono::seconds(2));
  shoot("Bulb", [&]{ return camera->shoot_bulb(chrono::duration<double>(45), shooter, mirror_lock); });
  

  return 0;
};