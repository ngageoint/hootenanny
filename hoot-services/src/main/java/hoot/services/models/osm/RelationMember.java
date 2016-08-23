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

import java.util.ArrayList;
import java.util.List;

import org.apache.commons.lang3.StringUtils;

import hoot.services.models.db.CurrentRelationMembers;
import hoot.services.models.osm.Element.ElementType;


/**
 * Represents the model for a member of an OSM relation
 */
public class RelationMember {

    private final ElementType type;
    private final long id;

    private String role = "";
    private long oldId;

    public ElementType getType() {
        return type;
    }

    public long getId() {
        return id;
    }

    public long getOldId() {
        return oldId;
    }

    public void setOldId(long elementId) {
        oldId = elementId;
    }

    public String getRole() {
        return role;
    }

    public RelationMember(long id, ElementType type, String role) {
        this.id = id;
        this.type = type;
        this.role = role;
    }

    public RelationMember(long id, ElementType type) {
        this.id = id;
        this.type = type;
    }

    public RelationMember(CurrentRelationMembers record) {
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
     * @return a relation member database record
     */
    static CurrentRelationMembers createRecord(long id, int sequenceId, String role, Object elementType,
            long relationId) {
        CurrentRelationMembers memberRecord = new CurrentRelationMembers();
        memberRecord.setMemberId(id);
        memberRecord.setSequenceId(sequenceId);
        memberRecord.setMemberRole(StringUtils.isEmpty(role) ? "" : role);
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
    static List<RelationMember> fromRecords(List<CurrentRelationMembers> records) {
        List<RelationMember> members = new ArrayList<>();
        for (CurrentRelationMembers memberRecord : records) {
            members.add(new RelationMember(memberRecord));
        }
        return members;
    }
}
