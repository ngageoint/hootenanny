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
  const QStringList& inputs, QString& visitorClassName, const QString& statType) const
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

  if (!visitorClassName.startsWith(MetadataTags::HootNamespacePrefix()))
  {
    visitorClassName.prepend(MetadataTags::HootNamespacePrefix());
  }
  if (!_isValidStatType(statType))
  {
    throw IllegalArgumentException("Invalid statistic type: " + statType);
  }

  ElementVisitorPtr statCollector = _getStatCollector(visitorClassName);
  std::shared_ptr<SingleStatistic> singleStatCtr =
    std::dynamic_pointer_cast<SingleStatistic>(statCollector);
  LOG_VART(singleStatCtr.get());
  if (!singleStatCtr.get())
  {
    throw IllegalArgumentException(
      "Visitors passed to the stat command must support the SingleStatistic interface.");
  }
  std::shared_ptr<NumericStatistic> numericStatCtr;
  if (statType != "total")
  {
    numericStatCtr = std::dynamic_pointer_cast<NumericStatistic>(singleStatCtr);
    if (!numericStatCtr.get())
    {
      throw IllegalArgumentException(
        "Visitors passed to the stat command with a statistic type other than \"total\" must "
         "support the NumericStatistic interface.");
    }
    // TODO
    if (inputs.size() > 1 && statType == "average")
    {
      throw IllegalArgumentException(
        "An average statistic may only be calculated against a single input.");
    }
  }

  double stat = 0.0;
  double statMin = std::numeric_limits<double>::max();
  double statMax = std::numeric_limits<double>::min();
  double statAvg = 0.0;
  for (int i = 0; i < inputs.size(); i++)
  {
    LOG_STATUS(
      "Calculating statistic of type: " << statType << ", using visitor: " << visitorClassName <<
      ", against " << FileUtils::toLogFormat(inputs.at(i), 25) << "...");

    std::shared_ptr<PartialOsmMapReader> reader = _getReader(inputs.at(i));
    ElementInputStreamPtr filteredInputStream =
      std::make_shared<ElementVisitorInputStream>(
        std::dynamic_pointer_cast<ElementInputStream>(reader), statCollector);

    long numElementsParsed = 0;
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

    if (numericStatCtr)
    {
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
        // TODO
        statAvg = numericStatCtr->getAverage();
      }
      else
      {
        // won't get here
        throw HootException("");
      }
    }
    else
    {
      stat += singleStatCtr->getStat();
    }

    reader->finalizePartial();
    reader->close();
    filteredInputStream->close();
  }

  LOG_STATUS(
    "Statistic calculated in " << StringUtils::millisecondsToDhms(timer.elapsed()) << " total.");

  if (numericStatCtr)
  {
    if (statType == "min")
    {
      return statMin;
    }
    else if (statType == "max")
    {
      return statMax;
    }
    else if (statType == "average")
    {
      return statAvg;
    }
    else
    {
      // won't get here
      throw HootException("");
    }
  }
  else
  {
    return stat;
  }
}

bool StatCalculator::_isValidStatType(const QString& statType) const
{
  return statType == "total" || statType == "min" || statType == "max" || statType == "average";
}

std::shared_ptr<PartialOsmMapReader> StatCalculator::_getReader(const QString& input) const
{
  std::shared_ptr<PartialOsmMapReader> reader =
    std::dynamic_pointer_cast<PartialOsmMapReader>(
      OsmMapReaderFactory::createReader(input));
  reader->setUseDataSourceIds(true);
  reader->open(input);
  reader->initializePartial();
  return reader;
}

ElementVisitorPtr StatCalculator::_getStatCollector(const QString& visClassName) const
{
  std::shared_ptr<ElementVisitor> statsCollector;
  try
  {
    statsCollector.reset(Factory::getInstance().constructObject<ElementVisitor>(visClassName));
  }
  catch (const boost::bad_any_cast&)
  {
    throw IllegalArgumentException("Invalid visitor: " + visClassName);
  }

  std::shared_ptr<Configurable> visConfig =
    std::dynamic_pointer_cast<Configurable>(statsCollector);
  LOG_VART(visConfig.get());
  if (visConfig.get())
  {
    visConfig->setConfiguration(conf());
  }

  return statsCollector;
}

}
