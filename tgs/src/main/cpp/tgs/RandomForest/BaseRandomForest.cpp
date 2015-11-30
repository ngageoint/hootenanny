#include "BaseRandomForest.h"

//Qt Includes
#include <QStringList>

//Std Includes
#include <cmath>

#include "../TgsException.h"

namespace Tgs
{
  TrainingInputs BaseRandomForest::_trainInputs;

  BaseRandomForest::BaseRandomForest() : _numSplitFactors(0), _forestCreated(false)
  {

  }

  BaseRandomForest::~BaseRandomForest(){}

  void BaseRandomForest::clear()
  {
    try
    {
      _forest.clear();
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void BaseRandomForest::classifyVector(std::vector<double> & dataVector,
    std::map<std::string, double> & scores) const
  {
    try
    {
      double itrVal = 1.0/(double)_forest.size();

      for(unsigned int i = 0; i < _forest.size(); i++)
      {
        std::string result;
        _forest[i]->classifyDataVector(dataVector, result);
        scores[result] += itrVal;
      }
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void BaseRandomForest::exportModel(QDomDocument & modelDoc, QDomElement & parentNode)
  {
    try
    {
      QDomElement forestNode = modelDoc.createElement("RandomForest");

      //Append number of trees
      QDomElement numTreesNode = modelDoc.createElement("NumTrees");
      QDomText numTreesText = modelDoc.createTextNode(QString::number(_forest.size()));
      numTreesNode.appendChild(numTreesText);
      forestNode.appendChild(numTreesNode);

      //Append number of split factors
      QDomElement numSplitFactorsNode = modelDoc.createElement("NumSplitFactors");
      QDomText numSplitFactorText = modelDoc.createTextNode(QString::number(_numSplitFactors));
      numSplitFactorsNode.appendChild(numSplitFactorText);
      forestNode.appendChild(numSplitFactorsNode);

      //Append factor labels
      QDomElement factorLabelsNode = modelDoc.createElement("FactorLabels");
      std:stringstream labelStream;

      for (size_t i = 0; i < _factorLabels.size(); ++i)
      {
        labelStream << _factorLabels[i];

        if(i != _factorLabels.size() - 1)
        {
          labelStream << " ";
        }
      }

      QDomText factorLabelsText = modelDoc.createTextNode(labelStream.str().c_str());
      factorLabelsNode.appendChild(factorLabelsText);
      forestNode.appendChild(factorLabelsNode);

      //Append Trees
      QDomElement randomTreesNode = modelDoc.createElement("RandomTrees");

      for(unsigned int i = 0; i < _forest.size(); i++)
      {
        _forest[i]->exportTree(modelDoc, randomTreesNode);
      }

      forestNode.appendChild(randomTreesNode);
      parentNode.appendChild(forestNode);
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void BaseRandomForest::findAverageError(boost::shared_ptr<DataFrame> data, double & average,
    double & stdDev)
  {
    try
    {
      if(isTrained())
      {
        double errorSum = 0;
        double errorSumSqr = 0;
        double variance;

        for(unsigned int i = 0; i < _forest.size(); i++)
        {
          double errRate = _forest[i]->computeErrorRate(data);
          errorSum += errRate;
          errorSumSqr += errRate * errRate;

        }

        average = errorSum / ((double)_forest.size());
        variance = errorSumSqr / ((double)_forest.size()) - average * average;
        stdDev = sqrt(variance);
      }
      else
      {
        throw Exception(__LINE__, "Forest has not been trained");
      }
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void BaseRandomForest::findProximity(boost::shared_ptr<DataFrame> data,
    std::vector<unsigned int> & proximity)
  {
    try
    {
      unsigned int dSize = data->getNumDataVectors();

      if(_forestCreated  && dSize > 0)
      {
        proximity.resize(dSize * dSize);
        std::fill(proximity.begin(), proximity.end(), 0);

        for(unsigned int i = 0; i < _forest.size(); i++)
        {
          _forest[i]->findProximity(data, proximity);
        }
      }
      else
      {
        throw Exception(__LINE__, "Forest has not been trained");
      }
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void BaseRandomForest::getFactorImportance(boost::shared_ptr<DataFrame> data,
    std::map<std::string, double> & factorImportance)
  {
    try
    {
      std::map<unsigned int, double>::iterator itr;
      std::vector<std::string> factorLabels = data->getFactorLabels();

      //Init factor importance map with all factors
      for(unsigned int j = 0; j < factorLabels.size(); j++)
      {
        factorImportance[factorLabels[j]] = 0;
      }

      //Calc factor importance for each tree in forest
      //and aggregate the results
      for(unsigned int i = 0; i < _forest.size(); i++)
      {
        std::map<unsigned int, double> factPureMap;
        _forest[i]->getFactorImportance(factPureMap);

        for(itr = factPureMap.begin(); itr != factPureMap.end(); ++itr)
        {
          factorImportance[factorLabels[itr->first]] += itr->second;
        }
      }
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void BaseRandomForest::importModel(QDomElement & e)
  {
    try
    {
      QDomNodeList childList = e.childNodes();

      for (unsigned int i = 0; i < (unsigned int)childList.size(); i++)
      {
        if(childList.at(i).nodeType() == QDomNode::CommentNode)
        {
          continue;
        }

        if(childList.at(i).isElement())
        {
          QDomElement e = childList.at(i).toElement(); // try to convert the node to an element.

          QString tag = e.tagName().toUpper();

          bool parseOkay = true;

          if(tag == "NUMTREES")
          {
            unsigned int numTrees = e.text().toUInt(&parseOkay);
            _forest.reserve(numTrees);
          }
          else if(tag == "NUMSPLITFACTORS")
          {
            _numSplitFactors = e.text().toUInt(&parseOkay);
          }
          else if(tag == "FACTORLABELS")
          {
            QStringList factorList = e.text().split(" ");

            for (unsigned int fIdx = 0; fIdx < (unsigned int)factorList.size(); fIdx++)
            {
              _factorLabels.push_back(factorList[fIdx].toLatin1().constData());
            }
          }
          else if(tag == "RANDOMTREES")
          {
            QDomNodeList treeList = e.childNodes();

            for (unsigned int rIdx = 0; rIdx < (unsigned int)treeList.size(); rIdx++)
            {
              QDomElement treeElement = treeList.at(rIdx).toElement();

              _forest.push_back(boost::shared_ptr<RandomTree>(new RandomTree()));
              _forest.back()->import(treeElement);
            }
          }
          else if (tag == "RANDOMTREE") //for hoot backward compatibility
          {
            _forest.push_back(boost::shared_ptr<RandomTree>(new RandomTree()));
            _forest.back()->import(e);
          }
          else
          {
            std::stringstream ss;
            ss << "The tag " << tag.toLatin1().constData() <<
              " is not supported within the <RandomForest> tag";
            throw Exception(__LINE__, ss.str());
          }

          if(!parseOkay)
          {
            std::stringstream ss;
            ss << "Unable to parse tag " << tag.toLatin1().constData() << " with value " <<
              e.text().toLatin1().constData();
          }
        }
      }
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }
}
