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
#ifndef SUBLINE_STRING_MATCHER_FACTORY_H
#define SUBLINE_STRING_MATCHER_FACTORY_H

// hoot
#include <hoot/core/algorithms/subline-matching/SublineStringMatcher.h>
#include <hoot/core/info/CreatorDescription.h>

namespace hoot
{

/**
 * @brief The SublineStringMatcherFactory class a factory for creating subline string matchers.
 */
class SublineStringMatcherFactory
{
public:

  /**
   * @brief getMatcher returns the appropriate subline string matcher given the feature type being
   * conflated
   * @param featureType the type of feature being conflated; if type is Unknown, then the default
   * general way parameters will be used to configure the returned subline string matcher
   * @param map optional map; needed to configure the matcher for some feature types
   * @return an initialized subline string matcher
   * @throws IllegalArgumentException if a map was needed for configuration and none was passed in
   */
  static SublineStringMatcherPtr getMatcher(const CreatorDescription::BaseFeatureType& featureType = CreatorDescription::BaseFeatureType::Unknown,
                                            const ConstOsmMapPtr& map = ConstOsmMapPtr());

private:

  static SublineStringMatcherPtr _getHighwayMatcher();
  static SublineStringMatcherPtr _getRiverMatcher(const ConstOsmMapPtr& map);
  static SublineStringMatcherPtr _getRailwayMatcher();
  static SublineStringMatcherPtr _getPowerLineMatcher();
  static SublineStringMatcherPtr _getGenericLineMatcher();
  static SublineStringMatcherPtr _getDefaultMatcher();

  static SublineStringMatcherPtr _getMatcher(const QString& sublineStringMatcherName, const QString& sublineMatcherName,
                                             const double maxAngle, const double headingDelta, const int maxRecursions);
};

}

#endif // SUBLINE_STRING_MATCHER_FACTORY_H
