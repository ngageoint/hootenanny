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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "MapComparator.h"

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/visitors/ElementConstOsmMapVisitor.h>

// Standard
#include <iomanip>

#define CHECK(con) \
  if (!(con)) \
  { \
    _matches = false; \
    _errorCount++; \
    if (_errorCount <= 10) \
      LOG_WARN("Check failed."); \
    if (_errorCount == 10) \
      LOG_WARN("More than 10 errors, suppressing errors."); \
    return; \
  }

#define CHECK_MSG(con, msg) \
  if (!(con)) \
  { \
    _matches = false; \
    _errorCount++; \
    if (_errorCount <= 10) \
      LOG_WARN(msg); \
    if (_errorCount == 10) \
      LOG_WARN("More than 10 errors, suppressing errors."); \
    return; \
  }

#define CHECK_DOUBLE(v1, v2, t) \
  if (fabs((v1) - (v2)) > t) \
  { \
    _matches = false; \
    _errorCount++; \
    if (_errorCount <= 10) \
      LOG_WARN("Check Double failed. " << v1 << " vs. " << v2); \
    if (_errorCount == 10) \
      LOG_WARN("More than 10 errors, suppressing errors."); \
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
    std::shared_ptr<OsmMap> ref, bool ignoreUUID, bool useDateTime, Meters threshold = 0.05)
  {
    _ref = ref;
    _threshold = threshold;
    _matches = true;
    _errorCount = 0;
    _ignoreUUID = ignoreUUID;
    _useDateTime = useDateTime;
  }

  bool isMatch() { return _matches; }

  virtual QString getDescription() const { return ""; }

  virtual void visit(const std::shared_ptr<const Element>& e)
  {
    CHECK_MSG(_ref->containsElement(e->getElementId()), "Did not find element: " <<
              e->getElementId());
    const std::shared_ptr<const Element>& re = _ref->getElement(e->getElementId());

    Tags in1 = re->getTags();
    Tags in2 = e->getTags();

    if (_ignoreUUID)
    {
      in1.set("uuid", "None");  // Wipe out the UUID's
      in2.set("uuid", "None");
    }

    // By default, hoot will usually set these tags when ingesting a file
    // this can cause problems when comparing files during testing, so we
    // have the option to ignore it here.
    if (!_useDateTime)
    {
      in1.set(MetadataTags::SourceIngestDateTime(), "None");  // Wipe out the ingest datetime
      in2.set(MetadataTags::SourceIngestDateTime(), "None");

      in1.set(MetadataTags::SourceDateTime(), "None");  // Wipe out the ingest datetime
      in2.set(MetadataTags::SourceDateTime(), "None");
    }

    if (in1 != in2)
    {
      _matches = false;
      _errorCount++;
      if (_errorCount <= 10)
      {
        LOG_WARN("Tags do not match:");
        QStringList keys = in1.keys();
        keys.append(in2.keys());
        keys.removeDuplicates();
        keys.sort();

        if (_errorCount < 10)
        {
          for (int i = 0; i < keys.size(); i++)
          {
            QString k = keys[i];
            if (in1[k] != in2[k])
            {
              LOG_WARN("< " + k + " = " + in1[k]);
              LOG_WARN("> " + k + " = " + in2[k]);
            }
          }
        }
      }
      return;
    }
    //CHECK_MSG(in1 == in2, "Tags do not match: " << in1.toString() << " vs. " << in2.toString());

    CHECK_DOUBLE(re->getCircularError(), e->getCircularError(), _threshold);
    CHECK_MSG(re->getStatus() == e->getStatus(),
          "Status does not match: " << re->getStatusString() << " vs. " << e->getStatusString());
    switch(e->getElementType().getEnum())
    {
    case ElementType::Unknown:
      _matches = false;
      LOG_WARN("Encountered an unexpected element type.");
      break;
    case ElementType::Node:
      compareNode(re, e);
      break;
    case ElementType::Way:
      compareWay(re, e);
      break;
    case ElementType::Relation:
      compareRelation(re, e);
      break;
    }
  }

  void compareNode(const std::shared_ptr<const Element>& re, const std::shared_ptr<const Element>& e)
  {
    ConstNodePtr rn = std::dynamic_pointer_cast<const Node>(re);
    ConstNodePtr n = std::dynamic_pointer_cast<const Node>(e);

    if (GeometryUtils::haversine(rn->toCoordinate(), n->toCoordinate()) > _threshold)
    {
      if (_errorCount <= 10)
      {
        LOG_WARN("rn: " << std::fixed << std::setprecision(15) << rn->getX() << ", " << rn->getY() <<
                 " n: " << n->getX() << ", " << n->getY());
      }
      _matches = false;
      _errorCount++;
    }
  }

  void compareWay(const std::shared_ptr<const Element>& re, const std::shared_ptr<const Element>& e)
  {
    ConstWayPtr rw = std::dynamic_pointer_cast<const Way>(re);
    ConstWayPtr w = std::dynamic_pointer_cast<const Way>(e);

    CHECK_MSG(rw->getNodeIds().size() == w->getNodeIds().size(),
              "Node count does not match.");
    for (size_t i = 0; i < rw->getNodeIds().size(); ++i)
    {
      CHECK_MSG(rw->getNodeIds()[i] == w->getNodeIds()[i],
        QString("Node IDs don't match. (%1 vs. %2)").
        arg(hoot::toString(rw)).
        arg(hoot::toString(w)));
    }
  }

  void compareRelation(const std::shared_ptr<const Element>& re, const std::shared_ptr<const Element>& e)
  {
    ConstRelationPtr rr = std::dynamic_pointer_cast<const Relation>(re);
    ConstRelationPtr r = std::dynamic_pointer_cast<const Relation>(e);

    QString relationStr = QString("%1 vs. %2").arg(hoot::toString(rr)).arg(hoot::toString(r));

    CHECK_MSG(rr->getType() == r->getType(), "Types do not match. " + relationStr);
    CHECK_MSG(rr->getMembers().size() == r->getMembers().size(),
      "Member count does not match. " + relationStr);
    for (size_t i = 0; i < rr->getMembers().size(); i++)
    {
      CHECK_MSG(rr->getMembers()[i].role == r->getMembers()[i].role,
        "Member role does not match. " + relationStr);
      CHECK_MSG(rr->getMembers()[i].getElementId() == r->getMembers()[i].getElementId(),
        "Member element ID does not match. " + relationStr);
    }
  }

