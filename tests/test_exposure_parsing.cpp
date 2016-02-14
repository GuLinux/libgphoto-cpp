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
#include <gphoto2/gphoto2.h>
#include <ctime>
#include <algorithm>

using namespace std;
using namespace GPhoto;
namespace {
  class TestExposureParsing : public ::testing::Test {
  public:
    TestExposureParsing();
    GPhotoWrapperPtr gphoto;
    CameraWidget *top_window;
    CameraWidget *menu_widget;
  };
}

TestExposureParsing::TestExposureParsing() : gphoto{new GPhotoWrapper}
{
  gphoto << GP2_RUN(this) { GPRET(gp_widget_new(GP_WIDGET_WINDOW, "Top Window", &top_window)) }
	 << GP2_RUN(this) { GPRET(gp_widget_set_name(top_window, "top_window")) }
	 
	 << GP2_RUN(this) { GPRET(gp_widget_new(GP_WIDGET_MENU, "Exposure", &menu_widget)) }
	 << GP2_RUN(this) { GPRET(gp_widget_set_name(menu_widget, "exposure")) }
	 << GP2_RUN(this) { GPRET(gp_widget_add_choice(menu_widget, "1/200")) }
	 << GP2_RUN(this) { GPRET(gp_widget_add_choice(menu_widget, "3/2")) }
	 << GP2_RUN(this) { GPRET(gp_widget_add_choice(menu_widget, "5")) }
	 << GP2_RUN(this) { GPRET(gp_widget_add_choice(menu_widget, "bulb")) }
	 << GP2_RUN(this) { GPRET(gp_widget_set_value(menu_widget, "Bulb")) }
	 << GP2_RUN(this) { GPRET(gp_widget_set_value(menu_widget, "Bulb-Widget")) }
	 << GP2_RUN(this) { GPRET(gp_widget_append(top_window, menu_widget)) }
	    ;
}