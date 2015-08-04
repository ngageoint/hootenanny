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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.writers.review;

import hoot.services.geo.BoundingBox;
import hoot.services.models.review.ReviewableItem;
import hoot.services.models.review.ReviewableItemsResponse;
import hoot.services.utils.JsonUtils;

import java.io.IOException;
import java.util.List;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * Response writer for a request for reviewable conflated data items
 */
public class ReviewableItemsResponseWriter
{
  private static final Logger log = LoggerFactory.getLogger(ReviewableItemsResponseWriter.class);
  
  /**
   * Writes a JSON response string containing items to be reviewed
   * 
   * @param reviewItems review items
   * @param mapId ID of the map owning the review items
   * @parm numItemsRequested number of review items requested
   * @param highestReviewScoreFirst true if reviewItems was sorted by score descending; false 
   * otherwise
   * @param reviewScoreThresholdMinimum minimum review score of any review item in reviewItems
   * @param geospatialBounds geospatial bounds used to filter the contents of reviewItems
   * @return a JSON review items string
   * @throws IOException 
   */
  public String writeResponseString(final List<ReviewableItem> reviewItems, final long mapId, 
    final int numItemsRequested, final boolean highestReviewScoreFirst, 
    final double reviewScoreThresholdMinimum, final BoundingBox geospatialBounds) 
    throws IOException
  {
    log.debug("Writing review items retrieval response...");
    
    return 
      JsonUtils.objectToJson(
        writeResponse(
          reviewItems, mapId, numItemsRequested, highestReviewScoreFirst, 
          reviewScoreThresholdMinimum, geospatialBounds));
  }
  
  /**
   * Writes a ReviewableItemsResponse object
   * 
   * @param reviewItems review items
   * @param mapId ID of the map owning the review items
   * @parm numItemsRequested number of review items requested
   * @param highestReviewScoreFirst true if reviewItems was sorted by score descending; false 
   * otherwise
   * @param reviewScoreThresholdMinimum minimum review score of any review item in reviewItems
   * @param geospatialBounds geospatial bounds used to filter the contents of reviewItems
   * @return a ReviewableItemsResponse object
   */
  public ReviewableItemsResponse writeResponse(final List<ReviewableItem> reviewItems, 
    final long mapId, final int numItemsRequested, final boolean highestReviewScoreFirst, 
    final double reviewScoreThresholdMinimum, final BoundingBox geospatialBounds)
  {
    log.debug("Writing review items retrieval response...");
    
    ReviewableItemsResponse response = new ReviewableItemsResponse();
    response.setGeospatialBounds(geospatialBounds.toServicesString());
    response.setCoordSys("ESPG:4326");
    response.setHighestReviewScoreFirst(highestReviewScoreFirst);
    response.setReviewableItems(reviewItems.toArray(new ReviewableItem[]{}));
    response.setNumItemsReturned(reviewItems.size());
    response.setMapId(mapId);
    response.setNumItemsRequested(numItemsRequested);
    response.setReviewScoreThresholdMinimum(reviewScoreThresholdMinimum);
    return response;
  }
}
