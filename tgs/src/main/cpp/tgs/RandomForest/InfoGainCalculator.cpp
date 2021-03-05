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

#include "InfoGainCalculator.h"

//STD Includes
#include <cmath>
#include <iostream>
#include <limits>

//Urgent Includes
#include <tgs/HashMap.h>
#include <tgs/TgsException.h>

namespace Tgs
{
  double InfoGainCalculator::_calcLogFunc(double n)
  {
    if (n < std::numeric_limits<double>::epsilon())
    {
      n = std::numeric_limits<double>::epsilon();
    }

    return (-n * (log(n)/log((double)2.0)));
  }

  double InfoGainCalculator::computeEntropyByClass(DataFrame & df, 
    std::vector<unsigned int> & indices)
  {
//     double entSum = 0;
//     std::vector<int> popDist;
//     df.getClassDistribution(indices, popDist);
// 
//     for (unsigned int i = 0; i < popDist.size(); i++)
//     {
//       entSum += _calcLogFunc((double)popDist[i] / (double)indices.size()); 
//     }
// 
//     return entSum;
    double entSum = 0;
    HashMap<std::string, int> popMap;
    df.getClassPopulations(indices, popMap);

    HashMap<std::string, int>::iterator itr;

    for (itr = popMap.begin(); itr != popMap.end(); ++itr)
    {
      entSum += _calcLogFunc((double)itr->second / (double)indices.size()); 
    }

    return entSum;
  }
 
  void InfoGainCalculator::_findCandidateSplits(DataFrame & df, std::vector<unsigned int> & indices, unsigned int fIdx, 
    std::vector<unsigned int> & splits)
  {
     splits.clear();
 
     //The split will be based on any difference between the value of 2 contiguous values
     //Note the miniumum split idx is always 1
     for (unsigned int i = 1; i < indices.size(); i++)
     {
       double val1 = df.getDataVector(indices[i-1])[fIdx];
       double val2 = df.getDataVector(indices[i])[fIdx];
      //std::cout << "Potential Split " << i << " " << val1 << " " << val2 << std::endl;
       if (fabs(val1 - val2) >= std::numeric_limits<double>::epsilon())
       {
         splits.push_back(i);
       }
     }
   }

  bool InfoGainCalculator::findDataSplit(DataFrame & df, std::vector<unsigned int> & fIndices, 
    std::vector<unsigned int> & indices, unsigned int & splitIdx, unsigned int & fIdx, 
    double & splitVal, double & purityDelta)
  {
    if (!df.empty())
    {
      //First find the class entropy
      double classEntropy = computeEntropyByClass(df, indices);

      //Set up container to hold results
      std::vector<double> infoGain;
      infoGain.resize(fIndices.size());
      std::vector<unsigned int> splitIndices;
      splitIndices.resize(fIndices.size());
      std::vector<std::vector<unsigned int>> sortedIndices;
      sortedIndices.resize(fIndices.size());

      //Sort each indices set based on the factor values
      //This maintains a list of indices for each factor
      //being considered.  Trade off is memory vs. cost
      //of resorting indices on winning factor
      //
      //Note fIndices is the list of factor indices to consider
      for (unsigned int i = 0; i < fIndices.size(); i++)
      {
        sortedIndices[i] = indices;                                  //Load up all the data vectors of interest
        df.sortIndicesOnFactorValue( sortedIndices[i], fIndices[i]); //Sort the data vectors by the factor values
        infoGain[i] = getMaxInfoGainByFactor(df, sortedIndices[i], fIndices[i], classEntropy, splitIndices[i]);
      }

      double maxGr = -1E10;
      unsigned int maxFactIdx = 0;
      unsigned int maxIdx = 0;
      for (unsigned int j = 0; j < fIndices.size(); j++)
      {
        //std::cout << "IGC: " << infoGain[j] << " " << j << std::endl;
        if (infoGain[j] > maxGr)
        {
          maxGr = infoGain[j];  //Get the max info gain ratio
          maxFactIdx = fIndices[j];
          maxIdx = j;
        }
      }

      if (fabs(maxGr - -1E10) <= std::numeric_limits<double>::epsilon())
      {
        return false;
      }
     
      //Set values for return
      fIdx = maxFactIdx;
     
      indices = sortedIndices[maxIdx];
      splitIdx = splitIndices[maxIdx];
      purityDelta = maxGr;

      splitVal = (df.getDataVector(indices[splitIdx])[fIdx] + df.getDataVector(indices[splitIdx - 1])[fIdx]) / 2.0;

      return true;

    }
    else
    {
      throw Exception("SaUrgent::DataFrame::findDataSplitFromInfoGain - Unable to operate on empty dataset");
    }
  }

