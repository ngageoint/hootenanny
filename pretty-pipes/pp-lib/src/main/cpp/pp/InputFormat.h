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
 * @copyright Copyright (C) 2015 Maxar (http://www.maxar.com/)
 */

#ifndef INPUTFORMAT_H
#define INPUTFORMAT_H

// Pretty Pipes
#include <pp/conf/Configuration.h>

#include "Exception.h"
#include "InputSplit.h"

namespace pp
{

class InputFormat
{
public:
  static std::string className() { return "pp::InputFormat"; }

  static InputFormat* create(const std::string& className) throw(Exception);

  InputFormat();

  virtual ~InputFormat();

  virtual const pp::InputSplit& getSplit(int i) = 0;

  virtual int getSplitCount() = 0;

  virtual void setConfiguration(const Configuration& conf) = 0;
};

}

#endif // INPUTFORMAT_H
