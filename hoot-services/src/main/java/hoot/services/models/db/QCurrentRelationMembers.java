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
package hoot.services.models.db;

import static com.querydsl.core.types.PathMetadataFactory.*;

import com.querydsl.core.types.dsl.*;

import com.querydsl.core.types.PathMetadata;
import javax.annotation.Generated;
import com.querydsl.core.types.Path;

import com.querydsl.sql.ColumnMetadata;
import java.sql.Types;




/**
 * QCurrentRelationMembers is a Querydsl query type for CurrentRelationMembers
 */
@Generated("com.querydsl.sql.codegen.MetaDataSerializer")
public class QCurrentRelationMembers extends com.querydsl.sql.RelationalPathBase<CurrentRelationMembers> {

    private static final long serialVersionUID = 738359070;

    public static final QCurrentRelationMembers currentRelationMembers = new QCurrentRelationMembers("current_relation_members");

    public final NumberPath<Long> memberId = createNumber("memberId", Long.class);

    public final StringPath memberRole = createString("memberRole");

    public final SimplePath<Object> memberType = createSimple("memberType", Object.class);

    public final NumberPath<Long> relationId = createNumber("relationId", Long.class);

    public final NumberPath<Integer> sequenceId = createNumber("sequenceId", Integer.class);

    public final com.querydsl.sql.PrimaryKey<CurrentRelationMembers> currentRelationMembersPkey = createPrimaryKey(relationId, memberType, memberId, memberRole, sequenceId);

    public QCurrentRelationMembers(String variable) {
        super(CurrentRelationMembers.class, forVariable(variable), "public", "current_relation_members");
        addMetadata();
    }

    public QCurrentRelationMembers(String variable, String schema, String table) {
        super(CurrentRelationMembers.class, forVariable(variable), schema, table);
        addMetadata();
    }

    public QCurrentRelationMembers(Path<? extends CurrentRelationMembers> path) {
        super(path.getType(), path.getMetadata(), "public", "current_relation_members");
        addMetadata();
    }

    public QCurrentRelationMembers(PathMetadata metadata) {
        super(CurrentRelationMembers.class, metadata, "public", "current_relation_members");
        addMetadata();
    }

    public void addMetadata() {
        addMetadata(memberId, ColumnMetadata.named("member_id").withIndex(3).ofType(Types.BIGINT).withSize(19).notNull());
        addMetadata(memberRole, ColumnMetadata.named("member_role").withIndex(4).ofType(Types.VARCHAR).withSize(255).notNull());
        addMetadata(memberType, ColumnMetadata.named("member_type").withIndex(2).ofType(Types.OTHER).withSize(2147483647).notNull());
        addMetadata(relationId, ColumnMetadata.named("relation_id").withIndex(1).ofType(Types.BIGINT).withSize(19).notNull());
        addMetadata(sequenceId, ColumnMetadata.named("sequence_id").withIndex(5).ofType(Types.INTEGER).withSize(10).notNull());
    }

}

