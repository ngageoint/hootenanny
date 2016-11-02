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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/Factory.h>
#include <hoot/core/MapProjector.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/conflate/MatchThreshold.h>
#include <hoot/core/conflate/UnifyingConflator.h>
#include <hoot/core/ops/BuildingOutlineUpdateOp.h>
#include <hoot/core/ops/NamedOp.h>
#include <hoot/core/scoring/MatchComparator.h>
#include <hoot/core/scoring/MatchScoringMapPreparer.h>
#include <hoot/core/scoring/MapScoringStatusAndRefTagValidator.h>
#include <hoot/core/util/OsmUtils.h>
#include <hoot/core/util/Settings.h>
//#include <hoot/core/visitors/CountManualMatchesVisitor.h>

// tgs
#include <tgs/Optimization/NelderMead.h>

namespace hoot
{

class ScoreMatchesCmd : public BaseCommand
{

public:

  static string className() { return "hoot::ScoreMatchesCmd"; }

  ScoreMatchesCmd() { }

//  QString evaluateThreshold(vector<OsmMapPtr> maps, QString output, shared_ptr<MatchThreshold> mt,
//    bool showConfusion, double& score)
//  {
//    return evaluateThreshold(maps, output, mt, showConfusion, score, -1);
//  }

  QString evaluateThreshold(vector<OsmMapPtr> maps, QString output, shared_ptr<MatchThreshold> mt,
    bool showConfusion, double& score/*, long numManualMatches*/)
  {
    MatchComparator comparator;

    QString result;

    for (size_t i = 0; i < maps.size(); i++)
    {
      OsmMapPtr copy(new OsmMap(maps[i]));

      // Apply any user specified operations.
      NamedOp(ConfigOptions().getConflatePreOps()).apply(copy);
      UnifyingConflator(mt).apply(copy);
      // Apply any user specified operations.
      NamedOp(ConfigOptions().getConflatePostOps()).apply(copy);

      comparator.evaluateMatches(maps[i], copy);

      if (i == 0 && !output.isEmpty())
      {
        BuildingOutlineUpdateOp().apply(copy);
        MapProjector::projectToWgs84(copy);
        saveMap(copy, output);
      }
    }

    if (showConfusion)
    {
      if (mt)
      {
        cout << "Threshold: " << mt->toString() << endl;
      }
      cout << comparator.toString() /*<< endl*/;
//      if (numManualMatches != -1)
//      {
//        cout << QString("number of manual matches made: %1\n").arg(numManualMatches) << endl;
//      }
    }
    QString line = QString("%1,%2,%3,%4\n").arg(-1)
        .arg(comparator.getPercentCorrect())
        .arg(comparator.getPercentWrong())
        .arg(comparator.getPercentUnnecessaryReview());
    result += line;

    score = -comparator.getPercentWrong() * 5 - comparator.getPercentUnnecessaryReview();
    cout << "Score: " << score << endl;

    return result;
  }

  virtual QString getName() const { return "score-matches"; }

  class ScoreFunction : public Tgs::NelderMead::Function
  {
  public:
    virtual double f(Tgs::Vector v)
    {
      double score;
      shared_ptr<MatchThreshold> mt(new MatchThreshold(v[0], v[1], v[2]));
      _cmd->evaluateThreshold(_maps, "", mt, _showConfusion, score/*, -1*/);
      return score;
    }

    ScoreMatchesCmd* _cmd;
    vector<OsmMapPtr> _maps;
    bool _showConfusion;
  };

  void optimize(vector<OsmMapPtr> maps, bool showConfusion)
  {
    ScoreFunction* sf = new ScoreFunction();
    sf->_cmd = this;
    sf->_maps = maps;
    sf->_showConfusion = showConfusion;

    NelderMead optimizer(3, sf, 0.0001);

    Vector result;

    result = Vector(.01, .01, 0.01);
    optimizer.step(result, sf->f(result));
    result = Vector(0, 1, 0.01);
    optimizer.step(result, sf->f(result));
    result = Vector(.22, 1, .33);
    optimizer.step(result, sf->f(result));
    result = Vector(0, 0.8, 0.01);
    optimizer.step(result, sf->f(result));

    while (optimizer.done() == false)
    {
      result = optimizer.step(result, sf->f(result));
      result[0] = min(1., max(0., result[0]));
      result[1] = min(1., max(0., result[1]));
      result[2] = min(1., max(0., result[2]));
      LOG_VAR(result.getVector());
    }
  }

  virtual int runSimple(QStringList args)
  {
    bool optimizeThresholds = false;
    bool showConfusion = false;
    if (args.contains("--confusion"))
    {
      args.removeAll("--confusion");
      showConfusion = true;
    }

    if (args.contains("--optimize"))
    {
      args.removeAll("--optimize");
      optimizeThresholds = true;
    }

    if (args.size() < 3 || args.size() % 2 != 1)
    {
      LOG_VAR(args);
      cout << getHelp() << endl << endl;
      throw HootException(
        QString("%1 takes at least three parameters: two or more input maps (even number) and an output map")
          .arg(getName()));
    }

    vector<OsmMapPtr> maps;
    QString output = args.last();
    //for calculating the actual number of manual matches made
    //shared_ptr<OsmMap> ref2Map(new OsmMap());

    for (int i = 0; i < args.size() - 1; i+=2)
    {
      shared_ptr<OsmMap> map(new OsmMap());
      loadMap(map, args[i], false, Status::Unknown1);
      loadMap(map, args[i + 1], false, Status::Unknown2);
      //loadMap(ref2Map, args[i + 1], false, Status::Unknown2);

      if (!MapScoringStatusAndRefTagValidator::allTagsAreValid(map))
      {
        throw HootException(
          "score-matches requires that the first input file contains REF1 tags (no REF2 tags) "
          "and the second input file contains REF2 tags (no REF1 tags).");
      }

      shared_ptr<OsmMap> mapCopy(map);
      MapProjector::projectToWgs84(mapCopy);
      OsmUtils::saveMap(mapCopy, "/tmp/score-matches-before-prep.osm");

      MatchScoringMapPreparer().prepMap(map, ConfigOptions().getScoreMatchesRemoveNodes());
      maps.push_back(map);
    }

    //This logic is oddly affecting some test scores.  Since this isn't a critical feature,
    //disabling it for now.  #1185 created to fix it.
    //shared_ptr<CountManualMatchesVisitor> manualMatchVisitor(new CountManualMatchesVisitor());
    //ref2Map->visitRo(*manualMatchVisitor);
    //const long numManualMatches = manualMatchVisitor->getStat();

    LOG_VARD(maps.size());
    shared_ptr<OsmMap> mapCopy(maps[0]);
    MapProjector::projectToWgs84(mapCopy);
    OsmUtils::saveMap(mapCopy, "/tmp/score-matches-after-prep.osm");

    if (optimizeThresholds)
    {
      optimize(maps, showConfusion);
    }
    else
    {
      double score;
      shared_ptr<MatchThreshold> mt;
      QString result = evaluateThreshold(maps, output, mt, showConfusion, score/*, numManualMatches*/);

      cout << result;
    }

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, ScoreMatchesCmd)

}

