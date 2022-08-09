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

#include "SpinImageStack.h"

// Standard Includes
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

#include "SpinImage.h"
#include "../Xml/XmlHelper.h"

namespace Tgs
{

  SpinImageStack::~SpinImageStack()
  {
    clear();
  }

  void SpinImageStack::addImage(int id, SpinImage* image) 
  { 
    delete _images[id];
    _images[id] = image;
  }

  void SpinImageStack::clear()
  {
    for (unsigned int i = 0; i < _images.size(); i++)
    {
      delete _images[i];
      _images[i] = NULL;
    }
    _images.resize(0);
  }

  void SpinImageStack::exportStack(std::ostream& s, std::string tabDepth) const
  {
    s << tabDepth << "<SpinImageStack size=" << getSize() << ">" << endl;
    for (unsigned int i = 0; i < _images.size(); i++)
    {
      if (_images[i] != NULL)
      {
        _images[i]->exportImage(s, tabDepth + " ");
      }
      else
      {
        s << tabDepth << " <SpinImage/>";
      }
    }
    s << tabDepth << "</SpinImageStack>" << endl;
  }

  void SpinImageStack::importStack(std::istream& s)
  {
    string buffer;
    getline(s, buffer);
    HashMap<string, string> kv = XmlHelper::parseAttributes(buffer);
    int size = atoi(kv["size"].c_str());

    resize(size);
    for (int i = 0; i < size; i++)
    {
      SpinImage* si = new SpinImage();
      si->importImage(s);
      addImage(i, si);
    }

    getline(s, buffer); // </SpinImageStack>
  }

  void SpinImageStack::resize(int size) 
  { 
    clear();
    _images.resize(size, NULL); 
  }
}
