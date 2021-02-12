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
 * @copyright Copyright (C) 2015, 2017 Maxar (http://www.maxar.com/)
 */

#ifndef __TGS__TREE_GENOME_NODE_H__
#define __TGS__TREE_GENOME_NODE_H__

#include "../TgsExport.h"

namespace Tgs
{
  class TGS_EXPORT TreeGenomeNode
  {
  public:

    TreeGenomeNode() {}

    virtual ~TreeGenomeNode() {}

    virtual boost::shared_ptr<TreeGenomeNode> clone() const = 0;

    /**
     * Copies the specified genome and all of its children into this one.
     */
    virtual void copy(const TreeGenomeNode& from) = 0;

    virtual void mutate(double p) = 0;

  protected:
  };
}

#endif
