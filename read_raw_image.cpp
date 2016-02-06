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


#include "read_raw_image.h"
#include "libraw/libraw.h"
#include <algorithm>
#include <iostream>
using namespace GPhoto;
using namespace std;

DPTR_CLASS(ReadRawImage) {
public:
  LibRaw raw;
  void run(function<int(LibRaw &)> f);
};

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

ReadRawImage::ReadRawImage() : dptr()
{

}

ReadRawImage::~ReadRawImage()
{

}

GPhoto::ReadImage::Image ReadRawImage::read(const string& filename)
{
  struct cleanup {
    cleanup(LibRaw &raw) : raw{raw} {}
    ~cleanup() { raw.recycle(); }
    LibRaw &raw;
  };
  cleanup _cleanup{d->raw};
  auto image_data = d->raw.imgdata;
  image_data.params.output_bps = 16;
  image_data.params.user_flip = 0;
  image_data.params.no_auto_scale = 1;
  image_data.params.no_auto_bright = 1;
  image_data.params.no_interpolation = 1;
  image_data.params.sraw_ycc =1;
  fill_n(begin(image_data.params.gamm), 2, 1.);
  d->run([=](LibRaw &r) { return r.open_file(filename.c_str()); });
  d->run([=](LibRaw &r) { return r.unpack(); });
  d->run([=](LibRaw &r) { return r.raw2image(); });
  
  image_data =  d->raw.imgdata;
  Image out_image{2, image_data.sizes.width, image_data.sizes.height, 16, vector<uint8_t>(out_image.h*out_image.w*2)};
  uint16_t *data_8_16 = reinterpret_cast<uint16_t*>(out_image.data.data());
  transform(image_data.image, image_data.image + out_image.data.size()/2, data_8_16, [](uint16_t *data){ return accumulate(data, data+4, 0); });
  d->raw.free_image();
  return out_image;
}

void ReadRawImage::Private::run(function<int(LibRaw &r)> f)
{
    int ret = f(raw);
    if(ret != LIBRAW_SUCCESS)
      throw ReadRawImage::error(libraw_strerror(ret));
}

ReadRawImage::error::error(const char* e): runtime_error(e)
{
}

