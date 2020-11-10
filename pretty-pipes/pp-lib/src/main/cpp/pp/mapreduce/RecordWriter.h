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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef RECORDWRITER_H
#define RECORDWRITER_H

#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <hadoop/Pipes.hh>
#pragma GCC diagnostic warning "-Wunused-parameter"

namespace pp
{

/**
 * This is here to avoid typeid (e.g. any pointer) errors in RHEL 5.8. That version of the
 * compiler (4.1.2) seems to require a virtual method implemented in the .cpp before this will
 * work.
 */
class RecordWriter : public HadoopPipes::RecordWriter
{
public:

  static std::string className() { return "pp::RecordWriter"; }

  RecordWriter();

  virtual ~RecordWriter();
};

}

#endif // RECORDWRITER_H
