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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef POPULATECONSUMERSJS_H
#define POPULATECONSUMERSJS_H

// hoot
#include <hoot/core/ConstOsmMapConsumer.h>
#include <hoot/core/algorithms/StringDistanceConsumer.h>
#include <hoot/core/algorithms/aggregator/ValueAggregatorConsumer.h>
#include <hoot/core/elements/ElementConsumer.h>
#include <hoot/core/filters/ElementCriterionConsumer.h>
#include <hoot/core/visitors/ElementVisitorConsumer.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/Settings.h>
#include <hoot/js/OsmMapJs.h>
#include <hoot/js/algorithms/StringDistanceJs.h>
#include <hoot/js/elements/ElementJs.h>
#include <hoot/js/filter/ElementCriterionJs.h>
#include <hoot/js/filter/JsFunctionCriterion.h>
#include <hoot/js/util/JsFunctionConsumer.h>
#include <hoot/js/visitors/ElementVisitorJs.h>
#include <hoot/js/algorithms/aggregator/ValueAggregatorJs.h>

// node.js
// #include <nodejs/node.h>
#include <hoot/js/SystemNodeJs.h>

#include "StringUtilsJs.h"

namespace hoot
{
using namespace node;

class PopulateConsumersJs
{
public:

  static Local<String> baseClass() { return String::New("baseClass"); }

  template <typename T>
  static void populateConsumers(T* consumer, const v8::Arguments& args)
  {
    for (int i = 0; i < args.Length(); i++)
    {
      populateConsumers<T>(consumer, args[i]);
    }
  }

  template <typename T>
  static void populateConsumers(T* consumer, const v8::Local<v8::Value>& v)
  {
    if (v->IsFunction())
    {
      populateFunctionConsumer<T>(consumer, v);
    }
    else if (v->IsObject())
    {
      Local<Object> obj = v->ToObject();

      if (obj->Has(baseClass()))
      {
        if (str(obj->Get(baseClass())) == QString::fromStdString(ElementCriterion::className()))
        {
          populateCriterionConsumer<T>(consumer, v);
        }
        else if (str(obj->Get(baseClass())) == QString::fromStdString(ElementVisitor::className()))
        {
          populateVisitorConsumer<T>(consumer, v);
        }
        else if (str(obj->Get(baseClass())) == QString::fromStdString(Element::className()))
        {
          populateElementConsumer<T>(consumer, v);
        }
        else if (str(obj->Get(baseClass())) == QString::fromStdString(StringDistance::className()))
        {
          populateStringDistanceConsumer<T>(consumer, v);
        }
        else if (str(obj->Get(baseClass())) == QString::fromStdString(ValueAggregator::className()))
        {
          populateValueAggregatorConsumer<T>(consumer, v);
        }
        else if (str(obj->Get(baseClass())) == QString::fromStdString(OsmMap::className()))
        {
          populateOsmMapConsumer<T>(consumer, v);
        }
        else
        {
          throw IllegalArgumentException("Unexpected object passed to consumer " +
                                         str(obj->Get(baseClass())));
        }
      }
      else
      {
        populateConfigurable<T>(consumer, obj);
      }
    }
  }

  template <typename T>
  static void populateConfigurable(T* consumer, const Local<Object>& obj)
  {
    Settings settings = conf();

    Local<Array> keys = obj->GetPropertyNames();
    if (keys->Length() == 0)
    {
      LOG_WARN("Populating object with empty configuration. Is this what you wanted?");
    }

    for (uint32_t i = 0; i < keys->Length(); i++)
    {
      Local<String> k = keys->Get(i)->ToString();
      Local<String> v = obj->Get(k)->ToString();
      settings.set(str(k), str(v));
    }

    Configurable* c = dynamic_cast<Configurable*>(consumer);
    if (c == 0)
    {
      throw IllegalArgumentException("Object does not accept custom settings as an argument.");
    }

    c->setConfiguration(settings);
  }

  template <typename T>
  static void populateCriterionConsumer(T* consumer, const v8::Local<v8::Value>& v)
  {
    ElementCriterionJs* obj = ObjectWrap::Unwrap<ElementCriterionJs>(v->ToObject());

    ElementCriterionConsumer* c = dynamic_cast<ElementCriterionConsumer*>(consumer);

    if (c == 0)
    {
      throw IllegalArgumentException("Object does not accept ElementCriterion as an argument.");
    }
    else
    {
      c->addCriterion(obj->getCriterion());
    }
  }

