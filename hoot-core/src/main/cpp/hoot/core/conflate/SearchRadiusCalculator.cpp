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

#include "SearchRadiusCalculator.h"

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/conflate/MapCleaner.h>
#include <hoot/core/conflate/RubberSheet.h>
#include <hoot/core/visitors/RemoveElementsVisitor.h>
#include <hoot/core/filters/StatusCriterion.h>
#include <hoot/core/filters/ChainCriterion.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/OsmMap.h>

using namespace std;

//TODO: a lot of the logging levels in this class need changed, as they've revealed issues
//with the element status reading...will handle that in #1262
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
  if (config.getUnifyPreOps().contains("hoot::RubberSheet"))
  {
    throw HootException(
      "Rubber sheeting cannot be used when automatically calculating search radius.");
  }
  setCircularError(config.getCircularErrorDefaultValue());
  setRubberSheetRef(config.getRubberSheetRef());
  setRubberSheetMinTies(config.getRubberSheetMinimumTies());
  setPrecision(config.getWriterPrecision());
}

void SearchRadiusCalculator::apply(boost::shared_ptr<OsmMap> &map)
{
  //make a copy of the map with previously conflated data removed, as the rubber sheeting can't
  //use it
  boost::shared_ptr<OsmMap> mapWithOnlyUnknown1And2(new OsmMap(map));
  RemoveElementsVisitor elementRemover1(ElementCriterionPtr(new StatusCriterion(Status::Conflated)));
  elementRemover1.setRecursive(true);
  mapWithOnlyUnknown1And2->visitRw(elementRemover1);
  RemoveElementsVisitor elementRemover2(ElementCriterionPtr(new StatusCriterion(Status::Invalid)));
  elementRemover2.setRecursive(true);
  mapWithOnlyUnknown1And2->visitRw(elementRemover2);
  if (map->getElementCount() > mapWithOnlyUnknown1And2->getElementCount())
  {
    //should this be a warning?
    LOG_DEBUG(
      "Skipping " << map->getElementCount() - mapWithOnlyUnknown1And2->getElementCount() <<
      " features with conflated or invalid status out of " << map->getElementCount() <<
      " total features.");
  }
  if (mapWithOnlyUnknown1And2->getElementCount() == 0)
  {
    _result = _circularError;
    LOG_INFO(
      "Unable to automatically calculate search radius.  All features have already been " <<
      "conflated or have an invalid status.\n Using default search radius value = " <<
      QString::number(_result));
    return;
  }

  boost::shared_ptr<RubberSheet> rubberSheet(new RubberSheet(false));
  rubberSheet->setReference(_rubberSheetRef);
  rubberSheet->setMinimumTies(_minTies);
  try
  {
    rubberSheet->calculateTransform(mapWithOnlyUnknown1And2);
  }
  catch (const HootException& e)
  {
    //In many cases, the input map will have already been cleaned by this point...but possibly not
    //(direct call to the stats command, for example).  Try to clean it to get around this error.
    LOG_DEBUG("Rubber sheeting error: " << e.getWhat());
    LOG_DEBUG(
      "An error occurred calculating the rubber sheet transform during automatic search radius " <<
      "calculation.  Cleaning the data and attempting to calculate the transform again...");
    try
    {
      MapCleaner().apply(mapWithOnlyUnknown1And2);
      rubberSheet.reset(new RubberSheet(false));
      rubberSheet->setReference(_rubberSheetRef);
      rubberSheet->setMinimumTies(_minTies);
      rubberSheet->calculateTransform(mapWithOnlyUnknown1And2);
    }
    catch (const HootException& e)
    {
      _result = _circularError;
      LOG_DEBUG(e.getWhat());
      LOG_INFO(
        QString("Unable to automatically calculate search radius: \n") +
        QString("Using default search radius value = ") + QString::number(_result));
      return;
    }
  }

  vector<double> tiePointDistances;
  try
  {
    tiePointDistances = rubberSheet->calculateTiePointDistances();
  }
  catch (const HootException&)
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
    LOG_INFO("Calculated search radius = " + QString::number(_result, 'g', _precision));
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
