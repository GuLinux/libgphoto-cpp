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
#include "gtest/gtest.h"
#include "backend/gphoto_wrapper.h"
#include "widgets/widgets.h"
#include <exposure.h>
#include <gphoto2/gphoto2.h>
#include <ctime>
#include <algorithm>
#include <iomanip>
#include "backend/exceptions.h"
using namespace std;
using namespace GPhotoCPP;
namespace {
  class TestExposureParsing : public ::testing::Test {
  public:
    struct Parsed {
      milliseconds duration;
      bool bulb;
      string text;
      bool operator==(const Parsed &o) const { return duration == o.duration && bulb == o.bulb && text == o.text; }
    };
    TestExposureParsing();
    CameraWidget *top_window;
    CameraWidget *menu_widget;
    void create_widget(const vector< string >& choices, int value_index = 0);
    ExposurePtr exposure;
    GPhotoWrapperPtr gphoto;
    WidgetPtr widget;
  };
}

ostream &operator<<(ostream &o, const TestExposureParsing::Parsed &p) {
  return o << "{" << p.duration.count() << ", bulb: " << boolalpha << p.bulb << ", \"" << p.text << "\"}";
}

TestExposureParsing::TestExposureParsing()
{
  gphoto = make_shared<GPhotoWrapper>();
  gphoto << GP2_RUN(&) { GPRET(gp_widget_new(GP_WIDGET_WINDOW, "Top Window", &top_window)) }
	 << GP2_RUN(&) { GPRET(gp_widget_set_name(top_window, "top_window")) }
  ;
  create_widget({"1/200","3/2", "5", "bulb", "Bulb", "Bulb-Widget"});
}

void TestExposureParsing::create_widget(const vector< string >& choices, int value_index)
{
  gphoto
	 << GP2_RUN(&) { GPRET(gp_widget_new(GP_WIDGET_MENU, "Exposure", &menu_widget)) }
	 << GP2_RUN(&) { GPRET(gp_widget_set_name(menu_widget, "exposure")) }
	 << GP2_RUN(&) { GPRET(gp_widget_append(top_window, menu_widget)) }
	    ;
  for(auto choice: choices) {
    gphoto << GP2_RUN(&) { GPRET(gp_widget_add_choice(menu_widget, choice.c_str()))};
  }
  gphoto << GP2_RUN(&) { GPRET(gp_widget_set_value(menu_widget, choices[value_index].c_str())) };
  widget = make_shared<Widget>(menu_widget, gphoto, LoggerPtr{});
  exposure = make_shared<Exposure>(widget);
}

TEST_F(TestExposureParsing, parseValues) {
  
  list<Parsed> expected{
    {milliseconds{5}, false, "1/200"},
    {milliseconds{1500}, false, "3/2"},
    {milliseconds{5000}, false, "5"},
    {milliseconds{-1}, true, "bulb"},
    {milliseconds{-1}, true, "Bulb"},
    {milliseconds{-1}, true, "Bulb-Widget"},
  };
  list<Parsed> actual;
  auto values = exposure->values();
  transform(begin(values), end(values), back_inserter(actual), [](const Exposure::Value &v){ return Parsed{ v.duration(), v.bulb(), v.text }; });
  ASSERT_EQ(expected, actual);
}

TEST_F(TestExposureParsing, setAndGetValues) {
  ASSERT_EQ("1/200", exposure->value().text);
  exposure->set(Exposure::Value{"3/2"});
  ASSERT_EQ("3/2", exposure->value().text);
}

TEST_F(TestExposureParsing, setBulb) {
  exposure->set_bulb();
  ASSERT_EQ("bulb", exposure->value().text);
}

TEST_F(TestExposureParsing, setBulb_throw_if_no_bulb_widget) {
  gphoto = make_shared<GPhotoWrapper>();
  gphoto << GP2_RUN(&) { GPRET(gp_widget_new(GP_WIDGET_WINDOW, "Top Window", &top_window)) }
	 << GP2_RUN(&) { GPRET(gp_widget_set_name(top_window, "top_window")) }
  ;
  create_widget({"1/200","3/2", "5"});
  try {
    exposure->set_bulb();
    FAIL() << "Expecting ValueError exception";
  } catch(ValueError &e) {
    ASSERT_EQ("Unable to find bulb exposure", string{e.what()});
  }
}

TEST_F(TestExposureParsing, findClosestDuration) {
  exposure->set(milliseconds{5});
  ASSERT_EQ("1/200", exposure->value().text);
  exposure->set(milliseconds{6});
  ASSERT_EQ("1/200", exposure->value().text);
  ASSERT_THROW( exposure->set(milliseconds{6}, 0.01), GPhotoCPP::ValueError);
}

TEST_F(TestExposureParsing, customBulbTextOrder) {
  create_widget({"Bulb","bulb"});
  exposure->set_bulb();
  ASSERT_EQ("bulb", exposure->value().text);
  create_widget({"BulbAAA", "Bulb","bulb"});
  exposure->set_bulb();
  ASSERT_EQ("bulb", exposure->value().text);
}