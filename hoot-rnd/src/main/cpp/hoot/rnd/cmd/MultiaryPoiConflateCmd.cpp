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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/conflate/matching/MatchFactory.h>
#include <hoot/core/conflate/merging/MergerFactory.h>
#include <hoot/core/conflate/UnifyingConflator.h>
#include <hoot/core/ops/NamedOp.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/visitors/CalculateHashVisitor.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Log.h>
#include <hoot/rnd/conflate/multiary/MultiaryUtilities.h>
#include <hoot/core/util/IoUtils.h>

// Qt
#include <QFileInfo>
#include <QStringList>

// Standard
#include <fstream>

// Tgs
#include <tgs/System/Timer.h>

using namespace std;
using namespace Tgs;

namespace hoot
{

class MultiaryConflatePoiCmd : public BaseCommand
{
public:

  static string className() { return "hoot::MultiaryConflatePoiCmd"; }

  MultiaryConflatePoiCmd() {}

  virtual QString getName() const { return "multiary-poi-conflate"; }

  virtual QString getDescription() const
  { return "Conflates two or more POI maps into a single map (experimental) "; }

  virtual QString getType() const { return "rnd"; }

  int runSimple(QStringList args) override
  {
    Timer totalTime;

    QStringList inputs;
    QString output;

    if (args.size() < 3)
    {
      throw HootException(
        "Expected at least three parameters (input1, input2, ..., output)");
    }

    inputs = args.mid(0, args.length() - 1);
    output = args.last();

    LOG_INFO(
      "Conflating " << inputs.join(", ") <<
      " and writing the output to " << output.right(50));

    // read input 1
    OsmMapPtr map(new OsmMap());

    // load all the inputs into a single map
    for (int i = 0; i < inputs.size(); ++i)
    {
      IoUtils::loadMap(map, inputs[i], false, Status::fromInput(i));
    }

    CalculateHashVisitor hashVisitor;
    map->visitRw(hashVisitor);

    LOG_INFO("Applying pre-conflation operations...");
    NamedOp(ConfigOptions().getConflatePreOps()).apply(map);

    MultiaryUtilities::conflate(map);

    // Apply any user specified operations.
    LOG_INFO("Applying post-conflation operations...");
    NamedOp(ConfigOptions().getConflatePostOps()).apply(map);

    MapProjector::projectToWgs84(map);

    map->visitRw(hashVisitor);

    IoUtils::saveMap(map, output);

    LOG_INFO("Total time elapsed: " << totalTime.getElapsed());

    LOG_INFO("Conflation job completed.");

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, MultiaryConflatePoiCmd)

}
