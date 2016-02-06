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

#ifndef GPHOTO_LOGGER_H
#define GPHOTO_LOGGER_H
#include "dptr.h"
#include <memory>
#include <list>
#include <string>
#include <sstream>
#include <functional>
#include <chrono>



namespace GPhoto {
class Logger {
public:
  typedef std::shared_ptr<Logger> ptr;
  enum Level {TRACE = 0, DEBUG = 10, INFO = 20, WARNING = 30, ERROR = 40};
  typedef std::function<void(const std::string &message, Level level)> Printer;
  Logger(const Printer &printer);
  ~Logger();
  friend class Out;
  class Out;  
  std::shared_ptr<Out> out(Level level);
private:
  DPTR
};
}

GPhoto::Logger::Out& operator<<(GPhoto::Logger::Out& out, const std::string& s);

namespace GPhoto {
  class Logger::Out {
  public:
    typedef std::shared_ptr<Out> ptr;
    ~Out();
    friend Out &::operator<<(Out &out, const std::string &t);
    friend class Logger;
  private:
    Out(Logger &logger, Level level);
    DPTR
  };
};

#include <iostream>
template<typename T> GPhoto::Logger::Out::ptr operator<<(const GPhoto::Logger::Out::ptr &p, const T &t) {
  *p << t;
  return p;
}
template<typename T> GPhoto::Logger::Out& operator<<(GPhoto::Logger::Out& out, const T& t)
{
  std::stringstream s;
  s << t;
  return out << s.str();
}

inline std::ostream &operator<<(std::ostream &o, const std::chrono::time_point<std::chrono::system_clock> &t) {
  std::time_t tt = std::chrono::system_clock::to_time_t(t);
  return o << std::ctime(&tt);
}

#define lLog(level, logger) if(logger) logger->out(GPhoto::Logger::level)
#define lTrace(logger) lLog(TRACE, logger)
#define lDebug(logger) lLog(DEBUG, logger)
#define lInfo(logger) lLog(INFO, logger)
#define lWarning(logger) lLog(WARNING, logger)
#define lError(logger) lLog(ERROR, logger)

#endif