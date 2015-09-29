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
 * Represents a reviewable conflated data entity
 */
@XmlRootElement
public class ReviewableItem
{
  private long id = -1;
  public long getId() { return id; }
  public void setId(long id) { this.id = id; }
  
  //primarily for debugging purposes on the client
  private String uuid = null;
  public String getUuid() { return uuid; }
  public void setUuid(String uuid) { this.uuid = uuid; }
  
  private String type = null;
  public String getType() { return type; }
  public void setType(String type) { this.type = type; } 
  
  private double reviewScore = -1;
  public double getReviewScore() { return reviewScore; }
  public void setReviewScore(double reviewScore) { this.reviewScore = reviewScore; }
  
  private String displayBounds = null;
  public String getDisplayBounds() { return displayBounds; }
  public void setDisplayBounds(String displayBounds) { 
  	this.displayBounds = displayBounds; 
  	}
  
  private ReviewAgainstItem itemToReviewAgainst = null;
  public ReviewAgainstItem getItemToReviewAgainst() { return itemToReviewAgainst; }
  public void setItemToReviewAgainst(ReviewAgainstItem itemToReviewAgainst) 
  { 
    this.itemToReviewAgainst = itemToReviewAgainst;
  }
  
  private long reviewId = -1;
  public long getReviewId() { return reviewId;}
  public void setReviewId(final long rid) {reviewId = rid;}
  
  private String againstList = null;
  public String getAgainstList() {return againstList;}
  public void setAgainstList(final String agList) {againstList = agList;}
  
  private long allReviewAgainstCnt = 0;
  public long getAllReviewAgainstCnt() {return allReviewAgainstCnt;}
  public void setAllReviewAgainstCnt(final long cnt) {this.allReviewAgainstCnt = cnt;}
  
  public ReviewableItem()
  {
    
  }
  
  @Override
  public String toString()
  {
    return "Reviewable Item ID: " + id + ", Reviewable Item Type: " + type + ", " +
      "Reviewable Item UUID: " + uuid + ", Review Against Item ID: " + itemToReviewAgainst.getId() + 
      ", Review Against Item Type: " + itemToReviewAgainst.getType() + 
      ", Review Against Item UUID: " + itemToReviewAgainst.getUuid();
  }
}
