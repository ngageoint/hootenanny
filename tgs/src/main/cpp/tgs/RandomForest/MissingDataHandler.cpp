#include "MissingDataHandler.h"

//Std Includes
#include <algorithm>

#include "../TgsException.h"

namespace Tgs
{
  void MissingDataHandler::replaceMissingValuesFast(double missingDataValue,
    boost::shared_ptr<DataFrame> data)
  {
    try
    {
      unsigned int numFactors = data->getNumFactors();

      //For each factor check if the null value is supposed to be a missing value and proceed from there
      //If null treatment has not been set for the factor then default to replace as missing values
      for(unsigned int fIdx = 0; fIdx < numFactors; fIdx++)
      {
        if(!data->hasNullTreatments() || data->getNullTreatment(fIdx) == DataFrame::NullAsMissingValue)
        {
          DataFrame::FactorType fType;

          if(data->hasFactorTypes())
          {
            fType = (DataFrame::FactorType)data->getFactorTypes()[fIdx];
          }
          else
          {
            //Default to numeric
            fType = DataFrame::Numerical;
          }

          if(fType == DataFrame::Numerical)
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
    boost::shared_ptr<DataFrame> data, unsigned int factorIndex)
  {
    try
    {
      unsigned int numDataVectors = data->getNumDataVectors();

      std::map<std::string, std::map<double, unsigned int > > frequencyMap;

      for(unsigned int dataIndex = 0; dataIndex < numDataVectors; dataIndex++)
      {
        double value = data->getDataVector(dataIndex)[factorIndex];

        if(value != missingDataValue)
        {
          std::string className = data->getTrainingLabel(dataIndex);

          frequencyMap[className][value]++;
        }
      }

      std::map<std::string, double> maxFrequencyMap;

      std::map<double, unsigned int >::iterator valueItr;
      std::map<std::string, std::map<double, unsigned int > >::iterator classItr;

      for(classItr = frequencyMap.begin(); classItr != frequencyMap.end(); ++classItr)
      {
        double maxValue;
        int maxCount = -1;

        for(valueItr = classItr->second.begin(); valueItr != classItr->second.end(); ++valueItr)
        {
          int valueCount = valueItr->second;

          if(valueCount > maxCount)
          {
            maxValue = valueItr->first;
            maxCount = valueCount;
          }
        }

        maxFrequencyMap[classItr->first] = maxValue;
      }

      for(unsigned int dataIndex = 0; dataIndex < numDataVectors; dataIndex++)
      {
        double value = data->getDataVector(dataIndex)[factorIndex];

        if(value == missingDataValue)
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
    boost::shared_ptr<DataFrame> data, unsigned int factorIndex)
  {
    try
    {
      unsigned int numDataVectors = data->getNumDataVectors();

      std::map<std::string, std::vector<double> > sortMap;

      for(unsigned int dataIndex = 0; dataIndex < numDataVectors; dataIndex++)
      {
        double value = data->getDataVector(dataIndex)[factorIndex];

        if(value != missingDataValue)
        {
          std::string className = data->getTrainingLabel(dataIndex);

          sortMap[className].push_back(value);
        }
      }

      std::map<std::string, double> medianMap;
      std::map<std::string, std::vector<double> >::iterator sortItr;

      for(sortItr = sortMap.begin(); sortItr != sortMap.end(); ++sortItr)
      {
        std::sort(sortItr->second.begin(), sortItr->second.end());
        medianMap[sortItr->first] = sortItr->second[sortItr->second.size() / 2];
      }

      for(unsigned int dataIndex = 0; dataIndex < numDataVectors; dataIndex++)
      {
        double value = data->getDataVector(dataIndex)[factorIndex];

        if(value == missingDataValue)
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
