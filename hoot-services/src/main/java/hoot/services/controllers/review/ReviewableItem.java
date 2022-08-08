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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2016, 2017, 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */
package hoot.services.controllers.review;

import java.util.HashSet;
import java.util.List;
import java.util.Set;

import org.json.simple.JSONObject;

import com.querydsl.core.Tuple;

import hoot.services.models.osm.Element;
import hoot.services.models.osm.Element.ElementType;
import hoot.services.models.osm.ElementFactory;
import hoot.services.models.osm.Relation;


public class ReviewableItem implements ReviewQueryMapper {
    private long sortOrder;
    private long mapId;
    private long relationId;
    private long resultCount;
    private String bounds;

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
        o.put("bounds", bounds);

        return o.toJSONString();
    }

    public String getBounds() {
        return bounds;
    }

    public void setBounds(String bounds) {
        this.bounds = bounds;
    }

    public void setBounds(Long id) {
        //Get the relation bounds and set it on reviewable item
        Set<Long> elementIds = new HashSet<>();
        elementIds.add(id);

        List<Tuple> elementRecords = (List<Tuple>) Element.getElementRecordsWithUserInfo(this.mapId,
                ElementType.Relation, elementIds);

        Relation relationElement = (Relation) ElementFactory.create(ElementType.Relation, elementRecords.get(0), getMapId());

        setBounds(relationElement.getBounds().toServicesString());

    }
}
