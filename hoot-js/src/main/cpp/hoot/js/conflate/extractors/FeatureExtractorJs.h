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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef FEATUREEXTRACTOR_JS_H
#define FEATUREEXTRACTOR_JS_H

// hoot
#include <hoot/core/algorithms/extractors/FeatureExtractor.h>

// node.js
#include <hoot/js/io/DataConvertJs.h>
#include <hoot/js/SystemNodeJs.h>

// Qt
#include <QString>

// Standard
#include <memory>

namespace hoot
{

class OsmMapOperation;

class FeatureExtractorJs : public node::ObjectWrap
{
public:

  static void Init(v8::Handle<v8::Object> target);

  FeatureExtractorPtr getFeatureExtractor() { return _fe; }

private:

  FeatureExtractorJs(FeatureExtractorPtr fe);
  ~FeatureExtractorJs();

  static void extract(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void getName(const v8::FunctionCallbackInfo<v8::Value>& args);

  QString _className;
  FeatureExtractorPtr _fe;
};

inline void toCpp(v8::Handle<v8::Value> v, FeatureExtractorPtr& p)
{
  if (!v->IsObject())
  {
    throw IllegalArgumentException("Expected an object, got: (" + toJson(v) + ")");
  }

  v8::Handle<v8::Object> obj = v8::Handle<v8::Object>::Cast(v);
  FeatureExtractorJs* fej = 0;
  fej = node::ObjectWrap::Unwrap<FeatureExtractorJs>(obj);
  if (fej)
  {
    p = fej->getFeatureExtractor();
  }
  else
  {
    throw IllegalArgumentException("Expected a FeatureExtractorJs, got: (" + toJson(v) + ")");
  }
}


}

#endif // FEATUREEXTRACTOR_JS_H
