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


public class ReviewableItem implements ReviewQueryMapper {
    private long sortOrder;
    private long mapId;
    private long relationId;
    private long resultCount;

    public ReviewableItem(long sortOrder, long mapid, long relationid) {
        this.sortOrder = sortOrder;
        this.mapId = mapid;
        this.relationId = relationid;
    }

    public long getRelationId() {
        return relationId;
    }

    public void setRelationId(long relationId) {
        this.relationId = relationId;
    }

    public long getSortOrder() {
        return sortOrder;
    }

    public void setSortOrder(long sortOrder) {
        this.sortOrder = sortOrder;
    }

    public long getMapId() {
        return mapId;
    }

    public void setMapId(long mapId) {
        this.mapId = mapId;
    }

    public long getResultCount() {
        return resultCount;
    }

    public void setResultCount(long cnt) {
        resultCount = cnt;
    }

    @Override
    public String toString() {
        JSONObject o = new JSONObject();
        o.put("sortorder", sortOrder);
        o.put("mapid", mapId);
        o.put("relationid", relationId);

        return o.toJSONString();
    }
}
