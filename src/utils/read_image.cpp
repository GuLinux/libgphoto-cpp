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

#include "read_jpeg_image.h"
#include "read_raw_image.h"
#include <unordered_map>
#include <list>
#include <algorithm>

using namespace GPhotoCPP;
using namespace std;

namespace GPhotoCPP {
namespace {
  typedef function<ReadImage::ptr()> ReadImageFactory;
struct CheckExtension {
  list<string> extensions;
  ReadImageFactory factory;
  bool operator==(const string &filename) const;
  ReadImage::ptr operator()() const { return factory(); }
};
}
}

bool CheckExtension::operator==(const string& filename) const
{
  if(extensions.empty())
    return true;
  auto pos = filename.find_last_of('.');
  auto extension = pos==string::npos ? string{} : filename.substr(pos);
  if(extension.empty())
    return false;
  transform(begin(extension), end(extension), begin(extension), ::tolower);
  auto found = find(begin(extensions), end(extensions), extension);
  return found != end(extensions);
}


ReadImage::ptr ReadImage::factory(const ReadImage::FileInfo& fileinfo)
{
  static ReadImageFactory jpeg_factory = []{ return make_shared<ReadJPEGImage>(); };
  static ReadImageFactory raw_factory = []{ return make_shared<ReadRawImage>(); };
  
  static list<CheckExtension> extensions_map {
    {{".jpeg", ".jpg"}, jpeg_factory },
    {{}, raw_factory},
  };

  
  if(!fileinfo.filename.empty()) {
    return (*find(begin(extensions_map), end(extensions_map), fileinfo.filename))();
  }
  if(!fileinfo.mimetype.empty()) {
    string lower_mime = fileinfo.mimetype;
    transform(begin(lower_mime), end(lower_mime), begin(lower_mime), ::tolower);
    return lower_mime == "image/jpeg" ? jpeg_factory() : raw_factory();
  }
  return {};
}