private:
  std::shared_ptr<OsmMap> _ref;
  Meters _threshold;
  Degrees _thresholdDeg;
  bool _matches;
  bool _ignoreUUID;
  bool _useDateTime;
  int _errorCount;
};

MapComparator::MapComparator():
  _ignoreUUID(false),
  _useDateTime(false)
{
  // blank
}

bool MapComparator::isMatch(const std::shared_ptr<OsmMap>& ref, const std::shared_ptr<OsmMap>& test)
{
  bool mismatch = false;
  if (ref->getNodes().size() != test->getNodes().size())
  {
    LOG_WARN(
      "Number of nodes does not match (1: " << ref->getNodes().size() << "; 2: " <<
      test->getNodes().size() << ")");
    mismatch = true;
  }
  else if (ref->getWays().size() != test->getWays().size())
  {
    LOG_WARN(
      "Number of ways does not match (1: " << ref->getWays().size() << "; 2: " <<
      test->getWays().size() << ")");
    mismatch = true;
  }
  else if (ref->getRelations().size() != test->getRelations().size())
  {
    LOG_WARN(
      "Number of relations does not match (1: " << ref->getRelations().size() << "; 2: " <<
      test->getRelations().size() << ")");
    mismatch = true;
  }
  if (mismatch)
  {
    return false;
  }

  CompareVisitor v(ref, _ignoreUUID, _useDateTime);
  test->visitRo(v);
  bool r = v.isMatch();

  return r;
}

}
