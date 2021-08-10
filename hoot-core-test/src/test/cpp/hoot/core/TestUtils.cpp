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
 * @copyright Copyright (C) 2012, 2013, 2014, 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#include "TestUtils.h"

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/conflate/merging/MergerFactory.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/schema/TagMergerFactory.h>
#include <hoot/core/scoring/MapComparator.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/UuidHelper.h>
#include <hoot/core/visitors/FilteredVisitor.h>
#include <hoot/core/conflate/SuperfluousConflateOpRemover.h>
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/ops/BuildingOutlineRemoveOp.h>
#include <hoot/core/ops/RemoveRoundabouts.h>
#include <hoot/core/ops/MapCleaner.h>
#include <hoot/core/algorithms/splitter/HighwayCornerSplitter.h>
#include <hoot/core/ops/SuperfluousNodeRemover.h>
#include <hoot/core/ops/SmallHighwayMerger.h>
#include <hoot/core/ops/ReplaceRoundabouts.h>
#include <hoot/core/visitors/RemoveMissingElementsVisitor.h>
#include <hoot/core/visitors/RemoveInvalidReviewRelationsVisitor.h>
#include <hoot/core/ops/RemoveDuplicateReviewsOp.h>
#include <hoot/core/ops/WayJoinerOp.h>
#include <hoot/core/visitors/RemoveInvalidRelationVisitor.h>
#include <hoot/core/visitors/RemoveInvalidMultilineStringMembersVisitor.h>
#include <hoot/core/ops/SuperfluousWayRemover.h>
#include <hoot/core/visitors/RemoveDuplicateWayNodesVisitor.h>
#include <hoot/core/ops/RemoveEmptyRelationsOp.h>
#include <hoot/core/visitors/ApiTagTruncateVisitor.h>
#include <hoot/core/ops/AddHilbertReviewSortOrderOp.h>
#include <hoot/core/ops/ReprojectToPlanarOp.h>
#include <hoot/core/ops/DuplicateNodeRemover.h>
#include <hoot/core/visitors/OneWayRoadStandardizer.h>
#include <hoot/core/ops/DuplicateWayRemover.h>
#include <hoot/core/algorithms/splitter/IntersectionSplitter.h>
#include <hoot/core/ops/UnlikelyIntersectionRemover.h>
#include <hoot/core/algorithms/splitter/DualHighwaySplitter.h>
#include <hoot/core/ops/HighwayImpliedDividedMarker.h>
#include <hoot/core/ops/DuplicateNameRemover.h>
#include <hoot/core/visitors/RemoveEmptyAreasVisitor.h>
#include <hoot/core/visitors/RemoveDuplicateRelationMembersVisitor.h>
#include <hoot/core/ops/RelationCircularRefRemover.h>
#include <hoot/core/ops/RemoveEmptyRelationsOp.h>
#include <hoot/core/visitors/RemoveDuplicateAreasVisitor.h>
#include <hoot/core/ops/NoInformationElementRemover.h>
#include <hoot/core/ops/BuildingOutlineUpdateOp.h>

//  tgs
#include <tgs/Statistics/Random.h>

// Qt
#include <QDir>
#include <QFile>

using namespace geos::geom;
using namespace std;

