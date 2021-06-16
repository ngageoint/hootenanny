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
#include "ElementCounter.h"

// Hoot
#include <hoot/core/criterion/CriterionUtils.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/visitors/ElementCountVisitor.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/visitors/FeatureCountVisitor.h>
#include <hoot/core/io/PartialOsmMapReader.h>
#include <hoot/core/io/ElementCriterionVisitorInputStream.h>
#include <hoot/core/io/ElementVisitorInputStream.h>
#include <hoot/core/util/DbUtils.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/visitors/FilteredVisitor.h>

// Qt
#include <QFileInfo>
#include <QElapsedTimer>

namespace hoot
{

ElementCounter::ElementCounter() :
_countFeaturesOnly(true),
_isStreamableCrit(false),
_total(0),
_taskStatusUpdateInterval(ConfigOptions().getTaskStatusUpdateInterval())
{
}

void ElementCounter::setCriteria(QStringList& names)
{
  if (!names.isEmpty())
  {
    ConfigOptions opts;
    // Test crit here to see if I/O can be streamed or not. If it requires a map, then it can't be
    // streamed.
    _isStreamableCrit = false;
    _crit =
      CriterionUtils::constructCriterion(
        names, opts.getElementCriteriaChain(), opts.getElementCriteriaNegate(), _isStreamableCrit);
  }
  else
  {
    _isStreamableCrit = true;
  }
}

long ElementCounter::count(const QStringList& inputs)
{
  if (inputs.empty())
  {
    throw IllegalArgumentException("No inputs available for element counting.");
  }

  _checkForMissingInputs(inputs);

  QElapsedTimer timer;
  timer.start();

  QString critStr;
  if (_crit)
  {
    critStr = _crit->toString();
  }
  else
  {
    _isStreamableCrit = true;
  }

  if (_isStreamableCrit && IoUtils::areStreamableInputs(inputs))
  {
    for (int i = 0; i < inputs.size(); i++)
    {
      LOG_STATUS(_getStatusMessage(inputs.at(i), critStr));
      _total += _countStreaming(inputs.at(i), _crit);
    }
  }
  else
  {
    LOG_STATUS(_getStatusMessage(inputs.size(), critStr));
    _total += _countMemoryBound(inputs, _crit);
  }

  LOG_STATUS(
    "Features counted in " << StringUtils::millisecondsToDhms(timer.elapsed()) << " total.");

  return _total;
}

void ElementCounter::_checkForMissingInputs(const QStringList& inputs) const
{
  LOG_VART(inputs.size());
  for (int i = 0; i < inputs.size(); i++)
  {
    const QString input = inputs.at(i);
    if (!DbUtils::isDbUrl(input))
    {
      QFileInfo fileInfo(input);
      if (!fileInfo.exists())
      {
        throw IllegalArgumentException("Input file does not exist: " + input);
      }
    }
  }
}

QString ElementCounter::_getStatusMessage(const QString& input, const QString& critStr) const
{
  const QString dataType = _countFeaturesOnly ? "features" : "elements";
  QString msg = "Counting " + dataType;
  if (!critStr.isEmpty())
  {
    msg += " satisfying " + critStr;
  }
  msg += " from ..." + FileUtils::toLogFormat(input, 25) + "...";
  return msg;
}

QString ElementCounter::_getStatusMessage(const int inputsSize, const QString& critStr) const
{
  const QString dataType = _countFeaturesOnly ? "features" : "elements";
  QString msg = "Counting " + dataType;
  if (!critStr.isEmpty())
  {
    msg += " satisfying " + critStr;
  }
  msg += " from " + QString::number(inputsSize) + " input(s)...";
  return msg;
}

std::shared_ptr<PartialOsmMapReader> ElementCounter::_getStreamingReader(const QString& input) const
{
  std::shared_ptr<PartialOsmMapReader> reader =
    std::dynamic_pointer_cast<PartialOsmMapReader>(
      OsmMapReaderFactory::createReader(input));
  reader->setUseDataSourceIds(true);
  reader->open(input);
  reader->initializePartial();
  return reader;
}

ElementInputStreamPtr ElementCounter::_getFilteredInputStream(
  ElementInputStreamPtr inputStream, const ElementCriterionPtr& criterion,
  ConstElementVisitorPtr countVis) const
{
  LOG_TRACE("Getting filtered input stream...");

  if (criterion)
  {
    LOG_VARD(criterion->toString());
  }

  ElementInputStreamPtr filteredInputStream;
  if (criterion)
  {
    filteredInputStream.reset(
      new ElementCriterionVisitorInputStream(inputStream, criterion, countVis));
  }
  else
  {
    filteredInputStream.reset(new ElementVisitorInputStream(inputStream, countVis));
  }

  return filteredInputStream;
}

ConstElementVisitorPtr ElementCounter::_getCountVis() const
{
  LOG_TRACE("Getting count vis...");

  ConstElementVisitorPtr countVis;
  LOG_VART(_countFeaturesOnly);
  if (_countFeaturesOnly)
  {
    countVis.reset(new FeatureCountVisitor());
  }
  else
  {
    countVis.reset(new ElementCountVisitor());
  }
  return countVis;
}

long ElementCounter::_countMemoryBound(
  const QStringList& inputs, const ElementCriterionPtr& criterion) const
{
  LOG_DEBUG("Counting memory bound...");

  OsmMapPtr map = std::make_shared<OsmMap>();
  // Don't read in file IDs or duplicated elements across inputs won't be counted.
  IoUtils::loadMaps(map, inputs, false);

  OsmMapConsumer* omc = dynamic_cast<OsmMapConsumer*>(criterion.get());
  if (omc)
  {
    omc->setOsmMap(map.get());
  }

  ConstElementVisitorPtr countVis = _getCountVis();
  ConstElementVisitorPtr vis;
  if (criterion)
  {
    vis.reset(new FilteredVisitor(criterion, countVis));
  }
  else
  {
    vis = countVis;
  }
  map->visitRo(*vis);

  std::shared_ptr<SingleStatistic> counter =
    std::dynamic_pointer_cast<SingleStatistic>(countVis);
  return (long)counter->getStat();
}

long ElementCounter::_countStreaming(
  const QString& input, const ElementCriterionPtr& criterion) const
{
  LOG_DEBUG("Counting streaming...");

  long inputTotal = 0;

  std::shared_ptr<PartialOsmMapReader> reader = _getStreamingReader(input);

  ConstElementVisitorPtr countVis = _getCountVis();
  ElementInputStreamPtr filteredInputStream =
    _getFilteredInputStream(
      std::dynamic_pointer_cast<ElementInputStream>(reader), criterion, countVis);
  std::shared_ptr<SingleStatistic> counter =
    std::dynamic_pointer_cast<SingleStatistic>(countVis);
  LOG_VART(counter.get());

  while (filteredInputStream->hasMoreElements())
  {
    ConstElementPtr element = filteredInputStream->readNextElement();
    LOG_VART(element->getElementId());
    inputTotal = (int)counter->getStat();

    // It would be nice if we could display information on the total elements processed, as well
    // as the ones counted, but since the element stream is already filtered by this point that
    // would take some extra work.
    const long runningTotal = _total + inputTotal;
    if (runningTotal > 0 && runningTotal % (_taskStatusUpdateInterval * 10) == 0)
    {
      QString msg = "Counted " + StringUtils::formatLargeNumber(runningTotal);
      if (_countFeaturesOnly)
      {
        msg += " features";
      }
      else
      {
        msg += " elements";
      }
      msg += " total.";
      // TODO: We could do a sliding interval here, like we do for poi/poly match counting. Would
      // help give better status for datasets with sparser number of features satisfying the crit.
      PROGRESS_STATUS(msg);
    }
  }
  LOG_VART(inputTotal);

  reader->finalizePartial();
  reader->close();
  filteredInputStream->close();

  return inputTotal;
}

}
