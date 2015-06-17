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
 * @copyright Copyright (C) 2012, 2013, 2014, 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "BaseCommand.h"

// Hoot
#include <hoot/core/Hoot.h>
#include <hoot/core/io/OgrReader.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmReader.h>
#include <hoot/core/io/OsmWriter.h>
#include <hoot/core/io/GeoNamesReader.h>
#include <hoot/core/io/PbfReader.h>
#include <hoot/core/io/PbfWriter.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/visitors/ReportMissingElementsVisitor.h>
#include <hoot/core/util/Progress.h>

namespace hoot
{

BaseCommand::BaseCommand()
{
}

void BaseCommand::loadMap(shared_ptr<OsmMap> map, QString path, bool useFileId,
                          Status defaultStatus)
{
  if (path.endsWith(".geonames"))
  {
    GeoNamesReader reader;
    reader.setDefaultStatus(defaultStatus);
    reader.read(path, map);
  }
  else
  {
    QStringList pathLayer = path.split(";");
    QString justPath = pathLayer[0];
    if (OgrReader::isReasonablePath(justPath))
    {
      OgrReader reader;
      Progress progress(getName());
      reader.setDefaultStatus(defaultStatus);
      reader.read(justPath, pathLayer.size() > 1 ? pathLayer[1] : "", map, progress);
    }
    else
    {
      OsmMapReaderFactory::read(map, path, useFileId, defaultStatus);
    }
  }
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

  args = args.mid(2);

  bool foundOne = true;

  while (args.size() > 0 && foundOne)
  {
    if (args[0] == "--conf" || args[0] == "-C")
    {
      if (args.size() < 2)
      {
        throw HootException("--conf must be followed by a file name.");
      }
      conf().loadJson(args[1]);
      // move on to the next argument.
      args = args.mid(2);
    }
    else if (args[0] == "--debug")
    {
      Log::getInstance().setLevel(Log::Debug);
      args = args.mid(1);
    }
    else if (args[0] == "--info")
    {
      Log::getInstance().setLevel(Log::Info);
      args = args.mid(1);
    }
    else if (args[0] == "--warn")
    {
      Log::getInstance().setLevel(Log::Warn);
      args = args.mid(1);
    }
    else if (args[0] == "--error")
    {
      Log::getInstance().setLevel(Log::Error);
      args = args.mid(1);
    }
    else if (args[0] == "--define" || args[0] == "-D")
    {
      if (args.size() < 2)
      {
        throw HootException("--define or -D must be followed by key=value.");
      }
      QString kv = args[1];
      QStringList kvl = kv.split("+=");
      bool append = true;
      if (kvl.size() != 2)
      {
        // split on the first '='
        int sep = kv.indexOf('=');
        kvl.clear();
        if (sep != -1)
        {
          kvl << kv.mid(0, sep);
          kvl << kv.mid(sep + 1);
        }
        append = false;
      }
      if (kvl.size() != 2)
      {
        throw HootException("define must takes the form key=value.");
      }
      if (append)
      {
        QStringList values = kvl[1].split(";", QString::SkipEmptyParts);
        conf().append(kvl[0], values);
      }
      else
      {
        conf().set(kvl[0], kvl[1]);
      }
      // move on to the next argument.
      args = args.mid(2);
    }
    else
    {
      foundOne = false;
    }
  }
  // re-initialize the logger and other resources after the settings have been parsed.
  Hoot::getInstance().reinit();

  return runSimple(args);
}

void BaseCommand::saveMap(shared_ptr<const OsmMap> map, QString path)
{
  OsmMapWriterFactory::write(map, path);
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
