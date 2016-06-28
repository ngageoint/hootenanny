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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "KskipBigramDistance.h"
//  Hoot
#include <hoot/core/util/Log.h>
#include <hoot/core/Factory.h>

//  Standard
#include <cmath>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(StringDistance, KskipBigramDistance)

KskipBigramDistance::KskipBigramDistance(unsigned k)
 : _k(2)
{
  if (k >= 1)
  {
    setK(k);
  }
}


void KskipBigramDistance::setK(unsigned k)
{
  if (k < 1)
  {
    throw IllegalArgumentException("Expected k to be >= 1.");
  }
  _k = k;
}

BigramType KskipBigramDistance::getBigrams(const QString& str) const
{
  QStringList corpus = str.toLower().split(" ", QString::KeepEmptyParts, Qt::CaseInsensitive);

  BigramType bigrams;

  if (corpus.length() < 3)
  {
    //  Two or less tokens can only result in 1 'bi-gram'
    bigrams.push_back(str.toLower());
  }
  else
  {
    //  Iterate the tokens to create k-skip bi-grams
    for (int i = 0; i < corpus.length() - 1; i++)
    {
      for (int j = i + 1; j < corpus.length() && j <= i + (int)_k + 1; j++)
      {
        bigrams.push_back(QString("%1 %2").arg(corpus[i]).arg(corpus[j]));
      }
    }
  }

  return bigrams;
}

double KskipBigramDistance::score(const QString& s1, const QString& s2) const
{
  //  Get both sets of bi-grams
  BigramType grams1 = getBigrams(s1);
  BigramType grams2 = getBigrams(s2);
  //  Iterate the smaller set (set1) and check if they exist in larger set (set2)
  BigramType* set1 = &grams1;
  BigramType* set2 = &grams2;
  if (grams1.length() < grams2.length())
  {
    set1 = &grams2;
    set2 = &grams1;
  }

  double s = 0.0;
  unsigned found = 0;
  //  Iterate set for s1
  for (BigramType::iterator it = set1->begin(); it != set1->end(); it++)
  {
    //  Check if the bi-gram is in set for s2
    if (set2->contains(*it, Qt::CaseInsensitive))
    {
      found++;
    }
  }
  //  Calculate the score using (f / s) ^ (1 / 8) where f is the number of matched bi-grams in set1 and s is the number of total bi-grams in set1
  if (found > 0)
    s = pow(found / (double)set1->length(), 0.125);

  return s;
}

}
