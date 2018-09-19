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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "ToEnglishTranslatorFactory.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Log.h>

// Boost
#include <boost/make_shared.hpp>

namespace hoot
{

boost::shared_ptr<ToEnglishTranslatorFactory> ToEnglishTranslatorFactory::_theInstance;

ToEnglishTranslatorFactory::ToEnglishTranslatorFactory()
{
}

ToEnglishTranslatorFactory& ToEnglishTranslatorFactory::getInstance()
{
  if (!_theInstance.get())
  {
    _theInstance.reset(new ToEnglishTranslatorFactory());
  }
  return *_theInstance;
}

boost::shared_ptr<ToEnglishTranslator> ToEnglishTranslatorFactory::create(const QString className)
{
  LOG_VART(className);
  boost::shared_ptr<ToEnglishTranslator> translator;
  if (className == "hoot::DictionaryTranslator")
  {
    if (!_dictTranslator.get())
    {
      // This isn't great, b/c DictionaryTranslator will be initialized twice here, once during
      // getInstance and then once with make_shared...but at least there will be no more copies
      // after.  Long story short, this was the best I could do to get the DictionaryTranslator
      // singleton returning from this factory without having to hack the Factory and NamedOp
      // classes with some special init cases.
      DictionaryTranslator& dictTranslator = DictionaryTranslator::getInstance();
      _dictTranslator = boost::make_shared<DictionaryTranslator>(dictTranslator);
    }
    translator = _dictTranslator;
  }
  else
  {
    translator.reset(Factory::getInstance().constructObject<ToEnglishTranslator>(className));
  }
  return translator;
}

}
