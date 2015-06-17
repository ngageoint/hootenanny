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
 * @copyright Copyright (C) 2013, 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef BUILDINGMERGEMANIPULATION_H
#define BUILDINGMERGEMANIPULATION_H

// hoot
#include <hoot/core/conflate/extractors/FeatureExtractor.h>
#include <hoot/core/elements/ElementId.h>
#include <hoot/core/manipulators/Manipulation.h>
#include <hoot/core/manipulators/ManipulationDetails.h>

namespace Tgs
{
  class RandomForest;
}

namespace hoot
{

/**
 * Performs a one to one or one to many match and merge of buildings.
 *
 * This explicitly does not handle many to many matches.
 */
class BuildingMergeManipulation : public Manipulation, public ManipulationDetails
{
public:
  BuildingMergeManipulation(const RandomForest* rf, const ElementId& eid1, const ElementId& eid2);

  virtual void applyManipulation(shared_ptr<OsmMap> wm,
                                 set<ElementId>& impactedWays, set<ElementId>& newWays) const;

  virtual double calculateProbability(shared_ptr<const OsmMap> map) const;

  virtual double calculateScore(shared_ptr<const OsmMap> map) const;

  /**
   * See Method::getClassification for a description.
   */
  void getClassification(const ConstOsmMapPtr& map, vector<double>& v);

  ElementId getEid1() const { return _eid1; }
  ElementId getEid2() const { return _eid1; }

  virtual map<QString, double> getFeatures(const shared_ptr<const OsmMap>& m) const;

  virtual const set<ElementId>& getImpactedElementIds(const ConstOsmMapPtr &map) const;

  virtual set<ElementId> getMatchedElements() const;

  virtual double getProbabilityEstimate() const { return _p; }

  virtual double getScoreEstimate() const { return getProbabilityEstimate(); }

  virtual bool isValid(shared_ptr<const OsmMap> map) const;

  virtual QString toString() const;

private:
  ElementId _eid1;
  ElementId _eid2;
  mutable double _p;
  const RandomForest* _rf;
  mutable set<ElementId> _impactedElements;
  static vector< shared_ptr<const FeatureExtractor> > _extractors;

  /**
   * Creates all the reasonable extractors for a data set. This can be quite large and is good
   * for experimentation.
   */
  static void _createAllExtractors();

  /**
   * Create the best extractors found during experimentation.
   */
  static void _createBestExtractors();

  shared_ptr<Relation> _createEid2Relation(const ConstOsmMapPtr& map) const;

  const vector< shared_ptr<const FeatureExtractor> >& _getExtractors() const;

};

}

#endif // BUILDINGMERGEMANIPULATION_H
