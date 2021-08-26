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

// Hoot
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/conflate/matching/MatchScorer.h>
#include <hoot/core/util/Factory.h>

using namespace std;
using namespace Tgs;

namespace hoot
{

class ScoreMatchesCmd : public BaseCommand
{

public:

  static QString className() { return "hoot::ScoreMatchesCmd"; }

  ScoreMatchesCmd() = default;

  QString getName() const override { return "score-matches"; }
  QString getDescription() const override
  { return "Scores conflation performance against a manually matched map"; }

  int runSimple(QStringList& args) override
  {
    bool showConfusion = false;
    if (args.contains("--confusion"))
    {
      args.removeAll("--confusion");
      showConfusion = true;
    }
    bool optimizeThresholds = false;
    if (args.contains("--optimize"))
    {
      args.removeAll("--optimize");
      optimizeThresholds = true;
    }
    bool validateManualMatches = true;
    if (args.contains("--validation-off"))
    {
      args.removeAll("--validation-off");
      validateManualMatches = false;
    }

    // With optimization on, no data is output.
    if (!optimizeThresholds)
    {
      if (args.size() < 3 || args.size() % 2 != 1)
      {
        LOG_VAR(args);
        cout << getHelp() << endl << endl;
        throw HootException(
          QString("%1 takes at least three parameters: one or more pairs of input maps and an output map")
            .arg(getName()));
      }
    }
    else
    {
      if (args.size() < 2 || args.size() % 2 != 0)
      {
        LOG_VAR(args);
        cout << getHelp() << endl << endl;
        throw HootException(
          QString("%1 takes at least two parameters: one or more pairs of input maps")
            .arg(getName()));
      }
    }

    QString output;
    if (!optimizeThresholds)
    {
      output = args.last();
    }

    // Load the maps as pairs.
    QStringList ref1Inputs;
    QStringList ref2Inputs;
    for (int i = 0; i < args.size() - 1; i += 2)
    {
      ref1Inputs.append(args[i]);
      ref2Inputs.append(args[i + 1]);
    }

    MatchScorer matchScorer;
    matchScorer.setPrintConfusion(showConfusion);
    matchScorer.setOptimizeThresholds(optimizeThresholds);
    matchScorer.setValidateManualMatches(validateManualMatches);
    matchScorer.score(ref1Inputs, ref2Inputs, output);

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, ScoreMatchesCmd)

}