namespace hoot
{

std::shared_ptr<TestUtils> TestUtils::_theInstance;

const QString HootTestFixture::UNUSED_PATH = "";

TestUtils::TestUtils()
{
}

bool TestUtils::compareMaps(const QString& refPath, const QString& testPath)
{
  OsmXmlReader reader;
  reader.setDefaultStatus(Status::Unknown1);
  reader.setUseDataSourceIds(true);
  reader.setUseFileStatus(true);
  reader.setAddSourceDateTime(false);

  OsmMapPtr ref = std::make_shared<OsmMap>();
  OsmMapPtr test = std::make_shared<OsmMap>();
  reader.read(refPath, ref);
  reader.read(testPath, test);
  return compareMaps(ref, test);
}

bool TestUtils::compareMaps(OsmMapPtr ref, OsmMapPtr test)
{
  return MapComparator().isMatch(ref, test);
}

NodePtr TestUtils::createNode(
  const OsmMapPtr& map, const QString& note, const Status& status, const double x, const double y,
  const Meters circularError, const Tags& tags)
{
  NodePtr node = std::make_shared<Node>(status, map->createNextNodeId(), x, y, circularError);
  map->addNode(node);
  node->getTags().add(tags);
  if (!note.isEmpty())
  {
    node->getTags().addNote(note);
  }
  return node;
}

WayPtr TestUtils::createWay(
  const OsmMapPtr& map, const geos::geom::Coordinate c[], const QString& note, const Status& s,
  const Meters circularError, const Tags& tags)
{
  WayPtr way = std::make_shared<Way>(s, map->createNextWayId(), circularError);
  if (c != nullptr)
  {
    for (size_t i = 0; c[i].isNull() == false; i++)
    {
      NodePtr n = std::make_shared<Node>(s, map->createNextNodeId(), c[i], circularError);
      map->addNode(n);
      way->addNode(n->getId());
    }
  }
  way->getTags().add(tags);
  if (!note.isEmpty())
  {
    way->getTags().addNote(note);
  }
  map->addWay(way);
  return way;
}

WayPtr TestUtils::createWay(
  const OsmMapPtr& map, const QList<NodePtr>& nodes, const QString& note, const Status& status,
  const Meters circularError, const Tags& tags)
{
  WayPtr way = std::make_shared<Way>(status, map->createNextWayId(), circularError);
  foreach (NodePtr node, nodes)
  {
    map->addNode(node);
    way->addNode(node->getId());
  }
  way->setTags(tags);
  if (!note.isEmpty())
  {
    way->getTags().addNote(note);
  }
  map->addWay(way);
  return way;
}

WayPtr TestUtils::createWay(
  const OsmMapPtr& map, const QList<ElementId>& nodeIds, const QString& note, const Status& status,
  const Meters circularError, const Tags& tags)
{
  WayPtr way = std::make_shared<Way>(status, map->createNextWayId(), circularError);
  foreach (ElementId nodeId, nodeIds)
  {
    if (!map->containsNode(nodeId.getId()))
    {
      throw IllegalArgumentException(nodeId.toString() + " not present in test map.");
    }
    way->addNode(nodeId.getId());
  }
  way->setTags(tags);
  if (!note.isEmpty())
  {
    way->getTags().addNote(note);
  }
  map->addWay(way);
  return way;
}

RelationPtr TestUtils::createRelation(
  const OsmMapPtr& map, const QList<ElementPtr>& elements, const QString& note,
  const Status& status, const Meters circularError, const Tags& tags)
{
  RelationPtr relation =
    std::make_shared<Relation>(status, map->createNextRelationId(), circularError);
  foreach (ElementPtr element, elements)
  {
    map->addElement(element);
    relation->addElement("test", element);
  }
  relation->setTags(tags);
  if (!note.isEmpty())
  {
    relation->getTags().addNote(note);
  }
  map->addRelation(relation);
  return relation;
}

RelationPtr TestUtils::createRelation(
  const OsmMapPtr& map, const QList<ElementId>& elementIds, const QString& note,
  const Status& status, const Meters circularError, const Tags& tags)
{
  RelationPtr relation =
    std::make_shared<Relation>(status, map->createNextRelationId(), circularError);
  foreach (ElementId elementId, elementIds)
  {
    if (!map->containsElement(elementId))
    {
      throw IllegalArgumentException(elementId.toString() + " not present in test map.");
    }
    relation->addElement("test", elementId);
  }
  relation->setTags(tags);
  if (!note.isEmpty())
  {
    relation->getTags().addNote(note);
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
    printf("%3u", (unsigned char)str.at(i));
  }
  cout << "};" << endl;
  cout << "size_t dataSize = " << str.size() << ";" << endl;
}

TestUtils& TestUtils::getInstance()
{
  //  Local static singleton instance
  static TestUtils instance;
  return instance;
}

std::string TestUtils::readFile(QString f1)
{
  QFile fpTest(f1);
  fpTest.open(QIODevice::ReadOnly);
  return QString(fpTest.readAll()).toStdString();
}

void TestUtils::resetBasic()
{
  LOG_DEBUG("Resetting test environment...");

  // provide the most basic configuration.
  OsmMap::resetCounters();
  // make sure the UUIDs are repeatable
  UuidHelper::resetRepeatableKey();
  //  Reset the pseudo random number generator seed
  Tgs::Random::instance()->seed();
}

void TestUtils::resetEnvironment(const QStringList confs)
{
  LOG_DEBUG("Resetting test environment...");

  // provide the most basic configuration

  OsmMap::resetCounters();

  conf().clear();
  ConfigOptions::populateDefaults(conf());

  // We require that all tests use Testing.conf as a starting point and any conf values
  // specified by it may be overridden when necessary.
  conf().loadJson(ConfPath::search("Testing.conf"));

  // The primary reason for allowing custom configs to be loaded here is in certain situations to
  // prevent the ConfigOptions defaults from being loaded, as they may be too bulky when running
  // many hoot commands at once.
  LOG_VART(confs.size());
  for (int i = 0; i < confs.size(); i++)
  {
    LOG_VART(confs[i]);
    conf().loadJson(confs[i]);
  }
  conf().set("HOOT_HOME", getenv("HOOT_HOME"));

  // Sometimes we add new projections to the MapProjector, when this happens it may pick a new
  // projection and subtly change the results.
  conf().set(ConfigOptions::getTestForceOrthographicProjectionKey(), true);

  // These factories cache the creators. Flush them so they get any config changes. Note that we're
  // not resetting MatchFactory here. Its call to ScriptMatchCreator::setArguments is expensive and
  // not many tests require it, so tests must specifically decide to reset it via ResetAll.
  MergerFactory::getInstance().reset();
  TagMergerFactory::getInstance().reset();

  // make sure the UUIDs are repeatable
  UuidHelper::resetRepeatableKey();

  foreach (RegisteredReset* rr, getInstance()._resets)
  {
    rr->reset();
  }
  //  Reset the pseudo random number generator seed
  Tgs::Random::instance()->seed();

  // Reset the debug map count used to name output files
  OsmMapWriterFactory::resetDebugMapCount();
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

void TestUtils::verifyStdMatchesOutputIgnoreDate(const QString& stdFilePath,
                                                 const QString& outFilePath)
{
  LOG_VART(stdFilePath);
  LOG_VART(outFilePath);
  const QStringList stdTokens = FileUtils::tokenizeOutputFileWithoutDates(stdFilePath);
  const QStringList outputTokens = FileUtils::tokenizeOutputFileWithoutDates(outFilePath);
  CPPUNIT_ASSERT_EQUAL(stdTokens.size(), outputTokens.size());
  for (int i = 0; i < stdTokens.size(); i++)
  {
    HOOT_STR_EQUALS(stdTokens.at(i), outputTokens.at(i));
  }
}

QStringList TestUtils::getConflateCmdSnapshotPreOps()
{
  QStringList conflatePreOps;
  conflatePreOps.append(BuildingOutlineRemoveOp::className());
  conflatePreOps.append(RemoveRoundabouts::className());
  conflatePreOps.append(MapCleaner::className());
  conflatePreOps.append(HighwayCornerSplitter::className());
  return conflatePreOps;
}

QStringList TestUtils::getConflateCmdSnapshotPostOps()
{
  QStringList conflatePostOps;
  conflatePostOps.append(SuperfluousNodeRemover::className());
  conflatePostOps.append(SmallHighwayMerger::className());
  conflatePostOps.append(ReplaceRoundabouts::className());
  conflatePostOps.append(RemoveMissingElementsVisitor::className());
  conflatePostOps.append(RemoveInvalidReviewRelationsVisitor::className());
  conflatePostOps.append(RemoveDuplicateReviewsOp::className());
  conflatePostOps.append(BuildingOutlineUpdateOp::className());
  conflatePostOps.append(WayJoinerOp::className());
  conflatePostOps.append(RemoveInvalidRelationVisitor::className());
  conflatePostOps.append(RemoveInvalidMultilineStringMembersVisitor::className());
  conflatePostOps.append(SuperfluousWayRemover::className());
  conflatePostOps.append(RemoveDuplicateWayNodesVisitor::className());
  conflatePostOps.append(RemoveEmptyRelationsOp::className());
  conflatePostOps.append(ApiTagTruncateVisitor::className());
  conflatePostOps.append(AddHilbertReviewSortOrderOp::className());
  return conflatePostOps;
}

QStringList TestUtils::getConflateCmdSnapshotCleaningOps()
{
  QStringList mapCleanerTransforms;
  mapCleanerTransforms.append(ReprojectToPlanarOp::className());
  mapCleanerTransforms.append(DuplicateNodeRemover::className());
  mapCleanerTransforms.append(OneWayRoadStandardizer::className());
  mapCleanerTransforms.append(DuplicateWayRemover::className());
  mapCleanerTransforms.append(SuperfluousWayRemover::className());
  mapCleanerTransforms.append(IntersectionSplitter::className());
  mapCleanerTransforms.append(UnlikelyIntersectionRemover::className());
  mapCleanerTransforms.append(DualHighwaySplitter::className());
  mapCleanerTransforms.append(HighwayImpliedDividedMarker::className());
  mapCleanerTransforms.append(DuplicateNameRemover::className());
  mapCleanerTransforms.append(SmallHighwayMerger::className());
  mapCleanerTransforms.append(RemoveEmptyAreasVisitor::className());
  mapCleanerTransforms.append(RemoveDuplicateRelationMembersVisitor::className());
  mapCleanerTransforms.append(RelationCircularRefRemover::className());
  mapCleanerTransforms.append(RemoveEmptyRelationsOp::className());
  mapCleanerTransforms.append(RemoveDuplicateAreasVisitor::className());
  mapCleanerTransforms.append(NoInformationElementRemover::className());
  return mapCleanerTransforms;
}

void TestUtils::runConflateOpReductionTest(
  const QStringList& matchCreators, const int expectedPreOpSize, const int expectedPostOpsSize,
  const int expectedCleaningOpsSize)
{
  QStringList actualOps;

  CPPUNIT_ASSERT_EQUAL(4, TestUtils::getConflateCmdSnapshotPreOps().size());
  CPPUNIT_ASSERT_EQUAL(15, TestUtils::getConflateCmdSnapshotPostOps().size());
  CPPUNIT_ASSERT_EQUAL(17, TestUtils::getConflateCmdSnapshotCleaningOps().size());

  MatchFactory::getInstance().reset();
  MatchFactory::getInstance()._setMatchCreators(matchCreators);
  // This is a snapshot of the ops in order to avoid any changes made to them result in requiring
  // this test's results to change over time. Clearly, any newly added ops could be being filtered
  // incorrectly, and we can update this list periodically if that's deemed important.
  conf().set(ConfigOptions::getConflatePreOpsKey(), TestUtils::getConflateCmdSnapshotPreOps());
  conf().set(ConfigOptions::getConflatePostOpsKey(), TestUtils::getConflateCmdSnapshotPostOps());
  conf().set(
    ConfigOptions::getMapCleanerTransformsKey(), TestUtils::getConflateCmdSnapshotCleaningOps());

  SuperfluousConflateOpRemover::removeSuperfluousOps();

  actualOps = conf().getList(ConfigOptions::getConflatePreOpsKey());
  LOG_VART(actualOps);
  CPPUNIT_ASSERT_EQUAL(expectedPreOpSize, actualOps.size());

  actualOps = conf().getList(ConfigOptions::getConflatePostOpsKey());
  LOG_VART(actualOps);
  CPPUNIT_ASSERT_EQUAL(expectedPostOpsSize, actualOps.size());

  actualOps = conf().getList(ConfigOptions::getMapCleanerTransformsKey());
  LOG_VART(actualOps);
  CPPUNIT_ASSERT_EQUAL(expectedCleaningOpsSize, actualOps.size());
}

}
