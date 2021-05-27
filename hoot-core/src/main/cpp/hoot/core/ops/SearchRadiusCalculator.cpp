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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#include "SearchRadiusCalculator.h"

// Hoot
#include <hoot/core/algorithms/rubber-sheet/RubberSheet.h>
#include <hoot/core/criterion/ChainCriterion.h>
#include <hoot/core/criterion/OrCriterion.h>
#include <hoot/core/criterion/StatusCriterion.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/ops/CopyMapSubsetOp.h>
#include <hoot/core/ops/MapCleaner.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/StringUtils.h>

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
  setCircularError(config.getCircularErrorDefaultValue());
  setRubberSheetRef(config.getRubberSheetRef());
  setRubberSheetMinTies(config.getRubberSheetMinimumTies());
  setPrecision(config.getWriterPrecision());
  _elementCriterion = config.getSearchRadiusCalculatorElementCriterion().trimmed();
}

void SearchRadiusCalculator::apply(std::shared_ptr<OsmMap>& map)
{
  LOG_VARD(map->getElementCount());

  // We need to filter out features that don't need to be included in the search radius calculation.
  OsmMapPtr filteredMap = _getFilteredMap(map);
  if (filteredMap->getElementCount() == 0)
  {
    // If all the elements were filtered out, use the CE.
    _result = _circularError;
    LOG_INFO(
      "Unable to automatically calculate search radius. All input features have been " <<
      "filtered out. Using default search radius value = " << _result);
    return;
  }

  // determine tie points with rubbersheeting
  const std::vector<double> tiePointDistances = _getTiePointDistances(filteredMap);
  if (tiePointDistances.empty())
  {
    // no tie points found, so use CE
    _result = _circularError;
    LOG_INFO(
      "Unable to automatically calculate search radius. Using default search radius value = " <<
      _result);
    return;
  }

  // calc the optimium search radius based off of the tie points
  _calculateSearchRadius(tiePointDistances);
}

OsmMapPtr SearchRadiusCalculator::_getFilteredMap(const ConstOsmMapPtr& map) const
{
  OsmMapPtr filteredMap(new OsmMap());

  // don't care about conflated data and invalid data, so filter them out always
  ElementCriterionPtr crit;
  ElementCriterionPtr unknownCrit(
    new OrCriterion(
      ElementCriterionPtr(new StatusCriterion(Status::Unknown1)),
      ElementCriterionPtr(new StatusCriterion(Status::Unknown2))));
  if (_elementCriterion.isEmpty())
  {
    // If no match candidate criterion was specified, then we'll use all remaining elements.
    crit = unknownCrit;
  }
  else
  {
    // If a match candidate criterion was specified, filter out the remaining elements that don't
    // fit the criterion.
    // TODO: This logic doesn't support Generic Conflation calling scripts who implement the
    // isMatchCandidate function. - see #3048
    ElementCriterionPtr candidateCrit(
      Factory::getInstance().constructObject<ElementCriterion>(_elementCriterion));
    crit.reset(new ChainCriterion(unknownCrit, candidateCrit));
  }

  // make a copy of the filtered map to do the search radius calc on
  CopyMapSubsetOp mapCopier(map, crit);
  mapCopier.apply(filteredMap);
  LOG_DEBUG(
    "Element count after search radius calculation filtering: " <<
    StringUtils::formatLargeNumber(filteredMap->getElementCount()));

  return filteredMap;
}

std::vector<double> SearchRadiusCalculator::_getTiePointDistances(OsmMapPtr& map) const
{
  std::vector<double> tiePointDistances;
  //  First check if there is a cached RubberSheet before creating a new one
  std::shared_ptr<RubberSheet> rubberSheet = map->getCachedRubberSheet();
  if (!rubberSheet)
  {
    rubberSheet.reset(new RubberSheet());
    rubberSheet->setReference(_rubberSheetRef);
    rubberSheet->setMinimumTies(_minTies);
    rubberSheet->setFailWhenMinimumTiePointsNotFound(false);
    rubberSheet->setLogWarningWhenRequirementsNotFound(false);
    try
    {
      rubberSheet->calculateTransform(map);
    }
    catch (const HootException& e)
    {
      // In many cases, the input map will have already been cleaned by this point...but possibly not
      // (direct call to the stats command, for example). So, try to clean it and re-run to get around
      // this error.
      LOG_DEBUG("Rubber sheeting error: " << e.getWhat());
      LOG_DEBUG(
        "An error occurred calculating the rubber sheet transform during automatic search radius " <<
        "calculation.  Cleaning the data and attempting to calculate the transform again...");
      try
      {
        MapCleaner().apply(map);

        rubberSheet.reset(new RubberSheet());
        rubberSheet->setReference(_rubberSheetRef);
        rubberSheet->setMinimumTies(_minTies);
        rubberSheet->setFailWhenMinimumTiePointsNotFound(false);
        rubberSheet->setLogWarningWhenRequirementsNotFound(false);
        rubberSheet->calculateTransform(map);
      }
      catch (const HootException&)
      {
        // after the second try, we give up
        return tiePointDistances;
      }
    }
  }

  try
  {
    tiePointDistances = rubberSheet->calculateTiePointDistances();
  }
  catch (const HootException&)
  {
    // unrecoverable error...we'll end up using the default search distance instead
  }
  return tiePointDistances;
}

void SearchRadiusCalculator::_calculateSearchRadius(const std::vector<double>& tiePointDistances)
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
    LOG_DEBUG("Calculated search radius = " + QString::number(_result, 'g', 2));
  }
}

double SearchRadiusCalculator::_calculateStandardDeviation(const std::vector<double>& samples) const
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
