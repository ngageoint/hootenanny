#ifndef CHANGSETPROVIDER_H
#define CHANGSETPROVIDER_H

// GDAL
#include <ogr_spatialref.h>

// hoot
#include <hoot/core/elements/Element.h>

namespace hoot
{

/**
 * Enumerates the allowable changeset types
 */
class Change
{

public:

  enum ChangeType
  {
    Unknown,
    Modify,
    Delete,
    Create
  };

  ElementPtr e;
  ChangeType type;

  Change() : type(Unknown) {}
};


/**
 * Interface for classes implementing OSM changeset capabilities
 */
class ChangeSetProvider
{

public:

  /**
   * @brief getProjection
   * @return
   */
  virtual boost::shared_ptr<OGRSpatialReference> getProjection() const = 0;

  /**
   * @brief ~ElementInputStream
   *
   * If the stream is open when the destructor is called, closeStream must be called in the destructor
   */
  virtual ~ChangeSetProvider() {}

  /**
   * @brief closeStream
   *
   * Releases all resources associated with the stream, if any
   */
  virtual void close() = 0;

  /**
   *
   */
  virtual bool hasMoreChanges() = 0;

  /**
   *
   */
  virtual Change readNextChange() = 0;

};

typedef shared_ptr<ChangeSetProvider> ChangeSetProviderPtr;

}

#endif // CHANGSETPROVIDER_H
