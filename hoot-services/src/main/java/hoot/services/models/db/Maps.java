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
 * @copyright Copyright (C) 2016, 2018, 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */
package hoot.services.models.db;

import java.util.HashMap;
import java.util.Map;

import javax.annotation.Generated;

import org.json.simple.JSONObject;

/**
 * Maps is a Querydsl bean type
 */
@Generated("com.querydsl.codegen.BeanSerializer")
public class Maps {

    private java.sql.Timestamp createdAt;

    private String displayName;

    private Long id;

    private Boolean publicCol;

    private Object tags;

    private Long userId;

    private Long folderId;

    private Long size;

    public java.sql.Timestamp getCreatedAt() {
        return createdAt;
    }

    public void setCreatedAt(java.sql.Timestamp createdAt) {
        this.createdAt = createdAt;
    }

    public String getDisplayName() {
        return displayName;
    }

    public void setDisplayName(String displayName) {
        this.displayName = displayName;
    }

    public Long getId() {
        return id;
    }

    public void setId(Long id) {
        this.id = id;
    }

    public Boolean getPublicCol() {
        return publicCol;
    }

    public void setPublicCol(Boolean publicCol) {
        this.publicCol = publicCol;
    }

    public Object getTags() {
        return tags;
    }

    public void setTags(Object tags) {
        this.tags = tags;
    }

    public Long getUserId() {
        return userId;
    }

    public void setUserId(Long userId) {
        this.userId = userId;
    }

    public Long getFolderId() {
        return folderId;
    }

    public void setFolderId(Long folderId) {
        this.folderId = folderId;
    }

    public void setSize(Long size) {
        this.size = size;
    }

    public Long getSize() {
        return this.size;
    }

    @Override
    public String toString() {
        Map<String,Object> map = new HashMap<String, Object>();
        map.put("createdAt", createdAt);
        map.put("tags", tags);
        map.put("id", id);
        map.put("displayName", displayName);
        map.put("userId", createdAt);
        map.put("publicCol", publicCol);
        map.put("folderId", folderId);
        map.put("size", size);
        return new JSONObject(map).toJSONString();
    }

}
