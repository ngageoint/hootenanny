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
 * @copyright Copyright (C) 2018, 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#include "RandomForestModelBuilder.h"

// Hoot
#include <hoot/core/conflate/matching/MatchCreator.h>
#include <hoot/core/io/ArffToRfConverter.h>
#include <hoot/core/io/ArffWriter.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/ops/MapCleaner.h>
#include <hoot/core/scoring/MatchFeatureExtractor.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/StringUtils.h>

// Standard
#include <fstream>

// Tgs
#include <tgs/RandomForest/RandomForest.h>
#include <tgs/Statistics/Random.h>
#include <tgs/System/DisableCout.h>

namespace hoot
{

void RandomForestModelBuilder::build(
  const QStringList& trainingData, QString& output, const bool exportArffOnly)
{
  if (output.endsWith(".rf"))
  {
    output = output.remove(output.size() - 3, 3);
  }

  LOG_STATUS("Building RF model ..." << FileUtils::toLogFormat(output, 25) << "...");

  MatchFeatureExtractor mfe;
  QStringList creators = ConfigOptions().getMatchCreators();
  for (int i = 0; i < creators.size(); i++)
  {
    QString creator = creators[i];
    QStringList creatorParts = creator.split(",");
    QString className = creatorParts[0];
    creatorParts.removeFirst();
    std::shared_ptr<MatchCreator> mc =
      Factory::getInstance().constructObject<MatchCreator>(className);
    if (!creatorParts.empty())
    {
      mc->setArguments(creatorParts);
    }
    mfe.addMatchCreator(mc);
  }

  int datasetPairCtr = 1;
  for (int i = 0; i < trainingData.size(); i += 2)
  {
    LOG_INFO(
      "Processing dataset pair " << datasetPairCtr << " of " << ((trainingData.size() - 1) / 2) <<
      ": " << trainingData[i].right(50) << " and " << trainingData[i + 1].right(50));
    datasetPairCtr++;
    OsmMapPtr map = std::make_shared<OsmMap>();

    IoUtils::loadMap(map, trainingData[i], false, Status::Unknown1);
    IoUtils::loadMap(map, trainingData[i + 1], false, Status::Unknown2);

    MapCleaner().apply(map);

    mfe.processMap(map);
  }
  LOG_INFO(
    "Processed " << StringUtils::formatLargeNumber(mfe.getSamples().size()) << " total samples.");

  ArffWriter aw(output + ".arff", true);
  aw.write(mfe.getSamples());
  if (exportArffOnly)
  {
    return;
  }

  // using -1 for null isn't ideal, but it doesn't seem to have a big impact on performance.
  // ideally we'll circle back and update RF to use null values.
  std::shared_ptr<Tgs::DataFrame> df = mfe.getSamples().toDataFrame(-1);

  Tgs::Random::instance()->seed(0);
  Tgs::RandomForest rf;
  std::shared_ptr<Tgs::DisableCout> dc;
  if (Log::getInstance().getLevel() >= Log::Warn)
  {
    // disable the printing of "Trained Tree ..."
    dc = std::make_shared<Tgs::DisableCout>();
  }
  const int numFactors =
    std::min(df->getNumFactors(), std::max<unsigned int>(3, df->getNumFactors() / 5));
  LOG_INFO("Training on data with " << numFactors << " factors...");
  //make the number of trees configurable?
  rf.trainMulticlass(df, ConfigOptions().getRandomForestModelTrees(), numFactors);
  dc.reset();

  double error;
  double sigma;
  rf.findAverageError(df, error, sigma);
  LOG_INFO("Error: " << error << " sigma: " << sigma);

  LOG_INFO("Writing .rf file...");
  std::ofstream fileStream;
  fileStream.open((output + ".rf").toStdString().data());
  rf.exportModel(fileStream);
  fileStream.close();
}

}
