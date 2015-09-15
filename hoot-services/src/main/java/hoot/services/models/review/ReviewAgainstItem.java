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
 * An item that a reviewable item is reviewed against
 */
@XmlRootElement
public class ReviewAgainstItem
{
  private long id = -1;
  public long getId() { return id; }
  public void setId(long id) { this.id = id; }
  
  //primarily for debugging purposes on the client
  private String uuid = null;
  public String getUuid() { return uuid; }
  public void setUuid(String uuid) { this.uuid = uuid; }
  
  private String type;
  public String getType() { return type; }
  public void setType(String type) { this.type = type; }
  
  private String displayBounds = null;
  public String getDisplayBounds() { return displayBounds; }
  public void setDisplayBounds(String displayBounds) { 
    this.displayBounds = displayBounds; 
    }

  public ReviewAgainstItem()
  {
  }
  
  @Override
  public String toString()
  {
  	return "id: " + id + ", uuid: " + uuid + ", type: " + type + ", displayBounds: " + displayBounds;
  }
}
