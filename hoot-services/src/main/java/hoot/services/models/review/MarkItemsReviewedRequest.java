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

import org.apache.commons.lang3.StringUtils;

/**
 * A request to mark items as reviewed
 */
@XmlRootElement
public class MarkItemsReviewedRequest
{
  private String reviewedItemsChangeset;
  public String getReviewedItemsChangeset() { return reviewedItemsChangeset; }
  public void setReviewedItemsChangeset(String reviewedItemsChangeset) 
  { this.reviewedItemsChangeset = reviewedItemsChangeset; }
  
  private ReviewedItems reviewedItems;
  public ReviewedItems getReviewedItems() { return reviewedItems; }
  public void setReviewedItems(ReviewedItems reviewedItems) { this.reviewedItems = reviewedItems; }
  
  public MarkItemsReviewedRequest()
  {
    
  }
  
  @Override
  public String toString()
  {
    String str = "";
    if (StringUtils.trimToNull(reviewedItemsChangeset) != null)
    {
      str += "reviewedItemsChangeset:\n";
      str += reviewedItemsChangeset;
      str += "\n\n";
    }
    if (reviewedItems != null && reviewedItems.getReviewedItems() != null)
    {
      str += "reviewedItems:\n";
      for (ReviewedItem reviewedItem : reviewedItems.getReviewedItems())
      {
        str += reviewedItem.toString() + "\n";  
      }
    }
    if (StringUtils.trimToNull(reviewedItemsChangeset) == null && 
      (reviewedItems == null || reviewedItems.getReviewedItems() == null))
    {
      return "empty object";
    }
    return str;
  }
}
