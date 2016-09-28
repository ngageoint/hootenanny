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
#include <hoot/core/conflate/MapCleaner.h>
#include <hoot/core/conflate/MatchCreator.h>
#include <hoot/core/conflate/polygon/BuildingMatchCreator.h>
#include <hoot/core/io/ArffReader.h>
#include <hoot/core/scoring/MatchFeatureExtractor.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Log.h>

// Standard
#include <fstream>
#include <iostream>

// Tgs
#include <tgs/RandomForest/RandomForestUtilities.h>
#include <tgs/RandomForest/MultithreadedRandomForestManager.h>
#include <tgs/System/DisableCout.h>

namespace hoot
{

class BuildRfCmd : public BaseCommand
{
public:

  static string className() { return "hoot::BuildRfCmd"; }

  BuildRfCmd() { }

  virtual QString getName() const { return "build-rf"; }

  virtual int runSimple(QStringList args)
  {
    if (args.size() < 2)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes two parameters.").
                          arg(getName()));
    }

    QString input = args[0];
    QString output = args[1];

    ArffReader ar(input);

    shared_ptr<DataFrame> df = ar.read()->toDataFrame(-1);

    srand(0);

    #warning remove these qHash log messages.
    LOG_VAR(qHash(df->toXmlString()));

    LOG_DEBUG("Building Random Forest");
    MultithreadedRandomForest mrf;

    auto_ptr<DisableCout> dc;
    if (Log::getInstance().getLevel() >= Log::Warn)
    {
      // disable the printing of "Trained Tree ..."
      dc.reset(new DisableCout());
    }
    int numFactors = min(df->getNumFactors(), max<unsigned int>(3, df->getNumFactors() / 5));

    LOG_VAR(qHash(df->toXmlString()));

    mrf.trainMulticlass(df, 40, numFactors);
    LOG_VAR(qHash(df->toXmlString()));
    dc.reset();

    double error;
    double sigma;
    mrf.findAverageError(df, error, sigma);
    LOG_VAR(qHash(df->toXmlString()));
    LOG_INFO("Error: " << error << " sigma: " << sigma);

    ofstream fileStream;
    fileStream.open((output).toStdString().data());
    mrf.exportModel(fileStream);
    fileStream.close();

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, BuildRfCmd)

}

