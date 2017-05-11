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

#include "SqlStatementLineRecordWriter.h"

// Hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/io/ApiDb.h>

// Pretty Pipes
#include <pp/Factory.h>
#include <pp/HadoopPipesUtils.h>
#include <pp/Hdfs.h>

// Qt
#include <QMap>

using namespace pp;
using namespace std;

namespace hoot
{

PP_FACTORY_REGISTER(pp::RecordWriter, SqlStatementLineRecordWriter)

SqlStatementLineRecordWriter::SqlStatementLineRecordWriter()
{
}

void SqlStatementLineRecordWriter::emitRecord(const char* keyData, size_t keySize,
                                              const char* valueData, size_t valueSize)
{
  ostream& os = *_out;
  if (os.good() == false)
  {
    throw Exception("output stream is not good.");
  }
  os.write(keyData, keySize);
  //this is a hack of sorts to get around a problem I was having with an extra tab being written
  //between the key and value pair when I didn't want it to be; will look into a better way to deal
  //with it at some point...
  //os.write("\t", 1);
  os.write(valueData, valueSize);
  os.write("\n", 1);
}

}
