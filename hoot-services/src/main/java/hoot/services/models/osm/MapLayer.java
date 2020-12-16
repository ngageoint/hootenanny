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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.models.osm;

import java.sql.Timestamp;
import java.text.SimpleDateFormat;

import com.fasterxml.jackson.annotation.JsonIgnore;
import com.fasterxml.jackson.annotation.JsonProperty;


/**
 * Model class for the Hootenanny map layer
 */
public class MapLayer {
    private long id;
    private String name;
    private Timestamp date;
    private String lastAccessed;
    private boolean grailReference;
    private boolean grailMerged;
    private Boolean isPublic;
    private long userId;
    private long folderId;
    private long size;
    private String bounds;

    @JsonIgnore
    public static final SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd'T'HH:mm:ss.SSS'Z'");

    public MapLayer() {}

    public long getId() {
        return id;
    }

    public void setId(long id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public long getUserId() {
        return userId;
    }

    public void setUserId(long userId) {
        this.userId = userId;
    }

    public String getDate() {
        return format.format(this.date);
    }

    public void setDate(Timestamp date) {
        this.date = new Timestamp(date.getTime());
    }

    public boolean getGrailMerged() {
        return grailMerged;
    }

    public void setGrailMerged(boolean grailMerged) {
        this.grailMerged = grailMerged;
    }

    public boolean getGrailReference() {
        return grailReference;
    }

    public void setGrailReference(boolean grailReference) {
        this.grailReference = grailReference;
    }

    public String getLastAccessed() {
        return this.lastAccessed;
    }

    public void setLastAccessed(String lastAccessed) {
        this.lastAccessed = lastAccessed;
    }
    @JsonProperty("public")
    public Boolean getPublicCol() {
        return this.isPublic;
    }
    public void setPublicCol(Boolean publicCol) {
        this.isPublic = publicCol;
    }

    public long getFolderId() {
        return folderId;
    }

    public void setFolderId(long folderId) {
        this.folderId = folderId;
    }

    public void setSize(long size) {
        this.size = size;
    }

    public long getSize() {
        return this.size;
    }

    public String getBounds() {
        return bounds;
    }

    public void setBounds(String bounds) {
        this.bounds = bounds;
    }
}
