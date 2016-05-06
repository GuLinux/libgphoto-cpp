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


#include "read_raw_image.h"
#include "libraw/libraw.h"
#include <algorithm>
#include <iostream>
using namespace GPhotoCPP;
using namespace std;

DPTR_CLASS(ReadRawImage) {
public:
  Private(ReadRawImage *q);
  LibRaw raw;
  typedef function<int(LibRaw &)> LoadRaWImage;
  void run(LoadRaWImage f);
  ReadImage::Image read_image(const LoadRaWImage &init_raw, const std::string &filename);
private:
  ReadRawImage *q;
};

ReadRawImage::Private::Private(ReadRawImage* q) : q{q}
{
}


ostream &operator<<(ostream &o, const libraw_image_sizes_t &s) {
  o << "{ "
    << "raw: " << s.raw_width << "x" << s.raw_height << ", raw_pitch: " << s.raw_pitch << ", "
    <<"wxh: " << s.width << "x" << s.height << ", "
    <<"iwxih: " << s.iwidth << "x" << s.iheight << ", "
    << "top margin: " << s.top_margin << ", left margin: " << s.left_margin << ", "
    << "flip: " << s.flip
    << "mask: [";
  for(int i=0; i<8; i++) {
    o << " [";
    for(int j=0; j<4; j++)
      o << s.mask[i][j] << ",";
    o << "]";
  }
  return o;
}

ReadRawImage::ReadRawImage() : dptr(this)
{

}

ReadRawImage::~ReadRawImage()
{
}

ReadImage::Image ReadRawImage::read(const vector< uint8_t >& data, const std::string &filename)
{
  return d->read_image([&](LibRaw &r) { return r.open_buffer(data.data(), data.size()); }, filename);
}


ReadImage::Image ReadRawImage::read(const string& file_path)
{
  return d->read_image([&](LibRaw &r) { return r.open_file(file_path.c_str()); }, file_path);
}

ReadImage::Image ReadRawImage::Private::read_image(const LoadRaWImage& init_raw, const string& filename)
{
  struct cleanup {
    cleanup(LibRaw &raw) : raw{raw} {}
    ~cleanup() { raw.recycle(); }
    LibRaw &raw;
  };
  cleanup _cleanup{raw};
  auto image_data = raw.imgdata;
  image_data.params.output_bps = 16;
  image_data.params.user_flip = 0;
  image_data.params.no_auto_scale = 1;
  image_data.params.no_auto_bright = 1;
  image_data.params.no_interpolation = 1;
  image_data.params.sraw_ycc =1;
  fill_n(begin(image_data.params.gamm), 2, 1.);
  run(init_raw);
  run([&](LibRaw &r) { return r.unpack(); });
  run([&](LibRaw &r) { return r.raw2image(); });
  
  image_data =  raw.imgdata;
  Image out_image{image_data.sizes.width, image_data.sizes.height, 16, filename};
  uint16_t *data_8_16 = reinterpret_cast<uint16_t*>(q->init_channel(out_image, Image::Grey).data() );
  transform(image_data.image, image_data.image + out_image.channels[Image::Grey].size()/2, data_8_16, [](uint16_t *data){ return accumulate(data, data+4, 0); });
  raw.free_image();
  return out_image;
}


void ReadRawImage::Private::run(LoadRaWImage f)
{
    int ret = f(raw);
    if(ret != LIBRAW_SUCCESS)
      throw ReadRawImage::error(libraw_strerror(ret));
}

ReadRawImage::error::error(const char* e): runtime_error(e)
{
}

