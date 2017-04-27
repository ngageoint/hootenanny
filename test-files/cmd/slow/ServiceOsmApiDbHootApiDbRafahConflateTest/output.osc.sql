INSERT INTO changesets (id, user_id, created_at, closed_at) VALUES (3, 1, (now() at time zone 'utc'), (now() at time zone 'utc'));
INSERT INTO changeset_tags (changeset_id, k, v) VALUES (3, 'written_by', 'Hootenanny');
/* create node 54*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (54, 311806942, 340508435, 3, true, (now() at time zone 'utc'), 3335527272, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (54, 311806942, 340508435, 3, true, (now() at time zone 'utc'), 3335527272, 1);
/* create node 55*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (55, 311815647, 340472592, 3, true, (now() at time zone 'utc'), 3335527272, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (55, 311815647, 340472592, 3, true, (now() at time zone 'utc'), 3335527272, 1);
/* create node 56*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (56, 311816708, 340482519, 3, true, (now() at time zone 'utc'), 3335527272, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (56, 311816708, 340482519, 3, true, (now() at time zone 'utc'), 3335527272, 1);
/* create node 57*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (57, 311832952, 340565418, 3, true, (now() at time zone 'utc'), 3335527275, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (57, 311832952, 340565418, 3, true, (now() at time zone 'utc'), 3335527275, 1);
/* create node 58*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (58, 311821476, 340507316, 3, true, (now() at time zone 'utc'), 3335527272, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (58, 311821476, 340507316, 3, true, (now() at time zone 'utc'), 3335527272, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (58, 'hoot', 'blank');
INSERT INTO node_tags (node_id, k, v, version) VALUES (58, 'hoot', 'blank', 1);
/* create node 59*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (59, 311811937, 340510083, 3, true, (now() at time zone 'utc'), 3335527272, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (59, 311811937, 340510083, 3, true, (now() at time zone 'utc'), 3335527272, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (59, 'hoot', 'blank');
INSERT INTO node_tags (node_id, k, v, version) VALUES (59, 'hoot', 'blank', 1);
/* create node 60*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (60, 311806916, 340508148, 3, true, (now() at time zone 'utc'), 3335527272, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (60, 311806916, 340508148, 3, true, (now() at time zone 'utc'), 3335527272, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (60, 'hoot', 'blank');
INSERT INTO node_tags (node_id, k, v, version) VALUES (60, 'hoot', 'blank', 1);
/* create node 61*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (61, 311801012, 340526027, 3, true, (now() at time zone 'utc'), 3335527272, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (61, 311801012, 340526027, 3, true, (now() at time zone 'utc'), 3335527272, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (61, 'hoot', 'blank');
INSERT INTO node_tags (node_id, k, v, version) VALUES (61, 'hoot', 'blank', 1);
/* create node 62*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (62, 311790696, 340528372, 3, true, (now() at time zone 'utc'), 3335527231, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (62, 311790696, 340528372, 3, true, (now() at time zone 'utc'), 3335527231, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (62, 'hoot', 'blank');
INSERT INTO node_tags (node_id, k, v, version) VALUES (62, 'hoot', 'blank', 1);
/* create node 63*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (63, 311798582, 340563643, 3, true, (now() at time zone 'utc'), 3335527274, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (63, 311798582, 340563643, 3, true, (now() at time zone 'utc'), 3335527274, 1);
/* create node 64*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (64, 311798141, 340513880, 3, true, (now() at time zone 'utc'), 3335527272, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (64, 311798141, 340513880, 3, true, (now() at time zone 'utc'), 3335527272, 1);
/* create node 65*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (65, 311790644, 340528071, 3, true, (now() at time zone 'utc'), 3335527231, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (65, 311790644, 340528071, 3, true, (now() at time zone 'utc'), 3335527231, 1);
/* create node 66*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (66, 311800186, 340525847, 3, true, (now() at time zone 'utc'), 3335527272, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (66, 311800186, 340525847, 3, true, (now() at time zone 'utc'), 3335527272, 1);
/* modify way 4*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (4, 3, true, (now() at time zone 'utc'), 2);
UPDATE current_ways SET changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), version=2 WHERE id=4;
DELETE FROM current_way_tags WHERE way_id = 4;
DELETE FROM way_tags WHERE way_id = 4;
INSERT INTO current_way_tags (way_id, k, v) VALUES (4, 'source', 'digitalglobe');
INSERT INTO way_tags (way_id, k, v, version) VALUES (4, 'source', 'digitalglobe', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (4, 'highway', 'residential');
INSERT INTO way_tags (way_id, k, v, version) VALUES (4, 'highway', 'residential', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (4, 'source:datetime', '2016-05-02T23:46:04Z');
INSERT INTO way_tags (way_id, k, v, version) VALUES (4, 'source:datetime', '2016-05-02T23:46:04Z', 2);
DELETE FROM current_way_nodes WHERE way_id = 4;
DELETE FROM way_nodes WHERE way_id = 4;
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (4, 25, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (4, 25, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (4, 54, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (4, 54, 2);
/* modify way 10*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (10, 3, true, (now() at time zone 'utc'), 2);
UPDATE current_ways SET changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), version=2 WHERE id=10;
DELETE FROM current_way_tags WHERE way_id = 10;
DELETE FROM way_tags WHERE way_id = 10;
INSERT INTO current_way_tags (way_id, k, v) VALUES (10, 'source', 'Bing');
INSERT INTO way_tags (way_id, k, v, version) VALUES (10, 'source', 'Bing', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (10, 'highway', 'unclassified');
INSERT INTO way_tags (way_id, k, v, version) VALUES (10, 'highway', 'unclassified', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (10, 'source:datetime', '2016-08-06T20:08:03Z');
INSERT INTO way_tags (way_id, k, v, version) VALUES (10, 'source:datetime', '2016-08-06T20:08:03Z', 2);
DELETE FROM current_way_nodes WHERE way_id = 10;
DELETE FROM way_nodes WHERE way_id = 10;
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (10, 15, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (10, 15, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (10, 63, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (10, 63, 2);
/* modify way 18*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (18, 3, true, (now() at time zone 'utc'), 2);
UPDATE current_ways SET changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), version=2 WHERE id=18;
DELETE FROM current_way_tags WHERE way_id = 18;
DELETE FROM way_tags WHERE way_id = 18;
INSERT INTO current_way_tags (way_id, k, v) VALUES (18, 'source', 'digitalglobe');
INSERT INTO way_tags (way_id, k, v, version) VALUES (18, 'source', 'digitalglobe', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (18, 'highway', 'residential');
INSERT INTO way_tags (way_id, k, v, version) VALUES (18, 'highway', 'residential', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (18, 'source:datetime', '2016-05-02T23:46:09Z');
INSERT INTO way_tags (way_id, k, v, version) VALUES (18, 'source:datetime', '2016-05-02T23:46:09Z', 2);
DELETE FROM current_way_nodes WHERE way_id = 18;
DELETE FROM way_nodes WHERE way_id = 18;
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (18, 64, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (18, 64, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (18, 39, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (18, 39, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (18, 5, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (18, 5, 3);
/* modify way 20*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (20, 3, true, (now() at time zone 'utc'), 2);
UPDATE current_ways SET changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), version=2 WHERE id=20;
DELETE FROM current_way_tags WHERE way_id = 20;
DELETE FROM way_tags WHERE way_id = 20;
INSERT INTO current_way_tags (way_id, k, v) VALUES (20, 'source', 'digitalglobe');
INSERT INTO way_tags (way_id, k, v, version) VALUES (20, 'source', 'digitalglobe', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (20, 'highway', 'residential');
INSERT INTO way_tags (way_id, k, v, version) VALUES (20, 'highway', 'residential', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (20, 'source:datetime', '2016-05-02T23:46:09Z');
INSERT INTO way_tags (way_id, k, v, version) VALUES (20, 'source:datetime', '2016-05-02T23:46:09Z', 2);
DELETE FROM current_way_nodes WHERE way_id = 20;
DELETE FROM way_nodes WHERE way_id = 20;
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (20, 66, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (20, 66, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (20, 65, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (20, 65, 2);
/* create way 24*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (24, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (24, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (24, 'source', 'Bing');
INSERT INTO way_tags (way_id, k, v, version) VALUES (24, 'source', 'Bing', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (24, 'highway', 'unclassified');
INSERT INTO way_tags (way_id, k, v, version) VALUES (24, 'highway', 'unclassified', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (24, 'source:datetime', '2016-08-06T20:08:03Z');
INSERT INTO way_tags (way_id, k, v, version) VALUES (24, 'source:datetime', '2016-08-06T20:08:03Z', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (24, 30, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (24, 30, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (24, 12, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (24, 12, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (24, 29, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (24, 29, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (24, 13, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (24, 13, 4);
/* create way 25*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (25, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (25, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (25, 'source', 'Bing');
INSERT INTO way_tags (way_id, k, v, version) VALUES (25, 'source', 'Bing', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (25, 'highway', 'unclassified');
INSERT INTO way_tags (way_id, k, v, version) VALUES (25, 'highway', 'unclassified', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (25, 'source:datetime', '2016-08-06T20:08:03Z');
INSERT INTO way_tags (way_id, k, v, version) VALUES (25, 'source:datetime', '2016-08-06T20:08:03Z', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (25, 63, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (25, 63, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (25, 26, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (25, 26, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (25, 31, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (25, 31, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (25, 10, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (25, 10, 4);
/* create way 26*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (26, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (26, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (26, 'highway', 'road');
INSERT INTO way_tags (way_id, k, v, version) VALUES (26, 'highway', 'road', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (26, 58, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (26, 58, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (26, 57, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (26, 57, 2);
/* create way 27*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (27, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (27, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (27, 'highway', 'road');
INSERT INTO way_tags (way_id, k, v, version) VALUES (27, 'highway', 'road', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (27, 55, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (27, 55, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (27, 56, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (27, 56, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (27, 58, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (27, 58, 3);
/* create way 28*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (28, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (28, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (28, 'source', 'Bing');
INSERT INTO way_tags (way_id, k, v, version) VALUES (28, 'source', 'Bing', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (28, 'highway', 'unclassified');
INSERT INTO way_tags (way_id, k, v, version) VALUES (28, 'highway', 'unclassified', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (28, 'source:datetime', '2016-08-06T20:08:03Z');
INSERT INTO way_tags (way_id, k, v, version) VALUES (28, 'source:datetime', '2016-08-06T20:08:03Z', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (28, 10, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (28, 10, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (28, 27, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (28, 27, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (28, 30, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (28, 30, 3);
/* create way 29*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (29, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (29, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (29, 'highway', 'road');
INSERT INTO way_tags (way_id, k, v, version) VALUES (29, 'highway', 'road', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (29, 64, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (29, 64, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (29, 61, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (29, 61, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (29, 66, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (29, 66, 3);
/* create way 30*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (30, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (30, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (30, 'source', 'digitalglobe');
INSERT INTO way_tags (way_id, k, v, version) VALUES (30, 'source', 'digitalglobe', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (30, 'highway', 'residential');
INSERT INTO way_tags (way_id, k, v, version) VALUES (30, 'highway', 'residential', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (30, 'source:datetime', '2016-05-02T23:46:09Z');
INSERT INTO way_tags (way_id, k, v, version) VALUES (30, 'source:datetime', '2016-05-02T23:46:09Z', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (30, 6, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (30, 6, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (30, 64, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (30, 64, 2);
/* create way 31*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (31, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (31, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (31, 'source', 'digitalglobe');
INSERT INTO way_tags (way_id, k, v, version) VALUES (31, 'source', 'digitalglobe', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (31, 'highway', 'residential');
INSERT INTO way_tags (way_id, k, v, version) VALUES (31, 'highway', 'residential', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (31, 'source:datetime', '2016-05-02T23:46:09Z');
INSERT INTO way_tags (way_id, k, v, version) VALUES (31, 'source:datetime', '2016-05-02T23:46:09Z', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (31, 65, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (31, 65, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (31, 30, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (31, 30, 2);
/* create way 32*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (32, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (32, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (32, 'source', 'digitalglobe');
INSERT INTO way_tags (way_id, k, v, version) VALUES (32, 'source', 'digitalglobe', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (32, 'highway', 'residential');
INSERT INTO way_tags (way_id, k, v, version) VALUES (32, 'highway', 'residential', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (32, 'source:datetime', '2016-05-02T23:46:09Z');
INSERT INTO way_tags (way_id, k, v, version) VALUES (32, 'source:datetime', '2016-05-02T23:46:09Z', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (32, 5, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (32, 5, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (32, 66, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (32, 66, 2);
/* create way 33*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (33, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (33, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (33, 'highway', 'road');
INSERT INTO way_tags (way_id, k, v, version) VALUES (33, 'highway', 'road', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (33, 54, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (33, 54, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (33, 25, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (33, 25, 2);
/* create way 34*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (34, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (34, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (34, 'source', 'digitalglobe');
INSERT INTO way_tags (way_id, k, v, version) VALUES (34, 'source', 'digitalglobe', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (34, 'highway', 'residential');
INSERT INTO way_tags (way_id, k, v, version) VALUES (34, 'highway', 'residential', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (34, 'source:datetime', '2016-05-02T23:46:04Z');
INSERT INTO way_tags (way_id, k, v, version) VALUES (34, 'source:datetime', '2016-05-02T23:46:04Z', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (34, 54, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (34, 54, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (34, 3, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (34, 3, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (34, 4, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (34, 4, 3);
/* create way 35*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (35, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (35, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (35, 'highway', 'road');
INSERT INTO way_tags (way_id, k, v, version) VALUES (35, 'highway', 'road', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (35, 58, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (35, 58, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (35, 59, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (35, 59, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (35, 17, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (35, 17, 3);
/* create relation 4*/
INSERT INTO relations (relation_id, changeset_id, visible, "timestamp", version) VALUES (4, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_relations (id, changeset_id, visible, "timestamp", version) VALUES (4, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_relation_tags (relation_id, k, v) VALUES (4, 'hoot:review:type', 'Highway');
INSERT INTO relation_tags (relation_id, k, v, version) VALUES (4, 'hoot:review:type', 'Highway', 1);
INSERT INTO current_relation_tags (relation_id, k, v) VALUES (4, 'hoot:review:needs', 'yes');
INSERT INTO relation_tags (relation_id, k, v, version) VALUES (4, 'hoot:review:needs', 'yes', 1);
INSERT INTO current_relation_tags (relation_id, k, v) VALUES (4, 'hoot:review:members', '1');
INSERT INTO relation_tags (relation_id, k, v, version) VALUES (4, 'hoot:review:members', '1', 1);
INSERT INTO current_relation_tags (relation_id, k, v) VALUES (4, 'hoot:review:score', '-1');
INSERT INTO relation_tags (relation_id, k, v, version) VALUES (4, 'hoot:review:score', '-1', 1);
INSERT INTO current_relation_tags (relation_id, k, v) VALUES (4, 'hoot:review:note', 'Missing match pair');
INSERT INTO relation_tags (relation_id, k, v, version) VALUES (4, 'hoot:review:note', 'Missing match pair', 1);
INSERT INTO current_relation_tags (relation_id, k, v) VALUES (4, 'type', 'review');
INSERT INTO relation_tags (relation_id, k, v, version) VALUES (4, 'type', 'review', 1);
/* create relation 5*/
INSERT INTO relations (relation_id, changeset_id, visible, "timestamp", version) VALUES (5, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_relations (id, changeset_id, visible, "timestamp", version) VALUES (5, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_relation_tags (relation_id, k, v) VALUES (5, 'hoot:review:type', 'Highway');
INSERT INTO relation_tags (relation_id, k, v, version) VALUES (5, 'hoot:review:type', 'Highway', 1);
INSERT INTO current_relation_tags (relation_id, k, v) VALUES (5, 'hoot:review:needs', 'yes');
INSERT INTO relation_tags (relation_id, k, v, version) VALUES (5, 'hoot:review:needs', 'yes', 1);
INSERT INTO current_relation_tags (relation_id, k, v) VALUES (5, 'hoot:review:members', '2');
INSERT INTO relation_tags (relation_id, k, v, version) VALUES (5, 'hoot:review:members', '2', 1);
INSERT INTO current_relation_tags (relation_id, k, v) VALUES (5, 'hoot:review:score', '0');
INSERT INTO relation_tags (relation_id, k, v, version) VALUES (5, 'hoot:review:score', '0', 1);
INSERT INTO current_relation_tags (relation_id, k, v) VALUES (5, 'hoot:review:note', 'Semi-similar highway orientation. Highway edges not very close to each other.');
INSERT INTO relation_tags (relation_id, k, v, version) VALUES (5, 'hoot:review:note', 'Semi-similar highway orientation. Highway edges not very close to each other.', 1);
INSERT INTO current_relation_tags (relation_id, k, v) VALUES (5, 'type', 'review');
INSERT INTO relation_tags (relation_id, k, v, version) VALUES (5, 'type', 'review', 1);
INSERT INTO current_relation_tags (relation_id, k, v) VALUES (5, 'hoot:review:sort_order', '0');
INSERT INTO relation_tags (relation_id, k, v, version) VALUES (5, 'hoot:review:sort_order', '0', 1);
INSERT INTO relation_members (relation_id, member_type, member_id, member_role, version, sequence_id) VALUES (5, 'Way', 19, 'reviewee', 1, 1);
INSERT INTO current_relation_members (relation_id, member_type, member_id, member_role, sequence_id) VALUES (5, 'Way', 19, 'reviewee', 1);
INSERT INTO relation_members (relation_id, member_type, member_id, member_role, version, sequence_id) VALUES (5, 'Way', 29, 'reviewee', 1, 2);
INSERT INTO current_relation_members (relation_id, member_type, member_id, member_role, sequence_id) VALUES (5, 'Way', 29, 'reviewee', 2);
UPDATE changesets SET min_lat=311790644, max_lat=311832952, min_lon=340472592, max_lon=340565418, num_changes=31 WHERE id=3;
