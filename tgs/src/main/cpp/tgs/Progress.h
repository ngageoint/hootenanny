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

#ifndef __TGS__PROGRESS__
#define __TGS__PROGRESS__

namespace Tgs
{
  /**
   * Used to report progress 
   */
  class TgsProgress
  {
  public:

    virtual ~TgsProgress() {}
    /** 
     * Creates a new progress object that is a child of this progress object. The child object 
     * will be calculated as a portion of this progress' value.
     * @param name The user readable name for this child.
     * @param weight The weight of this child. Valid weights are 0 to 1. All weights should add 
     *  up to 1.
     */
    virtual Tgs::TgsProgress* createTgsChild(const std::string& name, double weight) = 0;

    /**
     * Set progress value 
     * @param progressMeter value between 0 and 1
     */
    virtual void setProgress(double p) = 0;
  };
}

#endif

