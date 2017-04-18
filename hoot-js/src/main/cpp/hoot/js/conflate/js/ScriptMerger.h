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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef SCRIPTMERGER_H
#define SCRIPTMERGER_H

// hoot
#include <hoot/core/conflate/MergerBase.h>
#include <hoot/js/PluginContext.h>

namespace hoot
{

/**
 * @sa ScriptMatch
 */
class ScriptMerger : public MergerBase
{
public:
  ScriptMerger(boost::shared_ptr<PluginContext> script, Persistent<Object> plugin,
    const set<pair<ElementId, ElementId> > &pairs);

  virtual void apply(const OsmMapPtr& map, vector< pair<ElementId, ElementId> >& replaced) const;

  /**
   * Returns true if the plugin has a function with the specified name.
   */
  bool hasFunction(QString name) const;

  virtual QString toString() const { return QString("ScriptMerger"); }

protected:
  PairsSet _pairs;
  Persistent<Object> _plugin;
 boost::shared_ptr<PluginContext> _script;
  ElementId _eid1, _eid2;

  /**
   * Calls mergePair in the JS.
   */
  virtual void _applyMergePair(const OsmMapPtr& map, vector< pair<ElementId, ElementId> >& replaced)
    const;
  /**
   * Calls mergeSet in the JS.
   */
  virtual void _applyMergeSets(const OsmMapPtr& map, vector< pair<ElementId, ElementId> >& replaced)
    const;

  Handle<Value> _callMergePair(const OsmMapPtr& map) const;
  void _callMergeSets(const OsmMapPtr& map, vector<pair<ElementId, ElementId> > &replaced) const;
  virtual PairsSet& getPairs() { return _pairs; }
  virtual const PairsSet& getPairs() const { return _pairs; }

};

}

#endif // SCRIPTMERGER_H
