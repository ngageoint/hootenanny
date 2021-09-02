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
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
 */
#include "StatCalculator.h"

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/info/SingleStatistic.h>
#include <hoot/core/info/NumericStatistic.h>
#include <hoot/core/io/ElementVisitorInputStream.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/io/OsmMapReader.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/PartialOsmMapReader.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/StringUtils.h>

// Qt
#include <QElapsedTimer>

namespace hoot
{

StatCalculator::StatCalculator() :
_taskStatusUpdateInterval(ConfigOptions().getTaskStatusUpdateInterval())
{
}

double StatCalculator::calculateStat(
  const QStringList& inputs, const QString& visitorClassName, const QString& statType) const
{
  if (inputs.empty())
  {
    throw IllegalArgumentException("No inputs passed to StatCalculator.");
  }

  QElapsedTimer timer;
  timer.start();
  LOG_STATUS(
    "Calculating statistic of type: " << statType << ", using visitor: " << visitorClassName <<
    ", against " << inputs.size() << " inputs...");

  if (!_isValidStatType(statType))
  {
    throw IllegalArgumentException("Invalid statistic type: " + statType);
  }

  ConstElementVisitorPtr statCollector = _getStatCollector(statType, visitorClassName);
  double stat = -1.0;
  if (IoUtils::areStreamableInputs(inputs))
  {
    stat = _calcStatStreaming(inputs, statCollector, statType);
  }
  else
  {
    stat = _calcStatMemoryBound(inputs, statCollector, statType);
  }

  LOG_STATUS(
    "Statistic calculated in " << StringUtils::millisecondsToDhms(timer.elapsed()) << " total.");
  return stat;
}

bool StatCalculator::_isValidStatType(const QString& statType) const
{
  return statType == "total" || statType == "min" || statType == "max" || statType == "average";
}

std::shared_ptr<PartialOsmMapReader> StatCalculator::_getStreamableReader(
  const QString& input) const
{
  std::shared_ptr<PartialOsmMapReader> reader =
    std::dynamic_pointer_cast<PartialOsmMapReader>(
      OsmMapReaderFactory::createReader(input));
  reader->setUseDataSourceIds(true);
  reader->open(input);
  reader->initializePartial();
  return reader;
}

ConstElementVisitorPtr StatCalculator::_getStatCollector(
  const QString& statType, const QString& visClassName) const
{
  const QString errorMsg = "Invalid visitor: " + visClassName;
  std::shared_ptr<ConstElementVisitor> statCollector;
  ElementVisitorPtr vis;
  try
  {
    vis = Factory::getInstance().constructObject<ElementVisitor>(visClassName);
  }
  catch (const boost::bad_any_cast&)
  {
    throw IllegalArgumentException(errorMsg);
  }
  statCollector = std::dynamic_pointer_cast<ConstElementVisitor>(vis);
  if (!statCollector)
  {
    throw IllegalArgumentException(errorMsg);
  }

  std::shared_ptr<SingleStatistic> singleStatCtr =
    std::dynamic_pointer_cast<SingleStatistic>(statCollector);
  LOG_VART(singleStatCtr.get());
  if (!singleStatCtr.get())
  {
    throw IllegalArgumentException(
      "Visitors passed to the stat command must support the SingleStatistic interface.");
  }
  std::shared_ptr<NumericStatistic> numericStatCtr;
  if (statType != "total") // Use SingleStatistic for the total stat.
  {
    numericStatCtr = std::dynamic_pointer_cast<NumericStatistic>(singleStatCtr);
    if (!numericStatCtr.get())
    {
      throw IllegalArgumentException(
        "Visitors passed to the stat command with a statistic type other than \"total\" must "
         "support the NumericStatistic interface.");
    }
  }

  std::shared_ptr<Configurable> visConfig =
    std::dynamic_pointer_cast<Configurable>(statCollector);
  LOG_VART(visConfig.get());
  if (visConfig.get())
  {
    visConfig->setConfiguration(conf());
  }

  return statCollector;
}

double StatCalculator::_calcStatMemoryBound(
  const QStringList& inputs, const ConstElementVisitorPtr& statCollector,
  const QString& statType) const
{
  LOG_STATUS(
    "Calculating memory bound statistic of type: " << statType << ", using visitor: " <<
    statCollector->getClassName() << ", against ..." << inputs.size() << " inputs...");

  OsmMapPtr map = std::make_shared<OsmMap>();
  // Don't read in file IDs or duplicated elements across inputs won't be counted.
  IoUtils::loadMaps(map, inputs, false);

  map->visitRo(*statCollector);

  std::shared_ptr<SingleStatistic> singleStatCtr =
    std::dynamic_pointer_cast<SingleStatistic>(statCollector);
  std::shared_ptr<NumericStatistic> numericStatCtr;
  if (statType != "total") // Use SingleStatistic for the total stat.
  {
    numericStatCtr = std::dynamic_pointer_cast<NumericStatistic>(singleStatCtr);
  }

  if (statType == "min")
  {
    return numericStatCtr->getMin();
  }
  else if (statType == "max")
  {
    return numericStatCtr->getMax();
  }
  else if (statType == "average")
  {
    return numericStatCtr->getAverage();
  }
  else if (statType == "total")
  {
    return singleStatCtr->getStat();
  }
  else
  {
    // shouldn't get here
    throw HootException("Invalid statistic type.");
  }
}

double StatCalculator::_calcStatStreaming(
  const QStringList& inputs, const ConstElementVisitorPtr& statCollector,
  const QString& statType) const
{
  std::shared_ptr<SingleStatistic> singleStatCtr =
    std::dynamic_pointer_cast<SingleStatistic>(statCollector);
  std::shared_ptr<NumericStatistic> numericStatCtr;
  if (statType != "total") // Use SingleStatistic for the total stat.
  {
    numericStatCtr = std::dynamic_pointer_cast<NumericStatistic>(singleStatCtr);
  }

  double stat = 0.0;
  double statMin = std::numeric_limits<double>::max();
  double statMax = std::numeric_limits<double>::min();
  long numElementsParsed = 0;
  long numWithStat = 0;
  for (int i = 0; i < inputs.size(); i++)
  {
    LOG_STATUS(
      "Calculating streaming statistic of type: " << statType << ", using visitor: " <<
      statCollector->getClassName() << ", against ..." <<
      FileUtils::toLogFormat(inputs.at(i), 25) << "...");

    std::shared_ptr<PartialOsmMapReader> reader = _getStreamableReader(inputs.at(i));
    ElementInputStreamPtr filteredInputStream =
      std::make_shared<ElementVisitorInputStream>(
        std::dynamic_pointer_cast<ElementInputStream>(reader), statCollector);

    while (filteredInputStream->hasMoreElements())
    {
      /*ConstElementPtr element = */filteredInputStream->readNextElement();
      numElementsParsed++;

      if (numElementsParsed % (_taskStatusUpdateInterval * 10) == 0)
      {
        PROGRESS_INFO(
          "Calculated statistic for: " << StringUtils::formatLargeNumber(numElementsParsed) <<
          " elements.");
      }
    }

    if (statType == "min")
    {
      statMin = std::min(statMin, numericStatCtr->getMin());
    }
    else if (statType == "max")
    {
      statMax = std::max(statMax, numericStatCtr->getMax());
    }
    else if (statType == "average")
    {
      // numericStatCtr->getAverage() returns the average for the current file being parsed, but
      // we want to calculate the average for all files, so just record the current total here and
      // we'll calc the avg after all files have been parsed.
      stat += singleStatCtr->getStat();
      numWithStat += numericStatCtr->numWithStat();
    }
    else if (statType == "total")
    {
      stat += singleStatCtr->getStat();
    }
    else
    {
      // shouldn't get here
      throw HootException("Invalid statistic type.");
    }

    reader->finalizePartial();
    reader->close();
    filteredInputStream->close();
  }

  if (statType == "min")
  {
    return statMin;
  }
  else if (statType == "max")
  {
    return statMax;
  }
  else if (statType == "average")
  {;
    if (numWithStat > 0)
    {
      return stat / (double)numWithStat;
    }
    return 0.0;
  }
  else if (statType == "total")
  {
    return stat;
  }
  else
  {
    // won't get here
    throw HootException("Invalid statistic type.");
  }
}

}
