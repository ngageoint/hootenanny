INSERT INTO changesets (id, user_id, created_at, closed_at) VALUES (1, 1, now(), now());
INSERT INTO changeset_tags (changeset_id, k, v) VALUES (1, 'written_by', 'Hootenanny');
/* create node 1*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1, 0, 0, 1, true, now(), 3221225472, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1, 0, 0, 1, true, now(), 3221225472, 1);
/* delete relation 1*/
INSERT INTO relations (relation_id, changeset_id, visible, "timestamp", version) VALUES (1, 1, false, now(), 1);
DELETE FROM current_relation_tags WHERE relation_id = 1;
DELETE FROM relation_tags WHERE relation_id = 1;
DELETE FROM current_relation_members WHERE relation_id=1;
DELETE FROM current_relation_members WHERE member_type = 'Relation' AND member_id = 1;
UPDATE current_relations SET changeset_id=1, visible=false, version=1 WHERE id=1;
/* modify way 3*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (3, 1, true, now(), 1);
UPDATE current_ways SET changeset_id=1, visible=true, "timestamp"=now(), version=1 WHERE id=3;
DELETE FROM current_way_tags WHERE way_id = 3;
DELETE FROM way_tags WHERE way_id = 3;
INSERT INTO current_way_tags (way_id, k, v) VALUES (3, 'key1', 'value1');
INSERT INTO way_tags (way_id, k, v, version) VALUES (3, 'key1', 'value1', 1);
DELETE FROM current_way_nodes WHERE way_id = 3;
DELETE FROM way_nodes WHERE way_id = 3;
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (3, 1, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (3, 1, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (3, 2, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (3, 2, 2);
