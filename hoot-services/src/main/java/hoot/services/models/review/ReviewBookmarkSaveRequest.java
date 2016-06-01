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


@XmlRootElement
public class ReviewBookmarkSaveRequest {
    private long bookmarkId;

    public long getBookmarkId() {
        return bookmarkId;
    }

    public void setBookmarkId(final long id) {
        this.bookmarkId = id;
    }

    private long mapId;

    public long getMapId() {
        return mapId;
    }

    public void setMapId(final long id) {
        this.mapId = id;
    }

    private long relationId;

    public long getRelationId() {
        return this.relationId;
    }

    public void setRelationId(final long relId) {
        this.relationId = relId;
    }

    private JSONObject detail;

    public JSONObject getDetail() {
        return this.detail;
    }

    public void setDetail(final JSONObject d) throws Exception {
        String jsonStr = d.toJSONString();
        JSONParser p = new JSONParser();
        try {
            this.detail = (JSONObject) p.parse(jsonStr);
        }
        catch (Exception ex) {
            throw new Exception("Failed to parse review bookmark detail parameter to JSON: " + ex.getMessage());
        }
    }

    private long userId;

    public long getUserid() {
        return this.userId;
    }

    public void setUserId(final long uid) {
        this.userId = uid;
    }

    public ReviewBookmarkSaveRequest(final long mapid, final long relationid, final JSONObject reviewDetail,
            final long uid) {
        this.mapId = mapid;
        this.relationId = relationid;
        this.detail = reviewDetail;
        this.userId = uid;
    }

    public ReviewBookmarkSaveRequest() {
    }
}
