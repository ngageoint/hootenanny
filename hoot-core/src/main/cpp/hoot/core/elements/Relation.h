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
#ifndef RELATION_H
#define RELATION_H

// hoot
#include <hoot/core/elements/Element.h>
#include <hoot/core/elements/RelationData.h>
#include <hoot/core/schema/MetadataTags.h>

// Standard
#include <memory>

namespace hoot
{

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

  static std::string className() { return "hoot::Relation"; }

  static int logWarnCount;

  explicit Relation(const Relation& from);

  Relation(Status s, long id, Meters circularError = ElementData::CIRCULAR_ERROR_EMPTY,
           QString type = "", long changeset = ElementData::CHANGESET_EMPTY,
           long version = ElementData::VERSION_EMPTY,
           quint64 timestamp = ElementData::TIMESTAMP_EMPTY,
           QString user = ElementData::USER_EMPTY, long uid = ElementData::UID_EMPTY,
           bool visible = ElementData::VISIBLE_EMPTY);

  virtual ~Relation() {}

  void addElement(const QString& role, const std::shared_ptr<const Element>& e);
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

  /**
   * Finds the index of a member
   *
   * @param eid ID of the relation member
   * @return a numerical index
   */
  size_t indexOf(ElementId eid) const;

  /**
   * Inserts a relation member
   *
   * @param role role of the member
   * @param elementId ID of the member
   * @param pos position in the relation to insert the member
   */
  void insertElement(const QString& role, const ElementId& elementId, size_t pos);

  /**
   * Returns the number of member elements with the given relation role
   *
   * @param role role by which to examine elements
   * @return the number of member elements with the specified role
   */
  int numElementsByRole(const QString& role);

  /**
   * Retrieves all members with a particular role
   *
   * @param role role to search for
   * @return a collection of members
   */
  const std::vector<RelationData::Entry> getElementsByRole(const QString& role);

  const std::vector<RelationData::Entry>& getMembers() const
  { return _relationData->getElements(); }

  /**
   * Returns the IDs of members
   *
   * @param elementType optional element type of element Ids to return
   * @return a collection of element IDs
   */
  std::set<ElementId> getMemberIds(const ElementType& elementType = ElementType::Unknown) const;

  virtual geos::geom::Envelope* getEnvelope(
    const std::shared_ptr<const ElementProvider>& ep) const override;
  virtual const geos::geom::Envelope& getEnvelopeInternal(
    const std::shared_ptr<const ElementProvider>& ep) const override;

  virtual ElementType getElementType() const { return ElementType(ElementType::Relation); }

  QString getType() const { return _relationData->getType(); }

  /**
   * Returns true if this is a multipolygon type. No checking is done to determine if the geometry
   * is valid.
   */
  bool isMultiPolygon() const
  { return _relationData->getType() == MetadataTags::RelationMultiPolygon(); }

  /**
   * Returns true if this is a review.
   */
  bool isReview() const { return _relationData->getType() == MetadataTags::RelationReview(); }

  bool isRestriction() const
  { return _relationData->getType() == MetadataTags::RelationRestriction(); }

  /**
   * Remove all members that meet the specified criteria. If no members meet the criteria then
   * no changes are made.
   */
  void removeElement(const QString& role, const std::shared_ptr<const Element>& e);
  void removeElement(const QString& role, ElementId eid);
  void removeElement(ElementId eid);

  /**
   * Replaces all instances of from in the relation with to. If from is not in the relation then
   * no changes are made.
   */
  void replaceElement(const std::shared_ptr<const Element>& from,
                      const std::shared_ptr<const Element>& to);
  void replaceElement(const ConstElementPtr& from, const QList<ElementPtr>& to);
  void replaceElement(const ElementId& from, const ElementId& to);

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

  void setMembers(const std::vector<RelationData::Entry>& members);

  /**
   * Sets the "type" of the relation. See the OSM wiki [1] for a detailed description. Example
   * types include "building", "multipolygon" and "multilinestring".
   *
   * 1. http://wiki.openstreetmap.org/wiki/Relation
   */
  void setType(const QString& type);

  QString toString() const;

  virtual void visitRo(const ElementProvider& map, ConstElementVisitor& filter) const;
  virtual void visitRw(ElementProvider& map, ConstElementVisitor& filter);

private:

  std::shared_ptr<RelationData> _relationData;

  virtual ElementData& _getElementData() { _makeWritable(); return *_relationData; }
  virtual const ElementData& _getElementData() const { return *_relationData; }

  void _makeWritable();

  void _visitRo(const ElementProvider& map, ConstElementVisitor& filter,
    QList<long>& visitedRelations) const;
  void _visitRw(ElementProvider& map, ConstElementVisitor& filter,
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

typedef std::shared_ptr<Relation> RelationPtr;
typedef std::shared_ptr<const Relation> ConstRelationPtr;

}

#endif // RELATION_H
