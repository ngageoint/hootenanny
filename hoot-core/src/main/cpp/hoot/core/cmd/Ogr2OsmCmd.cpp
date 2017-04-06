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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/ops/NamedOp.h>
#include <hoot/core/io/OgrReader.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/util/Progress.h>
#include <hoot/core/visitors/SplitLongLinearWaysVisitor.h>
#include <hoot/core/util/ConfigOptions.h>

namespace hoot
{

class Ogr2OsmCmd : public BaseCommand
{
public:

  static string className() { return "hoot::Ogr2OsmCmd"; }

  Ogr2OsmCmd():
  _logWarnCount(0)
  {
  }

  virtual QString getName() const { return "ogr2osm"; }

  static QString opsKey() { return "ogr2osm.ops"; }

  virtual int runSimple(QStringList args)
  {
    if (args.size() < 3)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes at least three parameters.").arg(getName()));
    }

    int a = 0;
    long limit = -1;
    if (args[a] == "--limit")
    {
      a++;
      bool ok;
      limit = args[a++].toDouble(&ok);
      if (!ok)
      {
        throw HootException(QString("Error parsing limit. Expected a long integer and got: ").
          arg(args[a - 1]));
      }
    }
    QString translation = args[a++];
    QString output = args[a++];

    boost::shared_ptr<OsmMap> map(new OsmMap());
    OgrReader reader;
    reader.setLimit(limit);
    reader.setTranslationFile(translation);

    ConfigOptions configOptions;

    // define progress object
    Progress progress( getName() );
    progress.setReportType( configOptions.getProgressReportingFormat() );
    progress.setState("Running");

    for (int i = a; i < args.size(); i++)
    {
      QString input = args[i];

      if (input == "")
      {
        LOG_WARN("Got an empty layer, skipping.");
        continue;
      }

      QStringList layers;
      if (input.contains(";"))
      {
        QStringList list = input.split(";");
        input = list.at(0);
        layers.append(list.at(1));
      }
      else
      {
        layers = reader.getFilteredLayerNames(input);
        layers.sort();
      }

      if (layers.size() == 0)
      {
        if (_logWarnCount < ConfigOptions().getLogWarnMessageLimit())
        {
          LOG_WARN("Could not find any valid layers to read from in " + input + ".");
        }
        else if (_logWarnCount == ConfigOptions().getLogWarnMessageLimit())
        {
          LOG_WARN(typeid(this).name() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
        }
        _logWarnCount++;
      }

      // process the completion status report information first
      long featureCountTotal = 0;
      int undefinedCounts = 0;
      vector<float> progressWeights;
      for(int i=0; i<layers.size(); i++)
      {
        // simply open the file, get the meta feature count value, and close
        int featuresPerLayer = reader.getFeatureCount(input, layers[i]);
        progressWeights.push_back((float)featuresPerLayer);
        // cover the case where no feature count available efficiently
        if(featuresPerLayer == -1) undefinedCounts++;
        else featureCountTotal += featuresPerLayer;
      }

      int definedCounts = layers.size() - undefinedCounts;

      // determine weights for 3 possible cases
      if(undefinedCounts == layers.size())
      {
        for(int i=0;i<layers.size();i++) progressWeights[i]=1./(float)layers.size();
      }
      else if(definedCounts == layers.size())
      {
        for(int i=0;i<layers.size();i++) progressWeights[i] /= (float)featureCountTotal;
      }
      else
      {
        for(int i=0;i<layers.size();i++)
          if(progressWeights[i] == -1)
          {
            progressWeights[i] = (1./(float)definedCounts) * featureCountTotal;
          }
        // reset featurecount total and recalculate
        featureCountTotal = 0;
        for(int i=0;i<layers.size();i++) featureCountTotal += progressWeights[i];
        for(int i=0;i<layers.size();i++) progressWeights[i] /= (float)featureCountTotal;
      }

      // read each layer's data
      for (int i = 0; i < layers.size(); i++)
      {
        LOG_INFO("Reading: " + input + " " + layers[i]);
        progress.setTaskWeight( progressWeights[i] );
        reader.read(input, layers[i], map, progress);
      }
    }

    if (map->getNodes().size() == 0)
    {
      progress.set(1.0, "Failed", true, "After translation the map is empty.  Aborting.");
      throw HootException("After translation the map is empty. Aborting.");
    }

    MapProjector::projectToPlanar(map);

    // Apply any user specified operations.
    NamedOp(ConfigOptions().getOgr2osmOps()).apply(map);

    MapProjector::projectToWgs84(map);

    saveMap(map, output);

    progress.set(1.0, "Successful", true, "Finished successfully.");

    return 0;
  }

private:

  //this should be static, but there's no header file
  unsigned int _logWarnCount;

};

HOOT_FACTORY_REGISTER(Command, Ogr2OsmCmd)

}

