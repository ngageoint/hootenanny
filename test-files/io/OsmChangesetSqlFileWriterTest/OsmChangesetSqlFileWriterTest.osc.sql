INSERT INTO changesets (id, user_id, created_at, closed_at) VALUES (1, -1, now(), now());
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile,  version) VALUES (1, 0, 0, 1, true, now(), 3221225472, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile,  version) VALUES (1, 0, 0, 1, true, now(), 3221225472, 1);
IF (SELECT count(*) FROM current_relations WHERE id=1 AND version=0) == 0 THEN
  RAISE EXCEPTION 'current_relations to be modified/deleted with ID: 1 and version: 0 does not exist';
END IF;
UPDATE relations SET visible=false WHERE relation_id=1;
UPDATE current_relations SET visible=false WHERE id=1;
IF (SELECT count(*) FROM current_ways WHERE id=3 AND version=0) == 0 THEN
  RAISE EXCEPTION 'current_ways to be modified/deleted with ID: 3 and version: 0 does not exist';
END IF;
UPDATE ways SET way_id=3, changeset_id=1, visible=true, "timestamp"=now(), version=version+1 WHERE version=0;
UPDATE current_ways SET id=3, changeset_id=1, visible=true, "timestamp"=now(), version=version+1 WHERE version=0;
DELETE FROM current_way_nodes WHERE way_id = 3;
DELETE FROM way_nodes WHERE way_id = 3;
INSERT INTO way_nodes ((way_id, node_id, version sequence_id) VALUES (3, 1, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (3, 1, 1);
INSERT INTO way_nodes ((way_id, node_id, version sequence_id) VALUES (3, 2, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (3, 2, 2);
DELETE FROM current_way_tags WHERE current_way_tags_id = 3;
DELETE FROM way_tags WHERE way_tags_id = 3;
INSERT INTO current_way_tags (way_id, k, v) VALUES (3, 'key1', 'value1');
INSERT INTO way_tags (way_id, k, v, version) VALUES (3, 'key1', 'value1', 1);
