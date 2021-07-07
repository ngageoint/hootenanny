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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */

#include "DataFrame.h"

//Qt Includes
#include <QStringList>
#include <qnumeric.h>

//STD Includes
#include <algorithm>
#include <assert.h>
#include <cmath>
#include <ctime>
#include <iostream>
#include <float.h>
#include <fstream>
#include <limits>
#include <sstream>
#include <stdexcept>

// Tgs
#include <tgs/TgsException.h>
#include <tgs/Statistics/Random.h>

using namespace std;

namespace Tgs
{
  /**
  * Constructor
  */
  DataFrame::DataFrame() 
  {
    try
    {
      _trainingEnumCnt = 1;
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  DataFrame::DataFrame(const DataFrame& from)
  {
    try
    {
      *this = from;
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void DataFrame::addDataVector(std::string label, const std::vector<double>& dataItem,
    double eventWeight /*= 1.*/)
  {
    try
    {
      _data.push_back(dataItem);         //Append a new training vector
      _weights.push_back(eventWeight);        //Append the event weight
      _trainingLabels.push_back(label);  //Store its training class label
      _classSet.insert(label);           //Keep a set of unique class names
      if (_trainingLabelEnum.find(label) == _trainingLabelEnum.end())
      {
        _trainingLabelEnum[label] = _trainingEnumCnt++;
      }
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void DataFrame::addDataVector(std::string label, const double* dataItem,
    double eventWeight /*= 1.*/)
  {
    try
    {
      vector<double> d(dataItem, dataItem + getFactorTypes().size());
      addDataVector(label, d, eventWeight);
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void DataFrame::clear()
  {
    try
    {
      _classSet.clear();
      _data.clear();
      _weights.clear();
      _factorLabels.clear();
      _trainingLabels.clear();
      _trainingLabelsBak.clear();
      _activeFactorIndices.clear();
      _factorType.clear();
      _trainingLabelEnum.clear();
      _nullTreatment.clear();
      _medianMaps.clear();
      _trainingEnumCnt = 1;

    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  double DataFrame::computeBandwidthByFactor(unsigned int fIdx,
    std::vector<unsigned int> & dataIndices, double & minVal, double & maxVal, double & mean,
    double & q1, double & q3)
  {
    try
    {
      if (!dataIndices.empty() && fIdx < _factorLabels.size())
      {
        sortIndicesOnFactorValue(dataIndices, fIdx);

        minVal = _data[dataIndices[0]][fIdx];
        maxVal = _data[dataIndices[dataIndices.size() - 1]][fIdx];

        double variance = 0;
        double sum = 0;
        double sumsqr = 0;

        mean = 0;
        for (unsigned int i = 0; i < dataIndices.size(); i++)
        {
          sum += _data[dataIndices[i]][fIdx];
          sumsqr += _data[dataIndices[i]][fIdx] * _data[dataIndices[i]][fIdx];
        }

        mean = sum / (double)dataIndices.size();
        variance = (sumsqr - sum * mean) / ((double)dataIndices.size() - 1.0);

        double q25 = _data[dataIndices[(unsigned int)((double)dataIndices.size() * 0.25)]][fIdx];
        double q75 = _data[dataIndices[(unsigned int)((double)dataIndices.size() * 0.75)]][fIdx];
        q1 = q25;
        q3 = q75;
        double range = q75 - q25;
        double h = range / 1.34;

        double silv = 1.06 * pow((double)dataIndices.size(), -0.2);

        if (sqrt(variance) < h)
        {
          silv *= sqrt(variance);
        }
        else
        {
          silv *= h;
        }

        //std::cout << "FIdx " << fIdx << " Name " << getFactorLabelFromIndex(fIdx) << " Var " << variance << " Silv " << silv << " Min " << minVal << " Max " << maxVal <<  " Mean " << mean << std::endl;
        return silv;
      }
      else
      {
        throw Exception(__LINE__, "Either empty dataset or invalid factor index");
      }
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void DataFrame::deactivateFactor(std::string factor)
  {
    try
    {
      //Look up the index for the factor name
      for (unsigned int i = 0; i < _factorLabels.size(); i++)
      {
        if (_factorLabels[i] == factor)
        {
          //Find the index in the _activeFactorIndices list and remove it
          for (unsigned int k = 0; k < _activeFactorIndices.size(); k++)
          {
            if (_activeFactorIndices[k] == i)
            {
              _activeFactorIndices.erase(_activeFactorIndices.begin() + k);
              break;
            }
          }

          break;
        }
      }
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void DataFrame::exportData(QDomDocument & modelDoc, QDomElement & dataFrameNode) const
  {
    try
    {
      //Add Factor Labels
      QDomElement factorLabelNode = modelDoc.createElement("FactorLabels");
      modelDoc.appendChild(factorLabelNode);
      std::stringstream factorStream;

      for (unsigned int i = 0; i < _factorLabels.size(); i++)
      {
        factorStream <<  _factorLabels[i];

        if (i != _factorLabels.size() - 1)
        {
          factorStream << " ";
        }
      }

      QDomText factorLabelText = modelDoc.createTextNode(factorStream.str().c_str());
      factorLabelNode.appendChild(factorLabelText);
      dataFrameNode.appendChild(factorLabelNode);

      //Export Factors Data Type
      if (!_factorType.empty())
      {
        QDomElement factorTypeNode = modelDoc.createElement("FactorTypes");
        modelDoc.appendChild(factorTypeNode);
        std::stringstream factorTypeStream;

        for (unsigned int i = 0; i < _factorType.size(); i++)
        {
          factorTypeStream <<  _factorType[i];

          if (i != _factorType.size() - 1)
          {
            factorTypeStream << " ";
          }
        }

        QDomText factorTypeText = modelDoc.createTextNode(factorTypeStream.str().c_str());
        factorTypeNode.appendChild(factorTypeText);
        dataFrameNode.appendChild(factorTypeNode);

      }

      //Export NULL Treatment
      if (!_nullTreatment.empty())
      {
        QDomElement factorNullNode = modelDoc.createElement("FactorNullTreatment");
        modelDoc.appendChild(factorNullNode);
        std::stringstream factorNullStream;

        for (unsigned int i = 0; i < _nullTreatment.size(); i++)
        {
          factorNullStream << _nullTreatment[i];

          if (i != _nullTreatment.size() - 1)
          {
            factorNullStream << " ";
          }
        }

        QDomText factorNullText = modelDoc.createTextNode(factorNullStream.str().c_str());
        factorNullNode.appendChild(factorNullText);
        dataFrameNode.appendChild(factorNullNode);
      }

      //Export Missing Data Values

      if (!_medianMaps.empty())
      {
        QDomElement medianValuesNode = modelDoc.createElement("MedianValues");
        modelDoc.appendChild(medianValuesNode);

        std::map<std::string, double>::iterator medianItr;

        for (unsigned int i = 0; i < _medianMaps.size(); i++)
        {
          QDomElement medianValuesByFactorNode = modelDoc.createElement("MedianValuesByFactor");
          modelDoc.appendChild(medianValuesByFactorNode);

          std::map<std::string, double> factorMedianMap = _medianMaps[i];

          for (medianItr = factorMedianMap.begin(); medianItr != factorMedianMap.end(); ++medianItr)
          {
            QDomElement classMedianNode = modelDoc.createElement("ClassMedian");
            modelDoc.appendChild(classMedianNode);

            std::stringstream medianStream;
            medianStream << medianItr->first << " " << medianItr->second;
            QDomText classMedianText = modelDoc.createTextNode(medianStream.str().c_str());
            classMedianNode.appendChild(classMedianText);
            medianValuesByFactorNode.appendChild(classMedianNode);
          }

          medianValuesNode.appendChild(medianValuesByFactorNode);
        }

        dataFrameNode.appendChild(medianValuesNode);
      }

      //Create DataVectors node
      QDomElement dataVectorsNode = modelDoc.createElement("DataVectors");
      modelDoc.appendChild(dataVectorsNode);

      //Export data vectors as trainingClass and data values
      for (unsigned int i = 0; i < _data.size(); i++)
      {
        QDomElement vectorNode = modelDoc.createElement("DataVector");
        modelDoc.appendChild(vectorNode);

        QDomElement classNode = modelDoc.createElement("ClassName");
        modelDoc.appendChild(classNode);

        QDomText classText = modelDoc.createTextNode(_trainingLabels[i].c_str());
        classNode.appendChild(classText);
        vectorNode.appendChild(classNode);

        QDomElement dataNode = modelDoc.createElement("Data");
        modelDoc.appendChild(dataNode);

        std::stringstream dataStream;
        for (unsigned int j = 0; j < _data[i].size(); j++)
        {
          dataStream << _data[i][j];

          if (j != _data[i].size() - 1)
          {
            dataStream << " ";
          }
        }

        QDomText dataText = modelDoc.createTextNode(dataStream.str().c_str());
        dataNode.appendChild(dataText);

        vectorNode.appendChild(dataNode);
        dataVectorsNode.appendChild(vectorNode);
      }

      dataFrameNode.appendChild(dataVectorsNode);
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void DataFrame::exportData(std::ostream & fileStream) const
  {
    try
    {
      QDomDocument modelDoc("XML");
      QDomElement rootNode = modelDoc.createElement("DataFrame");
      modelDoc.appendChild(rootNode);
      exportData(modelDoc, rootNode);
      std::string xmlData = modelDoc.toString().toLatin1().constData();
      fileStream << xmlData;
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void DataFrame::getClassPopulations(const std::vector<unsigned int> & indices, 
    HashMap<std::string, int>& populations)
  {
    try
    {
      for (unsigned int i = 0; i < indices.size(); i++)
      {
        if (indices[i] < _trainingLabels.size())
        {
          populations[_trainingLabels[indices[i]]] += 1;
        }
        else
        {
          std::stringstream ss;
          ss << "Index " << indices[i] << " outside of bounds of training label size " <<
            _trainingLabels.size();
          throw Exception(__LINE__, ss.str());
        }
      }
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void DataFrame::getClassDistribution(const std::vector<unsigned int>& indices, 
    std::vector<int>& distribution) const
  {
    try
    {
      distribution.resize(_trainingLabels.size(), 0);
      for (unsigned int i = 0; i < indices.size(); i++)
      {
        if (indices[i] < _trainingLabels.size())
        {
          distribution[indices[i]] += 1;
        }
        else
        {
          std::stringstream ss;
          ss << "Index " << indices[i] << " outside of bounds of training label size " <<
            _trainingLabels.size();
          throw Exception(__LINE__, ss.str());
        }
      }
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  double DataFrame::getDataElement(unsigned int vIdx, int fIdx) const
  {
    try
    {
      if (fIdx == -1)
      {
        map<string, int>::const_iterator it = _trainingLabelEnum.find(_trainingLabels[vIdx]);
        assert(it != _trainingLabelEnum.end());
        return it->second;
      }

      return _data[vIdx][fIdx];
    }
    catch(const std::out_of_range& /*oor*/)
    {
      std::stringstream ss;
      ss << "Out of range accessing data vector " << vIdx << ", element " << fIdx;
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, ss.str());
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  const std::vector<double> & DataFrame::getDataVector(unsigned int vIdx) const
  {
    try
    {
      return _data[vIdx];
    }
    catch(const std::out_of_range& /*oor*/)
    {
      std::stringstream ss;
      ss << "Out of range accessing data vector index: "  << vIdx;
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, ss.str());
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }

  }

  std::string DataFrame::getFactorLabelFromIndex(int fIdx) const
  {
    try
    {
      if (fIdx == -1)
      {
        return "Training Label";
      }
      if (!_factorLabels.empty() && fIdx < (int)_factorLabels.size())
      {
        return _factorLabels[fIdx];
      }
      else
      {
        std::stringstream ss;
        ss << "Index " << fIdx << " out of bounds of factor label size " << _factorLabels.size();
        throw Exception(__LINE__, ss.str());
      }
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  vector<string> DataFrame::getFactorLabels() const
  {
    try
    {
      return _factorLabels;
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  unsigned int DataFrame::getIndexFromFactorLabel(const std::string& fLabel) const
  {
    try
    {
      for (size_t i = 0; i < _factorLabels.size(); i++)
      {
        if (_factorLabels[i] == fLabel)
        {
          return i;
        }
      }

      throw Exception(__LINE__, "Could not find factor label: " + fLabel);
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  std::string DataFrame::getMajorityTrainingLabel(std::vector<unsigned int> & dataSet)
  {
    try
    {
      std::map<std::string, unsigned int> classMap;
      std::map<std::string, unsigned int>::iterator itr;

      for (unsigned int i = 0; i < dataSet.size(); i++)
      {
        classMap[_trainingLabels[dataSet[i]]] += 1;
      }

      unsigned int maxCount = 0;

      std::string maxClass;

      for (itr = classMap.begin(); itr != classMap.end(); ++itr)
      {
        if (itr->second > maxCount)
        {
          maxCount = itr->second;
          maxClass = itr->first;
        }
      }

      return maxClass;
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  int DataFrame::getNullTreatment(int fIdx)
  {
    try
    {
      // -1 is mapped to the training labels
      if (fIdx == -1)
      {
        return NullAsMissingValue;
      }
      assert(fIdx >= 0);
      if (fIdx < (int)_nullTreatment.size())
      {
        return _nullTreatment[fIdx];
      }
      else
      {
        return NullAsMissingValue;
      }
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  std::string DataFrame::getTrainingLabel(unsigned int dIdx) const
  {
    try
    {
      return _trainingLabels[dIdx];
    }
    catch(const std::out_of_range& /*oor*/)
    {
      std::stringstream ss;
      ss << "Out of range accessing training label " << dIdx;
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, ss.str());
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void DataFrame::import(std::istream & fileStream)
  {
    try
    {
      std::istreambuf_iterator<char> eos;
      std::string stdString(std::istreambuf_iterator<char>(fileStream), eos);
      QDomDocument modelDoc("XML");
      modelDoc.setContent(QString::fromStdString(stdString));
      QDomElement elem = modelDoc.documentElement();
      import(elem);
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void DataFrame::import(QDomElement & e)
  {
    try
    {
      QDomNodeList childList = e.childNodes();

      for (unsigned int i = 0; i < (unsigned int)childList.size(); i++)
      {
        if (childList.at(i).nodeType() == QDomNode::CommentNode)
        {
          continue;
        }

        if (childList.at(i).isElement())
        {
          QDomElement childNode = childList.at(i).toElement(); // try to convert the node to an element.

          QString tag = childNode.tagName().toUpper();
          if (tag == "FACTORLABELS")
          {
            QStringList factorList = childNode.text().split(" ");

            for (unsigned int fIdx = 0; fIdx < (unsigned int)factorList.size(); fIdx++)
            {
              _factorLabels.emplace_back(factorList[fIdx].toLatin1().constData());
            }
          }
          else if (tag == "DATAVECTORS")
          {
            QDomNodeList vecNodeList = childNode.childNodes();

            for (unsigned int vIdx = 0; vIdx < (unsigned int)vecNodeList.size(); vIdx++)
            {
              QDomElement vecElement = vecNodeList.at(vIdx).toElement();
              _importDataVector(vecElement);
            }
          }
          else
          {
            std::stringstream ss;
            ss << "The tag " << tag.toLatin1().constData() <<
              " is not supported with in the <DataFrame> tag";
            //throw Exception(__LINE__, ss.str());
          }
        }
      }
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  bool DataFrame::isDataSetPure(std::vector<unsigned int> & indices)
  {
    try
    {
      if (!indices.empty())
      {
        HashMap<std::string, int> populations;

        getClassPopulations(indices, populations);

        if (populations.size() == 1)
        {
          return true;
        }
        else
        {
          return false;
        }
      }
      else
      {
        throw Exception(__LINE__, "Cannot operate on empty set of data");
      }
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  bool DataFrame::isNominal(int fIdx) const
  {
    try
    {
      // -1 is mapped to the training labels
      if (fIdx == -1)
      {
        return true;
      }
      if (_factorType.empty())
      {
        return false;
      }
      return _factorType[fIdx] == Nominal;
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  bool DataFrame::isNull(double v)
  {
    try
    {
#   ifdef _WIN32
      return _isnan(v) == 0 ? false : true;
#   else
      return ::qIsNaN(v) == 0 ? false : true;
#   endif
    }
    catch(const Exception & e)
    {
      throw Exception("DataFrame", __FUNCTION__, __LINE__, e);
    }
  }

  void DataFrame::makeBalancedBoostrapAndOobSets(std::vector<unsigned int> & bootstrap, 
    std::vector<unsigned int> & oob)
  {
    try
    {
      if (!_data.empty())
      {

        double r;

        std::vector<unsigned int> dataIndices; //Hold vector indices of interest (all of them)
        std::vector<bool> selectedVectors; //Tracks which vectors have been added to bootstrap

        dataIndices.resize(_data.size());
        selectedVectors.resize(_data.size());

        //Initialize a list with the indices to all the data vectors
        for (unsigned int i = 0; i < selectedVectors.size(); i++)
        {
          selectedVectors[i] = false;
          dataIndices[i] = i;
        }

        std::map<std::string, std::vector<unsigned int>> idxSortedByClass;
        std::map<std::string, std::vector<unsigned int>>::iterator itr;

        for (unsigned int m = 0; m < _data.size(); m++)
        {
          idxSortedByClass[_trainingLabels[m]].push_back(m);
        }

  //       for (itr = idxSortedByClass.begin(); itr != idxSortedByClass.end(); ++itr)
  //       {
  //         std::cout << "Bootstrap classes: " << itr->first << std::endl;
  //       }


        unsigned int picksPerClass =  (unsigned int)((double)_data.size() /
          (double)idxSortedByClass.size());

        //std::cout << "PicksPerClass " << picksPerClass << " Num Classes " << idxSortedByClass.size() << std::endl;
        //Create bootstrap set with replacement
        bootstrap.resize(picksPerClass * idxSortedByClass.size());

        unsigned int pickCtr = 0;
        while (pickCtr < bootstrap.size())
        {
          for (itr = idxSortedByClass.begin(); itr != idxSortedByClass.end(); ++itr)
          {
            r = ((double)Tgs::Random::instance()->generateInt() / ((double)(RAND_MAX)+(double)(1)));

            unsigned int rndIdx = (unsigned int)(r * (double)itr->second.size());
            bootstrap[pickCtr] = itr->second[rndIdx];
            selectedVectors[itr->second[rndIdx]] = true;
            pickCtr++;
          }
        }

        //Anything that did not get selected for the boot strap is added to oob
        for (unsigned int k = 0; k < selectedVectors.size(); k++)
        {
          if (selectedVectors[k] == false)
          {
            oob.push_back(k);
          }
        }
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

  void DataFrame::makeBalancedRoundRobinBootstrapAndOobSets(std::string className1, 
    std::string className2, std::vector<unsigned int> & bootstrap, std::vector<unsigned int> & oob)
  {
    try
    {
      if (!_data.empty())
      {
        std::vector<unsigned int> posIndices; //Vector to hold indices of positive class
        std::vector<unsigned int> negIndices; //Vector to hold indices of negative class
        std::vector<bool> selectedPos; //Tracks which vectors have been added to bootstrap
        std::vector<bool> selectedNeg; //Tracks which vectors have been added to bootstrap


        //Initialize a list with the indices to all the data vectors
        for (unsigned int i = 0; i < _data.size(); i++)
        {
          if (_trainingLabels[i] == className1)
          {
            posIndices.push_back(i);
          }
          else if (_trainingLabels[i] == className2)
          {
            negIndices.push_back(i);
          }
        }

        selectedPos.resize(posIndices.size());
        selectedNeg.resize(negIndices.size());

        std::fill(selectedPos.begin(), selectedPos.end(), false);
        std::fill(selectedNeg.begin(), selectedNeg.end(), false);

        unsigned int picksPerClass =  (unsigned int)((double)(posIndices.size() +
          negIndices.size()) / 2.0);

        //std::cout << "PicksPerClass " << picksPerClass << " Num Classes " << idxSortedByClass.size() << std::endl;
        //Create bootstrap set with replacement
        bootstrap.resize(picksPerClass * 2);

        unsigned int pickCtr = 0;

        //std::cout << "Boot Strap size " << bootstrap.size() << std::endl;
        while (pickCtr < bootstrap.size())
        {
          //Pick a positive training example
          double r = ((double)Tgs::Random::instance()->generateInt() / ((double)(RAND_MAX)+(double)(1)));
          unsigned int rndIdx = (unsigned int)(r * (double)posIndices.size());
          bootstrap[pickCtr] = posIndices[rndIdx];
          selectedPos[rndIdx] = true;
          //std::cout << "Pos Idx " << posIndices[rndIdx] << std::endl;
          pickCtr++;

          //Pick a negative training example
          r = ((double)Tgs::Random::instance()->generateInt() / ((double)(RAND_MAX)+(double)(1)));
          rndIdx = (unsigned int)(r * (double)negIndices.size());
          bootstrap[pickCtr] = negIndices[rndIdx];
          selectedNeg[rndIdx] = true;
          //std::cout << "Neg Idx " << negIndices[rndIdx] << std::endl;
          pickCtr++;
        }

        //Anything that did not get selected for the boot strap is added to oob
        for (unsigned int k = 0; k < selectedPos.size(); k++)
        {
          if (selectedPos[k] == false)
          {
            oob.push_back(posIndices[k]);
          }
        }

        for (unsigned int j = 0; j < selectedNeg.size(); j++)
        {
          if (selectedNeg[j] == false)
          {
            oob.push_back(negIndices[j]);
          }
        }
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


  void DataFrame::makeBoostrapAndOobSets(std::vector<unsigned int> & bootstrap, 
    std::vector<unsigned int> & oob) const
  {
    try
    {
      if (!_data.empty())
      {
        std::vector<bool> selectedVectors; //Tracks which vectors have been added to bootstrap

        selectedVectors.resize(_data.size());

        //Initialize a list with the indices to all the data vectors
        for (unsigned int i = 0; i < selectedVectors.size(); i++)
        {
          selectedVectors[i] = false;
        }

        //Create bootstrap set with replacement
        bootstrap.resize(_data.size());
        //std::cout << "Data Size " << _data.size() << " Bootstrap Size " << dataIndices.size() << std::endl;

        for (unsigned int j = 0; j < bootstrap.size(); j++)
        {
          double r = ((double)Tgs::Random::instance()->generateInt() / ((double)(RAND_MAX)+(double)(1)));

          unsigned int rndIdx = (unsigned int)(r * (double)_data.size());
          bootstrap[j] = rndIdx;
          selectedVectors[rndIdx] = true;
          //std::cout << " r: " << r << " Size " << dataIndices.size() << " " << rndIdx << " " << dataIndices[rndIdx] << " " << bootstrap[j] <<  std::endl;
        }

        //Anything that did not get selected for the boot strap is added to oob
        for (unsigned int k = 0; k < selectedVectors.size(); k++)
        {
           if (selectedVectors[k] == false)
           {
             oob.push_back(k);
           }
        }
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

  DataFrame& DataFrame::operator=(const DataFrame& from)
  {
    if (this != &from)
    {
      try
      {
        _trainingLabels = from._trainingLabels;
        _trainingLabelsBak = from._trainingLabelsBak;
        _classSet = from._classSet;
        _factorLabels = from._factorLabels;
        _activeFactorIndices = from._activeFactorIndices;
        _data = from._data;
        _factorType = from._factorType;
        _trainingLabelEnum = from._trainingLabelEnum;
        _nullTreatment = from._nullTreatment;
        _trainingEnumCnt = from._trainingEnumCnt;
        _randSeedCtr = from._randSeedCtr;
      }
      catch(const Exception & e)
      {
        throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
      }
    }
    return *this;
  }

  std::vector<double> & DataFrame::operator[](unsigned int vIdx)
  {
    try
    {
      return _data[vIdx];
    }
    catch(const std::out_of_range& /*oor*/)
    {
      std::stringstream ss;
      ss << "Out of range accessing data vector index: "  << vIdx;
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, ss.str());
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }



  void DataFrame::remapClassLabels(std::map<std::string, std::string> & labelMap)
  {
    try
    {
      _trainingLabelsBak = _trainingLabels;
      _trainingLabelEnum.clear();
      _trainingEnumCnt = 1;
      for (unsigned int i = 0; i < _trainingLabels.size(); i++)
      {
        const string& label = labelMap[_trainingLabels[i]];
        _trainingLabels[i] = labelMap[label];
        if (_trainingLabelEnum.find(label) == _trainingLabelEnum.end())
        {
          _trainingLabelEnum[label] = _trainingEnumCnt++;
        }
      }
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void DataFrame::restoreClassLabels()
  {
    try
    {
      _trainingLabels = _trainingLabelsBak;
      _trainingLabelEnum.clear();
      _trainingEnumCnt = 1;
      for (unsigned int i = 0; i < _trainingLabels.size(); i++)
      {
        const string& label = _trainingLabels[i];
        if (_trainingLabelEnum.find(label) == _trainingLabelEnum.end())
        {
          _trainingLabelEnum[label] = _trainingEnumCnt++;
        }
      }
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void DataFrame::selectRandomFactors(unsigned int numFactors,
    std::vector<unsigned int> & fIndices)
  {
    try
    {
      if (!_data.empty())
      {
        fIndices.resize(numFactors);

        std::vector<unsigned int> candidateFactors;
        candidateFactors.resize(_activeFactorIndices.size());

        for (unsigned int i = 0; i < _activeFactorIndices.size(); i++)
        {
          candidateFactors[i] = _activeFactorIndices[i];
        }

        for (unsigned int k = 0; k < numFactors; k++)
        {
          double rr = (double)Tgs::Random::instance()->generateInt();
          double r = (rr / ((double)(RAND_MAX)+(double)(1)));

          unsigned int rndIdx = (unsigned int)(r * (double)candidateFactors.size());
          fIndices[k] = candidateFactors[rndIdx];
          //std::cout << "RndIdx value is " << candidateFactors[rndIdx] << " " <<  _data[0].size() << " " << candidateFactors.size() << " " << rr << std::endl;
          assert(candidateFactors.size() > 0);
          candidateFactors.erase(candidateFactors.begin() + rndIdx);
        }
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

  void DataFrame::setAllFactorsActive()
  {
    try
    {
      _activeFactorIndices.resize(_factorLabels.size());

      for (unsigned int i = 0; i < _factorLabels.size(); i++)
      {
        _activeFactorIndices[i] = i;
      }
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void DataFrame::setBinaryClassLabels(std::string posClass)
  {
    try
    {
      _trainingLabelsBak = _trainingLabels;

      for (unsigned int i = 0; i < _trainingLabels.size(); i++)
      {
        if (_trainingLabels[i] != posClass)
        {
          _trainingLabels[i] = "other";
        }
      }
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void DataFrame::setFactorLabels(const vector<string> & factors)
  {
    try
    {
      _factorLabels = factors;
      _activeFactorIndices.resize(factors.size());

      for (unsigned int i = 0; i < factors.size(); i++)
      {
        _activeFactorIndices[i] = i;
      }
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void DataFrame::setFactorType(int fIdx, int factorType)
  {
    try
    {
      assert(fIdx < (int)getNumFactors() && fIdx >= 0);
      if ((int)_factorType.size() <= fIdx)
      {
        std::vector<int> tmp = _factorType;
        _factorType.resize(getNumFactors());
        for (unsigned int i = 0; i < getNumFactors(); i++)
        {
          if (i < tmp.size())
          {
            _factorType[i] = tmp[i];
          }
          else
          {
            _factorType[i] = Numerical;
          }
        }
      }
      _factorType[fIdx] = factorType;
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void DataFrame::setFactorTypes(const std::vector<int>& types)
  {
    try
    {
      _factorType = types;
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void DataFrame::setNullTreatment(int fIdx, int nullTreatment)
  {
    try
    {
      assert(fIdx < (int)getNumFactors() && fIdx >= 0);
      if ((int)_nullTreatment.size() <= fIdx)
      {
        std::vector<int> tmp = _nullTreatment;
        _nullTreatment.resize(getNumFactors());
        for (unsigned int i = 0; i < getNumFactors(); i++)
        {
          if (i < tmp.size())
          {
            _nullTreatment[i] = tmp[i];
          }
          else
          {
            _nullTreatment[i] = NullAsMissingValue;
          }
        }
      }
      _nullTreatment[fIdx] = nullTreatment;
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void DataFrame::sortIndicesOnFactorValue(std::vector<unsigned int> & indices, 
    unsigned int fIdx) const
  {
    try
    {
      //std::cout << "Fact value is " << fIdx << std::endl;
      //Set the index to use as the sort source
      if (!_data.empty() && fIdx < _data[0].size())
      {
        _qSortIndicesOnFactorValue(indices, 0, indices.size() - 1, fIdx);
      }
      else
      {
        std::stringstream ss;
        ss << "Index " << fIdx << " is outside of data vector bounds of " <<  _data[0].size();
        throw Exception(__LINE__, ss.str());
      }
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void DataFrame::validateData()
  {
    try
    {
      if (!_data.empty())
      {
        std::vector<std::string> badFactors;
        for (unsigned int i = 0; i < _activeFactorIndices.size(); i++)
        {
          std::set<double> dataCheck;

          for (unsigned int k = 0; k < _data.size(); k++)
          {
            dataCheck.insert(_data[k][_activeFactorIndices[i]]);
          }

          if (dataCheck.size() == 1) //All data has the same value
          {
            std::cout << "Deactivating factor due to all vectors having the same value: " <<
              _factorLabels[_activeFactorIndices[i]] << std::endl;

            badFactors.push_back(_factorLabels[_activeFactorIndices[i]]);
          }
        }

        for (unsigned int j = 0; j < badFactors.size(); j++)
        {
          deactivateFactor(badFactors[j]);
        }
      }
      else
      {
        throw Exception(__LINE__, "Can't validate empty data frame.");
      }
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  std::ostream& operator<<(std::ostream& s, const DataFrame& df)
  {
    s << "Training Label";
    for (unsigned int i = 0; i < df.getNumFactors(); i++)
    {
      s << "\t" << df.getFactorLabelFromIndex(i);
    }
    s << endl;
    for (unsigned int vIdx = 0; vIdx < df.getNumDataVectors(); vIdx++)
    {
      s << df.getTrainingLabel(vIdx);
      for (unsigned int fIdx = 0; fIdx < df.getNumFactors(); fIdx++)
      {
        s << "\t" << df.getDataElement(vIdx, fIdx);
      }
      s << endl;
    }
    return s;
  }

  void DataFrame::_importDataVector(QDomElement & e)
  {
    try
    {
      QDomNodeList childList = e.childNodes();

      for (unsigned int i = 0; i < (unsigned int)childList.size(); i++)
      {
        if (childList.at(i).nodeType() == QDomNode::CommentNode)
        {
          continue;
        }

        if (childList.at(i).isElement())
        {
          QDomElement element = childList.at(i).toElement(); // try to convert the node to an element.

          QString tag = element.tagName().toUpper();

          bool parseOk = true;

          if (tag == "CLASSNAME")
          {
            _trainingLabels.emplace_back(element.text().toLatin1().constData());
          }
          else if (tag == "DATA")
          {
            QStringList factorList = element.text().split(" ");

            std::vector<double> dataVector(factorList.size());

            for (unsigned int dIdx = 0; dIdx < (unsigned int)factorList.size(); dIdx++)
            {
              double dataValue = factorList[dIdx].toDouble(&parseOk);

              if (!parseOk)
              {
                std::stringstream ss;
                ss << "Unable to parse the data value " << factorList[dIdx].toLatin1().constData();
                throw Exception(__LINE__, ss.str());
              }

              dataVector[dIdx] = dataValue;
            }

            _data.push_back(dataVector);
          }
        }
      }
    }
    catch(const Exception & ex)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, ex);
    }
  }

  void DataFrame::_qSortIndicesOnFactorValue(std::vector<unsigned int> & indices, unsigned int posP,
    unsigned int posR, unsigned int fIdx) const
  {
    try
    {
      if (posP < posR)
      {
        unsigned int q = _qSortPartition(indices, posP, posR, fIdx);
        _qSortIndicesOnFactorValue(indices, posP, q, fIdx);
        _qSortIndicesOnFactorValue(indices, q + 1, posR, fIdx);
      }
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  unsigned int DataFrame::_qSortPartition(std::vector<unsigned int> & indices, unsigned int posP,
    unsigned int posR, unsigned int fIdx) const
  {
    try
    {
      const unsigned int loopLimit = 100000;
      double x = _data[indices[posP]][fIdx];
      unsigned int i = posP - 1;
      unsigned int j = posR + 1;

      unsigned int ctr = 0;
      while (ctr < loopLimit)
      {
        do
        {
          j = j - 1;
        } while (_data[indices[j]][fIdx] > x);

        do
        {
          i = i + 1;
        } while (_data[indices[i]][fIdx] < x);

        if (i < j)
        {
          unsigned int swapVal = indices[j];
          indices[j] = indices[i];
          indices[i] = swapVal;
        }
        else
        {
          return j;
        }
        ctr++;
      }
      return 0;
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }
}  //End namespace


