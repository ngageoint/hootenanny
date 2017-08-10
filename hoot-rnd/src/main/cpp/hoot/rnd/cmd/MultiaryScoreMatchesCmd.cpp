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
 * @copyright Copyright (C) 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/conflate/MatchThreshold.h>
#include <hoot/core/conflate/UnifyingConflator.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/ops/NamedOp.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/scoring/multiary/MultiaryMatchComparator.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/visitors/CalculateHashVisitor.h>
#include <hoot/rnd/conflate/multiary/MultiaryUtilities.h>

using namespace std;
using namespace Tgs;

namespace hoot
{

class MultiaryScoreMatchesCmd : public BaseCommand
{

public:

  static string className() { return "hoot::MultiaryScoreMatchesCmd"; }

  MultiaryScoreMatchesCmd() { }

  QString evaluateThreshold(OsmMapPtr map, QString output,
    boost::shared_ptr<MatchThreshold> mt, bool showConfusion)
  {
    MultiaryMatchComparator comparator;

    QString result;

    OsmMapPtr copy(new OsmMap(map));

    // Apply any user specified operations.
    NamedOp(ConfigOptions().getConflatePreOps()).apply(copy);

    MultiaryUtilities::conflate(copy);
    // Apply any user specified operations.
    NamedOp(ConfigOptions().getConflatePostOps()).apply(copy);

    comparator.evaluateMatches(map, copy);

    MapProjector::projectToWgs84(copy);
    saveMap(copy, output);
    OsmXmlWriter writer;
    writer.setIncludeHootInfo(true);
    writer.write(copy, output);

    if (showConfusion)
    {
      if (mt)
      {
        cout << "Threshold: " << mt->toString() << endl;
      }
      cout << comparator.toString() /*<< endl*/;
    }
    QString line = QString("%1,%2,%3,%4\n").arg(-1)
        .arg(comparator.getPercentCorrect())
        .arg(comparator.getPercentWrong())
        .arg(comparator.getPercentUnnecessaryReview());
    result += line;

    cout << "F1 Score: " << comparator.getFScore(1) << endl;

    return result;
  }

  virtual QString getName() const { return "multiary-score-matches"; }

  virtual int runSimple(QStringList args)
  {
    bool showConfusion = false;
    if (args.contains("--confusion"))
    {
      args.removeAll("--confusion");
      showConfusion = true;
    }

    if (args.size() < 3)
    {
      LOG_VAR(args);
      cout << getHelp() << endl << endl;
      throw HootException(
        QString("%1 takes at least two parameters: two or more input maps")
          .arg(getName()));
    }

    // modifying the schema is necessary to ensure the conflation concatenates values.
    SchemaVertex id;
    id.setName("ID");
    id.setType(SchemaVertex::Tag);
    id.valueType = Text;
    OsmSchema::getInstance().updateOrCreateVertex(id);

    SchemaVertex match;
    id.setName("MATCH");
    id.setType(SchemaVertex::Tag);
    id.valueType = Text;
    OsmSchema::getInstance().updateOrCreateVertex(match);

    SchemaVertex review;
    id.setName("REVIEW");
    id.setType(SchemaVertex::Tag);
    id.valueType = Text;
    OsmSchema::getInstance().updateOrCreateVertex(review);

    QString output = args.last();
    OsmMapPtr map(new OsmMap());

    for (int i = 0; i < args.size() - 1; i++)
    {
      Status s = Status::fromInput(i);

      loadMap(map, args[i], false, s);
    }

    CalculateHashVisitor hashVisitor;
    map->visitRw(hashVisitor);

    OsmMapWriterFactory::write(map, "/tmp/score-matches-after-prep.osm");
    MapProjector::projectToPlanar(map);

    boost::shared_ptr<MatchThreshold> mt;
    QString result = evaluateThreshold(map, output, mt, showConfusion);

    cout << result;

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, MultiaryScoreMatchesCmd)

}

