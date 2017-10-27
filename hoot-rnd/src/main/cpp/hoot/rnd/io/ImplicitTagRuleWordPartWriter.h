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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef IMPLICITTAGRULEWORDPARTWRITER_H
#define IMPLICITTAGRULEWORDPARTWRITER_H

namespace hoot
{

/**
 * Interface for writing implicit tag rules to output
 */
class ImplicitTagRuleWordPartWriter
{

public:

  static std::string className() { return "hoot::ImplicitTagRuleWordPartWriter"; }

  /**
   *  Writes a file containing implicit tag word rule parts to output
   *
   * @param totalParts optional line count of the input file
   */
  virtual void write(const QString inputUrl) = 0;

  /**
   * Returns true if the output URL is supported.
   */
  virtual bool isSupported(const QString outputUrl) = 0;

  /**
   * Opens the specified URL for writing.
   */
  virtual void open(const QString outputUrl) = 0;

  /**
   * Closes the output file
   */
  virtual void close() = 0;

};

}

#endif // IMPLICITTAGRULEWORDPARTWRITER_H
