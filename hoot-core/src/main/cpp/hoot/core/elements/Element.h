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
#ifndef ELEMENT_H
#define ELEMENT_H

// Hoot
#include <hoot/core/elements/ElementData.h>
#include <hoot/core/elements/ElementId.h>
#include <hoot/core/elements/ElementType.h>
#include <hoot/core/elements/Status.h>
#include <hoot/core/elements/Tags.h>
#include <hoot/core/util/Units.h>

// Standard
#include <cassert>

// GEOS Includes
// Strangely getting compile errors in hoot-test w/o this, even though its in HootCoreStable.h.
#include <geos/geom/Envelope.h>

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

  static QString className() { return "Element"; }

  Element();
  virtual ~Element() = default;

  virtual void clear() = 0;

  virtual std::shared_ptr<Element> clone() const = 0;

  virtual ElementType getElementType() const = 0;

  /**
   * Returns a string description of the object to aid in debugging.
   */
  virtual QString toString() const = 0;

  /**
   * Calculates and returns this element's envelope. Returns a null envelope if this element is
   * empty or any of the child elements are missing. The caller gets ownership of the returned
   * envelope.
   */
  virtual geos::geom::Envelope* getEnvelope(
    const std::shared_ptr<const ElementProvider>& ep) const = 0;
  virtual const geos::geom::Envelope& getEnvelopeInternal(
    const std::shared_ptr<const ElementProvider>& ep) const = 0;

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
   *
   * @param map the map to visit
   * @param visitor the visitor to visit with
   * @param recursive if true, child elements are visited
   */
  virtual void visitRo(
    const ElementProvider& map, ConstElementVisitor& visitor,
    const bool recursive = true) const = 0;

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
   *
   * @param map the map to visit
   * @param visitor the visitor to visit with
   * @param recursive if true, child elements are visited
   */
  virtual void visitRw(
    ElementProvider& map, ConstElementVisitor& visitor, const bool recursive = true) = 0;

  /**
   * At this point only one listener is supported, but we could support more later if needed.
   */
  void registerListener(ElementListener* l)
  { assert(_listener == 0 || _listener == l); _listener = l; }

  /**
   * Compares information tags with another element
   *
   * @param other element to compare this element's tags with
   * @return true if this element has the same information tags as the other element; false
   * otherwise
   */
  bool hasSameNonMetadataTags(const Element& other) const;
  bool hasTag(const QString& key) const { return _getElementData().getTags().contains(key); }
  bool hasTag(const QString& key, const QString& val) const
  { return _getElementData().getTags().get(key) == val; }

  bool hasCircularError() const { return _getElementData().hasCircularError(); }
  bool isUnknown() const { return getStatus().isUnknown(); }

  /**
   * Circular Error is in meters to 2 standard deviations. In other words it is about the 95%
   * confidence interval assuming a normal distribution.
   */
  Meters getCircularError() const { return _getElementData().getCircularError(); }
  ElementId getElementId() const { return ElementId(getElementType(), getId()); }
  long getId() const { return _getElementData().getId(); }
  long getChangeset() const { return _getElementData().getChangeset(); }
  Meters getRawCircularError() const { return _getElementData().getRawCircularError(); }
  long getVersion() const { return _getElementData().getVersion(); }
  quint64 getTimestamp() const { return _getElementData().getTimestamp(); }
  QString getUser() const { return _getElementData().getUser(); }
  long getUid() const { return _getElementData().getUid(); }
  const Tags& getTags() const { return _getElementData().getTags(); }
  Tags& getTags() { return _getElementData().getTags(); }
  QString getTag(const QString& key) const { return _getElementData().getTags().get(key); }
  int getTagCount() const { return _getElementData().getTags().size(); }
  bool getVisible() const { return _getElementData().getVisible(); }
  Status getStatus() const { return _status; }
  QString getStatusString() const;

  /**
   * Set the enumerated status code.
   */
  void setStatus(Status s) { _status = s; }
  void setId(long id) { _getElementData().setId(id); }
  void setVisible(bool visible) { _getElementData().setVisible(visible); }
  void setTags(const Tags& tags) { _getElementData().setTags(tags); }
  void setTag(QString k, QString v) { _getElementData().setTag(k, v); }
  void setCircularError(Meters circularError) { _getElementData().setCircularError(circularError); }
  void setUid(long uid) { _getElementData().setUid(uid); }
  void setUser(QString user) { _getElementData().setUser(user); }
  void setTimestamp(quint64 timestamp) { _getElementData().setTimestamp(timestamp); }
  void setVersion(long version) { _getElementData().setVersion(version); }
  void setChangeset(long changeset) { _getElementData().setChangeset(changeset); }
  void removeTag(QString k) { _getElementData().getTags().remove(k); }
  void addTags(const Tags& tags) { _getElementData().addTags(tags); }

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

using ElementPtr = std::shared_ptr<Element>;
using ConstElementPtr = std::shared_ptr<const Element>;

}

#endif // ELEMENT_H
