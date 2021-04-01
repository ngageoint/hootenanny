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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef ZCURVERANGER_H
#define ZCURVERANGER_H

//Hoot Includes
#include <hoot/core/algorithms/zindex/LongBox.h>
#include <hoot/core/algorithms/zindex/ZValue.h>

namespace hoot
{

class Range;
class BBox;

/**
 * Decomposes a bounding box into a range of zcurve values.
 *
 * Yes, ZCurve Ranger. Picture a guy wearing a space helmet in blue spandex
 * covered in Zs.
 */
class ZCurveRanger
{
public:

  static QString className() { return "hoot::ZCurveRanger"; }

  ZCurveRanger(const ZValue& zv);

  ~ZCurveRanger() = default;

  /** Find a good break point for the given box based on major z-value breaks
   * and break the box into two children.
   */
  std::vector<std::shared_ptr<LongBox>> breakBox(const std::shared_ptr<LongBox>& box);

  long int calculateExcess(const std::shared_ptr<LongBox>& box);

  /**
   * Recursively decompose a box over level iterations.
   */
  std::vector<std::shared_ptr<LongBox>> decomposeBox(const std::shared_ptr<LongBox>& box, int level);

  static int getMaxBitColumn(long int v);

  static long int getSplitValue(long int v1, long int v2);

  /**
   * Decomposes the z value ranges that cover box into an array of ranges.
   *
   * @param box - Must be a valid box within the ZValue.
   */
  std::vector<Range> decomposeRange(const BBox& box, int levels);

  /**
   * The number of cells that can be acceptably merged into one range. This
   * should never be less than one. Other than that it is a tunable parameter.
   * Denser data sets probably want a smaller value. Less dense data sets
   * should have a higher value.
   *
   * @param slop
   */
  void setSlop(int slop) { _slop = slop; }

  std::vector<Range> decomposeRange(const LongBox& box, int levels);

  bool rangeCoversIdentity(const Range& r);

private:
  /**
   * Condense ranges that overlap or are within "slop" cells of each other.
   */
  std::vector<Range> _condenseRanges(std::vector<Range>& r);

  Range _toRange(const std::shared_ptr<LongBox>& box);

  LongBox _toLongBox(const BBox& box);

  LongBox _clipBox(const LongBox& box);

  long int _slop;
  ZValue _zv;
};

}

#endif // ZCURVERANGER_H





