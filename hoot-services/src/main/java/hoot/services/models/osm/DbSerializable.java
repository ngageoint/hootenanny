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
package hoot.services.models.osm;

import java.util.Collection;
import java.util.List;

import com.querydsl.core.types.dsl.BooleanPath;
import com.querydsl.core.types.dsl.NumberPath;
import com.querydsl.sql.RelationalPathBase;

import hoot.services.models.osm.Element.ElementType;
import hoot.services.utils.DbUtils.EntityChangeType;


/**
 * All OSM element models which want to be serializable to the services database
 * should implement this
 */
public interface DbSerializable {
    // the record this implementation wraps
    Object getRecord();

    void setRecord(Object record);

    // related records to this element (e.g. way nodes for ways, relation
    // members for relations,
    // etc.)
    Collection<Object> getRelatedRecords();

    // type of change being made to the data (e.g. insert/update/delete)
    EntityChangeType getEntityChangeType();

    void setEntityChangeType(EntityChangeType entityChangeType);

    // table/field info for the record wrapped by this implementation
    RelationalPathBase<?> getElementTable();

    NumberPath<Long> getElementIdField();

    BooleanPath getElementVisibilityField();

    NumberPath<Long> getChangesetIdField();

    NumberPath<Long> getElementVersionField();

    // table/field info for the r wrapped by this implementation
    RelationalPathBase<?> getRelatedRecordTable();

    // field in the related record table that can be joined with the parent
    // element record table
    NumberPath<Long> getRelatedRecordJoinField();

    // OSM related element type (e.g. way nodes for ways, relation members for
    // relations) for the
    // wrapped record
    List<ElementType> getRelatedElementTypes();
}
