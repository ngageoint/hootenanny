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
 * @copyright Copyright (C) 2015, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef __RANDOM_FOREST_MANAGER_H__
#define __RANDOM_FOREST_MANAGER_H__

//STD Includes
#include <fstream>
#include <map>
#include <set>
#include <utility>
#include <vector>

#include <tgs/TgsExport.h>
#include <tgs/RandomForest/BaseRandomForestManager.h>

namespace Tgs
{
  /**
  * This class is to handle tasks external to the actual random
  * forest such as generate result lists and confusion matrices 
  */
  class TGS_EXPORT RandomForestManager : public BaseRandomForestManager
  {
  public:
    /**
    *  Constructor
    */
    RandomForestManager();

    /**
    *  Destructor
    */
    ~RandomForestManager() = default;

  protected:
    /**
     * @brief _initForests initializes the list of random forests
     * @param numForests the number of forests to create
     */
    void _initForests(int numForests) override;

    /**
     * @brief _parseXmlForestNodes loads the XML form of a fores
     * @param forestsNode the list of <RandomForest> DOM nodes
     */
    void _parseXmlForestNodes(QDomNodeList & forestNodes) override;

  };
}
#endif



