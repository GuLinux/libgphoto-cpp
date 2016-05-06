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
 #include <cstdio>
 #include <jpeglib.h>
 #include <jerror.h>
#define cimg_plugin "plugins/jpeg_buffer.h"

#include <CImg.h>
#include <fstream>
#include <list>
#include <unordered_map>
using namespace GPhotoCPP;
using namespace cimg_library;
using namespace std;


DPTR_CLASS(ReadJPEGImage) {
public:
  Private(ReadJPEGImage *q);
  typedef function<void(CImg<uint8_t> &)> LoadImage;
  ReadImage::Image read_from(LoadImage loader, const std::string &filename);
private:
  ReadJPEGImage *q;
};

ReadJPEGImage::Private::Private(ReadJPEGImage* q) : q{q}
{
}

ReadJPEGImage::ReadJPEGImage() : dptr(this)
{
}

ReadJPEGImage::~ReadJPEGImage()
{
}

ReadImage::Image ReadJPEGImage::read(const vector< uint8_t >& data, const string& filename)
{
  return d->read_from([&](CImg<uint8_t> &img){
    img.load_jpeg_buffer(data.data(), data.size());
  }, filename);
}


ReadImage::Image ReadJPEGImage::read(const string& file_path)
{
  return d->read_from([&](CImg<uint8_t> &img) { img.load_jpeg(file_path.c_str()); }, file_path);
}

ReadImage::Image ReadJPEGImage::Private::read_from(ReadJPEGImage::Private::LoadImage loader, const string& filename)
{
  CImg<uint8_t> cimg_image;
  loader(cimg_image);
  Image image{cimg_image.width(), cimg_image.height(), 8, filename};
  auto channels = cimg_image.get_split('c');
  static vector<Image::Channel> channels_map{ Image::Red, Image::Green, Image::Blue, Image::Grey };
  
  auto channels_map_offset = channels.size() == 1 ? 3 : 0;
  for(int i=0; i<channels.size(); i++) {
    auto &pixels = q->init_channel(image, channels_map[i+channels_map_offset]);
    copy(channels[i].begin(), channels[i].end(), pixels.begin());
  }
  return image;

}
