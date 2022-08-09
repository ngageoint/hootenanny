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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#include "TagCountVisitor.h"

#include <hoot/core/util/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, TagCountVisitor)

TagCountVisitor::TagCountVisitor() :
_totalCount(0),
_smallestCount(0),
_largestCount(0),
_numInformationAffected(0),
_informationCount(0),
_smallestInformationCount(0),
_largestInformationCount(0)
{
}

void TagCountVisitor::visit(const ConstElementPtr& e)
{
  const int nonDebugCount = e->getTags().getNonDebugCount();
  if (nonDebugCount > 0)
  {
    _numAffected++;
  }
  _totalCount += nonDebugCount;
  if (_smallestCount == 0 || nonDebugCount < _smallestCount)
  {
    _smallestCount = nonDebugCount;
  }
  if (nonDebugCount > _largestCount)
  {
    _largestCount = nonDebugCount;
  }

  const int informationCount = e->getTags().getInformationCount();
  if (informationCount > 0)
  {
    _numInformationAffected++;
  }
  _informationCount += informationCount;
  if (_smallestInformationCount == 0 || informationCount < _smallestInformationCount)
  {
    _smallestInformationCount = informationCount;
  }
  if (informationCount > _largestInformationCount)
  {
    _largestInformationCount = informationCount;
  }
}

}
