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
 * @copyright Copyright (C) 2015 Maxar (http://www.maxar.com/)
 */
#include "MetaCostAlgorithm.h"

//Std Includes
#include <fstream>
#include <typeinfo>

//Tgs Includes
#include "../TgsException.h"

namespace Tgs
{
  MetaCostAlgorithm::MetaCostAlgorithm()
  {
  }

  void MetaCostAlgorithm::generateResults(std::string basename)
  {
    try
    {
      std::string reportName = basename + "_metacost_reclass.txt";

      std::fstream reportStream(reportName.c_str(), std::fstream::out);

      if(!reportStream.is_open())
      {
        std::stringstream ss;
        ss << "Unable to open/create file " << reportName << " for writing";
        throw Tgs::Exception(__LINE__, ss.str());
      }

      if(_riskValues.size() != _minRiskLabels.size())
      {
        std::stringstream ss;
        ss << "INTERNAL DEV ERROR: the _riskValues and minRiskLabels vector are different sizes";
        throw Tgs::Exception(__LINE__, ss.str());
      }

      for(unsigned int i = 0; i < _riskValues.size(); i++)
      {
        reportStream << i << " ";

        for(unsigned int c = 0; c < _classLabels.size(); c++)
        {
          reportStream << _classLabels[c] << "=" << _riskValues[i][c] << " ";
        }

        reportStream << " : " << _classLabels[_minRiskLabels[i]] << std::endl;
      }

      reportStream.close();
    }
    catch(const Tgs::Exception & e)
    {
      throw Tgs::Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void MetaCostAlgorithm::initialize(std::vector<std::string> classLabels,
    std::vector<float> costMatrix)
  {
    try
    {
      if(classLabels.size() * classLabels.size() != costMatrix.size())
      {
        std::stringstream ss;
        ss << "The size of the cost matrix (" << costMatrix.size() <<
          ") should be the square of the size of the classLabels list (" << classLabels.size() <<
          ")";
        throw Tgs::Exception(__LINE__, ss.str());
      }
      _classLabels = classLabels;
      _costMatrix = costMatrix;
      _initialized = true;
    }
    catch(const Tgs::Exception & e)
    {
      throw Tgs::Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }


  std::string MetaCostAlgorithm::reclassifyVector(std::vector<double> probabilityVector, bool saveResults)
  {
    try
    {
      if(!_initialized)
      {
        throw Tgs::Exception(__LINE__, "This object has to be initialized before use.");
      }

      if(probabilityVector.size() != _classLabels.size())
      {
        std::stringstream ss;
        ss << "The size of the probability vector (" << probabilityVector.size() <<
          ") should be the sameas  the size of the classLabels list (" << _classLabels.size() <<
          ")";
        throw Tgs::Exception(__LINE__, ss.str());
      }

      int minLabelIndex = -1;
      double minCost = 1E10;

      if(saveResults)
      {
        _riskValues.push_back(std::vector<float>(3));
      }

      for(unsigned int rc = 0; rc < _classLabels.size(); rc++)
      {
        double sum = 0;
        for(unsigned int pr = 0; pr < _classLabels.size(); pr++)
        {
          double prob = probabilityVector[pr];
          sum += prob * _costMatrix[_classLabels.size() * rc + pr];
        }

        if(saveResults)
        {
          _riskValues.back()[rc] = sum;
        }

        if(sum < minCost)
        {
          minCost = sum;
          minLabelIndex = rc;
        }
      }

      if(saveResults)
      {
        _minRiskLabels.push_back(minLabelIndex);
      }

      return _classLabels[minLabelIndex];
    }
    catch(const Tgs::Exception & e)
    {
      throw Tgs::Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }
}

