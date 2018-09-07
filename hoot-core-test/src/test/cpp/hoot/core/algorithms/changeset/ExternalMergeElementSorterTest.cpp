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
 * @copyright Copyright (C) 2012, 2013, 2014, 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/algorithms/changeset/ExternalMergeElementSorter.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/util/Log.h>

namespace hoot
{

class ExternalMergeElementSorterTest : public HootTestFixture
{
    //TODO: finish
    CPPUNIT_TEST_SUITE(ExternalMergeElementSorterTest);
    //CPPUNIT_TEST(runTest);
    CPPUNIT_TEST_SUITE_END();

public:

  void runTest()
  {
    /*OsmMapPtr inputMap(new OsmMap());
    OsmMapReaderFactory::read(inputMap, "test-files/MultipolygonTest.osm", true);

    ExternalMergeElementSorter elementSorter(inputMap);

    int index = 0;
    while (elementSorter.hasMoreElements())
    {
      ElementPtr element = elementSorter.readNextElement();
      LOG_TRACE(element->toString());
      LOG_VART(index);
      //elements should be returned in the order nodes, ways, then relations
      if (index >= 0 && index <=15)
      {
        CPPUNIT_ASSERT(element->getElementType() == ElementType::Node);
      }
      else if (index >= 16 && index <= 19)
      {
        CPPUNIT_ASSERT(element->getElementType() == ElementType::Way);
      }
      if (index >= 20 && index <= 28)
      {
        CPPUNIT_ASSERT(element->getElementType() == ElementType::Relation);
      }
      index++;
    }
    CPPUNIT_ASSERT_EQUAL(29, index);*/
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ExternalMergeElementSorterTest, "quick");

}



