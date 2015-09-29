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
package hoot.services.models.review;

import javax.xml.bind.annotation.XmlRootElement;

/**
 * Represents statistics for a group of reviewable conflated data items associated with a particular
 * map ID.
 */
@XmlRootElement
public class ReviewableItemsStatistics
{
  private long mapId;
  public long getMapId()
  {
    return mapId;
  }
  public void setMapId(long mapId)
  {
    this.mapId = mapId;
  }
  
  private long numTotalItems;
  public long getNumTotalItems()
  {
    return numTotalItems;
  }
  public void setNumTotalItems(long numTotalItems)
  {
    this.numTotalItems = numTotalItems;
  }
  
  private int numReviewableItems;
  public int getNumReviewableItems()
  {
    return numReviewableItems;
  }
  public void setNumReviewableItems(int numReviewableItems)
  {
    this.numReviewableItems = numReviewableItems;
  }
  
  private int numReviewedItems;
  public int getNumReviewedItems()
  {
    return numReviewedItems;
  }
  public void setNumReviewedItems(int numReviewedItems)
  {
    this.numReviewedItems = numReviewedItems;
  }
  
  public ReviewableItemsStatistics()
  {
  }
}
