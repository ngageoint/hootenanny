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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef POPULATECONSUMERSJS_H
#define POPULATECONSUMERSJS_H

// hoot
#include <hoot/core/algorithms/string/StringDistanceConsumer.h>
#include <hoot/core/algorithms/aggregator/ValueAggregatorConsumer.h>
#include <hoot/core/criterion/ElementCriterionConsumer.h>
#include <hoot/core/elements/ConstOsmMapConsumer.h>
#include <hoot/core/elements/ElementConsumer.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/visitors/ElementVisitorConsumer.h>
#include <hoot/core/visitors/MultipleCriterionConsumerVisitor.h>

#include <hoot/js/algorithms/string/StringDistanceJs.h>
#include <hoot/js/algorithms/aggregator/ValueAggregatorJs.h>
#include <hoot/js/criterion/ElementCriterionJs.h>
#include <hoot/js/criterion/JsFunctionCriterion.h>
#include <hoot/js/elements/ElementJs.h>
#include <hoot/js/elements/OsmMapJs.h>
#include <hoot/js/util/JsFunctionConsumer.h>
#include <hoot/js/util/StringUtilsJs.h>
#include <hoot/js/visitors/ElementVisitorJs.h>

// node.js
#include <hoot/js/SystemNodeJs.h>

namespace hoot
{

class PopulateConsumersJs
{
public:

