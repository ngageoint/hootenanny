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
 * Represents a conflated data item that has been reviewed
 */
@XmlRootElement
public class ReviewedItem
{
  private long id = -1;
  public long getId() { return id; }
  public void setId(long id) { this.id = id; }
  
  private String type = null;
  public String getType() { return type; }
  public void setType(String type) { this.type = type; } 
  
  private long reviewedAgainstId = -1;
  public long getReviewedAgainstId() { return reviewedAgainstId; }
  public void setReviewedAgainstId(long id) { this.reviewedAgainstId = id; }
  
  private String reviewedAgainstType = null;
  public String getReviewedAgainstType() { return reviewedAgainstType; }
  public void setReviewedAgainstType(String type) { this.reviewedAgainstType = type; } 
  
  public ReviewedItem()
  {
    
  }
  
  @Override
  public String toString()
  {
    return "Reviewable Item ID: " + id + ", Reviewable Item Type: " + type + ", " +
      "Review Against Item ID: " + reviewedAgainstId + ", Review Against Item Type: " + 
      reviewedAgainstType;
  }
}
