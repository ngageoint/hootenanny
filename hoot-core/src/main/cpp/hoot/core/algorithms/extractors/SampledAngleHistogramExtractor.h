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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef SAMPLED_ANGLE_HISTOGRAM_EXTRACTOR_H
#define SAMPLED_ANGLE_HISTOGRAM_EXTRACTOR_H

// hoot
#include <hoot/core/algorithms/extractors/AngleHistogramExtractor.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{

class Histogram;

/**
 * Similar to the AngleHistogramExtractor with the following differences:
 *
 * - The angle is sampled at a configurable distance along the way
 * - The distance from the sampled location on the way to look to calculate the heading is
 * configurable
 * - 8 bins are used in the histogram.
 */
class SampledAngleHistogramExtractor : public AngleHistogramExtractor
{
public:

  SampledAngleHistogramExtractor();
  virtual ~SampledAngleHistogramExtractor() = default;

  static std::string className() { return "hoot::SampledAngleHistogramExtractor"; }

  virtual std::string getClassName() const override
  { return SampledAngleHistogramExtractor::className(); }

  virtual void setConfiguration(const Settings& conf) override;

  void setSampleDistance(double sampleDistance) { _sampleDistance = sampleDistance; }
  void setHeadingDelta(double headingDelta) { _headingDelta = headingDelta; }

  virtual QString getDescription() const override
  {
    return
      "Calculates the angle of each line segment in a sampled fashion and adds it to a histogram";
  }

protected:

  virtual Histogram* _createHistogram(const OsmMap& map, const ConstElementPtr& e) const;

private:

  double _sampleDistance;
  double _headingDelta;
};

}

#endif // SAMPLED_ANGLE_HISTOGRAM_EXTRACTOR_H
