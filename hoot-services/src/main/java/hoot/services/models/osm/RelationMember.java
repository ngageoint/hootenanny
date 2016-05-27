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

import java.sql.Connection;
import java.util.ArrayList;
import java.util.List;

import org.apache.commons.lang3.StringUtils;

import hoot.services.db2.CurrentRelationMembers;
import hoot.services.models.osm.Element.ElementType;


/**
 * Represents the model for a member of an OSM relation
 */
public class RelationMember {
    /**
     * element type
     */
    private ElementType type;

    public ElementType getType() {
        return type;
    }

    /**
     * member ID
     */
    private long id;

    public long getId() {
        return id;
    }

    /**
     * original member ID from the changeset upload request
     */
    private long oldId;

    public long getOldId() {
        return oldId;
    }

    public void setOldId(long elementId) {
        oldId = elementId;
    }

    /**
     * member's role
     */
    private String role = "";

    public String getRole() {
        return role;
    }

    public RelationMember(final long id, final ElementType type, final String role) {
        this.id = id;
        this.type = type;
        this.role = role;
    }

    public RelationMember(final long id, final ElementType type) {
        this.id = id;
        this.type = type;
    }

    public RelationMember(final CurrentRelationMembers record) {
        this.id = record.getMemberId();
        this.type = Element.elementTypeForElementEnum(record.getMemberType());
        this.role = record.getMemberRole();
        if (StringUtils.isEmpty(this.role)) {
            this.role = "";
        }
    }

    /**
     * Creates a relation member database record
     *
     * @param id
     *            the member's ID
     * @param sequenceId
     *            the member's sequence ordering (1..n)
     * @param role
     *            the member's role
     * @param elementType
     *            the member's element type
     * @param relationId
     *            the owning relation ID
     * @param dbConn
     *            JDBC Connection
     * @return a relation member database record
     * @throws Exception
     */
    public static CurrentRelationMembers createRecord(final long id, final int sequenceId, final String role,
            final Object elementType, final long relationId, Connection dbConn) throws Exception {
        CurrentRelationMembers memberRecord = new CurrentRelationMembers();
        memberRecord.setMemberId(id);
        memberRecord.setSequenceId(sequenceId);
        if (!StringUtils.isEmpty(role)) {
            memberRecord.setMemberRole(role);
        }
        else {
            memberRecord.setMemberRole("");
        }
        memberRecord.setMemberType(elementType);
        memberRecord.setRelationId(relationId);
        return memberRecord;
    }

    /**
     * Transforms a list of relation member database records into a list of
     * RelationMember objects
     *
     * @param records
     *            a list of relation member database records
     * @return a list of RelationMember objects
     */
    public static List<RelationMember> fromRecords(final List<CurrentRelationMembers> records) {
        List<RelationMember> members = new ArrayList<RelationMember>();
        for (CurrentRelationMembers memberRecord : records) {
            members.add(new RelationMember(memberRecord));
        }
        return members;
    }
}
