
#ifndef LIB_ADDRESS_INPUT_LOCAL_ADDRESS_VALIDATION_DATA_SOURCE_H_
#define LIB_ADDRESS_INPUT_LOCAL_ADDRESS_VALIDATION_DATA_SOURCE_H_

// hoot
#include <hoot/core/util/Configurable.h>

// libaddressinput
#include <libaddressinput/source.h>

// std
#include <string>

using namespace i18n::addressinput;

namespace hoot
{

// The name of the test data file.
extern const char kDataFileName[];

/**
 * TODO: clean up
 */
class LibAddressInputLocalAddressValidationDataSource : public Source, public Configurable
{
 public:

  LibAddressInputLocalAddressValidationDataSource(
    const LibAddressInputLocalAddressValidationDataSource&) = delete;
  LibAddressInputLocalAddressValidationDataSource& operator=(
    const LibAddressInputLocalAddressValidationDataSource&) = delete;

  // Will return aggregate data if |aggregate| is set to true.
  // This constructor uses a relative path to the test file.
  explicit LibAddressInputLocalAddressValidationDataSource(bool aggregate);

  // |src_path| is a path to the test data file.
  LibAddressInputLocalAddressValidationDataSource(bool aggregate, const std::string& src_path);

  ~LibAddressInputLocalAddressValidationDataSource() override;

  // Source implementation.
  void Get(const std::string& key, const Callback& data_ready) const override;

 private:

  const bool aggregate_;
  /*const*/ std::string src_path_;
};

}

#endif  // LIB_ADDRESS_INPUT_LOCAL_ADDRESS_VALIDATION_DATA_SOURCE_H_
