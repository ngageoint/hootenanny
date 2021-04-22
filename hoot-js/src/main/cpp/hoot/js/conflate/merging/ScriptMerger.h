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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef SCRIPTMERGER_H
#define SCRIPTMERGER_H

// hoot
#include <hoot/core/conflate/merging/MergerBase.h>
#include <hoot/js/PluginContext.h>

namespace hoot
{

/**
 * @sa ScriptMatch
 */
class ScriptMerger : public MergerBase
{
public:

  static QString className() { return "hoot::ScriptMerger"; }

  static int logWarnCount;

  ScriptMerger() = default;
  ~ScriptMerger() = default;

  ScriptMerger(const std::shared_ptr<PluginContext>& script, v8::Persistent<v8::Object>& plugin,
    const std::set<std::pair<ElementId, ElementId>>& pairs);

  void apply(const OsmMapPtr& map,
             std::vector<std::pair<ElementId, ElementId>>& replaced) override;

  /**
   * Returns true if the plugin has a function with the specified name.
   */
  bool hasFunction(QString name) const;

  QString toString() const override { return QString("ScriptMerger"); }

  QString getDescription() const override
  { return "Merges elements matched with Generic Conflation"; }
  QString getName() const override { return className() + "-" + _matchType; }
  QString getClassName() const override { return className(); }

  void setMatchType(const QString& matchType) { _matchType = matchType; }

protected:

  v8::Persistent<v8::Object> _plugin;
  std::shared_ptr<PluginContext> _script;
  ElementId _eid1, _eid2;
  QString _matchType;

  /**
   * Calls mergePair in the JS.
   */
  virtual void _applyMergePair(
    const OsmMapPtr& map, std::vector<std::pair<ElementId, ElementId>>& replaced) const;
  /**
   * Calls mergeSet in the JS.
   */
  virtual void _applyMergeSets(
    const OsmMapPtr& map, std::vector<std::pair<ElementId, ElementId>>& replaced) const;

  v8::Handle<v8::Value> _callMergePair(const OsmMapPtr& map) const;
  void _callMergeSets(const OsmMapPtr& map,
                      std::vector<std::pair<ElementId, ElementId>>& replaced) const;
};

}

#endif // SCRIPTMERGER_H
