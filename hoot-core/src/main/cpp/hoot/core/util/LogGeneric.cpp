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
 * @copyright Copyright (C) 2015, 2016, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "Log.h"

// Qt
#include <QDateTime>
#include <QDebug>

// Standard
#include <iomanip>
#include <iostream>
using namespace std;

namespace hoot
{

void Log::init()
{
}

void Log::log(WarningLevel level, const string& str)
{
  //  Mimic log4cxx with unknown file name (...?), function name ("") and line number (-1)
  log(level, str, "...?", "", -1);
}

void Log::log(WarningLevel level, const string& str, const string& filename,
  const string& prettyFunction, int lineNumber)
{  

  if (level >= _level && notFiltered(prettyFunction))
  {
    QDateTime dt = QDateTime::currentDateTime();

    // takes the form: "09:34:21.635 WARN  <filename>(<lineNumber>) <str>"
    cout << dt.toString("hh:mm:ss.zzz") << " " << setw(6) << left << getLevelString(level) << " " <<
            ellipsisStr(filename) << "(" << setw(4) << right << lineNumber << ")" << " " << str << endl;
  }
}

void Log::progress(WarningLevel level, const string& str, const string& filename,
  const string& prettyFunction, int lineNumber)
{
  if (level >= _level && notFiltered(prettyFunction))
  {
    QDateTime dt = QDateTime::currentDateTime();

    // takes the form: "09:34:21.635 WARN  <filename>(<lineNumber>) <str>"
    cout << dt.toString("hh:mm:ss.zzz") << " " << setw(6) << left << getLevelString(level) << " " <<
            ellipsisStr(filename) << "(" << setw(4) << right << lineNumber << ")" << " " << str <<
            "        \r" << flush;
  }
}

bool Log::notFiltered(const string& prettyFunction)
{
  // TODO: This check does not work at the trace level for some reason, and the class filter is
  // being reset to empty at some point after initialization. The result is no log filtering
  // occurs at the trace level and messages for all classes are logged.

  // init here instead of in init() since some logs are being produced before the init() call
//  if (!_classFilterInitialized)
//  {
//    _classFilter = ConfigOptions().getLogClassFilter().split(";");
//    _classFilter.removeAll(QString(""));
//    _classFilterInitialized = true;
//  }
//  if (_classFilter.isEmpty())
//  {
//    return true;
//  }

  // The unnecessary extra calls to this initialization will have to remain until the above problem
  // is fixed for trace logging.
  if (_classFilter.isEmpty())
  {
    _classFilter = ConfigOptions().getLogClassFilter().split(";");
    if (!_classFilter.isEmpty())
    {
      _classFilter.removeAll(QString(""));
    }
  }
  if (_classFilter.isEmpty())
  {
    return true;
  }

  // split arguments from function call name
  QStringList nameParts = QString::fromStdString(prettyFunction).split("(");
  if (nameParts.length() < 1) return true;

  // split class name from function name
  nameParts = nameParts[0].split("::");
  int listLen = nameParts.length();

  // Is there any way we can throw here if the class name isn't recognized, so we don't wonder
  // why a typo caused us not to get the logging we thought we were going to get?

  return (listLen > 1 && _classFilter.contains(nameParts[listLen - 2]));
}

void Log::setLevel(WarningLevel l)
{
  _level = l;
}

}
