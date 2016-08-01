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
#ifndef RELATION_H
#define RELATION_H

#include <tgs/SharedPtr.h>

#include "Element.h"
#include "RelationData.h"

namespace geos
{
namespace geom
{
class Geometry;
class LinearRing;
}
}

namespace hoot
{
using namespace geos::geom;

class Way;

/**
 * This is not a complete implementation of a Relation, but it should be enough to do basic multi-
 * polygons. See the OSM wiki [1] for a discussion on relations and how they're used in OSM.
 *
 * 1. http://wiki.openstreetmap.org/wiki/Relation
 *
 * @sa MultiLineStringVisitor
 */
class Relation : public Element
{
public:
  static QString INNER;
  static QString MULTILINESTRING;
  static QString MULTIPOLYGON;
  static QString OUTER;
  static QString REVIEW;

  static string className() { return "hoot::Relation"; }

  explicit Relation(const Relation& from);

  Relation(Status s, long id, Meters circularError, QString type = "",
           long changeset = ElementData::CHANGESET_EMPTY,
           long version = ElementData::VERSION_EMPTY,
           unsigned int timestamp = ElementData::TIMESTAMP_EMPTY,
           QString user = ElementData::USER_EMPTY, long uid = ElementData::UID_EMPTY,
           bool visible = ElementData::VISIBLE_EMPTY);

  virtual ~Relation() {}

  void addElement(const QString& role, const shared_ptr<const Element>& e);
  void addElement(const QString& role, ElementType t, long id);
  void addElement(const QString& role, ElementId);

  /**
   * Removes members, tags, type and circularError.
   */
  virtual void clear();

  Element* clone() const { return new Relation(*this); }

  /**
   * Returns true if this relation contains the specified ElementId. This does not recursively
   * search for the element.
   */
  bool contains(ElementId eid) const;

  const vector<RelationData::Entry>& getMembers() const { return _relationData->getElements(); }

  virtual Envelope* getEnvelope(const shared_ptr<const ElementProvider>& ep) const;

  virtual ElementType getElementType() const { return ElementType::Relation; }

  QString getType() const { return _relationData->getType(); }

  /**
   * Returns true if this is a multipolygon type. No checking is done to determine if the geometry
   * is valid.
   */
  bool isMultiPolygon() const { return _relationData->getType() == MULTIPOLYGON; }

  /**
   * Returns true if this is a review.
   */
  bool isReview() const { return _relationData->getType() == REVIEW; }


  /**
   * Remove all members that meet the speicified criteria. If no members meet the criteria then
   * no changes are made.
   */
  void removeElement(const QString& role, const shared_ptr<const Element>& e);
  void removeElement(const QString& role, ElementId eid);
  void removeElement(ElementId eid);

  /**
   * Replaces all instances of from in the relation with to. If from is not in the relation then
   * no changes are made.
   */
  void replaceElement(const shared_ptr<const Element>& from, const shared_ptr<const Element>& to);
  void replaceElement(const ConstElementPtr& from, const QList<ElementPtr>& to);

  /**
   * Replaces all instances of old with the values in the collection defined by start/end. Order is
   * maintained s/t old entries are replaced by new values in order and the order of the existing
   * members are maintained. This can be slow in some cases.
   *
   * This may not be what you want. Think long and hard to make sure the roles and relation order
   * will be maintained properly.
   *
   * @param start Iterator start position pointing to a RelationData::Entry
   * @param end Iterator end position pointing to a RelationData::Entry
   */
  template<typename IT>
  void replaceElements(RelationData::Entry old, IT start, IT end);

  void setMembers(const vector<RelationData::Entry>& members);

  /**
   * Sets the "type" of the relation. See the OSM wiki [1] for a detailed description. Example
   * types include "building", "multipolygon" and "multilinestring".
   *
   * 1. http://wiki.openstreetmap.org/wiki/Relation
   */
  void setType(const QString& type);

  QString toString() const;

  virtual void visitRo(const ElementProvider& map, ElementVisitor& filter) const;

  virtual void visitRw(ElementProvider& map, ElementVisitor& filter);

private:

  boost::shared_ptr<RelationData> _relationData;

  virtual ElementData& _getElementData() { _makeWritable(); return *_relationData; }

  virtual const ElementData& _getElementData() const { return *_relationData; }

  void _makeWritable();

  void _visitRo(const ElementProvider& map, ElementVisitor& filter,
    QList<long>& visitedRelations) const;

  void _visitRw(ElementProvider &map, ElementVisitor& filter,
    QList<long> &visitedRelations);

};

template<typename IT>
void Relation::replaceElements(RelationData::Entry old, IT start, IT end)
{
  _preGeometryChange();
  _makeWritable();
  _relationData->replaceElements(old, start, end);
  _postGeometryChange();
}

typedef boost::shared_ptr<Relation> RelationPtr;
typedef boost::shared_ptr<const Relation> ConstRelationPtr;
typedef const boost::shared_ptr<const Relation>& ConstRelationPtrR;

}

#endif // RELATION_H
