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
 * @copyright Copyright (C) 2012, 2013, 2014, 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/elements/ExternalMergeElementSorter.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/PartialOsmMapReader.h>
#include <hoot/core/util/Log.h>

namespace hoot
{

class ExternalMergeElementSorterTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ExternalMergeElementSorterTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST_SUITE_END();

public:

  ExternalMergeElementSorterTest() : HootTestFixture("test-files/elements/", UNUSED_PATH)
  {
  }

  void runTest()
  {
    //Since ExternalMergeElementSorter writes chunks of maps, it naturally sets off some of the
    //incomplete map warnings which we don't want to see.
    //comment out for debugging only
    DisableLog dl;

    const QString input = _inputPath + "ExternalMergeElementSorterTest.osm";

    std::shared_ptr<PartialOsmMapReader> reader =
      std::dynamic_pointer_cast<PartialOsmMapReader>(
        OsmMapReaderFactory::createReader(input));
    reader->setUseDataSourceIds(true);
    reader->open(input);
    reader->initializePartial();

    ExternalMergeElementSorter elementSorter;
    elementSorter.setMaxElementsPerFile(5);
    //only enable this for debugging
    //elementSorter.setRetainTempFiles(true);
    elementSorter.sort(std::dynamic_pointer_cast<ElementInputStream>(reader));

    int index = 0;
    long lastId = 0;
    ElementType lastElementType = ElementType::Node;

    while (elementSorter.hasMoreElements())
    {
      ElementPtr element = elementSorter.readNextElement();

      LOG_TRACE(element->toString());
      LOG_VART(index);
      LOG_VART(element->getElementId().getId());
      LOG_VART(lastId);

      //elements should be returned in the order nodes, ways, then relations; ids should always be
      //increasing within each element type
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
      if (lastId != 0 && lastElementType == element->getElementType().getEnum())
      {
        CPPUNIT_ASSERT(element->getElementId().getId() > lastId);
      }

      lastId = element->getElementId().getId();
      lastElementType = element->getElementType();
      index++;
    }
    CPPUNIT_ASSERT_EQUAL(29, index);
    CPPUNIT_ASSERT_EQUAL(6, elementSorter.getNumTempFiles());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ExternalMergeElementSorterTest, "quick");

}



