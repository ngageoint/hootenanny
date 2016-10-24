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
#include <hoot/core/algorithms/string/StringTokenizer.h>

//  Qt
#include <QMap>

//  Standard
#include <cmath>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(StringDistance, KskipBigramDistance)

KskipBigramDistance::KskipBigramDistance(int k)
 : _k(2)
{
  if (k >= 0)
  {
    setK(k);
  }
}


void KskipBigramDistance::setK(int k)
{
  if (k < 0)
  {
    throw IllegalArgumentException("Expected k to be >= 0.");
  }
  _k = k;
}

QSet<QString> KskipBigramDistance::getBigrams(const QString& str) const
{
  QStringList corpus = StringTokenizer().tokenize(str.toLower());

  //  Use a QMap to de-duplicate and sort on the fly
  QMap<QString, int> bigrams;
  //  Iterate all of the tokens making bi-grams within each token, don't create bi-grams between tokens
  for (int tok = 0; tok < corpus.length(); tok++)
  {
    QString token = corpus[tok].toLower();
    if (token.length() < 3)
    {
      bigrams.insert(token, 1);
    }
    else
    {
      //  Iterate the token to create k-skip bi-grams from the letters
      for (int i = 0; i < token.length() - 1; i++)
      {
        for (int j = i + 1; j < token.length() && j <= i + _k + 1; j++)
          bigrams.insert(QString("%1%2").arg(token[i]).arg(token[j]), 1);
      }
    }
  }

  return QSet<QString>::fromList(bigrams.keys());
}

double KskipBigramDistance::score(const QString& s1, const QString& s2) const
{
  //  Get both sets of bi-grams
  QSet<QString> grams1 = getBigrams(s1);
  QSet<QString> grams2 = getBigrams(s2);
  //  Validate bi-gram set size
  if (grams1.count() == 0 && grams2.count() == 0)
    return 1.0;
  else if (grams1.count() == 0 || grams2.count() == 0)
    return 0.0;
  /**  To calculate the score use the following
   *   intersection of sets 1 and 2
   *   ----------------------------
   *   union of sets 1 and 2
   *
   *   http://www.sis.uta.fi/infim/julkaisut/fire/Spire-llncs.pdf
   *
   *   Note:  QSet uses operator& for intersection and operator+ for union
   */
  return ((double)(grams1 & grams2).count() / (double)(grams1 + grams2).count());
}

}
