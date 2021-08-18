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

#ifndef PAINTNODESREDUCER_H
#define PAINTNODESREDUCER_H

// Pretty Pipes
#include <pp/io/RecordWriter.h>
#include <pp/mapreduce/Reducer.h>

// Qt
#include <QString>

namespace hoot
{

class PaintNodesReducer : public pp::Reducer
{
public:
  class Pixel
  {
  public:
    uint16_t x;
    uint16_t y;

    QString toString() const
    {
      return QString("%1, %2").arg(x).arg(y);
    }
  };

  static std::string className() { return "hoot::PaintNodesReducer"; }

  PaintNodesReducer();

  void reduce(HadoopPipes::ReduceContext& context);

private:
  pp::RecordWriter* _writer;

};

}

#endif // PAINTNODESREDUCER_H
