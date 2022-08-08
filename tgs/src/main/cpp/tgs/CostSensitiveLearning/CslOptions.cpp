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
 * @copyright Copyright (C) 2015, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "CslOptions.h"

//Qt Includes
#include <QFileInfo>

//Std Includes
#include <iostream>
#include <sstream>
#include <string>
#include <typeinfo>

#include "../RandomForest/RandomForestUtilities.h"
#include "../RandomForest/RandomForestManager.h"
#include "../RandomForest/MultithreadedRandomForestManager.h"
#include "CostSensitiveLearningTrainer.h"
#include "CslUtilities.h"
#include "../TgsException.h"

namespace Tgs
{

void CslOptions::run(int argc, char** argv)
{
  try
  {
    if(argc == 1)
    {
      throw Tgs::Exception(__LINE__, "At least one argument required");
    }

    std::string option = argv[1];

    if(option == "-basic")
    {
      _runBasicExample(argc, argv);
    }
    else if(option == "-validation")
    {
      _runValidationExample(argc, argv);
    }
    else
    {
      std::stringstream ss;
      ss << "The option " << option << " is not supported.";
      throw Tgs::Exception(__LINE__, ss.str());
    }
  }
  catch(const Tgs::Exception & e)
  {
    throw Tgs::Exception("CslOptions", __FUNCTION__, __LINE__, e);
  }
}

void CslOptions::_runBasicExample(int argc, char** argv)
{
  try
  {
    if(argc != 5)
    {
      std::stringstream ss;
      ss << "USAGE: CSL.bin -basic inputDataFile inputCostFile numberOfTrees";
      throw Tgs::Exception(__LINE__, ss.str());
    }

    std::string inputFile = argv[2];
    std::string costFile = argv[3];
    unsigned int numTrees = atoi(argv[4]);

    QFileInfo fi(inputFile.c_str());
    std::string inputFilebase = fi.baseName().toLatin1().constData() ;

    QFileInfo fi2(costFile.c_str());
    std::string costFilebase = fi2.baseName().toLatin1().constData() ;

    std::string reportBaseName = inputFilebase + "_" + costFilebase;

    RandomForestInputs rfInputs = RandomForestUtilities::generateTrainingDataFromFile(inputFile);
    std::vector<float> costMatrix = CslUtilities::readCostMatrix(costFile);

    if(costMatrix.size() != rfInputs.classLabels.size() * rfInputs.classLabels.size())
    {
      std::stringstream ss;
      ss << "The size of the cost matrix (" << costMatrix.size() <<
        ") does not equal the square of the number of classes (" <<
        rfInputs.classLabels.size() << ")";
      throw Tgs::Exception(__LINE__, ss.str());
    }

    std::cout << "FEATURES:" << std::endl;
    for(unsigned int i = 0; i < rfInputs.featureLabels.size(); i++)
    {
      std::cout << rfInputs.featureLabels[i] << std::endl;
    }
    std::cout << std::endl;

    std::cout << "CLASSES:" << std::endl;
    for(unsigned int i = 0; i < rfInputs.classLabels.size(); i++)
    {
      std::cout << rfInputs.classLabels[i] << std::endl;
    }

    std::cout << std::endl;

    std::map<std::string, std::vector<std::vector<double> > >::iterator trainItr;

    std::cout << "TRAINING INSTANCES" << std::endl;
    for(trainItr = rfInputs.trainingVectors.begin(); trainItr != rfInputs.trainingVectors.end(); ++trainItr)
    {
      std::cout << trainItr->first << " : " << trainItr->second.size() << std::endl;
    }

    std::cout << std::endl;

    //Indicates which percent of data to use for training intial model and then CSL model
    float firstTrainingPercent = 0.33;
    float secondTrainingPercent = 0.33;


    //Generate Training/Testing Ranges
    std::map<std::string, std::pair<unsigned int, unsigned int> > initialTrainingRanges;
    std::map<std::string, std::pair<unsigned int, unsigned int> > cslTrainingRanges;
    std::map<std::string, std::pair<unsigned int, unsigned int> > cslTestingRanges;

    for(trainItr = rfInputs.trainingVectors.begin(); trainItr != rfInputs.trainingVectors.end(); ++trainItr)
    {
      std::string className = trainItr->first;

      initialTrainingRanges[className].first = 0;
      initialTrainingRanges[className].second =
        rfInputs.trainingVectors[className].size() * firstTrainingPercent;

      std::cout << "Initial " << className << " " << initialTrainingRanges[className].first <<
         " : " << initialTrainingRanges[className].second << std::endl;

      cslTrainingRanges[className].first = initialTrainingRanges[className].second;
      cslTrainingRanges[className].second =
        rfInputs.trainingVectors[className].size() * secondTrainingPercent;

      std::cout << "CSL Train " << className << " " << cslTrainingRanges[className].first <<
         " : " << cslTrainingRanges[className].second << std::endl;

      cslTestingRanges[className].first =
        initialTrainingRanges[className].second + cslTrainingRanges[className].second;
      cslTestingRanges[className].second =
        rfInputs.trainingVectors[className].size() - cslTestingRanges[className].first;

      std::cout << "CSL Test " << className << " " << cslTestingRanges[className].first <<
         " : " << cslTestingRanges[className].second << std::endl;
    }

    MultithreadedRandomForestManager cslModel = CostSensitiveLearningTrainer::trainCslModel(rfInputs,
      numTrees, costMatrix, initialTrainingRanges, cslTrainingRanges,
      reportBaseName + "_InitRF");

    std::map<std::string, std::vector<std::vector<double> > >::iterator mapItr;

    unsigned int testObjId = 0;
    //Test next vector set
    //Iterate over vectors by class
    for(mapItr = rfInputs.trainingVectors.begin(); mapItr != rfInputs.trainingVectors.end(); ++mapItr)
    {
      std::string className = mapItr->first;

      unsigned int thirdTrainingIndex = cslTestingRanges[className].first;
      unsigned int thirdTrainingSize = cslTestingRanges[className].second;

      //Iterate over subset of training vectors
      for(unsigned int i = 0; i < thirdTrainingSize; i++)
      {
        std::stringstream objStr;
        objStr << testObjId;
        std::map<std::string, double> scores;

        //Perform initial classification
        cslModel.classifyTestVector(objStr.str(), className,
          mapItr->second[i + thirdTrainingIndex], scores);

        testObjId++;
      }
    }

    cslModel.generateReports(reportBaseName + "_CslRf");

    std::cerr << "RETURNED " << std::endl;
  }
  catch(const Tgs::Exception & e)
  {
    throw Tgs::Exception("CslOptions", __FUNCTION__, __LINE__, e);
  }
}


void CslOptions::_runValidationExample(int argc, char** argv)
{
  try
  {
    if(argc != 4)
    {
      std::stringstream ss;
      ss << "USAGE: CSL.bin -validation inputDataFile costMatrixFile numberOfTrees";
      throw Tgs::Exception(__LINE__, ss.str());
    }

    throw Tgs::Exception(__LINE__, "Not Implemented");

    std::string inputFile = argv[2];
    std::string costFile = argv[3];
    unsigned int numTrees = atoi(argv[4]);

    QFileInfo fi(inputFile.c_str());
    std::string reportBaseName = fi.baseName().toLatin1().constData();

    std::vector<float> costMatrix = CslUtilities::readCostMatrix(costFile);

    CostSensitiveLearningTrainer::cslCrossValidation(inputFile, numTrees, costMatrix,
      reportBaseName);
  }
  catch(const Tgs::Exception & e)
  {
    throw Tgs::Exception("CslOptions", __FUNCTION__, __LINE__, e);
  }
}

}
