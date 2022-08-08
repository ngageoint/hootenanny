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
#include "CslUtilities.h"

//Qt Includes
#include <QString>

//Std Includes
#include <fstream>
#include <iostream>

#include "../TgsException.h"

namespace Tgs
{

std::vector<float> CslUtilities::readCostMatrix(std::string filename)
{
  try
  {
    std::fstream fileStream(filename.c_str(), std::fstream::in);

    if(!fileStream.is_open())
    {
      std::stringstream ss;
      ss << "Unable to open file " << filename;
      throw Tgs::Exception(__LINE__, ss.str());
    }

    std::vector<float> costMatrix;

    int dataLine = 0;

    std::string buffer;
    while(!fileStream.eof())
    {
      std::getline(fileStream, buffer);

      if(buffer.empty())
      {
        dataLine++;
        continue;
      }

      std::stringstream bufferStr(buffer);
      std::vector<std::string> tokenList;
      std::string token;

      while(std::getline(bufferStr, token, ' '))
      {
        tokenList.push_back(token);
      }

      bool parseOk;

      for(unsigned int i = 0; i < tokenList.size(); i++)
      {
        float value = QString(tokenList[i].c_str()).toFloat(&parseOk);

        if(parseOk)
        {
          costMatrix.push_back(value);
        }
        else
        {
          std::stringstream ss;
          ss << "Unable to parse value from line " << dataLine << " in file " <<
            filename << " : " << buffer;
          throw Tgs::Exception(__LINE__, ss.str());
        }
      }

      dataLine++;
    }

    return costMatrix;
  }
  catch(const Tgs::Exception & e)
  {
    throw Tgs::Exception("CslUtilities", __FUNCTION__, __LINE__, e);
  }
}

}
