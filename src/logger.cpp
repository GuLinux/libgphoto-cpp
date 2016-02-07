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

#include "logger.h"
using namespace GPhoto;
DPTR_CLASS(Logger) {
public:
  Private(const Logger::Printer &printer, Logger *q);
  Logger::Printer print;
private:
  Logger *q;
};

Logger::Private::Private(const Logger::Printer &printer, Logger* q) : print{printer}, q{q}
{

}


GPhoto::Logger::Logger(const Printer &printer) : dptr(printer, this)
{
}

Logger::~Logger() {
}


DPTR_CLASS(Logger::Out) {
public:
  Private(Logger &logger, Level level);
  Logger &logger;
  Level level;
  std::list<std::string> entries;
};

Logger::Out::Private::Private(Logger& logger, Logger::Level level) : logger{logger}, level{level}
{

}


GPhoto::Logger::Out::~Out()
{
  std::stringstream s;
  for(auto entry: d->entries)
    s << entry;
  d->logger.d->print(s.str(), d->level);
}


GPhoto::Logger::Out::Out(GPhoto::Logger& logger, GPhoto::Logger::Level level) : dptr(logger, level) {
}

std::shared_ptr< GPhoto::Logger::Out > GPhoto::Logger::out(GPhoto::Logger::Level level)
{
  return std::shared_ptr<Out>(new Out{*this, level});
}



GPhoto::Logger::Out& operator<<(GPhoto::Logger::Out& out, const std::string& s)
{
  out.d->entries.push_back(s);
  return out;
}