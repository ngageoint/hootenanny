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

#include "HadoopPipesUtils.h"

// Avoid some annoying warnings in HadoopPipes.cc
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wformat-security"
#define private public
#include <pipes/impl/HadoopPipes.cc>
#pragma GCC diagnostic warning "-Wunused-parameter"
#pragma GCC diagnostic warning "-Wformat-security"

// Pretty Pipes
#include <pp/Exception.h>
#include <pp/io/RecordReaderProvider.h>
#include <pp/io/RecordWriterProvider.h>

using namespace std;

namespace pp
{

HadoopPipesUtils::HadoopPipesUtils()
{
}

Configuration* HadoopPipesUtils::toConfiguration(const HadoopPipes::JobConf* conf)
{
  const Configuration* c = dynamic_cast<const Configuration*>(conf);
  if (c != NULL)
  {
    return new Configuration(*c);
  }
  const HadoopPipes::JobConfImpl* impl = dynamic_cast<const HadoopPipes::JobConfImpl*>(conf);
  if (impl != NULL)
  {
    return new Configuration(impl->values);
  }
  throw Exception("Error getting Configuration from JobConf.");
}

HadoopPipes::RecordReader* HadoopPipesUtils::getRecordReader(HadoopPipes::TaskContext* context)
{
  HadoopPipes::TaskContextImpl* impl = dynamic_cast<HadoopPipes::TaskContextImpl*>(context);
  RecordReaderProvider* p = dynamic_cast<RecordReaderProvider*>(context);
  if (impl != NULL)
  {
    return impl->reader;
  }
  else if (p != NULL)
  {
    return p->getRecordReader();
  }
  else
  {
    throw Exception("Error getting RecordReader from context.");
  }
}

HadoopPipes::RecordWriter* HadoopPipesUtils::getRecordWriter(HadoopPipes::TaskContext* context)
{
  HadoopPipes::TaskContextImpl* impl = dynamic_cast<HadoopPipes::TaskContextImpl*>(context);
  RecordWriterProvider* p = dynamic_cast<RecordWriterProvider*>(context);
  if (impl != NULL)
  {
    return impl->writer;
  }
  else if (p != NULL)
  {
    return p->getRecordWriter();
  }
  else
  {
    throw Exception("Error getting RecordWriter from context.");
  }
}

void HadoopPipesUtils::mergeFilesToLocalFileSystem(const string& filesDir,
                                                   const string& mergedFileOutputLocation)
{
  const string cmd = "hadoop fs -getmerge " + filesDir + " " + mergedFileOutputLocation;
  if (system(cmd.c_str()) != 0)
  {
    throw Exception("Failed merging files into single file: " + mergedFileOutputLocation);
  }
}

}

