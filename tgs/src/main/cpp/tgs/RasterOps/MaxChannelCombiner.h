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

#ifndef __TGS__MAX_CHANNEL_COMBINER_H__
#define __TGS__MAX_CHANNEL_COMBINER_H__

// Standard Includes
#include <iostream>
#include <list>
#include <sstream>
#include <vector>

#include <tgs/TgsExport.h>
#include <tgs/RasterOps/Image.hpp>

#ifdef SWIG
%{
#include "../Tgs/src/RasterOps/MaxChannelCombiner.h"
%}
#endif

namespace Tgs
{
  /**
   * Combines multiple channels into a single image based on the maximum value.
   */
# ifdef SWIG
    class MaxChannelCombiner
# else
    class TGS_EXPORT MaxChannelCombiner
# endif
  {
  public:

    /**
     * Combines a multi channel image into a single channel where the result contains the 
     * index + 1 of the channel with the highest value. If all the channels are zero a zero is 
     * returned. If two or more channels have matching non-zero values the result will be evenly 
     * and deterministically distributed among the matching channels (think modulo).
     * @param channels Channels to combine
     * @returns The combined image.
     */
    static Tgs::Image<unsigned char> combineChannels(
      const std::vector<const Tgs::Image<unsigned char>*>& channels);
   
    static Tgs::Image<unsigned char> combine3Channels(const Tgs::Image<unsigned char>& c1,
      const Tgs::Image<unsigned char>& c2, const Tgs::Image<unsigned char>& c3);
  };
}

#endif
