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
 * @copyright Copyright (C) 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#ifndef PARALLEL_BOUNDED_API_READER_H
#define PARALLEL_BOUNDED_API_READER_H

//  Standard
#include <mutex>
#include <thread>
#include <queue>

//  Qt
#include <QUrl>

namespace hoot
{

class HootNetworkRequest;
/**
 * @brief The ParallelBoundedApiReader class is a base class for HTTP reader classes that are bounded
 *  by an envelope bounding box.  If the bounding box exceeds the `reader.http.bbox.max.size` value
 *  (default of 0.25 degrees) then it is split up and processed.  Some APIs will respond with an error
 *  if the amount of data for an area surpasses a certain threashold (50k elements in the OSM API for
 *  example) that area is divided into quarters and reprocessed until all areas are successfully read.
 */
class ParallelBoundedApiReader
{
public:

  /**
   * @brief ParallelBoundedApiReader - Constructor
   * @param useOsmApiBboxFormat True for using the x1,y1,x2,y2 format, false for the x1,x2,y1,y2 format
   * @param addProjection True adds the projection ",EPSG:4326" to the query string
   */
  ParallelBoundedApiReader(bool useOsmApiBboxFormat = true, bool addProjection = false);
  /** Destructor that stops all threads if necessary */
  virtual ~ParallelBoundedApiReader();
  /**
   * @brief beginRead - Start the reading process by dividing up the envelope if necessary
   *   and starting the receiving threads
   * @param endpoint URL to the HTTP endpoint to query
   * @param envelope Bounding box of the area to query
   */
  void beginRead(const QUrl& endpoint, const geos::geom::Envelope& envelope);
  /**
   * @brief isComplete
   * @return True if all bounding boxes have been successfully queried
   */
  bool isComplete();
  /**
   * @brief getSingleResult
   * @param result Single, full query result (output)
   * @return True if there is a result to return, false otherwise
   */
  bool getSingleResult(QString& result);
  /**
   * @brief hasMoreResults
   * @return True if there are more results to process
   */
  bool hasMoreResults();
  /**
   * @brief wait Wait on the threads to join
   */
  void wait();
  /**
   * @brief stop Stop the threads and then wait() for them to join
   */
  void stop();
  /**
   * @brief setMaxThreads
   * @param max_threads Maximum number of reciever threads to spin up
   */
  void setMaxThreads(int max_threads) { _threadCount = max_threads; }

  /**
   * @brief setCoordGridSize
   * @param gridSize Grid division size (0.25 degrees lat/lon default)
   */
  void setCoordGridSize(double gridSize)
  { if (gridSize > 0.0) _coordGridSize = gridSize; }

  /**
   * @brief setMaxGridSize
   * @param maxSize Maximum size of an area that can be downloaded
   *        (1 square degree default)
   */
  void setMaxGridSize(double maxSize)
  { if (maxSize >= _coordGridSize) _maxGridSize = maxSize; }

  bool isError() const { return _fatalError; }

protected:

  /** Type of data that is being downloaded, for internal use in derived classes */
  enum DataType
  {
    Text,
    OsmXml,
    GeoJson,
    Json
  };
  DataType _dataType;
  /** URL of the API endpoint to query */
  QUrl _sourceUrl;
  /** Grid division size (0.25 degrees lat/lon default) */
  double _coordGridSize;
  /** Number of threads to process the HTTP requests */
  int _threadCount;

  /**
   * @brief _sleep Sleep the current thread
   */
  void _sleep() const;

private:

  /**
   * @brief _process Thread function that does the actual work of sending
   *  the HTTP request for a give area and responds to the result
   */
  void _process();
  /**
   * @brief writeDebugMap Write out the reponse from the API to a file for
   *  debugging purposes
   * @param data Response from API to write to file
   * @param name Name of file to write in $HOOT_HOME/tmp/
   */
  void writeDebugMap(const QString& data, const QString& name);

  /**
   * @brief logNetworkError Function to log an unknown network request error
   * @param request Network request object
   */
  void logNetworkError(const HootNetworkRequest& request);

  /** List of result strings, one for each HTTP response */
  QStringList _resultsList;
  /** Total number of results received, should match _totalEnvelopes at the end to ensure all data has arrived */
  int _totalResults;
  /** Mutex guarding the results list */
  std::mutex _resultsMutex;
  /** Essentially the work queue of bounding boxes that the threads query from */
  std::queue<geos::geom::Envelope> _bboxes;
  /** Total number of envelopes created to query */
  int _totalEnvelopes;
  /** Mutex guarding the bounding box list */
  std::mutex _bboxMutex;
  /** Maximum size of an area, in square degrees, that can be downloaded */
  double _maxGridSize;
  /** Processing thread pool */
  std::vector<std::thread> _threads;
  /** Mutex guarding error processing code */
  std::mutex _errorMutex;
  /** Set to true if there was a fatal error in the query */
  bool _fatalError;
  /** Flag set to true if the bounding box is to be output in the x1,y1,x2,y2 format and
   *  false will output x1,x2,y1,y2 format */
  bool _useOsmApiBboxFormat;
  /** Add the projection (",EPSG:4326") to the end of the bounding box, required for some APIs */
  bool _addProjection;
  /** Flag to stop all running threads from continuing */
  bool _continueRunning;
  /** File number used to create unique filenames for debug output */
  int _filenumber;
  /** Mutex guarding the file number */
  std::mutex _filenumberMutex;
  /** HTTP timeout in seconds */
  int _timeout;
};

}

#endif  //  PARALLEL_BOUNDED_API_READER_H
