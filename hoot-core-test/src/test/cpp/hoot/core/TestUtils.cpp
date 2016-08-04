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
 * @copyright Copyright (C) 2012, 2013, 2014, 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "TestUtils.h"

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/conflate/MatchFactory.h>
#include <hoot/core/conflate/MergerFactory.h>
#include <hoot/core/filters/TagCriterion.h>
#include <hoot/core/schema/TagMergerFactory.h>
#include <hoot/core/scoring/MapComparator.h>
#include <hoot/core/io/OsmReader.h>
#include <hoot/core/schema/TagMergerFactory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/UuidHelper.h>
#include <hoot/core/visitors/FilteredVisitor.h>
#include <hoot/core/visitors/GetElementIdsVisitor.h>

// Qt
#include <QFile>

namespace hoot
{

shared_ptr<TestUtils> TestUtils::_theInstance;

TestUtils::TestUtils()
{
}

bool TestUtils::compareMaps(const QString& refPath, const QString testPath)
{
  OsmReader reader;
  reader.setDefaultStatus(Status::Unknown1);
  reader.setUseDataSourceIds(true);
  reader.setUseStatusFromFile(true);

  shared_ptr<OsmMap> ref(new OsmMap());
  shared_ptr<OsmMap> test(new OsmMap());
  reader.read(refPath, ref);
  reader.read(testPath, test);
  return compareMaps(ref, test);
}

bool TestUtils::compareMaps(shared_ptr<OsmMap> ref, shared_ptr<OsmMap> test)
{
  return MapComparator().isMatch(ref, test);
}

NodePtr TestUtils::createNode(OsmMapPtr map, Status status, double x, double y,
                              double circularError, Tags tags)
{
  NodePtr result(new Node(status, map->createNextNodeId(), x, y, circularError));
  map->addNode(result);
  result->getTags().addTags(tags);
  return result;
}

WayPtr TestUtils::createWay(OsmMapPtr map, Status s, Coordinate c[], Meters ce, const QString& note)
{
  WayPtr result(new Way(s, map->createNextWayId(), ce));
  for (size_t i = 0; c[i].isNull() == false; i++)
  {
    NodePtr n(new Node(s, map->createNextNodeId(), c[i], ce));
    map->addNode(n);
    result->addNode(n->getId());
  }

  if (!note.isEmpty())
  {
    result->getTags().addNote(note);
  }
  map->addWay(result);
  return result;
}

WayPtr TestUtils::createWay(OsmMapPtr map, const QList<NodePtr>& nodes, Status status,
                            Meters circularError, Tags tags)
{
  WayPtr way(new Way(status, map->createNextWayId(), circularError));
  foreach (NodePtr node, nodes)
  {
    map->addNode(node);
    way->addNode(node->getId());
  }
  way->setTags(tags);
  map->addWay(way);
  return way;
}

RelationPtr TestUtils::createRelation(OsmMapPtr map, const QList<ElementPtr>& elements,
  Status status, Meters circularError)
{
  RelationPtr relation(new Relation(status, map->createNextRelationId(), circularError));
  foreach (ElementPtr element, elements)
  {
    map->addElement(element);
    relation->addElement("test", element);
  }
  map->addRelation(relation);
  return relation;
}

void TestUtils::dumpString(const string& str)
{
  cout << "const unsigned char data[] = {";
  for (size_t i = 0; i < str.size(); i++)
  {
    if (i != 0)
    {
      cout << ", ";
    }
    if (i % 18 == 0)
    {
      cout << endl << "  ";
    }
    //printf("%3d%c", (unsigned char)str.at(i),
    //  str.at(i) >= 32 && str.at(i) <= 125 ? (char)(str.at(i)) : '#');
    printf("%3d", (unsigned char)str.at(i));
  }
  cout << "};" << endl;
  cout << "size_t dataSize = " << str.size() << ";" << endl;
}

ElementPtr TestUtils::getElementWithNote(OsmMapPtr map, QString note)
{
  return getElementWithTag(map, "note", note);
}

ElementPtr TestUtils::getElementWithTag(OsmMapPtr map, const QString tagKey,
                                        const QString tagValue)
{
  TagCriterion tc(tagKey, tagValue);
  set<ElementId> bag;
  GetElementIdsVisitor v(bag);
  FilteredVisitor fv(tc, v);
  map->visitRo(fv);

  if (bag.size() != 1)
  {
    throw HootException("Could not find an expected element with tag: " + tagKey + "=" + tagValue);
  }

  return map->getElement(*bag.begin());
}

shared_ptr<TestUtils> TestUtils::getInstance()
{
  if (!_theInstance)
  {
    _theInstance.reset(new TestUtils());
  }

  return _theInstance;
}

std::string TestUtils::readFile(QString f1)
{
  QFile fpTest(f1);
  fpTest.open(QIODevice::ReadOnly);
  return QString(fpTest.readAll()).toStdString();
}

void TestUtils::resetEnvironment()
{
  // provide the most basic configuration.
  OsmMap::resetCounters();
  conf().clear();
  ConfigOptions::populateDefaults(conf());
  conf().set("HOOT_HOME", getenv("HOOT_HOME"));

  // Sometimes we add new projections to the MapProjector, when this happens it may pick a new
  // projection and subtly change the results.
  conf().set(ConfigOptions::getTestForceOrthographicProjectionKey(), true);

  // these factories cache the creators. Flush them so they get any config changes.
  MatchFactory::getInstance().reset();
  MergerFactory::getInstance().reset();
  TagMergerFactory::getInstance().reset();

  // make sure the UUIDs are repeatable
  UuidHelper::resetRepeatableKey();

  foreach (RegisteredReset* rr, getInstance()->_resets)
  {
    rr->reset();
  }
}

QString TestUtils::toQuotedString(QString str)
{
  QString result;
  str.replace("\"", "\\\"");
  QStringList l = str.split("\n");
  for (int i = 0; i < l.size(); ++i)
  {
    if (i != l.size() - 1)
    {
      result += "\"" + l[i] + "\\n\"\n";
    }
    else
    {
      result += "\"" + l[i] + "\"\n";
    }
  }
  return result;
}

}
