
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

void ArffToRfConverter::convert(const QString input, const QString output)
{
  LOG_INFO("Converting from " << input << " to " << output << "...");

  ArffReader ar(input);

  boost::shared_ptr<Tgs::DataFrame> df = ar.read()->toDataFrame(-1);

  Tgs::Random::instance()->seed(0);
  LOG_INFO("Building Random Forest...");
  Tgs::RandomForest rf;
  boost::shared_ptr<Tgs::DisableCout> dc;
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
  LOG_INFO("Error: " << error << " sigma: " << sigma);

  std::ofstream fileStream;
  fileStream.open((output).toStdString().data());
  rf.exportModel(fileStream);
  fileStream.close();
}

}
