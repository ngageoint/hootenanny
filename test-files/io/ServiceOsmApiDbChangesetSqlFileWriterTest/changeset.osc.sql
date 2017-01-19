INSERT INTO changesets (id, user_id, created_at, closed_at) VALUES (1, 1, (now() at time zone 'utc'), (now() at time zone 'utc'));
INSERT INTO changeset_tags (changeset_id, k, v) VALUES (1, 'written_by', 'Hootenanny');
/* modify way 1*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (1, 1, true, (now() at time zone 'utc'), 1);
UPDATE current_ways SET changeset_id=1, visible=true, "timestamp"=(now() at time zone 'utc'), version=1 WHERE id=1;
DELETE FROM current_way_tags WHERE way_id = 1;
DELETE FROM way_tags WHERE way_id = 1;
INSERT INTO current_way_tags (way_id, k, v) VALUES (1, 'key1', 'value1');
INSERT INTO way_tags (way_id, k, v, version) VALUES (1, 'key1', 'value1', 1);
DELETE FROM current_way_nodes WHERE way_id = 1;
DELETE FROM way_nodes WHERE way_id = 1;
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (1, 1, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (1, 1, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (1, 2, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (1, 2, 2);
/* modify node 3*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3, -88, 69, 1, true, (now() at time zone 'utc'), 2505397589, 1);
UPDATE current_nodes SET latitude=-88, longitude=69, changeset_id=1, visible=true, "timestamp"=(now() at time zone 'utc'), tile=2505397589, version=1 WHERE id=3;
DELETE FROM current_node_tags WHERE node_id = 3;
DELETE FROM node_tags WHERE node_id = 3;
/* delete way 2*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (2, 1, false, (now() at time zone 'utc'), 1);
DELETE FROM current_way_tags WHERE way_id = 2;
DELETE FROM way_tags WHERE way_id = 2;
DELETE FROM current_way_nodes WHERE way_id=2;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 2;
UPDATE current_ways SET changeset_id=1, visible=false, version=1 WHERE id=2;
/* create node 1*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1, 46, -127, 1, true, (now() at time zone 'utc'), 1789569706, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1, 46, -127, 1, true, (now() at time zone 'utc'), 1789569706, 1);
/* delete relation 1*/
INSERT INTO relations (relation_id, changeset_id, visible, "timestamp", version) VALUES (1, 1, false, (now() at time zone 'utc'), 1);
DELETE FROM current_relation_tags WHERE relation_id = 1;
DELETE FROM relation_tags WHERE relation_id = 1;
DELETE FROM current_relation_members WHERE relation_id=1;
DELETE FROM current_relation_members WHERE member_type = 'Relation' AND member_id = 1;
UPDATE current_relations SET changeset_id=1, visible=false, version=1 WHERE id=1;
/* delete way 4*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (4, 1, false, (now() at time zone 'utc'), 1);
DELETE FROM current_way_tags WHERE way_id = 4;
DELETE FROM way_tags WHERE way_id = 4;
DELETE FROM current_way_nodes WHERE way_id=4;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 4;
UPDATE current_ways SET changeset_id=1, visible=false, version=1 WHERE id=4;
/* create node 2*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2, 57, -30, 1, true, (now() at time zone 'utc'), 1789569706, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2, 57, -30, 1, true, (now() at time zone 'utc'), 1789569706, 1);
/* modify way 5*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (5, 1, true, (now() at time zone 'utc'), 1);
UPDATE current_ways SET changeset_id=1, visible=true, "timestamp"=(now() at time zone 'utc'), version=1 WHERE id=5;
DELETE FROM current_way_tags WHERE way_id = 5;
DELETE FROM way_tags WHERE way_id = 5;
INSERT INTO current_way_tags (way_id, k, v) VALUES (5, 'key1', 'value1');
INSERT INTO way_tags (way_id, k, v, version) VALUES (5, 'key1', 'value1', 1);
DELETE FROM current_way_nodes WHERE way_id = 5;
DELETE FROM way_nodes WHERE way_id = 5;
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (5, 12, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (5, 12, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (5, 13, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (5, 13, 2);
/* create relation 1*/
INSERT INTO relations (relation_id, changeset_id, visible, "timestamp", version) VALUES (1, 1, true, (now() at time zone 'utc'), 1);
INSERT INTO current_relations (id, changeset_id, visible, "timestamp", version) VALUES (1, 1, true, (now() at time zone 'utc'), 1);
INSERT INTO current_relation_tags (relation_id, k, v) VALUES (1, 'key2', 'value2');
INSERT INTO relation_tags (relation_id, k, v, version) VALUES (1, 'key2', 'value2', 1);
INSERT INTO current_relation_tags (relation_id, k, v) VALUES (1, 'type', '');
INSERT INTO relation_tags (relation_id, k, v, version) VALUES (1, 'type', '', 1);
INSERT INTO relation_members (relation_id, member_type, member_id, member_role, version, sequence_id) VALUES (1, 'Node', 14, 'role1', 1, 1);
INSERT INTO current_relation_members (relation_id, member_type, member_id, member_role, sequence_id) VALUES (1, 'Node', 14, 'role1', 1);
INSERT INTO relation_members (relation_id, member_type, member_id, member_role, version, sequence_id) VALUES (1, 'Way', 6, 'role2', 1, 2);
INSERT INTO current_relation_members (relation_id, member_type, member_id, member_role, sequence_id) VALUES (1, 'Way', 6, 'role2', 2);
/* delete node 16*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (16, -43, -6, 1, false, (now() at time zone 'utc'), 1073741823, 1);
DELETE FROM current_node_tags WHERE node_id = 16;
DELETE FROM node_tags WHERE node_id = 16;
DELETE FROM current_way_nodes WHERE node_id=16;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 16;
UPDATE current_nodes SET changeset_id=1, visible=false, version=1 WHERE id=16;
UPDATE changesets SET min_lat=-88, max_lat=57, min_lon=-127, max_lon=69, num_changes=10 WHERE id=1;
