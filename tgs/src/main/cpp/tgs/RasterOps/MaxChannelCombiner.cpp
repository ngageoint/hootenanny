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
 * @copyright Copyright (C) 2015, 2018, 2019 Maxar (http://www.maxar.com/)
 */

#include "MaxChannelCombiner.h"

// Standard Includes
using namespace std;

#include <tgs/TgsException.h>

namespace Tgs
{

  Tgs::Image<unsigned char> MaxChannelCombiner::combine3Channels(
    const Tgs::Image<unsigned char>& c1, const Tgs::Image<unsigned char>& c2, 
    const Tgs::Image<unsigned char>& c3)
  {
    std::vector<const Image<unsigned char>* > channels;
    channels.push_back(&c1);
    channels.push_back(&c2);
    channels.push_back(&c3);

    return combineChannels(channels);
  }

  Image<unsigned char> MaxChannelCombiner::combineChannels(
    const std::vector<const Image<unsigned char>* >& channels)
  {
    if (channels.size() == 0 || channels.size() > 254)
    {
      throw Tgs::Exception("There must be between 1 and 254 channels.");
    }

    int width = channels[0]->getWidth();
    int height = channels[0]->getHeight();

    if (width <= 0 || height <= 0)
    {
      throw Tgs::Exception("Width and height must be greater than zero.");
    }

    for (unsigned int i = 0; i < channels.size(); i++)
    {
      if (channels[i]->isValid() == false || 
        channels[i]->getWidth() != width ||
        channels[i]->getHeight() != height)
      {
        throw Tgs::Exception("All channels must be valid and the same size.");
      }
    }

    Image<unsigned char> result(width, height);

    int max;
    vector<int> maxChannels(channels.size());
    int mcCount = 0;
    for (int y = 0; y < height; y++)
    {
      for (int x = 0; x < width; x++)
      {
        max = 0;
        mcCount = 0;
        for (unsigned int c = 0; c < channels.size(); c++)
        {
          unsigned char v = channels[c]->pixel(x, y);
          if (v > max)
          {
            mcCount = 0;
            max = v;
          }
          if (v == max && max > 0)
          {
            maxChannels[mcCount++] = c + 1;
          }
        }
        if (max == 0)
        {
          result.pixel(x, y) = 0;
        }
        else
        {
          result.pixel(x, y) = maxChannels[(x + y) % mcCount];
        }
      }
    }

    return result;
  }
}
