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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#include "RandomForest.h"

//STD Includes
#include <algorithm>
#include <cmath>
#include <ctime>
#include <fstream>
#include <map>
#include <utility>

#include <tgs/TgsException.h>
#include <tgs/Statistics/Random.h>

using namespace std;

namespace Tgs
{

void RandomForest::trainBinary(const std::shared_ptr<DataFrame>& data, unsigned int numTrees, unsigned int numFactors,
                               const std::string& posClass, unsigned int nodeSize, double retrain, bool balanced)
{
  try
  {
    data->validateData();

    _factorLabels = data->getFactorLabels();
    _forest.clear();
    _numSplitFactors = numFactors;

    if (!data->empty())
    {
      _forest.reserve(numTrees);

      data->setBinaryClassLabels(posClass); //Set all non pos example to negative

      for (unsigned int i = 0; i < numTrees; i++)
      {
        _forest.push_back(std::make_shared<RandomTree>());
        _forest.back()->trainBinary(data, numFactors, posClass, nodeSize, true);
        std::cout << "Trained Tree # " << i + 1 << " of " << numTrees << "     \r";
        std::cout.flush();
      }
      std::cout << std::endl;

      if (retrain >= 0 && retrain < 1.0)
      {
        std::cout << "Retraining model on top " << (retrain * 100) << "% of factors" << std::endl;
        std::map<std::string, double> topFactors;

        getFactorImportance(data, topFactors);

        std::vector<std::string> badFactors;

        unsigned int cutOffIdx = (unsigned int)(topFactors.size() - (unsigned int)((double)topFactors.size() * retrain));

        std::multimap<double, std::string> sortedFactors;

        //Create a map from lowest to highest of important mapped to factor type
        for (auto mapItr = topFactors.begin(); mapItr != topFactors.end(); ++mapItr)
          sortedFactors.insert(std::pair<double, std::string>(mapItr->second, mapItr->first));

        unsigned int cutOffCtr = 0;

        for (auto mMapItr = sortedFactors.begin(); mMapItr != sortedFactors.end(); ++mMapItr)
        {
          if (cutOffCtr < cutOffIdx)
          {
            badFactors.push_back(mMapItr->second);
            cutOffCtr++;
          }
          else
            break;
        }

        for (const auto& factor : badFactors)
          data->deactivateFactor(factor);

        _forest.clear();

        _forest.reserve(numTrees);

        for (unsigned int i = 0; i < numTrees; i++)
        {
          _forest.push_back(std::make_shared<RandomTree>());
          _forest.back()->trainBinary(data, (unsigned int)sqrt((double)(topFactors.size() - cutOffIdx)), posClass, 1 , balanced);
        }
      }

      _forestCreated = true;
      data->restoreClassLabels();  //Restore the original class labels
    }
    else
      throw Exception(__LINE__, "Unable to operate on empty dataset");
  }
  catch(const Exception & e)
  {
    throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
  }
}

void RandomForest::trainMulticlass(const std::shared_ptr<DataFrame>& data, unsigned int numTrees,
                                   unsigned int numFactors, unsigned int nodeSize, double retrain, bool balanced)
{
  try
  {
    data->validateData();

    _factorLabels = data->getFactorLabels();
    _forest.clear();
    numFactors = static_cast<unsigned int>(min<size_t>(data->getActiveFactorCount(), numFactors));
    _numSplitFactors = numFactors;
    cout << _numSplitFactors;

    if (!data->empty())
    {
      _forest.reserve(numTrees);
      //  Reset the ids so that all forests are created alike
      RandomTree::resetIds();

      for (unsigned int i = 0; i < numTrees; i++)
      {
        _forest.push_back(std::make_shared<RandomTree>());
        _forest.back()->trainMulticlass(data, numFactors, nodeSize, true);
        std::cout << "Trained Tree # " << i + 1 << " of " << numTrees << "     \r";
        std::cout.flush();
      }
      std::cout << std::endl;

      //std::cout << "Model Trained " << std::endl;
      if (retrain >= 0 && retrain < 1.0)
      {
        std::cout << "Retraining model on top " << (retrain * 100) << "% of factors" << std::endl;
        std::map<std::string, double> topFactors;

        getFactorImportance(data, topFactors);

        std::vector<std::string> badFactors;

        unsigned int cutOffIdx = (unsigned int)(topFactors.size() - (unsigned int)((double)topFactors.size() * retrain));

        std::multimap<double, std::string> sortedFactors;

        //Create a map from lowest to highest of important mapped to factor type
        for (auto mapItr = topFactors.begin(); mapItr != topFactors.end(); ++mapItr)
          sortedFactors.insert(std::pair<double, std::string>(mapItr->second, mapItr->first));

        unsigned int cutOffCtr = 0;

        for (auto mMapItr = sortedFactors.begin(); mMapItr != sortedFactors.end(); ++mMapItr)
        {
          if (cutOffCtr < cutOffIdx)
          {
            badFactors.push_back(mMapItr->second);
            cutOffCtr++;
          }
          else
            break;
        }

        for (const auto& factor : badFactors)
          data->deactivateFactor(factor);

        _forest.clear();

        _forest.reserve(numTrees);

        for (unsigned int i = 0; i < numTrees; i++)
        {
          _forest.push_back(std::make_shared<RandomTree>());
          _forest.back()->trainMulticlass(data, (unsigned int)sqrt((double)(topFactors.size() - cutOffIdx)), 1, balanced);
        }
      }
      _forestCreated = true;
    }
    else
      throw Exception(__LINE__, "Unable to operate on empty dataset");
  }
  catch(const Exception & e)
  {
    throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
  }
}

void RandomForest::trainRoundRobin(const std::shared_ptr<DataFrame>& data, unsigned int numTrees,
                                   unsigned int numFactors, const std::string& posClass, const std::string& negClass,
                                   unsigned int nodeSize, double retrain, bool balanced)
{
  try
  {
    data->validateData();

    _factorLabels = data->getFactorLabels();
    _forest.clear();
    _numSplitFactors = numFactors;

    if (!data->empty())
    {
      _forest.reserve(numTrees);

      for (unsigned int i = 0; i < numTrees; i++)
      {
        _forest.push_back(std::make_shared<RandomTree>());
        _forest.back()->trainRoundRobin(data, numFactors, posClass, negClass, nodeSize, true);
        std::cout << "Trained Tree # " << i + 1 << " of " << numTrees << "     \r";
        std::cout.flush();
      }
      std::cout << std::endl;

      if (retrain >= 0 && retrain < 1.0)
      {
        std::cout << "Retraining model on top " << (retrain * 100) << "% of factors" << std::endl;
        std::map<std::string, double> topFactors;

        getFactorImportance(data, topFactors);

        std::vector<std::string> badFactors;

        unsigned long cutOffIdx = topFactors.size() - (unsigned int)((double)topFactors.size() * retrain);

        std::multimap<double, std::string> sortedFactors;

        //Create a map from lowest to highest of important mapped to factor type
        for (auto mapItr = topFactors.begin(); mapItr != topFactors.end(); ++mapItr)
          sortedFactors.insert(std::pair<double, std::string>(mapItr->second, mapItr->first));

        unsigned int cutOffCtr = 0;

        for (auto mMapItr = sortedFactors.begin(); mMapItr != sortedFactors.end(); ++mMapItr)
        {
          if (cutOffCtr < cutOffIdx)
          {
            badFactors.push_back(mMapItr->second);
            cutOffCtr++;
          }
          else
            break;
        }

        for (const auto& factor : badFactors)
          data->deactivateFactor(factor);

        _forest.clear();

        _forest.reserve(numTrees);

        for (unsigned int i = 0; i < numTrees; i++)
        {
          _forest.push_back(std::make_shared<RandomTree>());
          _forest.back()->trainMulticlass(data, (unsigned int)sqrt((double)(topFactors.size() - cutOffIdx)), 1, balanced);
        }
      }

      _forestCreated = true;
    }
    else
      throw Exception(__LINE__, "Unable to operate on empty dataset");
  }
  catch(const Exception & e)
  {
    throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
  }
}

}  //End namespace
