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
 * @copyright Copyright (C) 2017, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef SEARCHBOUNDSCALCULATOR_H
#define SEARCHBOUNDSCALCULATOR_H

// hoot
#include <hoot/core/conflate/SearchRadiusProvider.h>
#include <hoot/core/elements/Node.h>

namespace hoot
{

/**
 * @brief The SearchBoundsCalculator class given a POI in WGS84, calculate the search bounds in
 * decimal degrees.
 *
 * Bad things may happen at the anti-meridian and poles.
 */
class SearchBoundsCalculator
{
public:

  SearchBoundsCalculator(const std::shared_ptr<SearchRadiusProvider>& radiusProvider);

  geos::geom::Envelope calculateSearchBounds(const ConstOsmMapPtr& map, const ConstNodePtr& n) const;

private:

  std::shared_ptr<SearchRadiusProvider> _radiusProvider;
};

using SearchBoundsCalculatorPtr = std::shared_ptr<SearchBoundsCalculator>;

}

#endif // SEARCHBOUNDSCALCULATOR_H