  template <typename T>
  static void populateElementConsumer(T* consumer, const v8::Local<v8::Value>& v)
  {
    ElementJs* obj = ObjectWrap::Unwrap<ElementJs>(v->ToObject());

    ElementConsumer* c = dynamic_cast<ElementConsumer*>(consumer);

    if (c == 0)
    {
      throw IllegalArgumentException("Object does not accept Element as an argument.");
    }
    else
    {
      c->addElement(obj->getElement());
    }
  }

  template <typename T>
  static void populateFunctionConsumer(T* consumer, const v8::Local<v8::Value>& v)
  {
    if (v.IsEmpty() || v->IsFunction() == false)
    {
      throw IllegalArgumentException("Expected the argument to be a valid function.");
    }

    Persistent<Function> func = Persistent<Function>::New(Handle<Function>::Cast(v));
    JsFunctionConsumer* c = dynamic_cast<JsFunctionConsumer*>(consumer);
    ElementCriterionConsumer* ecc = dynamic_cast<ElementCriterionConsumer*>(consumer);

    if (c != 0 && ecc != 0)
    {
      // At the time of this writing this isn't possible. Give a good hard think about how the code
      // should respond before you change it.
      throw IllegalArgumentException("Ambiguous consumption of both a function and an "
        "ElementCriterionConsumer.");
    }
    else if (c != 0)
    {
      c->addFunction(func);
    }
    else if (ecc != 0)
    {
      shared_ptr<JsFunctionCriterion> ecp(new JsFunctionCriterion());
      ecp->addFunction(func);
      ecc->addCriterion(ecp);
    }
    else
    {
      throw IllegalArgumentException("Object does not accept a function as an argument.");
    }
  }

  template <typename T>
  static void populateOsmMapConsumer(T* consumer, const v8::Local<v8::Value>& v)
  {
    OsmMapJs* obj = ObjectWrap::Unwrap<OsmMapJs>(v->ToObject());

    if (obj->isConst())
    {
      ConstOsmMapConsumer* c = dynamic_cast<ConstOsmMapConsumer*>(consumer);

      if (c == 0)
      {
        throw IllegalArgumentException("Object does not accept const OsmMap as an argument. Maybe "
          "try a non-const OsmMap?");
      }
      else
      {
        c->setOsmMap(obj->getConstMap().get());
      }
    }
    else
    {
      OsmMapConsumer* c = dynamic_cast<OsmMapConsumer*>(consumer);

      if (c == 0)
      {
        throw IllegalArgumentException("Object does not accept OsmMap as an argument.");
      }
      else
      {
        c->setOsmMap(obj->getMap().get());
      }
    }
  }

  template <typename T>
  static void populateStringDistanceConsumer(T* consumer, const Local<Value>& value)
  {
    StringDistancePtr sd = toCpp<StringDistancePtr>(value);

    StringDistanceConsumer* c = dynamic_cast<StringDistanceConsumer*>(consumer);

    if (c == 0)
    {
      throw IllegalArgumentException("Object does not accept StringDistance as an argument.");
    }
    else
    {
      c->setStringDistance(sd);
    }
  }

  template <typename T>
  static void populateValueAggregatorConsumer(T* consumer, const Local<Value>& value)
  {
    ValueAggregatorPtr va = toCpp<ValueAggregatorPtr>(value);

    ValueAggregatorConsumer* c = dynamic_cast<ValueAggregatorConsumer*>(consumer);

    if (c == 0)
    {
      throw IllegalArgumentException("Object does not accept ValueAggregator as an argument.");
    }
    else
    {
      c->setValueAggregator(va);
    }
  }

  template <typename T>
  static void populateVisitorConsumer(T* consumer, const v8::Local<v8::Value>& v)
  {
    ElementVisitorJs* obj = ObjectWrap::Unwrap<ElementVisitorJs>(v->ToObject());

    ElementVisitorConsumer* c = dynamic_cast<ElementVisitorConsumer*>(consumer);

    if (c == 0)
    {
      throw IllegalArgumentException("Object does not accept ElementCriterion as an argument.");
    }
    else
    {
      c->addVisitor(obj->getVisitor());
    }
  }

};

}

#endif // POPULATECONSUMERSJS_H

