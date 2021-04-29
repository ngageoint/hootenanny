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
 * @copyright Copyright (C) 2015, 2017, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef STDIODEVICE_H
#define STDIODEVICE_H

// Qt
#include <QIODevice>

// Standard
#include <istream>

namespace Tgs
{

/**
 * Wraps the standard istream as a QIODevice.
 */
class StdIoDevice : public QIODevice
{
public:

  /**
   * Constructs the device and assume ownership of in.
   */
  StdIoDevice(std::istream& in);
  ~StdIoDevice() = default;

  bool atEnd() const override;

  bool open(QIODevice::OpenMode mode) override { QIODevice::open(mode); return true; }

  void close() override { delete _in; _in = nullptr; }

  void flush() { }

  qint64 readData(char* data, qint64 maxlen) override;

  qint64 writeData(const char *data, qint64 len) override;

  bool getChar(char* c) { return readData(c, 1) == 1; }

private:

  std::istream* _in;
};

}

#endif // STDIODEVICE_H
