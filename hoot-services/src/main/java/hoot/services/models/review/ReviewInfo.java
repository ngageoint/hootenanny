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
 * Review information
 */
@XmlRootElement
public class ReviewInfo
{
  private long elementId = -1;
  public long getElementId() { return elementId; }
  public void setElementId(long id) { this.elementId = id; }
  
  private String uuid = null;
  public String getUuid() { return uuid; }
  public void setUuid(String id) { this.uuid = id; }
  
  private long mapId = -1;
  public long getMapId() { return mapId; }
  public void setMapId(long id) { this.mapId = id; }
  
  private String elementType;
  public String getElementType() { return elementType; }
  public void setElementType(String type) { this.elementType = type; }

  public ReviewInfo()
  {
  }
  
  @Override
  public String toString()
  {
  	return "map id: " + mapId + ", element id: " + elementId + ", unique ID: " + uuid + 
  		", element type: " + elementType;
  }
}
