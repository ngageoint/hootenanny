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
 * @copyright Copyright (C) 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef OSM_CHANGESET_ELEMENT_H
#define OSM_CHANGESET_ELEMENT_H

//  Hoot
#include <hoot/core/elements/ElementType.h>

//  Qt
#include <QList>
#include <QMap>
#include <QPair>
#include <QString>
#include <QVector>
#include <QXmlStreamReader>

//  Boost
#include <boost/shared_ptr.hpp>

//  Standard
#include <unordered_set>

namespace hoot
{

struct osm_id_sort
{
  bool operator() (long lhs, long rhs) const
  {
    if (lhs > 0 && rhs > 0)       return lhs < rhs; //  Positive numbers count up
    else if (lhs < 0 && rhs < 0)  return lhs > rhs; //  Negative numbers count down
    else                          return lhs < rhs; //  Negative numbers come before positive
  }
};

class XmlElement;
class XmlNode;
class XmlWay;
class XmlRelation;
class XmlMember;

typedef boost::shared_ptr<XmlElement> XmlElementPtr;
typedef std::map<long, XmlElementPtr, osm_id_sort> XmlElementMap;
typedef QVector<XmlElementMap> ChangesetTypeMap;
typedef QPair<QString, QXmlStreamAttributes> XmlObject;
typedef QVector<std::map<long, long, osm_id_sort> > XmlElementIdMap;

class ElementIdToIdMap
{
public:
  typedef typename std::map<long, long, osm_id_sort>::iterator iterator;
  typedef typename std::map<long, long, osm_id_sort>::const_iterator const_iterator;

  ElementIdToIdMap()
    : _oldIdToNew(ElementType::Unknown),
      _newIdToOld(ElementType::Unknown)
  {}

  void addId(ElementType::Type type, long id)
  {
    _oldIdToNew[type][id] = id;
    _newIdToOld[type][id] = id;
  }
  void updateId(ElementType::Type type, long old_id, long new_id)
  {
    _oldIdToNew[type][old_id] = new_id;
    _newIdToOld[type][new_id] = _newIdToOld[type][old_id];
    _newIdToOld[type].erase(old_id);
  }

  long getOldId(ElementType::Type type, long new_id)
  {
    if (_newIdToOld[type].find(new_id) == _newIdToOld[type].end())
      return new_id;
    else
      return _newIdToOld[type][new_id];
  }
  long getNewId(ElementType::Type type, long old_id)
  {
    if (_oldIdToNew[type].find(old_id) == _oldIdToNew[type].end())
      return old_id;
    else
      return _oldIdToNew[type][old_id];
  }

  iterator begin(ElementType::Type type)  { return _newIdToOld[type].begin(); }
  iterator end(ElementType::Type type)    { return _newIdToOld[type].end(); }

private:
  XmlElementIdMap _oldIdToNew;
  XmlElementIdMap _newIdToOld;
};

class ChangesetInfo;
typedef boost::shared_ptr<ChangesetInfo> ChangesetInfoPtr;

class XmlChangeset
{
public:
  XmlChangeset();
  XmlChangeset(const QList<QString>& changesets);

  void loadChangeset(const QString& changesetPath);

  void updateChangeset(const QString& changes);

  bool hasElementsToSend() { return (long)(_allNodes.size() + _allWays.size() + _allRelations.size()) > _sentCount; }
  bool isDone() { return (long)(_allNodes.size() + _allWays.size() + _allRelations.size()) == _processedCount; }

  enum ChangesetType
  {
    TypeCreate = 0,
    TypeModify,
    TypeDelete,
    TypeMax
  };

  bool calculateChangeset(ChangesetInfoPtr& changeset);

  ChangesetInfoPtr splitChangeset(ChangesetInfoPtr changeset);

  QString getChangesetString(ChangesetInfoPtr changeset, long changeset_id);

  void setMaxSize(long size) { _maxChangesetSize = size; }

private:

  void updateElement(ChangesetTypeMap& map, long old_id, long new_id, long version);

  void loadElements(QXmlStreamReader& reader, ChangesetType type);

  QString getChangeset(ChangesetInfoPtr changeset, long id, ChangesetType type);

  bool addNode(ChangesetInfoPtr& changeset, ChangesetType type, XmlNode* node);
  bool addNodes(ChangesetInfoPtr& changeset, ChangesetType type);
  bool addWay(ChangesetInfoPtr& changeset, ChangesetType type, XmlWay* way);
  bool addWays(ChangesetInfoPtr& changeset, ChangesetType type);
  bool addRelation(ChangesetInfoPtr& changeset, ChangesetType type, XmlRelation* relation);
  bool addRelations(ChangesetInfoPtr& changeset, ChangesetType type);

  bool isSent(XmlElement* element);
  bool canSend(XmlNode* node);
  bool canSend(XmlWay* way);
  bool canSend(XmlRelation* relation);

