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
#include <hoot/core/io/ArffWriter.h>
#include <hoot/core/scoring/MatchFeatureExtractor.h>
#include <hoot/core/util/ConfigOptions.h>

// Standard
#include <fstream>
#include <iostream>

// Tgs
#include <tgs/RandomForest/RandomForest.h>
#include <tgs/Statistics/Random.h>
#include <tgs/System/DisableCout.h>

namespace hoot
{

class BuildModelCmd : public BaseCommand
{
public:

  static string className() { return "hoot::BuildModelCmd"; }

  BuildModelCmd() { }

  virtual QString getName() const { return "build-model"; }

  virtual int runSimple(QStringList args)
  {
    bool exportArffOnly = false;
    if (args.contains("--export-arff-only"))
    {
      args.removeAll("--export-arff-only");
      exportArffOnly = true;
    }

    if (args.size() < 3 || args.size() % 2 == 0)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes an odd number of parameters, at least three.").
                          arg(getName()));
    }

    QString output = args.last();

    if (output.endsWith(".rf"))
    {
      output = output.remove(output.size() - 3, 3);
    }

    MatchFeatureExtractor mfe;
    QStringList creators = ConfigOptions().getMatchCreators().split(";");
    for (int i = 0; i < creators.size(); i++)
    {
      QString creator = creators[i];
      QStringList args = creator.split(",");
      QString className = args[0];
      args.removeFirst();
      MatchCreator* mc =
        Factory::getInstance().constructObject<MatchCreator>(className);
      if (args.size() > 0)
      {
        mc->setArguments(args);
      }
      mfe.addMatchCreator(shared_ptr<MatchCreator>(mc));
    }

    for (int i = 0; i < args.size() - 1; i+=2)
    {
      LOG_INFO("Processing map : " << args[i] << " and " << args[i + 1]);
      shared_ptr<OsmMap> map(new OsmMap());

      loadMap(map, args[i], false, Status::Unknown1);
      loadMap(map, args[i + 1], false, Status::Unknown2);

      MapCleaner().apply(map);

      mfe.processMap(map);
    }

    ArffWriter aw(output + ".arff", true);
    aw.write(mfe.getSamples());
    if (exportArffOnly)
    {
      return 0;
    }

    // using -1 for null isn't ideal, but it doesn't seem to have a big impact on performance.
    // ideally we'll circle back and update RF to use null values.
    shared_ptr<DataFrame> df = mfe.getSamples().toDataFrame(-1);

    Tgs::Random::instance()->seed(0);
    RandomForest rf;
    auto_ptr<DisableCout> dc;
    if (Log::getInstance().getLevel() >= Log::Warn)
    {
      // disable the printing of "Trained Tree ..."
      dc.reset(new DisableCout());
    }
    int numFactors = min(df->getNumFactors(), max<unsigned int>(3, df->getNumFactors() / 5));
    LOG_INFO("Training on data with " << numFactors << " factors...");
    rf.trainMulticlass(df, 40, numFactors);
    dc.reset();

    double error;
    double sigma;
    rf.findAverageError(df, error, sigma);
    LOG_INFO("Error: " << error << " sigma: " << sigma);

    LOG_INFO("Writing .rf file...");
    ofstream fileStream;
    fileStream.open((output + ".rf").toStdString().data());
    rf.exportModel(fileStream);
    fileStream.close();

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, BuildModelCmd)

}

