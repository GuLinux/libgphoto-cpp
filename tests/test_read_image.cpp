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
#include "utils/read_image.h"
#include <utils/read_jpeg_image.h>
#include <utils/read_raw_image.h>

using namespace std;
using namespace GPhotoCPP;
namespace {
  class TestReadImage : public ::testing::Test {
  public:
};
}
TEST_F(TestReadImage, Factory_Load_Simple) {
  auto read_image = ReadImage::factory({"test.jpg"});
  ASSERT_EQ(typeid(ReadJPEGImage), typeid(*read_image));
  read_image = ReadImage::factory({"test.jpeg"});
  ASSERT_EQ(typeid(ReadJPEGImage), typeid(*read_image));
  read_image = ReadImage::factory({"test.crw"});
  ASSERT_EQ(typeid(ReadRawImage), typeid(*read_image));
}

TEST_F(TestReadImage, Factory_Null_File) {
  auto read_image = ReadImage::factory({""});
  ASSERT_FALSE(read_image);
}

TEST_F(TestReadImage, Factory_Mime_Type) {
  auto read_image = ReadImage::factory({ {}, "image/jpeg"});
  ASSERT_EQ(typeid(ReadJPEGImage), typeid(*read_image));
  read_image = ReadImage::factory({{}, "anything"});
  ASSERT_EQ(typeid(ReadRawImage), typeid(*read_image));
}

