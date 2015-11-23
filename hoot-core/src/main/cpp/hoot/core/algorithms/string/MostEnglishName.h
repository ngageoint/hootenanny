#ifndef MOSTENGLISHNAME_H
#define MOSTENGLISHNAME_H

// Hoot
#include <hoot/core/algorithms/string/StringTokenizer.h>
#include <hoot/core/elements/Tags.h>
#include <hoot/core/util/Configurable.h>

// Qt
#include <QSet>

// Tgs
#include <tgs/SharedPtr.h>

namespace hoot
{

class MostEnglishName;

typedef shared_ptr<MostEnglishName> MostEnglishNamePtr;

/**
 * Return a best guess at the "most english" name in the list. There are no guarantees. This is
 * an ad-hoc routine that should be better than taking the first choice in most cases.
 *
 * If there are no names then an empty string is returned.
 *
 * Logic:
 *  - Tokenize the string into words.
 *  - If the word is in the known dictionary (case insensitive) increase the strings's score by 1
 *    per letter in the word.
 *  - If the word is not in the known dictionary:
 *    - Increase the score by 0.5 for every latin letter.
 *    - Don't modify the score for every latin non-letter (e.g. punctuation and numbers)
 *    - Decrease the score by 0.5 for every non-latin character.
 */
class MostEnglishName : public Configurable
{
public:
  static string className() { return "hoot::MostEnglishName"; }

  MostEnglishName();

  static const MostEnglishNamePtr& getInstance();

  QString getMostEnglishName(const Tags& tags);

  double scoreName(QString n);

  void setConfiguration(const Settings& conf);

private:
  static MostEnglishNamePtr _theInstance;

  QSet<QString> _englishWords;
  StringTokenizer _tokenizer;
  QStringList _wordPaths;
  bool _initialized;

  const QSet<QString>& _getWords();

  long _loadEnglishWords(QString path);

};

}

#endif // MOSTENGLISHNAME_H
