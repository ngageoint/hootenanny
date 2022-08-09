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
 * @copyright Copyright (C) 2015, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef __TGS__TRANSFORM_H__
#define __TGS__TRANSFORM_H__

#include "../TgsExport.h"

namespace Tgs
{
  class TGS_EXPORT Transform
  {
  public:

    /**
     * Converts the array that is passed in into approximate quantiles.
     */
    template<class _T>
    static std::vector<float> convertToQuantiles(const std::vector<_T>& v)
    {
      std::vector<float> tmp;
      tmp.resize(100000, 0.0);

      _T min = std::numeric_limits<_T>::max();
      _T max = std::numeric_limits<_T>::min();
      for (unsigned int i = 0; i < v.size(); i++)
      {
        float a = v[i];
        if (a < std::numeric_limits<_T>::infinity() && a > -std::numeric_limits<_T>::infinity())
        {
          min = std::min(min, v[i]);
          max = std::max(max, v[i]);
        }
      }
      cout << "s1" << min << " " << max << endl;
      cout.flush();
      int count = 0;
      for (unsigned int i = 0; i < v.size(); i++)
      {
        float a = v[i];
        if (a < std::numeric_limits<_T>::infinity() && a > -std::numeric_limits<_T>::infinity())
        {
          unsigned int index = int(((double)(a - min) / (double)(max - min)) * 
            (double)(tmp.size() - 1));
          if (index >= tmp.size())
          {
            cout << index << " " << a << " " << min << " " << max << endl;
          }
          tmp[index]++;
          count++;
        }
      }

      cout << "s1" << endl;
      cout.flush();
      float sum = 0.0;
      for (unsigned int i = 0; i < tmp.size(); i++)
      {
        sum += tmp[i] / (float)v.size();
        tmp[i] = sum;
      }

      cout << "s1" << endl;
      cout.flush();

      std::vector<float> result;
      result.resize(v.size());
      for (unsigned int i = 0; i < v.size(); i++)
      {
        float a = v[i];
        if (a < std::numeric_limits<_T>::infinity() && a > -std::numeric_limits<_T>::infinity())
        {
          int index = int(((double)(a - min) / (double)(max - min)) * 
            (double)(tmp.size() - 1));
          result[i] = tmp[index];
        }
        else if (a < std::numeric_limits<_T>::infinity())
        {
          result[i] = 0.0;
        }
        else
        {
          result[i] = 1.0;
        }
      }
      cout << "s1" << endl;
      cout.flush();

      return result;
    }
  };
}

#endif
