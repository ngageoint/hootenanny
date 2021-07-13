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
#include "MultithreadedRandomForestManager.h"

//Qt Includes
#include <QDomDocument>
#include <QFile>
#include <QStringList>
#include <QTextStream>

namespace Tgs
{

  void MultithreadedRandomForestManager::_initForests(int numForests)
  {
    try
    {
      for (unsigned int i = 0; i < (unsigned int)numForests; i++)
      {
        _rfList.push_back(std::make_shared<MultithreadedRandomForest>());
      }
    }
    catch(const Exception & e)
    {
      throw Tgs::Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void MultithreadedRandomForestManager::_parseXmlForestNodes(QDomNodeList & forestNodes)
  {
    try
    {
      _rfList.clear();

      for (unsigned int fIdx = 0; fIdx < (unsigned int)forestNodes.size(); fIdx++)
      {
        QDomElement forestElement = forestNodes.at(fIdx).toElement();
        _rfList.push_back(std::make_shared<MultithreadedRandomForest>());
        _rfList.back()->importModel(forestElement);
      }
    }
    catch(const Exception & e)
    {
      throw Tgs::Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void MultithreadedRandomForestManager::_trainBinary(unsigned int /*numTrees*/,
    unsigned int /*numFactors*/, unsigned int /*nodeSize*/, double /*retrain*/, bool /*balanced*/)
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

  void MultithreadedRandomForestManager::_trainMultiClass(unsigned int numTrees,
    unsigned int numFactors, unsigned int nodeSize, double retrain, bool balanced)
  {
    try
    {
      std::cerr << "DEBUG _TRAIN MT" << std::endl;
      _rfList.clear();
      _rfList.push_back(std::make_shared<MultithreadedRandomForest>());
      _rfList[0]->trainMulticlass(_data, numTrees, numFactors, nodeSize, retrain, balanced);
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void MultithreadedRandomForestManager::_trainRoundRobin(unsigned int /*numTrees*/,
    unsigned int /*numFactors*/, unsigned int /*nodeSize*/, double /*retrain*/, bool /*balanced*/)
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

