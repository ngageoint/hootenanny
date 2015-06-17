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
package hoot.services.review;

import hoot.services.geo.BoundingBox;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * Used to calculate a displayable bounds containing one or more bounding boxes
 * 
 * @todo this is closely coupled with the conflated data review logic and could perhaps be 
 * abstracted into hoot.services.geo or gotten id of altogether if reivew item display issues are 
 * resolved in some other way
 */
public class DisplayBoundsCalculator
{
  @SuppressWarnings("unused")
  private static final Logger log = LoggerFactory.getLogger(DisplayBoundsCalculator.class);
  
  /**
   * The types of operations that can be performed on an OSM element from a
   * changeset upload request
   */
  public enum DisplayMethod 
  {
    reviewableItemOnly, 
    reviewAgainstItemOnly, 
    reviewableAndReviewAgainstItemCombined
  }
  
  private double reviewDisplayBoundsBufferSize;
  private DisplayMethod displayMethod;
  
  public DisplayBoundsCalculator(final double reviewDisplayBoundsBufferSize, 
    final DisplayMethod displayMethod)
  {
    this.reviewDisplayBoundsBufferSize = reviewDisplayBoundsBufferSize;
    this.displayMethod = displayMethod;
  }
  
  /**
   * Calculates a displayable bounds based on the input bounds
   * 
   * @param reviewableItemBounds the bounds of the reviewable item
   * @param reviewAgainstItemBounds the bounds of the item the reviewable item is being reviewed 
   * against
   * @return a display bounds
   * @throws Exception
   */
  public BoundingBox calculateDisplayBounds(final BoundingBox reviewableItemBounds, 
    final BoundingBox reviewAgainstItemBounds) throws Exception
  {
    BoundingBox tempBounds = null;
    switch (displayMethod)
    {
      case reviewableItemOnly:
  
        tempBounds = reviewableItemBounds;
        tempBounds.adjust(reviewDisplayBoundsBufferSize);
        return tempBounds;
  
      case reviewAgainstItemOnly:
  
        tempBounds = reviewAgainstItemBounds;
        tempBounds.adjust(reviewDisplayBoundsBufferSize);
        return tempBounds;
  
      case reviewableAndReviewAgainstItemCombined:
  
        //TODO: What really needs to be done here is to reduce both elements down to nodes and find 
        //the smallest bounding box that contains the closest two nodes from both elements.  Just
        //expanding the bounds together to include all nodes in both for now, since I don't know
        //exactly how to do that yet
        return 
          BoundingBox.addBoundingBoxes(
            reviewableItemBounds, reviewAgainstItemBounds, reviewDisplayBoundsBufferSize);
  
      default:
  
        assert (false);
        return null;
    }
  }
}
