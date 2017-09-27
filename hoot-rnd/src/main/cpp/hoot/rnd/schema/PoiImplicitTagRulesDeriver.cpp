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
 * @copyright Copyright (C) 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "PoiImplicitTagRulesDeriver.h"

// hoot
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/io/PartialOsmMapReader.h>
#include <hoot/core/io/ElementInputStream.h>
#include <hoot/rnd/io/ImplicitTagRulesJsonWriter.h>

namespace hoot
{

PoiImplicitTagRulesDeriver::PoiImplicitTagRulesDeriver()
{
}

void PoiImplicitTagRulesDeriver::generateList(const QString input, const QString output)
{
  boost::shared_ptr<PartialOsmMapReader> inputReader =
    boost::dynamic_pointer_cast<PartialOsmMapReader>(
      OsmMapReaderFactory::getInstance().createReader(input));
  inputReader->open(input);
  boost::shared_ptr<ElementInputStream> inputStream =
    boost::dynamic_pointer_cast<ElementInputStream>(inputReader);

  assert(output.endsWith(".json"));
  ImplicitTagRulesJsonWriter listWriter;

  while (inputStream->hasMoreElements())
  {
    ElementPtr element = inputStream->readNextElement();

  }

  inputReader->finalizePartial();
  //listWriter.close();
}

}
