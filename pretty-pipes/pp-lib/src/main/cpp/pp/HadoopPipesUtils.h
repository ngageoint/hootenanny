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
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef HADOOPPIPESUTILS_H
#define HADOOPPIPESUTILS_H

// Hadoop Pipes
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <hadoop/Pipes.hh>
#pragma GCC diagnostic warning "-Wunused-parameter"

// Pretty Pipes
#include <pp/conf/Configuration.h>

namespace pp
{

/**
 * There are a lot of useful member variables that are not exposed through Hadoop Pipes. This
 * massive hack works around that problem. At some point it probably makes sense to copy out
 * Hadoop Pipes and maintain our own version, but until that time this will work.
 */
class HadoopPipesUtils
{

public:

  HadoopPipesUtils();

  static Configuration* toConfiguration(const HadoopPipes::JobConf* conf);

  static HadoopPipes::RecordReader* getRecordReader(HadoopPipes::TaskContext* context);

  static HadoopPipes::RecordWriter* getRecordWriter(HadoopPipes::TaskContext* context);

  /**
   * Merges all files in the specified HDFS dir into a single file on the local file system.
   *
   * @param filesDir HDFS directory containing the files to merge
   * @param mergedFileOutputLocation single output file
   */
  static void mergeFilesToLocalFileSystem(const std::string& filesDir,
                                          const std::string& mergedFileOutputLocation);
};

}

#endif // HADOOPPIPESUTILS_H
