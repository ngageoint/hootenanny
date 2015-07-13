#include "MultithreadedRandomForestManager.h"

//Qt Includes
#include <QDomDocument>
#include <QFile>
#include <QStringList>
#include <QTextStream>

namespace Tgs
{


  MultithreadedRandomForestManager::MultithreadedRandomForestManager()
  {

  }

  MultithreadedRandomForestManager::~MultithreadedRandomForestManager(){}


  void MultithreadedRandomForestManager::_initForests(int numForests)
  {
    try
    {
      for(unsigned int i = 0; i < numForests; i++)
      {
        _rfList.push_back(
          boost::shared_ptr<MultithreadedRandomForest>(new MultithreadedRandomForest()));
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

      for(unsigned int fIdx = 0; fIdx < forestNodes.size(); fIdx++)
      {
        QDomElement forestElement = forestNodes.at(fIdx).toElement();
        _rfList.push_back(
          boost::shared_ptr<MultithreadedRandomForest>(new MultithreadedRandomForest()));
        _rfList.back()->importModel(forestElement);
      }
    }
    catch(const Exception & e)
    {
      throw Tgs::Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void MultithreadedRandomForestManager::_trainBinary(unsigned int numTrees,
    unsigned int numFactors, unsigned int nodeSize, double retrain, bool balanced)
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
      _rfList.push_back(
        boost::shared_ptr<MultithreadedRandomForest>(new MultithreadedRandomForest()));
      _rfList[0]->trainMulticlass(_data, numTrees, numFactors, nodeSize, retrain, balanced);
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void MultithreadedRandomForestManager::_trainRoundRobin(unsigned int numTrees,
    unsigned int numFactors, unsigned int nodeSize, double retrain, bool balanced)
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

