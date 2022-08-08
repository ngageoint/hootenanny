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

#include "XmlHelper.h"

// Standard Includes
#include <iostream>
using namespace std;

#include "../TgsException.h"

namespace Tgs
{
  HashMap<std::string, std::string> XmlHelper::parseAttributes(const std::string& str)
  {
    HashMap<string, string> result;

    string tmp = trim(str);
    string::size_type i1 = 0, i2 = 0, i3 = 0;
    while (i2 != string::npos)
    {
      // find the beginning of the first key
      i1 = tmp.find_first_not_of(' ', tmp.find_first_of(' ', i3));
      i2 = tmp.find_first_of('=', i1);
      // if there is another equal sign
      if (i2 != string::npos)
      {
        string key = tmp.substr(i1, i2 - i1);
        i2++;
        if (tmp[i2] == '\'')
        {
          i2++;
          i3 = tmp.find_first_of('\'', i2);
        }
        else
        {
          i3 = tmp.find_first_of(' ', i2);
        }
        string value = tmp.substr(i2, i3 - i2);
        result[key] = value;
      }
    }

    return result;
  }

  void XmlHelper::parseTagValue(const std::string& str, std::string& tag, std::string& value)
  {
    tag = "";
    value = "";
    string::size_type i1 = str.find_first_of('<');
    string::size_type i2 = str.find_first_of('>', i1);
    if (i1 == string::npos || i2 == string::npos)
    {
      throw Tgs::Exception(string("Error parsing tag value. ") + str);
    }
    tag = str.substr(i1 + 1, i2 - i1 - 1);
    string::size_type i3 = str.find_first_of('<', i2);
    if (i3 == string::npos)
    {
      throw Tgs::Exception(string("Error parsing tag value. ") + str);
    }
    value = str.substr(i2 + 1, i3 - i2 - 1);
  }

  vector<string> XmlHelper::split(const string& str, const string& delimeter)
  {
    std::vector<std::string> result;
    size_t start = 0;
    size_t end;
    do
    {
      end = str.find(delimeter, start);
      string c(str, start, end - start);
      result.push_back(c);
      start = end + 1;
    } while (end < str.size());

    return result;
  }

  std::string XmlHelper::trim(const std::string& str)
  {
    int start = 0;
    int end = str.size() - 1;
    while (start < (int)str.size() && 
      (str.c_str()[start] == ' ' || str.c_str()[start] == '\t' || str.c_str()[start] == '\n'))
    {
      start++;
    }
    while (end > start && 
      (str.c_str()[end] == ' ' || str.c_str()[end] == '\t' || str.c_str()[end] == '\n'))
    {
      end--;
    }
    return string(str, start, end - start + 1);
  }
}
