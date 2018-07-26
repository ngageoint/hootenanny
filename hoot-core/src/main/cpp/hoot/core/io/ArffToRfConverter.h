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
 * @copyright Copyright (C) 2015, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef ARFFTORFCONVERTER_H
#define ARFFTORFCONVERTER_H

// boost
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/shared_ptr.hpp>

// Qt
#include <QString>

// Standard
#include <fstream>
#include <map>
#include <memory>
#include <vector>

namespace hoot
{

class DataSamples;

/**
 *
 */
class ArffToRfConverter
{
public:

  ArffToRfConverter();

  /**
   *
   *
   * @param input
   * @param output
   */
  void convert(const QString input, const QString output);

};

}

#endif // ARFFTORFCONVERTER_H
