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
 * @copyright Copyright (C) 2015, 2018, 2019 Maxar (http://www.maxar.com/)
 */
#include "MultithreadedRandomForest.h"

//Std Includes
#include <cmath>

//Qt Includes
#include <QtConcurrent/QtConcurrent>
#include <QThreadPool>


#include <tgs/TgsException.h>

namespace Tgs
{


  MultithreadedRandomForest::MultithreadedRandomForest()
  {
    try
    {

    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  std::shared_ptr<RandomTree> MultithreadedRandomForest::train(const std::shared_ptr<RandomTree>& tree)
  {
    try
    {
      tree->trainMulticlass(_trainInputs.data, _trainInputs.numFactors, _trainInputs.nodeSize,
        _trainInputs.balanced);

      return tree;
    }
    catch(const Exception & e)
    {
      throw Exception("MultithreadedRandomForest", __FUNCTION__, __LINE__, e);
    }
  }

  void MultithreadedRandomForest::trainBinary(const std::shared_ptr<DataFrame>& /*data*/,
    unsigned int /*numTrees*/, unsigned int /*numFactors*/, std::string /*posClass*/, unsigned int /*nodeSize*/,
    double /*retrain*/, bool /*balanced*/)
  {
    try
    {
      throw Exception(__LINE__, "Not implemented");
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void MultithreadedRandomForest::trainMulticlass(const std::shared_ptr<DataFrame>& data,
    unsigned int numTrees, unsigned int numFactors, unsigned int nodeSize, double retrain,
    bool balanced)
  {
    try
    {
      std::cerr << "DEBUG MTRF TRAINMULTI" << std::endl;

      data->validateData();

      _trainInputs.data = data;
      _trainInputs.numFactors = numFactors;
      _trainInputs.nodeSize = nodeSize;
      _trainInputs.balanced = balanced;

      _factorLabels = data->getFactorLabels();
      _forest.clear();
      numFactors = std::min<size_t>(data->getActiveFactorCount(), numFactors);
      _numSplitFactors = numFactors;

      if (!data->empty())
      {
        _forest.reserve(numTrees);

        //Initialize the thread pool
        QThreadPool pool;
        pool.setExpiryTimeout(-1);

        QList<std::shared_ptr<RandomTree>> mapTrees;
        for (unsigned int i = 0; i < numTrees; i++)
        {
          mapTrees.append(std::shared_ptr<RandomTree>(new RandomTree()));
        }

        QList<std::shared_ptr<RandomTree>> forestList =
          QtConcurrent::blockingMapped(mapTrees, train);

        for (unsigned int i = 0; i < numTrees; i++)
        {
          _forest.push_back(forestList[i]);
        }

        if (retrain >= 0 && retrain < 1.0)
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
          for (mapItr = topFactors.begin(); mapItr != topFactors.end(); ++mapItr)
          {
            sortedFactors.insert(std::pair<double, std::string>(mapItr->second, mapItr->first));
          }

          unsigned int cutOffCtr = 0;

          for (mMapItr = sortedFactors.begin(); mMapItr != sortedFactors.end(); ++mMapItr)
          {
            if (cutOffCtr <  cutOffIdx)
            {
              badFactors.push_back(mMapItr->second);
              cutOffCtr++;
            }
            else
            {
              break;
            }
          }

          for (unsigned int i = 0; i < badFactors.size(); i++)
          {
            data->deactivateFactor(badFactors[i]);
          }

          _forest.clear();

          _forest.reserve(numTrees);

          _trainInputs.numFactors = (unsigned int)sqrt((double)(topFactors.size() - cutOffIdx));
          _trainInputs.nodeSize = 1;

          QList<std::shared_ptr<RandomTree>> mapRetrainingTrees;
          for (unsigned int i = 0; i < numTrees; i++)
          {
             mapRetrainingTrees.append(std::shared_ptr<RandomTree>(new RandomTree()));
          }

          QList<std::shared_ptr<RandomTree>> forestRetrainList =
            QtConcurrent::blockingMapped(mapRetrainingTrees, train);

          for (unsigned int i = 0; i < numTrees; i++)
          {
            _forest[i] = forestRetrainList[i];
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

  void MultithreadedRandomForest::trainRoundRobin(const std::shared_ptr<DataFrame>& /*data*/,
    unsigned int /*numTrees*/, unsigned int /*numFactors*/, std::string /*posClass*/, std::string /*negClass*/,
    unsigned int /*nodeSize*/, double /*retrain*/, bool /*balanced*/)
  {
    try
    {
      throw Exception(__LINE__, "Not implemented");
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }
}

