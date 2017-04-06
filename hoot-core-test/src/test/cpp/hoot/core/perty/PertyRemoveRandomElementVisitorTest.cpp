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
 * @copyright Copyright (C) 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

// boost
#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_real.hpp>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Hoot
#include <hoot/core/util/Exception.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/perty/PertyRemoveRandomElementVisitor.h>
#include <hoot/core/util/Log.h>

// Qt
#include <QDir>

using namespace std;

namespace hoot
{

class PertyRemoveRandomElementVisitorTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(PertyRemoveRandomElementVisitorTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runBasicTest()
  {
    boost::shared_ptr<OsmMap> map(new OsmMap());
    OGREnvelope env;
    env.MinX = 0;
    env.MinY = 0;
    env.MaxX = 1;
    env.MaxY = 1;
    map->setProjection(MapProjector::createAeacProjection(env));

    boost::minstd_rand rng(1);
    boost::uniform_real<> uni(0.0, 1000.0);

    for (int i = 0; i < 100; i++)
    {
      NodePtr n(new Node(Status::Unknown1, map->createNextNodeId(), uni(rng), uni(rng), 10));
      map->addNode(n);
    }

    PertyRemoveRandomElementVisitor v;
    v.setRng(rng);
    map->visitRw(v);

    CPPUNIT_ASSERT_EQUAL(96, (int)map->getNodes().size());
  }

};

//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(RemoveRandomElementVisitorTest, "current");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PertyRemoveRandomElementVisitorTest, "quick");

}
