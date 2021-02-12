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
#include "MissingDataHandler.h"

//Std Includes
#include <algorithm>

#include <tgs/TgsException.h>

namespace Tgs
{
  void MissingDataHandler::replaceMissingValuesFast(double missingDataValue,
    std::shared_ptr<DataFrame> data)
  {
    try
    {
      unsigned int numFactors = data->getNumFactors();

      //For each factor check if the null value is supposed to be a missing value and proceed from there
      //If null treatment has not been set for the factor then default to replace as missing values
      for (unsigned int fIdx = 0; fIdx < numFactors; fIdx++)
      {
        if (!data->hasNullTreatments() || data->getNullTreatment(fIdx) == DataFrame::NullAsMissingValue)
        {
          DataFrame::FactorType fType;

          if (data->hasFactorTypes())
          {
            fType = (DataFrame::FactorType)data->getFactorTypes()[fIdx];
          }
          else
          {
            //Default to numeric
            fType = DataFrame::Numerical;
          }

          if (fType == DataFrame::Numerical)
          {
            _replaceMissingNumericValuesFast(missingDataValue, data, fIdx);
          }
          else
          {
            _replaceMissingNominalValuesFast(missingDataValue, data, fIdx);
          }
        }
      }
    }
    catch(const Exception & e)
    {
      throw Exception("MissingDataHandler", __FUNCTION__, __LINE__, e);
    }
  }

  void MissingDataHandler::_replaceMissingNominalValuesFast(double missingDataValue,
    std::shared_ptr<DataFrame> data, unsigned int factorIndex)
  {
    try
    {
      unsigned int numDataVectors = data->getNumDataVectors();

      std::map<std::string, std::map<double, unsigned int>> frequencyMap;

      for (unsigned int dataIndex = 0; dataIndex < numDataVectors; dataIndex++)
      {
        double value = data->getDataVector(dataIndex)[factorIndex];

        if (value != missingDataValue)
        {
          std::string className = data->getTrainingLabel(dataIndex);

          frequencyMap[className][value]++;
        }
      }

      std::map<std::string, double> maxFrequencyMap;

      std::map<double, unsigned int >::iterator valueItr;
      std::map<std::string, std::map<double, unsigned int>>::iterator classItr;

      for (classItr = frequencyMap.begin(); classItr != frequencyMap.end(); ++classItr)
      {
        double maxValue = 0.0;
        int maxCount = -1;

        for (valueItr = classItr->second.begin(); valueItr != classItr->second.end(); ++valueItr)
        {
          int valueCount = valueItr->second;

          if (valueCount > maxCount)
          {
            maxValue = valueItr->first;
            maxCount = valueCount;
          }
        }

        maxFrequencyMap[classItr->first] = maxValue;
      }

      for (unsigned int dataIndex = 0; dataIndex < numDataVectors; dataIndex++)
      {
        double value = data->getDataVector(dataIndex)[factorIndex];

        if (value == missingDataValue)
        {
          std::string className = data->getTrainingLabel(dataIndex);

          data->setDataElement(dataIndex, factorIndex, maxFrequencyMap[className]);
        }
      }

    }
    catch(const Exception & e)
    {
      throw Exception("MissingDataHandler", __FUNCTION__, __LINE__, e);
    }
  }

  void MissingDataHandler::_replaceMissingNumericValuesFast(double missingDataValue,
    std::shared_ptr<DataFrame> data, unsigned int factorIndex)
  {
    try
    {
      unsigned int numDataVectors = data->getNumDataVectors();

      std::map<std::string, std::vector<double>> sortMap;

      for (unsigned int dataIndex = 0; dataIndex < numDataVectors; dataIndex++)
      {
        double value = data->getDataVector(dataIndex)[factorIndex];

        if (value != missingDataValue)
        {
          std::string className = data->getTrainingLabel(dataIndex);

          sortMap[className].push_back(value);
        }
      }

      std::map<std::string, double> medianMap;
      std::map<std::string, std::vector<double>>::iterator sortItr;

      for (sortItr = sortMap.begin(); sortItr != sortMap.end(); ++sortItr)
      {
        std::sort(sortItr->second.begin(), sortItr->second.end());
        medianMap[sortItr->first] = sortItr->second[sortItr->second.size() / 2];
      }

      for (unsigned int dataIndex = 0; dataIndex < numDataVectors; dataIndex++)
      {
        double value = data->getDataVector(dataIndex)[factorIndex];

        if (value == missingDataValue)
        {
          std::string className = data->getTrainingLabel(dataIndex);

          data->setDataElement(dataIndex, factorIndex, medianMap[className]);
        }
      }

    }
    catch(const Exception & e)
    {
      throw Exception("MissingDataHandler", __FUNCTION__, __LINE__, e);
    }
  }


}
