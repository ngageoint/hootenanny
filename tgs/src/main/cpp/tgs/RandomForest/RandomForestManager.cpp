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
 * @copyright Copyright (C) 2015, 2018, 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#include "RandomForestManager.h"

#include <tgs/RandomForest/RandomForest.h>

//STD Includes
#include <exception>
#include <fstream>

namespace Tgs
{
  void RandomForestManager::_initForests(int numForests)
  {
    try
    {
      for (unsigned int i = 0; i < (unsigned int)numForests; i++)
      {
        _rfList.push_back(std::make_shared<RandomForest>());
      }
    }
    catch (const Exception & e)
    {
      throw Tgs::Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void RandomForestManager::_parseXmlForestNodes(QDomNodeList & forestNodes)
  {
    try
    {
      _rfList.clear();

      for (unsigned int fIdx = 0; fIdx < (unsigned int)forestNodes.size(); fIdx++)
      {
        QDomElement forestElement = forestNodes.at(fIdx).toElement();
        _rfList.push_back(std::make_shared<RandomForest>());
        _rfList.back()->importModel(forestElement);
      }
    }
    catch(const Exception & e)
    {
      throw Tgs::Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }
}
