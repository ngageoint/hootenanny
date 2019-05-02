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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "BaseCommand.h"

// Hoot
#include <hoot/core/Hoot.h>
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/util/Progress.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/HootException.h>

// Qt
#include <QFileInfo>

using namespace geos::geom;

namespace hoot
{

BaseCommand::BaseCommand()
{
}

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
  QString result = ConfPath::getHootHome() + "/docs/commands/" + getName() + ".asciidoc";

  return result;
}

Envelope BaseCommand::parseEnvelope(QString envStr) const
{
  QStringList envArr = envStr.split(",");

  if (envArr.size() != 4)
  {
    throw HootException("Invalid bounds format, requires 4 values: " + envStr);
  }

  bool ok, allOk = true;
  double left = envArr[0].toDouble(&ok);
  allOk &= ok;
  double bottom = envArr[1].toDouble(&ok);
  allOk &= ok;
  double right = envArr[2].toDouble(&ok);
  allOk &= ok;
  double top = envArr[3].toDouble(&ok);
  allOk &= ok;

  if (allOk == false)
  {
    throw HootException("Invalid bounds format: " + envStr);
  }

  return Envelope(left, right, bottom, top);
}

int BaseCommand::run(char* argv[], int argc)
{
  QStringList args = toQStringList(argv, argc);
  LOG_VART(args)

  args = args.mid(2);

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

}
