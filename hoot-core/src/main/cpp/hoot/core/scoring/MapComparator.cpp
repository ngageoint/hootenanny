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
#include "MapComparator.h"

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/visitors/ElementConstOsmMapVisitor.h>
#include <hoot/core/visitors/CountUniqueReviewsVisitor.h>
#include <hoot/core/util/ConfigOptions.h>

// Standard
#include <iomanip>

#define CHECK(con) \
  if (!(con)) \
  { \
    _matches = false; \
    _errorCount++; \
    if (_errorCount <= _errorLimit) \
      LOG_WARN("Check failed."); \
    if (_errorCount == _errorLimit) \
      LOG_WARN("More than " << _errorLimit << " errors, suppressing errors."); \
    return; \
  }

#define CHECK_MSG(con, msg) \
  if (!(con)) \
  { \
    _matches = false; \
    _errorCount++; \
    if (_errorCount <= _errorLimit) \
      LOG_WARN(msg); \
    if (_errorCount == _errorLimit) \
      LOG_WARN("More than " << _errorLimit << " errors, suppressing errors."); \
    return; \
  }

#define CHECK_DOUBLE(v1, v2, t) \
  if (fabs((v1) - (v2)) > t) \
  { \
    _matches = false; \
    _errorCount++; \
    if (_errorCount <= _errorLimit) \
      LOG_WARN("Check Double failed. " << v1 << " vs. " << v2); \
    if (_errorCount == _errorLimit) \
      LOG_WARN("More than " << _errorLimit << " errors, suppressing errors."); \
    return; \
  }

namespace hoot
{

class CompareVisitor : public ElementConstOsmMapVisitor
{
public:

  /**
   * Defaults to 5cm threshold
   */
  CompareVisitor(
    std::shared_ptr<OsmMap> refMap, bool ignoreUUID, bool useDateTime, int errorLimit = 10,
    Meters threshold = 0.05)
    : _refMap(refMap),
      _threshold(threshold),
      _matches(true),
      _ignoreUUID(ignoreUUID),
      _useDateTime(useDateTime),
      _errorCount(0),
      _errorLimit(errorLimit)
  {
  }

  bool isMatch() { return _matches; }

  virtual QString getDescription() const { return ""; }
  virtual std::string getClassName() const { return ""; }

  virtual void visit(const std::shared_ptr<const Element>& e)
  {
    // e is the test element

    CHECK_MSG(_refMap->containsElement(e->getElementId()), "Did not find element: " <<
              e->getElementId());
    const std::shared_ptr<const Element>& refElement = _refMap->getElement(e->getElementId());

    Tags refTags = refElement->getTags();
    Tags testTags = e->getTags();

    if (_ignoreUUID)
    {
      refTags.set("uuid", "None");  // Wipe out the UUID's
      testTags.set("uuid", "None");
    }

    // By default, hoot will usually set these tags when ingesting a file
    // this can cause problems when comparing files during testing, so we
    // have the option to ignore it here.
    if (!_useDateTime)
    {
      refTags.set(MetadataTags::SourceIngestDateTime(), "None");  // Wipe out the ingest datetime
      testTags.set(MetadataTags::SourceIngestDateTime(), "None");

      refTags.set(MetadataTags::SourceDateTime(), "None");  // Wipe out the ingest datetime
      testTags.set(MetadataTags::SourceDateTime(), "None");
    }

    if (refTags != testTags)
    {
      _matches = false;
      _errorCount++;
      if (_errorCount <= 10)
      {
        LOG_WARN(
          "Tags do not match (ref: " << refElement->getElementId() << ", test: " <<
          e->getElementId() << ":");

        QStringList keys = refTags.keys();
        keys.append(testTags.keys());
        keys.removeDuplicates();
        keys.sort();

        if (_errorCount < 10)
        {
          for (int i = 0; i < keys.size(); i++)
          {
            QString k = keys[i];
            if (refTags[k] != testTags[k])
            {
              LOG_WARN("< " + k + " = " + refTags[k]);
              LOG_WARN("> " + k + " = " + testTags[k]);
            }
          }
        }
      }
      return;
    }
    //CHECK_MSG(refTags == testTags, "Tags do not match: " << refTags.toString() << " vs. " << testTags.toString());

    CHECK_DOUBLE(refElement->getCircularError(), e->getCircularError(), _threshold);
    CHECK_MSG(refElement->getStatus() == e->getStatus(),
          "Status does not match: " << refElement->getStatusString() << " vs. " <<
          e->getStatusString());
    switch (e->getElementType().getEnum())
    {
    case ElementType::Unknown:
      _matches = false;
      LOG_WARN("Encountered an unexpected element type.");
      break;
    case ElementType::Node:
      compareNode(refElement, e);
      break;
    case ElementType::Way:
      compareWay(refElement, e);
      break;
    case ElementType::Relation:
      compareRelation(refElement, e);
      break;
    }
  }

