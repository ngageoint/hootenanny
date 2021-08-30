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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/conflate/network/EdgeMatchSetFinder.h>
#include <hoot/core/conflate/network/OsmNetworkExtractor.h>
#include <hoot/core/criterion/TagCriterion.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/elements/MapProjector.h>

namespace hoot
{

class EdgeMatchSetFinderTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(EdgeMatchSetFinderTest);
  CPPUNIT_TEST(partialTest1);
  CPPUNIT_TEST(partialTest2);
  CPPUNIT_TEST(partialTest3);
  CPPUNIT_TEST(partialTest4);
  CPPUNIT_TEST(partialTest5);
  CPPUNIT_TEST_SUITE_END();

public:
  OsmNetworkPtr network1;
  OsmNetworkPtr network2;
  IndexedEdgeMatchSetPtr matchSet;

  EdgeMatchSetFinderTest() :
  HootTestFixture("test-files/conflate/network/", "test-files/conflate/network/")
  {
    setResetType(ResetEnvironment);
  }

  EdgeMatchSetFinderPtr loadTest(int testNumber)
  {
    OsmMapPtr map = std::make_shared<OsmMap>();

    OsmMapReaderFactory::read(map, _inputPath + "ParitalEdgeMatch.osm",
      false, Status::Unknown1);
    MapProjector::projectToPlanar(map);

    OsmNetworkExtractor one;

    ElementCriterionPtr c1 =
      std::make_shared<TagCriterion>("name", QString("partialTest%1A").arg(testNumber));
    one.setCriterion(c1);
    network1 = one.extractNetwork(map);

    ElementCriterionPtr c2 =
      std::make_shared<TagCriterion>("name", QString("partialTest%1B").arg(testNumber));
    one.setCriterion(c2);
    network2 = one.extractNetwork(map);

    matchSet = std::make_shared<IndexedEdgeMatchSet>();
    NetworkDetailsPtr details = std::make_shared<NetworkDetails>(map, network1, network2);
    conf().set(ConfigOptions().getWaySublineMatcherKey(), "MaximalSublineMatcher");
    conf().set(
      ConfigOptions().getConflateMatchHighwayClassifierKey(), "HighwayExpertClassifier");
    details->setConfiguration(conf());
    EdgeMatchSetFinderPtr uut =
      std::make_shared<EdgeMatchSetFinder>(details, matchSet, network1, network2);

    return uut;
  }

  /**
   * Verify that two lines that start at the same vertex, but only have a partial match still work.
   */
  void partialTest1()
  {
    EdgeMatchSetFinderPtr uut = loadTest(1);

    uut->setIncludePartialMatches(true);
    uut->addEdgeMatches(network1->getEdgeMap().begin().value(),
      network2->getEdgeMap().begin().value());

   HOOT_STR_EQUALS("[1]{(s1: [1]{{ _start: { _e: (0) Node(-1) -- Way(-1) -- (1) Node(-9), _portion: 0 }, _end: { _e: (0) Node(-1) -- Way(-1) -- (1) Node(-9), _portion: 0.677087 } }} s2: [1]{{ _start: { _e: (2) Node(-10) -- Way(-2) -- (3) Node(-17), _portion: 0 }, _end: { _e: (2) Node(-10) -- Way(-2) -- (3) Node(-17), _portion: 1 } }}, 0.156892)}",
      matchSet);
  }

  /**
   * +--------------+ 2A
   * +-----+-----+ 2B
   *
   * Should create one match where 2A is a partial
   */
  void partialTest2()
  {
    EdgeMatchSetFinderPtr uut = loadTest(2);

    uut->setIncludePartialMatches(true);
    uut->addEdgeMatches(network1->getEdgeMap().begin().value(),
      network2->getEdgeMap().begin().value());

    HOOT_STR_EQUALS("[1]{(s1: [1]{{ _start: { _e: (0) Node(-23) -- Way(-5) -- (1) Node(-24), _portion: 0 }, _end: { _e: (0) Node(-23) -- Way(-5) -- (1) Node(-24), _portion: 0.71542 } }} s2: [2]{{ _start: { _e: (4) Node(-18) -- Way(-3) -- (3) Node(-20), _portion: 0 }, _end: { _e: (4) Node(-18) -- Way(-3) -- (3) Node(-20), _portion: 1 } }, { _start: { _e: (2) Node(-21) -- Way(-4) -- (3) Node(-20), _portion: 1 }, _end: { _e: (2) Node(-21) -- Way(-4) -- (3) Node(-20), _portion: 0 } }}, 0.100114)}",
      matchSet);
  }

