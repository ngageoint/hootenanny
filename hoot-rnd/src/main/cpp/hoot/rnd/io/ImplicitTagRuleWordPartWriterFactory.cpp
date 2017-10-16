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
#include "ImplicitTagRuleWordPartWriterFactory.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/ConfigOptions.h>

using namespace std;

namespace hoot
{

boost::shared_ptr<ImplicitTagRuleWordPartWriterFactory> ImplicitTagRuleWordPartWriterFactory::_theInstance;

ImplicitTagRuleWordPartWriterFactory::ImplicitTagRuleWordPartWriterFactory()
{
}

ImplicitTagRuleWordPartWriterFactory& ImplicitTagRuleWordPartWriterFactory::getInstance()
{
  if (!_theInstance.get())
  {
    _theInstance.reset(new ImplicitTagRuleWordPartWriterFactory());
  }
  return *_theInstance;
}

boost::shared_ptr<ImplicitTagRuleWordPartWriter> ImplicitTagRuleWordPartWriterFactory::createWriter(
  QString url)
{
  LOG_VART(url);

  vector<std::string> names =
    Factory::getInstance().getObjectNamesByBase(ImplicitTagRuleWordPartWriter::className());
  boost::shared_ptr<ImplicitTagRuleWordPartWriter> writer;
  for (size_t i = 0; i < names.size() && !writer; ++i)
  {
    LOG_VART(names[i]);
    writer.reset(Factory::getInstance().constructObject<ImplicitTagRuleWordPartWriter>(names[i]));
    if (writer->isSupported(url))
    {
      LOG_DEBUG("Using implicit tag rules writer: " << names[i]);
    }
    else
    {
      writer.reset();
    }
  }

  if (!writer)
  {
    throw HootException("A valid writer could not be found for the URL: " + url);
  }

  return writer;
}

}
