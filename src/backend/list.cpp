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
 * 
 */

#include "backend/list.h"
#include "backend/gphoto_wrapper.h"
#include <gphoto2/gphoto2.h>

using namespace GPhotoCPP;
using namespace std;

class List::Private {
public:
  Private(const GPhotoWrapperPtr& gphoto, List* q);
  CameraList *cameralist;
  GPhotoWrapperPtr gphoto;
private:
  List *q;
};

List::Private::Private(const GPhotoWrapperPtr &gphoto, List* q) : gphoto{gphoto}, q(q)
{
}

List::List(const GPhotoWrapperPtr &gphoto)
 : dptr(gphoto, this)
{
  d->gphoto << GP2_RUN(this) { GPRET(gp_list_new(&d->cameralist)) };
}

List::~List()
{
  d->gphoto << GP2_RUN(this) { GPRET(gp_list_free(d->cameralist)) };
}

List::operator CameraList*() const
{
  return d->cameralist;
}

List::operator multimap<string, string>() const
{
  int size = (*d->gphoto)(GP2_RUN(this) { GPRET(gp_list_count(d->cameralist)) });
  multimap<string, string> ret;
  for(int i=0; i<size; i++) {
    const char *key;
    const char *value;
    d->gphoto << GP2_RUN(this, &i, &key) { GPRET(gp_list_get_name(d->cameralist, i, &key)) }
	      << GP2_RUN(this, &i, &value) { GPRET(gp_list_get_value(d->cameralist, i, &value)) };
    ret.insert(make_pair(key?key:"", value?value:""));
  };
  return ret;
}
