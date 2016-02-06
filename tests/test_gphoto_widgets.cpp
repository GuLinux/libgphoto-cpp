#include "gtest/gtest.h"
#include "gphoto_wrapper.h"
#include "widgets.h"
#include <gphoto2/gphoto2.h>
#include <ctime>

using namespace std;
using namespace GPhoto;
namespace {
  class TestGPhotoWidgets : public ::testing::Test {
  public:
    TestGPhotoWidgets();
    GPhotoWrapperPtr gphoto;
    CameraWidget *top_window;
    CameraWidget *text_widget;
    CameraWidget *range_widget;
    CameraWidget *menu_widget;
    CameraWidget *date_widget;
    CameraWidget *bool_widget;
    chrono::system_clock::time_point time(int year, int month, int day, int hour, int min, int sec) const;
  };
}

TestGPhotoWidgets::TestGPhotoWidgets() : gphoto{new GPhotoWrapper}
{
  gphoto << GP2_RUN(this) { return gp_widget_new(GP_WIDGET_WINDOW, "Top Window", &top_window); }
	 << GP2_RUN(this) { return gp_widget_set_name(top_window, "top_window"); }
	 
	 << GP2_RUN(this) { return gp_widget_new(GP_WIDGET_TEXT, "Text Widget", &text_widget); }
	 << GP2_RUN(this) { return gp_widget_set_name(text_widget, "text_window"); }
	 << GP2_RUN(this) { return gp_widget_append(top_window, text_widget); }
	 << GP2_RUN(this) { return gp_widget_set_value(text_widget, "hello world"); }
	 
	 
	 << GP2_RUN(this) { return gp_widget_new(GP_WIDGET_MENU, "Menu Widget", &menu_widget); }
	 << GP2_RUN(this) { return gp_widget_set_name(menu_widget, "menu_window"); }
	 << GP2_RUN(this) { return gp_widget_add_choice(menu_widget, "First Choice"); }
	 << GP2_RUN(this) { return gp_widget_add_choice(menu_widget, "Second Choice"); }
	 << GP2_RUN(this) { return gp_widget_add_choice(menu_widget, "Third Choice"); }
	 << GP2_RUN(this) { return gp_widget_set_value(menu_widget, "Second Choice"); }
	 << GP2_RUN(this) { return gp_widget_append(top_window, menu_widget); }
	 
	 << GP2_RUN(this) { return gp_widget_new(GP_WIDGET_RANGE, "Range Widget", &range_widget); }
	 << GP2_RUN(this) { return gp_widget_set_name(range_widget, "range_window"); }
	 << GP2_RUN(this) { return gp_widget_set_range(range_widget, 1.2, 3.3, 0.5); }
	 << GP2_RUN(this) { return gp_widget_append(top_window, range_widget); }
	 << GP2_RUN(this) { float v = 1.2; return gp_widget_set_value(range_widget, &v); }
	 
	 << GP2_RUN(this) { return gp_widget_new(GP_WIDGET_DATE, "Date Widget", &date_widget); }
	 << GP2_RUN(this) { return gp_widget_set_name(date_widget, "date_window"); }
	 << GP2_RUN(this) { return gp_widget_append(top_window, date_widget); }
	 << GP2_RUN(this) { time_t t{426118342}; return gp_widget_set_value(date_widget, &t); }
	 
	 << GP2_RUN(this) { return gp_widget_new(GP_WIDGET_DATE, "Bool Widget", &bool_widget); }
	 << GP2_RUN(this) { return gp_widget_set_name(bool_widget, "bool_window"); }
	 << GP2_RUN(this) { return gp_widget_append(top_window, bool_widget); }
	 << GP2_RUN(this) { bool b = true; return gp_widget_set_value(bool_widget, &b); }
	    ;
}

TEST_F(TestGPhotoWidgets, testTopWindowCreation) {
  auto widget = make_shared<Widget>(top_window, gphoto, LoggerPtr{});
  ASSERT_EQ("Top Window", widget->label());
  ASSERT_EQ("top_window", widget->name());
  ASSERT_EQ(Widget::Window, widget->type());
}

TEST_F(TestGPhotoWidgets, testAllChildren) {
  auto widget = make_shared<Widget>(top_window, gphoto, LoggerPtr{});
  auto children = widget->children();
  ASSERT_EQ(5, children.size());
  ASSERT_EQ("text_window", (*children.begin())->name());
}

