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

import hoot.services.utils.StringsWebWrapper;

import javax.xml.bind.annotation.XmlRootElement;

/**
 * A response for review references to a set of UUID's
 */
@XmlRootElement
public class ReviewReferences
{
	//all review items that the specified set of UUID's still need to be reviewed against
	private StringsWebWrapper reviewAgainstItemUuids;
  public StringsWebWrapper getReviewAgainstItemUuids() { return reviewAgainstItemUuids; }
  public void setReviewAgainstItemUuids(StringsWebWrapper reviewAgainstItemUuids) 
  { 
    this.reviewAgainstItemUuids = reviewAgainstItemUuids;
  }
  
  //all reviewable items that reference the specified UUID's; uuid's populated only
  private StringsWebWrapper reviewableItemUuids;
  public StringsWebWrapper getReviewableItemUuids() { return reviewableItemUuids; }
  public void setReviewableItemUuids(StringsWebWrapper reviewableItemUuids) 
  { 
    this.reviewableItemUuids = reviewableItemUuids;
  }
  
  public ReviewReferences()
  {
  }
  
  @Override
  public String toString()
  {
    String str = "Review against item UUID's: ";
    for (String uuid : reviewAgainstItemUuids.getValues())
    {
    	str += uuid + ";";
    }
    str += "\nReviewableItem UUID's: ";
    for (String uuid : reviewableItemUuids.getValues())
    {
    	str += uuid + ";";
    }
    return str;
  }
}
