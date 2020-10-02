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
 * @copyright Copyright (C) 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
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
#include <mutex>
#include <set>
#include <vector>

//  Hoot
#include <hoot/core/io/OsmApiChangesetElement.h>
#include <hoot/core/io/OsmApiMatchFailure.h>
#include <hoot/core/util/DefaultIdGenerator.h>

namespace hoot
{

//  Forward declaration
class ChangesetInfo;
/** Helpful typedefs for pointers and vectors of maps */
typedef std::shared_ptr<ChangesetInfo> ChangesetInfoPtr;
typedef std::map<long, ChangesetElementPtr, osm_id_sort> ChangesetElementMap;
typedef std::vector<ChangesetElementMap> ChangesetTypeMap;
typedef std::map<long, std::set<long>> NodeIdToWayIdMap;
typedef std::map<long, std::set<long>> NodeIdToRelationIdMap;
typedef std::map<long, std::set<long>> WayIdToRelationIdMap;
typedef std::map<long, std::set<long>> RelationIdToRelationIdMap;
typedef std::vector<std::set<long>> ElementCountSet;

/** XML Changeset data object */
class XmlChangeset
{
public:
  /** Constructors */
  XmlChangeset();
  explicit XmlChangeset(const QString& changeset);
  explicit XmlChangeset(const QList<QString>& changesets);
  /**  Allow test class to access protected members for white box testing */
  friend class OsmApiChangesetTest;
  /**
   * @brief loadChangeset Load changeset file or string, can be called multiple times on changeset that are split across files
   * @param changeset Path to changeset file or changeset as a string
   */
  void loadChangeset(const QString& changeset);
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
  bool isDone() { return (long)(_allNodes.size() + _allWays.size() + _allRelations.size()) <= _processedCount + _failedCount; }
  /** Convert ChangesetType to string */
  static QString getString(ChangesetType type);
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
  ChangesetInfoPtr splitChangeset(const ChangesetInfoPtr& changeset, const QString& splitHint = "");
  /**
   * @brief updateFailedChangeset Update the changeset to mark elements as failed if the ChangesetInfo object has been "fixed"
   * @param changeset - Pointer to changeset info object with one element that has failed
   */
  void updateFailedChangeset(const ChangesetInfoPtr& changeset, bool forceFailure = false);
  /**
   * @brief getChangesetString Get the .OSC formatted string for this subset of the changeset with the changeset ID in it
   * @param changeset - Subset of the changeset to render
   * @param changeset_id - ID of the changeset from the OSM API to upload to
   * @return XML string in .OSC format
   */
  QString getChangesetString(const ChangesetInfoPtr& changeset, long changeset_id);
  /**
   * @brief getFailedChangesetString Get the .OSC formatted string for all of the failed elements in this changeset
   * @return XML string in .OSC format with the changeset ID set to 0
   */
  QString getFailedChangesetString();
  /**
   * @brief setMaxPushSize Set the maximum size of the push changeset.  Max is soft because if while creating a way there are still
   *  more required nodes to add and the max is hit, those nodes are added to the changeset to make it atomic
   * @param size - Number of elements in the soft max size
   */
  void setMaxPushSize(long size) { _maxPushSize = size; }
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
  /**
   * @brief setErrorPathname Record the pathname of the error changeset
   * @param path Pathname
   */
  void setErrorPathname(const QString& path) { _errorPathname = path; }
  /**
   * @brief writeErrorFile Writes our the error changeset
   * @return true if the file was written successfully
   */
  bool writeErrorFile();
  /**
   * @brief calculateRemainingChangeset This function is an error correction case for when a changeset cannot finish
   *  and the upload stalls indefinitely.  Move all remaining elements into a changeset so the job can finish or error out.
   * @param changeset Reference to the changeset info for changeset creation
   * @return true if there is anything in the changeset
   */
  bool calculateRemainingChangeset(ChangesetInfoPtr& changeset);
  /**
   * @brief updateRemainingChangeset
   */
  void updateRemainingChangeset();
  /**
   * @brief isMatch Function to compare two changesets
   * @param changeset Changeset object to compare this changeset against
   * @return true if they are equivalent
   */
  bool isMatch(const XmlChangeset& changeset);
  /**
   * @brief failRemainingChangeset Set all remaining elements to the failed state
   */
  void failRemainingChangeset();
  /**
   * @brief failChangeset Set all elements' status to failed that are in the changeset
   * @param changeset ChangesetInfo pointer of elements that all failed
   */
  void failChangeset(const ChangesetInfoPtr& changeset);

