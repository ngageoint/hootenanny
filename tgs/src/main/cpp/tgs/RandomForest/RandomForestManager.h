/***************************************************************************
* Copyright (c) 2005-2008 by SPADAC Inc. (formerly Spatial Data Analytics Corporation).  All rights reserved.
****************************************************************************/

#ifndef __RANDOM_FOREST_MANAGER_H__
#define __RANDOM_FOREST_MANAGER_H__

//Boost Includes
#include <boost/shared_ptr.hpp>

//STD Includes
#include <fstream>
#include <map>
#include <set>
#include <utility>
#include <vector>

#include "BaseRandomForestManager.h"
#include "../TgsExport.h"

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
    virtual ~RandomForestManager();

  protected:
    /**
     * @brief _initForests initializes the list of random forests
     * @param numForests the number of forests to create
     */
    virtual void _initForests(int numForests);

    /**
     * @brief _parseXmlForestNodes loads the XML form of a fores
     * @param forestsNode the list of <RandomForest> DOM nodes
     */
    virtual void _parseXmlForestNodes(QDomNodeList & forestNodes);

  };
}
#endif



