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
 * @copyright Copyright (C) 2015 Maxar (http://www.maxar.com/)
 */
#include "CostSensitiveLearningTrainer.h"

//Std Includes
#include <iostream>

#include "MetaCostAlgorithm.h"
#include "../RandomForest/RandomForestUtilities.h"
#include "../TgsException.h"

namespace Tgs
{
  void CostSensitiveLearningTrainer::cslCrossValidation(std::string filename, unsigned int numTrees,
    std::vector<float> & costMatrix, std::string reportName)
  {
    try
    {
      RandomForestInputs rfInputs =
        RandomForestUtilities::generateTrainingDataFromFile(filename);

      std::vector<std::map<std::string, std::pair<unsigned int, unsigned int> > > rangeMaps(3);

      std::map<std::string, std::vector<std::vector<double> > >::iterator mapItr;

      for(mapItr = rfInputs.trainingVectors.begin(); mapItr != rfInputs.trainingVectors.end(); ++mapItr)
      {
        std::string className = mapItr->first;
        rangeMaps[0][className].first = 0;
        rangeMaps[0][className].second = mapItr->second.size() * 0.33;
        rangeMaps[1][className].first = rangeMaps[0][className].second;
        rangeMaps[1][className].second = mapItr->second.size() * 0.33;
        rangeMaps[2][className].first = rangeMaps[1][className].second;
        rangeMaps[2][className].second = mapItr->second.size() - rangeMaps[2][className].first;
      }

      for(unsigned int x = 0; x < 3; x++)
      {
        MultithreadedRandomForestManager initialModel =
          RandomForestUtilities::generateMultithreadModel(rfInputs, numTrees, rangeMaps[x]);

        for(unsigned int y = 0; y < 3; y++)
        {
          if(y != x)
          {
            std::stringstream reportStr;
            reportStr << x << "_" << y;
            MultithreadedRandomForestManager cslModel = CostSensitiveLearningTrainer::trainCslModel(
              initialModel, rfInputs, numTrees, costMatrix, rangeMaps[y], reportStr.str());

            for(unsigned int z = 0; z < 3; z++)
            {
              if(z != x && z != y)
              {
                std::map<std::string, std::vector<std::vector<double> > >::iterator mapItr;

                unsigned int testObjId = 0;
                //Test next vector set
                //Iterate over vectors by class
                for(mapItr = rfInputs.trainingVectors.begin(); mapItr != rfInputs.trainingVectors.end(); ++mapItr)
                {
                  std::string className = mapItr->first;

                  unsigned int thirdTrainingIndex = rangeMaps[z][className].first;
                  unsigned int thirdTrainingSize = rangeMaps[z][className].second;

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

                std::stringstream reportStr;
                reportStr << x << "_" << y << "_" << z;
                cslModel.generateReports(reportName + reportStr.str() + "_CslRf");
              }
            }
          }
        }
      }

    }
    catch(const Tgs::Exception & e)
    {
      throw Tgs::Exception("CostSensitiveLearningTrainer", __FUNCTION__, __LINE__, e);
    }
  }

  MultithreadedRandomForestManager CostSensitiveLearningTrainer::trainCslModel(
    RandomForestInputs & rfInputs, unsigned int numTrees, const std::vector<float> &costMatrix,
    std::map<std::string, std::pair<unsigned int, unsigned int> > &initialTrainingRanges,
    std::map<std::string, std::pair<unsigned int, unsigned int> > &cslTrainingRanges,
    std::string reportsBasename)
  {
    try
    {
      MetaCostAlgorithm mca;
      mca.initialize(rfInputs.classLabels, costMatrix);

      MultithreadedRandomForestManager initialModel =
        RandomForestUtilities::generateMultithreadModel(rfInputs, numTrees, initialTrainingRanges);

      return trainCslModel(initialModel, rfInputs, numTrees, costMatrix, cslTrainingRanges,
        reportsBasename);
    }
    catch(const Tgs::Exception & e)
    {
      throw Tgs::Exception("CostSensitiveLearningTrainer", __FUNCTION__, __LINE__, e);
    }
  }

  MultithreadedRandomForestManager CostSensitiveLearningTrainer::trainCslModel(
    MultithreadedRandomForestManager & initialModel, RandomForestInputs & rfInputs, unsigned int numTrees,
    const std::vector<float> & costMatrix,
    std::map<std::string, std::pair<unsigned int, unsigned int> > & cslTrainingRanges,
    std::string reportsBasename)
  {
    try
    {
      MetaCostAlgorithm mca;
      mca.initialize(rfInputs.classLabels, costMatrix);

      std::vector<double> probVector(rfInputs.classLabels.size());
      std::vector<std::string> cslClasses; //The CSL class label list in the same order as they were tested

      unsigned int testObjId = 0;
      std::map<std::string, std::vector<std::vector<double> > >::iterator mapItr;
      //Test next vector set
      //Iterate over vectors by class
      for(mapItr = rfInputs.trainingVectors.begin(); mapItr != rfInputs.trainingVectors.end(); ++mapItr)
      {
        std::string className = mapItr->first;

        unsigned int secondTrainingIndex = cslTrainingRanges[className].first;
        unsigned int secondTrainingSize = cslTrainingRanges[className].second;

        //Iterate over subset of training vectors
        for(unsigned int i = 0; i < secondTrainingSize; i++)
        {
          std::stringstream objStr;
          objStr << testObjId;
          std::map<std::string, double> scores;

          //Perform initial classification
          initialModel.classifyTestVector(objStr.str(), className,
            mapItr->second[i + secondTrainingIndex], scores);

          double maxProb = -1E10;
          std::string testClass;
          for(unsigned int j = 0; j < rfInputs.classLabels.size(); j++)
          {
            probVector[j] = scores[rfInputs.classLabels[j]];

            if(probVector[j] > maxProb)
            {
              maxProb = probVector[j];
              testClass = rfInputs.classLabels[j];
            }
          }

          std::string newClass = mca.reclassifyVector(probVector, !reportsBasename.empty());

          if(!reportsBasename.empty())
          {
            mca.generateResults(reportsBasename);
          }

          if(testClass != newClass)
          {
  //          std::cerr << "DEBUG LABEL CLASS " << className << " TEST CLASS " << testClass <<
  //            " NEW CLASS " << newClass << " " << testObjId << std::endl;
          }

          cslClasses.push_back(newClass);
          testObjId++;
        }
      }

      if(!reportsBasename.empty())
      {
        initialModel.generateReports(reportsBasename + "initialModel");
      }

      MultithreadedRandomForestManager cslModel;
      cslModel.init(Tgs::BaseRandomForestManager::MULTICLASS, rfInputs.featureLabels);
      testObjId = 0;

      for(mapItr = rfInputs.trainingVectors.begin(); mapItr != rfInputs.trainingVectors.end(); ++mapItr)
      {
        std::string originalClassName = mapItr->first;

        unsigned int secondTrainingIndex = cslTrainingRanges[originalClassName].first;
        unsigned int secondTrainingSize = cslTrainingRanges[originalClassName].second;

        //Iterate over subset of training vectors
        for(unsigned int i = 0; i < secondTrainingSize; i++)
        {
          cslModel.addTrainingVector(cslClasses[testObjId], mapItr->second[i + secondTrainingIndex]);
          testObjId++;
        }
      }

      std::cerr << "DEBUG TRAIN CSL" << std::endl;

      cslModel.train(numTrees);

      std::cerr << "DEBUG TRAINED CSL" << std::endl;

      return cslModel;
    }
    catch(const Tgs::Exception & e)
    {
      throw Tgs::Exception("CostSensitiveLearningTrainer", __FUNCTION__, __LINE__, e);
    }
  }
}

