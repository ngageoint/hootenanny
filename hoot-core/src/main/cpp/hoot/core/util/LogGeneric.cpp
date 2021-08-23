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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015, 2016, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#include "Log.h"

// Hoot
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/StringUtils.h>

// Qt
#include <QDateTime>
#include <QDebug>

// Standard
#include <iomanip>

using namespace std;

namespace hoot
{

void Log::init() const
{
}

void Log::log(WarningLevel level, const string& str)
{
  //  Mimic log4cxx with unknown file name (...?), function name ("") and line number (-1)
  log(level, str, "...?", "", -1);
}

void Log::log(
  WarningLevel level, const string& str, const string& filename, const string& prettyFunction,
  int lineNumber)
{  
  if (level >= _level && _passesFilter(prettyFunction))
  {
    QDateTime dt = QDateTime::currentDateTime();

    // takes the form: "09:34:21.635 WARN  <filename>(<lineNumber>) <str>"
    cout << dt.toString("hh:mm:ss.zzz") << " " << setw(6) << left << Log::levelToString(level) <<
            " " << ellipsisStr(filename) << "(" << setw(4) << right << lineNumber << ")" << " " <<
            str << endl;
  }
}

void Log::progress(
  WarningLevel level, const string& str, const string& filename, const string& prettyFunction,
  int lineNumber)
{
  if (level >= _level && _passesFilter(prettyFunction))
  {
    QDateTime dt = QDateTime::currentDateTime();

    // takes the form: "09:34:21.635 WARN  <filename>(<lineNumber>) <str>"
    cout << dt.toString("hh:mm:ss.zzz") << " " << setw(6) << left << Log::levelToString(level) <<
            " " << ellipsisStr(filename) << "(" << setw(4) << right << lineNumber << ")" << " " <<
            str << "        \r" << flush;
  }
}

bool Log::_passesFilter(const string& prettyFunction)
{
  // Currently, this initialization call needs to be executed each time here, rather than in a
  // constructor. There may be a way to optimize this.
  if (_includeClassFilter.isEmpty() && _excludeClassFilter.isEmpty())
    _setFilter();

  if (_includeClassFilter.isEmpty() && _excludeClassFilter.isEmpty())
    return true;

  QString prettyFunctionQt = QString::fromStdString(prettyFunction);
  QString name;
  if (!prettyFunctionQt.endsWith(".js"))    // call from C++
  {
    // split arguments from function call name
    QStringList nameParts = prettyFunctionQt.split("(");
    if (nameParts.length() < 1) return true;

    // split class name from function name
    nameParts = nameParts[0].split("::", QString::SkipEmptyParts);
    const int listLen = nameParts.size();
    if (listLen <= 1)
    {
      return false;
    }
    name = nameParts[listLen - 2];
  }
  else  // call from a JS generic conflate script
  {
    // split arguments from script path
    const QStringList nameParts = prettyFunctionQt.split("/", QString::SkipEmptyParts);
    if (nameParts.length() < 1) return true;

    name = nameParts[nameParts.size() - 1];
  }

  // It would be nice if we could throw here if the class name isn't recognized, so we don't
  // wonder why a typo caused us not to get the logging we thought we were going to get. However,
  // Factory::hasClass as a check won't work here since not all classes doing logging are
  // factories and not sure of any other way to validate it.

  // If anything was added to the exclude filter and this class was explicitly excluded, we'll skip
  // logging.
  if (!_excludeClassFilter.isEmpty() && StringUtils::matchesWildcard(name, _excludeClassFilter))
  {
    return false;
  }
  // If nothing was added to the include list, everything is allowed to log. Otherwise, only allow
  // this class to log if it has been added to the include list.
  return _includeClassFilter.isEmpty() || StringUtils::matchesWildcard(name, _includeClassFilter);
}

void Log::setLevel(WarningLevel l)
{
  _level = l;
}

void Log::_setFilter()
{
  _includeClassFilter = ConfigOptions().getLogClassIncludeFilter();
  StringUtils::removePrefixes(MetadataTags::HootNamespacePrefix(), _includeClassFilter);
  _includeClassFilter.removeDuplicates();
  _excludeClassFilter = ConfigOptions().getLogClassExcludeFilter();
  StringUtils::removePrefixes(MetadataTags::HootNamespacePrefix(), _excludeClassFilter);
  _excludeClassFilter.removeDuplicates();
}

}
