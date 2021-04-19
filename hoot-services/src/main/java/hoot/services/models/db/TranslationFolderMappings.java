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
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
 */
package hoot.services.models.db;

import com.fasterxml.jackson.annotation.JsonProperty;

import javax.annotation.Generated;

/**
 * TranslationFolderMappings is a Querydsl bean type
 */
@Generated("com.querydsl.codegen.BeanSerializer")
public class TranslationFolderMappings {

    private Long id;

    @JsonProperty("name")
    private String displayName;

    private Long folderId;

    public void setId(Long id) {
        this.id = id;
    }

    public Long getId() {
        return id;
    }

    public void setFolderId(Long folderId) {
        this.folderId = folderId;
    }

    public Long getFolderId() {
        return folderId;
    }

    public void setDisplayName(String displayName) {
        this.displayName = displayName;
    }

    public String getDisplayName() {
        return displayName;
    }

}

