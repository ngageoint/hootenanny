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
 * @copyright Copyright (C) 2016, 2018 Maxar (http://www.maxar.com/)
 */
package hoot.services.models.db;

import java.sql.ResultSet;
import java.sql.SQLException;

import javax.annotation.Generated;

import com.fasterxml.jackson.annotation.JsonIgnore;
import com.fasterxml.jackson.annotation.JsonProperty;

/**
 * Folders is a Querydsl bean type
 */
@Generated("com.querydsl.codegen.BeanSerializer")
public class Folders {

    private java.sql.Timestamp createdAt;

    private String displayName;

    private Long id;

    private Long parentId;

    private Boolean publicCol;

    private Long userId;

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

    public Long getParentId() {
        return parentId;
    }

    public void setParentId(Long parentId) {
        this.parentId = parentId;
    }

    @JsonProperty("public")
    public Boolean getPublicCol() {
        return publicCol;
    }

    public void setPublicCol(Boolean publicCol) {
        this.publicCol = publicCol;
    }

    public Long getUserId() {
        return userId;
    }

    public void setUserId(Long userId) {
        this.userId = userId;
    }
    @JsonIgnore
    public boolean isPrivate() {
        return !this.publicCol.booleanValue();
    }
    @JsonIgnore
    public boolean isPublic() {
        return this.publicCol.booleanValue();
    }

    public static Folders fromResultSet(ResultSet rs) throws SQLException {
        Folders f = new Folders();
        f.setCreatedAt(rs.getTimestamp("created_at"));
        f.setDisplayName(rs.getString("display_name"));
        f.setId(rs.getLong("id"));
        f.setParentId(rs.getLong("parent_id"));
        f.setPublicCol(rs.getBoolean("public"));
        f.setUserId(rs.getLong("user_id"));
        return f;
    }

}

