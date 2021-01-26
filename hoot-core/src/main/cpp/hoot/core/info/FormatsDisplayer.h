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
 * @copyright Copyright (C) 2018, 2019, 2021 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef FORMATSDISPLAYER_H
#define FORMATSDISPLAYER_H

// Qt
#include <QString>

namespace hoot
{

/**
 * Displays supported IO formats
 */
class FormatsDisplayer
{
public:

  /**
   * Displays supported IO formats
   *
   * @param displayInputs if true, displays input formats
   * @param displayInputsSupportingStreaming if true, displays streamable input formats
   * @param displayInputsSupportingBounds if true, displays input formats that supported bounded
   * reading
   * @param displayOutputs if true, displays output formats
   * @param displayOutputsSupportingStreaming if true, displays streamable output formats
   * @param displayOgrOnly TODO
   * @return a string describing the requested formats
   */
  static QString display(const bool displayInputs, const bool displayInputsSupportingStreaming,
                         const bool displayInputsSupportingBounds, const bool displayOutputs,
                         const bool displayOutputsSupportingStreaming, const bool displayOgrOnly);

private:

  template<typename IoClass>
  static QString _getFormatsString(
    const QString& className, const QStringList extraFormats = QStringList(),
    const bool ogrOnly = false, const bool ogrReadOnly = false);

  template<typename IoClass>
  static QStringList _getFormats(
    const QString& className, const QStringList extraFormats = QStringList(),
    const bool ogrOnly = false, const bool ogrReadOnly = false);

  static QString _getFormatsSupportingBoundsString(const bool ogrOnly = false);

  static QString _getInputFormatsSupportingStreamingString(const bool ogrOnly = false);
  static QString _getOutputFormatsSupportingStreamingString(const bool ogrOnly = false);

  static  QString _getPrintableString(const QStringList& items);
};

}

#endif // FORMATSDISPLAYER_H
