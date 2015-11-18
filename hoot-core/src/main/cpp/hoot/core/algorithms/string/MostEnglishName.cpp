#include "MostEnglishName.h"

// Hoot
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>

// Qt
#include <QFile>

// Standard
#include <limits>

namespace hoot
{

MostEnglishNamePtr MostEnglishName::_theInstance;

MostEnglishName::MostEnglishName()
{
  _initialized = false;
  setConfiguration(conf());
}

const MostEnglishNamePtr& MostEnglishName::getInstance()
{
  if (_theInstance.get() == 0)
  {
    _theInstance.reset(new MostEnglishName());
  }

  return _theInstance;
}


QString MostEnglishName::getMostEnglishName(const Tags& tags)
{
  if (tags.contains("name:en") && tags.get("name:en").isEmpty() == false)
  {
    return tags.get("name:en");
  }

  QStringList names = tags.getNames();

  double bestScore = -numeric_limits<double>::max();
  QString bestName;

  for (int i = 0; i < names.size(); i++)
  {
    double score = scoreName(names[i]);

    if (score > bestScore)
    {
      bestScore = score;
      bestName = names[i];
    }
  }

  return bestName;
}

const QSet<QString>& MostEnglishName::_getWords()
{
  if (_initialized == false)
  {
    LOG_VAR(_wordPaths);
    for (int i = 0; i < _wordPaths.size(); i++)
    {
      if (_loadEnglishWords(_wordPaths[i]) != 0)
      {
        break;
      }
    }

    LOG_INFO("Unique (case-insensitive) words: " + QString::number(_englishWords.size()));
    _initialized = true;
  }

  return _englishWords;
}

long MostEnglishName::_loadEnglishWords(QString path)
{
  QFile fp(path);
  const int MAX_LINE_SIZE = 2048;
  int count = 0;
  long wordCount = 0;

  if (fp.open(QFile::ReadOnly))
  {
    LOG_DEBUG("Loading English word file: " << path);
    while (!fp.atEnd())
    {
      QByteArray ba = fp.readLine(MAX_LINE_SIZE);
      if (ba.size() == MAX_LINE_SIZE && count < 10)
      {
        LOG_WARN("Loaded a line of max size. Is this a proper dictionary?");
        count++;
        if (count == 10)
        {
          LOG_WARN("Silencing dictionary record warnings.");
        }
      }
      QString s = QString::fromUtf8(ba.data());
      wordCount++;

      // all words are converted to lower case. This is more liberal when matching words.
      _englishWords.insert(s.trimmed().toLower());
    }
  }

  return wordCount;
}

double MostEnglishName::scoreName(QString n)
{
  QStringList words = _tokenizer.tokenize(n);

  double score = 0;
  int characters = 0;

  const QSet<QString>& englishWords = _getWords();
  for (int i = 0; i < words.size(); i++)
  {
    if (englishWords.contains(words[i].toLower()))
    {
      LOG_INFO("In dictionary: " << words[i]);
      score += words[i].size();
      characters += words[i].size();
    }
    else
    {
      QString s = words[i];
      characters += s.size();
      LOG_VAR(s);
      for (int j = 0; j < s.size(); j++)
      {
        if (s[j].toLatin1() != 0)
        {
          // letters increase the score, numbers have no effect.
          if (s[j].isLetter())
          {
            score += 0.5;
          }
        }
      }
    }
  }

  return score / characters;
}

void MostEnglishName::setConfiguration(const Settings& conf)
{
  if (this == _theInstance.get())
  {
    throw HootException("Please do not set the configuration on the singleton instance.");
  }

  _wordPaths = ConfigOptions(conf).getEnglishWordsFiles();
  _tokenizer.setConfiguration(conf);
  _englishWords.clear();
  _initialized = false;
}

}
