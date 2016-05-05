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

#include "widget_p.h"
#include <functional>
#include "backend/exceptions.h"

using namespace GPhotoCPP;
using namespace std;


Widget::Private::Private(CameraWidget* widget, const GPhotoWrapperPtr& gphoto, const Logger::ptr& log, Widget* q) : widget{widget}, gphoto{gphoto}, log{log}, q(q)
{
}

Widget::Widget(CameraWidget *widget, const GPhotoCPP::GPhotoWrapperPtr &gphoto, const Logger::ptr &log)
 : dptr(widget, gphoto, log, this)
{
  const char *c_name, *c_label;
  CameraWidgetType c_type;
  int access;
  d->gphoto 
    << GP2_RUN(&) { GPRET(gp_widget_get_id(d->widget, &d->id)) }
    << GP2_RUN(&) { GPRET(gp_widget_get_name(d->widget, &c_name)) }
    << GP2_RUN(&) { GPRET(gp_widget_get_label(d->widget, &c_label)) }
    << GP2_RUN(&) { GPRET(gp_widget_get_type(d->widget, &c_type)) }
    << GP2_RUN(&) { GPRET(gp_widget_get_readonly(d->widget, &access)) }
  ;
  d->name = {c_name};
  d->label = {c_label};
  d->access = access == 0 ? ReadWrite : ReadOnly;
  
  static map<CameraWidgetType, Type> types{
    {GP_WIDGET_WINDOW, Window}, {GP_WIDGET_SECTION, Section}, {GP_WIDGET_TEXT, String}, {GP_WIDGET_RANGE, Range},
    {GP_WIDGET_TOGGLE, Toggle}, {GP_WIDGET_RADIO, Menu}, {GP_WIDGET_MENU, Menu}, {GP_WIDGET_BUTTON, Button}, {GP_WIDGET_DATE, Date},
  };
  d->type = types[c_type];
}

Widget::~Widget()
{
  // TODO: call gp_widget_free? when? only in parent, or in child too?
}

GPhotoCPP::Widgets Widget::children() const
{
  Widgets widgets;
  for(int i=0; i<gp_widget_count_children(d->widget); i++) {
    CameraWidget *widget;
    d->gphoto << GP2_RUN(this, &widget, i) { GPRET(gp_widget_get_child(d->widget, i, &widget)) };
    widgets.push_back(make_shared<Widget>(widget, d->gphoto, d->log));
  }
  return widgets;
}

Widgets Widget::all_children() const
{
  Widgets widgets;
  for(auto w: children()) {
    widgets.push_back(w);
    widgets.merge(w->all_children());
  };
  return widgets;
}


WidgetPtr Widget::Private::find_by(function<GPhotoReturn(::CameraWidget*&)> run, const string &field_name) const
{
    CameraWidget *widget;
    try {
      gphoto << GP2_RUN(this, &widget, run){ return run(widget); };
    return make_shared<Widget>(widget, gphoto, log);
    } catch(GPhotoCPP::Exception &e) {
      lDebug(log) << "unable to find widget by " << field_name << " " << label << ": " << e.what();
      return {};
    }

}


WidgetPtr Widget::child_by_label(const string& label) const
{
    return d->find_by([=](CameraWidget *&widget) { GPRET(gp_widget_get_child_by_label(d->widget, label.c_str(), &widget)) }, "label");
}

WidgetPtr Widget::child_by_name(const string& name) const
{
    return d->find_by([=](CameraWidget *&widget) { GPRET(gp_widget_get_child_by_name(d->widget, name.c_str(), &widget)) }, "name");
}


string Widget::name() const
{
  return d->name;
}

int Widget::id() const
{
  return d->id;
}

string Widget::label() const
{
  return d->label;
}


Widget::operator CameraWidget*() const
{
  return d->widget;
}

Widget::Type Widget::type() const
{
  return d->type;
}

void Widget::get_value(void* value)
{
  d->gphoto << GP2_RUN(this, &value) { GPRET(gp_widget_get_value(d->widget, value)) };
}

void Widget::set_value(void* value)
{
  d->gphoto << GP2_RUN(this, &value) { GPRET(gp_widget_set_value(d->widget, value)) };
}


void Widget::set_value(const shared_ptr< void >& value)
{
  d->gphoto << GP2_RUN(this, &value) { GPRET(gp_widget_set_value(d->widget, value.get())) };
}


bool Widget::operator!=(const Widget& other) const
{
  return ! (other == *this);
}

bool Widget::operator==(const Widget& other) const
{
  return other.id() == id();
}


WidgetPtr Widget::parent() const
{
  CameraWidget *parent;
  try {
    d->gphoto << GP2_RUN(this, &parent) { GPRET(gp_widget_get_parent(d->widget, &parent)) };
    return make_shared<Widget>(parent, d->gphoto, d->log);
  } catch(GPhotoCPP::Exception &e) {
    lTrace(d->log) << "Error getting widget " << name() << " parent: " << e.what();
    return {};
  }
}

string Widget::path() const
{
  list<string> paths;
  auto widget = shared_from_this();
  while(widget) {
    paths.push_front(widget->name());
    widget = widget->parent();
  }
  stringstream s;
  for(auto p: paths)
    s << '/' << p;
  return s.str();
}

#define WidgetP(type) { #type, [](Widget &w, ostream &o) -> ostream&{ return o << w.get<Widget::type ## Value>(); } }
ostream& operator<<(ostream& o, Widget& w)
{
  typedef function<ostream&(Widget&, ostream &)> w2stream;
  static map<Widget::Type, pair<string, w2stream>> types {
    { Widget::String, WidgetP(String) },
    { Widget::Range, WidgetP(Range) },
    { Widget::Toggle, WidgetP(Toggle) },
    { Widget::Button, {"Button", w2stream{}} },
    { Widget::Date, WidgetP(Date) },
    { Widget::Window, {"Window", w2stream{}} },
    { Widget::Section, {"Section", w2stream{}} },
    { Widget::Menu, WidgetP(Menu) },
  };
  auto type = types[w.type()];
  o << w.path() << "{ " << type.first << ": " << w.name();
  if(type.second) {
    o << ", value: ";
    type.second(w,o);
  }
  return o << " }";
}

Widget::Access Widget::access() const
{
  return d->access;
}

