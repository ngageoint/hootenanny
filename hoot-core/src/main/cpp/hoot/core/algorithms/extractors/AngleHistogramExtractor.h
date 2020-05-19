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
 * @copyright Copyright (C) 2005 VividSolutions (http://www.vividsolutions.com/)
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef ANGLE_HISTOGRAM_EXTRACTOR_H
#define ANGLE_HISTOGRAM_EXTRACTOR_H

// hoot
#include <hoot/core/algorithms/extractors/FeatureExtractorBase.h>
#include <hoot/core/elements/Element.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{

class Histogram;

/**
 * This idea was taken from JCS, but the code has been reimplemented to better take advantage
 * of the OSM schema.
 *
 * The angle histogram extractor works by calculating the angle of each line segment in a line
 * string and adding that angle to a histogram where the weight is the length of the line segment.
 * A histogram is built up in this way for both input lines, normalized and the difference
 * calculated. 16 bins are used in the histogram.
 *
 * 1 means the histograms have effectively no difference. 0 means they're completely different.
 */
class AngleHistogramExtractor : public FeatureExtractorBase, public Configurable
{
public:

  AngleHistogramExtractor();
  AngleHistogramExtractor(Radians smoothing, unsigned int bins = 16);

  static std::string className() { return "hoot::AngleHistogramExtractor"; }

  virtual std::string getClassName() const override { return AngleHistogramExtractor::className(); }

  virtual std::string getName() const override;

  virtual void setConfiguration(const Settings& conf);

  virtual double extract(const OsmMap& map, const std::shared_ptr<const Element>& target,
    const std::shared_ptr<const Element>& candidate) const override;

  void setSmoothing(Radians sigma) { _smoothing = sigma; }

  virtual QString getDescription() const
  { return "Calculates the angle of each line segment and adds it to a histogram"; }

protected:

  virtual Histogram* _createHistogram(const OsmMap& map, const ConstElementPtr& e) const;

  Radians _smoothing;
  unsigned int _bins;
};

}

#endif // ANGLE_HISTOGRAM_EXTRACTOR_H
