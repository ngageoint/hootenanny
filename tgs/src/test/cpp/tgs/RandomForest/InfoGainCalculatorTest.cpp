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
 * @copyright Copyright (C) 2015, 2019, 2021 Maxar (http://www.maxar.com/)
 */

// Standard includes
#include <iostream>
#include <vector>

// CPP Unit Includes
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

#include "../PluginFactory.h"
#include <tgs/RandomForest/DataFrame.h>
#include <tgs/RandomForest/InfoGainCalculator.h>

namespace Tgs
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
  const double maxGains[] = {0.520721, 0.750295, 1.0};
  const unsigned int maxSplits[] = {7, 5, 6};
  const unsigned int maxIgFact = 2;
  const double maxDataSplitVal = 0.69785;
  const unsigned int maxSplitIdx = 6;

  class InfoGainCalculatorTest : public CppUnit::TestFixture
  {
    CPPUNIT_TEST_SUITE(InfoGainCalculatorTest);
    CPPUNIT_TEST(test1);
    CPPUNIT_TEST_SUITE_END();

  public:
   
    void test1()
    {
      DataFrame df;
      InfoGainCalculator igc;

      std::vector<unsigned int> indices;
      indices.resize(12);

      for(unsigned int i = 0; i < 12; i++)
      {
        std::vector<double> vi;
        vi.push_back(area[i]);
        vi.push_back(perimeter[i]);
        vi.push_back(compactness[i]);
        df.addDataVector(labels[i], vi);
        indices[i] = i;
      }

      std::vector<unsigned int> indicesA, indicesP, indicesC;
      indicesA.resize(12);
      indicesP.resize(12);
      indicesC.resize(12);

      for(unsigned int j = 0; j < 12; j++)
      {
        indicesA[j] = j;
        indicesC[j] = j;
        indicesP[j] = j;
      }

      df.sortIndicesOnFactorValue(indicesA, 0);
      df.sortIndicesOnFactorValue(indicesP, 1);
      df.sortIndicesOnFactorValue(indicesC, 2);

      double totalEntropy = igc.computeEntropyByClass(df, indices);

      CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, totalEntropy, 1e-6);

      double maxGainA, maxGainP, maxGainC;
      unsigned int bestSplitA, bestSplitP, bestSplitC;

      maxGainA = igc.getMaxInfoGainByFactor(df, indicesA, 0, totalEntropy, bestSplitA);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(maxGainA, maxGains[0], 1E-6);
      CPPUNIT_ASSERT_EQUAL(bestSplitA, maxSplits[0]);

      maxGainP = igc.getMaxInfoGainByFactor(df, indicesP, 1, totalEntropy, bestSplitP);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(maxGainP, maxGains[1], 1E-6);
      CPPUNIT_ASSERT_EQUAL(bestSplitP, maxSplits[1]);

      maxGainC = igc.getMaxInfoGainByFactor(df, indicesC, 2, totalEntropy, bestSplitC);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(maxGainC, maxGains[2], 1E-6);
      CPPUNIT_ASSERT_EQUAL(bestSplitC, maxSplits[2]);

      //       std::cout << "Gain A: " << maxGainA << ", Split: " << bestSplitA << std::endl;
      //       std::cout << "Gain P: " << maxGainP << ", Split: " << bestSplitP << std::endl;
      //       std::cout << "Gain C: " << maxGainC << ", Split: " << bestSplitC << std::endl;


      unsigned int splitIdx = 0;
      unsigned int fIdx = 0;
      double splitVal = 0.0;
      double purityDelta = 0.0;

      std::vector<unsigned int> fIndices(3);
      for(unsigned int k = 0; k < 3; k++)
      {
        fIndices[k] = k;
      }

      igc.findDataSplit(df,fIndices, indices, splitIdx, fIdx, splitVal, purityDelta);
      CPPUNIT_ASSERT_EQUAL(splitIdx, maxSplitIdx);
      CPPUNIT_ASSERT_EQUAL(fIdx, maxIgFact);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(splitVal, maxDataSplitVal, 1E-6);
    }
  };
}  //End namespace

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(Tgs::InfoGainCalculatorTest, PluginFactory::testName());
