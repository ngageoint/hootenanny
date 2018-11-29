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
 * @copyright Copyright (C) 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "ConstElementCriterionVisitorInputStream.h"

#include <hoot/core/util/Log.h>
#include <hoot/core/schema/MetadataTags.h>

namespace hoot
{

ConstElementCriterionVisitorInputStream::ConstElementCriterionVisitorInputStream(
  const ElementInputStreamPtr& elementSource, const ElementCriterionPtr& criterion,
  const ConstElementVisitorPtr& visitor) :
_elementSource(elementSource),
_criterion(criterion),
_numFeaturesTotal(0),
_numFeaturesPassingCriterion(0)
{
  _visitors.append(visitor);
}

ConstElementCriterionVisitorInputStream::ConstElementCriterionVisitorInputStream(
  const ElementInputStreamPtr& elementSource, const ElementCriterionPtr& criterion,
  const QList<ConstElementVisitorPtr>& visitors) :
_elementSource(elementSource),
_criterion(criterion),
_numFeaturesTotal(0),
_numFeaturesPassingCriterion(0),
_visitors(visitors)
{
}

ConstElementCriterionVisitorInputStream::~ConstElementCriterionVisitorInputStream()
{
  close();
}

void ConstElementCriterionVisitorInputStream::close()
{
  _elementSource->close();
  _numFeaturesTotal = 0;
  _numFeaturesPassingCriterion = 0;
}

boost::shared_ptr<OGRSpatialReference> ConstElementCriterionVisitorInputStream::getProjection() const
{
  return _elementSource->getProjection();
}

bool ConstElementCriterionVisitorInputStream::hasMoreElements()
{
  return _elementSource->hasMoreElements();
}

ElementPtr ConstElementCriterionVisitorInputStream::readNextElement()
{
  do
  {
    ElementPtr element = _elementSource->readNextElement();
    _numFeaturesTotal++;
    LOG_VART(element->getElementId());

    if (!_criterion.get() || _criterion->isSatisfied(*element))
    {
      _numFeaturesPassingCriterion++;
      //LOG_VART(_numFeaturesPassingCriterion);
      for (QList<ConstElementVisitorPtr>::const_iterator itr = _visitors.begin();
           itr != _visitors.end(); ++itr)
      {
        ConstElementVisitorPtr visitor = *itr;
        //LOG_VART(visitor->toString());
        visitor->visit(element);
      }
      return element;
    }
    else
    {
      LOG_TRACE("Criterion not satisfied:");
      LOG_VART(element);
    }
  } while (hasMoreElements());

  return ElementPtr();
}

}
