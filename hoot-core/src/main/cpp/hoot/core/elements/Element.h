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
#ifndef ELEMENT_H
#define ELEMENT_H

#include "ElementData.h"
#include "ElementId.h"
#include "ElementType.h"
#include "Status.h"
#include "Tags.h"

// Hoot
#include <hoot/core/Units.h>

namespace hoot {
  class OsmMap;
}

// Boost
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

// Standard
#include <set>
#include <string>

// Qt
#include <QString>

namespace geos {
  namespace geom {
    class Envelope;
  }
}

namespace hoot {

using namespace boost;
using namespace geos::geom;

class ElementListener;
class ElementProvider;
class ElementVisitor;

/**
 * Base class for OSM elements -- node, way and relation.
 */
class Element
{
public:

  static string className() { return "hoot::Element"; }

  virtual ~Element() {}

  virtual void clear() = 0;

  virtual Element* clone() const = 0;

  /**
   * Circular Error is in meters to 2 standard deviations. In other words it is about the 95%
   * confidence interval assuming a normal distribution.
   */
  Meters getCircularError() const { return _getElementData().getCircularError(); }

  ElementId getElementId() const { return ElementId(getElementType(), getId()); }

  /**
   * Calculates and returns this element's envelope. Returns a null envelope if this element is
   * empty or any of the child elements are missing. The caller gets ownership of the returned
   * envelope.
   */
  virtual Envelope* getEnvelope(const shared_ptr<const ElementProvider>& ep) const = 0;

  long getId() const { return _getElementData().getId(); }

  void setId(long id) { _getElementData().setId(id); }

  long getChangeset() const { return _getElementData().getChangeset(); }

  void setChangeset(bool changeset) { _getElementData().setChangeset(changeset); }

  Meters getRawCircularError() const { return _getElementData().getRawCircularError(); }

  long getVersion() const { return _getElementData().getVersion(); }

  void setVersion(long version) { _getElementData().setVersion(version); }

  unsigned int getTimestamp() const { return _getElementData().getTimestamp(); }

  QString getUser() const { return _getElementData().getUser(); }

  long getUid() const { return _getElementData().getUid(); }

  const Tags& getTags() const { return _getElementData().getTags(); }

  Tags& getTags() { return _getElementData().getTags(); }

  bool hasCircularError() const { return _getElementData().hasCircularError(); }

  void removeTag(QString k) { _getElementData().getTags().remove(k); }

  void setCircularError(Meters circularError) { _getElementData().setCircularError(circularError); }

  void setTags(const Tags& tags) { _getElementData().setTags(tags); }

  void setTag(QString k, QString v) { _getElementData().setTag(k, v); }

  bool getVisible() const { return _getElementData().getVisible(); }

  void setVisible(bool visible) { _getElementData().setVisible(visible); }

  Status getStatus() const { return _status; }

  QString getStatusString() const;

  bool isUnknown() const { return getStatus().isUnknown(); }

  virtual ElementType getElementType() const = 0;

  /**
   * At this point only one listener is supported, but we could support more later if needed.
   */
  void registerListener(ElementListener* l) { assert(_listener == 0 || _listener == l); _listener = l; }

  /**
   * Set the enumerated status code.
   */
  void setStatus(Status s) { _status = s; }

  /**
   * Returns a string description of the object to aid in debugging.
   */
  virtual QString toString() const = 0;

  /**
   * Applies a read only visitor to this element and all child elements. The visitor will be called
   * at least once for each element in the tree. For instance if the Element is a Way it may be
   * called twice for a Node if that node appears twice in the way.
   *
   * Making changes while visiting will result in undefined behavior.
   *
   * Children that do not appear in the map will not be visited. This may happen during distributed
   * computations.
   *
   * "this" is guaranteed to be visited last.
   */

  virtual void visitRo(const ElementProvider& map, ElementVisitor& visitor) const = 0;


  /**
   * Applies a read write visitor to this element and all child elements. The visitor will be called
   * at least once for each element in the tree. For instance if the Element is a Way it may be
   * called twice for a Node if that node appears twice in the way.
   *
   * The visitor may change the element that is currently being visited or any of its children. The
   * filter should not change any other element. Changing elements while visiting may impact which
   * children are visited. The visiting occurs in a depth first fashion.
   *
   * Due to the read/write fashion this is slower than the read-only equivalent.
   *
   * Children that do not appear in the map will not be visited. This may happen during distributed
   * computations.
   *
   * "this" is guaranteed to be visited last.
   */
  virtual void visitRw(ElementProvider& map, ElementVisitor& visitor) = 0;


protected:

  Status _status;

  ElementListener* _listener;

  Element(Status s);

  virtual ElementData& _getElementData() = 0;

  virtual const ElementData& _getElementData() const = 0;

  void _postGeometryChange();

  void _preGeometryChange();

};

typedef boost::shared_ptr<Element> ElementPtr;
typedef boost::shared_ptr<const Element> ConstElementPtr;

}

#endif // ELEMENT_H
