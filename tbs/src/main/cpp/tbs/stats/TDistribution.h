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
 * @copyright Copyright (C) 2015, 2017, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef __T_DISTRIBUTION_H__
#define __T_DISTRIBUTION_H__

// OpenCV
#define int64 opencv_broken_int
#include <opencv/cv.h>
#undef int64

#include <opencv/cxcore.h>

namespace tbs
{

class CostT;

/**
 * Implements a Student t-distribution as defined by [1].
 *
 * This has been tested with 1 and 2 dimensional data. It should work with higher dimensions but has
 * not been tested.
 *
 * 1. Computer Vision: Models, Learning and Inference, Simon J. D. Prince
 *    http://computervisionmodels.com/ (Free PDF)
 *    http://www.amazon.com/Computer-Vision-Models-Learning-Inference/dp/1107011795/ref=sr_1_1?s=books&ie=UTF8&qid=1375736006&sr=1-1&keywords=computer+vision+models+learning+and+inference
 * 2. Computer Vision: Algorithms Booklet, Simon J. D. Prince
 *    http://computervisionmodels.com/
 *    http://www0.cs.ucl.ac.uk/staff/s.prince/book/Algorithms.pdf
 */
class TDistribution
{
public:

  TDistribution() = default;
  virtual ~TDistribution() = default;

  /**
   * Similar to calling initialize.
   */
  TDistribution(const cv::Mat& m);

  /**
   * Returns the likelihood at a given point.
   */
  double getLikelihood(const cv::Mat &p) const;

  double getLogLikelihood(const cv::Mat &p) const;

  /**
   * Creates a TDistribution for the provided Matrix. Each row is a sample and each column is a
   * dimension.
   */
  void initialize(const cv::Mat& m);

private:
  int _D;
  cv::Mat _mu;
  cv::Mat _sigma;
  double _v;

  friend class CostT;

  double _calculateDataLogLikelihood(const cv::Mat& m, double v) const;

  void _calculateNewMuAndSigma(const std::vector<double>& EH, const cv::Mat& m);

  void _calculateNewV(const cv::Mat& m, const std::vector<double>& EH, const std::vector<double>& ELogH);

  double _calculateTCost(double v, const std::vector<double> &EH, const std::vector<double> &ELogH) const;

  void _initMu(const cv::Mat& m);

  void _initSigma(const cv::Mat& m);

  cv::Mat _log(const cv::Mat& m) const;
};

}

#endif
