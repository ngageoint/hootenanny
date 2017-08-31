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
 * @copyright Copyright (C) 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "ElementCriterionVisitorInputStream.h"

#include <hoot/core/util/Log.h>
#include <hoot/core/util/MetadataTags.h>

namespace hoot
{

ElementCriterionVisitorInputStream::ElementCriterionVisitorInputStream(
  const ElementInputStreamPtr& elementSource, const ElementCriterionPtr& criterion,
  const QList<ElementVisitorPtr>& visitors) :
_elementSource(elementSource),
_criterion(criterion),
_visitors(visitors)
{
}

boost::shared_ptr<OGRSpatialReference> ElementCriterionVisitorInputStream::getProjection() const
{
  return _elementSource->getProjection();
}

ElementPtr ElementCriterionVisitorInputStream::readNextElement()
{
  do
  {
    ElementPtr e = _elementSource->readNextElement();
    LOG_VART(e->getElementId());

    LOG_VART(_criterion.get());
    if (!_criterion.get() || _criterion->isSatisfied(e))
    {
      for (QList<ElementVisitorPtr>::const_iterator itr = _visitors.begin();
           itr != _visitors.end(); ++itr)
      {
        ElementVisitorPtr visitor = *itr;
        LOG_VART(visitor->toString());
        visitor->visit(e);
        LOG_VART(e->getTags().contains(MetadataTags::HootHash()));
      }
      return e;
    }
    else
    {
      LOG_TRACE("Criterion not satisfied.");
    }
  } while (hasMoreElements());

  return ElementPtr();
}

}
