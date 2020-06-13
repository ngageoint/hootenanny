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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef ELEMENT_H
#define ELEMENT_H

#include <hoot/core/elements/ElementData.h>
#include <hoot/core/elements/ElementId.h>
#include <hoot/core/elements/ElementType.h>
#include <hoot/core/elements/Status.h>
#include <hoot/core/elements/Tags.h>

// Hoot
#include <hoot/core/util/Units.h>

// Standard
#include <cassert>
#include <memory>
#include <set>
#include <string>

// Qt
#include <QString>

namespace geos
{
  namespace geom
  {
    class Envelope;
  }
}

namespace hoot
{

class ElementListener;
class ElementProvider;
class ConstElementVisitor;

/**
 * Base class for OSM elements -- node, way and relation.
 */
class Element
{
public:

  static std::string className() { return "hoot::Element"; }

  Element();

  virtual ~Element() = default;

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
  virtual geos::geom::Envelope* getEnvelope(
    const std::shared_ptr<const ElementProvider>& ep) const = 0;
  virtual const geos::geom::Envelope& getEnvelopeInternal(
    const std::shared_ptr<const ElementProvider>& ep) const = 0;

  long getId() const { return _getElementData().getId(); }
  void setId(long id) { _getElementData().setId(id); }

  long getChangeset() const { return _getElementData().getChangeset(); }
  void setChangeset(long changeset) { _getElementData().setChangeset(changeset); }

  Meters getRawCircularError() const { return _getElementData().getRawCircularError(); }

  long getVersion() const { return _getElementData().getVersion(); }
  void setVersion(long version) { _getElementData().setVersion(version); }

  quint64 getTimestamp() const { return _getElementData().getTimestamp(); }
  void setTimestamp(quint64 timestamp) { _getElementData().setTimestamp(timestamp); }

  QString getUser() const { return _getElementData().getUser(); }
  void setUser(QString user) { _getElementData().setUser(user); }

  long getUid() const { return _getElementData().getUid(); }
  void setUid(long uid) { _getElementData().setUid(uid); }

  const Tags& getTags() const { return _getElementData().getTags(); }
  Tags& getTags() { return _getElementData().getTags(); }
  int getTagCount() const { return _getElementData().getTags().size(); }

  bool hasCircularError() const { return _getElementData().hasCircularError(); }
  void setCircularError(Meters circularError) { _getElementData().setCircularError(circularError); }

  void removeTag(QString k) { _getElementData().getTags().remove(k); }
  void setTags(const Tags& tags) { _getElementData().setTags(tags); }
  void setTag(QString k, QString v) { _getElementData().setTag(k, v); }
  void addTags(const Tags& tags) { _getElementData().addTags(tags); }

  /**
   * Compares information tags with another element
   *
   * @param other element to compare this element's tags with
   * @return true if this element has the same information tags as the other element; false
   * otherwise
   */
  bool hasSameNonMetadataTags(const Element& other) const;

  bool getVisible() const { return _getElementData().getVisible(); }
  void setVisible(bool visible) { _getElementData().setVisible(visible); }

  Status getStatus() const { return _status; }
  QString getStatusString() const;

  bool isUnknown() const { return getStatus().isUnknown(); }

  virtual ElementType getElementType() const = 0;

  /**
   * At this point only one listener is supported, but we could support more later if needed.
   */
  void registerListener(ElementListener* l)
  { assert(_listener == 0 || _listener == l); _listener = l; }

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
  virtual void visitRo(const ElementProvider& map, ConstElementVisitor& visitor) const = 0;

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
  virtual void visitRw(ElementProvider& map, ConstElementVisitor& visitor) = 0;

protected:

  Status _status;

  ElementListener* _listener;

  Element(Status s);

  virtual ElementData& _getElementData() = 0;
  virtual const ElementData& _getElementData() const = 0;

  void _postGeometryChange();
  void _preGeometryChange();

  /**
   * This envelope may be cached, but it also may not be exact.
   */
  mutable geos::geom::Envelope _cachedEnvelope;
};

typedef std::shared_ptr<Element> ElementPtr;
typedef std::shared_ptr<const Element> ConstElementPtr;

}

#endif // ELEMENT_H
