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

#ifndef CPPSEQFILERECORDWRITER_H
#define CPPSEQFILERECORDWRITER_H

// Boost
#include <boost/shared_ptr.hpp>

// Pretty Pipes
#include <pp/ReduceContextConsumer.h>
#include <pp/io/CppSeqFile.h>
#include <pp/io/RecordWriter.h>

namespace pp
{

class CppSeqFileRecordWriter : public pp::RecordWriter, public pp::ReduceContextConsumer
{
public:

  static std::string className() { return "pp::CppSeqFileRecordWriter"; }

  CppSeqFileRecordWriter();

  virtual ~CppSeqFileRecordWriter();

  virtual void emitRecord(const char* keyData, size_t keySize, const char* valueData,
    size_t valueSize);

  virtual void setReduceContext(HadoopPipes::ReduceContext& context);

private:
  std::string _path;
  boost::shared_ptr<std::ostream> _out;
  CppSeqFile::Writer* _writer;
};

}

#endif // CPPSEQFILERECORDWRITER_H
