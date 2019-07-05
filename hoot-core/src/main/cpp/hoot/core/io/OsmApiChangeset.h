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

#ifndef OSM_API_CHANGESET_H
#define OSM_API_CHANGESET_H

//  Qt
#include <QList>
#include <QString>
#include <QTextStream>
#include <QVector>
#include <QXmlStreamReader>

//  Standard
#include <array>
#include <map>
#include <set>
#include <vector>

//  Hoot
#include <hoot/core/io/OsmApiChangesetElement.h>
#include <hoot/core/util/DefaultIdGenerator.h>

namespace hoot
{

//  Forward declaration
class ChangesetInfo;
/** Helpful typedefs for pointers and vectors of maps */
typedef std::shared_ptr<ChangesetInfo> ChangesetInfoPtr;
typedef std::map<long, XmlElementPtr, osm_id_sort> XmlElementMap;
typedef QVector<XmlElementMap> ChangesetTypeMap;
typedef std::map<long, std::set<long>> NodeIdToWayIdMap;
typedef std::map<long, std::set<long>> NodeIdToRelationIdMap;
typedef std::map<long, std::set<long>> WayIdToRelationIdMap;
typedef std::map<long, std::set<long>> RelationIdToRelationIdMap;

/** XML Changeset data object */
class XmlChangeset
{
public:
  /** Constructors */
  XmlChangeset();
  explicit XmlChangeset(const QList<QString>& changesets);
  /**  Allow test class to access protected members for white box testing */
  friend class OsmApiChangesetTest;
  /**
   * @brief loadChangeset Load changeset file, can be called multiple times on changeset that are split across files
   * @param changesetPath
   */
  void loadChangeset(const QString& changesetPath);
  /**
   * @brief splitLongWays The API defaults to only allowing a maximum of 2000 nodes per way, split the way if necessary
   * @param maxWayNodes Maximum number of nodes per way from the API capabilities query
   */
  void splitLongWays(long maxWayNodes = 2000);
  /**
   * @brief updateChangeset Update the changeset with the response from the OSM API after part of the changeset is uploaded
   * @param changes - XML Changeset .OSC formatted text
   */
  void updateChangeset(const QString& changes);
  /**
   * @brief fixChangeset Update the underlying element to fix changeset upload errors
   * @param update - OSM XML from OSM API to fix changeset errors
   * @return True if a change was made to fix the changeset
   */
  bool fixChangeset(const QString& update);
  /**
   * @brief fixMalformedInput Fix bad IDs in data,
   *  -- Adds must be negative IDs - Positive IDs are fixed by creating new negative IDs
   *  -- Modifies must be positive IDs - Negative IDs are set as an error and never sent to the API
   *  -- Deletes must be positive IDs - Negative IDs are set as an error and never sent to the API
   */
  void fixMalformedInput();
  /**
   * @brief hasElementsToSend Checks if all elements have been marked as sent
   * @return true if there are elements that haven't been sent yet
   */
  bool hasElementsToSend() { return (long)(_allNodes.size() + _allWays.size() + _allRelations.size()) > _sentCount; }
  /**
   * @brief isDone Checks if all elements are either finalized or failed
   * @return true if there aren't any elements left to receive updates from
   */
  bool isDone() { return (long)(_allNodes.size() + _allWays.size() + _allRelations.size()) == _processedCount + _failedCount; }
  /** Elements in a changeset can be in three sections, create, modify, and delete.  Max is used for iterating */
  enum ChangesetType : int
  {
    TypeCreate = 0,
    TypeModify,
    TypeDelete,
    TypeMax
  };
  /**
   * @brief calculateChangeset Create an atomic subset of this changeset that can be sent independently from others
   * @param changeset - Pointer to a ChangesetInfo object holding IDs for a subset of the changeset
   * @return true if an atomic changeset subset was created
   */
  bool calculateChangeset(ChangesetInfoPtr& changeset);
  /**
   * @brief splitChangeset Split the subset in half, this happens if a subset fails.  This essentially is a binary search
   *  for the failing element(s) to isolate them into single element changesets that are then marked as failed.
   * @param changeset - Pointer to the current subset that failed, is changed to include half of the resulting split
   * @param splitHint - Text hint possibly indicating which record is failing
   * @return pointer to half of the subset to be sent back to the OSM API
   */
  ChangesetInfoPtr splitChangeset(ChangesetInfoPtr changeset, const QString& splitHint = "");
  /**
   * @brief updateFailedChangeset Update the changeset to mark elements as failed if the ChangesetInfo object has been "fixed"
   * @param changeset - Pointer to changeset info object with one element that has failed
   */
  void updateFailedChangeset(ChangesetInfoPtr changeset);
  /**
   * @brief getChangesetString Get the .OSC formatted string for this subset of the changeset with the changeset ID in it
   * @param changeset - Subset of the changeset to render
   * @param changeset_id - ID of the changeset from the OSM API to upload to
   * @return XML string in .OSC format
   */
  QString getChangesetString(ChangesetInfoPtr changeset, long changeset_id);
  /**
   * @brief getFailedChangesetString Get the .OSC formatted string for all of the failed elements in this changeset
   * @return XML string in .OSC format with the changeset ID set to 0
   */
  QString getFailedChangesetString();
  /**
   * @brief setMaxSize Set the soft maximum size of the changeset.  Max is soft because if while creating a way there are still
   *  more required nodes to add and the max is hit, those nodes are added to the changeset to make it atomic
   * @param size - Number of elements in the soft max size
   */
  void setMaxSize(long size) { _maxChangesetSize = size; }
  /**
   * @brief hasFailedElements
   * @return true if any elements failed upload
   */
  bool hasFailedElements() { return _failedCount > 0; }
  /**
   * @brief getFailedCount
   * @return number of failed elements
   */
  long getFailedCount()         { return _failedCount; }
  /**
   * @brief getTotalElementCount
   * @return total number of elements in the changeset
   */
  long getTotalElementCount()   { return _allNodes.size() + _allWays.size() + _allRelations.size(); }
  /**
   * @brief getTotalNodeCount
   * @return total number of nodes in the changeset (create, modify, or delete)
   */
  long getTotalNodeCount()      { return _allNodes.size(); }
  /**
   * @brief getTotalWayCount
   * @return total number of ways in the changeset (create, modify, or delete)
   */
  long getTotalWayCount()       { return _allWays.size(); }
  /**
   * @brief getTotalRelationCount
   * @return total number of relations in the changeset (create, modify, or delete)
   */
  long getTotalRelationCount()  { return _allRelations.size(); }
  /**
   * @brief getTotalCreateCount
   * @return total number of nodes/ways/relations created in the changeset
   */
  long getTotalCreateCount()    { return _nodes[TypeCreate].size() + _ways[TypeCreate].size() + _relations[TypeCreate].size(); }
  /**
   * @brief getTotalModifyCount
   * @return total number of nodes/ways/relations modified in the changeset
   */
  long getTotalModifyCount()    { return _nodes[TypeModify].size() + _ways[TypeModify].size() + _relations[TypeModify].size(); }
  /**
   * @brief getTotalDeleteCount
   * @return total number of nodes/ways/relations deleted in the changeset
   */
  long getTotalDeleteCount()    { return _nodes[TypeDelete].size() + _ways[TypeDelete].size() + _relations[TypeDelete].size(); }
  /**
   * @brief getProcessedCount
   * @return Number of elements processed so far
   */
  long getProcessedCount()      { return _processedCount; }

private:
  /**
   * @brief updateElement Update the element ID map with the new ID and update the version of the element
   * @param map Map of elements (nodes/ways/relations)
   * @param old_id
   * @param new_id
   * @param version New version of the element
   */
  void updateElement(ChangesetTypeMap& map, long old_id, long new_id, long version);
  /**
   * @brief fixElement Fix the element with ID by updating the version of the element and merge
   *     the set of tags
   *     Could expand in the future to correct other issues but for now just fix the version
   * @param map Map of elements (nodes/ways/relations)
   * @param id ID of the element to fix
   * @param version Latest version from OSM API
   * @param tags Current set of tags on the object
   * @return True if a change was made to fix the element
   */
  bool fixElement(ChangesetTypeMap& map, long id, long version, QMap<QString, QString> tags);
  /**
   * @brief loadElements Load elements from the XML reader of type 'type'
   * @param reader XML reader of the file
   * @param type Type of operation (create/modify/delete)
   */
  void loadElements(QXmlStreamReader& reader, ChangesetType type);
  /**
   * @brief getChangeset Get all of the elements' XML for the given type
   * @param changeset Subset of the changeset to render
   * @param changeset_id ID of the changeset
   * @param type Type of operation (create/modify/delete)
   * @return XML string for a particular
   */
  QString getChangeset(ChangesetInfoPtr changeset, long changeset_id, ChangesetType type);
  /**
   * @brief addNodes/Ways/Relations Add nodes/ways/relations of a type to the subset
   * @param changeset Subset of the changeset to add to
   * @param type Type of operation (create/modify/delete)
   * @return success
   */
  bool addNodes(ChangesetInfoPtr& changeset, ChangesetType type);
  bool addWays(ChangesetInfoPtr& changeset, ChangesetType type);
  bool addRelations(ChangesetInfoPtr& changeset, ChangesetType type);
  /**
   * @brief addNode/Way/Relation Add single node/way/relation of a type to the subset
   * @param changeset Subset of the changeset to add to
   * @param type Type of operation (create/modify/delete)
   * @param node/way/relation Pointer to the element that is being added
   * @return success
   */
  bool addNode(ChangesetInfoPtr& changeset, ChangesetType type, XmlNode* node);
  bool addWay(ChangesetInfoPtr& changeset, ChangesetType type, XmlWay* way);
  bool addRelation(ChangesetInfoPtr& changeset, ChangesetType type, XmlRelation* relation);
  /**
   * @brief addParentWays/Relations Add any parents (ways, relations) to the changeset if needed
   *  by a modify or delete operation.  For example, to delete a node, any way or relation parent
   *  also needs to be checked before the node can be added to the changeset.  If the parent is in
   *  the current changeset or the parent is finalized, the deleted element can be added.
   * @param changeset Changeset to add parent to
   * @param way_ids/relation_ids Set of IDs to check against if the parent is able to be added
   * @return False if adding the element is blocked by the parents that are passed in
   */
  bool addParentWays(ChangesetInfoPtr& changeset, const std::set<long>& way_ids);
  bool addParentRelations(ChangesetInfoPtr& changeset, const std::set<long>& relation_ids);
  /**
   * @brief moveNode/Way/Relation Move an element from one subset to another, used by the splitting action
   * @param source Subset containing the element
   * @param destination Subset to move to
   * @param type Type of operation (create/modify/delete)
   * @param node/way/relation Pointer to the element to be moved
   * @return
   */
  bool moveNode(ChangesetInfoPtr& source, ChangesetInfoPtr& destination, ChangesetType type, XmlNode* node);
  bool moveWay(ChangesetInfoPtr& source, ChangesetInfoPtr& destination, ChangesetType type, XmlWay* way);
  bool moveRelation(ChangesetInfoPtr& source, ChangesetInfoPtr& destination, ChangesetType type, XmlRelation* relation);
  /**
   * @brief canMoveNode/Way/Relation Query if a node/way/relation can be moved.  This checks downstream relations, ways,
   *  and nodes to see if they can also be moved.
   * @param source Subset containing the element
   * @param destination Subset to move to
   * @param type Type of operation (create/modify/delete)
   * @param node/way/relation Pointer to the element to be checked
   * @return
   */
  bool canMoveNode(ChangesetInfoPtr& source, ChangesetInfoPtr& destination, ChangesetType type, XmlNode* node);
  bool canMoveWay(ChangesetInfoPtr& source, ChangesetInfoPtr& destination, ChangesetType type, XmlWay* way);
  bool canMoveRelation(ChangesetInfoPtr& source, ChangesetInfoPtr& destination, ChangesetType type, XmlRelation* relation);
  /**
   * @brief getObjectCount Get the number of elements affected by this node/way/relation
   * @param changeset Subset containing the element
   * @param node/way/relation Pointer to the element to count
   * @return total number of elements within this element
   */
  size_t getObjectCount(ChangesetInfoPtr& changeset, XmlNode* node);
  size_t getObjectCount(ChangesetInfoPtr& changeset, XmlWay* way);
  size_t getObjectCount(ChangesetInfoPtr& changeset, XmlRelation* relation);
  /**
   * @brief isSent Check if this element's status is buffering, sent, or finalized
   * @param element Pointer to the element to check
   * @return true if the element has been sent to the API
   */
  bool isSent(XmlElement* element);
  /**
   * @brief canSend Check if the node/way/relation can be sent (if it is available) along with all of its downstream elements
   * @param node/way/relation Pointer to the element to check
   * @return true if element and all downstream elements are available to send
   */
  bool canSend(XmlNode* node);
  bool canSend(XmlWay* way);
  bool canSend(XmlRelation* relation);
  /**
   * @brief markBuffered Mark the element as buffered
   * @param element Pointer to the element to mark
   */
  void markBuffered(XmlElement* element);
  /**
   * @brief getNextNode/Way/RelationId searches the Create section of the changeset to find the next available ID
   *  for the desired element type
   * @return next available negative node/way/relation ID
   */
  long getNextNodeId();
  long getNextWayId();
  long getNextRelationId();
  /**
   * @brief replaceNode/Way/RelationId Replace the old ID with the new ID in the element ID to ID map
   * @param old_id ID from changeset file
   * @param new_id New ID from API or from changeset fixes
   */
  void replaceNodeId(long old_id, long new_id);
  void replaceWayId(long old_id, long new_id);
  void replaceRelationId(long old_id, long new_id);
  /**
   * @brief failNode/Way/Relation Set element's status to failed and up the failed count
   * @param id ID of the node/way/relation to fail
   * @param beforeSend True if it was set to failed before sending
   */
  void failNode(long id, bool beforeSend = false);
  void failWay(long id, bool beforeSend = false);
  void failRelation(long id, bool beforeSend = false);
  /**
   * @brief matchesPlaceholderFailure Checks the return from the API to see if it is similar to the following error message:
   *        "Placeholder node not found for reference -145213 in way -5687"
   * @param hint Error message from OSM API
   * @param member_id ID of the member element that caused the element to fail
   * @param member_type Type of the member element that caused the element to fail
   * @param element_id ID of the element that failed
   * @param element_type Type of the element that failed
   * @return True if the message matches and was parsed
   */
  bool matchesPlaceholderFailure(const QString& hint,
                                 long& member_id, ElementType::Type& member_type,
                                 long& element_id, ElementType::Type& element_type);
  /**
   * @brief matchesRelationFailure Checks the return from the API to see if it is similar to the following error message:
   * @param hint Error message from OSM API
   * @param element_id ID of the element that failed
   * @param member_id ID of the member element that caused the element to fail
   * @param member_type Type of the member element that caused the element to fail
   * @return True if the message matches and was parsed
   */
  bool matchesRelationFailure(const QString& hint, long& element_id, long& member_id, ElementType::Type& member_type);
  /**
   * @brief matchesChangesetPreconditionFailure
   * @param hint Error message from OSM API
   * @param member_id ID of the member element that caused the element to fail
   * @param member_type Type of the member element that caused the element to fail
   * @param element_id ID of the element that failed
   * @param element_type Type of the element that failed
   * @return True if the message matches and was parsed
   */
  bool matchesChangesetPreconditionFailure(const QString& hint,
                                           long& member_id, ElementType::Type& member_type,
                                           long& element_id, ElementType::Type& element_type);
  /**
   * @brief getNodes/Ways/Relations Output node/way/relation text to the text stream
   * @param changeset Pointer to the changeset to output
   * @param ts TextStream to output the element to
   * @param type Changeset type (create, modify, delete)
   * @param changeset_id ID of the changeset to write to the element attribute
   */
  void getNodes(const ChangesetInfoPtr& changeset, QTextStream& ts, ChangesetType type, long changeset_id);
  void getWays(const ChangesetInfoPtr& changeset, QTextStream& ts, ChangesetType type, long changeset_id);
  void getRelations(const ChangesetInfoPtr& changeset, QTextStream& ts, ChangesetType type, long changeset_id);
  /** Sorted map of all nodes, original node ID and a pointer to the element object */
  XmlElementMap _allNodes;
  /** Sorted map of all ways, original node ID and a pointer to the element object */
  XmlElementMap _allWays;
  /** Sorted map of all relations, original node ID and a pointer to the element object */
  XmlElementMap _allRelations;
  /** Three element (create/modify/delete) vector of node IDs */
  ChangesetTypeMap _nodes;
  /** Three element (create/modify/delete) vector of way IDs */
  ChangesetTypeMap _ways;
  /** Three element (create/modify/delete) vector of relation IDs */
  ChangesetTypeMap _relations;
  /** Element ID to ID data structure for checking old ID to new ID and new ID to old ID lookups */
  ElementIdToIdMap _idMap;
  /** Soft maximum changeset size */
  long _maxChangesetSize;
  /** Count of elements that have been sent */
  long _sentCount;
  /** Count of elements that have been processed */
  long _processedCount;
  /** Count of elements that failed to upload */
  long _failedCount;
  /** Buffer of elements that are about to be pushed into a subset */
  std::vector<XmlElement*> _sendBuffer;
  /** Negative ID generator */
  DefaultIdGenerator _idGen;
  /** Reverse ID to ID maps for deleting elements validation */
  NodeIdToWayIdMap _nodeIdsToWays;
  NodeIdToRelationIdMap _nodeIdsToRelations;
  WayIdToRelationIdMap _wayIdsToRelations;
  RelationIdToRelationIdMap _relationIdsToRelations;
};

/** Atomic subset of IDs that are sent to the OSM API, header only class */
class ChangesetInfo
{
public:
  /** Helpful typedefs for container and iterators */
  typedef std::unordered_set<long> container;
  typedef typename container::iterator iterator;
  typedef typename container::const_iterator const_iterator;
  /** Constructor */
  ChangesetInfo() : _changesetIssuesResolved(false) { }
  /**
   * @brief add Add an element ID of a certain type to the changeset type
   * @param element_type Describes the 'id' argument as a node, way, or relation
   * @param changeset_type Describes the changeset method as create, modify, or delete
   * @param id Element ID of the element to add
   */
  void add(ElementType::Type element_type, XmlChangeset::ChangesetType changeset_type, long id)
  { _changeset[element_type][changeset_type].insert(id); }
  /**
   * @brief remove Remove an element ID of a certain type from the changeset type
   * @param element_type Describes the 'id' argument as a node, way, or relation
   * @param changeset_type Describes the changeset method as create, modify, or delete
   * @param id Element ID of the element to remove
   */
  void remove(ElementType::Type element_type, XmlChangeset::ChangesetType changeset_type, long id)
  {
    container& selectedSet = _changeset[element_type][changeset_type];
    if (selectedSet.find(id) != selectedSet.end())
      selectedSet.erase(id);
  }

