
#include "LocalAddressValidationDataSource.h"

#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <string>

namespace hoot
{

const char kDataFileName[] = "conf/core/poi-polygon/countryinfo.txt";

// For historical reasons, normal and aggregated data is here stored in the
// same data structure, differentiated by giving each key a prefix. It does
// seem like a good idea to refactor this.
const char kNormalPrefix = '-';
const char kAggregatePrefix = '+';

// Each data key begins with this string. Example of a data key:
//     data/CH/AG
const char kDataKeyPrefix[] = "data/";

// The number of characters in the data key prefix.
const size_t kDataKeyPrefixLength = sizeof kDataKeyPrefix - 1;

// The number of characters in a CLDR region code, e.g. 'CH'.
const size_t kCldrRegionCodeLength = 2;

// The number of characters in an aggregate data key, e.g. 'data/CH'.
const size_t kAggregateDataKeyLength =
    kDataKeyPrefixLength + kCldrRegionCodeLength;

std::map<std::string, std::string> InitData(const std::string& src_path)
{
  std::map<std::string, std::string> data;
  std::ifstream file(src_path);
  if (!file.is_open())
  {
    std::cerr << "Error opening \"" << src_path << "\"." << std::endl;
    std::exit(EXIT_FAILURE);
  }

  const std::string normal_prefix(1, kNormalPrefix);
  const std::string aggregate_prefix(1, kAggregatePrefix);

  std::string key;
  std::string value;

  auto last_data_it = data.end();
  auto aggregate_data_it = data.end();

  while (file.good())
  {
    // Example line from countryinfo.txt:
    //     data/CH/AG={"name": "Aargau"}
    // Example key:
    //     data/CH/AG
    std::getline(file, key, '=');

    if (!key.empty())
    {
      // Example value:
      //     {"name": "Aargau"}
      std::getline(file, value, '\n');

      // For example, map '-data/CH/AG' to '{"name": "Aargau"}'.
      last_data_it =
          data.emplace_hint(last_data_it, normal_prefix + key, value);

      // Aggregate keys that begin with 'data/'. We don't aggregate keys that
      // begin with 'example/' because example data is not used anywhere.
      if (key.compare(0,
                      kDataKeyPrefixLength,
                      kDataKeyPrefix,
                      kDataKeyPrefixLength) == 0)
      {
        // If aggregate_data_it and key have the same prefix, e.g. "data/ZZ".
        if (aggregate_data_it != data.end() &&
            key.compare(0,
                        kAggregateDataKeyLength,
                        aggregate_data_it->first,
                        sizeof kAggregatePrefix,
                        kAggregateDataKeyLength) == 0)
        {
          // Append more data to the aggregate string, for example:
          //     , "data/CH/AG": {"name": "Aargau"}
          aggregate_data_it->second.append(", \"" + key + "\": " + value);
        }
        else
        {
          // The countryinfo.txt file must be sorted so that subkey data
          // follows directly after its parent key data.
          assert(key.size() == kAggregateDataKeyLength);

          // Make the aggregate data strings valid. For example, this incomplete
          // JSON data:
          //     {"data/CH/AG": {"name": "Aargau"},
          //      "data/CH": {"name": "SWITZERLAND"}
          //
          // becomes valid JSON data like so:
          //
          //     {"data/CH/AG": {"name": "Aargau"},
          //      "data/CH": {"name": "SWITZERLAND"}}
          if (aggregate_data_it != data.end())
          {
            aggregate_data_it->second.push_back('}');
          }

          // Example aggregate prefixed key:
          //     +data/CH
          const std::string& aggregate_key =
              aggregate_prefix + key.substr(0, kAggregateDataKeyLength);

          // Begin a new aggregate string, for example:
          //     {"data/CH/AG": {"name": "Aargau"}
          aggregate_data_it = data.emplace_hint(
              aggregate_data_it, aggregate_key, "{\"" + key + "\": " + value);
        }
      }
    }
  }

  file.close();
  LOG_VART(data.size());
  return data;
}

const std::map<std::string, std::string>& GetData(const std::string& src_path)
{
  static const std::map<std::string, std::string> kData(InitData(src_path));
  return kData;
}

LocalAddressValidationDataSource::LocalAddressValidationDataSource(bool aggregate,
                                                                   const std::string& src_path) :
aggregate_(aggregate),
src_path_(src_path)
{
}

LocalAddressValidationDataSource::LocalAddressValidationDataSource(bool aggregate) :
aggregate_(aggregate),
src_path_(kDataFileName)
{
}

LocalAddressValidationDataSource::~LocalAddressValidationDataSource() = default;

void LocalAddressValidationDataSource::Get(const std::string& key,
                                           const Callback& data_ready) const
{
  std::string prefixed_key(1, aggregate_ ? kAggregatePrefix : kNormalPrefix);
  prefixed_key += key;
  auto data_it = GetData(src_path_).find(prefixed_key);
  bool success = data_it != GetData(src_path_).end();
  std::string* data = nullptr;
  if (success)
  {
    data = new std::string(data_it->second);
  }
  else
  {
    // URLs that start with "https://chromium-i18n.appspot.com/ssl-address/" or
    // "https://chromium-i18n.appspot.com/ssl-aggregate-address/" prefix, but do
    // not have associated data will always return "{}" with status code 200.
    // TestdataSource imitates this behavior.
    success = true;
    data = new std::string("{}");
  }
  data_ready(success, key, data);
}

}
