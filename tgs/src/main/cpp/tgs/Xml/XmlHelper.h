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

#ifndef __TGS__XML_HELPER_H__
#define __TGS__XML_HELPER_H__

// Standard Includes
#include <string>
#include <vector>

#include "../TgsExport.h"
#include "../HashMap.h"

namespace Tgs
{
  /**
   * Provides some simple convenience function for efficiently reading XML. This is not attempting
   * to be a complete or strict XML interpreter.
   */
  class TGS_EXPORT XmlHelper
  {
  public:

    /**
     * Parses the attributes out of string. It is assumed that str is a single element. E.g.
     * parseAttributes("<Foo bar=1 alpha=2>") returns r['bar'] = '1', r['alpha'] = '2'
     */
    static HashMap<std::string, std::string> parseAttributes(const std::string& str);

    /**
     * Parses the value out of a tag. E.g. <Foo>MyText</Foo> will return 'Foo' for the tag and 
     * 'MyText' for the value. It is assumed that the text is all on one line.
     */
    static void parseTagValue(const std::string& str, std::string& tag, std::string& value);

    static std::vector<std::string> split(const std::string& str, const std::string& delimeter);

    static std::string trim(const std::string& str);
  };
}

#endif
