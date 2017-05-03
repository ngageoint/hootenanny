INSERT INTO changesets (id, user_id, created_at, closed_at) VALUES (3, 1, (now() at time zone 'utc'), (now() at time zone 'utc'));
INSERT INTO changeset_tags (changeset_id, k, v) VALUES (3, 'written_by', 'Hootenanny');
/* modify node 111*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (111, 388943332, -770415784, 3, true, (now() at time zone 'utc'), 1704401706, 2);
UPDATE current_nodes SET latitude=388943332, longitude=-770415784, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401706, version=2 WHERE id=111;
DELETE FROM current_node_tags WHERE node_id = 111;
DELETE FROM node_tags WHERE node_id = 111;
INSERT INTO current_node_tags (node_id, k, v) VALUES (111, 'hoot:id', '111');
INSERT INTO node_tags (node_id, k, v, version) VALUES (111, 'hoot:id', '111', 2);
/* modify node 113*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (113, 388943334, -770409538, 3, true, (now() at time zone 'utc'), 1704401706, 2);
UPDATE current_nodes SET latitude=388943334, longitude=-770409538, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401706, version=2 WHERE id=113;
DELETE FROM current_node_tags WHERE node_id = 113;
DELETE FROM node_tags WHERE node_id = 113;
INSERT INTO current_node_tags (node_id, k, v) VALUES (113, 'hoot:id', '113');
INSERT INTO node_tags (node_id, k, v, version) VALUES (113, 'hoot:id', '113', 2);
/* modify node 115*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (115, 388943362, -770405298, 3, true, (now() at time zone 'utc'), 1704401706, 2);
UPDATE current_nodes SET latitude=388943362, longitude=-770405298, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401706, version=2 WHERE id=115;
DELETE FROM current_node_tags WHERE node_id = 115;
DELETE FROM node_tags WHERE node_id = 115;
INSERT INTO current_node_tags (node_id, k, v) VALUES (115, 'hoot:id', '115');
INSERT INTO node_tags (node_id, k, v, version) VALUES (115, 'hoot:id', '115', 2);
/* modify node 117*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (117, 388943355, -770400893, 3, true, (now() at time zone 'utc'), 1704401706, 2);
UPDATE current_nodes SET latitude=388943355, longitude=-770400893, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401706, version=2 WHERE id=117;
DELETE FROM current_node_tags WHERE node_id = 117;
DELETE FROM node_tags WHERE node_id = 117;
INSERT INTO current_node_tags (node_id, k, v) VALUES (117, 'hoot:id', '117');
INSERT INTO node_tags (node_id, k, v, version) VALUES (117, 'hoot:id', '117', 2);
/* modify node 119*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (119, 388943360, -770396112, 3, true, (now() at time zone 'utc'), 1704401706, 2);
UPDATE current_nodes SET latitude=388943360, longitude=-770396112, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401706, version=2 WHERE id=119;
DELETE FROM current_node_tags WHERE node_id = 119;
DELETE FROM node_tags WHERE node_id = 119;
INSERT INTO current_node_tags (node_id, k, v) VALUES (119, 'hoot:id', '119');
INSERT INTO node_tags (node_id, k, v, version) VALUES (119, 'hoot:id', '119', 2);
/* modify node 121*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (121, 388943364, -770395947, 3, true, (now() at time zone 'utc'), 1704401706, 2);
UPDATE current_nodes SET latitude=388943364, longitude=-770395947, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401706, version=2 WHERE id=121;
DELETE FROM current_node_tags WHERE node_id = 121;
DELETE FROM node_tags WHERE node_id = 121;
INSERT INTO current_node_tags (node_id, k, v) VALUES (121, 'hoot:id', '121');
INSERT INTO node_tags (node_id, k, v, version) VALUES (121, 'hoot:id', '121', 2);
/* modify node 209*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (209, 388964025, -770336491, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388964025, longitude=-770336491, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=209;
DELETE FROM current_node_tags WHERE node_id = 209;
DELETE FROM node_tags WHERE node_id = 209;
INSERT INTO current_node_tags (node_id, k, v) VALUES (209, 'hoot:id', '209');
INSERT INTO node_tags (node_id, k, v, version) VALUES (209, 'hoot:id', '209', 2);
/* modify node 210*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (210, 388962715, -770336506, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388962715, longitude=-770336506, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=210;
DELETE FROM current_node_tags WHERE node_id = 210;
DELETE FROM node_tags WHERE node_id = 210;
INSERT INTO current_node_tags (node_id, k, v) VALUES (210, 'hoot:id', '210');
INSERT INTO node_tags (node_id, k, v, version) VALUES (210, 'hoot:id', '210', 2);
/* modify node 211*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (211, 388956977, -770336458, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388956977, longitude=-770336458, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=211;
DELETE FROM current_node_tags WHERE node_id = 211;
DELETE FROM node_tags WHERE node_id = 211;
INSERT INTO current_node_tags (node_id, k, v) VALUES (211, 'hoot:id', '211');
INSERT INTO node_tags (node_id, k, v, version) VALUES (211, 'hoot:id', '211', 2);
/* modify node 213*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (213, 388956578, -770336454, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388956578, longitude=-770336454, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=213;
DELETE FROM current_node_tags WHERE node_id = 213;
DELETE FROM node_tags WHERE node_id = 213;
INSERT INTO current_node_tags (node_id, k, v) VALUES (213, 'hoot:id', '213');
INSERT INTO node_tags (node_id, k, v, version) VALUES (213, 'hoot:id', '213', 2);
/* modify node 216*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (216, 388919782, -770336602, 3, true, (now() at time zone 'utc'), 1704401621, 2);
UPDATE current_nodes SET latitude=388919782, longitude=-770336602, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401621, version=2 WHERE id=216;
DELETE FROM current_node_tags WHERE node_id = 216;
DELETE FROM node_tags WHERE node_id = 216;
INSERT INTO current_node_tags (node_id, k, v) VALUES (216, 'hoot:id', '216');
INSERT INTO node_tags (node_id, k, v, version) VALUES (216, 'hoot:id', '216', 2);
/* modify node 219*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (219, 388918888, -770336611, 3, true, (now() at time zone 'utc'), 1704401621, 2);
UPDATE current_nodes SET latitude=388918888, longitude=-770336611, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401621, version=2 WHERE id=219;
DELETE FROM current_node_tags WHERE node_id = 219;
DELETE FROM node_tags WHERE node_id = 219;
INSERT INTO current_node_tags (node_id, k, v) VALUES (219, 'hoot:id', '219');
INSERT INTO node_tags (node_id, k, v, version) VALUES (219, 'hoot:id', '219', 2);
/* modify node 222*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (222, 388917146, -770336629, 3, true, (now() at time zone 'utc'), 1704401621, 2);
UPDATE current_nodes SET latitude=388917146, longitude=-770336629, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401621, version=2 WHERE id=222;
DELETE FROM current_node_tags WHERE node_id = 222;
DELETE FROM node_tags WHERE node_id = 222;
INSERT INTO current_node_tags (node_id, k, v) VALUES (222, 'hoot:id', '222');
INSERT INTO node_tags (node_id, k, v, version) VALUES (222, 'hoot:id', '222', 2);
/* modify node 225*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (225, 388915811, -770336632, 3, true, (now() at time zone 'utc'), 1704401621, 2);
UPDATE current_nodes SET latitude=388915811, longitude=-770336632, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401621, version=2 WHERE id=225;
DELETE FROM current_node_tags WHERE node_id = 225;
DELETE FROM node_tags WHERE node_id = 225;
INSERT INTO current_node_tags (node_id, k, v) VALUES (225, 'hoot:id', '225');
INSERT INTO node_tags (node_id, k, v, version) VALUES (225, 'hoot:id', '225', 2);
/* modify node 228*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (228, 388914173, -770336650, 3, true, (now() at time zone 'utc'), 1704401621, 2);
UPDATE current_nodes SET latitude=388914173, longitude=-770336650, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401621, version=2 WHERE id=228;
DELETE FROM current_node_tags WHERE node_id = 228;
DELETE FROM node_tags WHERE node_id = 228;
INSERT INTO current_node_tags (node_id, k, v) VALUES (228, 'hoot:id', '228');
INSERT INTO node_tags (node_id, k, v, version) VALUES (228, 'hoot:id', '228', 2);
/* modify node 231*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (231, 388913476, -770336656, 3, true, (now() at time zone 'utc'), 1704401621, 2);
UPDATE current_nodes SET latitude=388913476, longitude=-770336656, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401621, version=2 WHERE id=231;
DELETE FROM current_node_tags WHERE node_id = 231;
DELETE FROM node_tags WHERE node_id = 231;
INSERT INTO current_node_tags (node_id, k, v) VALUES (231, 'hoot:id', '231');
INSERT INTO node_tags (node_id, k, v, version) VALUES (231, 'hoot:id', '231', 2);
/* modify node 234*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (234, 388913229, -770336650, 3, true, (now() at time zone 'utc'), 1704401621, 2);
UPDATE current_nodes SET latitude=388913229, longitude=-770336650, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401621, version=2 WHERE id=234;
DELETE FROM current_node_tags WHERE node_id = 234;
DELETE FROM node_tags WHERE node_id = 234;
INSERT INTO current_node_tags (node_id, k, v) VALUES (234, 'hoot:id', '234');
INSERT INTO node_tags (node_id, k, v, version) VALUES (234, 'hoot:id', '234', 2);
/* modify node 237*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (237, 388913033, -770336638, 3, true, (now() at time zone 'utc'), 1704401621, 2);
UPDATE current_nodes SET latitude=388913033, longitude=-770336638, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401621, version=2 WHERE id=237;
DELETE FROM current_node_tags WHERE node_id = 237;
DELETE FROM node_tags WHERE node_id = 237;
INSERT INTO current_node_tags (node_id, k, v) VALUES (237, 'hoot:id', '237');
INSERT INTO node_tags (node_id, k, v, version) VALUES (237, 'hoot:id', '237', 2);
/* modify node 240*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (240, 388912657, -770336591, 3, true, (now() at time zone 'utc'), 1704401621, 2);
UPDATE current_nodes SET latitude=388912657, longitude=-770336591, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401621, version=2 WHERE id=240;
DELETE FROM current_node_tags WHERE node_id = 240;
DELETE FROM node_tags WHERE node_id = 240;
INSERT INTO current_node_tags (node_id, k, v) VALUES (240, 'hoot:id', '240');
INSERT INTO node_tags (node_id, k, v, version) VALUES (240, 'hoot:id', '240', 2);
/* modify node 243*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (243, 388912238, -770336528, 3, true, (now() at time zone 'utc'), 1704401621, 2);
UPDATE current_nodes SET latitude=388912238, longitude=-770336528, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401621, version=2 WHERE id=243;
DELETE FROM current_node_tags WHERE node_id = 243;
DELETE FROM node_tags WHERE node_id = 243;
INSERT INTO current_node_tags (node_id, k, v) VALUES (243, 'hoot:id', '243');
INSERT INTO node_tags (node_id, k, v, version) VALUES (243, 'hoot:id', '243', 2);
/* modify node 246*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (246, 388911836, -770336445, 3, true, (now() at time zone 'utc'), 1704401621, 2);
UPDATE current_nodes SET latitude=388911836, longitude=-770336445, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401621, version=2 WHERE id=246;
DELETE FROM current_node_tags WHERE node_id = 246;
DELETE FROM node_tags WHERE node_id = 246;
INSERT INTO current_node_tags (node_id, k, v) VALUES (246, 'hoot:id', '246');
INSERT INTO node_tags (node_id, k, v, version) VALUES (246, 'hoot:id', '246', 2);
/* modify node 249*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (249, 388911451, -770336357, 3, true, (now() at time zone 'utc'), 1704401621, 2);
UPDATE current_nodes SET latitude=388911451, longitude=-770336357, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401621, version=2 WHERE id=249;
DELETE FROM current_node_tags WHERE node_id = 249;
DELETE FROM node_tags WHERE node_id = 249;
INSERT INTO current_node_tags (node_id, k, v) VALUES (249, 'hoot:id', '249');
INSERT INTO node_tags (node_id, k, v, version) VALUES (249, 'hoot:id', '249', 2);
/* modify node 252*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (252, 388910963, -770336251, 3, true, (now() at time zone 'utc'), 1704401621, 2);
UPDATE current_nodes SET latitude=388910963, longitude=-770336251, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401621, version=2 WHERE id=252;
DELETE FROM current_node_tags WHERE node_id = 252;
DELETE FROM node_tags WHERE node_id = 252;
INSERT INTO current_node_tags (node_id, k, v) VALUES (252, 'hoot:id', '252');
INSERT INTO node_tags (node_id, k, v, version) VALUES (252, 'hoot:id', '252', 2);
/* modify node 254*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (254, 388910427, -770336080, 3, true, (now() at time zone 'utc'), 1704401621, 2);
UPDATE current_nodes SET latitude=388910427, longitude=-770336080, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401621, version=2 WHERE id=254;
DELETE FROM current_node_tags WHERE node_id = 254;
DELETE FROM node_tags WHERE node_id = 254;
INSERT INTO current_node_tags (node_id, k, v) VALUES (254, 'hoot:id', '254');
INSERT INTO node_tags (node_id, k, v, version) VALUES (254, 'hoot:id', '254', 2);
/* modify node 256*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (256, 388909771, -770335866, 3, true, (now() at time zone 'utc'), 1704401621, 2);
UPDATE current_nodes SET latitude=388909771, longitude=-770335866, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401621, version=2 WHERE id=256;
DELETE FROM current_node_tags WHERE node_id = 256;
DELETE FROM node_tags WHERE node_id = 256;
INSERT INTO current_node_tags (node_id, k, v) VALUES (256, 'hoot:id', '256');
INSERT INTO node_tags (node_id, k, v, version) VALUES (256, 'hoot:id', '256', 2);
/* modify node 258*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (258, 388909456, -770335744, 3, true, (now() at time zone 'utc'), 1704401621, 2);
UPDATE current_nodes SET latitude=388909456, longitude=-770335744, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401621, version=2 WHERE id=258;
DELETE FROM current_node_tags WHERE node_id = 258;
DELETE FROM node_tags WHERE node_id = 258;
INSERT INTO current_node_tags (node_id, k, v) VALUES (258, 'hoot:id', '258');
INSERT INTO node_tags (node_id, k, v, version) VALUES (258, 'hoot:id', '258', 2);
/* modify node 260*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (260, 388908854, -770335435, 3, true, (now() at time zone 'utc'), 1704401621, 2);
UPDATE current_nodes SET latitude=388908854, longitude=-770335435, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401621, version=2 WHERE id=260;
DELETE FROM current_node_tags WHERE node_id = 260;
DELETE FROM node_tags WHERE node_id = 260;
INSERT INTO current_node_tags (node_id, k, v) VALUES (260, 'hoot:id', '260');
INSERT INTO node_tags (node_id, k, v, version) VALUES (260, 'hoot:id', '260', 2);
/* modify node 261*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (261, 388908307, -770335151, 3, true, (now() at time zone 'utc'), 1704401621, 2);
UPDATE current_nodes SET latitude=388908307, longitude=-770335151, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401621, version=2 WHERE id=261;
DELETE FROM current_node_tags WHERE node_id = 261;
DELETE FROM node_tags WHERE node_id = 261;
INSERT INTO current_node_tags (node_id, k, v) VALUES (261, 'hoot:id', '261');
INSERT INTO node_tags (node_id, k, v, version) VALUES (261, 'hoot:id', '261', 2);
/* modify node 262*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (262, 388907970, -770334939, 3, true, (now() at time zone 'utc'), 1704401621, 2);
UPDATE current_nodes SET latitude=388907970, longitude=-770334939, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401621, version=2 WHERE id=262;
DELETE FROM current_node_tags WHERE node_id = 262;
DELETE FROM node_tags WHERE node_id = 262;
INSERT INTO current_node_tags (node_id, k, v) VALUES (262, 'hoot:id', '262');
INSERT INTO node_tags (node_id, k, v, version) VALUES (262, 'hoot:id', '262', 2);
/* modify node 263*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (263, 388907749, -770334799, 3, true, (now() at time zone 'utc'), 1704401621, 2);
UPDATE current_nodes SET latitude=388907749, longitude=-770334799, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401621, version=2 WHERE id=263;
DELETE FROM current_node_tags WHERE node_id = 263;
DELETE FROM node_tags WHERE node_id = 263;
INSERT INTO current_node_tags (node_id, k, v) VALUES (263, 'hoot:id', '263');
INSERT INTO node_tags (node_id, k, v, version) VALUES (263, 'hoot:id', '263', 2);
/* modify node 264*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (264, 388907142, -770334406, 3, true, (now() at time zone 'utc'), 1704401621, 2);
UPDATE current_nodes SET latitude=388907142, longitude=-770334406, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401621, version=2 WHERE id=264;
DELETE FROM current_node_tags WHERE node_id = 264;
DELETE FROM node_tags WHERE node_id = 264;
INSERT INTO current_node_tags (node_id, k, v) VALUES (264, 'hoot:id', '264');
INSERT INTO node_tags (node_id, k, v, version) VALUES (264, 'hoot:id', '264', 2);
/* modify node 265*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (265, 388906566, -770334017, 3, true, (now() at time zone 'utc'), 1704401621, 2);
UPDATE current_nodes SET latitude=388906566, longitude=-770334017, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401621, version=2 WHERE id=265;
DELETE FROM current_node_tags WHERE node_id = 265;
DELETE FROM node_tags WHERE node_id = 265;
INSERT INTO current_node_tags (node_id, k, v) VALUES (265, 'hoot:id', '265');
INSERT INTO node_tags (node_id, k, v, version) VALUES (265, 'hoot:id', '265', 2);
/* modify node 266*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (266, 388905804, -770333388, 3, true, (now() at time zone 'utc'), 1704401621, 2);
UPDATE current_nodes SET latitude=388905804, longitude=-770333388, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401621, version=2 WHERE id=266;
DELETE FROM current_node_tags WHERE node_id = 266;
DELETE FROM node_tags WHERE node_id = 266;
INSERT INTO current_node_tags (node_id, k, v) VALUES (266, 'hoot:id', '266');
INSERT INTO node_tags (node_id, k, v, version) VALUES (266, 'hoot:id', '266', 2);
/* modify node 267*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (267, 388905011, -770332760, 3, true, (now() at time zone 'utc'), 1704401621, 2);
UPDATE current_nodes SET latitude=388905011, longitude=-770332760, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401621, version=2 WHERE id=267;
DELETE FROM current_node_tags WHERE node_id = 267;
DELETE FROM node_tags WHERE node_id = 267;
INSERT INTO current_node_tags (node_id, k, v) VALUES (267, 'hoot:id', '267');
INSERT INTO node_tags (node_id, k, v, version) VALUES (267, 'hoot:id', '267', 2);
/* modify node 268*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (268, 388904356, -770332228, 3, true, (now() at time zone 'utc'), 1704401621, 2);
UPDATE current_nodes SET latitude=388904356, longitude=-770332228, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401621, version=2 WHERE id=268;
DELETE FROM current_node_tags WHERE node_id = 268;
DELETE FROM node_tags WHERE node_id = 268;
INSERT INTO current_node_tags (node_id, k, v) VALUES (268, 'hoot:id', '268');
INSERT INTO node_tags (node_id, k, v, version) VALUES (268, 'hoot:id', '268', 2);
/* modify node 269*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (269, 388903641, -770331693, 3, true, (now() at time zone 'utc'), 1704401621, 2);
UPDATE current_nodes SET latitude=388903641, longitude=-770331693, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401621, version=2 WHERE id=269;
DELETE FROM current_node_tags WHERE node_id = 269;
DELETE FROM node_tags WHERE node_id = 269;
INSERT INTO current_node_tags (node_id, k, v) VALUES (269, 'hoot:id', '269');
INSERT INTO node_tags (node_id, k, v, version) VALUES (269, 'hoot:id', '269', 2);
/* modify node 270*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (270, 388902918, -770331188, 3, true, (now() at time zone 'utc'), 1704401621, 2);
UPDATE current_nodes SET latitude=388902918, longitude=-770331188, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401621, version=2 WHERE id=270;
DELETE FROM current_node_tags WHERE node_id = 270;
DELETE FROM node_tags WHERE node_id = 270;
INSERT INTO current_node_tags (node_id, k, v) VALUES (270, 'hoot:id', '270');
INSERT INTO node_tags (node_id, k, v, version) VALUES (270, 'hoot:id', '270', 2);
/* modify node 271*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (271, 388902507, -770330934, 3, true, (now() at time zone 'utc'), 1704401621, 2);
UPDATE current_nodes SET latitude=388902507, longitude=-770330934, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401621, version=2 WHERE id=271;
DELETE FROM current_node_tags WHERE node_id = 271;
DELETE FROM node_tags WHERE node_id = 271;
INSERT INTO current_node_tags (node_id, k, v) VALUES (271, 'hoot:id', '271');
INSERT INTO node_tags (node_id, k, v, version) VALUES (271, 'hoot:id', '271', 2);
/* modify node 272*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (272, 388902237, -770330767, 3, true, (now() at time zone 'utc'), 1704401621, 2);
UPDATE current_nodes SET latitude=388902237, longitude=-770330767, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401621, version=2 WHERE id=272;
DELETE FROM current_node_tags WHERE node_id = 272;
DELETE FROM node_tags WHERE node_id = 272;
INSERT INTO current_node_tags (node_id, k, v) VALUES (272, 'hoot:id', '272');
INSERT INTO node_tags (node_id, k, v, version) VALUES (272, 'hoot:id', '272', 2);
/* modify node 273*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (273, 388901561, -770330426, 3, true, (now() at time zone 'utc'), 1704401621, 2);
UPDATE current_nodes SET latitude=388901561, longitude=-770330426, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401621, version=2 WHERE id=273;
DELETE FROM current_node_tags WHERE node_id = 273;
DELETE FROM node_tags WHERE node_id = 273;
INSERT INTO current_node_tags (node_id, k, v) VALUES (273, 'hoot:id', '273');
INSERT INTO node_tags (node_id, k, v, version) VALUES (273, 'hoot:id', '273', 2);
/* modify node 475*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (475, 388954051, -770417066, 3, true, (now() at time zone 'utc'), 1704401707, 2);
UPDATE current_nodes SET latitude=388954051, longitude=-770417066, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401707, version=2 WHERE id=475;
DELETE FROM current_node_tags WHERE node_id = 475;
DELETE FROM node_tags WHERE node_id = 475;
INSERT INTO current_node_tags (node_id, k, v) VALUES (475, 'hoot:id', '475');
INSERT INTO node_tags (node_id, k, v, version) VALUES (475, 'hoot:id', '475', 2);
/* modify node 477*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (477, 388954027, -770415736, 3, true, (now() at time zone 'utc'), 1704401707, 2);
UPDATE current_nodes SET latitude=388954027, longitude=-770415736, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401707, version=2 WHERE id=477;
DELETE FROM current_node_tags WHERE node_id = 477;
DELETE FROM node_tags WHERE node_id = 477;
INSERT INTO current_node_tags (node_id, k, v) VALUES (477, 'hoot:id', '477');
INSERT INTO node_tags (node_id, k, v, version) VALUES (477, 'hoot:id', '477', 2);
/* modify node 479*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (479, 388954006, -770414685, 3, true, (now() at time zone 'utc'), 1704401707, 2);
UPDATE current_nodes SET latitude=388954006, longitude=-770414685, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401707, version=2 WHERE id=479;
DELETE FROM current_node_tags WHERE node_id = 479;
DELETE FROM node_tags WHERE node_id = 479;
INSERT INTO current_node_tags (node_id, k, v) VALUES (479, 'hoot:id', '479');
INSERT INTO node_tags (node_id, k, v, version) VALUES (479, 'hoot:id', '479', 2);
/* modify node 481*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (481, 388953972, -770411176, 3, true, (now() at time zone 'utc'), 1704401707, 2);
UPDATE current_nodes SET latitude=388953972, longitude=-770411176, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401707, version=2 WHERE id=481;
DELETE FROM current_node_tags WHERE node_id = 481;
DELETE FROM node_tags WHERE node_id = 481;
INSERT INTO current_node_tags (node_id, k, v) VALUES (481, 'hoot:id', '481');
INSERT INTO node_tags (node_id, k, v, version) VALUES (481, 'hoot:id', '481', 2);
/* modify node 483*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (483, 388953986, -770407718, 3, true, (now() at time zone 'utc'), 1704401707, 2);
UPDATE current_nodes SET latitude=388953986, longitude=-770407718, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401707, version=2 WHERE id=483;
DELETE FROM current_node_tags WHERE node_id = 483;
DELETE FROM node_tags WHERE node_id = 483;
INSERT INTO current_node_tags (node_id, k, v) VALUES (483, 'hoot:id', '483');
INSERT INTO node_tags (node_id, k, v, version) VALUES (483, 'hoot:id', '483', 2);
/* modify node 485*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (485, 388953984, -770404389, 3, true, (now() at time zone 'utc'), 1704401707, 2);
UPDATE current_nodes SET latitude=388953984, longitude=-770404389, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401707, version=2 WHERE id=485;
DELETE FROM current_node_tags WHERE node_id = 485;
DELETE FROM node_tags WHERE node_id = 485;
INSERT INTO current_node_tags (node_id, k, v) VALUES (485, 'hoot:id', '485');
INSERT INTO node_tags (node_id, k, v, version) VALUES (485, 'hoot:id', '485', 2);
/* modify node 487*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (487, 388953982, -770401356, 3, true, (now() at time zone 'utc'), 1704401707, 2);
UPDATE current_nodes SET latitude=388953982, longitude=-770401356, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401707, version=2 WHERE id=487;
DELETE FROM current_node_tags WHERE node_id = 487;
DELETE FROM node_tags WHERE node_id = 487;
INSERT INTO current_node_tags (node_id, k, v) VALUES (487, 'hoot:id', '487');
INSERT INTO node_tags (node_id, k, v, version) VALUES (487, 'hoot:id', '487', 2);
/* modify node 489*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (489, 388953975, -770397559, 3, true, (now() at time zone 'utc'), 1704401707, 2);
UPDATE current_nodes SET latitude=388953975, longitude=-770397559, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401707, version=2 WHERE id=489;
DELETE FROM current_node_tags WHERE node_id = 489;
DELETE FROM node_tags WHERE node_id = 489;
INSERT INTO current_node_tags (node_id, k, v) VALUES (489, 'hoot:id', '489');
INSERT INTO node_tags (node_id, k, v, version) VALUES (489, 'hoot:id', '489', 2);
/* modify node 492*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (492, 388953972, -770396086, 3, true, (now() at time zone 'utc'), 1704401707, 2);
UPDATE current_nodes SET latitude=388953972, longitude=-770396086, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401707, version=2 WHERE id=492;
DELETE FROM current_node_tags WHERE node_id = 492;
DELETE FROM node_tags WHERE node_id = 492;
INSERT INTO current_node_tags (node_id, k, v) VALUES (492, 'hoot:id', '492');
INSERT INTO node_tags (node_id, k, v, version) VALUES (492, 'hoot:id', '492', 2);
/* modify node 494*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (494, 388952012, -770367641, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388952012, longitude=-770367641, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=494;
DELETE FROM current_node_tags WHERE node_id = 494;
DELETE FROM node_tags WHERE node_id = 494;
INSERT INTO current_node_tags (node_id, k, v) VALUES (494, 'hoot:id', '494');
INSERT INTO node_tags (node_id, k, v, version) VALUES (494, 'hoot:id', '494', 2);
/* modify node 496*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (496, 388952018, -770366655, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388952018, longitude=-770366655, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=496;
DELETE FROM current_node_tags WHERE node_id = 496;
DELETE FROM node_tags WHERE node_id = 496;
INSERT INTO current_node_tags (node_id, k, v) VALUES (496, 'hoot:id', '496');
INSERT INTO node_tags (node_id, k, v, version) VALUES (496, 'hoot:id', '496', 2);
/* modify node 499*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (499, 388952009, -770365382, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388952009, longitude=-770365382, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=499;
DELETE FROM current_node_tags WHERE node_id = 499;
DELETE FROM node_tags WHERE node_id = 499;
INSERT INTO current_node_tags (node_id, k, v) VALUES (499, 'hoot:id', '499');
INSERT INTO node_tags (node_id, k, v, version) VALUES (499, 'hoot:id', '499', 2);
/* modify node 502*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (502, 388951988, -770364392, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388951988, longitude=-770364392, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=502;
DELETE FROM current_node_tags WHERE node_id = 502;
DELETE FROM node_tags WHERE node_id = 502;
INSERT INTO current_node_tags (node_id, k, v) VALUES (502, 'hoot:id', '502');
INSERT INTO node_tags (node_id, k, v, version) VALUES (502, 'hoot:id', '502', 2);
/* modify node 505*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (505, 388951927, -770361298, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388951927, longitude=-770361298, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=505;
DELETE FROM current_node_tags WHERE node_id = 505;
DELETE FROM node_tags WHERE node_id = 505;
INSERT INTO current_node_tags (node_id, k, v) VALUES (505, 'hoot:id', '505');
INSERT INTO node_tags (node_id, k, v, version) VALUES (505, 'hoot:id', '505', 2);
/* modify node 508*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (508, 388951982, -770360983, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388951982, longitude=-770360983, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=508;
DELETE FROM current_node_tags WHERE node_id = 508;
DELETE FROM node_tags WHERE node_id = 508;
INSERT INTO current_node_tags (node_id, k, v) VALUES (508, 'hoot:id', '508');
INSERT INTO node_tags (node_id, k, v, version) VALUES (508, 'hoot:id', '508', 2);
/* modify node 511*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (511, 388952157, -770359977, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388952157, longitude=-770359977, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=511;
DELETE FROM current_node_tags WHERE node_id = 511;
DELETE FROM node_tags WHERE node_id = 511;
INSERT INTO current_node_tags (node_id, k, v) VALUES (511, 'hoot:id', '511');
INSERT INTO node_tags (node_id, k, v, version) VALUES (511, 'hoot:id', '511', 2);
/* modify node 513*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (513, 388952463, -770359076, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388952463, longitude=-770359076, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=513;
DELETE FROM current_node_tags WHERE node_id = 513;
DELETE FROM node_tags WHERE node_id = 513;
INSERT INTO current_node_tags (node_id, k, v) VALUES (513, 'hoot:id', '513');
INSERT INTO node_tags (node_id, k, v, version) VALUES (513, 'hoot:id', '513', 2);
/* modify node 515*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (515, 388952867, -770357782, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388952867, longitude=-770357782, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=515;
DELETE FROM current_node_tags WHERE node_id = 515;
DELETE FROM node_tags WHERE node_id = 515;
INSERT INTO current_node_tags (node_id, k, v) VALUES (515, 'hoot:id', '515');
INSERT INTO node_tags (node_id, k, v, version) VALUES (515, 'hoot:id', '515', 2);
/* modify node 517*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (517, 388952984, -770357407, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388952984, longitude=-770357407, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=517;
DELETE FROM current_node_tags WHERE node_id = 517;
DELETE FROM node_tags WHERE node_id = 517;
INSERT INTO current_node_tags (node_id, k, v) VALUES (517, 'hoot:id', '517');
INSERT INTO node_tags (node_id, k, v, version) VALUES (517, 'hoot:id', '517', 2);
/* modify node 519*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (519, 388953245, -770356525, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388953245, longitude=-770356525, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=519;
DELETE FROM current_node_tags WHERE node_id = 519;
DELETE FROM node_tags WHERE node_id = 519;
INSERT INTO current_node_tags (node_id, k, v) VALUES (519, 'hoot:id', '519');
INSERT INTO node_tags (node_id, k, v, version) VALUES (519, 'hoot:id', '519', 2);
/* modify node 522*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (522, 388953421, -770355533, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388953421, longitude=-770355533, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=522;
DELETE FROM current_node_tags WHERE node_id = 522;
DELETE FROM node_tags WHERE node_id = 522;
INSERT INTO current_node_tags (node_id, k, v) VALUES (522, 'hoot:id', '522');
INSERT INTO node_tags (node_id, k, v, version) VALUES (522, 'hoot:id', '522', 2);
/* modify node 525*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (525, 388953651, -770354237, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388953651, longitude=-770354237, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=525;
DELETE FROM current_node_tags WHERE node_id = 525;
DELETE FROM node_tags WHERE node_id = 525;
INSERT INTO current_node_tags (node_id, k, v) VALUES (525, 'hoot:id', '525');
INSERT INTO node_tags (node_id, k, v, version) VALUES (525, 'hoot:id', '525', 2);
/* modify node 529*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (529, 388953972, -770352169, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388953972, longitude=-770352169, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=529;
DELETE FROM current_node_tags WHERE node_id = 529;
DELETE FROM node_tags WHERE node_id = 529;
INSERT INTO current_node_tags (node_id, k, v) VALUES (529, 'hoot:id', '529');
INSERT INTO node_tags (node_id, k, v, version) VALUES (529, 'hoot:id', '529', 2);
/* modify node 533*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (533, 388954098, -770351619, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388954098, longitude=-770351619, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=533;
DELETE FROM current_node_tags WHERE node_id = 533;
DELETE FROM node_tags WHERE node_id = 533;
INSERT INTO current_node_tags (node_id, k, v) VALUES (533, 'hoot:id', '533');
INSERT INTO node_tags (node_id, k, v, version) VALUES (533, 'hoot:id', '533', 2);
/* modify node 598*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (598, 388953926, -770386796, 3, true, (now() at time zone 'utc'), 1704401707, 2);
UPDATE current_nodes SET latitude=388953926, longitude=-770386796, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401707, version=2 WHERE id=598;
DELETE FROM current_node_tags WHERE node_id = 598;
DELETE FROM node_tags WHERE node_id = 598;
INSERT INTO current_node_tags (node_id, k, v) VALUES (598, 'hoot:id', '598');
INSERT INTO node_tags (node_id, k, v, version) VALUES (598, 'hoot:id', '598', 2);
/* modify node 600*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (600, 388953918, -770384008, 3, true, (now() at time zone 'utc'), 1704401707, 2);
UPDATE current_nodes SET latitude=388953918, longitude=-770384008, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401707, version=2 WHERE id=600;
DELETE FROM current_node_tags WHERE node_id = 600;
DELETE FROM node_tags WHERE node_id = 600;
INSERT INTO current_node_tags (node_id, k, v) VALUES (600, 'hoot:id', '600');
INSERT INTO node_tags (node_id, k, v, version) VALUES (600, 'hoot:id', '600', 2);
/* modify node 602*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (602, 388953912, -770381514, 3, true, (now() at time zone 'utc'), 1704401707, 2);
UPDATE current_nodes SET latitude=388953912, longitude=-770381514, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401707, version=2 WHERE id=602;
DELETE FROM current_node_tags WHERE node_id = 602;
DELETE FROM node_tags WHERE node_id = 602;
INSERT INTO current_node_tags (node_id, k, v) VALUES (602, 'hoot:id', '602');
INSERT INTO node_tags (node_id, k, v, version) VALUES (602, 'hoot:id', '602', 2);
/* modify node 604*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (604, 388953951, -770379004, 3, true, (now() at time zone 'utc'), 1704401707, 2);
UPDATE current_nodes SET latitude=388953951, longitude=-770379004, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401707, version=2 WHERE id=604;
DELETE FROM current_node_tags WHERE node_id = 604;
DELETE FROM node_tags WHERE node_id = 604;
INSERT INTO current_node_tags (node_id, k, v) VALUES (604, 'hoot:id', '604');
INSERT INTO node_tags (node_id, k, v, version) VALUES (604, 'hoot:id', '604', 2);
/* modify node 606*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (606, 388953933, -770378067, 3, true, (now() at time zone 'utc'), 1704401707, 2);
UPDATE current_nodes SET latitude=388953933, longitude=-770378067, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401707, version=2 WHERE id=606;
DELETE FROM current_node_tags WHERE node_id = 606;
DELETE FROM node_tags WHERE node_id = 606;
INSERT INTO current_node_tags (node_id, k, v) VALUES (606, 'hoot:id', '606');
INSERT INTO node_tags (node_id, k, v, version) VALUES (606, 'hoot:id', '606', 2);
/* modify node 608*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (608, 388953914, -770377091, 3, true, (now() at time zone 'utc'), 1704401707, 2);
UPDATE current_nodes SET latitude=388953914, longitude=-770377091, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401707, version=2 WHERE id=608;
DELETE FROM current_node_tags WHERE node_id = 608;
DELETE FROM node_tags WHERE node_id = 608;
INSERT INTO current_node_tags (node_id, k, v) VALUES (608, 'hoot:id', '608');
INSERT INTO node_tags (node_id, k, v, version) VALUES (608, 'hoot:id', '608', 2);
/* modify node 610*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (610, 388953615, -770375295, 3, true, (now() at time zone 'utc'), 1704401707, 2);
UPDATE current_nodes SET latitude=388953615, longitude=-770375295, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401707, version=2 WHERE id=610;
DELETE FROM current_node_tags WHERE node_id = 610;
DELETE FROM node_tags WHERE node_id = 610;
INSERT INTO current_node_tags (node_id, k, v) VALUES (610, 'hoot:id', '610');
INSERT INTO node_tags (node_id, k, v, version) VALUES (610, 'hoot:id', '610', 2);
/* modify node 612*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (612, 388953302, -770373880, 3, true, (now() at time zone 'utc'), 1704401707, 2);
UPDATE current_nodes SET latitude=388953302, longitude=-770373880, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401707, version=2 WHERE id=612;
DELETE FROM current_node_tags WHERE node_id = 612;
DELETE FROM node_tags WHERE node_id = 612;
INSERT INTO current_node_tags (node_id, k, v) VALUES (612, 'hoot:id', '612');
INSERT INTO node_tags (node_id, k, v, version) VALUES (612, 'hoot:id', '612', 2);
/* modify node 614*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (614, 388953196, -770373482, 3, true, (now() at time zone 'utc'), 1704401707, 2);
UPDATE current_nodes SET latitude=388953196, longitude=-770373482, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401707, version=2 WHERE id=614;
DELETE FROM current_node_tags WHERE node_id = 614;
DELETE FROM node_tags WHERE node_id = 614;
INSERT INTO current_node_tags (node_id, k, v) VALUES (614, 'hoot:id', '614');
INSERT INTO node_tags (node_id, k, v, version) VALUES (614, 'hoot:id', '614', 2);
/* modify node 616*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (616, 388952805, -770372012, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388952805, longitude=-770372012, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=616;
DELETE FROM current_node_tags WHERE node_id = 616;
DELETE FROM node_tags WHERE node_id = 616;
INSERT INTO current_node_tags (node_id, k, v) VALUES (616, 'hoot:id', '616');
INSERT INTO node_tags (node_id, k, v, version) VALUES (616, 'hoot:id', '616', 2);
/* modify node 618*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (618, 388952384, -770370361, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388952384, longitude=-770370361, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=618;
DELETE FROM current_node_tags WHERE node_id = 618;
DELETE FROM node_tags WHERE node_id = 618;
INSERT INTO current_node_tags (node_id, k, v) VALUES (618, 'hoot:id', '618');
INSERT INTO node_tags (node_id, k, v, version) VALUES (618, 'hoot:id', '618', 2);
/* modify node 620*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (620, 388954549, -770349395, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388954549, longitude=-770349395, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=620;
DELETE FROM current_node_tags WHERE node_id = 620;
DELETE FROM node_tags WHERE node_id = 620;
INSERT INTO current_node_tags (node_id, k, v) VALUES (620, 'hoot:id', '620');
INSERT INTO node_tags (node_id, k, v, version) VALUES (620, 'hoot:id', '620', 2);
/* modify node 622*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (622, 388954604, -770348509, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388954604, longitude=-770348509, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=622;
DELETE FROM current_node_tags WHERE node_id = 622;
DELETE FROM node_tags WHERE node_id = 622;
INSERT INTO current_node_tags (node_id, k, v) VALUES (622, 'hoot:id', '622');
INSERT INTO node_tags (node_id, k, v, version) VALUES (622, 'hoot:id', '622', 2);
/* modify node 624*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (624, 388954625, -770348170, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388954625, longitude=-770348170, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=624;
DELETE FROM current_node_tags WHERE node_id = 624;
DELETE FROM node_tags WHERE node_id = 624;
INSERT INTO current_node_tags (node_id, k, v) VALUES (624, 'hoot:id', '624');
INSERT INTO node_tags (node_id, k, v, version) VALUES (624, 'hoot:id', '624', 2);
/* modify node 626*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (626, 388954656, -770345999, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388954656, longitude=-770345999, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=626;
DELETE FROM current_node_tags WHERE node_id = 626;
DELETE FROM node_tags WHERE node_id = 626;
INSERT INTO current_node_tags (node_id, k, v) VALUES (626, 'hoot:id', '626');
INSERT INTO node_tags (node_id, k, v, version) VALUES (626, 'hoot:id', '626', 2);
/* modify node 628*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (628, 388954703, -770344031, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388954703, longitude=-770344031, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=628;
DELETE FROM current_node_tags WHERE node_id = 628;
DELETE FROM node_tags WHERE node_id = 628;
INSERT INTO current_node_tags (node_id, k, v) VALUES (628, 'hoot:id', '628');
INSERT INTO node_tags (node_id, k, v, version) VALUES (628, 'hoot:id', '628', 2);
/* modify node 630*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (630, 388954751, -770342135, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388954751, longitude=-770342135, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=630;
DELETE FROM current_node_tags WHERE node_id = 630;
DELETE FROM node_tags WHERE node_id = 630;
INSERT INTO current_node_tags (node_id, k, v) VALUES (630, 'hoot:id', '630');
INSERT INTO node_tags (node_id, k, v, version) VALUES (630, 'hoot:id', '630', 2);
/* modify node 632*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (632, 388954742, -770340521, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388954742, longitude=-770340521, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=632;
DELETE FROM current_node_tags WHERE node_id = 632;
DELETE FROM node_tags WHERE node_id = 632;
INSERT INTO current_node_tags (node_id, k, v) VALUES (632, 'hoot:id', '632');
INSERT INTO node_tags (node_id, k, v, version) VALUES (632, 'hoot:id', '632', 2);
/* modify node 634*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (634, 388954738, -770339734, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388954738, longitude=-770339734, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=634;
DELETE FROM current_node_tags WHERE node_id = 634;
DELETE FROM node_tags WHERE node_id = 634;
INSERT INTO current_node_tags (node_id, k, v) VALUES (634, 'hoot:id', '634');
INSERT INTO node_tags (node_id, k, v, version) VALUES (634, 'hoot:id', '634', 2);
/* modify node 636*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (636, 388954753, -770338675, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388954753, longitude=-770338675, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=636;
DELETE FROM current_node_tags WHERE node_id = 636;
DELETE FROM node_tags WHERE node_id = 636;
INSERT INTO current_node_tags (node_id, k, v) VALUES (636, 'hoot:id', '636');
INSERT INTO node_tags (node_id, k, v, version) VALUES (636, 'hoot:id', '636', 2);
/* modify node 1180*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1180, 388935323, -770416401, 3, true, (now() at time zone 'utc'), 1704401706, 2);
UPDATE current_nodes SET latitude=388935323, longitude=-770416401, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401706, version=2 WHERE id=1180;
DELETE FROM current_node_tags WHERE node_id = 1180;
DELETE FROM node_tags WHERE node_id = 1180;
INSERT INTO current_node_tags (node_id, k, v) VALUES (1180, 'hoot:id', '1180');
INSERT INTO node_tags (node_id, k, v, version) VALUES (1180, 'hoot:id', '1180', 2);
/* modify node 1182*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1182, 388935327, -770416025, 3, true, (now() at time zone 'utc'), 1704401706, 2);
UPDATE current_nodes SET latitude=388935327, longitude=-770416025, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401706, version=2 WHERE id=1182;
DELETE FROM current_node_tags WHERE node_id = 1182;
DELETE FROM node_tags WHERE node_id = 1182;
INSERT INTO current_node_tags (node_id, k, v) VALUES (1182, 'hoot:id', '1182');
INSERT INTO node_tags (node_id, k, v, version) VALUES (1182, 'hoot:id', '1182', 2);
/* modify node 1184*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1184, 388935397, -770407639, 3, true, (now() at time zone 'utc'), 1704401706, 2);
UPDATE current_nodes SET latitude=388935397, longitude=-770407639, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401706, version=2 WHERE id=1184;
DELETE FROM current_node_tags WHERE node_id = 1184;
DELETE FROM node_tags WHERE node_id = 1184;
INSERT INTO current_node_tags (node_id, k, v) VALUES (1184, 'hoot:id', '1184');
INSERT INTO node_tags (node_id, k, v, version) VALUES (1184, 'hoot:id', '1184', 2);
/* modify node 1186*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1186, 388935325, -770396238, 3, true, (now() at time zone 'utc'), 1704401706, 2);
UPDATE current_nodes SET latitude=388935325, longitude=-770396238, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401706, version=2 WHERE id=1186;
DELETE FROM current_node_tags WHERE node_id = 1186;
DELETE FROM node_tags WHERE node_id = 1186;
INSERT INTO current_node_tags (node_id, k, v) VALUES (1186, 'hoot:id', '1186');
INSERT INTO node_tags (node_id, k, v, version) VALUES (1186, 'hoot:id', '1186', 2);
/* modify node 1188*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1188, 388935321, -770396015, 3, true, (now() at time zone 'utc'), 1704401706, 2);
UPDATE current_nodes SET latitude=388935321, longitude=-770396015, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401706, version=2 WHERE id=1188;
DELETE FROM current_node_tags WHERE node_id = 1188;
DELETE FROM node_tags WHERE node_id = 1188;
INSERT INTO current_node_tags (node_id, k, v) VALUES (1188, 'hoot:id', '1188');
INSERT INTO node_tags (node_id, k, v, version) VALUES (1188, 'hoot:id', '1188', 2);
/* modify node 1486*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1486, 388963905, -770379892, 3, true, (now() at time zone 'utc'), 1704401707, 2);
UPDATE current_nodes SET latitude=388963905, longitude=-770379892, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401707, version=2 WHERE id=1486;
DELETE FROM current_node_tags WHERE node_id = 1486;
DELETE FROM node_tags WHERE node_id = 1486;
INSERT INTO current_node_tags (node_id, k, v) VALUES (1486, 'hoot:id', '1486');
INSERT INTO node_tags (node_id, k, v, version) VALUES (1486, 'hoot:id', '1486', 2);
/* modify node 1487*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1487, 388962512, -770379778, 3, true, (now() at time zone 'utc'), 1704401707, 2);
UPDATE current_nodes SET latitude=388962512, longitude=-770379778, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401707, version=2 WHERE id=1487;
DELETE FROM current_node_tags WHERE node_id = 1487;
DELETE FROM node_tags WHERE node_id = 1487;
INSERT INTO current_node_tags (node_id, k, v) VALUES (1487, 'hoot:id', '1487');
INSERT INTO node_tags (node_id, k, v, version) VALUES (1487, 'hoot:id', '1487', 2);
/* modify node 1488*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1488, 388960889, -770379358, 3, true, (now() at time zone 'utc'), 1704401707, 2);
UPDATE current_nodes SET latitude=388960889, longitude=-770379358, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401707, version=2 WHERE id=1488;
DELETE FROM current_node_tags WHERE node_id = 1488;
DELETE FROM node_tags WHERE node_id = 1488;
INSERT INTO current_node_tags (node_id, k, v) VALUES (1488, 'hoot:id', '1488');
INSERT INTO node_tags (node_id, k, v, version) VALUES (1488, 'hoot:id', '1488', 2);
/* modify node 1489*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1489, 388959331, -770378804, 3, true, (now() at time zone 'utc'), 1704401707, 2);
UPDATE current_nodes SET latitude=388959331, longitude=-770378804, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401707, version=2 WHERE id=1489;
DELETE FROM current_node_tags WHERE node_id = 1489;
DELETE FROM node_tags WHERE node_id = 1489;
INSERT INTO current_node_tags (node_id, k, v) VALUES (1489, 'hoot:id', '1489');
INSERT INTO node_tags (node_id, k, v, version) VALUES (1489, 'hoot:id', '1489', 2);
/* modify node 1490*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1490, 388957577, -770377564, 3, true, (now() at time zone 'utc'), 1704401707, 2);
UPDATE current_nodes SET latitude=388957577, longitude=-770377564, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401707, version=2 WHERE id=1490;
DELETE FROM current_node_tags WHERE node_id = 1490;
DELETE FROM node_tags WHERE node_id = 1490;
INSERT INTO current_node_tags (node_id, k, v) VALUES (1490, 'hoot:id', '1490');
INSERT INTO node_tags (node_id, k, v, version) VALUES (1490, 'hoot:id', '1490', 2);
/* modify node 1491*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1491, 388957226, -770377181, 3, true, (now() at time zone 'utc'), 1704401707, 2);
UPDATE current_nodes SET latitude=388957226, longitude=-770377181, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401707, version=2 WHERE id=1491;
DELETE FROM current_node_tags WHERE node_id = 1491;
DELETE FROM node_tags WHERE node_id = 1491;
INSERT INTO current_node_tags (node_id, k, v) VALUES (1491, 'hoot:id', '1491');
INSERT INTO node_tags (node_id, k, v, version) VALUES (1491, 'hoot:id', '1491', 2);
/* modify node 1492*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1492, 388956303, -770376174, 3, true, (now() at time zone 'utc'), 1704401707, 2);
UPDATE current_nodes SET latitude=388956303, longitude=-770376174, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401707, version=2 WHERE id=1492;
DELETE FROM current_node_tags WHERE node_id = 1492;
DELETE FROM node_tags WHERE node_id = 1492;
INSERT INTO current_node_tags (node_id, k, v) VALUES (1492, 'hoot:id', '1492');
INSERT INTO node_tags (node_id, k, v, version) VALUES (1492, 'hoot:id', '1492', 2);
/* modify node 1493*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1493, 388956122, -770375976, 3, true, (now() at time zone 'utc'), 1704401707, 2);
UPDATE current_nodes SET latitude=388956122, longitude=-770375976, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401707, version=2 WHERE id=1493;
DELETE FROM current_node_tags WHERE node_id = 1493;
DELETE FROM node_tags WHERE node_id = 1493;
INSERT INTO current_node_tags (node_id, k, v) VALUES (1493, 'hoot:id', '1493');
INSERT INTO node_tags (node_id, k, v, version) VALUES (1493, 'hoot:id', '1493', 2);
/* modify node 1494*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1494, 388955533, -770375088, 3, true, (now() at time zone 'utc'), 1704401707, 2);
UPDATE current_nodes SET latitude=388955533, longitude=-770375088, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401707, version=2 WHERE id=1494;
DELETE FROM current_node_tags WHERE node_id = 1494;
DELETE FROM node_tags WHERE node_id = 1494;
INSERT INTO current_node_tags (node_id, k, v) VALUES (1494, 'hoot:id', '1494');
INSERT INTO node_tags (node_id, k, v, version) VALUES (1494, 'hoot:id', '1494', 2);
/* modify node 1495*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1495, 388955174, -770374546, 3, true, (now() at time zone 'utc'), 1704401707, 2);
UPDATE current_nodes SET latitude=388955174, longitude=-770374546, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401707, version=2 WHERE id=1495;
DELETE FROM current_node_tags WHERE node_id = 1495;
DELETE FROM node_tags WHERE node_id = 1495;
INSERT INTO current_node_tags (node_id, k, v) VALUES (1495, 'hoot:id', '1495');
INSERT INTO node_tags (node_id, k, v, version) VALUES (1495, 'hoot:id', '1495', 2);
/* modify node 1496*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1496, 388955049, -770374344, 3, true, (now() at time zone 'utc'), 1704401707, 2);
UPDATE current_nodes SET latitude=388955049, longitude=-770374344, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401707, version=2 WHERE id=1496;
DELETE FROM current_node_tags WHERE node_id = 1496;
DELETE FROM node_tags WHERE node_id = 1496;
INSERT INTO current_node_tags (node_id, k, v) VALUES (1496, 'hoot:id', '1496');
INSERT INTO node_tags (node_id, k, v, version) VALUES (1496, 'hoot:id', '1496', 2);
/* modify node 1497*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1497, 388954875, -770374062, 3, true, (now() at time zone 'utc'), 1704401707, 2);
UPDATE current_nodes SET latitude=388954875, longitude=-770374062, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401707, version=2 WHERE id=1497;
DELETE FROM current_node_tags WHERE node_id = 1497;
DELETE FROM node_tags WHERE node_id = 1497;
INSERT INTO current_node_tags (node_id, k, v) VALUES (1497, 'hoot:id', '1497');
INSERT INTO node_tags (node_id, k, v, version) VALUES (1497, 'hoot:id', '1497', 2);
/* modify node 1498*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1498, 388954425, -770373333, 3, true, (now() at time zone 'utc'), 1704401707, 2);
UPDATE current_nodes SET latitude=388954425, longitude=-770373333, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401707, version=2 WHERE id=1498;
DELETE FROM current_node_tags WHERE node_id = 1498;
DELETE FROM node_tags WHERE node_id = 1498;
INSERT INTO current_node_tags (node_id, k, v) VALUES (1498, 'hoot:id', '1498');
INSERT INTO node_tags (node_id, k, v, version) VALUES (1498, 'hoot:id', '1498', 2);
/* modify node 1499*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1499, 388953957, -770372218, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388953957, longitude=-770372218, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=1499;
DELETE FROM current_node_tags WHERE node_id = 1499;
DELETE FROM node_tags WHERE node_id = 1499;
INSERT INTO current_node_tags (node_id, k, v) VALUES (1499, 'hoot:id', '1499');
INSERT INTO node_tags (node_id, k, v, version) VALUES (1499, 'hoot:id', '1499', 2);
/* modify node 1500*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1500, 388953796, -770371834, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388953796, longitude=-770371834, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=1500;
DELETE FROM current_node_tags WHERE node_id = 1500;
DELETE FROM node_tags WHERE node_id = 1500;
INSERT INTO current_node_tags (node_id, k, v) VALUES (1500, 'hoot:id', '1500');
INSERT INTO node_tags (node_id, k, v, version) VALUES (1500, 'hoot:id', '1500', 2);
/* modify node 1501*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1501, 388953285, -770370123, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388953285, longitude=-770370123, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=1501;
DELETE FROM current_node_tags WHERE node_id = 1501;
DELETE FROM node_tags WHERE node_id = 1501;
INSERT INTO current_node_tags (node_id, k, v) VALUES (1501, 'hoot:id', '1501');
INSERT INTO node_tags (node_id, k, v, version) VALUES (1501, 'hoot:id', '1501', 2);
/* modify node 1502*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1502, 388952970, -770368486, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388952970, longitude=-770368486, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=1502;
DELETE FROM current_node_tags WHERE node_id = 1502;
DELETE FROM node_tags WHERE node_id = 1502;
INSERT INTO current_node_tags (node_id, k, v) VALUES (1502, 'hoot:id', '1502');
INSERT INTO node_tags (node_id, k, v, version) VALUES (1502, 'hoot:id', '1502', 2);
/* modify node 1503*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1503, 388952745, -770367161, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388952745, longitude=-770367161, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=1503;
DELETE FROM current_node_tags WHERE node_id = 1503;
DELETE FROM node_tags WHERE node_id = 1503;
INSERT INTO current_node_tags (node_id, k, v) VALUES (1503, 'hoot:id', '1503');
INSERT INTO node_tags (node_id, k, v, version) VALUES (1503, 'hoot:id', '1503', 2);
/* modify node 1504*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1504, 388952714, -770365945, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388952714, longitude=-770365945, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=1504;
DELETE FROM current_node_tags WHERE node_id = 1504;
DELETE FROM node_tags WHERE node_id = 1504;
INSERT INTO current_node_tags (node_id, k, v) VALUES (1504, 'hoot:id', '1504');
INSERT INTO node_tags (node_id, k, v, version) VALUES (1504, 'hoot:id', '1504', 2);
/* modify node 1505*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1505, 388952691, -770364509, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388952691, longitude=-770364509, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=1505;
DELETE FROM current_node_tags WHERE node_id = 1505;
DELETE FROM node_tags WHERE node_id = 1505;
INSERT INTO current_node_tags (node_id, k, v) VALUES (1505, 'hoot:id', '1505');
INSERT INTO node_tags (node_id, k, v, version) VALUES (1505, 'hoot:id', '1505', 2);
/* modify node 1506*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1506, 388952844, -770362969, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388952844, longitude=-770362969, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=1506;
DELETE FROM current_node_tags WHERE node_id = 1506;
DELETE FROM node_tags WHERE node_id = 1506;
INSERT INTO current_node_tags (node_id, k, v) VALUES (1506, 'hoot:id', '1506');
INSERT INTO node_tags (node_id, k, v, version) VALUES (1506, 'hoot:id', '1506', 2);
/* modify node 1507*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1507, 388953149, -770360946, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388953149, longitude=-770360946, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=1507;
DELETE FROM current_node_tags WHERE node_id = 1507;
DELETE FROM node_tags WHERE node_id = 1507;
INSERT INTO current_node_tags (node_id, k, v) VALUES (1507, 'hoot:id', '1507');
INSERT INTO node_tags (node_id, k, v, version) VALUES (1507, 'hoot:id', '1507', 2);
/* modify node 1508*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1508, 388953210, -770360545, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388953210, longitude=-770360545, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=1508;
DELETE FROM current_node_tags WHERE node_id = 1508;
DELETE FROM node_tags WHERE node_id = 1508;
INSERT INTO current_node_tags (node_id, k, v) VALUES (1508, 'hoot:id', '1508');
INSERT INTO node_tags (node_id, k, v, version) VALUES (1508, 'hoot:id', '1508', 2);
/* modify node 1509*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1509, 388953606, -770358686, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388953606, longitude=-770358686, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=1509;
DELETE FROM current_node_tags WHERE node_id = 1509;
DELETE FROM node_tags WHERE node_id = 1509;
INSERT INTO current_node_tags (node_id, k, v) VALUES (1509, 'hoot:id', '1509');
INSERT INTO node_tags (node_id, k, v, version) VALUES (1509, 'hoot:id', '1509', 2);
/* modify node 1510*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1510, 388954471, -770356984, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388954471, longitude=-770356984, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=1510;
DELETE FROM current_node_tags WHERE node_id = 1510;
DELETE FROM node_tags WHERE node_id = 1510;
INSERT INTO current_node_tags (node_id, k, v) VALUES (1510, 'hoot:id', '1510');
INSERT INTO node_tags (node_id, k, v, version) VALUES (1510, 'hoot:id', '1510', 2);
/* modify node 1511*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1511, 388954901, -770356358, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388954901, longitude=-770356358, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=1511;
DELETE FROM current_node_tags WHERE node_id = 1511;
DELETE FROM node_tags WHERE node_id = 1511;
INSERT INTO current_node_tags (node_id, k, v) VALUES (1511, 'hoot:id', '1511');
INSERT INTO node_tags (node_id, k, v, version) VALUES (1511, 'hoot:id', '1511', 2);
/* modify node 1512*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1512, 388955424, -770355596, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388955424, longitude=-770355596, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=1512;
DELETE FROM current_node_tags WHERE node_id = 1512;
DELETE FROM node_tags WHERE node_id = 1512;
INSERT INTO current_node_tags (node_id, k, v) VALUES (1512, 'hoot:id', '1512');
INSERT INTO node_tags (node_id, k, v, version) VALUES (1512, 'hoot:id', '1512', 2);
/* modify node 1513*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1513, 388956734, -770354140, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388956734, longitude=-770354140, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=1513;
DELETE FROM current_node_tags WHERE node_id = 1513;
DELETE FROM node_tags WHERE node_id = 1513;
INSERT INTO current_node_tags (node_id, k, v) VALUES (1513, 'hoot:id', '1513');
INSERT INTO node_tags (node_id, k, v, version) VALUES (1513, 'hoot:id', '1513', 2);
/* modify node 1514*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1514, 388957695, -770353232, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388957695, longitude=-770353232, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=1514;
DELETE FROM current_node_tags WHERE node_id = 1514;
DELETE FROM node_tags WHERE node_id = 1514;
INSERT INTO current_node_tags (node_id, k, v) VALUES (1514, 'hoot:id', '1514');
INSERT INTO node_tags (node_id, k, v, version) VALUES (1514, 'hoot:id', '1514', 2);
/* modify node 1515*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1515, 388958177, -770352777, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388958177, longitude=-770352777, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=1515;
DELETE FROM current_node_tags WHERE node_id = 1515;
DELETE FROM node_tags WHERE node_id = 1515;
INSERT INTO current_node_tags (node_id, k, v) VALUES (1515, 'hoot:id', '1515');
INSERT INTO node_tags (node_id, k, v, version) VALUES (1515, 'hoot:id', '1515', 2);
/* modify node 1516*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1516, 388959537, -770351775, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388959537, longitude=-770351775, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=1516;
DELETE FROM current_node_tags WHERE node_id = 1516;
DELETE FROM node_tags WHERE node_id = 1516;
INSERT INTO current_node_tags (node_id, k, v) VALUES (1516, 'hoot:id', '1516');
INSERT INTO node_tags (node_id, k, v, version) VALUES (1516, 'hoot:id', '1516', 2);
/* modify node 1517*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1517, 388960268, -770351528, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388960268, longitude=-770351528, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=1517;
DELETE FROM current_node_tags WHERE node_id = 1517;
DELETE FROM node_tags WHERE node_id = 1517;
INSERT INTO current_node_tags (node_id, k, v) VALUES (1517, 'hoot:id', '1517');
INSERT INTO node_tags (node_id, k, v, version) VALUES (1517, 'hoot:id', '1517', 2);
/* modify node 1518*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1518, 388961053, -770351263, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388961053, longitude=-770351263, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=1518;
DELETE FROM current_node_tags WHERE node_id = 1518;
DELETE FROM node_tags WHERE node_id = 1518;
INSERT INTO current_node_tags (node_id, k, v) VALUES (1518, 'hoot:id', '1518');
INSERT INTO node_tags (node_id, k, v, version) VALUES (1518, 'hoot:id', '1518', 2);
/* modify node 1519*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1519, 388964058, -770350709, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388964058, longitude=-770350709, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=1519;
DELETE FROM current_node_tags WHERE node_id = 1519;
DELETE FROM node_tags WHERE node_id = 1519;
INSERT INTO current_node_tags (node_id, k, v) VALUES (1519, 'hoot:id', '1519');
INSERT INTO node_tags (node_id, k, v, version) VALUES (1519, 'hoot:id', '1519', 2);
/* modify node 1667*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1667, 388920864, -770334375, 3, true, (now() at time zone 'utc'), 1704401621, 2);
UPDATE current_nodes SET latitude=388920864, longitude=-770334375, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401621, version=2 WHERE id=1667;
DELETE FROM current_node_tags WHERE node_id = 1667;
DELETE FROM node_tags WHERE node_id = 1667;
INSERT INTO current_node_tags (node_id, k, v) VALUES (1667, 'hoot:id', '1667');
INSERT INTO node_tags (node_id, k, v, version) VALUES (1667, 'hoot:id', '1667', 2);
/* modify node 1669*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1669, 388920839, -770332699, 3, true, (now() at time zone 'utc'), 1704401621, 2);
UPDATE current_nodes SET latitude=388920839, longitude=-770332699, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401621, version=2 WHERE id=1669;
DELETE FROM current_node_tags WHERE node_id = 1669;
DELETE FROM node_tags WHERE node_id = 1669;
INSERT INTO current_node_tags (node_id, k, v) VALUES (1669, 'hoot:id', '1669');
INSERT INTO node_tags (node_id, k, v, version) VALUES (1669, 'hoot:id', '1669', 2);
/* modify node 1671*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1671, 388920831, -770329990, 3, true, (now() at time zone 'utc'), 1704401621, 2);
UPDATE current_nodes SET latitude=388920831, longitude=-770329990, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401621, version=2 WHERE id=1671;
DELETE FROM current_node_tags WHERE node_id = 1671;
DELETE FROM node_tags WHERE node_id = 1671;
INSERT INTO current_node_tags (node_id, k, v) VALUES (1671, 'hoot:id', '1671');
INSERT INTO node_tags (node_id, k, v, version) VALUES (1671, 'hoot:id', '1671', 2);
/* modify node 1673*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1673, 388920819, -770326862, 3, true, (now() at time zone 'utc'), 1704401621, 2);
UPDATE current_nodes SET latitude=388920819, longitude=-770326862, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401621, version=2 WHERE id=1673;
DELETE FROM current_node_tags WHERE node_id = 1673;
DELETE FROM node_tags WHERE node_id = 1673;
INSERT INTO current_node_tags (node_id, k, v) VALUES (1673, 'hoot:id', '1673');
INSERT INTO node_tags (node_id, k, v, version) VALUES (1673, 'hoot:id', '1673', 2);
/* modify node 1675*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1675, 388920907, -770323536, 3, true, (now() at time zone 'utc'), 1704401621, 2);
UPDATE current_nodes SET latitude=388920907, longitude=-770323536, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401621, version=2 WHERE id=1675;
DELETE FROM current_node_tags WHERE node_id = 1675;
DELETE FROM node_tags WHERE node_id = 1675;
INSERT INTO current_node_tags (node_id, k, v) VALUES (1675, 'hoot:id', '1675');
INSERT INTO node_tags (node_id, k, v, version) VALUES (1675, 'hoot:id', '1675', 2);
/* modify node 1676*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1676, 388920905, -770321587, 3, true, (now() at time zone 'utc'), 1704401621, 2);
UPDATE current_nodes SET latitude=388920905, longitude=-770321587, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401621, version=2 WHERE id=1676;
DELETE FROM current_node_tags WHERE node_id = 1676;
DELETE FROM node_tags WHERE node_id = 1676;
INSERT INTO current_node_tags (node_id, k, v) VALUES (1676, 'hoot:id', '1676');
INSERT INTO node_tags (node_id, k, v, version) VALUES (1676, 'hoot:id', '1676', 2);
/* modify node 1921*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1921, 388954791, -770336453, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388954791, longitude=-770336453, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=1921;
DELETE FROM current_node_tags WHERE node_id = 1921;
DELETE FROM node_tags WHERE node_id = 1921;
INSERT INTO current_node_tags (node_id, k, v) VALUES (1921, 'hoot:id', '1921');
INSERT INTO node_tags (node_id, k, v, version) VALUES (1921, 'hoot:id', '1921', 2);
/* modify node 1923*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1923, 388953235, -770336453, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388953235, longitude=-770336453, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=1923;
DELETE FROM current_node_tags WHERE node_id = 1923;
DELETE FROM node_tags WHERE node_id = 1923;
INSERT INTO current_node_tags (node_id, k, v) VALUES (1923, 'hoot:id', '1923');
INSERT INTO node_tags (node_id, k, v, version) VALUES (1923, 'hoot:id', '1923', 2);
/* modify node 1925*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1925, 388951429, -770336452, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388951429, longitude=-770336452, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=1925;
DELETE FROM current_node_tags WHERE node_id = 1925;
DELETE FROM node_tags WHERE node_id = 1925;
INSERT INTO current_node_tags (node_id, k, v) VALUES (1925, 'hoot:id', '1925');
INSERT INTO node_tags (node_id, k, v, version) VALUES (1925, 'hoot:id', '1925', 2);
/* modify node 1927*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1927, 388943145, -770336410, 3, true, (now() at time zone 'utc'), 1704401792, 2);
UPDATE current_nodes SET latitude=388943145, longitude=-770336410, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401792, version=2 WHERE id=1927;
DELETE FROM current_node_tags WHERE node_id = 1927;
DELETE FROM node_tags WHERE node_id = 1927;
INSERT INTO current_node_tags (node_id, k, v) VALUES (1927, 'hoot:id', '1927');
INSERT INTO node_tags (node_id, k, v, version) VALUES (1927, 'hoot:id', '1927', 2);
/* modify node 1929*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1929, 388938772, -770336478, 3, true, (now() at time zone 'utc'), 1704401792, 2);
UPDATE current_nodes SET latitude=388938772, longitude=-770336478, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401792, version=2 WHERE id=1929;
DELETE FROM current_node_tags WHERE node_id = 1929;
DELETE FROM node_tags WHERE node_id = 1929;
INSERT INTO current_node_tags (node_id, k, v) VALUES (1929, 'hoot:id', '1929');
INSERT INTO node_tags (node_id, k, v, version) VALUES (1929, 'hoot:id', '1929', 2);
/* modify node 1931*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1931, 388936415, -770336448, 3, true, (now() at time zone 'utc'), 1704401792, 2);
UPDATE current_nodes SET latitude=388936415, longitude=-770336448, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401792, version=2 WHERE id=1931;
DELETE FROM current_node_tags WHERE node_id = 1931;
DELETE FROM node_tags WHERE node_id = 1931;
INSERT INTO current_node_tags (node_id, k, v) VALUES (1931, 'hoot:id', '1931');
INSERT INTO node_tags (node_id, k, v, version) VALUES (1931, 'hoot:id', '1931', 2);
/* modify node 1933*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1933, 388931976, -770336349, 3, true, (now() at time zone 'utc'), 1704401792, 2);
UPDATE current_nodes SET latitude=388931976, longitude=-770336349, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401792, version=2 WHERE id=1933;
DELETE FROM current_node_tags WHERE node_id = 1933;
DELETE FROM node_tags WHERE node_id = 1933;
INSERT INTO current_node_tags (node_id, k, v) VALUES (1933, 'hoot:id', '1933');
INSERT INTO node_tags (node_id, k, v, version) VALUES (1933, 'hoot:id', '1933', 2);
/* modify node 1935*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1935, 388930190, -770336353, 3, true, (now() at time zone 'utc'), 1704401792, 2);
UPDATE current_nodes SET latitude=388930190, longitude=-770336353, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401792, version=2 WHERE id=1935;
DELETE FROM current_node_tags WHERE node_id = 1935;
DELETE FROM node_tags WHERE node_id = 1935;
INSERT INTO current_node_tags (node_id, k, v) VALUES (1935, 'hoot:id', '1935');
INSERT INTO node_tags (node_id, k, v, version) VALUES (1935, 'hoot:id', '1935', 2);
/* modify node 1938*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1938, 388928514, -770336353, 3, true, (now() at time zone 'utc'), 1704401792, 2);
UPDATE current_nodes SET latitude=388928514, longitude=-770336353, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401792, version=2 WHERE id=1938;
DELETE FROM current_node_tags WHERE node_id = 1938;
DELETE FROM node_tags WHERE node_id = 1938;
INSERT INTO current_node_tags (node_id, k, v) VALUES (1938, 'hoot:id', '1938');
INSERT INTO node_tags (node_id, k, v, version) VALUES (1938, 'hoot:id', '1938', 2);
/* modify node 1941*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1941, 388926633, -770336356, 3, true, (now() at time zone 'utc'), 1704401792, 2);
UPDATE current_nodes SET latitude=388926633, longitude=-770336356, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401792, version=2 WHERE id=1941;
DELETE FROM current_node_tags WHERE node_id = 1941;
DELETE FROM node_tags WHERE node_id = 1941;
INSERT INTO current_node_tags (node_id, k, v) VALUES (1941, 'hoot:id', '1941');
INSERT INTO node_tags (node_id, k, v, version) VALUES (1941, 'hoot:id', '1941', 2);
/* modify node 1944*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1944, 388924758, -770336360, 3, true, (now() at time zone 'utc'), 1704401792, 2);
UPDATE current_nodes SET latitude=388924758, longitude=-770336360, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401792, version=2 WHERE id=1944;
DELETE FROM current_node_tags WHERE node_id = 1944;
DELETE FROM node_tags WHERE node_id = 1944;
INSERT INTO current_node_tags (node_id, k, v) VALUES (1944, 'hoot:id', '1944');
INSERT INTO node_tags (node_id, k, v, version) VALUES (1944, 'hoot:id', '1944', 2);
/* modify node 1946*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1946, 388922868, -770336400, 3, true, (now() at time zone 'utc'), 1704401792, 2);
UPDATE current_nodes SET latitude=388922868, longitude=-770336400, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401792, version=2 WHERE id=1946;
DELETE FROM current_node_tags WHERE node_id = 1946;
DELETE FROM node_tags WHERE node_id = 1946;
INSERT INTO current_node_tags (node_id, k, v) VALUES (1946, 'hoot:id', '1946');
INSERT INTO node_tags (node_id, k, v, version) VALUES (1946, 'hoot:id', '1946', 2);
/* modify node 1948*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1948, 388922717, -770336410, 3, true, (now() at time zone 'utc'), 1704401792, 2);
UPDATE current_nodes SET latitude=388922717, longitude=-770336410, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401792, version=2 WHERE id=1948;
DELETE FROM current_node_tags WHERE node_id = 1948;
DELETE FROM node_tags WHERE node_id = 1948;
INSERT INTO current_node_tags (node_id, k, v) VALUES (1948, 'hoot:id', '1948');
INSERT INTO node_tags (node_id, k, v, version) VALUES (1948, 'hoot:id', '1948', 2);
/* modify node 1950*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1950, 388920895, -770336529, 3, true, (now() at time zone 'utc'), 1704401621, 2);
UPDATE current_nodes SET latitude=388920895, longitude=-770336529, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401621, version=2 WHERE id=1950;
DELETE FROM current_node_tags WHERE node_id = 1950;
DELETE FROM node_tags WHERE node_id = 1950;
INSERT INTO current_node_tags (node_id, k, v) VALUES (1950, 'hoot:id', '1950');
INSERT INTO node_tags (node_id, k, v, version) VALUES (1950, 'hoot:id', '1950', 2);
/* modify node 1952*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1952, 388901296, -770330309, 3, true, (now() at time zone 'utc'), 1704401621, 2);
UPDATE current_nodes SET latitude=388901296, longitude=-770330309, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401621, version=2 WHERE id=1952;
DELETE FROM current_node_tags WHERE node_id = 1952;
DELETE FROM node_tags WHERE node_id = 1952;
INSERT INTO current_node_tags (node_id, k, v) VALUES (1952, 'hoot:id', '1952');
INSERT INTO node_tags (node_id, k, v, version) VALUES (1952, 'hoot:id', '1952', 2);
/* modify node 2148*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2148, 388951929, -770361414, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388951929, longitude=-770361414, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=2148;
DELETE FROM current_node_tags WHERE node_id = 2148;
DELETE FROM node_tags WHERE node_id = 2148;
INSERT INTO current_node_tags (node_id, k, v) VALUES (2148, 'hoot:id', '2148');
INSERT INTO node_tags (node_id, k, v, version) VALUES (2148, 'hoot:id', '2148', 2);
/* modify node 2150*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2150, 388951810, -770361006, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388951810, longitude=-770361006, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=2150;
DELETE FROM current_node_tags WHERE node_id = 2150;
DELETE FROM node_tags WHERE node_id = 2150;
INSERT INTO current_node_tags (node_id, k, v) VALUES (2150, 'hoot:id', '2150');
INSERT INTO node_tags (node_id, k, v, version) VALUES (2150, 'hoot:id', '2150', 2);
/* modify node 2152*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2152, 388951589, -770360254, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388951589, longitude=-770360254, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=2152;
DELETE FROM current_node_tags WHERE node_id = 2152;
DELETE FROM node_tags WHERE node_id = 2152;
INSERT INTO current_node_tags (node_id, k, v) VALUES (2152, 'hoot:id', '2152');
INSERT INTO node_tags (node_id, k, v, version) VALUES (2152, 'hoot:id', '2152', 2);
/* modify node 2154*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2154, 388951161, -770358619, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388951161, longitude=-770358619, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=2154;
DELETE FROM current_node_tags WHERE node_id = 2154;
DELETE FROM node_tags WHERE node_id = 2154;
INSERT INTO current_node_tags (node_id, k, v) VALUES (2154, 'hoot:id', '2154');
INSERT INTO node_tags (node_id, k, v, version) VALUES (2154, 'hoot:id', '2154', 2);
/* modify node 2156*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2156, 388950969, -770357344, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388950969, longitude=-770357344, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=2156;
DELETE FROM current_node_tags WHERE node_id = 2156;
DELETE FROM node_tags WHERE node_id = 2156;
INSERT INTO current_node_tags (node_id, k, v) VALUES (2156, 'hoot:id', '2156');
INSERT INTO node_tags (node_id, k, v, version) VALUES (2156, 'hoot:id', '2156', 2);
/* modify node 2158*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2158, 388950873, -770356704, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388950873, longitude=-770356704, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=2158;
DELETE FROM current_node_tags WHERE node_id = 2158;
DELETE FROM node_tags WHERE node_id = 2158;
INSERT INTO current_node_tags (node_id, k, v) VALUES (2158, 'hoot:id', '2158');
INSERT INTO node_tags (node_id, k, v, version) VALUES (2158, 'hoot:id', '2158', 2);
/* modify node 2160*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2160, 388950314, -770355189, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388950314, longitude=-770355189, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=2160;
DELETE FROM current_node_tags WHERE node_id = 2160;
DELETE FROM node_tags WHERE node_id = 2160;
INSERT INTO current_node_tags (node_id, k, v) VALUES (2160, 'hoot:id', '2160');
INSERT INTO node_tags (node_id, k, v, version) VALUES (2160, 'hoot:id', '2160', 2);
/* modify node 2162*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2162, 388949434, -770353391, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388949434, longitude=-770353391, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=2162;
DELETE FROM current_node_tags WHERE node_id = 2162;
DELETE FROM node_tags WHERE node_id = 2162;
INSERT INTO current_node_tags (node_id, k, v) VALUES (2162, 'hoot:id', '2162');
INSERT INTO node_tags (node_id, k, v, version) VALUES (2162, 'hoot:id', '2162', 2);
/* modify node 2164*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2164, 388948446, -770351693, 3, true, (now() at time zone 'utc'), 1704401792, 2);
UPDATE current_nodes SET latitude=388948446, longitude=-770351693, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401792, version=2 WHERE id=2164;
DELETE FROM current_node_tags WHERE node_id = 2164;
DELETE FROM node_tags WHERE node_id = 2164;
INSERT INTO current_node_tags (node_id, k, v) VALUES (2164, 'hoot:id', '2164');
INSERT INTO node_tags (node_id, k, v, version) VALUES (2164, 'hoot:id', '2164', 2);
/* modify node 2166*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2166, 388947284, -770350192, 3, true, (now() at time zone 'utc'), 1704401792, 2);
UPDATE current_nodes SET latitude=388947284, longitude=-770350192, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401792, version=2 WHERE id=2166;
DELETE FROM current_node_tags WHERE node_id = 2166;
DELETE FROM node_tags WHERE node_id = 2166;
INSERT INTO current_node_tags (node_id, k, v) VALUES (2166, 'hoot:id', '2166');
INSERT INTO node_tags (node_id, k, v, version) VALUES (2166, 'hoot:id', '2166', 2);
/* modify node 2168*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2168, 388945629, -770348598, 3, true, (now() at time zone 'utc'), 1704401792, 2);
UPDATE current_nodes SET latitude=388945629, longitude=-770348598, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401792, version=2 WHERE id=2168;
DELETE FROM current_node_tags WHERE node_id = 2168;
DELETE FROM node_tags WHERE node_id = 2168;
INSERT INTO current_node_tags (node_id, k, v) VALUES (2168, 'hoot:id', '2168');
INSERT INTO node_tags (node_id, k, v, version) VALUES (2168, 'hoot:id', '2168', 2);
/* modify node 2170*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2170, 388944050, -770347465, 3, true, (now() at time zone 'utc'), 1704401792, 2);
UPDATE current_nodes SET latitude=388944050, longitude=-770347465, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401792, version=2 WHERE id=2170;
DELETE FROM current_node_tags WHERE node_id = 2170;
DELETE FROM node_tags WHERE node_id = 2170;
INSERT INTO current_node_tags (node_id, k, v) VALUES (2170, 'hoot:id', '2170');
INSERT INTO node_tags (node_id, k, v, version) VALUES (2170, 'hoot:id', '2170', 2);
/* modify node 2172*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2172, 388943748, -770347337, 3, true, (now() at time zone 'utc'), 1704401792, 2);
UPDATE current_nodes SET latitude=388943748, longitude=-770347337, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401792, version=2 WHERE id=2172;
DELETE FROM current_node_tags WHERE node_id = 2172;
DELETE FROM node_tags WHERE node_id = 2172;
INSERT INTO current_node_tags (node_id, k, v) VALUES (2172, 'hoot:id', '2172');
INSERT INTO node_tags (node_id, k, v, version) VALUES (2172, 'hoot:id', '2172', 2);
/* modify node 2173*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2173, 388943582, -770347266, 3, true, (now() at time zone 'utc'), 1704401792, 2);
UPDATE current_nodes SET latitude=388943582, longitude=-770347266, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401792, version=2 WHERE id=2173;
DELETE FROM current_node_tags WHERE node_id = 2173;
DELETE FROM node_tags WHERE node_id = 2173;
INSERT INTO current_node_tags (node_id, k, v) VALUES (2173, 'hoot:id', '2173');
INSERT INTO node_tags (node_id, k, v, version) VALUES (2173, 'hoot:id', '2173', 2);
/* modify node 2174*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2174, 388942889, -770346973, 3, true, (now() at time zone 'utc'), 1704401792, 2);
UPDATE current_nodes SET latitude=388942889, longitude=-770346973, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401792, version=2 WHERE id=2174;
DELETE FROM current_node_tags WHERE node_id = 2174;
DELETE FROM node_tags WHERE node_id = 2174;
INSERT INTO current_node_tags (node_id, k, v) VALUES (2174, 'hoot:id', '2174');
INSERT INTO node_tags (node_id, k, v, version) VALUES (2174, 'hoot:id', '2174', 2);
/* modify node 2175*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2175, 388941454, -770346546, 3, true, (now() at time zone 'utc'), 1704401792, 2);
UPDATE current_nodes SET latitude=388941454, longitude=-770346546, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401792, version=2 WHERE id=2175;
DELETE FROM current_node_tags WHERE node_id = 2175;
DELETE FROM node_tags WHERE node_id = 2175;
INSERT INTO current_node_tags (node_id, k, v) VALUES (2175, 'hoot:id', '2175');
INSERT INTO node_tags (node_id, k, v, version) VALUES (2175, 'hoot:id', '2175', 2);
/* modify node 2176*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2176, 388940315, -770346324, 3, true, (now() at time zone 'utc'), 1704401792, 2);
UPDATE current_nodes SET latitude=388940315, longitude=-770346324, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401792, version=2 WHERE id=2176;
DELETE FROM current_node_tags WHERE node_id = 2176;
DELETE FROM node_tags WHERE node_id = 2176;
INSERT INTO current_node_tags (node_id, k, v) VALUES (2176, 'hoot:id', '2176');
INSERT INTO node_tags (node_id, k, v, version) VALUES (2176, 'hoot:id', '2176', 2);
/* modify node 2177*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2177, 388939032, -770346199, 3, true, (now() at time zone 'utc'), 1704401792, 2);
UPDATE current_nodes SET latitude=388939032, longitude=-770346199, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401792, version=2 WHERE id=2177;
DELETE FROM current_node_tags WHERE node_id = 2177;
DELETE FROM node_tags WHERE node_id = 2177;
INSERT INTO current_node_tags (node_id, k, v) VALUES (2177, 'hoot:id', '2177');
INSERT INTO node_tags (node_id, k, v, version) VALUES (2177, 'hoot:id', '2177', 2);
/* modify node 2178*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2178, 388937660, -770346382, 3, true, (now() at time zone 'utc'), 1704401792, 2);
UPDATE current_nodes SET latitude=388937660, longitude=-770346382, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401792, version=2 WHERE id=2178;
DELETE FROM current_node_tags WHERE node_id = 2178;
DELETE FROM node_tags WHERE node_id = 2178;
INSERT INTO current_node_tags (node_id, k, v) VALUES (2178, 'hoot:id', '2178');
INSERT INTO node_tags (node_id, k, v, version) VALUES (2178, 'hoot:id', '2178', 2);
/* modify node 2179*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2179, 388936412, -770346732, 3, true, (now() at time zone 'utc'), 1704401792, 2);
UPDATE current_nodes SET latitude=388936412, longitude=-770346732, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401792, version=2 WHERE id=2179;
DELETE FROM current_node_tags WHERE node_id = 2179;
DELETE FROM node_tags WHERE node_id = 2179;
INSERT INTO current_node_tags (node_id, k, v) VALUES (2179, 'hoot:id', '2179');
INSERT INTO node_tags (node_id, k, v, version) VALUES (2179, 'hoot:id', '2179', 2);
/* modify node 2180*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2180, 388935443, -770347170, 3, true, (now() at time zone 'utc'), 1704401792, 2);
UPDATE current_nodes SET latitude=388935443, longitude=-770347170, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401792, version=2 WHERE id=2180;
DELETE FROM current_node_tags WHERE node_id = 2180;
DELETE FROM node_tags WHERE node_id = 2180;
INSERT INTO current_node_tags (node_id, k, v) VALUES (2180, 'hoot:id', '2180');
INSERT INTO node_tags (node_id, k, v, version) VALUES (2180, 'hoot:id', '2180', 2);
/* modify node 2181*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2181, 388934273, -770347805, 3, true, (now() at time zone 'utc'), 1704401792, 2);
UPDATE current_nodes SET latitude=388934273, longitude=-770347805, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401792, version=2 WHERE id=2181;
DELETE FROM current_node_tags WHERE node_id = 2181;
DELETE FROM node_tags WHERE node_id = 2181;
INSERT INTO current_node_tags (node_id, k, v) VALUES (2181, 'hoot:id', '2181');
INSERT INTO node_tags (node_id, k, v, version) VALUES (2181, 'hoot:id', '2181', 2);
/* modify node 2182*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2182, 388933784, -770348099, 3, true, (now() at time zone 'utc'), 1704401792, 2);
UPDATE current_nodes SET latitude=388933784, longitude=-770348099, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401792, version=2 WHERE id=2182;
DELETE FROM current_node_tags WHERE node_id = 2182;
DELETE FROM node_tags WHERE node_id = 2182;
INSERT INTO current_node_tags (node_id, k, v) VALUES (2182, 'hoot:id', '2182');
INSERT INTO node_tags (node_id, k, v, version) VALUES (2182, 'hoot:id', '2182', 2);
/* modify node 2183*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2183, 388933540, -770348246, 3, true, (now() at time zone 'utc'), 1704401792, 2);
UPDATE current_nodes SET latitude=388933540, longitude=-770348246, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401792, version=2 WHERE id=2183;
DELETE FROM current_node_tags WHERE node_id = 2183;
DELETE FROM node_tags WHERE node_id = 2183;
INSERT INTO current_node_tags (node_id, k, v) VALUES (2183, 'hoot:id', '2183');
INSERT INTO node_tags (node_id, k, v, version) VALUES (2183, 'hoot:id', '2183', 2);
/* modify node 2184*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2184, 388932646, -770348669, 3, true, (now() at time zone 'utc'), 1704401792, 2);
UPDATE current_nodes SET latitude=388932646, longitude=-770348669, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401792, version=2 WHERE id=2184;
DELETE FROM current_node_tags WHERE node_id = 2184;
DELETE FROM node_tags WHERE node_id = 2184;
INSERT INTO current_node_tags (node_id, k, v) VALUES (2184, 'hoot:id', '2184');
INSERT INTO node_tags (node_id, k, v, version) VALUES (2184, 'hoot:id', '2184', 2);
/* modify node 2185*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2185, 388931701, -770349710, 3, true, (now() at time zone 'utc'), 1704401792, 2);
UPDATE current_nodes SET latitude=388931701, longitude=-770349710, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401792, version=2 WHERE id=2185;
DELETE FROM current_node_tags WHERE node_id = 2185;
DELETE FROM node_tags WHERE node_id = 2185;
INSERT INTO current_node_tags (node_id, k, v) VALUES (2185, 'hoot:id', '2185');
INSERT INTO node_tags (node_id, k, v, version) VALUES (2185, 'hoot:id', '2185', 2);
/* modify node 2186*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2186, 388931311, -770350140, 3, true, (now() at time zone 'utc'), 1704401792, 2);
UPDATE current_nodes SET latitude=388931311, longitude=-770350140, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401792, version=2 WHERE id=2186;
DELETE FROM current_node_tags WHERE node_id = 2186;
DELETE FROM node_tags WHERE node_id = 2186;
INSERT INTO current_node_tags (node_id, k, v) VALUES (2186, 'hoot:id', '2186');
INSERT INTO node_tags (node_id, k, v, version) VALUES (2186, 'hoot:id', '2186', 2);
/* modify node 2187*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2187, 388930755, -770350845, 3, true, (now() at time zone 'utc'), 1704401792, 2);
UPDATE current_nodes SET latitude=388930755, longitude=-770350845, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401792, version=2 WHERE id=2187;
DELETE FROM current_node_tags WHERE node_id = 2187;
DELETE FROM node_tags WHERE node_id = 2187;
INSERT INTO current_node_tags (node_id, k, v) VALUES (2187, 'hoot:id', '2187');
INSERT INTO node_tags (node_id, k, v, version) VALUES (2187, 'hoot:id', '2187', 2);
/* modify node 2188*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2188, 388930030, -770351834, 3, true, (now() at time zone 'utc'), 1704401792, 2);
UPDATE current_nodes SET latitude=388930030, longitude=-770351834, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401792, version=2 WHERE id=2188;
DELETE FROM current_node_tags WHERE node_id = 2188;
DELETE FROM node_tags WHERE node_id = 2188;
INSERT INTO current_node_tags (node_id, k, v) VALUES (2188, 'hoot:id', '2188');
INSERT INTO node_tags (node_id, k, v, version) VALUES (2188, 'hoot:id', '2188', 2);
/* modify node 2189*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2189, 388929420, -770352946, 3, true, (now() at time zone 'utc'), 1704401792, 2);
UPDATE current_nodes SET latitude=388929420, longitude=-770352946, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401792, version=2 WHERE id=2189;
DELETE FROM current_node_tags WHERE node_id = 2189;
DELETE FROM node_tags WHERE node_id = 2189;
INSERT INTO current_node_tags (node_id, k, v) VALUES (2189, 'hoot:id', '2189');
INSERT INTO node_tags (node_id, k, v, version) VALUES (2189, 'hoot:id', '2189', 2);
/* modify node 2191*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2191, 388929289, -770353221, 3, true, (now() at time zone 'utc'), 1704401792, 2);
UPDATE current_nodes SET latitude=388929289, longitude=-770353221, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401792, version=2 WHERE id=2191;
DELETE FROM current_node_tags WHERE node_id = 2191;
DELETE FROM node_tags WHERE node_id = 2191;
INSERT INTO current_node_tags (node_id, k, v) VALUES (2191, 'hoot:id', '2191');
INSERT INTO node_tags (node_id, k, v, version) VALUES (2191, 'hoot:id', '2191', 2);
/* modify node 2193*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2193, 388928914, -770353952, 3, true, (now() at time zone 'utc'), 1704401792, 2);
UPDATE current_nodes SET latitude=388928914, longitude=-770353952, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401792, version=2 WHERE id=2193;
DELETE FROM current_node_tags WHERE node_id = 2193;
DELETE FROM node_tags WHERE node_id = 2193;
INSERT INTO current_node_tags (node_id, k, v) VALUES (2193, 'hoot:id', '2193');
INSERT INTO node_tags (node_id, k, v, version) VALUES (2193, 'hoot:id', '2193', 2);
/* modify node 2195*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2195, 388928748, -770354276, 3, true, (now() at time zone 'utc'), 1704401792, 2);
UPDATE current_nodes SET latitude=388928748, longitude=-770354276, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401792, version=2 WHERE id=2195;
DELETE FROM current_node_tags WHERE node_id = 2195;
DELETE FROM node_tags WHERE node_id = 2195;
INSERT INTO current_node_tags (node_id, k, v) VALUES (2195, 'hoot:id', '2195');
INSERT INTO node_tags (node_id, k, v, version) VALUES (2195, 'hoot:id', '2195', 2);
/* modify node 2197*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2197, 388928670, -770354442, 3, true, (now() at time zone 'utc'), 1704401792, 2);
UPDATE current_nodes SET latitude=388928670, longitude=-770354442, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401792, version=2 WHERE id=2197;
DELETE FROM current_node_tags WHERE node_id = 2197;
DELETE FROM node_tags WHERE node_id = 2197;
INSERT INTO current_node_tags (node_id, k, v) VALUES (2197, 'hoot:id', '2197');
INSERT INTO node_tags (node_id, k, v, version) VALUES (2197, 'hoot:id', '2197', 2);
/* modify node 2199*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2199, 388928505, -770354805, 3, true, (now() at time zone 'utc'), 1704401792, 2);
UPDATE current_nodes SET latitude=388928505, longitude=-770354805, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401792, version=2 WHERE id=2199;
DELETE FROM current_node_tags WHERE node_id = 2199;
DELETE FROM node_tags WHERE node_id = 2199;
INSERT INTO current_node_tags (node_id, k, v) VALUES (2199, 'hoot:id', '2199');
INSERT INTO node_tags (node_id, k, v, version) VALUES (2199, 'hoot:id', '2199', 2);
/* modify node 2201*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2201, 388928420, -770355014, 3, true, (now() at time zone 'utc'), 1704401792, 2);
UPDATE current_nodes SET latitude=388928420, longitude=-770355014, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401792, version=2 WHERE id=2201;
DELETE FROM current_node_tags WHERE node_id = 2201;
DELETE FROM node_tags WHERE node_id = 2201;
INSERT INTO current_node_tags (node_id, k, v) VALUES (2201, 'hoot:id', '2201');
INSERT INTO node_tags (node_id, k, v, version) VALUES (2201, 'hoot:id', '2201', 2);
/* modify node 2203*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2203, 388928193, -770355578, 3, true, (now() at time zone 'utc'), 1704401792, 2);
UPDATE current_nodes SET latitude=388928193, longitude=-770355578, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401792, version=2 WHERE id=2203;
DELETE FROM current_node_tags WHERE node_id = 2203;
DELETE FROM node_tags WHERE node_id = 2203;
INSERT INTO current_node_tags (node_id, k, v) VALUES (2203, 'hoot:id', '2203');
INSERT INTO node_tags (node_id, k, v, version) VALUES (2203, 'hoot:id', '2203', 2);
/* modify node 2205*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2205, 388928024, -770356056, 3, true, (now() at time zone 'utc'), 1704401792, 2);
UPDATE current_nodes SET latitude=388928024, longitude=-770356056, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401792, version=2 WHERE id=2205;
DELETE FROM current_node_tags WHERE node_id = 2205;
DELETE FROM node_tags WHERE node_id = 2205;
INSERT INTO current_node_tags (node_id, k, v) VALUES (2205, 'hoot:id', '2205');
INSERT INTO node_tags (node_id, k, v, version) VALUES (2205, 'hoot:id', '2205', 2);
/* modify node 2207*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2207, 388927847, -770356546, 3, true, (now() at time zone 'utc'), 1704401792, 2);
UPDATE current_nodes SET latitude=388927847, longitude=-770356546, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401792, version=2 WHERE id=2207;
DELETE FROM current_node_tags WHERE node_id = 2207;
DELETE FROM node_tags WHERE node_id = 2207;
INSERT INTO current_node_tags (node_id, k, v) VALUES (2207, 'hoot:id', '2207');
INSERT INTO node_tags (node_id, k, v, version) VALUES (2207, 'hoot:id', '2207', 2);
/* modify node 2209*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2209, 388927220, -770358463, 3, true, (now() at time zone 'utc'), 1704401792, 2);
UPDATE current_nodes SET latitude=388927220, longitude=-770358463, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401792, version=2 WHERE id=2209;
DELETE FROM current_node_tags WHERE node_id = 2209;
DELETE FROM node_tags WHERE node_id = 2209;
INSERT INTO current_node_tags (node_id, k, v) VALUES (2209, 'hoot:id', '2209');
INSERT INTO node_tags (node_id, k, v, version) VALUES (2209, 'hoot:id', '2209', 2);
/* modify node 2210*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2210, 388926857, -770360004, 3, true, (now() at time zone 'utc'), 1704401792, 2);
UPDATE current_nodes SET latitude=388926857, longitude=-770360004, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401792, version=2 WHERE id=2210;
DELETE FROM current_node_tags WHERE node_id = 2210;
DELETE FROM node_tags WHERE node_id = 2210;
INSERT INTO current_node_tags (node_id, k, v) VALUES (2210, 'hoot:id', '2210');
INSERT INTO node_tags (node_id, k, v, version) VALUES (2210, 'hoot:id', '2210', 2);
/* modify node 2211*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2211, 388926807, -770360240, 3, true, (now() at time zone 'utc'), 1704401792, 2);
UPDATE current_nodes SET latitude=388926807, longitude=-770360240, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401792, version=2 WHERE id=2211;
DELETE FROM current_node_tags WHERE node_id = 2211;
DELETE FROM node_tags WHERE node_id = 2211;
INSERT INTO current_node_tags (node_id, k, v) VALUES (2211, 'hoot:id', '2211');
INSERT INTO node_tags (node_id, k, v, version) VALUES (2211, 'hoot:id', '2211', 2);
/* modify node 2212*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2212, 388926662, -770360899, 3, true, (now() at time zone 'utc'), 1704401792, 2);
UPDATE current_nodes SET latitude=388926662, longitude=-770360899, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401792, version=2 WHERE id=2212;
DELETE FROM current_node_tags WHERE node_id = 2212;
DELETE FROM node_tags WHERE node_id = 2212;
INSERT INTO current_node_tags (node_id, k, v) VALUES (2212, 'hoot:id', '2212');
INSERT INTO node_tags (node_id, k, v, version) VALUES (2212, 'hoot:id', '2212', 2);
/* modify node 2213*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2213, 388926399, -770362665, 3, true, (now() at time zone 'utc'), 1704401792, 2);
UPDATE current_nodes SET latitude=388926399, longitude=-770362665, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401792, version=2 WHERE id=2213;
DELETE FROM current_node_tags WHERE node_id = 2213;
DELETE FROM node_tags WHERE node_id = 2213;
INSERT INTO current_node_tags (node_id, k, v) VALUES (2213, 'hoot:id', '2213');
INSERT INTO node_tags (node_id, k, v, version) VALUES (2213, 'hoot:id', '2213', 2);
/* modify node 2214*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2214, 388926249, -770363669, 3, true, (now() at time zone 'utc'), 1704401792, 2);
UPDATE current_nodes SET latitude=388926249, longitude=-770363669, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401792, version=2 WHERE id=2214;
DELETE FROM current_node_tags WHERE node_id = 2214;
DELETE FROM node_tags WHERE node_id = 2214;
INSERT INTO current_node_tags (node_id, k, v) VALUES (2214, 'hoot:id', '2214');
INSERT INTO node_tags (node_id, k, v, version) VALUES (2214, 'hoot:id', '2214', 2);
/* modify node 2215*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2215, 388926088, -770364968, 3, true, (now() at time zone 'utc'), 1704401792, 2);
UPDATE current_nodes SET latitude=388926088, longitude=-770364968, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401792, version=2 WHERE id=2215;
DELETE FROM current_node_tags WHERE node_id = 2215;
DELETE FROM node_tags WHERE node_id = 2215;
INSERT INTO current_node_tags (node_id, k, v) VALUES (2215, 'hoot:id', '2215');
INSERT INTO node_tags (node_id, k, v, version) VALUES (2215, 'hoot:id', '2215', 2);
/* modify node 2687*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2687, 388926095, -770365439, 3, true, (now() at time zone 'utc'), 1704401792, 2);
UPDATE current_nodes SET latitude=388926095, longitude=-770365439, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401792, version=2 WHERE id=2687;
DELETE FROM current_node_tags WHERE node_id = 2687;
DELETE FROM node_tags WHERE node_id = 2687;
INSERT INTO current_node_tags (node_id, k, v) VALUES (2687, 'hoot:id', '2687');
INSERT INTO node_tags (node_id, k, v, version) VALUES (2687, 'hoot:id', '2687', 2);
/* modify node 2690*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2690, 388924599, -770365459, 3, true, (now() at time zone 'utc'), 1704401792, 2);
UPDATE current_nodes SET latitude=388924599, longitude=-770365459, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401792, version=2 WHERE id=2690;
DELETE FROM current_node_tags WHERE node_id = 2690;
DELETE FROM node_tags WHERE node_id = 2690;
INSERT INTO current_node_tags (node_id, k, v) VALUES (2690, 'hoot:id', '2690');
INSERT INTO node_tags (node_id, k, v, version) VALUES (2690, 'hoot:id', '2690', 2);
/* modify node 2693*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2693, 388924345, -770365457, 3, true, (now() at time zone 'utc'), 1704401792, 2);
UPDATE current_nodes SET latitude=388924345, longitude=-770365457, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401792, version=2 WHERE id=2693;
DELETE FROM current_node_tags WHERE node_id = 2693;
DELETE FROM node_tags WHERE node_id = 2693;
INSERT INTO current_node_tags (node_id, k, v) VALUES (2693, 'hoot:id', '2693');
INSERT INTO node_tags (node_id, k, v, version) VALUES (2693, 'hoot:id', '2693', 2);
/* modify node 2696*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2696, 388922608, -770365446, 3, true, (now() at time zone 'utc'), 1704401792, 2);
UPDATE current_nodes SET latitude=388922608, longitude=-770365446, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401792, version=2 WHERE id=2696;
DELETE FROM current_node_tags WHERE node_id = 2696;
DELETE FROM node_tags WHERE node_id = 2696;
INSERT INTO current_node_tags (node_id, k, v) VALUES (2696, 'hoot:id', '2696');
INSERT INTO node_tags (node_id, k, v, version) VALUES (2696, 'hoot:id', '2696', 2);
/* modify node 2699*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2699, 388922264, -770365444, 3, true, (now() at time zone 'utc'), 1704401792, 2);
UPDATE current_nodes SET latitude=388922264, longitude=-770365444, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401792, version=2 WHERE id=2699;
DELETE FROM current_node_tags WHERE node_id = 2699;
DELETE FROM node_tags WHERE node_id = 2699;
INSERT INTO current_node_tags (node_id, k, v) VALUES (2699, 'hoot:id', '2699');
INSERT INTO node_tags (node_id, k, v, version) VALUES (2699, 'hoot:id', '2699', 2);
/* modify node 2702*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2702, 388920965, -770365414, 3, true, (now() at time zone 'utc'), 1704401621, 2);
UPDATE current_nodes SET latitude=388920965, longitude=-770365414, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401621, version=2 WHERE id=2702;
DELETE FROM current_node_tags WHERE node_id = 2702;
DELETE FROM node_tags WHERE node_id = 2702;
INSERT INTO current_node_tags (node_id, k, v) VALUES (2702, 'hoot:id', '2702');
INSERT INTO node_tags (node_id, k, v, version) VALUES (2702, 'hoot:id', '2702', 2);
/* modify node 3041*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3041, 388954050, -770417271, 3, true, (now() at time zone 'utc'), 1704401707, 2);
UPDATE current_nodes SET latitude=388954050, longitude=-770417271, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401707, version=2 WHERE id=3041;
DELETE FROM current_node_tags WHERE node_id = 3041;
DELETE FROM node_tags WHERE node_id = 3041;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3041, 'hoot:id', '3041');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3041, 'hoot:id', '3041', 2);
/* modify node 3056*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3056, 388943331, -770417297, 3, true, (now() at time zone 'utc'), 1704401706, 2);
UPDATE current_nodes SET latitude=388943331, longitude=-770417297, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401706, version=2 WHERE id=3056;
DELETE FROM current_node_tags WHERE node_id = 3056;
DELETE FROM node_tags WHERE node_id = 3056;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3056, 'hoot:id', '3056');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3056, 'hoot:id', '3056', 2);
/* modify node 3071*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3071, 388935291, -770417432, 3, true, (now() at time zone 'utc'), 1704401706, 2);
UPDATE current_nodes SET latitude=388935291, longitude=-770417432, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401706, version=2 WHERE id=3071;
DELETE FROM current_node_tags WHERE node_id = 3071;
DELETE FROM node_tags WHERE node_id = 3071;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3071, 'hoot:id', '3071');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3071, 'hoot:id', '3071', 2);
/* modify node 3093*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3093, 388926119, -770366844, 3, true, (now() at time zone 'utc'), 1704401792, 2);
UPDATE current_nodes SET latitude=388926119, longitude=-770366844, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401792, version=2 WHERE id=3093;
DELETE FROM current_node_tags WHERE node_id = 3093;
DELETE FROM node_tags WHERE node_id = 3093;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3093, 'hoot:id', '3093');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3093, 'hoot:id', '3093', 2);
/* modify node 3095*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3095, 388926545, -770369358, 3, true, (now() at time zone 'utc'), 1704401792, 2);
UPDATE current_nodes SET latitude=388926545, longitude=-770369358, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401792, version=2 WHERE id=3095;
DELETE FROM current_node_tags WHERE node_id = 3095;
DELETE FROM node_tags WHERE node_id = 3095;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3095, 'hoot:id', '3095');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3095, 'hoot:id', '3095', 2);
/* modify node 3097*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3097, 388926602, -770369770, 3, true, (now() at time zone 'utc'), 1704401792, 2);
UPDATE current_nodes SET latitude=388926602, longitude=-770369770, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401792, version=2 WHERE id=3097;
DELETE FROM current_node_tags WHERE node_id = 3097;
DELETE FROM node_tags WHERE node_id = 3097;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3097, 'hoot:id', '3097');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3097, 'hoot:id', '3097', 2);
/* modify node 3099*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3099, 388926646, -770370063, 3, true, (now() at time zone 'utc'), 1704401792, 2);
UPDATE current_nodes SET latitude=388926646, longitude=-770370063, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401792, version=2 WHERE id=3099;
DELETE FROM current_node_tags WHERE node_id = 3099;
DELETE FROM node_tags WHERE node_id = 3099;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3099, 'hoot:id', '3099');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3099, 'hoot:id', '3099', 2);
/* modify node 3101*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3101, 388926919, -770371339, 3, true, (now() at time zone 'utc'), 1704401792, 2);
UPDATE current_nodes SET latitude=388926919, longitude=-770371339, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401792, version=2 WHERE id=3101;
DELETE FROM current_node_tags WHERE node_id = 3101;
DELETE FROM node_tags WHERE node_id = 3101;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3101, 'hoot:id', '3101');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3101, 'hoot:id', '3101', 2);
/* modify node 3103*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3103, 388927130, -770372249, 3, true, (now() at time zone 'utc'), 1704401792, 2);
UPDATE current_nodes SET latitude=388927130, longitude=-770372249, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401792, version=2 WHERE id=3103;
DELETE FROM current_node_tags WHERE node_id = 3103;
DELETE FROM node_tags WHERE node_id = 3103;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3103, 'hoot:id', '3103');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3103, 'hoot:id', '3103', 2);
/* modify node 3105*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3105, 388927764, -770374330, 3, true, (now() at time zone 'utc'), 1704401706, 2);
UPDATE current_nodes SET latitude=388927764, longitude=-770374330, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401706, version=2 WHERE id=3105;
DELETE FROM current_node_tags WHERE node_id = 3105;
DELETE FROM node_tags WHERE node_id = 3105;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3105, 'hoot:id', '3105');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3105, 'hoot:id', '3105', 2);
/* modify node 3107*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3107, 388928254, -770375649, 3, true, (now() at time zone 'utc'), 1704401706, 2);
UPDATE current_nodes SET latitude=388928254, longitude=-770375649, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401706, version=2 WHERE id=3107;
DELETE FROM current_node_tags WHERE node_id = 3107;
DELETE FROM node_tags WHERE node_id = 3107;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3107, 'hoot:id', '3107');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3107, 'hoot:id', '3107', 2);
/* modify node 3109*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3109, 388928727, -770376765, 3, true, (now() at time zone 'utc'), 1704401706, 2);
UPDATE current_nodes SET latitude=388928727, longitude=-770376765, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401706, version=2 WHERE id=3109;
DELETE FROM current_node_tags WHERE node_id = 3109;
DELETE FROM node_tags WHERE node_id = 3109;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3109, 'hoot:id', '3109');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3109, 'hoot:id', '3109', 2);
/* modify node 3111*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3111, 388929141, -770377597, 3, true, (now() at time zone 'utc'), 1704401706, 2);
UPDATE current_nodes SET latitude=388929141, longitude=-770377597, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401706, version=2 WHERE id=3111;
DELETE FROM current_node_tags WHERE node_id = 3111;
DELETE FROM node_tags WHERE node_id = 3111;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3111, 'hoot:id', '3111');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3111, 'hoot:id', '3111', 2);
/* modify node 3113*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3113, 388929603, -770378379, 3, true, (now() at time zone 'utc'), 1704401706, 2);
UPDATE current_nodes SET latitude=388929603, longitude=-770378379, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401706, version=2 WHERE id=3113;
DELETE FROM current_node_tags WHERE node_id = 3113;
DELETE FROM node_tags WHERE node_id = 3113;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3113, 'hoot:id', '3113');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3113, 'hoot:id', '3113', 2);
/* modify node 3115*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3115, 388930085, -770379118, 3, true, (now() at time zone 'utc'), 1704401706, 2);
UPDATE current_nodes SET latitude=388930085, longitude=-770379118, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401706, version=2 WHERE id=3115;
DELETE FROM current_node_tags WHERE node_id = 3115;
DELETE FROM node_tags WHERE node_id = 3115;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3115, 'hoot:id', '3115');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3115, 'hoot:id', '3115', 2);
/* modify node 3117*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3117, 388931007, -770380346, 3, true, (now() at time zone 'utc'), 1704401706, 2);
UPDATE current_nodes SET latitude=388931007, longitude=-770380346, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401706, version=2 WHERE id=3117;
DELETE FROM current_node_tags WHERE node_id = 3117;
DELETE FROM node_tags WHERE node_id = 3117;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3117, 'hoot:id', '3117');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3117, 'hoot:id', '3117', 2);
/* modify node 3119*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3119, 388931660, -770381138, 3, true, (now() at time zone 'utc'), 1704401706, 2);
UPDATE current_nodes SET latitude=388931660, longitude=-770381138, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401706, version=2 WHERE id=3119;
DELETE FROM current_node_tags WHERE node_id = 3119;
DELETE FROM node_tags WHERE node_id = 3119;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3119, 'hoot:id', '3119');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3119, 'hoot:id', '3119', 2);
/* modify node 3121*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3121, 388931777, -770381257, 3, true, (now() at time zone 'utc'), 1704401706, 2);
UPDATE current_nodes SET latitude=388931777, longitude=-770381257, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401706, version=2 WHERE id=3121;
DELETE FROM current_node_tags WHERE node_id = 3121;
DELETE FROM node_tags WHERE node_id = 3121;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3121, 'hoot:id', '3121');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3121, 'hoot:id', '3121', 2);
/* modify node 3123*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3123, 388932417, -770381908, 3, true, (now() at time zone 'utc'), 1704401706, 2);
UPDATE current_nodes SET latitude=388932417, longitude=-770381908, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401706, version=2 WHERE id=3123;
DELETE FROM current_node_tags WHERE node_id = 3123;
DELETE FROM node_tags WHERE node_id = 3123;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3123, 'hoot:id', '3123');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3123, 'hoot:id', '3123', 2);
/* modify node 3125*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3125, 388932775, -770382199, 3, true, (now() at time zone 'utc'), 1704401706, 2);
UPDATE current_nodes SET latitude=388932775, longitude=-770382199, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401706, version=2 WHERE id=3125;
DELETE FROM current_node_tags WHERE node_id = 3125;
DELETE FROM node_tags WHERE node_id = 3125;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3125, 'hoot:id', '3125');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3125, 'hoot:id', '3125', 2);
/* modify node 3127*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3127, 388933046, -770382419, 3, true, (now() at time zone 'utc'), 1704401706, 2);
UPDATE current_nodes SET latitude=388933046, longitude=-770382419, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401706, version=2 WHERE id=3127;
DELETE FROM current_node_tags WHERE node_id = 3127;
DELETE FROM node_tags WHERE node_id = 3127;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3127, 'hoot:id', '3127');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3127, 'hoot:id', '3127', 2);
/* modify node 3129*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3129, 388933744, -770382909, 3, true, (now() at time zone 'utc'), 1704401706, 2);
UPDATE current_nodes SET latitude=388933744, longitude=-770382909, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401706, version=2 WHERE id=3129;
DELETE FROM current_node_tags WHERE node_id = 3129;
DELETE FROM node_tags WHERE node_id = 3129;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3129, 'hoot:id', '3129');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3129, 'hoot:id', '3129', 2);
/* modify node 3131*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3131, 388934653, -770383418, 3, true, (now() at time zone 'utc'), 1704401706, 2);
UPDATE current_nodes SET latitude=388934653, longitude=-770383418, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401706, version=2 WHERE id=3131;
DELETE FROM current_node_tags WHERE node_id = 3131;
DELETE FROM node_tags WHERE node_id = 3131;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3131, 'hoot:id', '3131');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3131, 'hoot:id', '3131', 2);
/* modify node 3133*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3133, 388936401, -770384321, 3, true, (now() at time zone 'utc'), 1704401706, 2);
UPDATE current_nodes SET latitude=388936401, longitude=-770384321, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401706, version=2 WHERE id=3133;
DELETE FROM current_node_tags WHERE node_id = 3133;
DELETE FROM node_tags WHERE node_id = 3133;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3133, 'hoot:id', '3133');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3133, 'hoot:id', '3133', 2);
/* modify node 3135*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3135, 388937108, -770384571, 3, true, (now() at time zone 'utc'), 1704401706, 2);
UPDATE current_nodes SET latitude=388937108, longitude=-770384571, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401706, version=2 WHERE id=3135;
DELETE FROM current_node_tags WHERE node_id = 3135;
DELETE FROM node_tags WHERE node_id = 3135;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3135, 'hoot:id', '3135');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3135, 'hoot:id', '3135', 2);
/* modify node 3137*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3137, 388938352, -770384861, 3, true, (now() at time zone 'utc'), 1704401706, 2);
UPDATE current_nodes SET latitude=388938352, longitude=-770384861, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401706, version=2 WHERE id=3137;
DELETE FROM current_node_tags WHERE node_id = 3137;
DELETE FROM node_tags WHERE node_id = 3137;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3137, 'hoot:id', '3137');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3137, 'hoot:id', '3137', 2);
/* modify node 3139*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3139, 388939365, -770384890, 3, true, (now() at time zone 'utc'), 1704401706, 2);
UPDATE current_nodes SET latitude=388939365, longitude=-770384890, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401706, version=2 WHERE id=3139;
DELETE FROM current_node_tags WHERE node_id = 3139;
DELETE FROM node_tags WHERE node_id = 3139;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3139, 'hoot:id', '3139');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3139, 'hoot:id', '3139', 2);
/* modify node 3141*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3141, 388940373, -770384836, 3, true, (now() at time zone 'utc'), 1704401706, 2);
UPDATE current_nodes SET latitude=388940373, longitude=-770384836, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401706, version=2 WHERE id=3141;
DELETE FROM current_node_tags WHERE node_id = 3141;
DELETE FROM node_tags WHERE node_id = 3141;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3141, 'hoot:id', '3141');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3141, 'hoot:id', '3141', 2);
/* modify node 3143*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3143, 388941190, -770384688, 3, true, (now() at time zone 'utc'), 1704401706, 2);
UPDATE current_nodes SET latitude=388941190, longitude=-770384688, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401706, version=2 WHERE id=3143;
DELETE FROM current_node_tags WHERE node_id = 3143;
DELETE FROM node_tags WHERE node_id = 3143;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3143, 'hoot:id', '3143');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3143, 'hoot:id', '3143', 2);
/* modify node 3145*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3145, 388942156, -770384397, 3, true, (now() at time zone 'utc'), 1704401706, 2);
UPDATE current_nodes SET latitude=388942156, longitude=-770384397, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401706, version=2 WHERE id=3145;
DELETE FROM current_node_tags WHERE node_id = 3145;
DELETE FROM node_tags WHERE node_id = 3145;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3145, 'hoot:id', '3145');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3145, 'hoot:id', '3145', 2);
/* modify node 3147*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3147, 388943307, -770383982, 3, true, (now() at time zone 'utc'), 1704401706, 2);
UPDATE current_nodes SET latitude=388943307, longitude=-770383982, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401706, version=2 WHERE id=3147;
DELETE FROM current_node_tags WHERE node_id = 3147;
DELETE FROM node_tags WHERE node_id = 3147;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3147, 'hoot:id', '3147');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3147, 'hoot:id', '3147', 2);
/* modify node 3149*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3149, 388944098, -770383517, 3, true, (now() at time zone 'utc'), 1704401706, 2);
UPDATE current_nodes SET latitude=388944098, longitude=-770383517, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401706, version=2 WHERE id=3149;
DELETE FROM current_node_tags WHERE node_id = 3149;
DELETE FROM node_tags WHERE node_id = 3149;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3149, 'hoot:id', '3149');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3149, 'hoot:id', '3149', 2);
/* modify node 3151*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3151, 388945057, -770382887, 3, true, (now() at time zone 'utc'), 1704401706, 2);
UPDATE current_nodes SET latitude=388945057, longitude=-770382887, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401706, version=2 WHERE id=3151;
DELETE FROM current_node_tags WHERE node_id = 3151;
DELETE FROM node_tags WHERE node_id = 3151;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3151, 'hoot:id', '3151');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3151, 'hoot:id', '3151', 2);
/* modify node 3153*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3153, 388946073, -770382063, 3, true, (now() at time zone 'utc'), 1704401706, 2);
UPDATE current_nodes SET latitude=388946073, longitude=-770382063, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401706, version=2 WHERE id=3153;
DELETE FROM current_node_tags WHERE node_id = 3153;
DELETE FROM node_tags WHERE node_id = 3153;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3153, 'hoot:id', '3153');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3153, 'hoot:id', '3153', 2);
/* modify node 3155*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3155, 388946771, -770381404, 3, true, (now() at time zone 'utc'), 1704401706, 2);
UPDATE current_nodes SET latitude=388946771, longitude=-770381404, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401706, version=2 WHERE id=3155;
DELETE FROM current_node_tags WHERE node_id = 3155;
DELETE FROM node_tags WHERE node_id = 3155;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3155, 'hoot:id', '3155');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3155, 'hoot:id', '3155', 2);
/* modify node 3157*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3157, 388947461, -770380665, 3, true, (now() at time zone 'utc'), 1704401706, 2);
UPDATE current_nodes SET latitude=388947461, longitude=-770380665, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401706, version=2 WHERE id=3157;
DELETE FROM current_node_tags WHERE node_id = 3157;
DELETE FROM node_tags WHERE node_id = 3157;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3157, 'hoot:id', '3157');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3157, 'hoot:id', '3157', 2);
/* modify node 3159*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3159, 388948023, -770379957, 3, true, (now() at time zone 'utc'), 1704401706, 2);
UPDATE current_nodes SET latitude=388948023, longitude=-770379957, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401706, version=2 WHERE id=3159;
DELETE FROM current_node_tags WHERE node_id = 3159;
DELETE FROM node_tags WHERE node_id = 3159;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3159, 'hoot:id', '3159');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3159, 'hoot:id', '3159', 2);
/* modify node 3161*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3161, 388948600, -770379079, 3, true, (now() at time zone 'utc'), 1704401706, 2);
UPDATE current_nodes SET latitude=388948600, longitude=-770379079, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401706, version=2 WHERE id=3161;
DELETE FROM current_node_tags WHERE node_id = 3161;
DELETE FROM node_tags WHERE node_id = 3161;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3161, 'hoot:id', '3161');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3161, 'hoot:id', '3161', 2);
/* modify node 3163*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3163, 388949138, -770378227, 3, true, (now() at time zone 'utc'), 1704401706, 2);
UPDATE current_nodes SET latitude=388949138, longitude=-770378227, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401706, version=2 WHERE id=3163;
DELETE FROM current_node_tags WHERE node_id = 3163;
DELETE FROM node_tags WHERE node_id = 3163;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3163, 'hoot:id', '3163');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3163, 'hoot:id', '3163', 2);
/* modify node 3165*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3165, 388949735, -770376975, 3, true, (now() at time zone 'utc'), 1704401707, 2);
UPDATE current_nodes SET latitude=388949735, longitude=-770376975, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401707, version=2 WHERE id=3165;
DELETE FROM current_node_tags WHERE node_id = 3165;
DELETE FROM node_tags WHERE node_id = 3165;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3165, 'hoot:id', '3165');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3165, 'hoot:id', '3165', 2);
/* modify node 3167*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3167, 388950504, -770375312, 3, true, (now() at time zone 'utc'), 1704401707, 2);
UPDATE current_nodes SET latitude=388950504, longitude=-770375312, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401707, version=2 WHERE id=3167;
DELETE FROM current_node_tags WHERE node_id = 3167;
DELETE FROM node_tags WHERE node_id = 3167;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3167, 'hoot:id', '3167');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3167, 'hoot:id', '3167', 2);
/* modify node 3169*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3169, 388951081, -770373750, 3, true, (now() at time zone 'utc'), 1704401707, 2);
UPDATE current_nodes SET latitude=388951081, longitude=-770373750, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401707, version=2 WHERE id=3169;
DELETE FROM current_node_tags WHERE node_id = 3169;
DELETE FROM node_tags WHERE node_id = 3169;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3169, 'hoot:id', '3169');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3169, 'hoot:id', '3169', 2);
/* modify node 3171*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3171, 388951178, -770373282, 3, true, (now() at time zone 'utc'), 1704401707, 2);
UPDATE current_nodes SET latitude=388951178, longitude=-770373282, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401707, version=2 WHERE id=3171;
DELETE FROM current_node_tags WHERE node_id = 3171;
DELETE FROM node_tags WHERE node_id = 3171;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3171, 'hoot:id', '3171');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3171, 'hoot:id', '3171', 2);
/* modify node 3173*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3173, 388951347, -770372462, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388951347, longitude=-770372462, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=3173;
DELETE FROM current_node_tags WHERE node_id = 3173;
DELETE FROM node_tags WHERE node_id = 3173;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3173, 'hoot:id', '3173');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3173, 'hoot:id', '3173', 2);
/* modify node 3175*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3175, 388951529, -770371471, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388951529, longitude=-770371471, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=3175;
DELETE FROM current_node_tags WHERE node_id = 3175;
DELETE FROM node_tags WHERE node_id = 3175;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3175, 'hoot:id', '3175');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3175, 'hoot:id', '3175', 2);
/* modify node 3177*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3177, 388951831, -770369787, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388951831, longitude=-770369787, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=3177;
DELETE FROM current_node_tags WHERE node_id = 3177;
DELETE FROM node_tags WHERE node_id = 3177;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3177, 'hoot:id', '3177');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3177, 'hoot:id', '3177', 2);
/* modify node 3179*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3179, 388951948, -770368786, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388951948, longitude=-770368786, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=3179;
DELETE FROM current_node_tags WHERE node_id = 3179;
DELETE FROM node_tags WHERE node_id = 3179;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3179, 'hoot:id', '3179');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3179, 'hoot:id', '3179', 2);
/* modify node 3181*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3181, 388951961, -770368556, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388951961, longitude=-770368556, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=3181;
DELETE FROM current_node_tags WHERE node_id = 3181;
DELETE FROM node_tags WHERE node_id = 3181;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3181, 'hoot:id', '3181');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3181, 'hoot:id', '3181', 2);
/* modify node 3183*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3183, 388951967, -770368441, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388951967, longitude=-770368441, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=3183;
DELETE FROM current_node_tags WHERE node_id = 3183;
DELETE FROM node_tags WHERE node_id = 3183;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3183, 'hoot:id', '3183');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3183, 'hoot:id', '3183', 2);
/* modify node 3233*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3233, 388952866, -770394662, 3, true, (now() at time zone 'utc'), 1704401707, 2);
UPDATE current_nodes SET latitude=388952866, longitude=-770394662, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401707, version=2 WHERE id=3233;
DELETE FROM current_node_tags WHERE node_id = 3233;
DELETE FROM node_tags WHERE node_id = 3233;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3233, 'hoot:id', '3233');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3233, 'hoot:id', '3233', 2);
/* modify node 3235*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3235, 388948249, -770394660, 3, true, (now() at time zone 'utc'), 1704401706, 2);
UPDATE current_nodes SET latitude=388948249, longitude=-770394660, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401706, version=2 WHERE id=3235;
DELETE FROM current_node_tags WHERE node_id = 3235;
DELETE FROM node_tags WHERE node_id = 3235;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3235, 'hoot:id', '3235');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3235, 'hoot:id', '3235', 2);
/* modify node 3237*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3237, 388945254, -770394694, 3, true, (now() at time zone 'utc'), 1704401706, 2);
UPDATE current_nodes SET latitude=388945254, longitude=-770394694, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401706, version=2 WHERE id=3237;
DELETE FROM current_node_tags WHERE node_id = 3237;
DELETE FROM node_tags WHERE node_id = 3237;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3237, 'hoot:id', '3237');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3237, 'hoot:id', '3237', 2);
/* modify node 3239*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3239, 388944218, -770394686, 3, true, (now() at time zone 'utc'), 1704401706, 2);
UPDATE current_nodes SET latitude=388944218, longitude=-770394686, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401706, version=2 WHERE id=3239;
DELETE FROM current_node_tags WHERE node_id = 3239;
DELETE FROM node_tags WHERE node_id = 3239;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3239, 'hoot:id', '3239');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3239, 'hoot:id', '3239', 2);
/* modify node 3241*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3241, 388943400, -770394679, 3, true, (now() at time zone 'utc'), 1704401706, 2);
UPDATE current_nodes SET latitude=388943400, longitude=-770394679, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401706, version=2 WHERE id=3241;
DELETE FROM current_node_tags WHERE node_id = 3241;
DELETE FROM node_tags WHERE node_id = 3241;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3241, 'hoot:id', '3241');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3241, 'hoot:id', '3241', 2);
/* modify node 3243*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3243, 388942364, -770394671, 3, true, (now() at time zone 'utc'), 1704401706, 2);
UPDATE current_nodes SET latitude=388942364, longitude=-770394671, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401706, version=2 WHERE id=3243;
DELETE FROM current_node_tags WHERE node_id = 3243;
DELETE FROM node_tags WHERE node_id = 3243;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3243, 'hoot:id', '3243');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3243, 'hoot:id', '3243', 2);
/* modify node 3245*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3245, 388942182, -770394667, 3, true, (now() at time zone 'utc'), 1704401706, 2);
UPDATE current_nodes SET latitude=388942182, longitude=-770394667, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401706, version=2 WHERE id=3245;
DELETE FROM current_node_tags WHERE node_id = 3245;
DELETE FROM node_tags WHERE node_id = 3245;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3245, 'hoot:id', '3245');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3245, 'hoot:id', '3245', 2);
/* modify node 3247*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3247, 388939243, -770394658, 3, true, (now() at time zone 'utc'), 1704401706, 2);
UPDATE current_nodes SET latitude=388939243, longitude=-770394658, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401706, version=2 WHERE id=3247;
DELETE FROM current_node_tags WHERE node_id = 3247;
DELETE FROM node_tags WHERE node_id = 3247;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3247, 'hoot:id', '3247');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3247, 'hoot:id', '3247', 2);
/* modify node 3249*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3249, 388936397, -770394672, 3, true, (now() at time zone 'utc'), 1704401706, 2);
UPDATE current_nodes SET latitude=388936397, longitude=-770394672, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401706, version=2 WHERE id=3249;
DELETE FROM current_node_tags WHERE node_id = 3249;
DELETE FROM node_tags WHERE node_id = 3249;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3249, 'hoot:id', '3249');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3249, 'hoot:id', '3249', 2);
/* modify node 3251*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3251, 388936137, -770394674, 3, true, (now() at time zone 'utc'), 1704401706, 2);
UPDATE current_nodes SET latitude=388936137, longitude=-770394674, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401706, version=2 WHERE id=3251;
DELETE FROM current_node_tags WHERE node_id = 3251;
DELETE FROM node_tags WHERE node_id = 3251;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3251, 'hoot:id', '3251');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3251, 'hoot:id', '3251', 2);
/* modify node 3253*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3253, 388935304, -770394682, 3, true, (now() at time zone 'utc'), 1704401706, 2);
UPDATE current_nodes SET latitude=388935304, longitude=-770394682, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401706, version=2 WHERE id=3253;
DELETE FROM current_node_tags WHERE node_id = 3253;
DELETE FROM node_tags WHERE node_id = 3253;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3253, 'hoot:id', '3253');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3253, 'hoot:id', '3253', 2);
/* modify node 3255*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3255, 388934551, -770394688, 3, true, (now() at time zone 'utc'), 1704401706, 2);
UPDATE current_nodes SET latitude=388934551, longitude=-770394688, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401706, version=2 WHERE id=3255;
DELETE FROM current_node_tags WHERE node_id = 3255;
DELETE FROM node_tags WHERE node_id = 3255;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3255, 'hoot:id', '3255');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3255, 'hoot:id', '3255', 2);
/* modify node 3257*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3257, 388933677, -770394696, 3, true, (now() at time zone 'utc'), 1704401706, 2);
UPDATE current_nodes SET latitude=388933677, longitude=-770394696, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401706, version=2 WHERE id=3257;
DELETE FROM current_node_tags WHERE node_id = 3257;
DELETE FROM node_tags WHERE node_id = 3257;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3257, 'hoot:id', '3257');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3257, 'hoot:id', '3257', 2);
/* modify node 3259*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3259, 388923420, -770394787, 3, true, (now() at time zone 'utc'), 1704401706, 2);
UPDATE current_nodes SET latitude=388923420, longitude=-770394787, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401706, version=2 WHERE id=3259;
DELETE FROM current_node_tags WHERE node_id = 3259;
DELETE FROM node_tags WHERE node_id = 3259;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3259, 'hoot:id', '3259');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3259, 'hoot:id', '3259', 2);
/* modify node 3261*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3261, 388922586, -770394794, 3, true, (now() at time zone 'utc'), 1704401706, 2);
UPDATE current_nodes SET latitude=388922586, longitude=-770394794, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401706, version=2 WHERE id=3261;
DELETE FROM current_node_tags WHERE node_id = 3261;
DELETE FROM node_tags WHERE node_id = 3261;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3261, 'hoot:id', '3261');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3261, 'hoot:id', '3261', 2);
/* modify node 3263*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3263, 388920997, -770394809, 3, true, (now() at time zone 'utc'), 1704401535, 2);
UPDATE current_nodes SET latitude=388920997, longitude=-770394809, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401535, version=2 WHERE id=3263;
DELETE FROM current_node_tags WHERE node_id = 3263;
DELETE FROM node_tags WHERE node_id = 3263;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3263, 'hoot:id', '3263');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3263, 'hoot:id', '3263', 2);
/* modify node 3265*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3265, 388919246, -770394779, 3, true, (now() at time zone 'utc'), 1704401535, 2);
UPDATE current_nodes SET latitude=388919246, longitude=-770394779, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401535, version=2 WHERE id=3265;
DELETE FROM current_node_tags WHERE node_id = 3265;
DELETE FROM node_tags WHERE node_id = 3265;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3265, 'hoot:id', '3265');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3265, 'hoot:id', '3265', 2);
/* modify node 3267*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3267, 388919061, -770394779, 3, true, (now() at time zone 'utc'), 1704401535, 2);
UPDATE current_nodes SET latitude=388919061, longitude=-770394779, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401535, version=2 WHERE id=3267;
DELETE FROM current_node_tags WHERE node_id = 3267;
DELETE FROM node_tags WHERE node_id = 3267;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3267, 'hoot:id', '3267');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3267, 'hoot:id', '3267', 2);
/* modify node 3269*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3269, 388909732, -770394759, 3, true, (now() at time zone 'utc'), 1704401535, 2);
UPDATE current_nodes SET latitude=388909732, longitude=-770394759, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401535, version=2 WHERE id=3269;
DELETE FROM current_node_tags WHERE node_id = 3269;
DELETE FROM node_tags WHERE node_id = 3269;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3269, 'hoot:id', '3269');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3269, 'hoot:id', '3269', 2);
/* modify node 3271*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3271, 388901537, -770394792, 3, true, (now() at time zone 'utc'), 1704401535, 2);
UPDATE current_nodes SET latitude=388901537, longitude=-770394792, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401535, version=2 WHERE id=3271;
DELETE FROM current_node_tags WHERE node_id = 3271;
DELETE FROM node_tags WHERE node_id = 3271;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3271, 'hoot:id', '3271');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3271, 'hoot:id', '3271', 2);
/* modify node 3273*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3273, 388900553, -770394794, 3, true, (now() at time zone 'utc'), 1704401535, 2);
UPDATE current_nodes SET latitude=388900553, longitude=-770394794, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401535, version=2 WHERE id=3273;
DELETE FROM current_node_tags WHERE node_id = 3273;
DELETE FROM node_tags WHERE node_id = 3273;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3273, 'hoot:id', '3273');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3273, 'hoot:id', '3273', 2);
/* modify node 3275*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3275, 388899568, -770394797, 3, true, (now() at time zone 'utc'), 1704401535, 2);
UPDATE current_nodes SET latitude=388899568, longitude=-770394797, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401535, version=2 WHERE id=3275;
DELETE FROM current_node_tags WHERE node_id = 3275;
DELETE FROM node_tags WHERE node_id = 3275;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3275, 'hoot:id', '3275');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3275, 'hoot:id', '3275', 2);
/* modify node 3277*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3277, 388898790, -770394801, 3, true, (now() at time zone 'utc'), 1704401535, 2);
UPDATE current_nodes SET latitude=388898790, longitude=-770394801, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401535, version=2 WHERE id=3277;
DELETE FROM current_node_tags WHERE node_id = 3277;
DELETE FROM node_tags WHERE node_id = 3277;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3277, 'hoot:id', '3277');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3277, 'hoot:id', '3277', 2);
/* modify node 3547*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3547, 388964016, -770394614, 3, true, (now() at time zone 'utc'), 1704401707, 2);
UPDATE current_nodes SET latitude=388964016, longitude=-770394614, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401707, version=2 WHERE id=3547;
DELETE FROM current_node_tags WHERE node_id = 3547;
DELETE FROM node_tags WHERE node_id = 3547;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3547, 'hoot:id', '3547');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3547, 'hoot:id', '3547', 2);
/* modify node 3549*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3549, 388962890, -770394617, 3, true, (now() at time zone 'utc'), 1704401707, 2);
UPDATE current_nodes SET latitude=388962890, longitude=-770394617, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401707, version=2 WHERE id=3549;
DELETE FROM current_node_tags WHERE node_id = 3549;
DELETE FROM node_tags WHERE node_id = 3549;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3549, 'hoot:id', '3549');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3549, 'hoot:id', '3549', 2);
/* modify node 3551*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3551, 388962776, -770394617, 3, true, (now() at time zone 'utc'), 1704401707, 2);
UPDATE current_nodes SET latitude=388962776, longitude=-770394617, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401707, version=2 WHERE id=3551;
DELETE FROM current_node_tags WHERE node_id = 3551;
DELETE FROM node_tags WHERE node_id = 3551;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3551, 'hoot:id', '3551');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3551, 'hoot:id', '3551', 2);
/* modify node 3553*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3553, 388958448, -770394572, 3, true, (now() at time zone 'utc'), 1704401707, 2);
UPDATE current_nodes SET latitude=388958448, longitude=-770394572, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401707, version=2 WHERE id=3553;
DELETE FROM current_node_tags WHERE node_id = 3553;
DELETE FROM node_tags WHERE node_id = 3553;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3553, 'hoot:id', '3553');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3553, 'hoot:id', '3553', 2);
/* modify node 3554*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3554, 388955444, -770394599, 3, true, (now() at time zone 'utc'), 1704401707, 2);
UPDATE current_nodes SET latitude=388955444, longitude=-770394599, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401707, version=2 WHERE id=3554;
DELETE FROM current_node_tags WHERE node_id = 3554;
DELETE FROM node_tags WHERE node_id = 3554;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3554, 'hoot:id', '3554');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3554, 'hoot:id', '3554', 2);
/* modify node 3555*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3555, 388954987, -770394609, 3, true, (now() at time zone 'utc'), 1704401707, 2);
UPDATE current_nodes SET latitude=388954987, longitude=-770394609, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401707, version=2 WHERE id=3555;
DELETE FROM current_node_tags WHERE node_id = 3555;
DELETE FROM node_tags WHERE node_id = 3555;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3555, 'hoot:id', '3555');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3555, 'hoot:id', '3555', 2);
/* modify node 3556*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3556, 388953966, -770394634, 3, true, (now() at time zone 'utc'), 1704401707, 2);
UPDATE current_nodes SET latitude=388953966, longitude=-770394634, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401707, version=2 WHERE id=3556;
DELETE FROM current_node_tags WHERE node_id = 3556;
DELETE FROM node_tags WHERE node_id = 3556;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3556, 'hoot:id', '3556');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3556, 'hoot:id', '3556', 2);
/* modify node 3557*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3557, 388954828, -770336214, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388954828, longitude=-770336214, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=3557;
DELETE FROM current_node_tags WHERE node_id = 3557;
DELETE FROM node_tags WHERE node_id = 3557;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3557, 'hoot:id', '3557');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3557, 'hoot:id', '3557', 2);
/* modify node 3558*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3558, 388955055, -770334080, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388955055, longitude=-770334080, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=3558;
DELETE FROM current_node_tags WHERE node_id = 3558;
DELETE FROM node_tags WHERE node_id = 3558;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3558, 'hoot:id', '3558');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3558, 'hoot:id', '3558', 2);
/* modify node 3559*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3559, 388955063, -770330751, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388955063, longitude=-770330751, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=3559;
DELETE FROM current_node_tags WHERE node_id = 3559;
DELETE FROM node_tags WHERE node_id = 3559;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3559, 'hoot:id', '3559');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3559, 'hoot:id', '3559', 2);
/* modify node 3560*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3560, 388955041, -770325569, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388955041, longitude=-770325569, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=3560;
DELETE FROM current_node_tags WHERE node_id = 3560;
DELETE FROM node_tags WHERE node_id = 3560;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3560, 'hoot:id', '3560');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3560, 'hoot:id', '3560', 2);
/* modify node 3622*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3622, 388958157, -770351001, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388958157, longitude=-770351001, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=3622;
DELETE FROM current_node_tags WHERE node_id = 3622;
DELETE FROM node_tags WHERE node_id = 3622;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3622, 'hoot:id', '3622');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3622, 'hoot:id', '3622', 2);
/* modify node 3625*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3625, 388955768, -770350599, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388955768, longitude=-770350599, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=3625;
DELETE FROM current_node_tags WHERE node_id = 3625;
DELETE FROM node_tags WHERE node_id = 3625;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3625, 'hoot:id', '3625');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3625, 'hoot:id', '3625', 2);
/* modify node 3628*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3628, 388954409, -770350236, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388954409, longitude=-770350236, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=3628;
DELETE FROM current_node_tags WHERE node_id = 3628;
DELETE FROM node_tags WHERE node_id = 3628;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3628, 'hoot:id', '3628');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3628, 'hoot:id', '3628', 2);
/* modify node 3723*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3723, 388920998, -770406759, 3, true, (now() at time zone 'utc'), 1704401535, 2);
UPDATE current_nodes SET latitude=388920998, longitude=-770406759, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401535, version=2 WHERE id=3723;
DELETE FROM current_node_tags WHERE node_id = 3723;
DELETE FROM node_tags WHERE node_id = 3723;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3723, 'hoot:id', '3723');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3723, 'hoot:id', '3723', 2);
/* modify node 3724*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3724, 388920997, -770406004, 3, true, (now() at time zone 'utc'), 1704401535, 2);
UPDATE current_nodes SET latitude=388920997, longitude=-770406004, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401535, version=2 WHERE id=3724;
DELETE FROM current_node_tags WHERE node_id = 3724;
DELETE FROM node_tags WHERE node_id = 3724;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3724, 'hoot:id', '3724');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3724, 'hoot:id', '3724', 2);
/* modify node 3725*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3725, 388921004, -770396835, 3, true, (now() at time zone 'utc'), 1704401535, 2);
UPDATE current_nodes SET latitude=388921004, longitude=-770396835, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401535, version=2 WHERE id=3725;
DELETE FROM current_node_tags WHERE node_id = 3725;
DELETE FROM node_tags WHERE node_id = 3725;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3725, 'hoot:id', '3725');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3725, 'hoot:id', '3725', 2);
/* modify node 3726*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3726, 388921003, -770396366, 3, true, (now() at time zone 'utc'), 1704401535, 2);
UPDATE current_nodes SET latitude=388921003, longitude=-770396366, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401535, version=2 WHERE id=3726;
DELETE FROM current_node_tags WHERE node_id = 3726;
DELETE FROM node_tags WHERE node_id = 3726;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3726, 'hoot:id', '3726');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3726, 'hoot:id', '3726', 2);
/* modify node 3727*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3727, 388920987, -770391229, 3, true, (now() at time zone 'utc'), 1704401535, 2);
UPDATE current_nodes SET latitude=388920987, longitude=-770391229, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401535, version=2 WHERE id=3727;
DELETE FROM current_node_tags WHERE node_id = 3727;
DELETE FROM node_tags WHERE node_id = 3727;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3727, 'hoot:id', '3727');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3727, 'hoot:id', '3727', 2);
/* modify node 3728*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3728, 388920984, -770389101, 3, true, (now() at time zone 'utc'), 1704401535, 2);
UPDATE current_nodes SET latitude=388920984, longitude=-770389101, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401535, version=2 WHERE id=3728;
DELETE FROM current_node_tags WHERE node_id = 3728;
DELETE FROM node_tags WHERE node_id = 3728;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3728, 'hoot:id', '3728');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3728, 'hoot:id', '3728', 2);
/* modify node 3729*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3729, 388920985, -770384868, 3, true, (now() at time zone 'utc'), 1704401535, 2);
UPDATE current_nodes SET latitude=388920985, longitude=-770384868, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401535, version=2 WHERE id=3729;
DELETE FROM current_node_tags WHERE node_id = 3729;
DELETE FROM node_tags WHERE node_id = 3729;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3729, 'hoot:id', '3729');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3729, 'hoot:id', '3729', 2);
/* modify node 3730*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3730, 388920977, -770380054, 3, true, (now() at time zone 'utc'), 1704401535, 2);
UPDATE current_nodes SET latitude=388920977, longitude=-770380054, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401535, version=2 WHERE id=3730;
DELETE FROM current_node_tags WHERE node_id = 3730;
DELETE FROM node_tags WHERE node_id = 3730;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3730, 'hoot:id', '3730');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3730, 'hoot:id', '3730', 2);
/* modify node 3731*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3731, 388920970, -770376233, 3, true, (now() at time zone 'utc'), 1704401535, 2);
UPDATE current_nodes SET latitude=388920970, longitude=-770376233, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401535, version=2 WHERE id=3731;
DELETE FROM current_node_tags WHERE node_id = 3731;
DELETE FROM node_tags WHERE node_id = 3731;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3731, 'hoot:id', '3731');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3731, 'hoot:id', '3731', 2);
/* modify node 3732*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3732, 388920966, -770371314, 3, true, (now() at time zone 'utc'), 1704401621, 2);
UPDATE current_nodes SET latitude=388920966, longitude=-770371314, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401621, version=2 WHERE id=3732;
DELETE FROM current_node_tags WHERE node_id = 3732;
DELETE FROM node_tags WHERE node_id = 3732;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3732, 'hoot:id', '3732');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3732, 'hoot:id', '3732', 2);
/* modify node 3733*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3733, 388920966, -770367335, 3, true, (now() at time zone 'utc'), 1704401621, 2);
UPDATE current_nodes SET latitude=388920966, longitude=-770367335, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401621, version=2 WHERE id=3733;
DELETE FROM current_node_tags WHERE node_id = 3733;
DELETE FROM node_tags WHERE node_id = 3733;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3733, 'hoot:id', '3733');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3733, 'hoot:id', '3733', 2);
/* modify node 3734*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3734, 388920966, -770367105, 3, true, (now() at time zone 'utc'), 1704401621, 2);
UPDATE current_nodes SET latitude=388920966, longitude=-770367105, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401621, version=2 WHERE id=3734;
DELETE FROM current_node_tags WHERE node_id = 3734;
DELETE FROM node_tags WHERE node_id = 3734;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3734, 'hoot:id', '3734');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3734, 'hoot:id', '3734', 2);
/* modify node 3735*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3735, 388920964, -770363974, 3, true, (now() at time zone 'utc'), 1704401621, 2);
UPDATE current_nodes SET latitude=388920964, longitude=-770363974, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401621, version=2 WHERE id=3735;
DELETE FROM current_node_tags WHERE node_id = 3735;
DELETE FROM node_tags WHERE node_id = 3735;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3735, 'hoot:id', '3735');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3735, 'hoot:id', '3735', 2);
/* modify node 3736*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3736, 388920963, -770362713, 3, true, (now() at time zone 'utc'), 1704401621, 2);
UPDATE current_nodes SET latitude=388920963, longitude=-770362713, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401621, version=2 WHERE id=3736;
DELETE FROM current_node_tags WHERE node_id = 3736;
DELETE FROM node_tags WHERE node_id = 3736;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3736, 'hoot:id', '3736');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3736, 'hoot:id', '3736', 2);
/* modify node 3737*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3737, 388920951, -770358681, 3, true, (now() at time zone 'utc'), 1704401621, 2);
UPDATE current_nodes SET latitude=388920951, longitude=-770358681, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401621, version=2 WHERE id=3737;
DELETE FROM current_node_tags WHERE node_id = 3737;
DELETE FROM node_tags WHERE node_id = 3737;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3737, 'hoot:id', '3737');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3737, 'hoot:id', '3737', 2);
/* modify node 3738*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3738, 388920932, -770355016, 3, true, (now() at time zone 'utc'), 1704401621, 2);
UPDATE current_nodes SET latitude=388920932, longitude=-770355016, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401621, version=2 WHERE id=3738;
DELETE FROM current_node_tags WHERE node_id = 3738;
DELETE FROM node_tags WHERE node_id = 3738;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3738, 'hoot:id', '3738');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3738, 'hoot:id', '3738', 2);
/* modify node 3739*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3739, 388920935, -770344439, 3, true, (now() at time zone 'utc'), 1704401621, 2);
UPDATE current_nodes SET latitude=388920935, longitude=-770344439, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401621, version=2 WHERE id=3739;
DELETE FROM current_node_tags WHERE node_id = 3739;
DELETE FROM node_tags WHERE node_id = 3739;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3739, 'hoot:id', '3739');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3739, 'hoot:id', '3739', 2);
/* modify node 3740*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3740, 388920933, -770342148, 3, true, (now() at time zone 'utc'), 1704401621, 2);
UPDATE current_nodes SET latitude=388920933, longitude=-770342148, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401621, version=2 WHERE id=3740;
DELETE FROM current_node_tags WHERE node_id = 3740;
DELETE FROM node_tags WHERE node_id = 3740;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3740, 'hoot:id', '3740');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3740, 'hoot:id', '3740', 2);
/* modify node 3741*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3741, 388920931, -770339251, 3, true, (now() at time zone 'utc'), 1704401621, 2);
UPDATE current_nodes SET latitude=388920931, longitude=-770339251, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401621, version=2 WHERE id=3741;
DELETE FROM current_node_tags WHERE node_id = 3741;
DELETE FROM node_tags WHERE node_id = 3741;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3741, 'hoot:id', '3741');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3741, 'hoot:id', '3741', 2);
/* modify node 3743*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3743, 388920930, -770338928, 3, true, (now() at time zone 'utc'), 1704401621, 2);
UPDATE current_nodes SET latitude=388920930, longitude=-770338928, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401621, version=2 WHERE id=3743;
DELETE FROM current_node_tags WHERE node_id = 3743;
DELETE FROM node_tags WHERE node_id = 3743;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3743, 'hoot:id', '3743');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3743, 'hoot:id', '3743', 2);
/* modify node 3745*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3745, 388920928, -770338771, 3, true, (now() at time zone 'utc'), 1704401621, 2);
UPDATE current_nodes SET latitude=388920928, longitude=-770338771, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401621, version=2 WHERE id=3745;
DELETE FROM current_node_tags WHERE node_id = 3745;
DELETE FROM node_tags WHERE node_id = 3745;
INSERT INTO current_node_tags (node_id, k, v) VALUES (3745, 'hoot:id', '3745');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3745, 'hoot:id', '3745', 2);
/* modify node 4251*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (4251, 388954986, -770319550, 3, true, (now() at time zone 'utc'), 1704401793, 2);
UPDATE current_nodes SET latitude=388954986, longitude=-770319550, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401793, version=2 WHERE id=4251;
DELETE FROM current_node_tags WHERE node_id = 4251;
DELETE FROM node_tags WHERE node_id = 4251;
INSERT INTO current_node_tags (node_id, k, v) VALUES (4251, 'hoot:id', '4251');
INSERT INTO node_tags (node_id, k, v, version) VALUES (4251, 'hoot:id', '4251', 2);
/* modify node 4254*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (4254, 388920932, -770319637, 3, true, (now() at time zone 'utc'), 1704401621, 2);
UPDATE current_nodes SET latitude=388920932, longitude=-770319637, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1704401621, version=2 WHERE id=4254;
DELETE FROM current_node_tags WHERE node_id = 4254;
DELETE FROM node_tags WHERE node_id = 4254;
INSERT INTO current_node_tags (node_id, k, v) VALUES (4254, 'hoot:id', '4254');
INSERT INTO node_tags (node_id, k, v, version) VALUES (4254, 'hoot:id', '4254', 2);
/* create node 7499*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7499, 388920987, -770391229, 3, true, (now() at time zone 'utc'), 1704401535, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7499, 388920987, -770391229, 3, true, (now() at time zone 'utc'), 1704401535, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (7499, 'hoot:id', '-255');
INSERT INTO node_tags (node_id, k, v, version) VALUES (7499, 'hoot:id', '-255', 1);
/* create node 7500*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7500, 388920931, -770339249, 3, true, (now() at time zone 'utc'), 1704401621, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7500, 388920931, -770339249, 3, true, (now() at time zone 'utc'), 1704401621, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (7500, 'hoot:id', '-253');
INSERT INTO node_tags (node_id, k, v, version) VALUES (7500, 'hoot:id', '-253', 1);
/* create node 7501*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7501, 388953918, -770384009, 3, true, (now() at time zone 'utc'), 1704401707, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7501, 388953918, -770384009, 3, true, (now() at time zone 'utc'), 1704401707, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (7501, 'hoot:id', '-246');
INSERT INTO node_tags (node_id, k, v, version) VALUES (7501, 'hoot:id', '-246', 1);
/* create node 7502*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7502, 388953915, -770377119, 3, true, (now() at time zone 'utc'), 1704401707, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7502, 388953915, -770377119, 3, true, (now() at time zone 'utc'), 1704401707, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (7502, 'hoot:id', '-245');
INSERT INTO node_tags (node_id, k, v, version) VALUES (7502, 'hoot:id', '-245', 1);
/* create node 7503*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7503, 388954753, -770338679, 3, true, (now() at time zone 'utc'), 1704401793, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7503, 388954753, -770338679, 3, true, (now() at time zone 'utc'), 1704401793, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (7503, 'hoot:id', '-242');
INSERT INTO node_tags (node_id, k, v, version) VALUES (7503, 'hoot:id', '-242', 1);
/* create node 7504*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7504, 388943750, -770347338, 3, true, (now() at time zone 'utc'), 1704401792, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7504, 388943750, -770347338, 3, true, (now() at time zone 'utc'), 1704401792, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (7504, 'hoot:id', '-240');
INSERT INTO node_tags (node_id, k, v, version) VALUES (7504, 'hoot:id', '-240', 1);
/* create node 7505*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7505, 388932650, -770348667, 3, true, (now() at time zone 'utc'), 1704401792, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7505, 388932650, -770348667, 3, true, (now() at time zone 'utc'), 1704401792, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (7505, 'hoot:id', '-239');
INSERT INTO node_tags (node_id, k, v, version) VALUES (7505, 'hoot:id', '-239', 1);
/* create node 7506*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7506, 388932771, -770382196, 3, true, (now() at time zone 'utc'), 1704401706, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7506, 388932771, -770382196, 3, true, (now() at time zone 'utc'), 1704401706, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (7506, 'hoot:id', '-236');
INSERT INTO node_tags (node_id, k, v, version) VALUES (7506, 'hoot:id', '-236', 1);
/* create node 7507*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7507, 388949632, -770377191, 3, true, (now() at time zone 'utc'), 1704401707, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7507, 388949632, -770377191, 3, true, (now() at time zone 'utc'), 1704401707, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (7507, 'hoot:id', '-234');
INSERT INTO node_tags (node_id, k, v, version) VALUES (7507, 'hoot:id', '-234', 1);
/* create node 7508*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7508, 388957228, -770377183, 3, true, (now() at time zone 'utc'), 1704401707, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7508, 388957228, -770377183, 3, true, (now() at time zone 'utc'), 1704401707, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (7508, 'hoot:id', '-231');
INSERT INTO node_tags (node_id, k, v, version) VALUES (7508, 'hoot:id', '-231', 1);
/* create node 7509*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7509, 388957693, -770353234, 3, true, (now() at time zone 'utc'), 1704401793, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7509, 388957693, -770353234, 3, true, (now() at time zone 'utc'), 1704401793, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (7509, 'hoot:id', '-229');
INSERT INTO node_tags (node_id, k, v, version) VALUES (7509, 'hoot:id', '-229', 1);
/* create node 7512*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7512, 388921830, -770390989, 3, true, (now() at time zone 'utc'), 1704401535, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7512, 388921830, -770390989, 3, true, (now() at time zone 'utc'), 1704401535, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (7512, 'hoot:id', '7160');
INSERT INTO node_tags (node_id, k, v, version) VALUES (7512, 'hoot:id', '7160', 1);
/* create node 7513*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7513, 388922160, -770390789, 3, true, (now() at time zone 'utc'), 1704401706, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7513, 388922160, -770390789, 3, true, (now() at time zone 'utc'), 1704401706, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (7513, 'hoot:id', '7161');
INSERT INTO node_tags (node_id, k, v, version) VALUES (7513, 'hoot:id', '7161', 1);
/* create node 7514*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7514, 388922430, -770390559, 3, true, (now() at time zone 'utc'), 1704401706, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7514, 388922430, -770390559, 3, true, (now() at time zone 'utc'), 1704401706, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (7514, 'hoot:id', '7162');
INSERT INTO node_tags (node_id, k, v, version) VALUES (7514, 'hoot:id', '7162', 1);
/* create node 7515*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7515, 388922600, -770390359, 3, true, (now() at time zone 'utc'), 1704401706, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7515, 388922600, -770390359, 3, true, (now() at time zone 'utc'), 1704401706, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (7515, 'hoot:id', '7163');
INSERT INTO node_tags (node_id, k, v, version) VALUES (7515, 'hoot:id', '7163', 1);
/* create node 7516*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7516, 388922800, -770390099, 3, true, (now() at time zone 'utc'), 1704401706, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7516, 388922800, -770390099, 3, true, (now() at time zone 'utc'), 1704401706, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (7516, 'hoot:id', '7164');
INSERT INTO node_tags (node_id, k, v, version) VALUES (7516, 'hoot:id', '7164', 1);
/* create node 7517*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7517, 388923100, -770389479, 3, true, (now() at time zone 'utc'), 1704401706, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7517, 388923100, -770389479, 3, true, (now() at time zone 'utc'), 1704401706, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (7517, 'hoot:id', '7165');
INSERT INTO node_tags (node_id, k, v, version) VALUES (7517, 'hoot:id', '7165', 1);
/* create node 7518*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7518, 388923580, -770388699, 3, true, (now() at time zone 'utc'), 1704401706, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7518, 388923580, -770388699, 3, true, (now() at time zone 'utc'), 1704401706, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (7518, 'hoot:id', '7166');
INSERT INTO node_tags (node_id, k, v, version) VALUES (7518, 'hoot:id', '7166', 1);
/* create node 7519*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7519, 388924230, -770387599, 3, true, (now() at time zone 'utc'), 1704401706, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7519, 388924230, -770387599, 3, true, (now() at time zone 'utc'), 1704401706, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (7519, 'hoot:id', '7167');
INSERT INTO node_tags (node_id, k, v, version) VALUES (7519, 'hoot:id', '7167', 1);
/* create node 7520*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7520, 388924550, -770387149, 3, true, (now() at time zone 'utc'), 1704401706, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7520, 388924550, -770387149, 3, true, (now() at time zone 'utc'), 1704401706, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (7520, 'hoot:id', '7168');
INSERT INTO node_tags (node_id, k, v, version) VALUES (7520, 'hoot:id', '7168', 1);
/* create node 7521*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7521, 388925210, -770386439, 3, true, (now() at time zone 'utc'), 1704401706, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7521, 388925210, -770386439, 3, true, (now() at time zone 'utc'), 1704401706, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (7521, 'hoot:id', '7169');
INSERT INTO node_tags (node_id, k, v, version) VALUES (7521, 'hoot:id', '7169', 1);
/* create node 7522*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7522, 388926760, -770384959, 3, true, (now() at time zone 'utc'), 1704401706, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7522, 388926760, -770384959, 3, true, (now() at time zone 'utc'), 1704401706, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (7522, 'hoot:id', '7170');
INSERT INTO node_tags (node_id, k, v, version) VALUES (7522, 'hoot:id', '7170', 1);
/* create node 7523*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7523, 388927690, -770384339, 3, true, (now() at time zone 'utc'), 1704401706, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7523, 388927690, -770384339, 3, true, (now() at time zone 'utc'), 1704401706, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (7523, 'hoot:id', '7171');
INSERT INTO node_tags (node_id, k, v, version) VALUES (7523, 'hoot:id', '7171', 1);
/* create node 7524*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7524, 388928370, -770383959, 3, true, (now() at time zone 'utc'), 1704401706, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7524, 388928370, -770383959, 3, true, (now() at time zone 'utc'), 1704401706, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (7524, 'hoot:id', '7172');
INSERT INTO node_tags (node_id, k, v, version) VALUES (7524, 'hoot:id', '7172', 1);
/* create node 7525*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7525, 388929020, -770383689, 3, true, (now() at time zone 'utc'), 1704401706, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7525, 388929020, -770383689, 3, true, (now() at time zone 'utc'), 1704401706, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (7525, 'hoot:id', '7173');
INSERT INTO node_tags (node_id, k, v, version) VALUES (7525, 'hoot:id', '7173', 1);
/* create node 7526*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7526, 388930140, -770383289, 3, true, (now() at time zone 'utc'), 1704401706, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7526, 388930140, -770383289, 3, true, (now() at time zone 'utc'), 1704401706, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (7526, 'hoot:id', '7174');
INSERT INTO node_tags (node_id, k, v, version) VALUES (7526, 'hoot:id', '7174', 1);
/* create node 7527*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7527, 388931330, -770382849, 3, true, (now() at time zone 'utc'), 1704401706, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7527, 388931330, -770382849, 3, true, (now() at time zone 'utc'), 1704401706, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (7527, 'hoot:id', '7175');
INSERT INTO node_tags (node_id, k, v, version) VALUES (7527, 'hoot:id', '7175', 1);
/* create node 7528*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7528, 388931960, -770382519, 3, true, (now() at time zone 'utc'), 1704401706, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7528, 388931960, -770382519, 3, true, (now() at time zone 'utc'), 1704401706, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (7528, 'hoot:id', '7176');
INSERT INTO node_tags (node_id, k, v, version) VALUES (7528, 'hoot:id', '7176', 1);
/* create node 7529*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7529, 388921430, -770339749, 3, true, (now() at time zone 'utc'), 1704401621, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7529, 388921430, -770339749, 3, true, (now() at time zone 'utc'), 1704401621, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (7529, 'hoot:id', '6422');
INSERT INTO node_tags (node_id, k, v, version) VALUES (7529, 'hoot:id', '6422', 1);
/* create node 7530*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7530, 388923170, -770341889, 3, true, (now() at time zone 'utc'), 1704401792, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7530, 388923170, -770341889, 3, true, (now() at time zone 'utc'), 1704401792, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (7530, 'hoot:id', '6423');
INSERT INTO node_tags (node_id, k, v, version) VALUES (7530, 'hoot:id', '6423', 1);
/* create node 7531*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7531, 388924220, -770343379, 3, true, (now() at time zone 'utc'), 1704401792, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7531, 388924220, -770343379, 3, true, (now() at time zone 'utc'), 1704401792, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (7531, 'hoot:id', '6424');
INSERT INTO node_tags (node_id, k, v, version) VALUES (7531, 'hoot:id', '6424', 1);
/* create node 7532*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7532, 388924990, -770344269, 3, true, (now() at time zone 'utc'), 1704401792, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7532, 388924990, -770344269, 3, true, (now() at time zone 'utc'), 1704401792, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (7532, 'hoot:id', '6425');
INSERT INTO node_tags (node_id, k, v, version) VALUES (7532, 'hoot:id', '6425', 1);
/* create node 7533*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7533, 388925740, -770344989, 3, true, (now() at time zone 'utc'), 1704401792, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7533, 388925740, -770344989, 3, true, (now() at time zone 'utc'), 1704401792, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (7533, 'hoot:id', '6426');
INSERT INTO node_tags (node_id, k, v, version) VALUES (7533, 'hoot:id', '6426', 1);
/* create node 7534*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7534, 388926440, -770345509, 3, true, (now() at time zone 'utc'), 1704401792, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7534, 388926440, -770345509, 3, true, (now() at time zone 'utc'), 1704401792, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (7534, 'hoot:id', '6427');
INSERT INTO node_tags (node_id, k, v, version) VALUES (7534, 'hoot:id', '6427', 1);
/* create node 7535*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7535, 388927690, -770346329, 3, true, (now() at time zone 'utc'), 1704401792, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7535, 388927690, -770346329, 3, true, (now() at time zone 'utc'), 1704401792, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (7535, 'hoot:id', '6428');
INSERT INTO node_tags (node_id, k, v, version) VALUES (7535, 'hoot:id', '6428', 1);
/* create node 7536*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7536, 388928170, -770346629, 3, true, (now() at time zone 'utc'), 1704401792, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7536, 388928170, -770346629, 3, true, (now() at time zone 'utc'), 1704401792, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (7536, 'hoot:id', '6429');
INSERT INTO node_tags (node_id, k, v, version) VALUES (7536, 'hoot:id', '6429', 1);
/* create node 7537*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7537, 388929390, -770347109, 3, true, (now() at time zone 'utc'), 1704401792, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7537, 388929390, -770347109, 3, true, (now() at time zone 'utc'), 1704401792, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (7537, 'hoot:id', '6430');
INSERT INTO node_tags (node_id, k, v, version) VALUES (7537, 'hoot:id', '6430', 1);
/* create node 7615*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7615, 388955950, -770376549, 3, true, (now() at time zone 'utc'), 1704401707, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7615, 388955950, -770376549, 3, true, (now() at time zone 'utc'), 1704401707, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (7615, 'hoot:id', '5160');
INSERT INTO node_tags (node_id, k, v, version) VALUES (7615, 'hoot:id', '5160', 1);
/* create node 7616*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7616, 388955500, -770376419, 3, true, (now() at time zone 'utc'), 1704401707, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7616, 388955500, -770376419, 3, true, (now() at time zone 'utc'), 1704401707, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (7616, 'hoot:id', '5162');
INSERT INTO node_tags (node_id, k, v, version) VALUES (7616, 'hoot:id', '5162', 1);
/* create node 7617*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7617, 388955200, -770376379, 3, true, (now() at time zone 'utc'), 1704401707, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7617, 388955200, -770376379, 3, true, (now() at time zone 'utc'), 1704401707, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (7617, 'hoot:id', '5164');
INSERT INTO node_tags (node_id, k, v, version) VALUES (7617, 'hoot:id', '5164', 1);
/* create node 7618*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7618, 388954950, -770376389, 3, true, (now() at time zone 'utc'), 1704401707, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7618, 388954950, -770376389, 3, true, (now() at time zone 'utc'), 1704401707, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (7618, 'hoot:id', '5167');
INSERT INTO node_tags (node_id, k, v, version) VALUES (7618, 'hoot:id', '5167', 1);
/* create node 7619*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7619, 388954580, -770376569, 3, true, (now() at time zone 'utc'), 1704401707, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7619, 388954580, -770376569, 3, true, (now() at time zone 'utc'), 1704401707, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (7619, 'hoot:id', '5170');
INSERT INTO node_tags (node_id, k, v, version) VALUES (7619, 'hoot:id', '5170', 1);
/* create node 7639*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7639, 388954130, -770338939, 3, true, (now() at time zone 'utc'), 1704401793, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7639, 388954130, -770338939, 3, true, (now() at time zone 'utc'), 1704401793, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (7639, 'hoot:id', '4568');
INSERT INTO node_tags (node_id, k, v, version) VALUES (7639, 'hoot:id', '4568', 1);
/* create node 7640*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7640, 388953470, -770339679, 3, true, (now() at time zone 'utc'), 1704401793, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7640, 388953470, -770339679, 3, true, (now() at time zone 'utc'), 1704401793, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (7640, 'hoot:id', '4569');
INSERT INTO node_tags (node_id, k, v, version) VALUES (7640, 'hoot:id', '4569', 1);
/* create node 7641*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7641, 388952670, -770340889, 3, true, (now() at time zone 'utc'), 1704401793, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7641, 388952670, -770340889, 3, true, (now() at time zone 'utc'), 1704401793, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (7641, 'hoot:id', '4570');
INSERT INTO node_tags (node_id, k, v, version) VALUES (7641, 'hoot:id', '4570', 1);
/* create node 7642*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7642, 388952110, -770341679, 3, true, (now() at time zone 'utc'), 1704401793, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7642, 388952110, -770341679, 3, true, (now() at time zone 'utc'), 1704401793, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (7642, 'hoot:id', '4571');
INSERT INTO node_tags (node_id, k, v, version) VALUES (7642, 'hoot:id', '4571', 1);
/* create node 7643*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7643, 388951590, -770342339, 3, true, (now() at time zone 'utc'), 1704401793, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7643, 388951590, -770342339, 3, true, (now() at time zone 'utc'), 1704401793, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (7643, 'hoot:id', '4572');
INSERT INTO node_tags (node_id, k, v, version) VALUES (7643, 'hoot:id', '4572', 1);
/* create node 7644*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7644, 388951040, -770343009, 3, true, (now() at time zone 'utc'), 1704401793, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7644, 388951040, -770343009, 3, true, (now() at time zone 'utc'), 1704401793, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (7644, 'hoot:id', '4573');
INSERT INTO node_tags (node_id, k, v, version) VALUES (7644, 'hoot:id', '4573', 1);
/* create node 7645*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7645, 388950460, -770343639, 3, true, (now() at time zone 'utc'), 1704401793, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7645, 388950460, -770343639, 3, true, (now() at time zone 'utc'), 1704401793, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (7645, 'hoot:id', '4574');
INSERT INTO node_tags (node_id, k, v, version) VALUES (7645, 'hoot:id', '4574', 1);
/* create node 7646*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7646, 388950020, -770344029, 3, true, (now() at time zone 'utc'), 1704401793, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7646, 388950020, -770344029, 3, true, (now() at time zone 'utc'), 1704401793, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (7646, 'hoot:id', '4575');
INSERT INTO node_tags (node_id, k, v, version) VALUES (7646, 'hoot:id', '4575', 1);
/* create node 7647*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7647, 388949380, -770344549, 3, true, (now() at time zone 'utc'), 1704401792, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7647, 388949380, -770344549, 3, true, (now() at time zone 'utc'), 1704401792, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (7647, 'hoot:id', '4576');
INSERT INTO node_tags (node_id, k, v, version) VALUES (7647, 'hoot:id', '4576', 1);
/* create node 7648*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7648, 388948110, -770345249, 3, true, (now() at time zone 'utc'), 1704401792, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7648, 388948110, -770345249, 3, true, (now() at time zone 'utc'), 1704401792, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (7648, 'hoot:id', '4577');
INSERT INTO node_tags (node_id, k, v, version) VALUES (7648, 'hoot:id', '4577', 1);
/* create node 7649*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7649, 388946860, -770345799, 3, true, (now() at time zone 'utc'), 1704401792, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7649, 388946860, -770345799, 3, true, (now() at time zone 'utc'), 1704401792, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (7649, 'hoot:id', '4578');
INSERT INTO node_tags (node_id, k, v, version) VALUES (7649, 'hoot:id', '4578', 1);
/* create node 7650*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7650, 388945220, -770346549, 3, true, (now() at time zone 'utc'), 1704401792, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7650, 388945220, -770346549, 3, true, (now() at time zone 'utc'), 1704401792, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (7650, 'hoot:id', '4579');
INSERT INTO node_tags (node_id, k, v, version) VALUES (7650, 'hoot:id', '4579', 1);
/* modify way 4*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (4, 3, true, (now() at time zone 'utc'), 2);
UPDATE current_ways SET changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), version=2 WHERE id=4;
DELETE FROM current_way_tags WHERE way_id = 4;
DELETE FROM way_tags WHERE way_id = 4;
INSERT INTO current_way_tags (way_id, k, v) VALUES (4, 'accuracy', '5');
INSERT INTO way_tags (way_id, k, v, version) VALUES (4, 'accuracy', '5', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (4, 'hoot:id', '4');
INSERT INTO way_tags (way_id, k, v, version) VALUES (4, 'hoot:id', '4', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (4, 'alt_name', 'Constitution Ave NW;US Hwy 50');
INSERT INTO way_tags (way_id, k, v, version) VALUES (4, 'alt_name', 'Constitution Ave NW;US Hwy 50', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (4, 'name', 'CONSTITUTION AVE NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (4, 'name', 'CONSTITUTION AVE NW', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (4, 'highway', 'road');
INSERT INTO way_tags (way_id, k, v, version) VALUES (4, 'highway', 'road', 2);
DELETE FROM current_way_nodes WHERE way_id = 4;
DELETE FROM way_nodes WHERE way_id = 4;
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (4, 4254, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (4, 4254, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (4, 1676, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (4, 1676, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (4, 1675, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (4, 1675, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (4, 1673, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (4, 1673, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (4, 1671, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (4, 1671, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (4, 1669, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (4, 1669, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (4, 1667, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (4, 1667, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (4, 1950, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (4, 1950, 8);
/* modify way 27*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (27, 3, true, (now() at time zone 'utc'), 2);
UPDATE current_ways SET changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), version=2 WHERE id=27;
DELETE FROM current_way_tags WHERE way_id = 27;
DELETE FROM way_tags WHERE way_id = 27;
INSERT INTO current_way_tags (way_id, k, v) VALUES (27, 'accuracy', '5');
INSERT INTO way_tags (way_id, k, v, version) VALUES (27, 'accuracy', '5', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (27, 'hoot:id', '27');
INSERT INTO way_tags (way_id, k, v, version) VALUES (27, 'hoot:id', '27', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (27, 'alt_name', '17th St NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (27, 'alt_name', '17th St NW', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (27, 'name', '17TH ST NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (27, 'name', '17TH ST NW', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (27, 'highway', 'road');
INSERT INTO way_tags (way_id, k, v, version) VALUES (27, 'highway', 'road', 2);
DELETE FROM current_way_nodes WHERE way_id = 27;
DELETE FROM way_nodes WHERE way_id = 27;
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (27, 3556, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (27, 3556, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (27, 3555, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (27, 3555, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (27, 3554, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (27, 3554, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (27, 3553, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (27, 3553, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (27, 3551, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (27, 3551, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (27, 3549, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (27, 3549, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (27, 3547, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (27, 3547, 7);
/* modify way 43*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (43, 3, true, (now() at time zone 'utc'), 2);
UPDATE current_ways SET changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), version=2 WHERE id=43;
DELETE FROM current_way_tags WHERE way_id = 43;
DELETE FROM way_tags WHERE way_id = 43;
INSERT INTO current_way_tags (way_id, k, v) VALUES (43, 'accuracy', '5');
INSERT INTO way_tags (way_id, k, v, version) VALUES (43, 'accuracy', '5', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (43, 'hoot:id', '43');
INSERT INTO way_tags (way_id, k, v, version) VALUES (43, 'hoot:id', '43', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (43, 'alt_name', 'E St NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (43, 'alt_name', 'E St NW', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (43, 'name', 'E ST NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (43, 'name', 'E ST NW', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (43, 'highway', 'road');
INSERT INTO way_tags (way_id, k, v, version) VALUES (43, 'highway', 'road', 2);
DELETE FROM current_way_nodes WHERE way_id = 43;
DELETE FROM way_nodes WHERE way_id = 43;
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (43, 7503, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (43, 7503, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (43, 634, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (43, 634, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (43, 632, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (43, 632, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (43, 630, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (43, 630, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (43, 628, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (43, 628, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (43, 626, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (43, 626, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (43, 624, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (43, 624, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (43, 622, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (43, 622, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (43, 620, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (43, 620, 9);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (43, 3628, 1, 10);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (43, 3628, 10);
/* modify way 44*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (44, 3, true, (now() at time zone 'utc'), 2);
UPDATE current_ways SET changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), version=2 WHERE id=44;
DELETE FROM current_way_tags WHERE way_id = 44;
DELETE FROM way_tags WHERE way_id = 44;
INSERT INTO current_way_tags (way_id, k, v) VALUES (44, 'accuracy', '5');
INSERT INTO way_tags (way_id, k, v, version) VALUES (44, 'accuracy', '5', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (44, 'hoot:id', '44');
INSERT INTO way_tags (way_id, k, v, version) VALUES (44, 'hoot:id', '44', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (44, 'alt_name', 'E St NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (44, 'alt_name', 'E St NW', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (44, 'name', 'E ST NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (44, 'name', 'E ST NW', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (44, 'highway', 'road');
INSERT INTO way_tags (way_id, k, v, version) VALUES (44, 'highway', 'road', 2);
DELETE FROM current_way_nodes WHERE way_id = 44;
DELETE FROM way_nodes WHERE way_id = 44;
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (44, 7502, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (44, 7502, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (44, 606, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (44, 606, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (44, 604, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (44, 604, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (44, 602, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (44, 602, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (44, 600, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (44, 600, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (44, 7501, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (44, 7501, 6);
/* modify way 62*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (62, 3, true, (now() at time zone 'utc'), 2);
UPDATE current_ways SET changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), version=2 WHERE id=62;
DELETE FROM current_way_tags WHERE way_id = 62;
DELETE FROM way_tags WHERE way_id = 62;
INSERT INTO current_way_tags (way_id, k, v) VALUES (62, 'accuracy', '5');
INSERT INTO way_tags (way_id, k, v, version) VALUES (62, 'accuracy', '5', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (62, 'hoot:id', '62');
INSERT INTO way_tags (way_id, k, v, version) VALUES (62, 'hoot:id', '62', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (62, 'alt_name', '17th St NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (62, 'alt_name', '17th St NW', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (62, 'name', '17TH ST NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (62, 'name', '17TH ST NW', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (62, 'highway', 'road');
INSERT INTO way_tags (way_id, k, v, version) VALUES (62, 'highway', 'road', 2);
DELETE FROM current_way_nodes WHERE way_id = 62;
DELETE FROM way_nodes WHERE way_id = 62;
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (62, 3277, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (62, 3277, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (62, 3275, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (62, 3275, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (62, 3273, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (62, 3273, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (62, 3271, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (62, 3271, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (62, 3269, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (62, 3269, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (62, 3267, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (62, 3267, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (62, 3265, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (62, 3265, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (62, 3263, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (62, 3263, 8);
/* modify way 63*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (63, 3, true, (now() at time zone 'utc'), 2);
UPDATE current_ways SET changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), version=2 WHERE id=63;
DELETE FROM current_way_tags WHERE way_id = 63;
DELETE FROM way_tags WHERE way_id = 63;
INSERT INTO current_way_tags (way_id, k, v) VALUES (63, 'accuracy', '5');
INSERT INTO way_tags (way_id, k, v, version) VALUES (63, 'accuracy', '5', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (63, 'hoot:id', '63');
INSERT INTO way_tags (way_id, k, v, version) VALUES (63, 'hoot:id', '63', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (63, 'alt_name', '17th St NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (63, 'alt_name', '17th St NW', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (63, 'name', '17TH ST NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (63, 'name', '17TH ST NW', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (63, 'highway', 'road');
INSERT INTO way_tags (way_id, k, v, version) VALUES (63, 'highway', 'road', 2);
DELETE FROM current_way_nodes WHERE way_id = 63;
DELETE FROM way_nodes WHERE way_id = 63;
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (63, 3263, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (63, 3263, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (63, 3261, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (63, 3261, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (63, 3259, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (63, 3259, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (63, 3257, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (63, 3257, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (63, 3255, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (63, 3255, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (63, 3253, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (63, 3253, 6);
/* modify way 65*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (65, 3, true, (now() at time zone 'utc'), 2);
UPDATE current_ways SET changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), version=2 WHERE id=65;
DELETE FROM current_way_tags WHERE way_id = 65;
DELETE FROM way_tags WHERE way_id = 65;
INSERT INTO current_way_tags (way_id, k, v) VALUES (65, 'accuracy', '5');
INSERT INTO way_tags (way_id, k, v, version) VALUES (65, 'accuracy', '5', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (65, 'hoot:id', '65');
INSERT INTO way_tags (way_id, k, v, version) VALUES (65, 'hoot:id', '65', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (65, 'alt_name', '17th St NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (65, 'alt_name', '17th St NW', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (65, 'name', '17TH ST NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (65, 'name', '17TH ST NW', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (65, 'highway', 'road');
INSERT INTO way_tags (way_id, k, v, version) VALUES (65, 'highway', 'road', 2);
DELETE FROM current_way_nodes WHERE way_id = 65;
DELETE FROM way_nodes WHERE way_id = 65;
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (65, 3253, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (65, 3253, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (65, 3251, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (65, 3251, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (65, 3249, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (65, 3249, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (65, 3247, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (65, 3247, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (65, 3245, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (65, 3245, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (65, 3243, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (65, 3243, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (65, 3241, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (65, 3241, 7);
/* modify way 67*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (67, 3, true, (now() at time zone 'utc'), 2);
UPDATE current_ways SET changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), version=2 WHERE id=67;
DELETE FROM current_way_tags WHERE way_id = 67;
DELETE FROM way_tags WHERE way_id = 67;
INSERT INTO current_way_tags (way_id, k, v) VALUES (67, 'accuracy', '5');
INSERT INTO way_tags (way_id, k, v, version) VALUES (67, 'accuracy', '5', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (67, 'hoot:id', '67');
INSERT INTO way_tags (way_id, k, v, version) VALUES (67, 'hoot:id', '67', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (67, 'alt_name', '17th St NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (67, 'alt_name', '17th St NW', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (67, 'name', '17TH ST NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (67, 'name', '17TH ST NW', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (67, 'highway', 'road');
INSERT INTO way_tags (way_id, k, v, version) VALUES (67, 'highway', 'road', 2);
DELETE FROM current_way_nodes WHERE way_id = 67;
DELETE FROM way_nodes WHERE way_id = 67;
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (67, 3241, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (67, 3241, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (67, 3239, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (67, 3239, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (67, 3237, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (67, 3237, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (67, 3235, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (67, 3235, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (67, 3233, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (67, 3233, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (67, 3556, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (67, 3556, 6);
/* modify way 70*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (70, 3, true, (now() at time zone 'utc'), 2);
UPDATE current_ways SET changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), version=2 WHERE id=70;
DELETE FROM current_way_tags WHERE way_id = 70;
DELETE FROM way_tags WHERE way_id = 70;
INSERT INTO current_way_tags (way_id, k, v) VALUES (70, 'accuracy', '5');
INSERT INTO way_tags (way_id, k, v, version) VALUES (70, 'accuracy', '5', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (70, 'hoot:id', '70');
INSERT INTO way_tags (way_id, k, v, version) VALUES (70, 'hoot:id', '70', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (70, 'alt_name', '15th St NW;Raoul Wallenberg Pl SW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (70, 'alt_name', '15th St NW;Raoul Wallenberg Pl SW', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (70, 'name', '15TH ST NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (70, 'name', '15TH ST NW', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (70, 'highway', 'road');
INSERT INTO way_tags (way_id, k, v, version) VALUES (70, 'highway', 'road', 2);
DELETE FROM current_way_nodes WHERE way_id = 70;
DELETE FROM way_nodes WHERE way_id = 70;
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (70, 1952, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (70, 1952, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (70, 273, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (70, 273, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (70, 272, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (70, 272, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (70, 271, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (70, 271, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (70, 270, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (70, 270, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (70, 269, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (70, 269, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (70, 268, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (70, 268, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (70, 267, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (70, 267, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (70, 266, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (70, 266, 9);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (70, 265, 1, 10);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (70, 265, 10);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (70, 264, 1, 11);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (70, 264, 11);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (70, 263, 1, 12);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (70, 263, 12);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (70, 262, 1, 13);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (70, 262, 13);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (70, 261, 1, 14);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (70, 261, 14);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (70, 260, 1, 15);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (70, 260, 15);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (70, 258, 1, 16);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (70, 258, 16);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (70, 256, 1, 17);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (70, 256, 17);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (70, 254, 1, 18);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (70, 254, 18);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (70, 252, 1, 19);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (70, 252, 19);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (70, 249, 1, 20);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (70, 249, 20);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (70, 246, 1, 21);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (70, 246, 21);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (70, 243, 1, 22);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (70, 243, 22);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (70, 240, 1, 23);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (70, 240, 23);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (70, 237, 1, 24);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (70, 237, 24);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (70, 234, 1, 25);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (70, 234, 25);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (70, 231, 1, 26);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (70, 231, 26);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (70, 228, 1, 27);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (70, 228, 27);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (70, 225, 1, 28);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (70, 225, 28);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (70, 222, 1, 29);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (70, 222, 29);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (70, 219, 1, 30);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (70, 219, 30);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (70, 216, 1, 31);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (70, 216, 31);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (70, 1950, 1, 32);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (70, 1950, 32);
/* modify way 74*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (74, 3, true, (now() at time zone 'utc'), 2);
UPDATE current_ways SET changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), version=2 WHERE id=74;
DELETE FROM current_way_tags WHERE way_id = 74;
DELETE FROM way_tags WHERE way_id = 74;
INSERT INTO current_way_tags (way_id, k, v) VALUES (74, 'accuracy', '5');
INSERT INTO way_tags (way_id, k, v, version) VALUES (74, 'accuracy', '5', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (74, 'hoot:id', '74');
INSERT INTO way_tags (way_id, k, v, version) VALUES (74, 'hoot:id', '74', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (74, 'alt_name', '15th St NW;Raoul Wallenberg Pl SW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (74, 'alt_name', '15th St NW;Raoul Wallenberg Pl SW', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (74, 'name', '15TH ST NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (74, 'name', '15TH ST NW', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (74, 'highway', 'road');
INSERT INTO way_tags (way_id, k, v, version) VALUES (74, 'highway', 'road', 2);
DELETE FROM current_way_nodes WHERE way_id = 74;
DELETE FROM way_nodes WHERE way_id = 74;
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (74, 1921, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (74, 1921, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (74, 213, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (74, 213, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (74, 211, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (74, 211, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (74, 210, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (74, 210, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (74, 209, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (74, 209, 5);
/* modify way 84*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (84, 3, true, (now() at time zone 'utc'), 2);
UPDATE current_ways SET changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), version=2 WHERE id=84;
DELETE FROM current_way_tags WHERE way_id = 84;
DELETE FROM way_tags WHERE way_id = 84;
INSERT INTO current_way_tags (way_id, k, v) VALUES (84, 'accuracy', '5');
INSERT INTO way_tags (way_id, k, v, version) VALUES (84, 'accuracy', '5', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (84, 'hoot:id', '84');
INSERT INTO way_tags (way_id, k, v, version) VALUES (84, 'hoot:id', '84', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (84, 'alt_name', 'C St NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (84, 'alt_name', 'C St NW', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (84, 'name', 'C ST NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (84, 'name', 'C ST NW', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (84, 'highway', 'road');
INSERT INTO way_tags (way_id, k, v, version) VALUES (84, 'highway', 'road', 2);
DELETE FROM current_way_nodes WHERE way_id = 84;
DELETE FROM way_nodes WHERE way_id = 84;
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (84, 3253, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (84, 3253, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (84, 1188, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (84, 1188, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (84, 1186, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (84, 1186, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (84, 1184, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (84, 1184, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (84, 1182, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (84, 1182, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (84, 1180, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (84, 1180, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (84, 3071, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (84, 3071, 7);
/* modify way 86*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (86, 3, true, (now() at time zone 'utc'), 2);
UPDATE current_ways SET changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), version=2 WHERE id=86;
DELETE FROM current_way_tags WHERE way_id = 86;
DELETE FROM way_tags WHERE way_id = 86;
INSERT INTO current_way_tags (way_id, k, v) VALUES (86, 'accuracy', '5');
INSERT INTO way_tags (way_id, k, v, version) VALUES (86, 'accuracy', '5', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (86, 'hoot:id', '86');
INSERT INTO way_tags (way_id, k, v, version) VALUES (86, 'hoot:id', '86', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (86, 'alt_name', 'D St NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (86, 'alt_name', 'D St NW', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (86, 'name', 'D ST NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (86, 'name', 'D ST NW', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (86, 'highway', 'road');
INSERT INTO way_tags (way_id, k, v, version) VALUES (86, 'highway', 'road', 2);
DELETE FROM current_way_nodes WHERE way_id = 86;
DELETE FROM way_nodes WHERE way_id = 86;
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (86, 3241, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (86, 3241, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (86, 121, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (86, 121, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (86, 119, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (86, 119, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (86, 117, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (86, 117, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (86, 115, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (86, 115, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (86, 113, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (86, 113, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (86, 111, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (86, 111, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (86, 3056, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (86, 3056, 8);
/* modify way 112*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (112, 3, true, (now() at time zone 'utc'), 2);
UPDATE current_ways SET changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), version=2 WHERE id=112;
DELETE FROM current_way_tags WHERE way_id = 112;
DELETE FROM way_tags WHERE way_id = 112;
INSERT INTO current_way_tags (way_id, k, v) VALUES (112, 'accuracy', '5');
INSERT INTO way_tags (way_id, k, v, version) VALUES (112, 'accuracy', '5', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (112, 'hoot:id', '112');
INSERT INTO way_tags (way_id, k, v, version) VALUES (112, 'hoot:id', '112', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (112, 'alt_name', 'E St NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (112, 'alt_name', 'E St NW', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (112, 'name', 'E ST NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (112, 'name', 'E ST NW', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (112, 'highway', 'road');
INSERT INTO way_tags (way_id, k, v, version) VALUES (112, 'highway', 'road', 2);
DELETE FROM current_way_nodes WHERE way_id = 112;
DELETE FROM way_nodes WHERE way_id = 112;
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (112, 3628, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (112, 3628, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (112, 533, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (112, 533, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (112, 529, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (112, 529, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (112, 525, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (112, 525, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (112, 522, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (112, 522, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (112, 519, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (112, 519, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (112, 517, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (112, 517, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (112, 515, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (112, 515, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (112, 513, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (112, 513, 9);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (112, 511, 1, 10);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (112, 511, 10);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (112, 508, 1, 11);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (112, 508, 11);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (112, 505, 1, 12);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (112, 505, 12);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (112, 2148, 1, 13);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (112, 2148, 13);
/* modify way 113*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (113, 3, true, (now() at time zone 'utc'), 2);
UPDATE current_ways SET changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), version=2 WHERE id=113;
DELETE FROM current_way_tags WHERE way_id = 113;
DELETE FROM way_tags WHERE way_id = 113;
INSERT INTO current_way_tags (way_id, k, v) VALUES (113, 'accuracy', '5');
INSERT INTO way_tags (way_id, k, v, version) VALUES (113, 'accuracy', '5', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (113, 'hoot:id', '113');
INSERT INTO way_tags (way_id, k, v, version) VALUES (113, 'hoot:id', '113', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (113, 'alt_name', 'E St NW;Ellipse Rd NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (113, 'alt_name', 'E St NW;Ellipse Rd NW', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (113, 'name', 'E ST NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (113, 'name', 'E ST NW', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (113, 'highway', 'road');
INSERT INTO way_tags (way_id, k, v, version) VALUES (113, 'highway', 'road', 2);
DELETE FROM current_way_nodes WHERE way_id = 113;
DELETE FROM way_nodes WHERE way_id = 113;
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (113, 2148, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (113, 2148, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (113, 502, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (113, 502, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (113, 499, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (113, 499, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (113, 496, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (113, 496, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (113, 494, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (113, 494, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (113, 3183, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (113, 3183, 6);
/* modify way 115*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (115, 3, true, (now() at time zone 'utc'), 2);
UPDATE current_ways SET changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), version=2 WHERE id=115;
DELETE FROM current_way_tags WHERE way_id = 115;
DELETE FROM way_tags WHERE way_id = 115;
INSERT INTO current_way_tags (way_id, k, v) VALUES (115, 'accuracy', '5');
INSERT INTO way_tags (way_id, k, v, version) VALUES (115, 'accuracy', '5', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (115, 'hoot:id', '115');
INSERT INTO way_tags (way_id, k, v, version) VALUES (115, 'hoot:id', '115', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (115, 'alt_name', 'E St NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (115, 'alt_name', 'E St NW', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (115, 'name', 'E ST NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (115, 'name', 'E ST NW', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (115, 'highway', 'road');
INSERT INTO way_tags (way_id, k, v, version) VALUES (115, 'highway', 'road', 2);
DELETE FROM current_way_nodes WHERE way_id = 115;
DELETE FROM way_nodes WHERE way_id = 115;
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (115, 3556, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (115, 3556, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (115, 492, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (115, 492, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (115, 489, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (115, 489, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (115, 487, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (115, 487, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (115, 485, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (115, 485, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (115, 483, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (115, 483, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (115, 481, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (115, 481, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (115, 479, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (115, 479, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (115, 477, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (115, 477, 9);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (115, 475, 1, 10);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (115, 475, 10);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (115, 3041, 1, 11);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (115, 3041, 11);
/* modify way 136*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (136, 3, true, (now() at time zone 'utc'), 2);
UPDATE current_ways SET changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), version=2 WHERE id=136;
DELETE FROM current_way_tags WHERE way_id = 136;
DELETE FROM way_tags WHERE way_id = 136;
INSERT INTO current_way_tags (way_id, k, v) VALUES (136, 'accuracy', '5');
INSERT INTO way_tags (way_id, k, v, version) VALUES (136, 'accuracy', '5', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (136, 'hoot:id', '136');
INSERT INTO way_tags (way_id, k, v, version) VALUES (136, 'hoot:id', '136', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (136, 'alt_name', '16th St NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (136, 'alt_name', '16th St NW', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (136, 'name', '16TH ST NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (136, 'name', '16TH ST NW', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (136, 'highway', 'road');
INSERT INTO way_tags (way_id, k, v, version) VALUES (136, 'highway', 'road', 2);
DELETE FROM current_way_nodes WHERE way_id = 136;
DELETE FROM way_nodes WHERE way_id = 136;
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (136, 2702, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (136, 2702, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (136, 2699, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (136, 2699, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (136, 2696, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (136, 2696, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (136, 2693, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (136, 2693, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (136, 2690, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (136, 2690, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (136, 2687, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (136, 2687, 6);
/* modify way 140*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (140, 3, true, (now() at time zone 'utc'), 2);
UPDATE current_ways SET changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), version=2 WHERE id=140;
DELETE FROM current_way_tags WHERE way_id = 140;
DELETE FROM way_tags WHERE way_id = 140;
INSERT INTO current_way_tags (way_id, k, v) VALUES (140, 'accuracy', '5');
INSERT INTO way_tags (way_id, k, v, version) VALUES (140, 'accuracy', '5', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (140, 'hoot:id', '140');
INSERT INTO way_tags (way_id, k, v, version) VALUES (140, 'hoot:id', '140', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (140, 'alt_name', 'Constitution Ave NW;US Hwy 50');
INSERT INTO way_tags (way_id, k, v, version) VALUES (140, 'alt_name', 'Constitution Ave NW;US Hwy 50', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (140, 'name', 'CONSTITUTION AVE NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (140, 'name', 'CONSTITUTION AVE NW', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (140, 'highway', 'road');
INSERT INTO way_tags (way_id, k, v, version) VALUES (140, 'highway', 'road', 2);
DELETE FROM current_way_nodes WHERE way_id = 140;
DELETE FROM way_nodes WHERE way_id = 140;
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (140, 7500, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (140, 7500, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (140, 3741, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (140, 3741, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (140, 3740, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (140, 3740, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (140, 3739, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (140, 3739, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (140, 3738, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (140, 3738, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (140, 3737, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (140, 3737, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (140, 3736, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (140, 3736, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (140, 3735, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (140, 3735, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (140, 2702, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (140, 2702, 9);
/* modify way 142*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (142, 3, true, (now() at time zone 'utc'), 2);
UPDATE current_ways SET changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), version=2 WHERE id=142;
DELETE FROM current_way_tags WHERE way_id = 142;
DELETE FROM way_tags WHERE way_id = 142;
INSERT INTO current_way_tags (way_id, k, v) VALUES (142, 'accuracy', '5');
INSERT INTO way_tags (way_id, k, v, version) VALUES (142, 'accuracy', '5', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (142, 'hoot:id', '142');
INSERT INTO way_tags (way_id, k, v, version) VALUES (142, 'hoot:id', '142', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (142, 'alt_name', 'Constitution Ave NW;US Hwy 50');
INSERT INTO way_tags (way_id, k, v, version) VALUES (142, 'alt_name', 'Constitution Ave NW;US Hwy 50', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (142, 'name', 'CONSTITUTION AVE NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (142, 'name', 'CONSTITUTION AVE NW', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (142, 'highway', 'road');
INSERT INTO way_tags (way_id, k, v, version) VALUES (142, 'highway', 'road', 2);
DELETE FROM current_way_nodes WHERE way_id = 142;
DELETE FROM way_nodes WHERE way_id = 142;
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (142, 2702, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (142, 2702, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (142, 3734, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (142, 3734, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (142, 3733, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (142, 3733, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (142, 3732, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (142, 3732, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (142, 3731, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (142, 3731, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (142, 3730, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (142, 3730, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (142, 3729, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (142, 3729, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (142, 3728, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (142, 3728, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (142, 3727, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (142, 3727, 9);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (142, 7499, 1, 10);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (142, 7499, 10);
/* modify way 143*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (143, 3, true, (now() at time zone 'utc'), 2);
UPDATE current_ways SET changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), version=2 WHERE id=143;
DELETE FROM current_way_tags WHERE way_id = 143;
DELETE FROM way_tags WHERE way_id = 143;
INSERT INTO current_way_tags (way_id, k, v) VALUES (143, 'accuracy', '5');
INSERT INTO way_tags (way_id, k, v, version) VALUES (143, 'accuracy', '5', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (143, 'hoot:id', '143');
INSERT INTO way_tags (way_id, k, v, version) VALUES (143, 'hoot:id', '143', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (143, 'alt_name', 'Constitution Ave NW;US Hwy 50');
INSERT INTO way_tags (way_id, k, v, version) VALUES (143, 'alt_name', 'Constitution Ave NW;US Hwy 50', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (143, 'name', 'CONSTITUTION AVE NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (143, 'name', 'CONSTITUTION AVE NW', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (143, 'highway', 'road');
INSERT INTO way_tags (way_id, k, v, version) VALUES (143, 'highway', 'road', 2);
DELETE FROM current_way_nodes WHERE way_id = 143;
DELETE FROM way_nodes WHERE way_id = 143;
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (143, 3263, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (143, 3263, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (143, 3726, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (143, 3726, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (143, 3725, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (143, 3725, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (143, 3724, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (143, 3724, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (143, 3723, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (143, 3723, 5);
/* modify way 155*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (155, 3, true, (now() at time zone 'utc'), 2);
UPDATE current_ways SET changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), version=2 WHERE id=155;
DELETE FROM current_way_tags WHERE way_id = 155;
DELETE FROM way_tags WHERE way_id = 155;
INSERT INTO current_way_tags (way_id, k, v) VALUES (155, 'accuracy', '5');
INSERT INTO way_tags (way_id, k, v, version) VALUES (155, 'accuracy', '5', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (155, 'hoot:id', '155');
INSERT INTO way_tags (way_id, k, v, version) VALUES (155, 'hoot:id', '155', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (155, 'alt_name', 'E Executive Ave NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (155, 'alt_name', 'E Executive Ave NW', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (155, 'name', 'EAST EXECUTIVE AVE NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (155, 'name', 'EAST EXECUTIVE AVE NW', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (155, 'highway', 'road');
INSERT INTO way_tags (way_id, k, v, version) VALUES (155, 'highway', 'road', 2);
DELETE FROM current_way_nodes WHERE way_id = 155;
DELETE FROM way_nodes WHERE way_id = 155;
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (155, 3628, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (155, 3628, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (155, 3625, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (155, 3625, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (155, 3622, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (155, 3622, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (155, 1519, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (155, 1519, 4);
/* modify way 207*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (207, 3, true, (now() at time zone 'utc'), 2);
UPDATE current_ways SET changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), version=2 WHERE id=207;
DELETE FROM current_way_tags WHERE way_id = 207;
DELETE FROM way_tags WHERE way_id = 207;
INSERT INTO current_way_tags (way_id, k, v) VALUES (207, 'accuracy', '5');
INSERT INTO way_tags (way_id, k, v, version) VALUES (207, 'accuracy', '5', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (207, 'hoot:id', '207');
INSERT INTO way_tags (way_id, k, v, version) VALUES (207, 'hoot:id', '207', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (207, 'name', 'SOUTH EXECUTIVE AVE NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (207, 'name', 'SOUTH EXECUTIVE AVE NW', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (207, 'highway', 'road');
INSERT INTO way_tags (way_id, k, v, version) VALUES (207, 'highway', 'road', 2);
DELETE FROM current_way_nodes WHERE way_id = 207;
DELETE FROM way_nodes WHERE way_id = 207;
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (207, 7509, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (207, 7509, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (207, 1513, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (207, 1513, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (207, 1512, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (207, 1512, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (207, 1511, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (207, 1511, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (207, 1510, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (207, 1510, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (207, 1509, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (207, 1509, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (207, 1508, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (207, 1508, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (207, 1507, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (207, 1507, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (207, 1506, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (207, 1506, 9);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (207, 1505, 1, 10);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (207, 1505, 10);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (207, 1504, 1, 11);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (207, 1504, 11);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (207, 1503, 1, 12);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (207, 1503, 12);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (207, 1502, 1, 13);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (207, 1502, 13);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (207, 1501, 1, 14);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (207, 1501, 14);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (207, 1500, 1, 15);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (207, 1500, 15);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (207, 1499, 1, 16);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (207, 1499, 16);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (207, 1498, 1, 17);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (207, 1498, 17);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (207, 1497, 1, 18);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (207, 1497, 18);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (207, 1496, 1, 19);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (207, 1496, 19);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (207, 1495, 1, 20);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (207, 1495, 20);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (207, 1494, 1, 21);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (207, 1494, 21);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (207, 1493, 1, 22);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (207, 1493, 22);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (207, 1492, 1, 23);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (207, 1492, 23);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (207, 1491, 1, 24);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (207, 1491, 24);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (207, 7508, 1, 25);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (207, 7508, 25);
/* modify way 209*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (209, 3, true, (now() at time zone 'utc'), 2);
UPDATE current_ways SET changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), version=2 WHERE id=209;
DELETE FROM current_way_tags WHERE way_id = 209;
DELETE FROM way_tags WHERE way_id = 209;
INSERT INTO current_way_tags (way_id, k, v) VALUES (209, 'accuracy', '5');
INSERT INTO way_tags (way_id, k, v, version) VALUES (209, 'accuracy', '5', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (209, 'hoot:id', '209');
INSERT INTO way_tags (way_id, k, v, version) VALUES (209, 'hoot:id', '209', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (209, 'alt_name', '15th St NW;Raoul Wallenberg Pl SW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (209, 'alt_name', '15th St NW;Raoul Wallenberg Pl SW', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (209, 'name', '15TH ST NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (209, 'name', '15TH ST NW', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (209, 'highway', 'road');
INSERT INTO way_tags (way_id, k, v, version) VALUES (209, 'highway', 'road', 2);
DELETE FROM current_way_nodes WHERE way_id = 209;
DELETE FROM way_nodes WHERE way_id = 209;
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (209, 1950, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (209, 1950, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (209, 1948, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (209, 1948, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (209, 1946, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (209, 1946, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (209, 1944, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (209, 1944, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (209, 1941, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (209, 1941, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (209, 1938, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (209, 1938, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (209, 1935, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (209, 1935, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (209, 1933, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (209, 1933, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (209, 1931, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (209, 1931, 9);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (209, 1929, 1, 10);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (209, 1929, 10);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (209, 1927, 1, 11);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (209, 1927, 11);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (209, 1925, 1, 12);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (209, 1925, 12);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (209, 1923, 1, 13);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (209, 1923, 13);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (209, 1921, 1, 14);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (209, 1921, 14);
/* modify way 224*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (224, 3, true, (now() at time zone 'utc'), 2);
UPDATE current_ways SET changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), version=2 WHERE id=224;
DELETE FROM current_way_tags WHERE way_id = 224;
DELETE FROM way_tags WHERE way_id = 224;
INSERT INTO current_way_tags (way_id, k, v) VALUES (224, 'accuracy', '5');
INSERT INTO way_tags (way_id, k, v, version) VALUES (224, 'accuracy', '5', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (224, 'hoot:id', '224');
INSERT INTO way_tags (way_id, k, v, version) VALUES (224, 'hoot:id', '224', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (224, 'alt_name', 'Ellipse Rd NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (224, 'alt_name', 'Ellipse Rd NW', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (224, 'name', 'ELLIPSE RD NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (224, 'name', 'ELLIPSE RD NW', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (224, 'highway', 'road');
INSERT INTO way_tags (way_id, k, v, version) VALUES (224, 'highway', 'road', 2);
DELETE FROM current_way_nodes WHERE way_id = 224;
DELETE FROM way_nodes WHERE way_id = 224;
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (224, 7507, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (224, 7507, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (224, 3163, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (224, 3163, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (224, 3161, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (224, 3161, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (224, 3159, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (224, 3159, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (224, 3157, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (224, 3157, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (224, 3155, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (224, 3155, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (224, 3153, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (224, 3153, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (224, 3151, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (224, 3151, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (224, 3149, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (224, 3149, 9);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (224, 3147, 1, 10);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (224, 3147, 10);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (224, 3145, 1, 11);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (224, 3145, 11);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (224, 3143, 1, 12);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (224, 3143, 12);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (224, 3141, 1, 13);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (224, 3141, 13);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (224, 3139, 1, 14);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (224, 3139, 14);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (224, 3137, 1, 15);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (224, 3137, 15);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (224, 3135, 1, 16);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (224, 3135, 16);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (224, 3133, 1, 17);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (224, 3133, 17);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (224, 3131, 1, 18);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (224, 3131, 18);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (224, 3129, 1, 19);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (224, 3129, 19);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (224, 3127, 1, 20);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (224, 3127, 20);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (224, 3125, 1, 21);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (224, 3125, 21);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (224, 7506, 1, 22);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (224, 7506, 22);
/* modify way 330*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (330, 3, true, (now() at time zone 'utc'), 2);
UPDATE current_ways SET changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), version=2 WHERE id=330;
DELETE FROM current_way_tags WHERE way_id = 330;
DELETE FROM way_tags WHERE way_id = 330;
INSERT INTO current_way_tags (way_id, k, v) VALUES (330, 'accuracy', '5');
INSERT INTO way_tags (way_id, k, v, version) VALUES (330, 'accuracy', '5', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (330, 'hoot:id', '330');
INSERT INTO way_tags (way_id, k, v, version) VALUES (330, 'hoot:id', '330', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (330, 'alt_name', 'Ellipse Rd NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (330, 'alt_name', 'Ellipse Rd NW', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (330, 'name', 'ELLIPSE RD NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (330, 'name', 'ELLIPSE RD NW', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (330, 'highway', 'road');
INSERT INTO way_tags (way_id, k, v, version) VALUES (330, 'highway', 'road', 2);
DELETE FROM current_way_nodes WHERE way_id = 330;
DELETE FROM way_nodes WHERE way_id = 330;
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (330, 7505, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (330, 7505, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (330, 2183, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (330, 2183, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (330, 2182, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (330, 2182, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (330, 2181, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (330, 2181, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (330, 2180, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (330, 2180, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (330, 2179, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (330, 2179, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (330, 2178, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (330, 2178, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (330, 2177, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (330, 2177, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (330, 2176, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (330, 2176, 9);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (330, 2175, 1, 10);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (330, 2175, 10);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (330, 2174, 1, 11);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (330, 2174, 11);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (330, 2173, 1, 12);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (330, 2173, 12);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (330, 2172, 1, 13);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (330, 2172, 13);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (330, 7504, 1, 14);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (330, 7504, 14);
/* modify way 332*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (332, 3, true, (now() at time zone 'utc'), 2);
UPDATE current_ways SET changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), version=2 WHERE id=332;
DELETE FROM current_way_tags WHERE way_id = 332;
DELETE FROM way_tags WHERE way_id = 332;
INSERT INTO current_way_tags (way_id, k, v) VALUES (332, 'accuracy', '5');
INSERT INTO way_tags (way_id, k, v, version) VALUES (332, 'accuracy', '5', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (332, 'hoot:id', '332');
INSERT INTO way_tags (way_id, k, v, version) VALUES (332, 'hoot:id', '332', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (332, 'alt_name', 'E St NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (332, 'alt_name', 'E St NW', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (332, 'name', 'PENNSYLVANIA AVE NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (332, 'name', 'PENNSYLVANIA AVE NW', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (332, 'highway', 'road');
INSERT INTO way_tags (way_id, k, v, version) VALUES (332, 'highway', 'road', 2);
DELETE FROM current_way_nodes WHERE way_id = 332;
DELETE FROM way_nodes WHERE way_id = 332;
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (332, 4251, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (332, 4251, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (332, 3560, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (332, 3560, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (332, 3559, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (332, 3559, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (332, 3558, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (332, 3558, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (332, 3557, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (332, 3557, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (332, 1921, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (332, 1921, 6);
/* create way 604*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (604, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (604, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (604, 'accuracy', '5');
INSERT INTO way_tags (way_id, k, v, version) VALUES (604, 'accuracy', '5', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (604, 'hoot:id', '593');
INSERT INTO way_tags (way_id, k, v, version) VALUES (604, 'hoot:id', '593', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (604, 'name', 'Ellipse Rd NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (604, 'name', 'Ellipse Rd NW', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (604, 'highway', 'unclassified');
INSERT INTO way_tags (way_id, k, v, version) VALUES (604, 'highway', 'unclassified', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (604, 7504, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (604, 7504, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (604, 7650, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (604, 7650, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (604, 7649, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (604, 7649, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (604, 7648, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (604, 7648, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (604, 7647, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (604, 7647, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (604, 7646, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (604, 7646, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (604, 7645, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (604, 7645, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (604, 7644, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (604, 7644, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (604, 7643, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (604, 7643, 9);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (604, 7642, 1, 10);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (604, 7642, 10);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (604, 7641, 1, 11);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (604, 7641, 11);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (604, 7640, 1, 12);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (604, 7640, 12);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (604, 7639, 1, 13);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (604, 7639, 13);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (604, 7503, 1, 14);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (604, 7503, 14);
/* create way 608*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (608, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (608, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (608, 'accuracy', '5');
INSERT INTO way_tags (way_id, k, v, version) VALUES (608, 'accuracy', '5', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (608, 'hoot:id', '417');
INSERT INTO way_tags (way_id, k, v, version) VALUES (608, 'hoot:id', '417', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (608, 'name', 'Ellipse Rd NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (608, 'name', 'Ellipse Rd NW', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (608, 'highway', 'unclassified');
INSERT INTO way_tags (way_id, k, v, version) VALUES (608, 'highway', 'unclassified', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (608, 7507, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (608, 7507, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (608, 7501, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (608, 7501, 2);
/* create way 610*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (610, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (610, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (610, 'accuracy', '5');
INSERT INTO way_tags (way_id, k, v, version) VALUES (610, 'accuracy', '5', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (610, 'hoot:id', '-619');
INSERT INTO way_tags (way_id, k, v, version) VALUES (610, 'hoot:id', '-619', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (610, 'alt_name', 'E St NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (610, 'alt_name', 'E St NW', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (610, 'name', 'E ST NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (610, 'name', 'E ST NW', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (610, 'highway', 'road');
INSERT INTO way_tags (way_id, k, v, version) VALUES (610, 'highway', 'road', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (610, 7501, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (610, 7501, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (610, 598, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (610, 598, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (610, 3556, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (610, 3556, 3);
/* create way 611*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (611, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (611, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (611, 'accuracy', '5');
INSERT INTO way_tags (way_id, k, v, version) VALUES (611, 'accuracy', '5', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (611, 'hoot:id', '-616');
INSERT INTO way_tags (way_id, k, v, version) VALUES (611, 'hoot:id', '-616', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (611, 'alt_name', 'E St NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (611, 'alt_name', 'E St NW', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (611, 'name', 'E ST NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (611, 'name', 'E ST NW', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (611, 'highway', 'road');
INSERT INTO way_tags (way_id, k, v, version) VALUES (611, 'highway', 'road', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (611, 3183, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (611, 3183, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (611, 618, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (611, 618, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (611, 616, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (611, 616, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (611, 614, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (611, 614, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (611, 612, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (611, 612, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (611, 610, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (611, 610, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (611, 608, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (611, 608, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (611, 7502, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (611, 7502, 8);
/* create way 612*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (612, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (612, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (612, 'accuracy', '5');
INSERT INTO way_tags (way_id, k, v, version) VALUES (612, 'accuracy', '5', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (612, 'hoot:id', '438');
INSERT INTO way_tags (way_id, k, v, version) VALUES (612, 'hoot:id', '438', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (612, 'name', 'Ellipse Rd NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (612, 'name', 'Ellipse Rd NW', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (612, 'highway', 'unclassified');
INSERT INTO way_tags (way_id, k, v, version) VALUES (612, 'highway', 'unclassified', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (612, 7506, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (612, 7506, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (612, 7528, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (612, 7528, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (612, 7527, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (612, 7527, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (612, 7526, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (612, 7526, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (612, 7525, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (612, 7525, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (612, 7524, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (612, 7524, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (612, 7523, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (612, 7523, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (612, 7522, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (612, 7522, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (612, 7521, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (612, 7521, 9);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (612, 7520, 1, 10);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (612, 7520, 10);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (612, 7519, 1, 11);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (612, 7519, 11);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (612, 7518, 1, 12);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (612, 7518, 12);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (612, 7517, 1, 13);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (612, 7517, 13);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (612, 7516, 1, 14);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (612, 7516, 14);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (612, 7515, 1, 15);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (612, 7515, 15);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (612, 7514, 1, 16);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (612, 7514, 16);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (612, 7513, 1, 17);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (612, 7513, 17);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (612, 7512, 1, 18);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (612, 7512, 18);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (612, 7499, 1, 19);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (612, 7499, 19);
/* create way 613*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (613, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (613, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (613, 'accuracy', '5');
INSERT INTO way_tags (way_id, k, v, version) VALUES (613, 'accuracy', '5', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (613, 'hoot:id', '-29');
INSERT INTO way_tags (way_id, k, v, version) VALUES (613, 'hoot:id', '-29', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (613, 'name', 'South Pl NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (613, 'name', 'South Pl NW', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (613, 'foot', 'designated');
INSERT INTO way_tags (way_id, k, v, version) VALUES (613, 'foot', 'designated', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (613, 'highway', 'path');
INSERT INTO way_tags (way_id, k, v, version) VALUES (613, 'highway', 'path', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (613, 7502, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (613, 7502, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (613, 7619, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (613, 7619, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (613, 7618, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (613, 7618, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (613, 7617, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (613, 7617, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (613, 7616, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (613, 7616, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (613, 7615, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (613, 7615, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (613, 7508, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (613, 7508, 7);
/* create way 614*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (614, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (614, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (614, 'accuracy', '5');
INSERT INTO way_tags (way_id, k, v, version) VALUES (614, 'accuracy', '5', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (614, 'hoot:id', '-601');
INSERT INTO way_tags (way_id, k, v, version) VALUES (614, 'hoot:id', '-601', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (614, 'alt_name', 'E St NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (614, 'alt_name', 'E St NW', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (614, 'name', 'E ST NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (614, 'name', 'E ST NW', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (614, 'highway', 'road');
INSERT INTO way_tags (way_id, k, v, version) VALUES (614, 'highway', 'road', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (614, 1921, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (614, 1921, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (614, 636, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (614, 636, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (614, 7503, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (614, 7503, 3);
/* create way 615*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (615, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (615, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (615, 'accuracy', '5');
INSERT INTO way_tags (way_id, k, v, version) VALUES (615, 'accuracy', '5', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (615, 'hoot:id', '-598');
INSERT INTO way_tags (way_id, k, v, version) VALUES (615, 'hoot:id', '-598', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (615, 'alt_name', 'Ellipse Rd NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (615, 'alt_name', 'Ellipse Rd NW', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (615, 'name', 'ELLIPSE RD NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (615, 'name', 'ELLIPSE RD NW', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (615, 'highway', 'road');
INSERT INTO way_tags (way_id, k, v, version) VALUES (615, 'highway', 'road', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (615, 2687, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (615, 2687, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (615, 2215, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (615, 2215, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (615, 2214, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (615, 2214, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (615, 2213, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (615, 2213, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (615, 2212, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (615, 2212, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (615, 2211, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (615, 2211, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (615, 2210, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (615, 2210, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (615, 2209, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (615, 2209, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (615, 2207, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (615, 2207, 9);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (615, 2205, 1, 10);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (615, 2205, 10);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (615, 2203, 1, 11);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (615, 2203, 11);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (615, 2201, 1, 12);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (615, 2201, 12);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (615, 2199, 1, 13);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (615, 2199, 13);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (615, 2197, 1, 14);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (615, 2197, 14);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (615, 2195, 1, 15);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (615, 2195, 15);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (615, 2193, 1, 16);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (615, 2193, 16);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (615, 2191, 1, 17);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (615, 2191, 17);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (615, 2189, 1, 18);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (615, 2189, 18);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (615, 2188, 1, 19);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (615, 2188, 19);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (615, 2187, 1, 20);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (615, 2187, 20);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (615, 2186, 1, 21);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (615, 2186, 21);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (615, 2185, 1, 22);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (615, 2185, 22);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (615, 2184, 1, 23);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (615, 2184, 23);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (615, 7505, 1, 24);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (615, 7505, 24);
/* create way 616*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (616, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (616, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (616, 'accuracy', '5');
INSERT INTO way_tags (way_id, k, v, version) VALUES (616, 'accuracy', '5', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (616, 'hoot:id', '-595');
INSERT INTO way_tags (way_id, k, v, version) VALUES (616, 'hoot:id', '-595', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (616, 'alt_name', 'Ellipse Rd NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (616, 'alt_name', 'Ellipse Rd NW', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (616, 'name', 'ELLIPSE RD NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (616, 'name', 'ELLIPSE RD NW', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (616, 'highway', 'road');
INSERT INTO way_tags (way_id, k, v, version) VALUES (616, 'highway', 'road', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (616, 7504, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (616, 7504, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (616, 2170, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (616, 2170, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (616, 2168, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (616, 2168, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (616, 2166, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (616, 2166, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (616, 2164, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (616, 2164, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (616, 2162, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (616, 2162, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (616, 2160, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (616, 2160, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (616, 2158, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (616, 2158, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (616, 2156, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (616, 2156, 9);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (616, 2154, 1, 10);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (616, 2154, 10);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (616, 2152, 1, 11);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (616, 2152, 11);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (616, 2150, 1, 12);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (616, 2150, 12);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (616, 2148, 1, 13);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (616, 2148, 13);
/* create way 618*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (618, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (618, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (618, 'accuracy', '5');
INSERT INTO way_tags (way_id, k, v, version) VALUES (618, 'accuracy', '5', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (618, 'hoot:id', '-580');
INSERT INTO way_tags (way_id, k, v, version) VALUES (618, 'hoot:id', '-580', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (618, 'alt_name', 'Ellipse Rd NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (618, 'alt_name', 'Ellipse Rd NW', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (618, 'name', 'ELLIPSE RD NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (618, 'name', 'ELLIPSE RD NW', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (618, 'highway', 'road');
INSERT INTO way_tags (way_id, k, v, version) VALUES (618, 'highway', 'road', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (618, 7506, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (618, 7506, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (618, 3123, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (618, 3123, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (618, 3121, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (618, 3121, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (618, 3119, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (618, 3119, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (618, 3117, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (618, 3117, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (618, 3115, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (618, 3115, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (618, 3113, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (618, 3113, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (618, 3111, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (618, 3111, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (618, 3109, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (618, 3109, 9);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (618, 3107, 1, 10);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (618, 3107, 10);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (618, 3105, 1, 11);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (618, 3105, 11);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (618, 3103, 1, 12);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (618, 3103, 12);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (618, 3101, 1, 13);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (618, 3101, 13);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (618, 3099, 1, 14);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (618, 3099, 14);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (618, 3097, 1, 15);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (618, 3097, 15);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (618, 3095, 1, 16);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (618, 3095, 16);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (618, 3093, 1, 17);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (618, 3093, 17);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (618, 2687, 1, 18);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (618, 2687, 18);
/* create way 620*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (620, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (620, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (620, 'accuracy', '5');
INSERT INTO way_tags (way_id, k, v, version) VALUES (620, 'accuracy', '5', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (620, 'hoot:id', '-576');
INSERT INTO way_tags (way_id, k, v, version) VALUES (620, 'hoot:id', '-576', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (620, 'alt_name', 'Ellipse Rd NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (620, 'alt_name', 'Ellipse Rd NW', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (620, 'name', 'ELLIPSE RD NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (620, 'name', 'ELLIPSE RD NW', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (620, 'highway', 'road');
INSERT INTO way_tags (way_id, k, v, version) VALUES (620, 'highway', 'road', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (620, 3183, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (620, 3183, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (620, 3181, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (620, 3181, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (620, 3179, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (620, 3179, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (620, 3177, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (620, 3177, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (620, 3175, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (620, 3175, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (620, 3173, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (620, 3173, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (620, 3171, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (620, 3171, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (620, 3169, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (620, 3169, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (620, 3167, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (620, 3167, 9);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (620, 3165, 1, 10);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (620, 3165, 10);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (620, 7507, 1, 11);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (620, 7507, 11);
/* create way 622*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (622, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (622, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (622, 'accuracy', '5');
INSERT INTO way_tags (way_id, k, v, version) VALUES (622, 'accuracy', '5', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (622, 'hoot:id', '-559');
INSERT INTO way_tags (way_id, k, v, version) VALUES (622, 'hoot:id', '-559', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (622, 'alt_name', 'South Pl NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (622, 'alt_name', 'South Pl NW', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (622, 'name', 'SOUTH EXECUTIVE AVE NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (622, 'name', 'SOUTH EXECUTIVE AVE NW', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (622, 'foot', 'designated');
INSERT INTO way_tags (way_id, k, v, version) VALUES (622, 'foot', 'designated', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (622, 'highway', 'road');
INSERT INTO way_tags (way_id, k, v, version) VALUES (622, 'highway', 'road', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (622, 7508, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (622, 7508, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (622, 1490, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (622, 1490, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (622, 1489, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (622, 1489, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (622, 1488, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (622, 1488, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (622, 1487, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (622, 1487, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (622, 1486, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (622, 1486, 6);
/* create way 623*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (623, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (623, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (623, 'accuracy', '5');
INSERT INTO way_tags (way_id, k, v, version) VALUES (623, 'accuracy', '5', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (623, 'hoot:id', '-555');
INSERT INTO way_tags (way_id, k, v, version) VALUES (623, 'hoot:id', '-555', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (623, 'alt_name', 'Executive Ave NW;South Pl NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (623, 'alt_name', 'Executive Ave NW;South Pl NW', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (623, 'name', 'SOUTH EXECUTIVE AVE NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (623, 'name', 'SOUTH EXECUTIVE AVE NW', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (623, 'highway', 'road');
INSERT INTO way_tags (way_id, k, v, version) VALUES (623, 'highway', 'road', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (623, 1519, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (623, 1519, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (623, 1518, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (623, 1518, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (623, 1517, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (623, 1517, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (623, 1516, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (623, 1516, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (623, 1515, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (623, 1515, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (623, 1514, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (623, 1514, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (623, 7509, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (623, 7509, 7);
/* create way 624*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (624, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (624, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (624, 'accuracy', '5');
INSERT INTO way_tags (way_id, k, v, version) VALUES (624, 'accuracy', '5', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (624, 'hoot:id', '539');
INSERT INTO way_tags (way_id, k, v, version) VALUES (624, 'hoot:id', '539', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (624, 'name', 'Ellipse Rd NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (624, 'name', 'Ellipse Rd NW', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (624, 'highway', 'unclassified');
INSERT INTO way_tags (way_id, k, v, version) VALUES (624, 'highway', 'unclassified', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (624, 7505, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (624, 7505, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (624, 7537, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (624, 7537, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (624, 7536, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (624, 7536, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (624, 7535, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (624, 7535, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (624, 7534, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (624, 7534, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (624, 7533, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (624, 7533, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (624, 7532, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (624, 7532, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (624, 7531, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (624, 7531, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (624, 7530, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (624, 7530, 9);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (624, 7529, 1, 10);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (624, 7529, 10);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (624, 7500, 1, 11);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (624, 7500, 11);
/* create way 626*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (626, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (626, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (626, 'accuracy', '5');
INSERT INTO way_tags (way_id, k, v, version) VALUES (626, 'accuracy', '5', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (626, 'hoot:id', '-673');
INSERT INTO way_tags (way_id, k, v, version) VALUES (626, 'hoot:id', '-673', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (626, 'alt_name', 'Constitution Ave NW;US Hwy 50');
INSERT INTO way_tags (way_id, k, v, version) VALUES (626, 'alt_name', 'Constitution Ave NW;US Hwy 50', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (626, 'name', 'CONSTITUTION AVE NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (626, 'name', 'CONSTITUTION AVE NW', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (626, 'highway', 'road');
INSERT INTO way_tags (way_id, k, v, version) VALUES (626, 'highway', 'road', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (626, 7499, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (626, 7499, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (626, 3263, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (626, 3263, 2);
/* create way 627*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (627, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (627, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (627, 'accuracy', '5');
INSERT INTO way_tags (way_id, k, v, version) VALUES (627, 'accuracy', '5', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (627, 'hoot:id', '-666');
INSERT INTO way_tags (way_id, k, v, version) VALUES (627, 'hoot:id', '-666', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (627, 'alt_name', 'Constitution Ave NW;US Hwy 50');
INSERT INTO way_tags (way_id, k, v, version) VALUES (627, 'alt_name', 'Constitution Ave NW;US Hwy 50', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (627, 'name', 'CONSTITUTION AVE NW');
INSERT INTO way_tags (way_id, k, v, version) VALUES (627, 'name', 'CONSTITUTION AVE NW', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (627, 'highway', 'road');
INSERT INTO way_tags (way_id, k, v, version) VALUES (627, 'highway', 'road', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (627, 1950, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (627, 1950, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (627, 3745, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (627, 3745, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (627, 3743, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (627, 3743, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (627, 7500, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (627, 7500, 4);
UPDATE changesets SET min_lat=388898790, max_lat=388964058, min_lon=-770417432, max_lon=-770319550, num_changes=400 WHERE id=3;
