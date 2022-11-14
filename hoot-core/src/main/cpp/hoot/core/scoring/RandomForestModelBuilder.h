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
 * @copyright Copyright (C) 2018, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#ifndef RANDOMFORESTMODELBUILDER_H
#define RANDOMFORESTMODELBUILDER_H

namespace hoot
{

/**
 * Random Forest model builder
 */
class RandomForestModelBuilder
{
public:

  /**
   * Builds a Random Forest model file (.rf) from training data
   *
   * @param trainingData pairs of paths to training data files (containing REF tags); the first
   * file in the pair must contain only REF1 tags and the second in the pair must contain only
   * REF2 tags
   * @param output model output path
   * @param exportArffOnly if true; will build an .arff file only (.arff is a pre-req for
   * generating the .rf file)
   */
  static void build(const QStringList& trainingData, QString& output, const bool exportArffOnly = false);
};

}

#endif // RANDOMFORESTMODELBUILDER_H
