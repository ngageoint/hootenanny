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
 * @copyright Copyright (C) 2015, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#include "DataFrameDiscretizer.h"

// Standard Includes
#include <assert.h>
#include <float.h>
#include <iostream>
#include <math.h>
using namespace std;

#include "../TgsException.h"
#include "../Progress.h"
#include "FayyadMdlDiscretizer.h"

namespace Tgs
{
  void DataFrameDiscretizer::discretize(DataFrame& df, TgsProgress* progress)
  {
    _df = &df;

    for (unsigned int i = 0; i < df.getNumFactors(); i++)
    {
      if (progress)
      {
        progress->setProgress((double)i / (double)df.getNumFactors());
      }
      if (_df->isNominal(i) == false)
      {
        if (_df->getNullTreatment(i) == DataFrame::NullAsMissingValue)
        {
          // replace nulls with random sampling of data (imputation), otherwise nulls get put
          // into their own category.
          _replaceNulls(i);
        } 
        _discretizeColumn(i);
      }
    }
    if (progress)
    {
      progress->setProgress(1.0);
    }
  }

  void DataFrameDiscretizer::_discretizeColumn(int column)
  {
    DataFrame& df = *_df;
    assert(df.isNominal(column) == false);

    HashMap<std::string, int> enumMap;
    int enumCnt = 1;

    std::vector<unsigned int> indices;
    indices.reserve(df.getNumDataVectors());
    for (unsigned int i = 0; i < df.getNumDataVectors(); i++)
    {
      if (DataFrame::isNull(df.getDataElement(i, column)) == false)
      {
        indices.push_back(i);
      }
      else
      {
        // the zero enumeration is reserved for NULL values.
        df.setDataElement(i, column, 0);
      }
    }

    df.sortIndicesOnFactorValue(indices, column);

    vector<int> classEnum;
    vector<double> values;
    values.reserve(df.getNumDataVectors());
    classEnum.reserve(df.getNumDataVectors());
    // enumerate the classes as integers and push the enumerations onto a vector
    for (unsigned int i = 0; i < indices.size(); i++)
    {
      const string& className = df.getTrainingLabel(indices[i]);
      if (enumMap.find(className) == enumMap.end())
      {
        enumMap[className] = enumCnt++;
      }
      classEnum.push_back(enumMap[className]);
      values.push_back(df.getDataElement(indices[i], column));
    }

    df.setFactorType(column, DataFrame::Nominal);

    FayyadMdlDiscretizer discrete;
    FayyadMdlDiscretizer::SplitPoints splits = discrete.calculateSplitPoints(classEnum, values);
    // the last element should never be a split point -- it wouldn't make sense
    assert(splits.size() == 0 || splits[splits.size() - 1] < (int)indices.size() - 1);
    splits.push_back(indices.size() - 1);

    string upper, lower;
    int lowerIndex = 0;
    int upperIndex;
    for (unsigned int i = 0; i < splits.size(); i++)
    {
      upperIndex = splits[i];
      for (int j = lowerIndex; j <= upperIndex; j++)
      {
        // add one, we're reserving zero for null values
        df.setDataElement(indices[j], column, i + 1);
      }
      lower = upper;
      lowerIndex = upperIndex + 1;
    }
  }

  void DataFrameDiscretizer::_replaceNulls(int column)
  {
    DataFrame& df = *_df;
    HashMap< string, vector<int> > sampleIndices;

    // generate a vector of the indices that we can sample data from.
    for (unsigned int i = 0; i < df.getNumDataVectors(); i++)
    {
      if (DataFrame::isNull(df.getDataElement(i, column)) == false)
      {
        sampleIndices[df.getTrainingLabel(i)].push_back(i);
      }
    }

    // if there are no valid values, it doesn't matter what we put in the missing data fields.
    if (sampleIndices.size() == 0)
    {
      for (unsigned int i = 0; i < df.getNumDataVectors(); i++)
      {
        df.setDataElement(i, column, -1);
      }
    }
    else
    {
      for (unsigned int i = 0; i < df.getNumDataVectors(); i++)
      {
        if (DataFrame::isNull(df.getDataElement(i, column)))
        {
          int s = sampleIndices[df.getTrainingLabel(i)].size();
          if (s == 0)
          {
            throw Exception("While attempting to replace missing values (imputation) one or more "
              "classes were found to have 0 valid samples.");
          }
          int index = (rand() * RAND_MAX + rand()) % sampleIndices[df.getTrainingLabel(i)].size();
          double replacement = df.getDataElement(sampleIndices[df.getTrainingLabel(i)][index], 
            column);
          df.setDataElement(i, column, replacement);
        }
      }
    }
  }
}
