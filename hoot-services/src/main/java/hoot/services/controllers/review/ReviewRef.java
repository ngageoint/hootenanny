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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.review;


/**
 * Element information
 */
public class ReviewRef extends ElementInfo {
    private long reviewRelationId = -1;

    public ReviewRef() {}

    public ReviewRef(String mapId, long elementId, String elementType, long reviewRelationId) {
        super(mapId, elementId, elementType);
        this.reviewRelationId = reviewRelationId;
    }

    public long getReviewRelationId() {
        return reviewRelationId;
    }

    public void setReviewRelationId(long id) {
        this.reviewRelationId = id;
    }

    @Override
    public String toString() {
        return super.toString() + ", review relation id: " + reviewRelationId;
    }
}
