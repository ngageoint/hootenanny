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

//import java.beans.IntrospectionException;

//import hoot.services.utils.ReflectUtils;

import javax.xml.bind.annotation.XmlRootElement;

/**
 * A response returned from a mark items as reviewed operation
 */
@XmlRootElement
public class MarkItemsReviewedResponse
{
  private String changesetUploadResponse;
  public String getChangesetUploadResponse()
  {
    return changesetUploadResponse;
  }
  public void setChangesetUploadResponse(String changesetUploadResponse)
  {
    this.changesetUploadResponse = changesetUploadResponse;
  }
  
  private int numItemsMarkedReviewed;
  public int getNumItemsMarkedReviewed()
  {
    return numItemsMarkedReviewed;
  }
  public void setNumItemsMarkedReviewed(int numItemsMarkedReviewed)
  {
    this.numItemsMarkedReviewed = numItemsMarkedReviewed;
  }
  
  private long changesetId;
  public long getChangesetId()
  {
    return changesetId;
  }
  public void setChangesetId(long changesetId)
  {
    this.changesetId = changesetId;
  }
  
  private long mapId;
  public long getMapId()
  {
    return mapId;
  }
  public void setMapId(long mapId)
  {
    this.mapId = mapId;
  }
  
  public MarkItemsReviewedResponse()
  {
    
  }
  
  //@Override
  //public String toString()
  //{
//    try
//    {
//      return ReflectUtils.toString(this, MarkItemsReviewedResponse.class);
//    }
//    catch (IntrospectionException e)
//    {
//      // TODO Auto-generated catch block
//      //e.printStackTrace();
//    }
//    return null;
	  
  //}
}
