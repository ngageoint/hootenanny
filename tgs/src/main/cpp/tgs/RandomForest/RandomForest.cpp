/***************************************************************************
* Copyright (c) 2005-2008 by SPADAC Inc. (formerly Spatial Data Analytics Corporation).  All rights reserved.
****************************************************************************/

#include "RandomForest.h"

//STD Includes
#include <algorithm>
#include <cmath>
#include <ctime>
#include <fstream>
#include <map>
#include <utility>

#include "../TgsException.h"

namespace Tgs
{
  RandomForest::RandomForest()
  {
    //Set the random seed
    //clock_t t1 = clock();
    //srand((unsigned int)t1);
  }

  RandomForest::~RandomForest()
  {
   
  }

  void RandomForest::trainBinary(boost::shared_ptr<DataFrame> data, unsigned int numTrees,
    unsigned int numFactors, std::string posClass, unsigned int nodeSize, double retrain,
    bool balanced)
  {
    try
    {
      data->validateData();

      _factorLabels = data->getFactorLabels();
      _forest.clear();
      _numSplitFactors = numFactors;

      if(!data->empty())
      {
        _forest.reserve(numTrees);

        data->setBinaryClassLabels(posClass); //Set all non pos example to negative

        for(unsigned int i = 0; i < numTrees; i++)
        {
          _forest.push_back(boost::shared_ptr<RandomTree>(new RandomTree()));
          _forest.back()->trainBinary(data, numFactors, posClass, nodeSize, true);
          std::cout << "Trained Tree # " << i + 1 << " / " << numTrees << "     \r";
          std::cout.flush();
        }
        std::cout << std::endl;

        //std::cout << "Mode Trained " << std::endl;
        if(retrain >= 0 && retrain < 1.0)
        {
          std::cout << "Retraining model on top " << (retrain * 100) << "% of factors" << std::endl;
          std::map<std::string, double> topFactors;
          std::map<std::string, double>::iterator mapItr;

          getFactorImportance(data, topFactors);

          std::vector<std::string> badFactors;

          unsigned int cutOffIdx = topFactors.size() - (unsigned int)((double)topFactors.size() * retrain);

          std::multimap<double, std::string> sortedFactors;
          std::multimap<double, std::string>::iterator mMapItr;

          //Create a map from lowest to highest of important mapped to factor type
          for(mapItr = topFactors.begin(); mapItr != topFactors.end(); ++mapItr)
          {
            sortedFactors.insert(std::pair<double, std::string>(mapItr->second, mapItr->first));
          }

          unsigned int cutOffCtr = 0;

          for(mMapItr = sortedFactors.begin(); mMapItr != sortedFactors.end(); ++mMapItr)
          {
            if(cutOffCtr <  cutOffIdx)
            {
              badFactors.push_back(mMapItr->second);
              cutOffCtr++;
            }
            else
            {
              break;
            }
          }

          for(unsigned int i = 0; i < badFactors.size(); i++)
          {
            data->deactivateFactor(badFactors[i]);
          }

          _forest.clear();

          _forest.reserve(numTrees);

          for(unsigned int i = 0; i < numTrees; i++)
          {
            _forest.push_back(boost::shared_ptr<RandomTree>(new RandomTree()));
            _forest.back()->trainBinary(data,
              (unsigned int)sqrt((double)(topFactors.size() - cutOffIdx)), posClass, 1 , balanced);
          }
        }

        _forestCreated = true;
        data->restoreClassLabels();  //Restore the original class labels
      }
      else
      {
        throw Exception(__LINE__, "Unable to operate on empty dataset");
      }
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void RandomForest::trainMulticlass(boost::shared_ptr<DataFrame> data, unsigned int numTrees,
    unsigned int numFactors, unsigned int nodeSize, double retrain, bool balanced)
  {
    try
    {
      data->validateData();

      _factorLabels = data->getFactorLabels();
      _forest.clear();
      numFactors = min<size_t>(data->getActiveFactorCount(), numFactors);
      _numSplitFactors = numFactors;
      cout << _numSplitFactors;

      if(!data->empty())
      {
        _forest.reserve(numTrees);

        for(unsigned int i = 0; i < numTrees; i++)
        {
          _forest.push_back(boost::shared_ptr<RandomTree>(new RandomTree()));
          _forest.back()->trainMulticlass(data, numFactors, nodeSize, true);
          std::cout << "Trained Tree # " << i + 1 << " / " << numTrees << "     \r";
          std::cout.flush();
        }
        std::cout << std::endl;

        //std::cout << "Mode Trained " << std::endl;
        if(retrain >= 0 && retrain < 1.0)
        {
          std::cout << "Retraining model on top " << (retrain * 100) << "% of factors" << std::endl;
          std::map<std::string, double> topFactors;
          std::map<std::string, double>::iterator mapItr;

          getFactorImportance(data, topFactors);

          std::vector<std::string> badFactors;

          unsigned int cutOffIdx =
            topFactors.size() - (unsigned int)((double)topFactors.size() * retrain);

          std::multimap<double, std::string> sortedFactors;
          std::multimap<double, std::string>::iterator mMapItr;

          //Create a map from lowest to highest of important mapped to factor type
          for(mapItr = topFactors.begin(); mapItr != topFactors.end(); ++mapItr)
          {
            sortedFactors.insert(std::pair<double, std::string>(mapItr->second, mapItr->first));
          }

          unsigned int cutOffCtr = 0;

          for(mMapItr = sortedFactors.begin(); mMapItr != sortedFactors.end(); ++mMapItr)
          {
            if(cutOffCtr <  cutOffIdx)
            {
              badFactors.push_back(mMapItr->second);
              cutOffCtr++;
            }
            else
            {
              break;
            }
          }

          for(unsigned int i = 0; i < badFactors.size(); i++)
          {
            data->deactivateFactor(badFactors[i]);
          }

          _forest.clear();

          _forest.reserve(numTrees);

          for(unsigned int i = 0; i < numTrees; i++)
          {
            _forest.push_back(boost::shared_ptr<RandomTree>(new RandomTree()));
            _forest.back()->trainMulticlass(data,
              (unsigned int)sqrt((double)(topFactors.size() - cutOffIdx)), 1, balanced);
          }
        }

        _forestCreated = true;
      }
      else
      {
        throw Exception(__LINE__, "Unable to operate on empty dataset");
      }
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void RandomForest::trainRoundRobin(boost::shared_ptr<DataFrame> data, unsigned int numTrees,
    unsigned int numFactors, std::string posClass, std::string negClass, unsigned int nodeSize,
    double retrain, bool balanced)
  {
    try
    {
      data->validateData();

      _factorLabels = data->getFactorLabels();
      _forest.clear();
      _numSplitFactors = numFactors;

      if(!data->empty())
      {
        _forest.reserve(numTrees);

        for(unsigned int i = 0; i < numTrees; i++)
        {
          _forest.push_back(boost::shared_ptr<RandomTree>(new RandomTree()));
          _forest.back()->trainRoundRobin(data, numFactors, posClass, negClass, nodeSize, true);
          std::cout << "Trained Tree # " << i + 1 << " / " << numTrees << "     \r";
          std::cout.flush();
        }
        std::cout << std::endl;

       //std::cout << "Mode Trained " << std::endl;
        if(retrain >= 0 && retrain < 1.0)
        {
          std::cout << "Retraining model on top " << (retrain * 100) << "% of factors" << std::endl;
          std::map<std::string, double> topFactors;
          std::map<std::string, double>::iterator mapItr;

          getFactorImportance(data, topFactors);

          std::vector<std::string> badFactors;

          unsigned int cutOffIdx =
            topFactors.size() - (unsigned int)((double)topFactors.size() * retrain);

          std::multimap<double, std::string> sortedFactors;
          std::multimap<double, std::string>::iterator mMapItr;

          //Create a map from lowest to highest of important mapped to factor type
          for(mapItr = topFactors.begin(); mapItr != topFactors.end(); ++mapItr)
          {
            sortedFactors.insert(std::pair<double, std::string>(mapItr->second, mapItr->first));
          }

          unsigned int cutOffCtr = 0;

          for(mMapItr = sortedFactors.begin(); mMapItr != sortedFactors.end(); ++mMapItr)
          {
            if(cutOffCtr <  cutOffIdx)
            {
              badFactors.push_back(mMapItr->second);
              cutOffCtr++;
            }
            else
            {
              break;
            }
          }

          for(unsigned int i = 0; i < badFactors.size(); i++)
          {
            data->deactivateFactor(badFactors[i]);
          }

          _forest.clear();

          _forest.reserve(numTrees);

          for(unsigned int i = 0; i < numTrees; i++)
          {
            _forest.push_back(boost::shared_ptr<RandomTree>(new RandomTree()));
            _forest.back()->trainMulticlass(data,
              (unsigned int)sqrt((double)(topFactors.size() - cutOffIdx)), 1, balanced);
          }
        }

        _forestCreated = true;
      }
      else
      {
        throw Exception(__LINE__, "Unable to operate on empty dataset");
      }
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }
}  //End namespace



