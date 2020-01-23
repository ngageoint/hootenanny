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
#ifndef SCRIPTMATCHCREATOR_H
#define SCRIPTMATCHCREATOR_H

#include <hoot/core/conflate/SearchRadiusProvider.h>
#include <hoot/core/conflate/matching/MatchCreator.h>
#include <hoot/core/util/NotImplementedException.h>

#include <hoot/js/PluginContext.h>

namespace hoot
{

class ScriptMatchVisitor;

/**
 * Match creator for all generic conflation scripts
 *
 * @sa ScriptMatch
 */
class ScriptMatchCreator : public MatchCreator, public SearchRadiusProvider
{
public:

  static std::string className() { return "hoot::ScriptMatchCreator"; }

  static const QString POINT_POLYGON_SCRIPT_NAME;

  ScriptMatchCreator();

  virtual ~ScriptMatchCreator();

  /**
   * @see SearchRadiusProvider
   */
  virtual Meters calculateSearchRadius(const ConstOsmMapPtr& map, const ConstElementPtr& e) override;

  /**
   * Not implemented.
   */
  virtual MatchPtr createMatch(const ConstOsmMapPtr&, ElementId, ElementId) override;

  /**
   * Search the provided map for POI matches and add the matches to the matches vector.
   */
  virtual void createMatches(const ConstOsmMapPtr& map, std::vector<ConstMatchPtr>& matches,
    ConstMatchThresholdPtr threshold) override;

  virtual std::vector<CreatorDescription> getAllCreators() const override;

  virtual void setArguments(QStringList args) override;

  /**
   * Determines whether an element is a candidate for matching for this match creator
   *
   * @param element element to determine the match candidate status of
   * @param map the map the element whose candidacy is being determined belongs to
   * @return true if the element is a match candidate; false otherwise
   */
  virtual bool isMatchCandidate(ConstElementPtr element, const ConstOsmMapPtr& map) override;

  virtual std::shared_ptr<MatchThreshold> getMatchThreshold() override;

private:

  std::shared_ptr<PluginContext> _script;
  QString _scriptPath;

  CreatorDescription _getScriptDescription(QString path) const;

  std::shared_ptr<ScriptMatchVisitor> _cachedScriptVisitor;
  std::shared_ptr<MatchThreshold> _matchThreshold;
  QMap<QString, Meters> _cachedCustomSearchRadii;

  std::shared_ptr<ScriptMatchVisitor> _getCachedVisitor(const ConstOsmMapPtr& map);
};

}

#endif // SCRIPTMATCHCREATOR_H
