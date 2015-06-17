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

#include "PaintNodesReducer.h"

// Hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>

// Pretty Pipes
#include <pp/Factory.h>
#include <pp/HadoopPipesUtils.h>
#include <pp/Hdfs.h>
#include <pp/io/CppSeqFileRecordWriter.h>

// Standard
#include <string>
#include <sstream>
using namespace std;

namespace hoot
{

PP_FACTORY_REGISTER(pp::Reducer, PaintNodesReducer)

PaintNodesReducer::PaintNodesReducer()
{
  _writer = NULL;
}

void PaintNodesReducer::reduce(HadoopPipes::ReduceContext& context)
{
  if (_writer == NULL)
  {
    HadoopPipes::RecordWriter* writer = pp::HadoopPipesUtils::getRecordWriter(&context);
    _writer = dynamic_cast<pp::RecordWriter*>(writer);
    if (_writer == NULL)
    {
      throw HootException("Error getting RecordWriter.");
    }
  }

  int* count;

  const string& key = context.getInputKey();
  if (key.size() == sizeof(Pixel))
  {
    Pixel* p = (Pixel*)(key.data());
    int sum = 0;

    while (context.nextValue())
    {
      const string& value = context.getInputValue();
      count = (int*)(value.data());
      sum += *count;
    }

    _writer->emitRaw<Pixel, int>(*p, sum);
  }
  else
  {
    throw HootException("Unexpected key size.");
  }
}

}
