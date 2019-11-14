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
 * @copyright Copyright (C) 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef OSM_API_CHANGESET_ELEMENT_H
#define OSM_API_CHANGESET_ELEMENT_H

//  Hoot
#include <hoot/core/elements/ElementType.h>

//  Qt
#include <QList>
#include <QMap>
#include <QPair>
#include <QString>
#include <QVector>
#include <QXmlStreamReader>

//  Standard
#include <unordered_set>

namespace hoot
{

//  Forward declaration
class ElementIdToIdMap;

/** Object that matches an XML tag to its XML attributes */
typedef QPair<QString, QXmlStreamAttributes> XmlObject;
typedef std::vector<std::pair<QString, QString>> ElementAttributes;
typedef std::pair<QString, QString> ElementTag;
typedef std::vector<ElementTag> ElementTags;

/** Changeset element abstraction for simplified nodes, ways, and relations */
class ChangesetElement
{
public:
  /**
   * @brief XmlElement constructor
   * @param object XML tag name and attributes
   * @param idMap ID to ID Map for updated IDs
   */
  ChangesetElement(const XmlObject& object, ElementIdToIdMap* idMap);
  /**
   * @brief XmlElement copy constructor
   * @param element XmlElement object to copy
   */
  ChangesetElement(const ChangesetElement& element);
  /**
   * @brief addTag  Add a tag to the element
   * @param tag XML tag with key and value attributes
   */
  void addTag(const XmlObject& tag);
  /**
   * @brief getTagK/V
   * @param index
   * @return
   */
  QString getTagKey(int index);
  QString getTagValue(int index);
  /**
   * @brief getTagCount
   * @return Number of tags in this element
   */
  int getTagCount() { return _tags.size(); }
  /**
   * @brief id Get the element ID
   * @return Element ID
   */
  long id() { return _id; }
  /**
   * @brief changeId Change the ID of the element
   * @param id ID to change to
   */
  void changeId(long id) { _id = id; }
  /**
   * @brief getType Get the element type
   * @return Element type (node/way/relation)
   */
  ElementType::Type getType() { return _type; }
  /**
   * @brief toString Get the XML string equivalent for the element
   * @param changesetId ID of the changeset to insert into the element
   * @return XML string
   */
  virtual QString toString(long changesetId) const = 0;
  /**
   *  Getter/setter for the element version
   */
  long getVersion() { return _version; }
  void setVersion(long version) { _version = version; }
  /** Enumeration of the element status, described below */
  enum ElementStatus
  {
    Available,  //  Not sent, ready to send, or errored
    Buffering,  //  Ready to send, allows for other dependant elements to be sent together
    Sent,       //  Sent to the OSM API
    Finalized,  //  Returned from the OSM API and done
    Failed      //  Returned from the OSM API as an error
  };
  /**
   *  Getter/setter for the element status
   */
  ElementStatus getStatus() { return _status; }
  void setStatus(ElementStatus status) { _status = status; }

protected:
  /**
   * @brief toString Get the XML string of the attributes only
   * @param attributes XML attributes
   * @param changesetId ID of the changeset to insert into the attributes
   * @return XML string
   */
  QString toString(const ElementAttributes& attributes, long changesetId) const;
  /**
   * @brief toTagString Get the XML string of a single tag with key/value pair
   * @param tag Key/value pair from tag
   * @return XML string
   */
  QString toTagString(const ElementTag& tag) const;
  /**
   * @brief escapeString XML encode certain characters in value
   * @param value String value
   * @return XML ecoded string
   */
  QString& escapeString(QString& value) const;
  /** Element type node/way/relation */
  ElementType::Type _type;
  /** Element ID */
  long _id;
  /** Element version */
  long _version;
  /** Element attributes */
  ElementAttributes _object;
  /** Element tag list */
  ElementTags _tags;
  /** Pointer to the ID to ID map */
  ElementIdToIdMap* _idMap;
  /** Element status */
  ElementStatus _status;
};
/** Handy typedef for element shared pointer */
typedef std::shared_ptr<ChangesetElement> ChangesetElementPtr;

/** Simplified changeset node abstraction */
class ChangesetNode : public ChangesetElement
{
public:
  /**
   * @brief ChangesetNode constructor
   * @param node XML node tag and attributes
   * @param idMap ID to ID Map for updated IDs
   */
  ChangesetNode(const XmlObject& node, ElementIdToIdMap* idMap);
  /**
   * @brief ChangesetNode copy constructor
   * @param node ChangesetNode object to copy
   */
  ChangesetNode(const ChangesetNode& node);
  /** Virtual destructor */
  virtual ~ChangesetNode() { }
  /**
   * @brief toString Get the XML string equivalent for the node
   * @param changesetId ID of the changeset to insert into the node
   * @return XML string
   */
  virtual QString toString(long changesetId) const;
};
/** Handy typedef for node shared pointer */
typedef std::shared_ptr<ChangesetNode> ChangesetNodePtr;

/** Simplified changeset way abstraction */
class ChangesetWay : public ChangesetElement
{
public:
  /**
   * @brief ChangesetWay constructor
   * @param way XML way tag and attributes
   * @param idMap ID to ID Map for updated IDs
   */
  ChangesetWay(const XmlObject& way, ElementIdToIdMap* idMap);
  /**
   * @brief ChangesetWay copy constructor
   * @param way ChangesetWay object to copy
   */
  ChangesetWay(const ChangesetWay& way);
  /** Virtual destructor */
  virtual ~ChangesetWay() { }
  /**
   * @brief addNode Add a node ID to the node (in order)
   * @param id Node ID
   */
  void addNode(long id) { _nodes.push_back(id); }
  /**
   * @brief getNode Get node ID based on index
   * @param index Index in the node vector
   * @return Node ID
   */
  long getNode(int index) { return _nodes[index]; }
  /**
   * @brief removeNodes
   * @param position
   * @param count
   */
  void removeNodes(int position, int count = -1);
  /**
   * @brief getNodeCount Get the number of nodes in the way
   * @return number of nodes
   */
  int getNodeCount() { return _nodes.size(); }
  /**
   * @brief toString Get the XML string equivalent for the way
   * @param changesetId ID of the changeset to insert into the way
   * @return XML string
   */
  virtual QString toString(long changesetId) const;

private:
  /** Vector of node ID in the way */
  QVector<long> _nodes;
};
/** Handy typedef for way shared pointer */
typedef std::shared_ptr<ChangesetWay> ChangesetWayPtr;

/** Simplified changeset relation member abstraction */
class ChangesetRelationMember
{
public:
  /**
   * @brief ChangesetRelationMember constructor
   * @param member Member attributes
   * @param idMap ID to ID Map for updated IDs
   */
  ChangesetRelationMember(const QXmlStreamAttributes& member, ElementIdToIdMap* idMap);
  /**
   * @brief isNode/Way/Relation
   * @return true if relation member is node/way/relation
   */
  bool isNode()     { return _type == ElementType::Node; }
  bool isWay()      { return _type == ElementType::Way; }
  bool isRelation() { return _type == ElementType::Relation; }
  /**
   * @brief getType Get the relation member type
   * @return relation type as an enumeration
   */
  ElementType::Type getType() { return _type; }
  /**
   * @brief getRef ID of the member referenced
   * @return Element ID
   */
  long getRef()     { return _ref; }
  /**
   * @brief getRole Get the member roll as a string
   * @return role string
   */
  QString getRole() { return _role; }
  /**
   * @brief toString Get the XML string equivalent for the relation member
   * @return XML string
   */
  QString toString() const;

private:
  /** Member type (node/way/relation) */
  ElementType::Type _type;
  /** Member element ID */
  long _ref;
  /** Member role */
  QString _role;
  /** Pointer to the ID to ID map */
  ElementIdToIdMap* _idMap;
};

/** Simplified changeset relation abstraction */
class ChangesetRelation : public ChangesetElement
{
public:
  /**
   * @brief ChangesetRelation constructor
   * @param relation XML relation tag and attributes
   * @param idMap ID to ID Map for updated IDs
   */
  ChangesetRelation(const XmlObject& relation, ElementIdToIdMap* idMap);
  /**
   * @brief ChangesetRelation copy constructor
   * @param relation ChangesetRelation object to copy
   */
  ChangesetRelation(const ChangesetRelation& relation);
  /** Virtual destructor */
  virtual ~ChangesetRelation() { }
  /**
   * @brief addMember Add relation member
   * @param member XML attributes of the relation member
   */
  void addMember(const QXmlStreamAttributes& member) {  _members.append(ChangesetRelationMember(member, _idMap)); }
  /**
   * @brief getMember Get the member at index
   * @param index Index in the member vector
   * @return relation member
   */
  ChangesetRelationMember& getMember(int index) { return _members[index]; }
  /**
   * @brief getMemberCount Get the number of relation members
   * @return relation member count
   */
  int getMemberCount() { return _members.size(); }
  /**
   * @brief hasMember Search the relation for a specific element type with the given ID
   * @param type Element type to search for (node/way/relation)
   * @param id ID of the element to search for
   * @return True if the given element is a member of this relation
   */
  bool hasMember(ElementType::Type type, long id);
  /**
   * @brief toString Get the XML string equivalent for the relation
   * @param changesetId ID of the changeset to insert into the relation
   * @return XML string
   */
  virtual QString toString(long changesetId) const;

private:
  /** List of relation members */
  QList<ChangesetRelationMember> _members;
};
/** Handy typedef for relation shared pointer */
typedef std::shared_ptr<ChangesetRelation> ChangesetRelationPtr;

/** Custom sorting function to sort IDs from -1 to -n followed by 1 to m */
bool id_sort_order(long lhs, long rhs);
class osm_id_sort
{
public:
  bool operator() (long lhs, long rhs) const
  {
    return id_sort_order(lhs, rhs);
  }
};
/** Handy typedef for a vector of sorted ID maps */
typedef std::vector<std::map<long, long, osm_id_sort>> ChangesetElementIdMap;

/**
 *  Class for storing ID to ID associations, this is required because elements that are created in
 *  a changeset have negative IDs until they are processed by the OSM API.  If a node is used in a
 *  way that is in the same changeset then the negative ID is used.  If they are split across two
 *  different changesets then the node ID must be the updated ID from the API.  The object keeps
 *  those connections.
 */
class ElementIdToIdMap
{
public:
  /** Helpful typedefs for iterators */
  typedef typename std::map<long, long, osm_id_sort>::iterator iterator;
  typedef typename std::map<long, long, osm_id_sort>::const_iterator const_iterator;
  /** Constructor */
  ElementIdToIdMap()
    : _idToId(ElementType::Unknown)
  {}
  /**
   * @brief addId Add ID to the map
   * @param type Element type (node/way/relation)
   * @param id Original (old) ID
   */
  void addId(ElementType::Type type, long id)
  {
    _idToId[type][id] = id;
  }
  /**
   * @brief updateId Set the new ID
   * @param type Element type (node/way/relation)
   * @param old_id Original (old) ID
   * @param new_id Updated (new) ID
   */
  void updateId(ElementType::Type type, long old_id, long new_id)
  {
    _idToId[type][old_id] = new_id;
    _idToId[type][new_id] = old_id;
  }
  /**
   * @brief getId Get the new or old ID from the ID passed in
   * @param type Element type (node/way/relation)
   * @param id ID
   * @return new or old ID
   */
  long getId(ElementType::Type type, long id)
  {
    if (_idToId[type].find(id) == _idToId[type].end())
      return id;
    else
      return _idToId[type][id];
  }
  /**
   * @brief containsId
   * @param type
   * @param id
   * @return
   */
  bool containsId(ElementType::Type type, long id)
  {
    return _idToId[type].find(id) != _idToId[type].end();
  }
  /**
   * @brief begin Get beginning iterator
   * @param type Element type (node/way/relation)
   * @return begin iterator
   */
  iterator begin(ElementType::Type type)  { return _idToId[type].begin(); }
  /**
   * @brief end Get ending iterator
   * @param type Element type (node/way/relation)
   * @return end iterator
   */
  iterator end(ElementType::Type type)    { return _idToId[type].end(); }

private:
  /** Old ID to new ID mapping */
  ChangesetElementIdMap _idToId;
};

}

#endif  //  OSM_API_CHANGESET_ELEMENT_H