  const OsmApiMatchFailure& getFailureCheck() const { return _failureCheck; }
  /**
   * @brief getCleanupElements Get the last elements that need to be cleaned up
   * @return ChangesetInfo list of elements for cleaning
   */
  ChangesetInfoPtr getCleanupElements();
  /**
   * @brief clearCleanupElements Empty the clean-up list
   */
  void clearCleanupElements();
  /**
   * @brief getCleanupCount Get the number of elements that need clean-up
   * @return Element count
   */
  int getCleanupCount() { return _cleanupCount; }

private:
  /**
   * @brief loadChangesetFile Load changeset file, can be called multiple times on changeset that are split across files
   * @param changesetPath
   */
  void loadChangesetFile(const QString& changesetPath);
  /**
   * @brief loadChangesetXml Load changeset XML text, can be called multiple times on changesets that are split
   * @param changesetXml
   */
  void loadChangesetXml(const QString& changesetXml);
  /**
   * @brief loadChangesetDirectory Load directory of changeset files, can include request/response changeset files
   * @param changesetDirectory Full pathname of the directory to load
   */
  void loadChangesetDirectory(const QString& changesetDirectory);
  /**
   * @brief loadChangeset Load a .osc changeset file
   * @param reader
   */
  void loadChangeset(QXmlStreamReader& reader);
  /**
   * @brief loadOsmAsChangeset Load an OSM file as a changeset of <create> elements
   * @param reader
   */
  void loadOsmAsChangeset(QXmlStreamReader& reader);
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
  QString getChangeset(const ChangesetInfoPtr& changeset, long changeset_id, ChangesetType type);
  /**
   * @brief addNodes/Ways/Relations Add nodes/ways/relations of a type to the subset
   * @param changeset Subset of the changeset to add to
   * @param type Type of operation (create/modify/delete)
   * @return success
   */
  bool addNodes(const ChangesetInfoPtr& changeset, ChangesetType type);
  bool addWays(const ChangesetInfoPtr& changeset, ChangesetType type);
  bool addRelations(const ChangesetInfoPtr& changeset, ChangesetType type);
  /**
   * @brief addNode/Way/Relation Add single node/way/relation of a type to the subset
   * @param changeset Subset of the changeset to add to
   * @param type Type of operation (create/modify/delete)
   * @param node/way/relation Pointer to the element that is being added
   * @return success
   */
  bool addNode(const ChangesetInfoPtr& changeset, ChangesetType type, ChangesetNode* node);
  bool addWay(const ChangesetInfoPtr& changeset, ChangesetType type, ChangesetWay* way);
  bool addRelation(const ChangesetInfoPtr& changeset, ChangesetType type, ChangesetRelation* relation);
  /**
   * @brief addParentWays/Relations Add any parents (ways, relations) to the changeset if needed
   *  by a modify or delete operation.  For example, to delete a node, any way or relation parent
   *  also needs to be checked before the node can be added to the changeset.  If the parent is in
   *  the current changeset or the parent is finalized, the deleted element can be added.
   * @param changeset Changeset to add parent to
   * @param way_ids/relation_ids Set of IDs to check against if the parent is able to be added
   * @return False if adding the element is blocked by the parents that are passed in
   */
  bool addParentWays(const ChangesetInfoPtr& changeset, const std::set<long>& way_ids);
  bool addParentRelations(const ChangesetInfoPtr& changeset, const std::set<long>& relation_ids);
  /**
   * @brief moveNode/Way/Relation Move an element from one subset to another, used by the splitting action
   * @param source Subset containing the element
   * @param destination Subset to move to
   * @param type Type of operation (create/modify/delete)
   * @param node/way/relation Pointer to the element to be moved
   * @param failing Set to true if the element is getting set to failed state, it is more selective about moves
   * @return false if the element cannot be moved successfully
   */
  bool moveNode(const ChangesetInfoPtr& source, const ChangesetInfoPtr& destination, ChangesetType type, ChangesetNode* node, bool failing = false);
  bool moveWay(const ChangesetInfoPtr& source, const ChangesetInfoPtr& destination, ChangesetType type, ChangesetWay* way, bool failing = false);
  bool moveRelation(const ChangesetInfoPtr& source, const ChangesetInfoPtr& destination, ChangesetType type, ChangesetRelation* relation, bool failing = false);
  /**
   * @brief moveOrRemoveNode/Way/Relation Move an element from one subset to another, or if all related elements aren't
   *   able to be moved, the element is removed from the subset and returned to the `available` state
   * @param source Subset containing the element
   * @param destination Subset to move to
   * @param type Type of operation (create/modify/delete)
   * @param node/way/relation Pointer to the element to be moved
   */
  void moveOrRemoveNode(const ChangesetInfoPtr& source, const ChangesetInfoPtr& destination, ChangesetType type, ChangesetNode* node);
  void moveOrRemoveWay(const ChangesetInfoPtr& source, const ChangesetInfoPtr& destination, ChangesetType type, ChangesetWay* way);
  void moveOrRemoveRelation(const ChangesetInfoPtr& source, const ChangesetInfoPtr& destination, ChangesetType type, ChangesetRelation* relation);
  /**
   * @brief canMoveNode/Way/Relation Query if a node/way/relation can be moved.  This checks downstream relations, ways,
   *  and nodes to see if they can also be moved.
   * @param source Subset containing the element
   * @param destination Subset to move to
   * @param type Type of operation (create/modify/delete)
   * @param node/way/relation Pointer to the element to be checked
   * @return
   */
  bool canMoveNode(const ChangesetInfoPtr& source, const ChangesetInfoPtr& destination, ChangesetType type, ChangesetNode* node);
  bool canMoveWay(const ChangesetInfoPtr& source, const ChangesetInfoPtr& destination, ChangesetType type, ChangesetWay* way);
  bool canMoveRelation(const ChangesetInfoPtr& source, const ChangesetInfoPtr& destination, ChangesetType type, ChangesetRelation* relation);
  /**
   * @brief getObjectCount Get the number of elements affected by this node/way/relation
   * @param changeset Subset containing the element
   * @param node/way/relation Pointer to the element to count
   * @param elements Reference to a vector of sets of IDs so that node/way/relation IDs aren't counted twice
   * @param countSent
   * @return total number of elements within this element
   */
  size_t getObjectCount(ChangesetNode* node, ElementCountSet& elements, bool countSent = true);
  size_t getObjectCount(ChangesetWay* way, ElementCountSet& elements, bool countSent = true);
  size_t getObjectCount(ChangesetRelation* relation, ElementCountSet& elements, bool countSent = true);
  size_t getObjectCount(const ChangesetInfoPtr& changeset, ChangesetNode* node, ElementCountSet& elements, bool countSent = true);
  size_t getObjectCount(const ChangesetInfoPtr& changeset, ChangesetWay* way, ElementCountSet& elements, bool countSent = true);
  size_t getObjectCount(const ChangesetInfoPtr& changeset, ChangesetRelation* relation, ElementCountSet& elements, bool countSent = true);
  /**
   * @brief isSent Check if this element's status is buffering, sent, or finalized
   * @param element Pointer to the element to check
   * @return true if the element has been sent to the API
   */
  bool isSent(ChangesetElement* element);
  /**
   * @brief canSend Check if the node/way/relation can be sent (if it is available) along with all of its downstream elements
   * @param node/way/relation Pointer to the element to check
   * @return true if element and all downstream elements are available to send
   */
  bool canSend(ChangesetNode* node);
  bool canSend(ChangesetWay* way);
  bool canSend(ChangesetRelation* relation);
  /**
   * @brief markBuffered Mark the element as buffered
   * @param element Pointer to the element to mark
   */
  void markBuffered(ChangesetElement* element);
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
   * @brief writeNodes/Ways/Relations Output node/way/relation text to the text stream
   * @param changeset Pointer to the changeset to output
   * @param ts TextStream to output the element to
   * @param type Changeset type (create, modify, delete)
   * @param changeset_id ID of the changeset to write to the element attribute
   */
  void writeNodes(const ChangesetInfoPtr& changeset, QTextStream& ts, ChangesetType type, long changeset_id);
  void writeWays(const ChangesetInfoPtr& changeset, QTextStream& ts, ChangesetType type, long changeset_id);
  void writeRelations(const ChangesetInfoPtr& changeset, QTextStream& ts, ChangesetType type, long changeset_id);
  void writeElements(const ChangesetInfoPtr& changeset, QTextStream& ts, ChangesetType type, long changeset_id,
                     ElementType::Type elementType, const ChangesetElementMap& elements);
  /**
   * @brief failRemainingElements Fail all non-finalized elements
   * @param elements Map of elements to fail
   */
  void failRemainingElements(const ChangesetElementMap& elements);
  /**
   * @brief getRemainingFilename Get the filename for the "remaining" elements
   * @return _errorPathname with "error" replaced by "remaining"
   */
  QString getRemainingFilename();
  /**
   * @brief fixOrphanedNodesSplit When a changeset info object is split it may produce orphaned nodes, this
   *   function splits them out to the `_cleanup` object
   * @param changeset Changeset info with one half of the data and any potential orphaned nodes
   * @param split Changeset info with the other half of the data split off
   * @return Split object to push back on queue
   */
  ChangesetInfoPtr fixOrphanedNodesSplit(const ChangesetInfoPtr& changeset, const ChangesetInfoPtr& split);
  /**
   * @brief insertElement Insert an element into the changeset in a generic function
   * @param element Element to insert
   * @param type Changeset type (create/modify/delete)
   * @param elementMap List of elements for the changeset types (_nodes/_ways/_relations)
   * @param all List of all elements in the changeset (_allNodes/_allWays/_allRelations)
   */
  void insertElement(const ChangesetElementPtr& element, ChangesetType type, ChangesetTypeMap& elementMap, ChangesetElementMap& all);
  /** Sorted map of all nodes, original node ID and a pointer to the element object */
  ChangesetElementMap _allNodes;
  /** Sorted map of all ways, original node ID and a pointer to the element object */
  ChangesetElementMap _allWays;
  /** Sorted map of all relations, original node ID and a pointer to the element object */
  ChangesetElementMap _allRelations;
  /** Three element (create/modify/delete) vector of node IDs */
  ChangesetTypeMap _nodes;
  /** Three element (create/modify/delete) vector of way IDs */
  ChangesetTypeMap _ways;
  /** Three element (create/modify/delete) vector of relation IDs */
  ChangesetTypeMap _relations;
  /** Element ID to ID data structure for checking old ID to new ID and new ID to old ID lookups */
  ElementIdToIdMap _idMap;
  /** Maximum changeset push size, could be slightly over to get an entire element */
  long _maxPushSize;
  /** Maximum size of a changeset that cannot be exceeded */
  long _maxChangesetSize;
  /** Count of elements that have been sent */
  long _sentCount;
  /** Count of elements that have been processed */
  long _processedCount;
  /** Count of elements that failed to upload */
  long _failedCount;
  /** Buffer of elements that are about to be pushed into a subset */
  std::vector<ChangesetElement*> _sendBuffer;
  /** Negative ID generator */
  DefaultIdGenerator _idGen;
  /** Reverse ID to ID maps for deleting elements validation */
  NodeIdToWayIdMap _nodeIdsToWays;
  NodeIdToRelationIdMap _nodeIdsToRelations;
  WayIdToRelationIdMap _wayIdsToRelations;
  RelationIdToRelationIdMap _relationIdsToRelations;
  /** Full pathname of the error file changeset, if any errors occur */
  QString _errorPathname;
  std::mutex _errorMutex;
  /** OSM API error matching object */
  OsmApiMatchFailure _failureCheck;
  /** Mutex for cleanup changeset info */
  std::mutex _cleanupMutex;
  /** Changeset information for cleanup at the end to reduce orphaned deletes */
  ChangesetInfoPtr _cleanup;
  /** Count of elements in the cleanup work */
  int _cleanupCount;
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
  ChangesetInfo();
  /**
   * @brief add Add an element ID of a certain type to the changeset type
   * @param element_type Describes the 'id' argument as a node, way, or relation
   * @param changeset_type Describes the changeset method as create, modify, or delete
   * @param id Element ID of the element to add
   */
  void add(ElementType::Type element_type, ChangesetType changeset_type, long id);
  /**
   * @brief remove Remove an element ID of a certain type from the changeset type
   * @param element_type Describes the 'id' argument as a node, way, or relation
   * @param changeset_type Describes the changeset method as create, modify, or delete
   * @param id Element ID of the element to remove
   */
  void remove(ElementType::Type element_type, ChangesetType changeset_type, long id);
  /**
   * @brief getFirst Get the first element of the types
   * @param element_type Describes the element type: node, way, or relation
   * @param changeset_type Describes the type: create, modify, delete
   * @return
   */
  long getFirst(ElementType::Type element_type, ChangesetType changeset_type);
  /**
   * @brief clear Clear out this entire changeset subset
   */
  void clear();
  /**
   * @brief contains Check if this subset contains an element described by the type and ID
   * @param element_type Describes the 'id' argument as a node, way, or relation
   * @param id Element ID that is being searched for
   * @return true if it is found in any changeset type
   */
  bool contains(ElementType::Type element_type, long id);
  /**
   * @brief contains Check if this subset contains an element described by types and ID
   * @param element_type Describes the 'id' argument as a node, way, or relation
   * @param changeset_type Describes the changeset method as create, modify, or delete
   * @param id Element ID that is being searched for
   * @return true if it is found
   */
  bool contains(ElementType::Type element_type, ChangesetType changeset_type, long id);
  /**
   * @brief begin Begin iterator
   * @param element_type Describes the type (node/way/relation) to iterate
   * @param changeset_type Describes the type (create/modify/delete) to iterate
   * @return iterator pointing to the beginning of the set
   */
  iterator begin(ElementType::Type element_type, ChangesetType changeset_type);
  /**
   * @brief end End iterator
   * @param element_type Describes the type (node/way/relation) to iterate
   * @param changeset_type Describes the type (create/modify/delete) to iterate
   * @return iterator pointing off of the end of the set
   */
  iterator end(ElementType::Type element_type, ChangesetType changeset_type);
  /**
   * @brief size Total number of ElementType::Type elements (node/way/relation) of a specific changeset
   *  type (create/modify/delete) within this subset
   * @param elementType Describes the type (node/way/relation) to count
   * @param changesetType Describes the type (create/modify/delete) to count
   * @return count based on types
   */
  size_t size(ElementType::Type elementType, ChangesetType changesetType);
  /**
   * @brief size Total number of ElementType::Type elements (node/way/relation) of all changeset types within
   *  this subset
   * @param elementType Describes the type (node/way/relation) to count
   * @return count based on the type
   */
  size_t size(ElementType::Type elementType);
  /**
   * @brief size Total number of elements in the subset
   * @return total count
   */
  size_t size();
  /** Set/get _attemptedResolveChangesetIssues member */
  bool getAttemptedResolveChangesetIssues();
  void setAttemptedResolveChangesetIssues(bool attempted);
  /** Set/get _numRetries member */
  bool canRetryFailure();
  void retryFailure();
  /** Set/get _versionRetries member */
  bool canRetryVersion();
  void retryVersion();
  /** Set/get _last member for final error checking */
  void setLast() { _last = true; }
  bool getLast() { return _last; }
  /** Set/get _isError member */
  void setError() { _isError = true; }
  bool getError() { return _isError; }
  /** Append another changeset info object to this one */
  void append(const std::shared_ptr<ChangesetInfo>& info);

private:
  /** 3x3 array of containers for elements in this subset */
  std::array<std::array<container, ChangesetType::TypeMax>, ElementType::Unknown> _changeset;
  /** Flag set after attempt to resolve changeset issues has completed. */
  bool _attemptedResolveChangesetIssues;
  /** Number of times this exact changeset has been retried from failures unsuccessfully */
  int _numFailureRetries;
  const int MAX_FAILURE_RETRIES = 5;
  /** Number of times this exact changeset has had version errors */
  int _numVersionRetries;
  const int MAX_VERSION_RETRIES = 25;
  /** Flag set when this is the last changeset because of error */
  bool _last;
  /** When `true` this entire changeset consists of elements that cannot be pushed without an error.
   *  For example: A way cannot be added because it references a node that doesn't exists, this changeset
   *  would contain the new way and any new nodes that shouldn't be added by themselves.
   */
  bool _isError;
};

}

#endif  //  OSM_API_CHANGESET_H
