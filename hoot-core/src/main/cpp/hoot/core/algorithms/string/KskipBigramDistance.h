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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef KSKIPBIGRAMDISTANCE_H
#define KSKIPBIGRAMDISTANCE_H

//  Hoot
#include <hoot/core/algorithms/string/StringDistance.h>

//  Qt
#include <QSet>


//  Standard
#include <vector>

namespace hoot
{

/**
 * @brief The KskipBigramDistance class compares two strings using K-skip bigram distance.
 *
 * This is a work based on:
 *  A Closer Look at Skip-gram Modelling
 *  David Guthrie, Ben Allison, Wei Liu, Louise Guthrie, Yorick Wilks
 *  http://homepages.inf.ed.ac.uk/ballison/pdf/lrec_skipgrams.pdf
 *
 *  This algorithm isn't being used anywhere because tests on Geonames data showed
 *  that the Levenshtein distance outperformed or was equal to Skip-grams in 98.3% of the
 *  test cases.  The test were run as follows, Geonames data was used from four countries
 *  AF, AR, RU and US (http://download.geonames.org/export/dump/).  The test cases used only
 *  geonames POIs that contained alternate names to compare the original name against.  The
 *  first tests were run solely Skip-grams vs. Levenshtein and Skip-grams fared better on
 *  its own but when combined with other distance classes (ToEnglishTranslateStringDistance and
 *  MeanWordSetDistance) those results dropped off tremendously.  Since the Random Forest
 *  extractors apply both of those distances to Levenshtein, the test had to include them
 *  thus degrading and already degraded results.  So the ToEnglishTranslateStringDistance and
 *  MeanWordSetDistance classes were applied to both algorithms, counted, averaged, and
 *  reported.  Below is a sample of the data both before the two distance algorithms were
 *  applied and after:
 *
 *  RESULTS:
 *                                         | Algorithms     | Algorithms          |
 *                                         | Compared Solo  | Including Translate |
 *                                         |                | and MeanWordSet     |
 *                                         |                |                     |
 *   Total POIs:                           |    240483      |    240483           |
 *   POIs where Levenshtein == Skip-grams  |     11928      |     42005           |
 *   POIs where Levenshtein > Skip-grams   |    161401      |    194201           |
 *   POIs where Levenshtein < Skip-grams   |     67154      |      4277**         |
 *
 *    ** - Only 1.77% of the time did Skip-grams give a better result on training data
 *         than did Levenshtein
 */
class KskipBigramDistance : public StringDistance
{
public:

  static QString className() { return "hoot::KskipBigramDistance"; }

  KskipBigramDistance(int k = 2);
  ~KskipBigramDistance() = default;

  void setK(int k);

  /**
   * @see StringDistance
   */
  double compare(const QString& s1, const QString& s2) const override { return score(s1, s2); }

  QSet<QString> getBigrams(const QString& str) const;

  double score(const QString& s1, const QString& s2) const;

  QString toString() const override { return QString("%1-skip bi-gram").arg(_k); }
  QString getDescription() const override
  { return "Returns a string comparison score derived using Skip-gram modeling"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }

private:
  /**
   * @brief _k is a value 'k' >= 0 for k-skip bi-grams
   */
  int _k;
};

}
#endif // KSKIPBIGRAMDISTANCE_H
