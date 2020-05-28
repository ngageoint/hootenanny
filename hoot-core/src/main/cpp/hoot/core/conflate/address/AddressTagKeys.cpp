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
 * @copyright Copyright (C) 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "AddressTagKeys.h"

// hoot
#include <hoot/core/util/Log.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/FileUtils.h>

namespace hoot
{

AddressTagKeys::AddressTagKeys()
{
  ConfigOptions config = ConfigOptions(conf());
  _readAddressTagKeys(config.getAddressTagKeysFile());
  _additionalTagKeys = config.getAddressAdditionalTagKeys().toSet();
  LOG_VART(_additionalTagKeys);
}

AddressTagKeys& AddressTagKeys::getInstance()
{
  //  Local static singleton instance
  static AddressTagKeys instance;
  return instance;
}

void AddressTagKeys::_readAddressTagKeys(const QString& configFile)
{
  const QStringList addressTagKeyEntries = FileUtils::readFileToList(configFile);
  for (int i = 0; i < addressTagKeyEntries.size(); i++)
  {
    const QString addressTagKeyEntry = addressTagKeyEntries.at(i);
    const QStringList addressTagKeyEntryParts = addressTagKeyEntry.split("=");
    if (addressTagKeyEntryParts.size() != 2)
    {
      throw HootException("Invalid address tag key entry: " + addressTagKeyEntry);
    }
    const QString addressType = addressTagKeyEntryParts[0].trimmed().toLower();
    if (!addressType.isEmpty())
    {
      const QStringList addressTags = addressTagKeyEntryParts[1].split(",");
      for (int j = 0; j < addressTags.size(); j++)
      {
        const QString addressTag = addressTags.at(j).trimmed().toLower();
        if (!addressTag.isEmpty())
        {
          _addressTypeToTagKeys.insert(addressType, addressTag);
        }
      }
    }
  }
  LOG_VART(_addressTypeToTagKeys.size());
}

QSet<QString> AddressTagKeys::getAddressTagKeys(const Element& element) const
{
  QSet<QString> foundAddressTagKeys;
  const QList<QString> addressTagTypeKeys = _addressTypeToTagKeys.keys();
  for (int i = 0; i < addressTagTypeKeys.size(); i++)
  {
    const QString addressTypeTagKey = addressTagTypeKeys.at(i);
    LOG_VART(addressTypeTagKey);
    const QString addressTagKey = getAddressTagKey(element.getTags(), addressTypeTagKey);
    if (!addressTagKey.isEmpty())
    {
      LOG_TRACE("Found: " << addressTagKey);
      foundAddressTagKeys.insert(addressTagKey);
    }
  }
  for (QSet<QString>::const_iterator additionalTagKeyItr = _additionalTagKeys.begin();
       additionalTagKeyItr != _additionalTagKeys.end(); ++additionalTagKeyItr)
  {
    const QString addressTypeTagKey = *additionalTagKeyItr;
    LOG_VART(addressTypeTagKey);
    const QString addressTagKey = getAddressTagKey(element.getTags(), addressTypeTagKey);
    if (!addressTagKey.isEmpty())
    {
      LOG_TRACE("Found: " << addressTagKey);
      foundAddressTagKeys.insert(addressTagKey);
    }
  }
  return foundAddressTagKeys;
}

QString AddressTagKeys::getAddressTagKey(const Tags& tags, const QString& addressTagType) const
{
  return _getAddressTag(tags, addressTagType, true);
}

QString AddressTagKeys::getAddressTagValue(const Tags& tags, const QString& addressTagType) const
{
  return _getAddressTag(tags, addressTagType, false);
}

QString AddressTagKeys::_getAddressTag(const Tags& tags, const QString& addressTagType, bool key) const
{
  const QStringList tagKeys = _addressTypeToTagKeys.values(addressTagType);
  for (int i = 0; i < tagKeys.size(); i++)
  {
    const QString tagKey = tagKeys.at(i);
    if (tags.contains(tagKey))
    {
      if (key)
        return tagKey;
      else
        return tags.get(tagKey);
    }
  }
  return "";
}

}
