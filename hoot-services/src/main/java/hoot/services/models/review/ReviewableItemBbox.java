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

import org.json.simple.JSONObject;

import hoot.services.geo.BoundingBox;


public class ReviewableItemBbox extends ReviewQueryMapper {

    private long _mapId;
    private long _relationId;
    private BoundingBox _bbox;

    public ReviewableItemBbox(final BoundingBox bbox, final long mapid, final long relationid) {
        _bbox = bbox;
        _mapId = mapid;
        _relationId = relationid;
    }

    @Override
    public String toString() {
        JSONObject o = new JSONObject();
        o.put("bbox", _bbox.toServicesString());
        o.put("mapid", _mapId);
        o.put("relationid", _relationId);

        return o.toJSONString();
    }

    public long getRelationId() {
        return _relationId;
    }

    public void setRelationId(final long relid) {
        _relationId = relid;
    }

    public BoundingBox getBbox() {
        return _bbox;
    }

    public void setBbox(final BoundingBox bbox) {
        _bbox = bbox;
    }

    public long getMapId() {
        return _mapId;
    }

    public void setMapId(final long mapid) {
        _mapId = mapid;
    }
}
