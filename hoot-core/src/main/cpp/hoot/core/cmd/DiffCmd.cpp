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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/io/OsmApiChangeset.h>
#include <hoot/core/scoring/MapComparator.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/ops/DuplicateNodeRemover.h>

using namespace std;

#include <QDir>
#include <QFileInfo>

namespace hoot
{

class DiffCmd : public BaseCommand
{
public:

  static string className() { return "hoot::DiffCmd"; }

  DiffCmd() { }

  virtual QString getName() const override { return "diff"; }

  virtual QString getDescription() const override
  { return "Calculates the difference between two maps or changesets"; }

  virtual int runSimple(QStringList& args) override
  {
    bool ignoreUuid = false;
    bool useDateTime = false;
    bool setErrorLimit = false;
    int errorLimit = -1;

    if (args.contains("--ignore-uuid"))
    {
      args.removeAll("--ignore-uuid");
      ignoreUuid = true;
    }

    if (args.contains("--use-datetime"))
    {
      args.removeAll("--use-datetime");
      useDateTime = true;
    }

    if (args.contains("--error-limit"))
    {
      const int errorLimitIndex = args.indexOf("--error-limit");
      bool ok = false;
      errorLimit = args.at(errorLimitIndex + 1).trimmed().toInt(&ok);
      if (!ok)
      {
        throw IllegalArgumentException("Invalid error limit: " + args.at(errorLimitIndex + 1));
      }
      args.removeAt(errorLimitIndex + 1);
      args.removeAt(errorLimitIndex);
      setErrorLimit = true;
    }

    if (args.size() != 2)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes two parameters.").arg(getName()));
    }

    // We always want to know if there are duplicate nodes during comparison.
    conf().set(ConfigOptions::getMapMergeIgnoreDuplicateIdsKey(), true);

    QString pathname1 = args[0];
    QString pathname2 = args[1];

    int result = 1;
    //  Compare changesets differently than all other types
    if (pathIsChangeset(pathname1) && pathIsChangeset(pathname2))
    {
      XmlChangeset changeset1(pathname1);
      XmlChangeset changeset2(pathname2);

      if (changeset1.isMatch(changeset2))
        result = 0;
    }
    else
    {
      MapComparator mapCompare;
      if (ignoreUuid)
        mapCompare.setIgnoreUUID();
      if (useDateTime)
        mapCompare.setUseDateTime();
      if (setErrorLimit)
        mapCompare.setErrorLimit(errorLimit);

      OsmMapPtr map1(new OsmMap());
      IoUtils::loadMap(map1, pathname1, true, Status::Unknown1);
      //  Some maps that don't have IDs cooked in will fail comparison if the IDs aren't reset
      OsmMap::resetCounters();
      OsmMapPtr map2(new OsmMap());
      IoUtils::loadMap(map2, pathname2, true, Status::Unknown1);

      if (mapCompare.isMatch(map1, map2))
        result = 0;
    }

    return result;
  }

  bool pathIsChangeset(const QString& path)
  {
    QFileInfo fi(path);
    //  .osc files
    if (fi.exists() && path.endsWith(".osc", Qt::CaseInsensitive))
      return true;
    //  Directories containing .osc files
    if (fi.isDir())
    {
      QDir dir(path);
      dir.setFilter(QDir::Files | QDir::NoDotAndDotDot);
      dir.setSorting(QDir::Name | QDir::IgnoreCase);
      QStringList filters;
      filters << "*.osc";
      dir.setNameFilters(filters);
      //  As long as the directory contains at least one .osc file
      return dir.entryInfoList().size() > 0;
    }
    return false;
  }
};

HOOT_FACTORY_REGISTER(Command, DiffCmd)

}

