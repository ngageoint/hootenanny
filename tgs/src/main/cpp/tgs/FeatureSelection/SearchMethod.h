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
 * @copyright Copyright (C) 2015, 2017 Maxar (http://www.maxar.com/)
 */

#ifndef __SEARCH_METHOD_H__
#define __SEARCH_METHOD_H__

// Standard Includes
#include <string>
#include <vector>

#include "../Progress.h"
#include "../SharedPtr.h"

namespace Tgs
{
  class DataFrame;
  class FactorSubsetEvaluator;

  /**
  * A SearchMethod searches the available factor space for a good subset of factors. The quality
  * of the subset found will depend on the evaluator and the search method.
  * See http://en.wikipedia.org/wiki/Feature_selection for a more detailed discussion.
  */
  class TGS_EXPORT SearchMethod
  {
  public:

    virtual ~SearchMethod() {}

    /**
     * Searches for a good subset of factors.
     */
    virtual std::vector<std::string> findSubset(const DataFrame& df, TgsProgress* p = NULL) = 0;

    /**
     * This evaluator will be used to evaluate the fitness of a factor subset.
     */
    virtual void setEvaluator(boost::shared_ptr<FactorSubsetEvaluator> fe) = 0;
  };
}

#endif
