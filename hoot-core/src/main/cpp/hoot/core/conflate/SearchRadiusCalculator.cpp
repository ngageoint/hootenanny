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

#include "SearchRadiusCalculator.h"

// Hoot
#include <hoot/core/Factory.h>
#include <hoot/core/conflate/MapCleaner.h>
#include <hoot/core/conflate/RubberSheet.h>
#include <hoot/core/visitors/RemoveElementsVisitor.h>
#include <hoot/core/filters/StatusCriterion.h>
#include <hoot/core/filters/ChainCriterion.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, SearchRadiusCalculator)

SearchRadiusCalculator::SearchRadiusCalculator()
{
  setConfiguration(conf());
}

void SearchRadiusCalculator::setConfiguration(const Settings& conf)
{
  ConfigOptions config(conf);
  QString unifyPreOpsStr = config.getUnifyPreOps();
  if (unifyPreOpsStr.contains(";"))
  {
    setUnifyPreOps(unifyPreOpsStr.split(";"));
  }
  else
  {
    setUnifyPreOps(QStringList(unifyPreOpsStr));
  }
  setCircularError(config.getCircularErrorDefaultValue());
  setRubberSheetRef(config.getRubberSheetRef());
  setRubberSheetMinTies(config.getRubberSheetMinimumTies());
}

void SearchRadiusCalculator::setUnifyPreOps(QStringList ops)
{
  if (ops.contains("hoot::RubberSheet"))
  {
    throw HootException(
      "Rubber sheeting cannot be used when automatically calculating search radius.");
  }
}

void SearchRadiusCalculator::apply(shared_ptr<OsmMap>& map)
{
  //make a copy of the map with previously conflated data removed, as the rubber sheeting can't
  //use it
  OsmMapPtr mapWithOnlyUnknown1And2(new OsmMap(map));
  RemoveElementsVisitor elementRemover1(ElementCriterionPtr(new StatusCriterion(Status::Conflated)));
  elementRemover1.setRecursive(true);
  mapWithOnlyUnknown1And2->visitRw(elementRemover1);
  RemoveElementsVisitor elementRemover2(ElementCriterionPtr(new StatusCriterion(Status::Invalid)));
  elementRemover2.setRecursive(true);
  mapWithOnlyUnknown1And2->visitRw(elementRemover2);
  if (map->getElementCount() > mapWithOnlyUnknown1And2->getElementCount())
  {
    LOG_INFO(
      "Skipping " << map->getElementCount() - mapWithOnlyUnknown1And2->getElementCount() <<
      " conflated or invalid features out of " << map->getElementCount() << " total features.");
  }
  if (mapWithOnlyUnknown1And2->getElementCount() == 0)
  {
    _result = _circularError;
    LOG_INFO(
      "Unable to automatically calculate search radius.  All features have already been " <<
      "conflated or are invalid.\n Using default search radius value = " << QString::number(_result));
    return;
  }

  shared_ptr<RubberSheet> rubberSheet(new RubberSheet());
  rubberSheet->setReference(_rubberSheetRef);
  rubberSheet->setMinimumTies(_minTies);
  try
  {
    rubberSheet->calculateTransform(mapWithOnlyUnknown1And2);
  }
  catch (const HootException& e)
  {
    //In many cases, the input map will have already been cleaned by this point...but possibly not.
    //Try to clean it to get around this error (call to the stats command, for example).
    LOG_INFO(
      "An error occurred calculating the rubber sheet transform during automatic search radius " <<
      "calculation.  Cleaning the data and attempting to calculate the transform again...");
    LOG_DEBUG(e.getWhat());
    try
    {
      MapCleaner().apply(mapWithOnlyUnknown1And2);
      rubberSheet.reset(new RubberSheet());
      rubberSheet->setReference(_rubberSheetRef);
      rubberSheet->setMinimumTies(_minTies);
      rubberSheet->calculateTransform(mapWithOnlyUnknown1And2);
    }
    catch (const HootException& e)
    {
      _result = _circularError;
      LOG_INFO(
        QString("Unable to automatically calculate search radius: ") + e.getWhat() + QString("\n") +
        QString("Using default search radius value = ") + QString::number(_result));
      return;
    }
  }

  vector<double> tiePointDistances;
  try
  {
    tiePointDistances = rubberSheet->calculateTiePointDistances();
  }
  catch (const HootException& /*e*/)
  {
    //unrecoverable error...we'll end up using the default search distance instead
  }

  _calculateSearchRadius(tiePointDistances);
}

void SearchRadiusCalculator::_calculateSearchRadius(const vector<double>& tiePointDistances)
{
  if (tiePointDistances.size() < 2)
  {
    _result = _circularError;
    LOG_INFO(
      QString("Unable to automatically calculate search radius.  Not enough tie points.  ") +
      QString("Using default search radius value = ") + QString::number(_result));
  }
  else
  {
    _result = 2 * _calculateStandardDeviation(tiePointDistances);
    LOG_INFO("Calculated search radius = " + QString::number(_result, 'g', 16));
  }
}

double SearchRadiusCalculator::_calculateStandardDeviation(const vector<double>& samples)
{
  double sumSquares = 0.0;
  for (size_t i = 0; i < samples.size(); i++)
  {
    const double v = samples[i];
    sumSquares += v * v;
  }
  return sqrt(sumSquares / (double)(samples.size() - 1));
}

}