  long getFirst(ElementType::Type element_type, XmlChangeset::ChangesetType changeset_type)
  { return *(_changeset[element_type][changeset_type].begin()); }
  /**
   * @brief clear Clear out this entire changeset subset
   */
  void clear()
  {
    for (int i = 0; i < (int)ElementType::Unknown; ++i)
    {
      for (int j = 0; j < (int)XmlChangeset::TypeMax; ++j)
        _changeset[i][j].clear();
    }
  }
  /**
   * @brief contains Check if this subset contains an element described by types and ID
   * @param element_type Describes the 'id' argument as a node, way, or relation
   * @param changeset_type Describes the changeset method as create, modify, or delete
   * @param id Element ID that is being searched for
   * @return true if it is found
   */
  bool contains(ElementType::Type element_type, XmlChangeset::ChangesetType changeset_type, long id)
  {
    return _changeset[element_type][changeset_type].find(id) != end(element_type, changeset_type);
  }
  /**
   * @brief begin Begin iterator
   * @param element_type Describes the type (node/way/relation) to iterate
   * @param changeset_type Describes the type (create/modify/delete) to iterate
   * @return iterator pointing to the beginning of the set
   */
  iterator begin(ElementType::Type element_type, XmlChangeset::ChangesetType changeset_type)
  {
    return _changeset[element_type][changeset_type].begin();
  }
  /**
   * @brief end End iterator
   * @param element_type Describes the type (node/way/relation) to iterate
   * @param changeset_type Describes the type (create/modify/delete) to iterate
   * @return iterator pointing off of the end of the set
   */
  iterator end(ElementType::Type element_type, XmlChangeset::ChangesetType changeset_type)
  { return _changeset[element_type][changeset_type].end(); }
  /**
   * @brief size Total number of ElementType::Type elements (node/way/relation) of a specific changeset
   *  type (create/modify/delete) within this subset
   * @param elementType Describes the type (node/way/relation) to count
   * @param changesetType Describes the type (create/modify/delete) to count
   * @return count based on types
   */
  size_t size(ElementType::Type elementType, XmlChangeset::ChangesetType changesetType)
  {
    return _changeset[(int)elementType][(int)changesetType].size();
  }
  /**
   * @brief size Total number of elements in the subset
   * @return total count
   */
  size_t size()
  {
    size_t s = 0;
    //  Iterate element types
    for (int i = 0; i < (int)ElementType::Unknown; ++i)
    {
      //  Sum up all counts for each changeset type
      for (int j = 0; j < (int)XmlChangeset::TypeMax; ++j)
        s += _changeset[i][j].size();
    }
    return s;
  }

  bool getChangesetIssuesResolved() { return _changesetIssuesResolved; }
  void setChangesetIssuesResolved(bool resolved) { _changesetIssuesResolved = resolved; }

private:
  /** 3x3 array of containers for elements in this subset */
  std::array<std::array<container, XmlChangeset::TypeMax>, ElementType::Unknown> _changeset;
  /** Flag set after attempt to resolve changeset issues has completed. */
  bool _changesetIssuesResolved;
};

}

#endif  //  OSM_API_CHANGESET_H
