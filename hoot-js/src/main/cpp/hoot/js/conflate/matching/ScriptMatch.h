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
#ifndef SCRIPTMATCH_H
#define SCRIPTMATCH_H

// hoot
#include <hoot/core/conflate/matching/Match.h>
#include <hoot/core/conflate/matching/MatchThreshold.h>
#include <hoot/core/conflate/matching/MatchDetails.h>
#include <hoot/core/conflate/matching/MatchClassification.h>
#include <hoot/js/PluginContext.h>

namespace hoot
{

class ScriptMatchTest;

/**
 * The script match and merge routines expose a way for matching and merging elements using
 * JavaScript. See the _JavaScript Overview_ in the _Supplemental User Documentation_ for details.
 */
class ScriptMatch : public Match, public MatchDetails
{
public:

  static QString className() { return "hoot::ScriptMatch"; }

  static int logWarnCount;

  ScriptMatch() = default;

  /**
   * mapObj could be derived from the map, but destructing an OsmMapJs object is quite expensive
   * due to the amount of memory cleanup we must do in the general case.
   */
  ScriptMatch(
    const std::shared_ptr<PluginContext>& script, const v8::Persistent<v8::Object>& plugin,
    const ConstOsmMapPtr& map, const v8::Local<v8::Object>& mapObj, const ElementId& eid1,
    const ElementId& eid2, const ConstMatchThresholdPtr& mt);
  ~ScriptMatch() = default;

  const MatchClassification& getClassification() const override { return _p; }

  MatchMembers getMatchMembers() const override { return _matchMembers; }
  void setMatchMembers(const MatchMembers& matchMembers) { _matchMembers = matchMembers; }

  QString explain() const override { return _explainText; }
  QString getName() const override { return _matchName; }

  double getProbability() const override;

  bool isConflicting(
    const ConstMatchPtr& other, const ConstOsmMapPtr& map,
    const QHash<QString, ConstMatchPtr>& matches = QHash<QString, ConstMatchPtr>()) const override;

  bool isWholeGroup() const override { return _isWholeGroup; }

  /**
   * Returns the two elements that were matched
   */
  std::set<std::pair<ElementId, ElementId>> getMatchPairs() const override;

  QString toString() const override;

  std::map<QString, double> getFeatures(const ConstOsmMapPtr& map) const override;

  /**
   * Given a geometry type from a conflate script, returns the corresponding MatchMembers value.
   *
   * @param geometryType a geometry type string
   * @return a MatchMebers enumeration value
   */
  static MatchMembers geometryTypeToMatchMembers(const QString& geometryType);

  QString getClassName() const override { return className(); }
  QString getDescription() const override
  { return "Matches elements using Generic Conflation via Javascript"; }

  v8::Local<v8::Object> getPlugin() const { return ToLocal(&_plugin); }

  std::shared_ptr<PluginContext> getScript() const { return _script; }

private:

  friend class ScriptMatchTest;

  bool _isWholeGroup;
  QString _matchName;
  bool _neverCausesConflict;
  MatchClassification _p;
  MatchMembers _matchMembers;

  v8::Persistent<v8::Object> _plugin;
  std::shared_ptr<PluginContext> _script;
  QString _explainText;

  using ConflictKey = std::pair<ElementId, ElementId>;
  mutable QHash<ConflictKey, bool> _conflicts;

  void _calculateClassification(
    const ConstOsmMapPtr& map, v8::Local<v8::Object> mapObj, v8::Local<v8::Object> plugin);

  v8::Local<v8::Value> _call(
    const ConstOsmMapPtr& map, v8::Local<v8::Object> mapObj, v8::Local<v8::Object> plugin);

  ConflictKey _getConflictKey() const { return ConflictKey(_eid1, _eid2); }

  bool _isOrderedConflicting(const ConstOsmMapPtr& map, ElementId sharedEid,
    ElementId other1, ElementId other2,
    const QHash<QString, ConstMatchPtr>& matches = QHash<QString, ConstMatchPtr>()) const;

  /*
   * Either creates a new match or retrieves an existing one from the global set of matches
   */
  std::shared_ptr<const ScriptMatch> _getMatch(
    OsmMapPtr map, v8::Local<v8::Object> mapJs, const ElementId& eid1, const ElementId& eid2,
    const QHash<QString, ConstMatchPtr>& matches) const;

  v8::Local<v8::Value> _callGetMatchFeatureDetails(const ConstOsmMapPtr& map) const;
};

}

#endif // JSMATCH_H
