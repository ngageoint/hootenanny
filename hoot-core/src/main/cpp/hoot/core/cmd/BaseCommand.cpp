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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#include "BaseCommand.h"

// Hoot
#include <hoot/core/Hoot.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/util/Progress.h>
#include <hoot/core/util/Settings.h>

using namespace geos::geom;

namespace hoot
{

QString BaseCommand::getHelp() const
{
  QFile fp(_getHelpPath());

  if (fp.open(QFile::ReadOnly) == false)
  {
    LOG_ERROR(QString("Error opening command help for reading. (%1) Is HOOT_HOME set properly?").
      arg(_getHelpPath()));
  }

  return QString::fromUtf8(fp.readAll());
}

QString BaseCommand::_getHelpPath() const
{
  return ConfPath::getHootHome() + "/docs/commands/" + getName() + ".asciidoc";
}

int BaseCommand::run(char* argv[], int argc)
{
  QStringList args = toQStringList(argv, argc);
  LOG_VART(args);
  LOG_VART(args.join(" "));

  args = args.mid(2);
  _rawArgs = args;

  Settings::parseCommonArguments(args);
  LOG_VART(args);

  if (ConfigOptions().getHashSeedZero())
  {
    qSetGlobalQHashSeed(0);
    LOG_TRACE("Qt hash seed set to 0");
  }

  return runSimple(args);
}

QStringList BaseCommand::toQStringList(char* argv[], int argc)
{
  QStringList result;
  for (int i = 0; i < argc; i++)
  {
    result << argv[i];
  }
  return result;
}

QStringList BaseCommand::_parseRecursiveInputParameter(QStringList& args, bool& paramPresent)
{
  QStringList inputFilters;
  paramPresent = false;
  if (args.contains("--recursive"))
  {
    paramPresent = true;
    const int recursiveIndex = args.indexOf("--recursive");
    if (args.size() < recursiveIndex + 2)
    {
      throw IllegalArgumentException(
        "The --recursive option must be followed by either \"*\" for no filtering or one or "
        "more filters.");
    }
    const QString filter = args.at(recursiveIndex + 1).trimmed();
    // "*" denotes no filtering
    if (filter != "*")
    {
      inputFilters = filter.split(";");
    }
    args.removeAt(recursiveIndex + 1);
    args.removeAt(recursiveIndex);
  }
  LOG_VARD(inputFilters);
  return inputFilters;
}

}
