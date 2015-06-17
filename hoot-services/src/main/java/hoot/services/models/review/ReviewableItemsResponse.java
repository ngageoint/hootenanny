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
 * @copyright Copyright (C) 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.models.review;

import javax.xml.bind.annotation.XmlRootElement;

/**
 * Represents the response to a request for reviewable conflated data items owned by a particular
 * map ID.
 */
@XmlRootElement
public class ReviewableItemsResponse
{
  private long mapId;
  public long getMapId() { return mapId; }
  public void setMapId(long mapId) { this.mapId = mapId; }
  
  private int numItemsRequested;
  public int getNumItemsRequested() { return numItemsRequested; }
  public void setNumItemsRequested(int numItemsRequested) 
  { 
    this.numItemsRequested = numItemsRequested;
  }
  
  private int numItemsReturned;
  public int getNumItemsReturned() { return numItemsReturned; }
  public void setNumItemsReturned(int numItemsReturned) 
  { 
    this.numItemsReturned = numItemsReturned;
  }
  
  private double reviewScoreThresholdMinimum;
  public double getReviewScoreThresholdMinimum() { return reviewScoreThresholdMinimum; }
  public void setReviewScoreThresholdMinimum(double reviewScoreThresholdMinimum) 
  {
    this.reviewScoreThresholdMinimum = reviewScoreThresholdMinimum;
  }
  
  private boolean highestReviewScoreFirst;
  public boolean getHighestReviewScoreFirst() { return highestReviewScoreFirst; }
  public void setHighestReviewScoreFirst(boolean highestReviewScoreFirst)
  {
    this.highestReviewScoreFirst = highestReviewScoreFirst;
  }
  
  private String geospatialBounds;
  public String getGeospatialBounds() { return geospatialBounds; }
  public void setGeospatialBounds(String geospatialBounds) 
  { 
    this.geospatialBounds = geospatialBounds;
  }
  
  private String coordSys;
  public String getCoordSys() { return coordSys; }
  public void setCoordSys(String coordSys) { this.coordSys = coordSys; }
  
  private ReviewableItem[] reviewableItems;
  public ReviewableItem[] getReviewableItems() { return reviewableItems; }
  public void setReviewableItems(ReviewableItem[] reviewableItems) 
  { 
    this.reviewableItems = reviewableItems;
  }
  
  public ReviewableItemsResponse()
  {
    
  }
}
