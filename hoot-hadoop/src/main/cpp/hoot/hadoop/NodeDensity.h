/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#ifndef NODEDENSITY_H
#define NODEDENSITY_H

// Qt
#include <QHash>

namespace hoot
{
using namespace std;

class NodeDensity
{
public:
  NodeDensity() { reset(0, 0); }

  NodeDensity(int width, int height);

  int getPixel(int px, int py)
  { return _count.contains(py * _width + px) ?_count[py * _width + px] : 0; }

  int getWidth() const { return _width; }

  void reset() { _count.clear(); }

  void reset(int width, int height);

  void setPixel(int px, int py, int v) { _count[py * _width + px] = v; }

  long size() { return _count.size(); }

  const QHash<long, int>& getHash() const { return _count; }

private:
  QHash<long, int> _count;
  int _width;
  int _height;
};

}

#endif // NODEDENSITY_H
