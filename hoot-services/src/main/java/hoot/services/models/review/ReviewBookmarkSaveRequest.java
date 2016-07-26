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
package hoot.services.models.review;

import javax.xml.bind.annotation.XmlRootElement;

import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;


@XmlRootElement
public class ReviewBookmarkSaveRequest {
    private long bookmarkId;
    private long userId;
    private long mapId;
    private long relationId;
    private JSONObject detail;

    public ReviewBookmarkSaveRequest() {
    }

    public long getBookmarkId() {
        return bookmarkId;
    }

    public void setBookmarkId(long id) {
        this.bookmarkId = id;
    }

    public long getMapId() {
        return mapId;
    }

    public void setMapId(long id) {
        this.mapId = id;
    }

    public long getRelationId() {
        return this.relationId;
    }

    public void setRelationId(long relId) {
        this.relationId = relId;
    }

    public JSONObject getDetail() {
        return this.detail;
    }

    public void setDetail(JSONObject d) {
        String jsonStr = d.toJSONString();
        JSONParser p = new JSONParser();
        try {
            this.detail = (JSONObject) p.parse(jsonStr);
        }
        catch (ParseException ex) {
            throw new RuntimeException("Failed to convert review bookmark detail parameter to JSON!", ex);
        }
    }

    public long getUserId() {
        return this.userId;
    }

    public void setUserId(long uid) {
        this.userId = uid;
    }

    public ReviewBookmarkSaveRequest(long mapid, long relationid, JSONObject reviewDetail, long uid) {
        this.mapId = mapid;
        this.relationId = relationid;
        this.detail = reviewDetail;
        this.userId = uid;
    }
}
