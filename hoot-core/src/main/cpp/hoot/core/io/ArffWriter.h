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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef ARFFWRITER_H
#define ARFFWRITER_H

// Qt
#include <QString>

// Standard
#include <fstream>
#include <map>
#include <memory>
#include <vector>

namespace hoot
{

/**
 * @brief The ArffWriter class
 *
 * Special rules:
 *  - The only enumerated class supported is "class" which expects one of the MatchType classes.
 *  - The "class" column will always be last.
 */
class ArffWriter
{
public:

  typedef std::map<QString, double> Sample;

  /**
   * @brief ArffWriter
   * @param strm Does not take ownership.
   * @param useNulls if true writes "?" for null values. Otherwise writes "-1".
   */
  ArffWriter(std::ostream* strm, bool useNulls);
  ArffWriter(QString path, bool useNulls);

  /**
   * @brief writes a vector of samples to the given output stream.
   *
   * Each time this method is called it writes an entirely new arff file. This should only really
   * be called once unless you're doing some other fanciness to demarcate the beginning/end.
   *
   * @param samples vector of samples to write.
   */
  void write(const std::vector<Sample>& samples);

private:

  QString _path;
  std::shared_ptr<std::fstream> _autoStrm;
  std::ostream* _strm;
  bool _useNulls;

  void _w(const QString& s);
};

}

#endif // ARFFWRITER_H
