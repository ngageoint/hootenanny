/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#ifndef CONFLATEDRIVER_H
#define CONFLATEDRIVER_H

// Geos
#include <geos/geom/Envelope.h>

// hoot
#include <hoot/hadoop/Driver.h>

// Qt
#include <QString>

// Standard
#include <vector>

namespace hoot
{

class ConflateDriver : public Driver
{
public:
  ConflateDriver();

  /**
   * @param in Path to the input directory on HDFS
   * @param envelopes Envelopes of regions to conflate. The envelopes must be buffered before the
   *    call.
   * @param buffer The size of the buffer in degrees.
   * @param out The output path on HDFS.
   */
  void conflate(QString in, std::vector<geos::geom::Envelope> envelopes, double buffer, QString out);

private:

  std::string _toString(const std::vector<geos::geom::Envelope>& e);
};

}

#endif // CONFLATEDRIVER_H
