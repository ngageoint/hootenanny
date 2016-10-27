///*
// * This file is part of Hootenanny.
// *
// * Hootenanny is free software: you can redistribute it and/or modify
// * it under the terms of the GNU General Public License as published by
// * the Free Software Foundation, either version 3 of the License, or
// * (at your option) any later version.
// *
// * This program is distributed in the hope that it will be useful,
// * but WITHOUT ANY WARRANTY; without even the implied warranty of
// * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// * GNU General Public License for more details.
// *
// * You should have received a copy of the GNU General Public License
// * along with this program.  If not, see <http://www.gnu.org/licenses/>.
// *
// * --------------------------------------------------------------------
// *
// * The following copyright notices are generated automatically. If you
// * have a new notice to add, please use the format:
// * " * @copyright Copyright ..."
// * This will properly maintain the copyright information. DigitalGlobe
// * copyrights will be updated automatically.
// *
// * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
// */

//// Hoot
//#include <hoot/core/TestUtils.h>
//#include <hoot/core/io/OsmMapReaderFactory.h>
//#include <hoot/core/io/OsmMapWriterFactory.h>
//#include <hoot/core/conflate/polygon/AreaMerger.h>
//#include <hoot/core/MapProjector.h>

//// Qt
//#include <QDir>

//namespace hoot
//{

///**
// *
// */
//class AreaMergerTest : public CppUnit::TestFixture
//{
//  CPPUNIT_TEST_SUITE(AreaMergerTest);
//  //CPPUNIT_TEST(basicTest);
//  //CPPUNIT_TEST(mergeLessThanTwoInputsTest);
//  //CPPUNIT_TEST(mergeMoreThanTwoInputsTest);
//  CPPUNIT_TEST_SUITE_END();

//public:

//  void basicTest()
//  {
//    OsmMap::resetCounters();
//    OsmMapPtr map(new OsmMap());
//    OsmMapReaderFactory::read(
//      map, "?", false,
//      Status::Unknown1);

//    AreaMerger::merge(map);

//    QDir().mkdir("test-output/conflate/polygon");
//    MapProjector::projectToWgs84(map);
//    OsmMapWriterFactory::getInstance().write(map,
//      "test-output/conflate/polygon/?.osm");

//    HOOT_FILE_EQUALS(
//      "test-files/conflate/polygon/?.osm",
//      "test-output/conflate/polygon/?.osm");
//  }

//  void mergeLessThanTwoInputsTest()
//  {
//    QString exceptionMsg("");
//    try
//    {
//      OsmMapPtr map(new OsmMap());
//      OsmMapReaderFactory::read(
//        map, "", false,
//        Status::Unknown1);

//      AreaMerger::merge(map);
//    }
//    catch (HootException e)
//    {
//      exceptionMsg = e.what();
//    }
//    HOOT_STR_EQUALS("Two areas were not passed to the area merger.", exceptionMsg.toStdString());
//  }

//  void mergeMoreThanTwoInputsTest()
//  {
//    QString exceptionMsg("");
//    try
//    {
//      OsmMapPtr map(new OsmMap());
//      OsmMapReaderFactory::read(
//        map, "", false,
//        Status::Unknown1);

//      AreaMerger::merge(map);
//    }
//    catch (HootException e)
//    {
//      exceptionMsg = e.what();
//    }
//    HOOT_STR_EQUALS("Two areas were not passed to the area merger.", exceptionMsg.toStdString());
//  }

//};

//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(AreaMergerTest, "quick");

//}
