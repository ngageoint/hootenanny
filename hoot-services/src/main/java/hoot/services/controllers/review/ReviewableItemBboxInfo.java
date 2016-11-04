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

import org.json.simple.JSONObject;

import hoot.services.geo.BoundingBox;


public class ReviewableItemBboxInfo extends ReviewableItemBbox {
    private String needReview;

    public ReviewableItemBboxInfo(BoundingBox bbox, long mapId, long relationId, String needReview) {
        super(bbox, mapId, relationId);
        this.needReview = needReview;
    }

    public String getNeedReview() {
        return needReview;
    }

    public void setNeedReview(String needReview) {
        this.needReview = needReview;
    }

    @Override
    public String toString() {
        JSONObject o = new JSONObject();
        o.put("bbox", this.getBbox());
        o.put("mapid", this.getMapId());
        o.put("relationid", this.getRelationId());
        o.put("needreview", needReview);
        return o.toJSONString();
    }
}
