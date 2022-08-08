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
 * @copyright Copyright (C) 2015, 2017, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#ifndef __TGS__SPIN_IMAGE_STACK_H__
#define __TGS__SPIN_IMAGE_STACK_H__

#include "../TgsExport.h"
#include "SpinImage.h"

// Standard Includes
#include <vector>

namespace Tgs
{
  class SpinImage;

  class  TGS_EXPORT SpinImageStack
  {
  public:
    SpinImageStack() {}

    virtual ~SpinImageStack();

    void addImage(int id, SpinImage* image);

    void clear();

    const SpinImage* getImage(int id) const { return _images[id]; }

    void resize(int size);

    unsigned int getSize() const { return _images.size(); }

    void exportStack(std::ostream& s, std::string tabDepth) const;

    void importStack(std::istream& s);

    friend inline bool operator==(const SpinImageStack& s1, const SpinImageStack& s2);

  private:
    std::vector<SpinImage*> _images;
  };

  inline bool operator==(const SpinImageStack& s1, const SpinImageStack& s2)
  {
    bool result = s1._images.size() == s2._images.size();

    if (result)
    {
      for (unsigned int i = 0; i < s1._images.size(); i++)
      {
        result &= (*s1._images[i]) == (*s2._images[i]);
      }
    }

    return result;
  }

  inline std::ostream& operator<<(std::ostream& s, const SpinImageStack& si)
  {
    si.exportStack(s, "");
    return s;
  }
}

#endif
