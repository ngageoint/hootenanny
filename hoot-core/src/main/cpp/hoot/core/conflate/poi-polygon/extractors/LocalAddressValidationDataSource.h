
#ifndef LOCAL_ADDRESS_VALIDATION_DATA_SOURCE_H_
#define LOCAL_ADDRESS_VALIDATION_DATA_SOURCE_H_

#include <libaddressinput/source.h>

#include <string>

using namespace i18n::addressinput;

namespace hoot
{

// The name of the test data file.
extern const char kDataFileName[];

class LocalAddressValidationDataSource : public Source
{
 public:

  LocalAddressValidationDataSource(const LocalAddressValidationDataSource&) = delete;
  LocalAddressValidationDataSource& operator=(const LocalAddressValidationDataSource&) = delete;

  // Will return aggregate data if |aggregate| is set to true.
  // This constructor uses a relative path to the test file.
  explicit LocalAddressValidationDataSource(bool aggregate);

  // |src_path| is a path to the test data file.
  LocalAddressValidationDataSource(bool aggregate, const std::string& src_path);

  ~LocalAddressValidationDataSource() override;

  // Source implementation.
  void Get(const std::string& key, const Callback& data_ready) const override;

 private:

  const bool aggregate_;
  const std::string src_path_;
};

}

#endif  // LOCAL_ADDRESS_VALIDATION_DATA_SOURCE_H_
