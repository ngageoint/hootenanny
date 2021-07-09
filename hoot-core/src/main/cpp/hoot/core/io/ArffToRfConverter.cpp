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
 * @copyright Copyright (C) 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */

#include "ArffToRfConverter.h"

// Hoot
#include <hoot/core/io/ArffReader.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/scoring/DataSamples.h>

// Standard
#include <fstream>
#include <iostream>

// Tgs
#include <tgs/RandomForest/RandomForest.h>
#include <tgs/Statistics/Random.h>
#include <tgs/System/DisableCout.h>

namespace hoot
{

void ArffToRfConverter::convert(const QString& input, const QString& output)
{
  LOG_INFO("Converting from " << input << " to " << output << "...");

  ArffReader ar(input);

  std::shared_ptr<Tgs::DataFrame> df = ar.read()->toDataFrame(-1);

  Tgs::Random::instance()->seed(0);
  LOG_INFO("Building Random Forest...");
  Tgs::RandomForest rf;
  std::shared_ptr<Tgs::DisableCout> dc;
  if (Log::getInstance().getLevel() >= Log::Warn)
  {
    // disable the printing of "Trained Tree ..."
    dc.reset(new Tgs::DisableCout());
  }
  const int numFactors =
    std::min(df->getNumFactors(), std::max<unsigned int>(3, df->getNumFactors() / 5));
  rf.trainMulticlass(df, ConfigOptions().getRandomForestModelTrees(), numFactors);
  dc.reset();

  double error;
  double sigma;
  rf.findAverageError(df, error, sigma);
  LOG_DEBUG("Error: " << error << " sigma: " << sigma);

  std::ofstream fileStream;
  fileStream.open(output.toStdString().data());
  rf.exportModel(fileStream);
  fileStream.close();
}

}
