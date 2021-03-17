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
 * @copyright Copyright (C) 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/conflate/matching/MatchThreshold.h>
#include <hoot/core/conflate/UnifyingConflator.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/ops/OpExecutor.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/rnd/conflate/multiary/MultiaryUtilities.h>
#include <hoot/rnd/scoring/multiary/MultiaryMatchComparator.h>
#include <hoot/rnd/visitors/MultiaryPoiHashVisitor.h>

// Qt
#include <QElapsedTimer>

using namespace std;
using namespace Tgs;

namespace hoot
{

class MultiaryScorePoiMatchesCmd : public BaseCommand
{

public:

  static QString className() { return "hoot::MultiaryScorePoiMatchesCmd"; }

  MultiaryScorePoiMatchesCmd() = default;

  QString evaluateThreshold(OsmMapPtr map, QString output,
    std::shared_ptr<MatchThreshold> mt, bool showConfusion)
  {
    MultiaryMatchComparator comparator;
    comparator.setTranslationScript(_translator);

    QString result;

    OsmMapPtr copy(new OsmMap(map));

    // Apply any user specified operations.
    OpExecutor(ConfigOptions().getConflatePreOps()).apply(copy);

    MultiaryUtilities::conflate(copy);
    // Apply any user specified operations.
    OpExecutor(ConfigOptions().getConflatePostOps()).apply(copy);

    comparator.evaluateMatches(map, copy);

    MapProjector::projectToWgs84(copy);
    IoUtils::saveMap(copy, output);
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

  virtual QString getName() const override { return "multiary-score-poi-matches"; }

  virtual QString getDescription() const override
  { return "Scores the performance of multiary-poi-conflate against a manually matched map (experimental) "; }

  virtual QString getType() const override { return "rnd"; }

  virtual int runSimple(QStringList& args) override
  {
    QElapsedTimer timer;
    timer.start();

    bool showConfusion = false;
    if (args.contains("--confusion"))
    {
      args.removeAll("--confusion");
      showConfusion = true;
    }

    if (args.contains("--translator"))
    {
      int at = args.indexOf("--translator");
      args.removeAt(at);

      if (args.size() <= at)
      {
        throw HootException("Expected the translator path to be after --translator.");
      }
      _translator = args.at(at);
      args.removeAt(at);
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
      IoUtils::loadMap(map, args[i], false, s);
    }

    MultiaryPoiHashVisitor hashVisitor;
    hashVisitor.setIncludeCircularError(true);
    map->visitRw(hashVisitor);

    OsmMapWriterFactory::write(map, ConfPath::getHootHome() + "/tmp/score-matches-after-prep.osm");
    MapProjector::projectToPlanar(map);

    // Apparently, multiary will allow with > 1.0 review thresholds.
    std::shared_ptr<MatchThreshold> mt(new MatchThreshold(0.5, 0.5, 1.0, false));
    QString result = evaluateThreshold(map, output, mt, showConfusion);

    cout << result;

    LOG_STATUS(
      "Matches scored in " << StringUtils::millisecondsToDhms(timer.elapsed()) << " total.");

    return 0;
  }

private:
  QString _translator;

};

HOOT_FACTORY_REGISTER(Command, MultiaryScorePoiMatchesCmd)

}

