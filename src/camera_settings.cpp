#include "camera_settings.h"

/*
 * libgphoto++ - modern c++ wrapper library for gphoto2
 * Copyright (C) 2016 Marco Gulino <marco AT gulinux.net>
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
*/

#include <map>
#include <algorithm>
#include "utils/logger.h"
#include "exposure.h"
#include "widgets/widgets.h"
#include "gphotowidgetshooter.h"
#include "serialshooter.h"

using namespace std;
using namespace GPhotoCPP;
class GPhotoCPP::Camera::Settings::Private {
public:
  Private(const CameraPtr& camera, const LoggerPtr& logger);
  enum Setting { ISO, Format, ShutterSpeed, ShutterControl, };
  const GPhotoCPP::CameraPtr &camera;
  map<Setting, WidgetPtr> widgets;
  ShooterPtr shutterControl;
  ExposurePtr exposure;
  const LoggerPtr logger;
  
  void set_choice(Setting setting, const string &choice);
  vector< string > choices(Setting setting) const;
  string choice_value(Setting setting) const;
private:
};

GPhotoCPP::Camera::Settings::Private::Private(const GPhotoCPP::CameraPtr& camera, const LoggerPtr& logger) : camera{camera}, logger{logger}
{
  widgets.clear();
  static multimap<Setting, string> widget_names{
    {ShutterControl, "eosremoterelease"}, 
    {ShutterControl, "bulb"},
    {ShutterSpeed, "shutterspeed"}, 
    {ShutterSpeed, "shutterspeed2"}, 
    {ShutterSpeed, "eos-shutterspeed"},
    {Format, "imageformat"},
    {ISO, "iso"},
    {ISO, "eos-iso"},
  };
  multimap<Setting, WidgetPtr> widgets_init;
  auto settings = camera->widgets_settings();
  transform(begin(widget_names), end(widget_names), inserter(widgets_init, end(widgets_init)), [&](const pair<Setting,string> &p){ return make_pair(p.first,  settings->child_by_name(p.second));});
  copy_if(begin(widgets_init), end(widgets_init), inserter(widgets, end(widgets)), [](const pair<Setting,WidgetPtr> &p){ return static_cast<bool>(p.second);});
  static map<Setting, string> settings_names { {ShutterControl, "ShutterControl"}, {ShutterSpeed, "ShutterSpeed"}, {Format, "Format"}, {ISO, "ISO"} };
  
  lDebug(logger) << "Main widgets found: ";
  for(auto widget: widgets) lDebug(logger) << settings_names[widget.first] << ": " << widget.second;
  if(widgets[ShutterControl]) {
    shutterControl = widgets[ShutterControl]->name() == "eos-shutterspeed" ? ShooterPtr(new EOSRemoteReleaseShutter{camera, logger}) : ShooterPtr(new BulbSettingShutter{camera, logger});
  }
  exposure = make_shared<Exposure>(widgets[ShutterSpeed]);
}

GPhotoCPP::Camera::Settings::Settings(const GPhotoCPP::CameraPtr& camera, const LoggerPtr& logger) : dptr(camera, logger)
{
}

GPhotoCPP::Camera::Settings::~Settings()
{
}

std::string GPhotoCPP::Camera::Settings::format() const
{
  return d->choice_value(Private::Format);
}


std::string GPhotoCPP::Camera::Settings::iso() const
{
  return d->choice_value(Private::ISO);
}
std::vector< std::string > GPhotoCPP::Camera::Settings::format_choices() const
{
  return d->choices(Private::Format);
}

std::vector< std::string > GPhotoCPP::Camera::Settings::iso_choices() const
{
  return d->choices(Private::ISO);
}

bool GPhotoCPP::Camera::Settings::needs_serial_port() const
{
  return ! d->widgets[Private::ShutterControl];
}

void GPhotoCPP::Camera::Settings::set_format(const std::string& format)
{
  d->set_choice(Private::Format, format);
}

void GPhotoCPP::Camera::Settings::set_iso(const std::string& iso)
{
  d->set_choice(Private::ISO, iso);
}

void GPhotoCPP::Camera::Settings::set_serial_port(const string& port)
{
  d->shutterControl = make_shared<SerialShooter>(port, d->logger);
}


void GPhotoCPP::Camera::Settings::Private::set_choice(GPhotoCPP::Camera::Settings::Private::Setting setting, const string& choice)
{
  if(widgets[setting])
    widgets[setting]->get<Widget::MenuValue>()->set(choice);
  camera->save_settings();
}


vector< string > GPhotoCPP::Camera::Settings::Private::choices(Setting setting) const
{
  if(!widgets.at(setting))
    return {};
  auto choices = widgets.at(setting)->get<Widget::MenuValue>()->choices();
  vector<string> choices_strings(choices.size());
  transform(begin(choices), end(choices), begin(choices_strings), [](const Widget::MenuValue::Choice &c){ return c.text;});
  return choices_strings;
}


string GPhotoCPP::Camera::Settings::Private::choice_value(GPhotoCPP::Camera::Settings::Private::Setting setting) const
{
  if(!widgets.at(setting))
    return {};
  return *widgets.at(setting)->get<Widget::MenuValue>();
}

ExposurePtr GPhotoCPP::Camera::Settings::exposure() const
{
  return d->exposure;
}

ShooterPtr GPhotoCPP::Camera::Settings::shooter() const
{
  return d->shutterControl;
}