  void compareNode(const std::shared_ptr<const Element>& refElement,
                   const std::shared_ptr<const Element>& testElement)
  {
    ConstNodePtr refNode = std::dynamic_pointer_cast<const Node>(refElement);
    ConstNodePtr testNode = std::dynamic_pointer_cast<const Node>(testElement);

    if (GeometryUtils::haversine(refNode->toCoordinate(), testNode->toCoordinate()) > _threshold)
    {
      if (_errorCount <= 10)
      {
        LOG_WARN(
          "refNode: " << std::fixed << std::setprecision(15) << refNode->getX() << ", " <<
          refNode->getY() << " testNode: " << testNode->getX() << ", " << testNode->getY());
      }
      _matches = false;
      _errorCount++;
    }
  }

  void compareWay(const std::shared_ptr<const Element>& refElement,
                  const std::shared_ptr<const Element>& testElement)
  {
    ConstWayPtr refWay = std::dynamic_pointer_cast<const Way>(refElement);
    ConstWayPtr testWay = std::dynamic_pointer_cast<const Way>(testElement);

    CHECK_MSG(
      refWay->getNodeIds().size() == testWay->getNodeIds().size(),
      "Node count does not match. " << refWay->getElementId() << ": " <<
      refWay->getNodeIds().size() << ", " << testWay->getElementId() << ": " <<
      testWay->getNodeIds().size());
    for (size_t i = 0; i < refWay->getNodeIds().size(); ++i)
    {
      CHECK_MSG(refWay->getNodeIds()[i] == testWay->getNodeIds()[i],
        QString("Node IDs don't match. (%1 vs. %2)").
        arg(hoot::toString(refWay)).
        arg(hoot::toString(testWay)));
    }
  }

  void compareRelation(const std::shared_ptr<const Element>& refElement,
                       const std::shared_ptr<const Element>& testElement)
  {
    ConstRelationPtr refRelation = std::dynamic_pointer_cast<const Relation>(refElement);
    ConstRelationPtr testRelation = std::dynamic_pointer_cast<const Relation>(testElement);

    QString relationStr =
      QString("%1 vs. %2").arg(hoot::toString(refRelation)).arg(hoot::toString(testRelation));

    CHECK_MSG(
      refRelation->getType() == testRelation->getType(), "Types do not match. " + relationStr);
    CHECK_MSG(refRelation->getMembers().size() == testRelation->getMembers().size(),
      "Member count does not match. " + relationStr);
    for (size_t i = 0; i < refRelation->getMembers().size(); i++)
    {
      CHECK_MSG(
        refRelation->getMembers()[i].role == testRelation->getMembers()[i].role,
        "Member role does not match. " + relationStr);
      CHECK_MSG(
        refRelation->getMembers()[i].getElementId() == testRelation->getMembers()[i].getElementId(),
        "Member element ID does not match. " + relationStr);
    }
  }

private:

