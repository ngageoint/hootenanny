INSERT INTO changesets (id, user_id, created_at, closed_at) VALUES (2, 1, (now() at time zone 'utc'), (now() at time zone 'utc'));
INSERT INTO changeset_tags (changeset_id, k, v) VALUES (2, 'written_by', 'Hootenanny');
/* create node 3 */
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3, 388541670, 1048997070, 2, true, (now() at time zone 'utc'), 3852309124, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3, 388541670, 1048997070, 2, true, (now() at time zone 'utc'), 3852309124, 1);
/* create node 4 */
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (4, 388540851, 1049014477, 2, true, (now() at time zone 'utc'), 3852309124, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (4, 388540851, 1049014477, 2, true, (now() at time zone 'utc'), 3852309124, 1);
/* create node 5 */
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (5, 388542471, 1049010789, 2, true, (now() at time zone 'utc'), 3852309124, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (5, 388542471, 1049010789, 2, true, (now() at time zone 'utc'), 3852309124, 1);
/* create node 6 */
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (6, 388542600, 1049005795, 2, true, (now() at time zone 'utc'), 3852309124, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (6, 388542600, 1049005795, 2, true, (now() at time zone 'utc'), 3852309124, 1);
/* create node 7 */
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7, 388549614, 1048997124, 2, true, (now() at time zone 'utc'), 3852309124, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7, 388549614, 1048997124, 2, true, (now() at time zone 'utc'), 3852309124, 1);
/* create node 8 */
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (8, 388545785, 1048997171, 2, true, (now() at time zone 'utc'), 3852309124, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (8, 388545785, 1048997171, 2, true, (now() at time zone 'utc'), 3852309124, 1);
/* create node 9 */
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (9, 388542618, 1048997171, 2, true, (now() at time zone 'utc'), 3852309124, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (9, 388542618, 1048997171, 2, true, (now() at time zone 'utc'), 3852309124, 1);
/* create node 10 */
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (10, 388539526, 1048996840, 2, true, (now() at time zone 'utc'), 3852309124, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (10, 388539526, 1048996840, 2, true, (now() at time zone 'utc'), 3852309124, 1);
/* create node 11 */
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11, 388536248, 1048996935, 2, true, (now() at time zone 'utc'), 3852309121, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11, 388536248, 1048996935, 2, true, (now() at time zone 'utc'), 3852309121, 1);
/* create node 12 */
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12, 388532583, 1048997543, 2, true, (now() at time zone 'utc'), 3852309121, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12, 388532583, 1048997543, 2, true, (now() at time zone 'utc'), 3852309121, 1);
/* create node 13 */
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13, 388532424, 1048996868, 2, true, (now() at time zone 'utc'), 3852309121, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13, 388532424, 1048996868, 2, true, (now() at time zone 'utc'), 3852309121, 1);
/* create node 14 */
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (14, 388533001, 1048994529, 2, true, (now() at time zone 'utc'), 3852309121, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (14, 388533001, 1048994529, 2, true, (now() at time zone 'utc'), 3852309121, 1);
/* create node 15 */
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (15, 388533470, 1048993672, 2, true, (now() at time zone 'utc'), 3852309121, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (15, 388533470, 1048993672, 2, true, (now() at time zone 'utc'), 3852309121, 1);
/* create node 16 */
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (16, 388535166, 1048992699, 2, true, (now() at time zone 'utc'), 3852309121, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (16, 388535166, 1048992699, 2, true, (now() at time zone 'utc'), 3852309121, 1);
/* create node 17 */
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (17, 388535689, 1048990568, 2, true, (now() at time zone 'utc'), 3852309035, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (17, 388535689, 1048990568, 2, true, (now() at time zone 'utc'), 3852309035, 1);
/* create node 18 */
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (18, 388535509, 1048988321, 2, true, (now() at time zone 'utc'), 3852309035, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (18, 388535509, 1048988321, 2, true, (now() at time zone 'utc'), 3852309035, 1);
/* create node 19 */
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (19, 388536194, 1048987070, 2, true, (now() at time zone 'utc'), 3852309035, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (19, 388536194, 1048987070, 2, true, (now() at time zone 'utc'), 3852309035, 1);
/* create node 20 */
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (20, 388537204, 1048987233, 2, true, (now() at time zone 'utc'), 3852309035, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (20, 388537204, 1048987233, 2, true, (now() at time zone 'utc'), 3852309035, 1);
/* create node 21 */
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (21, 388539585, 1048989155, 2, true, (now() at time zone 'utc'), 3852309038, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (21, 388539585, 1048989155, 2, true, (now() at time zone 'utc'), 3852309038, 1);
/* create node 22 */
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (22, 388541335, 1048988900, 2, true, (now() at time zone 'utc'), 3852309038, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (22, 388541335, 1048988900, 2, true, (now() at time zone 'utc'), 3852309038, 1);
/* create node 23 */
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (23, 388541768, 1048987070, 2, true, (now() at time zone 'utc'), 3852309038, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (23, 388541768, 1048987070, 2, true, (now() at time zone 'utc'), 3852309038, 1);
/* create node 24 */
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (24, 388542327, 1048983110, 2, true, (now() at time zone 'utc'), 3852309038, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (24, 388542327, 1048983110, 2, true, (now() at time zone 'utc'), 3852309038, 1);
/* create node 25 */
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (25, 388544293, 1048980654, 2, true, (now() at time zone 'utc'), 3852309038, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (25, 388544293, 1048980654, 2, true, (now() at time zone 'utc'), 3852309038, 1);
/* create node 26 */
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (26, 388546043, 1048977759, 2, true, (now() at time zone 'utc'), 3852309038, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (26, 388546043, 1048977759, 2, true, (now() at time zone 'utc'), 3852309038, 1);
/* create node 27 */
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (27, 388547197, 1048973219, 2, true, (now() at time zone 'utc'), 3852309038, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (27, 388547197, 1048973219, 2, true, (now() at time zone 'utc'), 3852309038, 1);
/* create node 28 */
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (28, 388548604, 1048968587, 2, true, (now() at time zone 'utc'), 3852309038, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (28, 388548604, 1048968587, 2, true, (now() at time zone 'utc'), 3852309038, 1);
/* create node 29 */
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (29, 388549019, 1048964394, 2, true, (now() at time zone 'utc'), 3852309038, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (29, 388549019, 1048964394, 2, true, (now() at time zone 'utc'), 3852309038, 1);
/* create node 30 */
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (30, 388549073, 1048961823, 2, true, (now() at time zone 'utc'), 3852309038, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (30, 388549073, 1048961823, 2, true, (now() at time zone 'utc'), 3852309038, 1);
/* create node 31 */
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (31, 388548514, 1049005693, 2, true, (now() at time zone 'utc'), 3852309124, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (31, 388548514, 1049005693, 2, true, (now() at time zone 'utc'), 3852309124, 1);
/* create node 32 */
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (32, 388549290, 1049005253, 2, true, (now() at time zone 'utc'), 3852309124, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (32, 388549290, 1049005253, 2, true, (now() at time zone 'utc'), 3852309124, 1);
/* create node 33 */
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (33, 388548207, 1049008079, 2, true, (now() at time zone 'utc'), 3852309124, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (33, 388548207, 1049008079, 2, true, (now() at time zone 'utc'), 3852309124, 1);
/* create node 34 */
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (34, 388543319, 1049017877, 2, true, (now() at time zone 'utc'), 3852309124, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (34, 388543319, 1049017877, 2, true, (now() at time zone 'utc'), 3852309124, 1);
/* create node 35 */
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (35, 388541299, 1049023065, 2, true, (now() at time zone 'utc'), 3852309124, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (35, 388541299, 1049023065, 2, true, (now() at time zone 'utc'), 3852309124, 1);
/* create node 36 */
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (36, 388540541, 1049024316, 2, true, (now() at time zone 'utc'), 3852309124, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (36, 388540541, 1049024316, 2, true, (now() at time zone 'utc'), 3852309124, 1);
/* create way 2 */
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (2, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (2, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (2, 'highway', 'road');
INSERT INTO way_tags (way_id, k, v, version) VALUES (2, 'highway', 'road', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (2, 'note', '2');
INSERT INTO way_tags (way_id, k, v, version) VALUES (2, 'note', '2', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (2, 4, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (2, 4, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (2, 5, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (2, 5, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (2, 6, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (2, 6, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (2, 9, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (2, 9, 4);
/* create way 3 */
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (3, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (3, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (3, 'highway', 'road');
INSERT INTO way_tags (way_id, k, v, version) VALUES (3, 'highway', 'road', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (3, 'note', '0');
INSERT INTO way_tags (way_id, k, v, version) VALUES (3, 'note', '0', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (3, 36, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (3, 36, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (3, 35, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (3, 35, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (3, 34, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (3, 34, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (3, 33, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (3, 33, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (3, 32, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (3, 32, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (3, 7, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (3, 7, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (3, 8, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (3, 8, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (3, 9, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (3, 9, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (3, 3, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (3, 3, 9);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (3, 10, 1, 10);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (3, 10, 10);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (3, 11, 1, 11);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (3, 11, 11);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (3, 12, 1, 12);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (3, 12, 12);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (3, 13, 1, 13);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (3, 13, 13);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (3, 14, 1, 14);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (3, 14, 14);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (3, 15, 1, 15);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (3, 15, 15);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (3, 16, 1, 16);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (3, 16, 16);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (3, 17, 1, 17);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (3, 17, 17);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (3, 18, 1, 18);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (3, 18, 18);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (3, 19, 1, 19);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (3, 19, 19);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (3, 20, 1, 20);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (3, 20, 20);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (3, 21, 1, 21);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (3, 21, 21);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (3, 22, 1, 22);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (3, 22, 22);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (3, 23, 1, 23);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (3, 23, 23);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (3, 24, 1, 24);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (3, 24, 24);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (3, 25, 1, 25);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (3, 25, 25);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (3, 26, 1, 26);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (3, 26, 26);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (3, 27, 1, 27);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (3, 27, 27);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (3, 28, 1, 28);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (3, 28, 28);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (3, 29, 1, 29);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (3, 29, 29);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (3, 30, 1, 30);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (3, 30, 30);
/* create way 4 */
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (4, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (4, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (4, 'highway', 'road');
INSERT INTO way_tags (way_id, k, v, version) VALUES (4, 'highway', 'road', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (4, 'note', '3');
INSERT INTO way_tags (way_id, k, v, version) VALUES (4, 'note', '3', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (4, 6, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (4, 6, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (4, 31, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (4, 31, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (4, 32, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (4, 32, 3);
UPDATE changesets SET min_lat=388532424, max_lat=388549614, min_lon=1048961823, max_lon=1049024316, num_changes=37 WHERE id=2;