  double InfoGainCalculator::getMaxInfoGainByFactor(DataFrame & df, 
    std::vector<unsigned int> & indices, unsigned int fIdx, double totalEntropy, 
    unsigned int & bestSplit)
  {
    //Indices need to be sorted on the factor to use this as a public function 

    //Next find a list of candidate splits per factor
    //Split rules
    // - split value can not split adjacent vectors with same value
    std::vector<unsigned int> splits;
    _findCandidateSplits(df, indices, fIdx, splits);  //Find all potential splits among the factor data

    std::vector<double> leftEntVec;
    std::vector<double> rightEntVec;
    std::vector<unsigned int> tmpIndices;
    tmpIndices.resize(1);
    double entSum = 0;
    HashMap<std::string, int> popMap;
    HashMap<std::string, int>::iterator itr;

    ////
    // Calculate left entropy for all possible splits (not just candidate splits)
    ////
    leftEntVec.reserve(indices.size());
    for (unsigned int i = 0; i < indices.size(); i++)
    {
      tmpIndices[0] = indices[i];
      df.getClassPopulations(tmpIndices, popMap);

      entSum = 0.0;
      for (itr = popMap.begin(); itr != popMap.end(); ++itr)
      {
        entSum += _calcLogFunc((double)itr->second / (double)(i + 1)); 
      }
      leftEntVec.push_back(entSum);
    }

    ////
    // Calculate right entropy for all possible splits (not just candidate splits)
    ////
    popMap.clear();
    rightEntVec.resize(indices.size());
    for (int i = ((int)indices.size()) - 1; i >= 0; i--)
    {
      tmpIndices[0] = indices[i];
      df.getClassPopulations(tmpIndices, popMap);

      int tmpSize = indices.size() - i;
      entSum = 0.0;
      for (itr = popMap.begin(); itr != popMap.end(); ++itr)
      {
        entSum += _calcLogFunc((double)itr->second / (double)tmpSize); 
      }
      rightEntVec[i] = entSum;
    }

    double maxIg = -1E20;
    unsigned int maxSplitIdx = 1;
    //Compute the class entropy per split
    for (unsigned int i = 0; i < splits.size(); i++)
    {
//       //Construct the left and right splits based on splits[i] (a split point within indices)
//       std::vector<unsigned int> leftSplit;
//       std::vector<unsigned int> rightSplit;
// 
//       for (unsigned int j = 0; j < indices.size(); j++)
//       {
//         if (j < splits[i])
//         {
//           leftSplit.push_back(indices[j]);
//         }
//         else
//         {
//           rightSplit.push_back(indices[j]);
//         }
//       }

      double leftEnt;
      if (splits[i] <= 0)
      {
        leftEnt = 0.0;
      }
      else
      {
        leftEnt = leftEntVec[splits[i] - 1];
      }
      double rightEnt;
      if (splits[i] <= 0)
      {
        rightEnt = 0.0;
      }
      else
      {
        rightEnt = rightEntVec[splits[i]];
      }
      
//       double leftEnt2 = computeEntropyByClass(df, leftSplit);
//       if (fabs(leftEnt2 - leftEnt) > 1e-6)
//       {
//         std::cout << "not close " << leftEnt << " " << leftEnt2 << std::endl;
//       }
//       double rightEnt2 = computeEntropyByClass(df, rightSplit);
//       if (fabs(rightEnt2 - rightEnt) > 1e-6)
//       {
//         std::cout << "not close " << rightEnt << " " << rightEnt2 << std::endl;
//       }
      double splitEnt = (double)(splits[i])/(double)(indices.size()) * leftEnt +
        (double)(indices.size() - splits[i])/(double)(indices.size()) * rightEnt;
      double infoGain = totalEntropy - splitEnt;
      
      //std::cout << " InfoGain1 " << infoGain << std::endl;
      //std::cout << "left " << leftEnt << " right " << rightEnt << " split " << splitEnt << std::endl;
      //std::cout << "leftSize " << leftSplit.size() << " rightSize " << rightSplit.size() << std::endl;

      if (infoGain > maxIg)
      {
        maxIg = infoGain;
        maxSplitIdx = splits[i];
      }
    }

    bestSplit = maxSplitIdx;
    return maxIg;
  }

  double InfoGainCalculator::getMaxInfoGainRatioByFactor(DataFrame & df, 
    std::vector<unsigned int> & indices, unsigned int fIdx, double totalEntropy, 
    unsigned int & bestSplit)
  {
    //Indices need to be sorted on the factor to use this as a public function 

    //Next find a list of candidate splits per factor
    //Split rules
    // - split value can not split adjacent vectors with same value
    std::vector<unsigned int> splits;
    _findCandidateSplits(df, indices, fIdx, splits);

    double maxGr = -1E10;
    unsigned int maxSplitIdx = 0;
    //Compute the class entropy per split
    for (unsigned int i = 0; i < splits.size(); i++)
    {
      std::vector<unsigned int> leftSplit;
      std::vector<unsigned int> rightSplit;

      for (unsigned int j = 0; j < indices.size(); j++)
      {
        if (j < splits[i])
        {
          leftSplit.push_back(indices[j]);
        }
        else
        {
          rightSplit.push_back(indices[j]);
        }
      }

      double leftEnt = computeEntropyByClass(df, leftSplit);
      double rightEnt = computeEntropyByClass(df, rightSplit);
      double splitEnt = (double)(splits[i])/(double)(indices.size()) * leftEnt +
        (double)(indices.size() - splits[i])/(double)(indices.size()) * rightEnt;
      double infoGain = totalEntropy - splitEnt;
      double splitInfo = leftEnt + rightEnt;

      if (splitInfo <= 0)
      {
        splitInfo = 1E-10;
      }
      double gainRatio = infoGain / splitInfo;

      //std::cout << " InfoGain1 " << infoGain << std::endl;
      //std::cout << "left " << leftEnt << " right " << rightEnt << " split " << splitEnt << std::endl;
      //std::cout << "leftSize " << leftSplit.size() << " rightSize " << rightSplit.size() << std::endl;

      if (gainRatio > maxGr)
      {
        maxGr = gainRatio;
        maxSplitIdx = splits[i];
      }
    }

    bestSplit = maxSplitIdx;
    return maxGr;
  }
  
}//end namespace

