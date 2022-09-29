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
 * @copyright Copyright (C) 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "SublineStringMatcherFactory.h"

// hoot>
#include <hoot/core/algorithms/subline-matching/MaximalSublineStringMatcher.h>
#include <hoot/core/algorithms/subline-matching/MultipleMatcherSublineStringMatcher.h>
#include <hoot/core/conflate/river/RiverMaximalSublineSettingOptimizer.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>

namespace hoot
{

SublineStringMatcherPtr SublineStringMatcherFactory::getMatcher(const CreatorDescription::BaseFeatureType& featureType,
                                                                const ConstOsmMapPtr& map)
{
  switch (featureType)
  {
  case CreatorDescription::BaseFeatureType::Highway:
    return _getHighwayMatcher();
  case CreatorDescription::BaseFeatureType::Line:
    return _getGenericLineMatcher();
  case CreatorDescription::BaseFeatureType::Railway:
    return _getRailwayMatcher();
  case CreatorDescription::BaseFeatureType::PowerLine:
    return _getPowerLineMatcher();
  case CreatorDescription::BaseFeatureType::River:
    return _getRiverMatcher(map);
  case CreatorDescription::BaseFeatureType::Unknown:
    return _getDefaultMatcher();
  default:
    throw IllegalArgumentException("Invalid feature type: " + CreatorDescription::baseFeatureTypeToString(featureType));
  }
}

SublineStringMatcherPtr SublineStringMatcherFactory::_getHighwayMatcher()
{
  ConfigOptions opts;
  return
    _getMatcher(
      MaximalSublineStringMatcher::className(), opts.getHighwaySublineMatcher(),
      opts.getHighwayMatcherMaxAngle(), opts.getHighwayMatcherHeadingDelta(), 1e7);
}

SublineStringMatcherPtr SublineStringMatcherFactory::_getRiverMatcher(const ConstOsmMapPtr& map)
{
  if (!map)
    throw IllegalArgumentException("No map passed to river subline string matcher initialization.");

  ConfigOptions opts;
  int maxRecursions = -1; // default value
  if (opts.getRiverMaximalSublineAutoOptimize())
    maxRecursions = RiverMaximalSublineSettingOptimizer().getFindBestMatchesMaxRecursions(map);
  return
    _getMatcher(
      MaximalSublineStringMatcher::className(), opts.getRiverSublineMatcher(),
      opts.getRiverMatcherMaxAngle(), opts.getRiverMatcherHeadingDelta(), maxRecursions);
}

SublineStringMatcherPtr SublineStringMatcherFactory::_getRailwayMatcher()
{
  ConfigOptions opts;
  return
    _getMatcher(
      MaximalSublineStringMatcher::className(), opts.getRailwaySublineMatcher(),
      opts.getRailwayMatcherMaxAngle(), opts.getRailwayMatcherHeadingDelta(), 1e7);
}

SublineStringMatcherPtr SublineStringMatcherFactory::_getPowerLineMatcher()
{
  ConfigOptions opts;
  return
    _getMatcher(
      MaximalSublineStringMatcher::className(), opts.getPowerLineSublineMatcher(),
      opts.getPowerLineMatcherMaxAngle(), opts.getPowerLineMatcherHeadingDelta(), 1e7);
}

SublineStringMatcherPtr SublineStringMatcherFactory::_getGenericLineMatcher()
{
  ConfigOptions opts;
  return
    _getMatcher(
      MaximalSublineStringMatcher::className(), opts.getGenericLineSublineMatcher(),
      opts.getGenericLineMatcherMaxAngle(), opts.getGenericLineMatcherHeadingDelta(), 1e7);
}

SublineStringMatcherPtr SublineStringMatcherFactory::_getDefaultMatcher()
{
  ConfigOptions opts;
  return
    _getMatcher(
      MaximalSublineStringMatcher::className(), opts.getWaySublineMatcher(),
      opts.getWayMatcherMaxAngle(), opts.getWayMatcherHeadingDelta(), 1e7);
}

SublineStringMatcherPtr SublineStringMatcherFactory::_getMatcher(const QString& sublineStringMatcherName, const QString& sublineMatcherName,
                                                                 const double maxAngle, const double headingDelta, const int maxRecursions)
{
  // Create the primary matcher. This one will always be used.
  std::shared_ptr<SublineStringMatcher> primaryMatcher = Factory::getInstance().constructObject<SublineStringMatcher>(sublineStringMatcherName);
  Settings settings = conf();
  settings.set(ConfigOptions::getWaySublineMatcherKey(), sublineMatcherName);
  settings.set(ConfigOptions::getWayMatcherMaxAngleKey(), maxAngle);
  settings.set(ConfigOptions::getWayMatcherHeadingDeltaKey(), headingDelta);
  settings.set(ConfigOptions::getMaximalSublineMaxRecursionsKey(), maxRecursions);
  primaryMatcher->setConfiguration(settings);

  const QString secondarySublineMatcher = ConfigOptions().getSublineMatcherSecondary();
  if (sublineMatcherName != secondarySublineMatcher)
  {
    // Create a secondary, more performant matcher as a backup. This one may not end up being used.
    std::shared_ptr<SublineStringMatcher> secondaryMatcher(Factory::getInstance().constructObject<SublineStringMatcher>(sublineStringMatcherName));
    Settings secondarySettings = settings;
    secondarySettings.set(ConfigOptions::getWaySublineMatcherKey(), secondarySublineMatcher);
    secondaryMatcher->setConfiguration(secondarySettings);

    // Wrap use of the two matchers with MultipleMatcherSublineStringMatcher. Don't call
    // setConfiguration here, as we've already set a separate configuration on each of the matchers
    // being passed in.
    return std::make_shared<MultipleMatcherSublineStringMatcher>(primaryMatcher, secondaryMatcher);
  }
  else
  {
    // No point in creating a secondary matcher, if the primary matcher specified matches the
    // default secondary matcher.
    return primaryMatcher;
  }
}

}