  /**
   * +--------------+ 3A
   *      +-----+ 3B
   *
   * Should create one match where 3A is a partial at both ends
   */
  void partialTest3()
  {
    EdgeMatchSetFinderPtr uut = loadTest(3);

    uut->setIncludePartialMatches(true);
    uut->addEdgeMatches(network1->getEdgeMap().begin().value(),
      network2->getEdgeMap().begin().value());

    LOG_VAR(matchSet);
    HOOT_STR_EQUALS("[1]{(s1: [1]{{ _start: { _e: (0) Node(-25) -- Way(-6) -- (1) Node(-26), _portion: 0.276842 }, _end: { _e: (0) Node(-25) -- Way(-6) -- (1) Node(-26), _portion: 0.742578 } }} s2: [1]{{ _start: { _e: (2) Node(-27) -- Way(-7) -- (3) Node(-28), _portion: 0 }, _end: { _e: (2) Node(-27) -- Way(-7) -- (3) Node(-28), _portion: 1 } }}, 0.110612)}",
      matchSet);
  }

  /**
   * +-------+--------+-------+ 4A
   *      +-------+-------+ 4B
   *
   * Two partials on 4A and no partials on 4B
   */
  void partialTest4()
  {
    EdgeMatchSetFinderPtr uut = loadTest(4);

    uut->setIncludePartialMatches(true);
    uut->addEdgeMatches(network1->getEdgeMap().begin().value(),
      network2->getEdgeMap().begin().value());

    LOG_VAR(matchSet);
    HOOT_STR_EQUALS("[1]{(s1: [3]{{ _start: { _e: (2) Node(-32) -- Way(-9) -- (0) Node(-31), _portion: 0.492027 }, _end: { _e: (2) Node(-32) -- Way(-9) -- (0) Node(-31), _portion: 1 } }, { _start: { _e: (0) Node(-31) -- Way(-12) -- (1) Node(-30), _portion: 0 }, _end: { _e: (0) Node(-31) -- Way(-12) -- (1) Node(-30), _portion: 1 } }, { _start: { _e: (3) Node(-29) -- Way(-8) -- (1) Node(-30), _portion: 1 }, _end: { _e: (3) Node(-29) -- Way(-8) -- (1) Node(-30), _portion: 0.478191 } }} s2: [2]{{ _start: { _e: (4) Node(-35) -- Way(-11) -- (5) Node(-34), _portion: 0 }, _end: { _e: (4) Node(-35) -- Way(-11) -- (5) Node(-34), _portion: 1 } }, { _start: { _e: (6) Node(-33) -- Way(-10) -- (5) Node(-34), _portion: 1 }, _end: { _e: (6) Node(-33) -- Way(-10) -- (5) Node(-34), _portion: 0 } }}, 0.104083)}",
      matchSet);
  }

  /**
   * +---------------------+ 5A
   *        +--------------+ 5B
   *
   * One partial on 5A
   */
  void partialTest5()
  {
    EdgeMatchSetFinderPtr uut = loadTest(5);

    uut->setIncludePartialMatches(true);
    uut->addEdgeMatches(network1->getEdgeMap().begin().value(),
      network2->getEdgeMap().begin().value());

    LOG_VAR(matchSet);
    HOOT_STR_EQUALS("[1]{(s1: [1]{{ _start: { _e: (0) Node(-38) -- Way(-14) -- (1) Node(-39), _portion: 0.531148 }, _end: { _e: (0) Node(-38) -- Way(-14) -- (1) Node(-39), _portion: 1 } }} s2: [1]{{ _start: { _e: (2) Node(-36) -- Way(-13) -- (3) Node(-37), _portion: 0 }, _end: { _e: (2) Node(-36) -- Way(-13) -- (3) Node(-37), _portion: 1 } }}, 0.108275)}",
      matchSet);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(EdgeMatchSetFinderTest, "quick");

}
