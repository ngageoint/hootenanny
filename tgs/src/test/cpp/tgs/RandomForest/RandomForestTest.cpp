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
#include <fstream>
#include <map>
#include <set>
#include <sstream>
#include <utility>
#include <vector>

// CPP Unit Includes
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

#include "../PluginFactory.h"
#include <tgs/RandomForest/DataFrame.h>
#include <tgs/RandomForest/RandomForest.h>

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

  const std::string rfdFactors[] = {"Area", "Slice1Area", "Slice2Area", "Slice3Area", "Slice4Area", "Slice5Area", "Slice6Area",
    "Slice7Area", "CenterX" ,"CenterY", "CenterZ", "Height", "Perimeter", "Volume", "Compact2d"};

  class RandomForestTest : public CppUnit::TestFixture
  {
    CPPUNIT_TEST_SUITE(RandomForestTest);
    CPPUNIT_TEST(test1);
    CPPUNIT_TEST(test2);
    CPPUNIT_TEST_SUITE_END();
  public:

    void test1()
    {
//       std::cout <<"Start Data Frame Test"  << std::endl;
//       RandomForest rf;
//       for(int i = 0; i < 12; i++)
//       {
//         std::vector<double> vi;
//         vi.push_back(area[i]);
//         vi.push_back(perimeter[i]);
//         vi.push_back(compactness[i]);
//         rf.addTrainingVector(labels[i], vi);
//       }
// 
//       clock_t t1 = clock();
//       rf.train(100, 2);
//       clock_t t2 = clock();
// 
//       std::cout << "Time to build: " << (double)(t2 - t1)/(double)CLOCKS_PER_SEC << std::endl;
// 
// //       std::fstream rfStream;
// //       std::fstream rfStream2;
// //       std::fstream rfStream3;
// // 
// //       rfStream.open("exportRf.txt", std::fstream::out);
// //       rf.export(rfStream);
// //       rfStream.close();
// // 
// //       rfStream2.open("exportRf.txt", std::fstream::in);
// //       rf.import(rfStream2);
// //       rfStream2.close();
// // 
// //       rfStream3.open("exportRfTest.txt", std::fstream::out);
// //       rf.export(rfStream3);
// //       rfStream3.close();
// 
// 
//       std::set<std::string> classLabels = rf.getClassLabels();
//       std::set<std::string>::iterator setItr;
// 
//       std::map<std::string, double>::iterator itr;
//       for(unsigned int k  = 0; k < 4; k++)
//       {
//         std::vector<double> dataVec(3);
//         dataVec[0] = testArea[k];
//         dataVec[1] = testPerimeter[k];
//         dataVec[2] = testCompactness[k];
// 
//         std::map<std::string, double> scores;
// 
//         rf.classifyVector(dataVec, scores);
// 
//         std::cout << "Vector:  " << k << std::endl;
// 
//         for(setItr = classLabels.begin(); setItr != classLabels.end(); ++setItr)
//         {
//           std::cout << "Class " << *setItr << " Prob " << scores[*setItr] << std::endl;
//         }
//       }
    }

    void test2()
    {
//       std::string trainFile = "c:\\GIS_DATA\\OttawaData\\rfd_data\\train_15_0_small_rfd.txt";
//       std::string testFile = "c:\\GIS_DATA\\OttawaData\\rfd_data\\test_15_0_small_rfd.txt";
// 
//       std::vector<std::string> factorLabels(15);
// 
//       for(unsigned int i = 0; i < 15; i++)
//       {
//         factorLabels[i] = rfdFactors[i];
//       }
// 
//       RandomForest rf;
// 
//       rf.setFactorLabels(factorLabels);
// 
//       std::fstream inStream;
//       std::string inBuffer;
// 
//       inStream.open(trainFile.c_str(), std::fstream::in);
// 
//       if(inStream.is_open())
//       {
//         std::cout << "File Open" << std::endl;
//         bool headerSkipped = false;
//         while(!inStream.eof())
//         {
//           if(!headerSkipped)
//           {
//             std::getline(inStream, inBuffer);
//             headerSkipped = true;
//           }
//           //std::getline(inStream, inBuffer);
//           //if(inBuffer!= "")
//           {
//             std::vector<double> dataVec(factorLabels.size());
//             std::string classStr;
//             //std::istringstream ss(inBuffer);
// 
//             unsigned int id;
//             for(unsigned int j = 0; j < factorLabels.size() + 2; j++)
//             {
//               if(j == 0)
//               {
//                 inStream >> classStr;
//                 //std::cout << classStr << " ";
//               }
//               else if(j == 1)
//               {
//                 inStream >> id;
//                 //std::cout << id << " ";
//               }
//               else
//               {
//                 inStream >> dataVec[j - 2];
//                 //std::cout << dataVec[j - 2] << " ";
//               }
//             }
//            
//             //std::cout << std::endl;
//             rf.addTrainingVector(classStr, dataVec);
//           } 
//         }
//         
//         inStream.close();
//         //exit(0);
//       }
// 
//       clock_t t1 = clock();
//       rf.buildTest(100, 3);
//       clock_t t2 = clock();
// 
//       std::cout << "Time to build: " << (double)(t2 - t1)/(double)CLOCKS_PER_SEC << std::endl;
// 
//       std::map<std::string, double> topFactors;
//       std::map<std::string, double>::iterator itr;
// 
//       rf.getFactorImportance(topFactors);
// 
//       for(itr = topFactors.begin(); itr != topFactors.end(); itr++)
//       {
//         std::cout << "F " << itr->first << " " << itr->second << std::endl;
//       }
// 
//       std::vector<std::pair<std::string, std::vector<double>>> results;
//       std::map<std::string, unsigned int> classLabels = rf.getClassLabels();
// 
//       std::fstream inStream2;
//       inStream2.open(testFile.c_str(), std::fstream::in);
//       std::string inBuffer2;
// 
//       std::map<std::string, unsigned int>::iterator mapItr;
//       //std::map<std:string, std:map<std:string, unsigned int>>> confusionMatrix;
// 
// 
//       std::fstream outStream;
//       outStream.open("ResultsRf.txt", std::fstream::out);
// 
//       bool headerWritten = false;
//       if(inStream2.is_open())// && outStream.is_open())
//       {
//         std::cout << "Files Open2" << std::endl;
// 
//         unsigned int testCtr = 0;
//         bool headerSkipped = false;
// 
//         while(!inStream2.eof())
//         {
//           if(!headerSkipped)
//           {
//             std::getline(inStream2, inBuffer2);
//             headerSkipped = true;
//           }
//           //std::getline(inStream, inBuffer);
//           //if(inBuffer!= "")
//           {
//             std::vector<double> dataVec2(factorLabels.size());
//             std::string classStr;
//             //std::istringstream ss(inBuffer);
// 
//             unsigned int id;
//             for(unsigned int j = 0; j < factorLabels.size() + 2; j++)
//             {
//               if(j == 0)
//               {
//                 inStream2 >> classStr;
//                 //std::cout << classStr << " ";
//               }
//               else if(j == 1)
//               {
//                 inStream2 >> id;
//                 //std::cout << id << " ";
//               }
//               else
//               {
//                 inStream2 >> dataVec2[j - 2];
//                 //std::cout << dataVec2[j - 2] << " ";
//               }
//             }
// 
//             //std::cout << std::endl;
// 
//             std::map<std::string, double> scores;
// 
//             rf.classifyVector(dataVec2, scores);
// 
//             outStream << "Vector:  " << testCtr << " Class: " << classStr << std::endl;
//             testCtr++;
//             for(mapItr = classLabels.begin(); mapItr != classLabels.end(); ++mapItr)
//             {
//               outStream << "Class " << mapItr->first << " Prob " << scores[mapItr->first] << std::endl;
//             }
//  
// 
//           } 
//         }
//         //outStream.close();
//         inStream2.close();
//       }
// 

    }
  };
}  //End namespace

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(SaUrgent::RandomForestTest, PluginFactory::testName());
