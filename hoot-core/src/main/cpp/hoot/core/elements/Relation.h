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

  static QString className() { return "hoot::Relation"; }

  Relation(
    Status s, long id, Meters circularError = ElementData::CIRCULAR_ERROR_EMPTY, QString type = "",
    long changeset = ElementData::CHANGESET_EMPTY, long version = ElementData::VERSION_EMPTY,
    quint64 timestamp = ElementData::TIMESTAMP_EMPTY, QString user = ElementData::USER_EMPTY,
    long uid = ElementData::UID_EMPTY, bool visible = ElementData::VISIBLE_EMPTY);
  explicit Relation(const Relation& from);
  ~Relation() = default;

  Element* clone() const override { return new Relation(*this); }

  void addElement(const QString& role, const std::shared_ptr<const Element>& e);
  void addElement(const QString& role, ElementType t, long id);
  void addElement(const QString& role, ElementId);
  /**
   * Inserts a relation member
   *
   * @param role role of the member
   * @param elementId ID of the member
   * @param pos position in the relation to insert the member
   */
  void insertElement(const QString& role, const ElementId& elementId, size_t pos);

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
  void replaceElement(
    const std::shared_ptr<const Element>& from, const std::shared_ptr<const Element>& to);
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

  const std::vector<RelationData::Entry>& getMembers() const
  { return _relationData->getElements(); }
  size_t getMemberCount() const { return _relationData->getElements().size(); }
  void setMembers(const std::vector<RelationData::Entry>& members);

  /**
   * Removes members, tags, type and circularError.
   */
  void clear() override;

  /**
   * Returns true if this relation contains the specified ElementId. This does not recursively
   * search for the element.
   */
  bool contains(const ElementId& eid) const;
  /**
   * Finds the index of a member
   *
   * @param eid ID of the relation member
   * @return a numerical index
   */
  size_t indexOf(const ElementId& eid) const;
  /**
   * Retrieves the relation member element at a specified index
   *
   * @param index the index to retrieve the element member from
   * @return a valid element, if found; an invalid element otherwise
   */
  ElementId memberIdAt(const size_t index) const;
  /**
   * Determines if the first relation member has a specified ID
   *
   * @param eid the element ID to search for
   * @return true if an element having the specified ID is contained at the first relation member
   * location; false otherwise
   */
  bool isFirstMember(const ElementId& eid) const;
  /**
   * Determines if the last relation member has a specified ID
   *
   * @param eid the element ID to search for
   * @return true if an element having the specified ID is contained at the last relation member
   * location; false otherwise
   */
  bool isLastMember(const ElementId& eid) const;
  /**
   * Returns the IDs of members
   *
   * @param elementType optional element type of element Ids to return
   * @return a collection of element IDs
   */
  std::set<ElementId> getMemberIds(const ElementType& elementType = ElementType::Unknown) const;
  /**
   * Retrieves the member element IDs for members placed immediately before and after the member
   * element with the specified ID
   *
   * @param memberId the ID of the member element to retrieve adjoining member element IDs for
   * @return If a member with the specified ID exists 1) and is neither the first nor last member, a
   * list with two elements IDs where the first ID is the ID of the member element directly
   * preceding the element with the specified ID and the second ID is the ID of the member directly
   * succeeding the element with the specified ID. 2) and is the first member, a list with one
   * element ID where the ID is the ID of the member directly succeeding the element with the
   * specified ID. 3) and is the last member, a list with one element ID where the ID is the ID of
   * the member directly preceding the element with the specified ID. If the relation contains no
   * member with the specified ID, then an empty list is returned.
   */
  QList<ElementId> getAdjoiningMemberIds(const ElementId& memberId) const;

  /**
   * Returns the number of member elements with the given relation role
   *
   * @param role role by which to examine elements
   * @return the number of member elements with the specified role
   */
  int numElementsByRole(const QString& role) const;
  /**
   * Retrieves all members with a particular role
   *
   * @param role role to search for
   * @return a collection of members
   */
  std::vector<RelationData::Entry> getElementsByRole(const QString& role) const;

  geos::geom::Envelope* getEnvelope(
    const std::shared_ptr<const ElementProvider>& ep) const override;
  const geos::geom::Envelope& getEnvelopeInternal(
    const std::shared_ptr<const ElementProvider>& ep) const override;

  ElementType getElementType() const override { return ElementType(ElementType::Relation); }

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
   * Sets the "type" of the relation. See the OSM wiki [1] for a detailed description. Example
   * types include "building", "multipolygon" and "multilinestring".
   *
   * 1. http://wiki.openstreetmap.org/wiki/Relation
   */
  void setType(const QString& type);

  QString toString() const override;

  /**
   * @see Element
   */
  void visitRo(
    const ElementProvider& map, ConstElementVisitor& filter,
    const bool recursive = true) const override;

  /**
   * @see Element
   */
  void visitRw(
    ElementProvider& map, ConstElementVisitor& filter, const bool recursive = true) override;

private:

  static int logWarnCount;

  std::shared_ptr<RelationData> _relationData;

  ElementData& _getElementData() override { _makeWritable(); return *_relationData; }
  const ElementData& _getElementData() const override { return *_relationData; }

  void _makeWritable();

  void _visitRo(const ElementProvider& map, ConstElementVisitor& filter,
    QList<long>& visitedRelations, const bool recursive = true) const;
  void _visitRw(ElementProvider& map, ConstElementVisitor& filter,
    QList<long>& visitedRelations, const bool recursive = true);
};

template<typename IT>
void Relation::replaceElements(RelationData::Entry old, IT start, IT end)
{
  _preGeometryChange();
  _makeWritable();
  _relationData->replaceElements(old, start, end);
  _postGeometryChange();
}

using RelationPtr = std::shared_ptr<Relation>;
using ConstRelationPtr = std::shared_ptr<const Relation>;

}

#endif // RELATION_H
