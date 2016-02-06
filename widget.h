/*
 *     Driver type: GPhoto Camera INDI Driver
 * 
 *     Copyright (C) 2016 Marco Gulino (marco AT gulinux.net)
 * 
 *     This library is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU Lesser General Public License as published
 *     by the Free Software Foundation; either version 2.1 of the License, or
 *     (at your option) any later version.
 * 
 *     This library is distributed in the hope that it will be useful, but
 *     WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 *     or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 *     License for more details.
 * 
 *     You should have received a copy of the GNU Lesser General Public License
 *     along with this library; if not, write to the Free Software Foundation,
 *     Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 * 
 */

#ifndef GPHOTO_WIDGET_H
#define GPHOTO_WIDGET_H
#include "fwd.h"
#include "dptr.h"
#include <list>
#include "logger.h"
namespace GPhoto {

class Widget
{
public:
  Widget(CameraWidget *widget, const GPhoto::GPhotoWrapperPtr &gphoto, const Logger::ptr &log);
  ~Widget();
  class StringValue;
  class RangeValue;
  class MenuValue;
  class BoolValue;
  class ButtonValue;
  class DateValue;
  enum Type {String, Range, Toggle, Button, Date, Window, Section, Menu};
  Widgets children() const;
  
  WidgetPtr child_by_name(const std::string &name) const;
  WidgetPtr child_by_label(const std::string &label) const;
  
  std::string name() const;
    int id() const;
  std::string label() const;
  Type type() const;

  template<typename T> std::shared_ptr<T> get() { return std::shared_ptr<T>{new T(this)}; }
  operator CameraWidget*() const;
  
  template<typename V, typename C = V, typename Cs = C> class Value {
  public:
    typedef std::function<Cs(V &)> V2C;
    typedef std::function<V(C c)> C2V;
    operator V() const { return value; }
    Value &operator=(const V &value) { this->value = value; return *this; }
    void set(const V &value) { *this = value; }
    virtual ~Value() {
      if(original_value != value) {
	Cs c = v2c(value);
	widget->set_value(c);
      }
    }
  protected:
    Value(Widget *widget, V2C v2c= [](V &v){ return v; }, C2V c2v = [](C c){ return V{c};}) : widget{widget}, v2c{v2c} {
      C c;
      widget->get_value(&c);
      value=c2v(c);
      original_value=c2v(c);
    }
    V value;
    V original_value;
    Widget *widget;
    V2C v2c;
  };
  
private:
  void get_value(void* value);
  void set_value(void* value);
  void set_value(const std::shared_ptr<void> &value);
  
    DPTR
};
}


#endif // GPHOTO_WIDGET_H
