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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/conflate/matching/MatchFactory.h>
#include <hoot/core/conflate/merging/MergerFactory.h>
#include <hoot/core/conflate/UnifyingConflator.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/ops/OpExecutor.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/conflate/multiary/MultiaryUtilities.h>
#include <hoot/core/visitors/MultiaryPoiHashVisitor.h>

// Qt
#include <QElapsedTimer>

// Standard
#include <fstream>

using namespace std;
using namespace Tgs;

namespace hoot
{

class MultiaryConflatePoiCmd : public BaseCommand
{
public:

  static QString className() { return "MultiaryConflatePoiCmd"; }

  MultiaryConflatePoiCmd() = default;

  QString getName() const override { return "multiary-poi-conflate"; }
  QString getDescription() const override
  { return "Conflates two or more POI maps into a single map (experimental) "; }
  QString getType() const override { return "advanced"; }

  int runSimple(QStringList& args) override
  {
    QElapsedTimer timer;
    timer.start();

    QStringList inputs;
    QString output;

    if (args.size() < 3)
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw IllegalArgumentException(
        QString("%1 takes at least three parameters. You provided %2: %3")
          .arg(getName())
          .arg(args.size())
          .arg(args.join(",")));
    }

    inputs = args.mid(0, args.length() - 1);
    output = args.last();

    LOG_STATUS(
      "Conflating " << FileUtils::toLogFormat(inputs) <<
      " and writing the output to " << FileUtils::toLogFormat(output, 50) << "...");

    OsmMapPtr map = std::make_shared<OsmMap>();
    // Load all the inputs into a single map.
    for (int i = 0; i < inputs.size(); ++i)
    {
      IoUtils::loadMap(map, inputs[i], false, Status::fromInput(i));
    }

    MultiaryPoiHashVisitor hashVisitor;
    hashVisitor.setIncludeCircularError(true);
    map->visitRw(hashVisitor);

    LOG_INFO("Applying pre-conflation operations...");
    OpExecutor(ConfigOptions().getConflatePreOps()).apply(map);

    MultiaryUtilities::conflate(map);

    // Apply any user specified operations.
    LOG_INFO("Applying post-conflation operations...");
    OpExecutor(ConfigOptions().getConflatePostOps()).apply(map);

    MapProjector::projectToWgs84(map);
    map->visitRw(hashVisitor);
    IoUtils::saveMap(map, output);

    LOG_STATUS(
      "Conflation completed in " << StringUtils::millisecondsToDhms(timer.elapsed()) << " total.");

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, MultiaryConflatePoiCmd)

}