  void markBuffered(XmlElement* element);

  XmlElementMap _allNodes;
  XmlElementMap _allWays;
  XmlElementMap _allRelations;

  ChangesetTypeMap _nodes;
  ChangesetTypeMap _ways;
  ChangesetTypeMap _relations;

  ElementIdToIdMap _idMap;

  std::map<long, std::vector<long>> _nodesToWays;
  std::array<std::map<long, std::vector<long>>, ElementType::Unknown> _idsToRelations;

  long _maxChangesetSize;
  long _sentCount;
  long _processedCount;

  std::vector<XmlElement*> _sendBuffer;
};

class ChangesetInfo
{
public:
  typedef std::unordered_set<long> container;
  typedef typename container::iterator iterator;
  typedef typename container::const_iterator const_iterator;

  ChangesetInfo() { }
  void add(ElementType::Type element_type, XmlChangeset::ChangesetType changeset_type, long id)
  { _changeset[element_type][changeset_type].insert(id); }
  void remove(ElementType::Type element_type, XmlChangeset::ChangesetType changeset_type, long id)
  {
    container& selectedSet = _changeset[element_type][changeset_type];
    if (selectedSet.find(id) != selectedSet.end())
      selectedSet.erase(id);
  }

  void clear()
  {
    for (int i = 0; i < (int)ElementType::Unknown; ++i)
    {
      for (int j = 0; j < (int)XmlChangeset::TypeMax; ++j)
        _changeset[i][j].clear();
    }
  }
  bool contains(ElementType::Type element_type, XmlChangeset::ChangesetType changeset_type, long id)
  { return _changeset[element_type][changeset_type].find(id) != end(element_type, changeset_type); }

  iterator begin(ElementType::Type element_type, XmlChangeset::ChangesetType changeset_type)
  { return _changeset[element_type][changeset_type].begin(); }
  iterator end(ElementType::Type element_type, XmlChangeset::ChangesetType changeset_type)
  { return _changeset[element_type][changeset_type].end(); }

  size_t size()
  {
    size_t s = 0;
    for (int i = 0; i < (int)ElementType::Unknown; ++i)
    {
      for (int j = 0; j < (int)XmlChangeset::TypeMax; ++j)
        s += _changeset[i][j].size();
    }
    return s;
  }

  ChangesetInfoPtr splitChangeset();

private:
  std::array<std::array<container, XmlChangeset::TypeMax>, ElementType::Unknown> _changeset;
};

class XmlElement
{
public:
  XmlElement(const XmlObject& object, ElementIdToIdMap* idMap);
  void addTag(const XmlObject& tag);

  long id() { return _id; }
  ElementType::Type getType() { return _type; }
  virtual QString toString(long changesetId) const = 0;

  long getVersion() { return _version; }
  void setVersion(long version) { _version = version; }

  enum ElementStatus
  {
    Available,
    Buffering,
    Sent,
    Finalized
  };

  ElementStatus getStatus() { return _status; }
  void setStatus(ElementStatus status);

protected:

  QString toString(const QXmlStreamAttributes& attributes, long changesetId) const;
  QString toTagString(const QXmlStreamAttributes& attributes) const;

  QString& escapeString(QString& value) const;

  ElementType::Type _type;
  long _id;
  long _version;
  XmlObject _object;
  QVector<XmlObject> _tags;

  ElementIdToIdMap* _idMap;

  ElementStatus _status;
};

class XmlNode : public XmlElement
{
public:
  XmlNode(const XmlObject& node, ElementIdToIdMap* idMap);

  virtual QString toString(long changesetId) const;
};

class XmlWay : public XmlElement
{
public:
  XmlWay(const XmlObject& way, ElementIdToIdMap* idMap);

  void addNode(long id) { _nodes.push_back(id); }
  long getNode(int index) { return _nodes[index]; }
  int getNodeCount() { return _nodes.size(); }

  virtual QString toString(long changesetId) const;

private:

  QList<long> _nodes;
};

class XmlRelation : public XmlElement
{
public:
  XmlRelation(const XmlObject& relation, ElementIdToIdMap* idMap);
  void addMember(const QXmlStreamAttributes& member);
  XmlMember& getMember(int index);
  int getMemberCount();

  virtual QString toString(long changesetId) const;

private:

  QList<XmlMember> _members;
};

class XmlMember
{
public:
  XmlMember(const QXmlStreamAttributes& member, ElementIdToIdMap* idMap);

  bool isNode()     { return _type == "node"; }
  bool isWay()      { return _type == "way"; }
  bool isRelation() { return _type == "relation"; }

  QString getType() { return _type; }
  long getRef()     { return _ref; }
  QString getRole() { return _role; }

  QString toString() const;

private:
  QString _type;
  long _ref;
  QString _role;
  ElementIdToIdMap* _idMap;
};

}

#endif  //  OSM_CHANGESET_ELEMENT_H
