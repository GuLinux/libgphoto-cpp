/*
 * Copyright (C) 2016  Marco Gulino <marco@gulinux.net>
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
#include <eosremotereleaseshooter.h>
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
  ShooterPtr shooter = make_shared<SerialShooter>("/dev/ttyUSB0", logger);
  if(static_cast<bool>(settings->child_by_name("eosremoterelease"))) {
    shooter = make_shared<EOSRemoteReleaseShooter>(camera, logger);
    cerr << "Using eosremoterelease shooter" << endl;
  }
  

  auto customfunc = settings->child_by_name("customfuncex");
  cerr << "custom func widget: " << static_cast<bool>(customfunc) << endl;
  
  auto mirror_lock = has_option(args, "-m") ? GPhoto::Camera::MirrorLock{chrono::duration<double>(2), shooter} : GPhoto::Camera::MirrorLock{};
  cout << "Widgets: " << endl;
  for(auto setting: camera->settings()->all_children()) {
    cout << "** " << setting << ", value: " << value2string(setting) << endl;
  }
  
  
  auto shoot = [&](const string &speed, function<future<CameraFilePtr>()> shoot_f){
    settings->child_by_name("shutterspeed")->get<Widget::MenuValue>()->set(speed);
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