  std::shared_ptr<OsmMap> _refMap;
  Meters _threshold;
  bool _matches;
  bool _ignoreUUID;
  bool _useDateTime;
  int _errorCount;
  int _errorLimit;
};

MapComparator::MapComparator():
  _ignoreUUID(false),
  _useDateTime(false),
  _errorLimit(10)
{
}

void MapComparator::_printIdDiff(
  const std::shared_ptr<OsmMap>& map1, const std::shared_ptr<OsmMap>& map2,
  const ElementType& elementType, const int limit)
{
  QSet<long> ids1;
  QSet<long> ids2;
  LOG_VARD(limit);

  switch (elementType.getEnum())
  {
  case ElementType::Node:
    ids1 = map1->getNodeIds();
    ids2 = map2->getNodeIds();
    break;
  case ElementType::Way:
    ids1 = map1->getWayIds();
    ids2 = map2->getWayIds();
    break;
  case ElementType::Relation:
    ids1 = map1->getRelationIds();
    ids2 = map2->getRelationIds();
    break;
  default:
    throw HootException(QString("Unexpected element type: %1").arg(elementType.toString()));
  }

  QSet<long> ids1Copy = ids1;
  const QSet<long> idsIn1AndNotIn2 = ids1Copy.subtract(ids2);
  QSet<long> idsIn1AndNotIn2Limited;
  if (limit < idsIn1AndNotIn2.size())
  {
     int ctr = 0;
     for (QSet<long>::const_iterator it = idsIn1AndNotIn2.begin(); it != idsIn1AndNotIn2.end(); ++it)
     {
       idsIn1AndNotIn2Limited.insert(*it);
       ctr++;

       if (ctr == limit)
       {
         break;
       }
     }
  }
  else
  {
    idsIn1AndNotIn2Limited = idsIn1AndNotIn2;
  }

  QSet<long> ids2Copy = ids2;
  const QSet<long> idsIn2AndNotIn1 = ids2Copy.subtract(ids1);
  QSet<long> idsIn2AndNotIn1Limited;
  if (limit < idsIn2AndNotIn1.size())
  {
     int ctr = 0;
     for (QSet<long>::const_iterator it = idsIn2AndNotIn1.begin(); it != idsIn2AndNotIn1.end(); ++it)
     {
       idsIn2AndNotIn1Limited.insert(*it);
       ctr++;

       if (ctr == limit)
       {
         break;
       }
     }
  }
  else
  {
    idsIn2AndNotIn1Limited = idsIn2AndNotIn1;
  }

  //map.comparator.print.full.mismatch.elements.on.map.size.diff
  const bool printFullElements =
    ConfigOptions().getMapComparatorPrintFullMismatchElementsOnMapSizeDiff();
  if (idsIn1AndNotIn2Limited.size() > 0)
  {
    LOG_WARN(
      "\t" << elementType.toString() << "s in map 1 and not in map 2 (limit " << limit << "): " <<
      idsIn1AndNotIn2Limited);
    if (printFullElements)
    {
      for (QSet<long>::const_iterator it = idsIn1AndNotIn2Limited.begin();
           it != idsIn1AndNotIn2Limited.end(); ++it)
      {
        LOG_WARN(map1->getElement(ElementId(elementType, *it)));
      }
    }
  }
  if (idsIn2AndNotIn1Limited.size() > 0)
  {
    LOG_WARN(
      "\t" << elementType.toString() << "s in map 2 and not in map 1 (limit " << limit << "): " <<
      idsIn2AndNotIn1Limited);
    if (printFullElements)
    {
      for (QSet<long>::const_iterator it = idsIn2AndNotIn1Limited.begin();
           it != idsIn2AndNotIn1Limited.end(); ++it)
      {
        LOG_WARN(map2->getElement(ElementId(elementType, *it)));
      }
    }
  }
}

bool MapComparator::isMatch(const std::shared_ptr<OsmMap>& refMap,
                            const std::shared_ptr<OsmMap>& testMap)
{
  bool mismatch = false;
  if (refMap->getNodes().size() != testMap->getNodes().size())
  {
    LOG_WARN(
      "Number of nodes does not match (1: " << refMap->getNodes().size() << "; 2: " <<
      testMap->getNodes().size() << ")");
    // Yes, the two map could have the same number of the same type of elements and they still
    // might not completely match up, but we'll let CompareVisitor educate us on that. This gives
    // us a quick rundown of element ID diffs if count discrepancy is detected.
    _printIdDiff(refMap, testMap, ElementType::Node);
    mismatch = true;
  }
  else if (refMap->getWays().size() != testMap->getWays().size())
  {
    LOG_WARN(
      "Number of ways does not match (1: " << refMap->getWays().size() << "; 2: " <<
      testMap->getWays().size() << ")");
    _printIdDiff(refMap, testMap, ElementType::Way);
    mismatch = true;
  }
  else if (refMap->getRelations().size() != testMap->getRelations().size())
  {
    LOG_WARN(
      "Number of relations does not match (1: " << refMap->getRelations().size() << "; 2: " <<
      testMap->getRelations().size() << ")");
    _printIdDiff(refMap, testMap, ElementType::Relation);
    mismatch = true;
  }

  CountUniqueReviewsVisitor countReviewsVis;
  refMap->visitRo(countReviewsVis);
  const int refReviews = (int)countReviewsVis.getStat();
  countReviewsVis.clear();
  testMap->visitRo(countReviewsVis);
  const int testReviews = (int)countReviewsVis.getStat();
  if (refReviews != testReviews)
  {
    LOG_WARN(
      "Number of reviews does not match (1: " << refReviews << "; 2: " << testReviews << ")");
    mismatch = true;
  }

  if (mismatch)
  {
    return false;
  }

  CompareVisitor compareVis(refMap, _ignoreUUID, _useDateTime, _errorLimit);
  testMap->visitRo(compareVis);
  return compareVis.isMatch();
}

}
