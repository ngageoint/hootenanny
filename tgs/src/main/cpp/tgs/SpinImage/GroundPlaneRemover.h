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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#ifndef __TGS__GROUND_PLANE_REMOVER_H__
#define __TGS__GROUND_PLANE_REMOVER_H__

// Standard Includes
#include <queue>

#include <tgs/HashMap.h>
#include <tgs/TgsExport.h>
#include <tgs/SpinImage/NormalEstimator.h>

class GroundPlaneRemoverTest;

namespace Tgs
{
  /**
   */
  class TGS_EXPORT GroundPlaneRemover : public NormalEstimator
  {
  public:

    virtual ~GroundPlaneRemover() {}

    /**
     * This function will calculate normals and remove the ground plane all at once.
     */
    void removeGroundPlane(PointCloud& pc);

  protected:

    bool _areNeighborsVertical(int id) const;

    bool _isVertical(const Point3d& p) const;
  };
}

#endif
