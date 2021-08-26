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
 * @copyright Copyright (C) 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/OsmJsonReader.h>
#include <hoot/core/io/OsmJsonWriter.h>
#include <hoot/core/io/OsmPbfReader.h>
#include <hoot/core/io/OsmPbfWriter.h>

#include <hoot/rnd/conflate/multiary/MultiaryUtilities.h>

using namespace std;

namespace hoot
{

class MultiaryUtilitiesTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(MultiaryUtilitiesTest);
  CPPUNIT_TEST(conflateClusterTest);
  CPPUNIT_TEST(findMatchesTest);
  CPPUNIT_TEST_SUITE_END();

public:

  QByteArray convertJsonToPbf(QString json)
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmJsonReader().loadFromString(json, map);

    assert(map->getNodeCount() == 1);

    std::stringstream ss;
    OsmPbfWriter writer;
    writer.writePb(map->getNodes().begin()->second, &ss);

    QByteArray result(ss.str().data(), ss.str().size());

    return result;
  }

  /**
   * Basic set of tests to see if the scores returned are sensible.
   */
  void conflateClusterTest()
  {
    QByteArray e1 = convertJsonToPbf(
      "{ 'elements': [ { 'type': 'node', 'id': -1, 'lat': 2.0, 'lon': -3.0, \n"
      "   'tags': { 'amenity': 'pub', 'name': 'My Restaurant' } } ] }");
    QByteArray e2 = convertJsonToPbf(
      "{ 'elements': [ { 'type': 'node', 'id': -1, 'lat': 2.0, 'lon': -3.0, \n"
      "   'tags': { 'amenity': 'pub', 'name': 'My Restaurant', \n"
      "   'hoot:hash': 'sha1sum:c40b57f15ac814272b1b4cd42302c29d15cab9d9', \n"
      "   'source:url': 'http://wikimapia.org/36956748/15-43', \n"
      "   'is_in:country': 'Yemen'\n"
      "} } ] }");

    QByteArray e3 = convertJsonToPbf(
      "{ 'elements': [ { 'type': 'node', 'id': -1, 'lat': 3.0, 'lon': -3.0, \n"
      "   'tags': { 'amenity': 'pub', 'name': 'My Restaurant' } } ] }");


    TestUtils::resetEnvironment();

    QList<QByteArray> elements;
    elements.append(e1);
    elements.append(e2);
    elements.append(e3);

    QList<hoot::MultiaryElement> result = MultiaryUtilities::conflateCluster(elements);

    HOOT_STR_EQUALS("[2]{{hash: 'sha1sum:537bc7a383257c8c1cdf69b461af87344a0a0ba5', bounds: 'Env[-3.0018:-2.9982,2.9982:3.0018]', payload: 'AAAApwqGAQoACgdhbWVuaXR5CgNwdWIKCWhvb3Q6aGFzaAowc2hhMXN1bTo1MzdiYzdhMzgzMjU3YzhjMWNkZjY5YjQ2MWFmODczNDRhMGEwYmE1CgRuYW1lCg1NeSBSZXN0YXVyYW50Cg5lcnJvcjpjaXJjdWxhcgoCMTUKC2hvb3Q6c3RhdHVzCgExEhwKGggFEgUBAwUHCRoFAgQGCApAgI7OHEj/jc4c'}, {hash: 'sha1sum:bd4ba434e321fd3eb0d1597044b962412067fd8a', bounds: 'Env[-3.0018:-2.9982,1.9982:2.0018]', payload: 'AAABMwqMAgoACg1pc19pbjpjb3VudHJ5CgVZZW1lbgoHYW1lbml0eQoDcHViCglob290Omhhc2gKMHNoYTFzdW06YmQ0YmE0MzRlMzIxZmQzZWIwZDE1OTcwNDRiOTYyNDEyMDY3ZmQ4YQoEbmFtZQoNTXkgUmVzdGF1cmFudAoKc291cmNlOnVybAojaHR0cDovL3dpa2ltYXBpYS5vcmcvMzY5NTY3NDgvMTUtNDMKC3NvdXJjZTpoYXNoCjBzaGExc3VtOmM0MGI1N2YxNWFjODE0MjcyYjFiNGNkNDIzMDJjMjlkMTVjYWI5ZDkKDmVycm9yOmNpcmN1bGFyCgIxNQoLaG9vdDpzdGF0dXMKATMSIgogCAESCAEDBQcJCw0PGggCBAYICgwOEECAtIkTSP+Nzhw='}}", result);
  }

  /**
   * Basic set of tests to see if the scores returned are sensible.
   */
  void findMatchesTest()
  {
    QByteArray e1 = convertJsonToPbf(
      "{ 'elements': [ { 'type': 'node', 'id': -1, 'lat': 2.0, 'lon': -3.0, \n"
      "   'tags': { 'amenity': 'pub', 'name': 'My Restaurant' } } ] }");
    QByteArray e2 = convertJsonToPbf(
      "{ 'elements': [ { 'type': 'node', 'id': -1, 'lat': 2.0, 'lon': -3.0, \n"
      "   'tags': { 'amenity': 'pub', 'name': 'My Restaurant', \n"
      "   'hoot:hash': 'sha1sum:c40b57f15ac814272b1b4cd42302c29d15cab9d9', \n"
      "   'source:url': 'http://wikimapia.org/36956748/15-43', \n"
      "   'is_in:country': 'Yemen'\n"
      "} } ] }");

    QByteArray e3 = convertJsonToPbf(
      "{ 'elements': [ { 'type': 'node', 'id': -1, 'lat': 3.0, 'lon': -3.0, \n"
      "   'tags': { 'amenity': 'pub', 'name': 'My Restaurant' } } ] }");


    QList<QByteArray> againstElements;
    againstElements.append(e2);
    againstElements.append(e3);

    QList<hoot::MultiarySimpleMatch> result = MultiaryUtilities::findMatches(e1, againstElements);

    HOOT_STR_EQUALS("[1]{{neighborIndex: 0, score: 0.757576}}", result);
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MultiaryUtilitiesTest, "glacial");

}
