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
 * @copyright Copyright (C) 2015, 2016, 2017, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "TranslatedTagDifferencer.h"

// hoot
#include <hoot/core/algorithms/optimizer/SingleAssignmentProblemSolver.h>
#include <hoot/core/elements/Element.h>
#include <hoot/core/elements/Tags.h>
#include <hoot/core/geometry/ElementToGeometryConverter.h>
#include <hoot/core/io/schema/Feature.h>
#include <hoot/core/schema/ScriptSchemaTranslator.h>
#include <hoot/core/schema/ScriptSchemaTranslatorFactory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(TagDifferencer, TranslatedTagDifferencer)

TranslatedTagDifferencer::TranslatedTagDifferencer()
{
  setConfiguration(conf());
}

TranslatedTagDifferencer::Comparison TranslatedTagDifferencer::_compare(const Tags& t1,
  const Tags& t2) const
{
  Comparison result;
  result.different = 0;
  result.same = 0;
  QSet<QString> touched;

  for (Tags::const_iterator it = t1.begin(); it != t1.end(); ++it)
  {
    touched.insert(it.key());
    if (_ignoreList.contains(it.key()) == false)
    {
      if (it.value() == t2.get(it.key()))
      {
        result.same++;
      }
      else
      {
        result.different++;
      }
    }
  }

  for (Tags::const_iterator it = t2.begin(); it != t2.end(); ++it)
  {
    if (_ignoreList.contains(it.key()) == false && touched.contains(it.key()) == false)
    {
      if (it.value() == t1.get(it.key()))
      {
        result.same++;
      }
      else
      {
        result.different++;
      }
    }
  }

  return result;
}

double TranslatedTagDifferencer::diff(const ConstOsmMapPtr& map, const ConstElementPtr& e1,
  const ConstElementPtr& e2) const
{
  // translate the tags for comparison
  vector<ScriptToOgrSchemaTranslator::TranslatedFeature> tf1 = _translate(map, e1);
  vector<ScriptToOgrSchemaTranslator::TranslatedFeature> tf2 = _translate(map, e2);

  class CostFunction :
      public SingleAssignmentProblemSolver<
        ScriptToOgrSchemaTranslator::TranslatedFeature,
        ScriptToOgrSchemaTranslator::TranslatedFeature>::CostFunction
  {
  public:
    CostFunction() = default;
    virtual ~CostFunction() = default;

    const TranslatedTagDifferencer* ttd;
    /**
     * Returns the cost associated with assigning actor a to task t.
     */
    double cost(const ScriptToOgrSchemaTranslator::TranslatedFeature* tf1,
                const ScriptToOgrSchemaTranslator::TranslatedFeature* tf2) const override
    {
      Tags t1 = _toTags(tf1);
      Tags t2 = _toTags(tf2);
      return ttd->_compare(t1, t2).same;
    }
  };

  CostFunction cost;
  cost.ttd = this;
  using Saps = SingleAssignmentProblemSolver<ScriptToOgrSchemaTranslator::TranslatedFeature,
      ScriptToOgrSchemaTranslator::TranslatedFeature> ;
  Saps sap(cost);

  for (size_t i = 0; i < tf1.size(); i++)
  {
    sap.addActor(&(tf1[i]));
  }
  for (size_t i = 0; i < tf2.size(); i++)
  {
    sap.addTask(&(tf2[i]));
  }

  vector<Saps::ResultPair> pairs = sap.calculatePairing();

  Comparison c;
  for (size_t i = 0; i < pairs.size(); i++)
  {
    Tags t1 = _toTags(pairs[i].actor);
    Tags t2 = _toTags(pairs[i].task);
    Comparison c1 = _compare(t1, t2);
    c.same += c1.same;
    c.different += c1.different;
  }

  return 1.0 - ((double)c.same / (double)(c.same + c.different));
}

std::shared_ptr<ScriptToOgrSchemaTranslator> TranslatedTagDifferencer::_getTranslator() const
{
  if (_translator == nullptr)
  {
    std::shared_ptr<ScriptSchemaTranslator> st =
      ScriptSchemaTranslatorFactory::getInstance().createTranslator(_script);

    st->setErrorTreatment(StrictOff);
    _translator = std::dynamic_pointer_cast<ScriptToOgrSchemaTranslator>(st);
    if (!_translator)
    {
      throw HootException("Error allocating translator, the translation script must support "
                          "converting to OGR.");
    }
    _translator->getOgrOutputSchema();
  }

  return _translator;
}

void TranslatedTagDifferencer::setConfiguration(const Settings& conf)
{
  ConfigOptions configOptions(conf);
  QStringList il = configOptions.getTranslatedTagDifferencerIgnoreList().split(";");
  _ignoreList = QSet<QString>::fromList(il);
  _script = configOptions.getTranslatedTagDifferencerScript();
  _translator.reset();
}

Tags TranslatedTagDifferencer::_toTags(const ScriptToOgrSchemaTranslator::TranslatedFeature* tf)
{
  Tags result;

  if (tf)
  {
    std::shared_ptr<Feature> f = tf->feature;
    QString layer = tf->tableName;

    const QVariantMap& vm = f->getValues();
    for (QVariantMap::const_iterator it = vm.begin(); it != vm.end(); ++it)
    {
      result[it.key()] = it.value().toString();
    }
    result["__LAYER__"] = layer;
  }

  return result;
}

vector<ScriptToOgrSchemaTranslator::TranslatedFeature> TranslatedTagDifferencer::_translate(
  const ConstOsmMapPtr& map, const ConstElementPtr& e) const
{
  std::shared_ptr<Geometry> g = ElementToGeometryConverter(map).convertToGeometry(e);
  Tags t = e->getTags();

  return _getTranslator()->translateToOgr(t, e->getElementType(), g->getGeometryTypeId());
}

}
