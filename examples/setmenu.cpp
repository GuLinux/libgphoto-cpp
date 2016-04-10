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
#include "commons.h"
#include <exposure.h>

using namespace std;
using namespace GPhotoCPP;


int main(int argc, char **argv) {
  init_options(argc, argv);
  if(has_option("-h") || has_option("--help")) {
    cout << args[0] << " usage: " << endl
	 << endl;
    return 0;
  }
  cerr << "current locale: " << std::setlocale(LC_ALL, nullptr) << endl;
  std::setlocale(LC_ALL, "it_IT.UTF-8");

  auto logger = make_logger();
  
  
  auto camera = init_camera(logger);
  if(!camera)
    return 1;
  auto settings = camera->widgets_settings();
  auto format = settings->child_by_name("imageformat")->get<Widget::MenuValue>();
  cerr << "Current format: " << format->get() << endl;
  for(auto f: format->choices())
    cerr << "* '" << f << "'" << endl;
  return 0;
};