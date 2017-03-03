/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef LOGJS_H
#define LOGJS_H

// Hoot
#include <hoot/core/util/Log.h>

// node.js
#include <hoot/js/SystemNodeJs.h>

// v8
//#include <v8.h>

namespace hoot
{
using namespace node;
using namespace v8;

class LogJs : public node::ObjectWrap
{
public:
 static void Init(v8::Handle<v8::Object> target);

private:
  LogJs();
  ~LogJs();

  static v8::Handle<v8::Value> log(const v8::Arguments& args, Log::WarningLevel level);
  static v8::Handle<v8::Value> debug(const v8::Arguments& args);
  static v8::Handle<v8::Value> trace(const v8::Arguments& args);
  static v8::Handle<v8::Value> logTrace(const v8::Arguments& args);
  static v8::Handle<v8::Value> logDebug(const v8::Arguments& args);
  static v8::Handle<v8::Value> logInfo(const v8::Arguments& args);
  static v8::Handle<v8::Value> logWarn(const v8::Arguments& args);
  static v8::Handle<v8::Value> logError(const v8::Arguments& args);
  static v8::Handle<v8::Value> logFatal(const v8::Arguments& args);
  static v8::Handle<v8::Value> init(const v8::Arguments& args);
  static v8::Handle<v8::Value> setLogLevel(const v8::Arguments& args);

  /**
   * Returns the number of times that this log message has been emitted.
   */
  static int getLogCount(QString log);

  static QHash<QString, int> _logs;
};

}

#endif // LOGJS_H