TEST_F(TestGPhotoWidgets, testFindChild) {
  auto widget = make_shared<Widget>(top_window, gphoto, LoggerPtr{});
  auto button = widget->child_by_label("Button Widget");
  ASSERT_FALSE(button);
  auto menu = widget->child_by_label("Menu Widget");
  ASSERT_EQ("menu_window", menu->name());
  ASSERT_EQ(Widget::Menu, menu->type());
  auto text = widget->child_by_name("text_window");
  ASSERT_EQ("Text Widget", text->label());
  ASSERT_EQ(Widget::String, text->type());
}

TEST_F(TestGPhotoWidgets, testGetStringValue) {
  auto widget = make_shared<Widget>(text_widget, gphoto, LoggerPtr{});
  string text = *widget->get<Widget::StringValue>();
  ASSERT_EQ("hello world", text);
}

TEST_F(TestGPhotoWidgets, testSetStringValue) {
  auto widget = make_shared<Widget>(text_widget, gphoto, LoggerPtr{});
  widget->get<Widget::StringValue>()->set("Foo Bla");
  
  char *new_value;
  gphoto << GP2_RUN(this, &new_value) { return gp_widget_get_value(text_widget, &new_value); };
  ASSERT_EQ(string{"Foo Bla"}, string{new_value});
}

TEST_F(TestGPhotoWidgets, testGetRangeValue) {
  auto widget = make_shared<Widget>(range_widget, gphoto, LoggerPtr{});
  float value = *widget->get<Widget::RangeValue>();
  ASSERT_EQ(1.2f, value);
}

TEST_F(TestGPhotoWidgets, testSetRangeValue) {
  auto widget = make_shared<Widget>(range_widget, gphoto, LoggerPtr{});
  widget->get<Widget::RangeValue>()->set(2.9);
  
  float new_value;
  gphoto << GP2_RUN(this, &new_value) { return gp_widget_get_value(range_widget, &new_value); };
  ASSERT_EQ(2.9f, new_value);
}

TEST_F(TestGPhotoWidgets, testGetRangeParameters) {
  auto widget = make_shared<Widget>(range_widget, gphoto, LoggerPtr{});
  Widget::RangeValue::Range range = widget->get<Widget::RangeValue>()->range();
  auto expected = Widget::RangeValue::Range{1.2, 3.3, 0.5};
  ASSERT_EQ(expected, range);
}


TEST_F(TestGPhotoWidgets, testMenuChoices) {
  auto widget = make_shared<Widget>(menu_widget, gphoto, LoggerPtr{});
  auto choices = widget->get<Widget::MenuValue>()->choices();
  auto expected = Widget::MenuValue::Choices{{"First Choice"}, {"Second Choice"}, {"Third Choice"}};
  ASSERT_EQ(expected, choices);
}

ostream &operator<<(ostream &o, const chrono::system_clock::time_point &t) {
  auto time = chrono::system_clock::to_time_t(t);
  return o << std::asctime(std::gmtime(&time));
}

TEST_F(TestGPhotoWidgets, testGetDate) {
  auto widget = make_shared<Widget>(date_widget, gphoto, LoggerPtr{});
  chrono::system_clock::time_point value = *widget->get<Widget::DateValue>();
  ASSERT_EQ(time(1983, 7, 3, 22, 12, 22), value);
}

TEST_F(TestGPhotoWidgets, testSetDate) {
  auto widget = make_shared<Widget>(date_widget, gphoto, LoggerPtr{});
  widget->get<Widget::DateValue>()->set(chrono::system_clock::from_time_t(134567));
  time_t new_value;
  gphoto << GP2_RUN(this, &new_value) { return gp_widget_get_value(date_widget, &new_value); };
  ASSERT_EQ(134567, new_value);
}

TEST_F(TestGPhotoWidgets, testGetBool) {
  auto widget = make_shared<Widget>(bool_widget, gphoto, LoggerPtr{});
  bool value = *widget->get<Widget::BoolValue>();
  ASSERT_TRUE(value);
}

TEST_F(TestGPhotoWidgets, testSetBool) {
  auto widget = make_shared<Widget>(bool_widget, gphoto, LoggerPtr{});
  widget->get<Widget::BoolValue>()->set(false);
  bool value = *widget->get<Widget::BoolValue>();
  ASSERT_FALSE(value);
}


chrono::system_clock::time_point TestGPhotoWidgets::time(int year, int month, int day, int hour, int min, int sec) const
{
  tm t;
  t.tm_sec = sec;
  t.tm_min = min;
  t.tm_hour = hour;
  t.tm_mday = day;
  t.tm_mon = month-1;
  t.tm_year = year-1900;
  time_t time = mktime(&t);
  time += t.tm_gmtoff;
  return chrono::system_clock::from_time_t(time);
}
