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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.models.review;

import javax.xml.bind.annotation.XmlRootElement;

import hoot.services.models.osm.ElementInfo;


/**
 * Response to a review references web request
 */
@XmlRootElement
public class ReviewRefsResponse {
    private ReviewRef[] reviewRefs;

    public ReviewRef[] getReviewRefs() {
        return reviewRefs;
    }

    public void setReviewRefs(ReviewRef[] refs) {
        this.reviewRefs = refs;
    }

    private ElementInfo queryElementInfo;

    public ElementInfo getQueryElementInfo() {
        return queryElementInfo;
    }

    public void setQueryElementInfo(ElementInfo info) {
        this.queryElementInfo = info;
    }

    public ReviewRefsResponse() {
    }

    public ReviewRefsResponse(final ElementInfo requestingElementInfo, final ReviewRef[] reviewReferences) {
        this.queryElementInfo = requestingElementInfo;
        this.reviewRefs = reviewReferences;
    }

    @Override
    public String toString() {
        String str = "Request element " + queryElementInfo.toString() + ", Review references:\n";
        for (int i = 0; i < reviewRefs.length; i++) {
            str += reviewRefs[i].toString() + "\n";
        }
        return str;
    }
}