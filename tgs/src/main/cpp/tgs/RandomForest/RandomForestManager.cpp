/***************************************************************************
* Copyright (c) 2005-2008 by SPADAC Inc. (formerly Spatial Data Analytics Corporation).  All rights reserved.
****************************************************************************/

#include "RandomForestManager.h"
#include "RandomForest.h"
//#include "InternalRandomForestManager.h"

//STD Includes
#include <exception>
#include <fstream>

namespace Tgs
{
  RandomForestManager::RandomForestManager()
  {
    try
    {

    }
    catch(const Exception & e)
    {
      throw Tgs::Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }

  }

  RandomForestManager::~RandomForestManager()
  {

  }

  void RandomForestManager::_initForests(int numForests)
  {
    try
    {
      for(unsigned int i = 0; i < numForests; i++)
      {
        _rfList.push_back(boost::shared_ptr<RandomForest>(new RandomForest()));
      }
    }
    catch(const Exception & e)
    {
      throw Tgs::Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void RandomForestManager::_parseXmlForestNodes(QDomNodeList & forestNodes)
  {
    try
    {
      _rfList.clear();

      for(unsigned int fIdx = 0; fIdx < forestNodes.size(); fIdx++)
      {
        QDomElement forestElement = forestNodes.at(fIdx).toElement();
        _rfList.push_back(
          boost::shared_ptr<RandomForest>(new RandomForest()));
        _rfList.back()->importModel(forestElement);
      }
    }
    catch(const Exception & e)
    {
      throw Tgs::Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

}  //End namespace
