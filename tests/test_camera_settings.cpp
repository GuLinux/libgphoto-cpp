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
#include "camera.h"
#include "camera_settings.h"
#include "backend/gphoto_wrapper.h"
#include "widgets/widgets.h"
#include "shooter.h"
#include <gphotowidgetshooter.h>
#include <exposure.h>
#include <serialshooter.h>

using namespace std;
using namespace GPhotoCPP;
namespace GPhotoCPP {
class FakeCamera : public GPhotoCPP::iCamera {
public:
  FakeCamera(const WidgetPtr &settings) : _settings{settings} {}
  virtual WidgetPtr widgets_settings() const { return _settings; }
  virtual void save_settings() {}
  WidgetPtr _settings;
};

class CameraSettingsTest : public ::testing::Test{
public:
  CameraSettingsTest();
  Camera::SettingsPtr make_settings();
  GPhotoWrapperPtr gphoto;
  WidgetPtr _settings;
  CameraWidget *top_window;
  CameraSettingsTest *add_menu(const string &name, const list<string> &choices, const string &value = {});
};
}

CameraSettingsTest::CameraSettingsTest() : gphoto{new GPhotoWrapper}
{
  gphoto << GP2_RUN(&) { GPRET(gp_widget_new(GP_WIDGET_WINDOW, "Top Window", &top_window)) }
	 << GP2_RUN(&) { GPRET(gp_widget_set_name(top_window, "top_window")) }
	 << GP2_RUN(&) { GPRET(gp_widget_set_readonly(top_window, 0)) };
  _settings = make_shared<Widget>(top_window, gphoto, LoggerPtr{});
}

CameraSettingsTest* CameraSettingsTest::add_menu(const string& name, const list< string >& choices, const string& value)
{
  CameraWidget *menu_widget;
  gphoto
	 << GP2_RUN(&) { GPRET(gp_widget_new(GP_WIDGET_MENU, name.c_str(), &menu_widget)) }
	 << GP2_RUN(&) { GPRET(gp_widget_set_name(menu_widget, name.c_str())) };
  for(auto choice: choices)
	 gphoto << GP2_RUN(&) { GPRET(gp_widget_add_choice(menu_widget, choice.c_str() )) };
  string new_value = value.empty() ? choices.front() : value;
  gphoto
	 << GP2_RUN(&) { GPRET(gp_widget_set_value(menu_widget, new_value.c_str())) }
	 << GP2_RUN(&) { GPRET(gp_widget_append(top_window, menu_widget)) }
	 << GP2_RUN(&) { GPRET(gp_widget_set_readonly(menu_widget, 0)) };
  return this;
}



TEST_F(CameraSettingsTest, test_widgets_set_1) {
  add_menu("eosremoterelease", {"shoot", "release"})->add_menu("shutterspeed", {"2"})->add_menu("imageformat", {"format1", "format2"})->add_menu("iso", {"100", "200", "400"});
  iCamera::ptr camera{new FakeCamera{_settings}};
  GPhotoCPP::Camera::Settings settings{camera, {}};
  ASSERT_EQ(typeid(EOSRemoteReleaseShutter), typeid(*settings.shooter().get() ));
  ASSERT_EQ("format1", settings.format());
  vector<string> expected_format_choices{"format1", "format2"};
  ASSERT_EQ(expected_format_choices, settings.format_choices());
  ASSERT_EQ("100", settings.iso());
  vector<string> expected_iso_choices{"100", "200", "400"};
  ASSERT_EQ(expected_iso_choices, settings.iso_choices());
  ASSERT_EQ(2000, settings.exposure()->value().duration().count());
}


TEST_F(CameraSettingsTest, test_widgets_set_2) {
  add_menu("bulb", {"shoot", "release"})->add_menu("shutterspeed2", {"1"})->add_menu("imageformat", {"format1", "format2"}, "format2")->add_menu("iso", {"100", "200", "400"}, "200");
  iCamera::ptr camera{new FakeCamera{_settings}};
  GPhotoCPP::Camera::Settings settings{camera, {}};
  ASSERT_EQ(typeid(BulbSettingShutter), typeid(*settings.shooter().get() ));
  ASSERT_EQ("format2", settings.format());
  vector<string> expected_format_choices{"format1", "format2"};
  ASSERT_EQ(expected_format_choices, settings.format_choices());
  ASSERT_EQ("200", settings.iso());
  vector<string> expected_iso_choices{"100", "200", "400"};
  ASSERT_EQ(expected_iso_choices, settings.iso_choices());
  ASSERT_EQ(1000, settings.exposure()->value().duration().count());
}

TEST_F(CameraSettingsTest, test_widgets_set_3) {
  add_menu("shutterspeed2", {"1/500"})->add_menu("imageformat", {"format1", "format2"}, "format2")->add_menu("iso", {"100", "200", "400"}, "400");
  iCamera::ptr camera{new FakeCamera{_settings}};
  GPhotoCPP::Camera::Settings settings{camera, {}};
  ASSERT_FALSE(settings.shooter());
  settings.set_serial_port("/dev/ttyUSB0");
  ASSERT_EQ(typeid(SerialShooter), typeid(*settings.shooter().get() ));
  ASSERT_EQ("format2", settings.format());
  vector<string> expected_format_choices{"format1", "format2"};
  ASSERT_EQ(expected_format_choices, settings.format_choices());
  ASSERT_EQ("400", settings.iso());
  vector<string> expected_iso_choices{"100", "200", "400"};
  ASSERT_EQ(expected_iso_choices, settings.iso_choices());
  ASSERT_EQ(2, settings.exposure()->value().duration().count());
}