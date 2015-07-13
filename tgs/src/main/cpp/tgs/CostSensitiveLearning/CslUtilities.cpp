#include "CslUtilities.h"

//Qt Includes
#include <QString>

//Std Includes
#include <fstream>
#include <iostream>

#include "TgsException.h"

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
