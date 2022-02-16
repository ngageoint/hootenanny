INSERT INTO changesets (id, user_id, created_at, closed_at) VALUES (3, 1, (now() at time zone 'utc'), (now() at time zone 'utc'));
INSERT INTO changeset_tags (changeset_id, k, v) VALUES (3, 'written_by', 'Hootenanny');
/* create node 37 */
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (37, 388549524, 1048987389, 3, true, (now() at time zone 'utc'), 3852309038, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (37, 388549524, 1048987389, 3, true, (now() at time zone 'utc'), 3852309038, 1);
/* create node 38 */
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (38, 388549321, 1048979050, 3, true, (now() at time zone 'utc'), 3852309038, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (38, 388549321, 1048979050, 3, true, (now() at time zone 'utc'), 3852309038, 1);
/* create way 5 */
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (5, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (5, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (5, 'highway', 'road');
INSERT INTO way_tags (way_id, k, v, version) VALUES (5, 'highway', 'road', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (5, 'note', '1');
INSERT INTO way_tags (way_id, k, v, version) VALUES (5, 'note', '1', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (5, 1669731, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (5, 1669731, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (5, 37, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (5, 37, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (5, 38, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (5, 38, 3);
UPDATE changesets SET min_lat=388549321, max_lat=388549524, min_lon=1048979050, max_lon=1048987389, num_changes=3 WHERE id=3;
