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
 * @copyright Copyright (C) 2015, 2021 Maxar (http://www.maxar.com/)
 */

// Standard includes
#include <ctime>
#include <vector>

// CPP Unit Includes
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

#include "../PluginFactory.h"
#include <tgs/RandomForest/DataFrame.h>
#include <tgs/RandomForest/RandomTree.h>

namespace SaUrgent
{

  // A set of factors with labels to test
  const std::string labels[] = {"T", "L", "R", "O", "T", "L", "R", "O", "T", "L", "R", "O"};
  const double area[] = {0.5, 0.3125, 0.64, 0.7850, 0.1250,  0.3126, 1, 0.3925, 0.25, 0.75, 0.51, 0.3926};
  const double perimeter[] = {3.0000, 3.0001, 3.2, 3.14, 2.25, 3.0002, 4, 2.2203, 2.8, 4.0001, 3.0003, 2.2204};
  const double compactness[] = {0.6978, 0.4361, 0.7850, 1, 0.3101, 0.4362, 0.7851, 1.0001, 0.4005, 0.5888, 0.6979, 1.0002};
  const unsigned int areaSort[] = {4, 8, 1, 5, 7, 11, 0, 10, 2, 9, 3, 6};
  const unsigned int perimSort[] = {7, 11, 4, 8, 0, 1, 5, 10, 3, 2, 6, 9};
  const unsigned int compactSort[] = {4, 8, 1, 5, 9, 0, 10, 2, 6, 3, 7, 11};
  const std::string testLabels[] = {"T", "L", "L", "O", "O", "O", "R", "R", "R", "R", "Q", "X"};
  const std::string testClasses[] = {"T", "L", "O", "R", "Q", "X"};
  const unsigned int testClassCounts[] = {1, 2, 3, 4, 1, 1};
  const std::string testTreeString = "S 2 0.69785\nS 1 3.00005\nP T\nP L\nS 2 0.89255\nP R\nP O\n";

  const std::string classTestLabels[] = {"T", "L", "R", "O"};
  const double testArea[] = {0.25, 0.4375, 0.5, 0.79};
  const double testPerimeter[] = {2.3, 3.75, 3, 1.57};
  const double testCompactness[] = {0.59, 0.39, 0.7, 1.00};

  class RandomTreeTest : public CppUnit::TestFixture
  {
    CPPUNIT_TEST_SUITE(RandomTreeTest);
    CPPUNIT_TEST(test1);
    CPPUNIT_TEST_SUITE_END();
  public:
    void test1()
    {
//       //std::cout <<"Start Data Frame Test"  << std::endl;
//       DataFrame df;
//       for(int i = 0; i < 12; i++)
//       {
//         std::vector<double> vi;
//         vi.push_back(area[i]);
//         vi.push_back(perimeter[i]);
//         vi.push_back(compactness[i]);
//         df.addDataVector(labels[i], vi);
//       }
// 
//       RandomTree rt;
//       rt.buildTest(df, 3);
// 
//       std::map<unsigned int, double> factImport;
//       rt.getFactorImportance(factImport);
// 
//       std::map<unsigned int, double>::iterator itr;
// 
//       for(itr = factImport.begin(); itr != factImport.end(); ++itr)
//       {
//         std::cout << "Factor " << itr->first << " - " << itr->second << std::endl;
//       }
// 
//       std::string treeOutput;
// 
//       rt.toString(treeOutput);
// 
//       //Test consistent tree build
//       CPPUNIT_ASSERT(treeOutput == testTreeString);
// 
//       RandomTree rt2;
//       rt2.load(treeOutput);
// 
//       std::string treeOutput2;
// 
//       rt2.toString(treeOutput2);
// 
//       //Test tree importing
//       CPPUNIT_ASSERT(treeOutput == treeOutput2);
// 
//       std::string resultClass;
// 
//       for(unsigned int j = 0; j < 4; j++)
//       {
//         std::vector<double> dataVec(3);
//         dataVec[0] = testArea[j];
//         dataVec[1] = testPerimeter[j];
//         dataVec[2] = testCompactness[j];
// 
//         rt.classifyDataVector(dataVec, resultClass);
// 
//         //Test correct classification result
//         CPPUNIT_ASSERT( resultClass == classTestLabels[j]);
//         //std::cout << "The result is " << resultClass << " " << classTestLabels[j] << std::endl;
//       }
    }
  };
}  //End namespace

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(SaUrgent::RandomTreeTest, PluginFactory::testName());
