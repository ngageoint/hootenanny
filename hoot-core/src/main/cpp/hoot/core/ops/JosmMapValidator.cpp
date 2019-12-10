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
 * @copyright Copyright (C) 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "JosmMapValidator.h"

// hoot
#include <hoot/core/util/Log.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/JniConversion.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, JosmMapValidator)

JosmMapValidator::JosmMapValidator() :
JosmMapValidatorAbstract()
{
  // Don't see this changing any time soon, so not making it configurable until necessary
  _josmInterfaceName = "hoot/services/josm/JosmMapValidator";
}

OsmMapPtr JosmMapValidator::_getUpdatedMap(OsmMapPtr& inputMap)
{
  LOG_DEBUG("Retrieving validated map...");

  // call into hoot-josm to validate features in the map and convert result to a map
  jstring validatedMapJavaStr =
    (jstring)_javaEnv->CallObjectMethod(
      _josmInterface,
      // JNI sig format: (input params...)return type
      // Java sig: String validate(List<String> validators, String elementsXml)
      _javaEnv->GetMethodID(
        _josmInterfaceClass, "validate", "(Ljava/util/List;Ljava/lang/String;)Ljava/lang/String;"),
      // validators to use
      JniConversion::toJavaStringList(_javaEnv, _josmValidators),
      // convert input map to xml string to pass in
      JniConversion::toJavaString(_javaEnv, OsmXmlWriter::toString(inputMap, false)));
  if (_javaEnv->ExceptionCheck())
  {
    _javaEnv->ExceptionDescribe();
    _javaEnv->ExceptionClear();
    throw HootException("Error calling validate.");
  }

  return
    OsmXmlReader::fromXml(
      JniConversion::fromJavaString(_javaEnv, validatedMapJavaStr).trimmed(), true, true, false,
      true);
}

}