  static v8::Local<v8::String> baseClass()
  { return v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), "baseClass").ToLocalChecked(); }

  template <typename T>
  static void populateConsumers(T* consumer, const v8::FunctionCallbackInfo<v8::Value>& args)
  {
    for (int i = 0; i < args.Length(); i++)
    {
      populateConsumers<T>(consumer, args[i]);
    }
  }

  template <typename T>
  static void populateConsumers(T* consumer, const v8::Local<v8::Value>& v)
  {
    v8::Isolate* current = v8::Isolate::GetCurrent();
    v8::HandleScope scope(current);
    v8::Local<v8::Context> context = current->GetCurrentContext();
    if (v->IsFunction())
    {
      populateFunctionConsumer<T>(consumer, v);
    }
    else if (v->IsObject())
    {
      v8::Local<v8::Object> obj = v->ToObject(context).ToLocalChecked();

      if (obj->Has(context, baseClass()).ToChecked())
      {
        if (str(obj->Get(context, baseClass()).ToLocalChecked()) == ElementCriterion::className())
        {
          populateCriterionConsumer<T>(consumer, v);
        }
        else if (str(obj->Get(context, baseClass()).ToLocalChecked()) == ElementVisitor::className())
        {
          populateVisitorConsumer<T>(consumer, v);
        }
        else if (str(obj->Get(context, baseClass()).ToLocalChecked()) == Element::className())
        {
          populateElementConsumer<T>(consumer, v);
        }
        else if (str(obj->Get(context, baseClass()).ToLocalChecked()) == StringDistance::className())
        {
          populateStringDistanceConsumer<T>(consumer, v);
        }
        else if (str(obj->Get(context, baseClass()).ToLocalChecked()) == ValueAggregator::className())
        {
          populateValueAggregatorConsumer<T>(consumer, v);
        }
        else if (str(obj->Get(context, baseClass()).ToLocalChecked()) == OsmMap::className())
        {
          populateOsmMapConsumer<T>(consumer, v);
        }
        else
        {
          throw IllegalArgumentException(
            "Unexpected object passed to consumer " + str(obj->Get(context, baseClass()).ToLocalChecked()));
        }
      }
      else
      {
        populateConfigurable<T>(consumer, obj);
      }
    }
  }

  template <typename T>
  static void populateConfigurable(T* consumer, const v8::Local<v8::Object>& obj)
  {
    LOG_TRACE("Populating configurable...");

    v8::Isolate* current = v8::Isolate::GetCurrent();
    v8::HandleScope scope(current);
    v8::Local<v8::Context> context = current->GetCurrentContext();

    Settings settings = conf();

    v8::Local<v8::Array> keys = obj->GetPropertyNames(context).ToLocalChecked();
    if (keys->Length() == 0)
    {
      LOG_WARN("Populating object with empty configuration. Is this what you wanted?");
    }

    for (uint32_t i = 0; i < keys->Length(); i++)
    {
      v8::Local<v8::String> k = keys->Get(context, i).ToLocalChecked()->ToString(context).ToLocalChecked();
      v8::Local<v8::String> v = obj->Get(context, k).ToLocalChecked()->ToString(context).ToLocalChecked();
      LOG_VART(str(k));
      LOG_VART(str(v));
      settings.set(str(k), str(v));
    }

    Configurable* c = dynamic_cast<Configurable*>(consumer);
    if (c == nullptr)
    {
      throw IllegalArgumentException(
        "Object does not accept custom settings as an argument: " + str(obj->Get(context, baseClass()).ToLocalChecked()));
    }

    // Configuration from Javascript for criterion consumers is handled a little differently where
    // we expect the child crits to be configured separately outside of the parent visitor.
    MultipleCriterionConsumerVisitor* multipleCritVis =
      dynamic_cast<MultipleCriterionConsumerVisitor*>(consumer);
    LOG_VART(multipleCritVis == nullptr);
    if (multipleCritVis != nullptr)
    {
      multipleCritVis->setConfigureChildren(false);
    }

    c->setConfiguration(settings);
  }

  template <typename T>
  static void populateCriterionConsumer(T* consumer, const v8::Local<v8::Value>& v)
  {
    LOG_TRACE("Populating criterion consumer...");

    v8::Isolate* current = v8::Isolate::GetCurrent();
    v8::HandleScope scope(current);
    v8::Local<v8::Context> context = current->GetCurrentContext();

    ElementCriterionJs* obj = node::ObjectWrap::Unwrap<ElementCriterionJs>(v->ToObject(context).ToLocalChecked());
    ElementCriterionConsumer* c = dynamic_cast<ElementCriterionConsumer*>(consumer);

    if (c == nullptr)
    {
      throw IllegalArgumentException(
        "Object does not accept ElementCriterion as an argument: " +
        str(v->ToObject(context).ToLocalChecked()->Get(context, baseClass()).ToLocalChecked()));
    }
    else
    {
      c->addCriterion(obj->getCriterion());
    }
  }

  template <typename T>
  static void populateElementConsumer(T* consumer, const v8::Local<v8::Value>& v)
  {
    LOG_TRACE("Populating element consumer...");

    v8::Isolate* current = v8::Isolate::GetCurrent();
    v8::HandleScope scope(current);
    v8::Local<v8::Context> context = current->GetCurrentContext();

    ElementJs* obj = node::ObjectWrap::Unwrap<ElementJs>(v->ToObject(context).ToLocalChecked());
    ElementConsumer* c = dynamic_cast<ElementConsumer*>(consumer);

    if (c == nullptr)
    {
      throw IllegalArgumentException(
        "Object does not accept Element as an argument: " + str(v->ToObject(context).ToLocalChecked()->Get(context, baseClass()).ToLocalChecked()));
    }
    else
    {
      c->addElement(obj->getElement());
    }
  }

  template <typename T>
  static void populateOsmMapConsumer(T* consumer, const v8::Local<v8::Value>& v)
  {
    LOG_TRACE("Populating osm map consumer...");

    v8::Isolate* current = v8::Isolate::GetCurrent();
    v8::HandleScope scope(current);
    v8::Local<v8::Context> context = current->GetCurrentContext();

    OsmMapJs* obj = node::ObjectWrap::Unwrap<OsmMapJs>(v->ToObject(context).ToLocalChecked());

    if (obj->isConst())
    {
      ConstOsmMapConsumer* c = dynamic_cast<ConstOsmMapConsumer*>(consumer);

      if (c == nullptr)
      {
        throw IllegalArgumentException(
          "Object does not accept const OsmMap as an argument. Maybe try a non-const OsmMap?: " +
          str(v->ToObject(context).ToLocalChecked()->Get(context, baseClass()).ToLocalChecked()));
      }
      else
      {
        c->setOsmMap(obj->getConstMap().get());
      }
    }
    else
    {
      OsmMapConsumer* c = dynamic_cast<OsmMapConsumer*>(consumer);

      if (c == nullptr)
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
  static void populateStringDistanceConsumer(T* consumer, const v8::Local<v8::Value>& v)
  {
    LOG_TRACE("Populating string distance consumer...");

    v8::Isolate* current = v8::Isolate::GetCurrent();
    v8::HandleScope scope(current);
    v8::Local<v8::Context> context = current->GetCurrentContext();

    StringDistancePtr sd = toCpp<StringDistancePtr>(v);

    StringDistanceConsumer* c = dynamic_cast<StringDistanceConsumer*>(consumer);

    if (c == nullptr)
    {
      throw IllegalArgumentException(
        "Object does not accept StringDistance as an argument: " +
        str(v->ToObject(context).ToLocalChecked()->Get(context, baseClass()).ToLocalChecked()));
    }
    else
    {
      c->setStringDistance(sd);
    }
  }

  template <typename T>
  static void populateValueAggregatorConsumer(T* consumer, const v8::Local<v8::Value>& v)
  {
    LOG_TRACE("Populating aggregator consumer...");

    v8::Isolate* current = v8::Isolate::GetCurrent();
    v8::HandleScope scope(current);
    v8::Local<v8::Context> context = current->GetCurrentContext();

    ValueAggregatorPtr va = toCpp<ValueAggregatorPtr>(v);

    ValueAggregatorConsumer* c = dynamic_cast<ValueAggregatorConsumer*>(consumer);

    if (c == nullptr)
    {
      throw IllegalArgumentException(
        "Object does not accept ValueAggregator as an argument: " +
        str(v->ToObject(context).ToLocalChecked()->Get(context, baseClass()).ToLocalChecked()));
    }
    else
    {
      c->setValueAggregator(va);
    }
  }

  template <typename T>
  static void populateVisitorConsumer(T* consumer, const v8::Local<v8::Value>& v)
  {
    LOG_TRACE("Populating visitor consumer...");

    v8::Isolate* current = v8::Isolate::GetCurrent();
    v8::HandleScope scope(current);
    v8::Local<v8::Context> context = current->GetCurrentContext();

    ElementVisitorJs* obj = node::ObjectWrap::Unwrap<ElementVisitorJs>(v->ToObject(context).ToLocalChecked());

    ElementVisitorConsumer* c = dynamic_cast<ElementVisitorConsumer*>(consumer);

    if (c == nullptr)
    {
      throw IllegalArgumentException(
        "Object does not accept ElementCriterion as an argument: " +
        str(v->ToObject(context).ToLocalChecked()->Get(context, baseClass()).ToLocalChecked()));
    }
    else
    {
      c->addVisitor(obj->getVisitor());
    }
  }

  template <typename T>
  static void populateFunctionConsumer(T* consumer, const v8::Local<v8::Value>& v)
  {
    if (v.IsEmpty() || v->IsFunction() == false)
    {
      throw IllegalArgumentException("Expected the argument to be a valid function.");
    }

    LOG_TRACE("Populating function consumer...");

    v8::Isolate* current = v8::Isolate::GetCurrent();
    v8::Local<v8::Function> func(v8::Local<v8::Function>::Cast(v));
    JsFunctionConsumer* c = dynamic_cast<JsFunctionConsumer*>(consumer);
    ElementCriterionConsumer* ecc = dynamic_cast<ElementCriterionConsumer*>(consumer);

    if (c != nullptr && ecc != nullptr)
    {
      // At the time of this writing this isn't possible. Give a good hard think about how the code
      // should respond before you change it.
      throw IllegalArgumentException(
        "Ambiguous consumption of both a function and an ElementCriterionConsumer.");
    }
    else if (c != nullptr)
    {
      c->addFunction(current, func);
    }
    else if (ecc != nullptr)
    {
      std::shared_ptr<JsFunctionCriterion> ecp(new JsFunctionCriterion());
      ecp->addFunction(current, func);
      ecc->addCriterion(ecp);
    }
    else
    {
      throw IllegalArgumentException("Object does not accept a function as an argument.");
    }
  }
};

}

#endif // POPULATECONSUMERSJS_H

