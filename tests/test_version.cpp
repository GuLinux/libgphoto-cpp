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
#include <iostream>
using namespace std;

typedef GPhotoCPP::GPhotoWrapper::Version Version;
Version version(int major, int minor, int patch) { return {major,minor,patch}; }

TEST(Version, Equals) {
  Version v{2,5,9};
  ASSERT_TRUE(version(2,5,9) == v);
  ASSERT_FALSE(version(2,5,9) != v);
  ASSERT_FALSE(version(2,5,9) > v);
  ASSERT_FALSE(version(2,5,9) < v);
  ASSERT_TRUE(version(2,5,9) >= v);
  ASSERT_TRUE(version(2,5,9) <= v);
}

TEST(Version, Lower) {
  Version v{2,5,8};
  ASSERT_FALSE(version(2,5,9) == v);
  ASSERT_TRUE(version(2,5,9) != v);
  ASSERT_TRUE(version(2,5,9) > v);
  ASSERT_FALSE(version(2,5,9) < v);
  ASSERT_TRUE(version(2,5,9) >= v);
  ASSERT_FALSE(version(2,5,9) <= v);
}

TEST(Version, Higher) {
  Version v{2,5,10};
  ASSERT_FALSE(version(2,5,9) == v);
  ASSERT_TRUE(version(2,5,9) != v);
  ASSERT_FALSE(version(2,5,9) > v);
  ASSERT_TRUE(version(2,5,9) < v);
  ASSERT_FALSE(version(2,5,9) >= v);
  ASSERT_TRUE(version(2,5,9) <= v);
}


TEST(Version, MinorLower) {
  Version v{2,4,9};
  ASSERT_FALSE(version(2,5,9) == v);
  ASSERT_TRUE(version(2,5,9) != v);
  ASSERT_TRUE(version(2,5,9) > v);
  ASSERT_FALSE(version(2,5,9) < v);
  ASSERT_TRUE(version(2,5,9) >= v);
  ASSERT_FALSE(version(2,5,9) <= v);
}

TEST(Version, MinorHigher) {
  Version v{2,6,9};
  ASSERT_FALSE(version(2,5,9) == v);
  ASSERT_TRUE(version(2,5,9) != v);
  ASSERT_FALSE(version(2,5,9) > v);
  ASSERT_TRUE(version(2,5,9) < v);
  ASSERT_FALSE(version(2,5,9) >= v);
  ASSERT_TRUE(version(2,5,9) <= v);
}


TEST(Version, MajorLower) {
  Version v{1,5,9};
  ASSERT_FALSE(version(2,5,9) == v);
  ASSERT_TRUE(version(2,5,9) != v);
  ASSERT_TRUE(version(2,5,9) > v);
  ASSERT_FALSE(version(2,5,9) < v);
  ASSERT_TRUE(version(2,5,9) >= v);
  ASSERT_FALSE(version(2,5,9) <= v);
}

TEST(Version, MajorHigher) {
  Version v{3,5,9};
  ASSERT_FALSE(version(2,5,9) == v);
  ASSERT_TRUE(version(2,5,9) != v);
  ASSERT_FALSE(version(2,5,9) > v);
  ASSERT_TRUE(version(2,5,9) < v);
  ASSERT_FALSE(version(2,5,9) >= v);
  ASSERT_TRUE(version(2,5,9) <= v);
}

TEST(Version, GetCurrentVersion) {
  GPhotoCPP::GPhotoWrapper wrapper;
  cerr << "Current gphoto version: " << wrapper.version() << endl;
}