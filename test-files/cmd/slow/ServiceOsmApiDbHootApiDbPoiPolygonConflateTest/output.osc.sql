INSERT INTO changesets (id, user_id, created_at, closed_at) VALUES (3, 1, (now() at time zone 'utc'), (now() at time zone 'utc'));
INSERT INTO changeset_tags (changeset_id, k, v) VALUES (3, 'written_by', 'Hootenanny');
/* delete node 2*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2, 377646765, -1224201418, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 2;
DELETE FROM node_tags WHERE node_id = 2;
DELETE FROM current_way_nodes WHERE node_id=2;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 2;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=2;
/* delete node 3*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3, 377646646, -1224203261, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 3;
DELETE FROM node_tags WHERE node_id = 3;
DELETE FROM current_way_nodes WHERE node_id=3;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 3;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=3;
/* delete node 4*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (4, 377647569, -1224217023, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 4;
DELETE FROM node_tags WHERE node_id = 4;
DELETE FROM current_way_nodes WHERE node_id=4;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 4;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=4;
/* delete node 6*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (6, 377648614, -1224210767, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 6;
DELETE FROM node_tags WHERE node_id = 6;
DELETE FROM current_way_nodes WHERE node_id=6;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 6;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=6;
/* delete node 8*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (8, 377635994, -1224237890, 3, false, (now() at time zone 'utc'), 1301409555, 2);
DELETE FROM current_node_tags WHERE node_id = 8;
DELETE FROM node_tags WHERE node_id = 8;
DELETE FROM current_way_nodes WHERE node_id=8;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 8;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=8;
/* delete node 21*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (21, 377649400, -1224199504, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 21;
DELETE FROM node_tags WHERE node_id = 21;
DELETE FROM current_way_nodes WHERE node_id=21;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 21;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=21;
/* delete node 23*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (23, 377648246, -1224197530, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 23;
DELETE FROM node_tags WHERE node_id = 23;
DELETE FROM current_way_nodes WHERE node_id=23;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 23;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=23;
/* delete node 25*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (25, 377618662, -1224197444, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 25;
DELETE FROM node_tags WHERE node_id = 25;
DELETE FROM current_way_nodes WHERE node_id=25;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 25;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=25;
/* delete node 51*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (51, 377650919, -1224216926, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 51;
DELETE FROM node_tags WHERE node_id = 51;
DELETE FROM current_way_nodes WHERE node_id=51;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 51;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=51;
/* delete node 53*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (53, 377650583, -1224220700, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 53;
DELETE FROM node_tags WHERE node_id = 53;
DELETE FROM current_way_nodes WHERE node_id=53;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 53;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=53;
/* delete node 56*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (56, 377647640, -1224227283, 3, false, (now() at time zone 'utc'), 1301409555, 2);
DELETE FROM current_node_tags WHERE node_id = 56;
DELETE FROM node_tags WHERE node_id = 56;
DELETE FROM current_way_nodes WHERE node_id=56;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 56;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=56;
/* delete node 58*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (58, 377649404, -1224239385, 3, false, (now() at time zone 'utc'), 1301409555, 2);
DELETE FROM current_node_tags WHERE node_id = 58;
DELETE FROM node_tags WHERE node_id = 58;
DELETE FROM current_way_nodes WHERE node_id=58;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 58;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=58;
/* delete node 59*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (59, 377649564, -1224237344, 3, false, (now() at time zone 'utc'), 1301409555, 2);
DELETE FROM current_node_tags WHERE node_id = 59;
DELETE FROM node_tags WHERE node_id = 59;
DELETE FROM current_way_nodes WHERE node_id=59;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 59;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=59;
/* delete node 60*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (60, 377646987, -1224235800, 3, false, (now() at time zone 'utc'), 1301409555, 2);
DELETE FROM current_node_tags WHERE node_id = 60;
DELETE FROM node_tags WHERE node_id = 60;
DELETE FROM current_way_nodes WHERE node_id=60;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 60;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=60;
/* delete node 61*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (61, 377647122, -1224233224, 3, false, (now() at time zone 'utc'), 1301409555, 2);
DELETE FROM current_node_tags WHERE node_id = 61;
DELETE FROM node_tags WHERE node_id = 61;
DELETE FROM current_way_nodes WHERE node_id=61;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 61;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=61;
/* delete node 64*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (64, 377632581, -1224196732, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 64;
DELETE FROM node_tags WHERE node_id = 64;
DELETE FROM current_way_nodes WHERE node_id=64;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 64;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=64;
/* delete node 65*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (65, 377648278, -1224222608, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 65;
DELETE FROM node_tags WHERE node_id = 65;
DELETE FROM current_way_nodes WHERE node_id=65;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 65;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=65;
/* delete node 66*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (66, 377635113, -1224199872, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 66;
DELETE FROM node_tags WHERE node_id = 66;
DELETE FROM current_way_nodes WHERE node_id=66;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 66;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=66;
/* delete node 68*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (68, 377649940, -1224226909, 3, false, (now() at time zone 'utc'), 1301409555, 2);
DELETE FROM current_node_tags WHERE node_id = 68;
DELETE FROM node_tags WHERE node_id = 68;
DELETE FROM current_way_nodes WHERE node_id=68;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 68;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=68;
/* delete node 69*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (69, 377634830, -1224201147, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 69;
DELETE FROM node_tags WHERE node_id = 69;
DELETE FROM current_way_nodes WHERE node_id=69;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 69;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=69;
/* delete node 70*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (70, 377649810, -1224223199, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 70;
DELETE FROM node_tags WHERE node_id = 70;
DELETE FROM current_way_nodes WHERE node_id=70;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 70;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=70;
/* delete node 71*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (71, 377634905, -1224200277, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 71;
DELETE FROM node_tags WHERE node_id = 71;
DELETE FROM current_way_nodes WHERE node_id=71;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 71;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=71;
/* delete node 72*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (72, 377636560, -1224200439, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 72;
DELETE FROM node_tags WHERE node_id = 72;
DELETE FROM current_way_nodes WHERE node_id=72;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 72;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=72;
/* delete node 73*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73, 377636620, -1224199454, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 73;
DELETE FROM node_tags WHERE node_id = 73;
DELETE FROM current_way_nodes WHERE node_id=73;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 73;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=73;
/* delete node 74*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (74, 377637399, -1224201437, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 74;
DELETE FROM node_tags WHERE node_id = 74;
DELETE FROM current_way_nodes WHERE node_id=74;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 74;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=74;
/* delete node 75*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (75, 377637461, -1224200555, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 75;
DELETE FROM node_tags WHERE node_id = 75;
DELETE FROM current_way_nodes WHERE node_id=75;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 75;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=75;
/* delete node 76*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (76, 377647569, -1224223631, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 76;
DELETE FROM node_tags WHERE node_id = 76;
DELETE FROM current_way_nodes WHERE node_id=76;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 76;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=76;
/* delete node 82*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (82, 377647407, -1224220141, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 82;
DELETE FROM node_tags WHERE node_id = 82;
DELETE FROM current_way_nodes WHERE node_id=82;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 82;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=82;
/* delete node 83*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (83, 377650804, -1224196567, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 83;
DELETE FROM node_tags WHERE node_id = 83;
DELETE FROM current_way_nodes WHERE node_id=83;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 83;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=83;
/* delete node 84*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (84, 377634905, -1224200277, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 84;
DELETE FROM node_tags WHERE node_id = 84;
DELETE FROM current_way_nodes WHERE node_id=84;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 84;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=84;
/* delete node 85*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (85, 377634965, -1224199292, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 85;
DELETE FROM node_tags WHERE node_id = 85;
DELETE FROM current_way_nodes WHERE node_id=85;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 85;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=85;
/* delete node 86*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (86, 377635606, -1224199355, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 86;
DELETE FROM node_tags WHERE node_id = 86;
DELETE FROM current_way_nodes WHERE node_id=86;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 86;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=86;
/* delete node 87*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (87, 377636560, -1224200439, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 87;
DELETE FROM node_tags WHERE node_id = 87;
DELETE FROM current_way_nodes WHERE node_id=87;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 87;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=87;
/* delete node 101*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (101, 377627526, -1224195999, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 101;
DELETE FROM node_tags WHERE node_id = 101;
DELETE FROM current_way_nodes WHERE node_id=101;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 101;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=101;
/* delete node 104*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (104, 377627592, -1224195999, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 104;
DELETE FROM node_tags WHERE node_id = 104;
DELETE FROM current_way_nodes WHERE node_id=104;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 104;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=104;
/* delete node 107*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (107, 377624302, -1224195999, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 107;
DELETE FROM node_tags WHERE node_id = 107;
DELETE FROM current_way_nodes WHERE node_id=107;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 107;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=107;
/* delete node 110*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (110, 377623582, -1224195999, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 110;
DELETE FROM node_tags WHERE node_id = 110;
DELETE FROM current_way_nodes WHERE node_id=110;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 110;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=110;
/* delete node 113*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (113, 377623582, -1224195999, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 113;
DELETE FROM node_tags WHERE node_id = 113;
DELETE FROM current_way_nodes WHERE node_id=113;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 113;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=113;
/* delete node 115*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (115, 377622886, -1224195999, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 115;
DELETE FROM node_tags WHERE node_id = 115;
DELETE FROM current_way_nodes WHERE node_id=115;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 115;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=115;
/* delete node 117*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (117, 377622250, -1224195999, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 117;
DELETE FROM node_tags WHERE node_id = 117;
DELETE FROM current_way_nodes WHERE node_id=117;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 117;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=117;
/* delete node 119*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (119, 377620776, -1224195999, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 119;
DELETE FROM node_tags WHERE node_id = 119;
DELETE FROM current_way_nodes WHERE node_id=119;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 119;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=119;
/* delete node 120*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (120, 377625025, -1224195999, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 120;
DELETE FROM node_tags WHERE node_id = 120;
DELETE FROM current_way_nodes WHERE node_id=120;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 120;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=120;
/* delete node 121*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (121, 377624302, -1224195999, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 121;
DELETE FROM node_tags WHERE node_id = 121;
DELETE FROM current_way_nodes WHERE node_id=121;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 121;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=121;
/* delete node 122*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (122, 377630525, -1224199611, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 122;
DELETE FROM node_tags WHERE node_id = 122;
DELETE FROM current_way_nodes WHERE node_id=122;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 122;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=122;
/* delete node 123*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (123, 377620776, -1224195999, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 123;
DELETE FROM node_tags WHERE node_id = 123;
DELETE FROM current_way_nodes WHERE node_id=123;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 123;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=123;
/* delete node 124*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (124, 377630734, -1224196305, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 124;
DELETE FROM node_tags WHERE node_id = 124;
DELETE FROM current_way_nodes WHERE node_id=124;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 124;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=124;
/* delete node 125*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (125, 377619142, -1224195999, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 125;
DELETE FROM node_tags WHERE node_id = 125;
DELETE FROM current_way_nodes WHERE node_id=125;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 125;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=125;
/* delete node 126*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (126, 377631140, -1224198902, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 126;
DELETE FROM node_tags WHERE node_id = 126;
DELETE FROM current_way_nodes WHERE node_id=126;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 126;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=126;
/* delete node 127*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (127, 377622250, -1224195999, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 127;
DELETE FROM node_tags WHERE node_id = 127;
DELETE FROM current_way_nodes WHERE node_id=127;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 127;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=127;
/* delete node 128*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (128, 377631193, -1224198058, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 128;
DELETE FROM node_tags WHERE node_id = 128;
DELETE FROM current_way_nodes WHERE node_id=128;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 128;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=128;
/* delete node 129*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (129, 377622886, -1224195999, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 129;
DELETE FROM node_tags WHERE node_id = 129;
DELETE FROM current_way_nodes WHERE node_id=129;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 129;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=129;
/* delete node 130*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (130, 377631302, -1224199690, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 130;
DELETE FROM node_tags WHERE node_id = 130;
DELETE FROM current_way_nodes WHERE node_id=130;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 130;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=130;
/* delete node 131*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (131, 377626590, -1224195999, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 131;
DELETE FROM node_tags WHERE node_id = 131;
DELETE FROM current_way_nodes WHERE node_id=131;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 131;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=131;
/* delete node 132*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (132, 377631314, -1224199503, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 132;
DELETE FROM node_tags WHERE node_id = 132;
DELETE FROM current_way_nodes WHERE node_id=132;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 132;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=132;
/* delete node 133*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (133, 377625700, -1224195999, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 133;
DELETE FROM node_tags WHERE node_id = 133;
DELETE FROM current_way_nodes WHERE node_id=133;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 133;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=133;
/* delete node 134*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (134, 377631350, -1224198924, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 134;
DELETE FROM node_tags WHERE node_id = 134;
DELETE FROM current_way_nodes WHERE node_id=134;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 134;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=134;
/* delete node 135*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (135, 377625700, -1224195999, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 135;
DELETE FROM node_tags WHERE node_id = 135;
DELETE FROM current_way_nodes WHERE node_id=135;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 135;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=135;
/* delete node 136*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (136, 377631362, -1224197418, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 136;
DELETE FROM node_tags WHERE node_id = 136;
DELETE FROM current_way_nodes WHERE node_id=136;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 136;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=136;
/* delete node 137*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (137, 377625025, -1224195999, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 137;
DELETE FROM node_tags WHERE node_id = 137;
DELETE FROM current_way_nodes WHERE node_id=137;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 137;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=137;
/* delete node 138*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (138, 377631390, -1224196973, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 138;
DELETE FROM node_tags WHERE node_id = 138;
DELETE FROM current_way_nodes WHERE node_id=138;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 138;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=138;
/* delete node 140*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (140, 377631404, -1224198080, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 140;
DELETE FROM node_tags WHERE node_id = 140;
DELETE FROM current_way_nodes WHERE node_id=140;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 140;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=140;
/* delete node 142*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (142, 377631445, -1224197427, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 142;
DELETE FROM node_tags WHERE node_id = 142;
DELETE FROM current_way_nodes WHERE node_id=142;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 142;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=142;
/* delete node 144*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (144, 377631473, -1224196981, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 144;
DELETE FROM node_tags WHERE node_id = 144;
DELETE FROM current_way_nodes WHERE node_id=144;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 144;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=144;
/* delete node 146*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (146, 377631511, -1224196383, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 146;
DELETE FROM node_tags WHERE node_id = 146;
DELETE FROM current_way_nodes WHERE node_id=146;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 146;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=146;
/* delete node 147*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (147, 377651000, -1224213486, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 147;
DELETE FROM node_tags WHERE node_id = 147;
DELETE FROM current_way_nodes WHERE node_id=147;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 147;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=147;
/* delete node 148*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (148, 377633209, -1224199695, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 148;
DELETE FROM node_tags WHERE node_id = 148;
DELETE FROM current_way_nodes WHERE node_id=148;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 148;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=148;
/* delete node 149*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (149, 377651000, -1224211840, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 149;
DELETE FROM node_tags WHERE node_id = 149;
DELETE FROM current_way_nodes WHERE node_id=149;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 149;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=149;
/* delete node 150*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (150, 377633406, -1224196575, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 150;
DELETE FROM node_tags WHERE node_id = 150;
DELETE FROM current_way_nodes WHERE node_id=150;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 150;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=150;
/* delete node 151*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (151, 377618000, -1224195999, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 151;
DELETE FROM node_tags WHERE node_id = 151;
DELETE FROM current_way_nodes WHERE node_id=151;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 151;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=151;
/* delete node 152*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (152, 377650414, -1224195999, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 152;
DELETE FROM node_tags WHERE node_id = 152;
DELETE FROM current_way_nodes WHERE node_id=152;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 152;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=152;
/* delete node 153*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (153, 377618000, -1224197813, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 153;
DELETE FROM node_tags WHERE node_id = 153;
DELETE FROM current_way_nodes WHERE node_id=153;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 153;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=153;
/* delete node 154*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (154, 377633924, -1224199658, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 154;
DELETE FROM node_tags WHERE node_id = 154;
DELETE FROM current_way_nodes WHERE node_id=154;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 154;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=154;
/* delete node 155*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (155, 377634138, -1224200163, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 155;
DELETE FROM node_tags WHERE node_id = 155;
DELETE FROM current_way_nodes WHERE node_id=155;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 155;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=155;
/* delete node 156*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (156, 377634965, -1224199292, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 156;
DELETE FROM node_tags WHERE node_id = 156;
DELETE FROM current_way_nodes WHERE node_id=156;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 156;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=156;
/* delete node 157*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (157, 377635119, -1224196664, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 157;
DELETE FROM node_tags WHERE node_id = 157;
DELETE FROM current_way_nodes WHERE node_id=157;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 157;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=157;
/* delete node 158*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (158, 377635606, -1224199355, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 158;
DELETE FROM node_tags WHERE node_id = 158;
DELETE FROM current_way_nodes WHERE node_id=158;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 158;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=158;
/* delete node 159*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (159, 377635626, -1224198994, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 159;
DELETE FROM node_tags WHERE node_id = 159;
DELETE FROM current_way_nodes WHERE node_id=159;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 159;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=159;
/* delete node 160*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (160, 377635710, -1224199003, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 160;
DELETE FROM node_tags WHERE node_id = 160;
DELETE FROM current_way_nodes WHERE node_id=160;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 160;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=160;
/* delete node 161*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (161, 377635859, -1224196742, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 161;
DELETE FROM node_tags WHERE node_id = 161;
DELETE FROM current_way_nodes WHERE node_id=161;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 161;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=161;
/* delete node 163*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (163, 377621084, -1224200980, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 163;
DELETE FROM node_tags WHERE node_id = 163;
DELETE FROM current_way_nodes WHERE node_id=163;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 163;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=163;
/* delete node 164*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (164, 377621211, -1224199089, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 164;
DELETE FROM node_tags WHERE node_id = 164;
DELETE FROM current_way_nodes WHERE node_id=164;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 164;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=164;
/* delete node 165*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (165, 377618942, -1224198901, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 165;
DELETE FROM node_tags WHERE node_id = 165;
DELETE FROM current_way_nodes WHERE node_id=165;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 165;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=165;
/* delete node 166*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (166, 377618815, -1224200792, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 166;
DELETE FROM node_tags WHERE node_id = 166;
DELETE FROM current_way_nodes WHERE node_id=166;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 166;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=166;
/* delete node 167*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (167, 377621020, -1224198901, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 167;
DELETE FROM node_tags WHERE node_id = 167;
DELETE FROM current_way_nodes WHERE node_id=167;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 167;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=167;
/* delete node 168*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (168, 377621041, -1224198727, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 168;
DELETE FROM node_tags WHERE node_id = 168;
DELETE FROM current_way_nodes WHERE node_id=168;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 168;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=168;
/* delete node 169*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (169, 377620967, -1224198539, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 169;
DELETE FROM node_tags WHERE node_id = 169;
DELETE FROM current_way_nodes WHERE node_id=169;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 169;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=169;
/* delete node 170*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (170, 377620919, -1224198151, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 170;
DELETE FROM node_tags WHERE node_id = 170;
DELETE FROM current_way_nodes WHERE node_id=170;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 170;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=170;
/* delete node 171*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (171, 377620850, -1224199062, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 171;
DELETE FROM node_tags WHERE node_id = 171;
DELETE FROM current_way_nodes WHERE node_id=171;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 171;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=171;
/* delete node 172*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (172, 377622748, -1224201141, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 172;
DELETE FROM node_tags WHERE node_id = 172;
DELETE FROM current_way_nodes WHERE node_id=172;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 172;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=172;
/* delete node 173*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (173, 377621810, -1224201081, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 173;
DELETE FROM node_tags WHERE node_id = 173;
DELETE FROM current_way_nodes WHERE node_id=173;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 173;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=173;
/* delete node 174*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (174, 377621974, -1224198881, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 174;
DELETE FROM node_tags WHERE node_id = 174;
DELETE FROM current_way_nodes WHERE node_id=174;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 174;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=174;
/* delete node 175*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (175, 377622907, -1224198901, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 175;
DELETE FROM node_tags WHERE node_id = 175;
DELETE FROM current_way_nodes WHERE node_id=175;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 175;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=175;
/* delete node 176*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (176, 377623384, -1224201181, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 176;
DELETE FROM node_tags WHERE node_id = 176;
DELETE FROM current_way_nodes WHERE node_id=176;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 176;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=176;
/* delete node 177*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (177, 377623522, -1224198982, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 177;
DELETE FROM node_tags WHERE node_id = 177;
DELETE FROM current_way_nodes WHERE node_id=177;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 177;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=177;
/* delete node 178*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (178, 377624752, -1224201356, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 178;
DELETE FROM node_tags WHERE node_id = 178;
DELETE FROM current_way_nodes WHERE node_id=178;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 178;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=178;
/* delete node 179*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (179, 377623925, -1224201262, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 179;
DELETE FROM node_tags WHERE node_id = 179;
DELETE FROM current_way_nodes WHERE node_id=179;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 179;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=179;
/* delete node 180*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (180, 377624058, -1224198955, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 180;
DELETE FROM node_tags WHERE node_id = 180;
DELETE FROM current_way_nodes WHERE node_id=180;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 180;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=180;
/* delete node 181*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (181, 377624890, -1224199009, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 181;
DELETE FROM node_tags WHERE node_id = 181;
DELETE FROM current_way_nodes WHERE node_id=181;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 181;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=181;
/* delete node 182*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (182, 377626369, -1224201570, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 182;
DELETE FROM node_tags WHERE node_id = 182;
DELETE FROM current_way_nodes WHERE node_id=182;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 182;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=182;
/* delete node 183*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (183, 377626491, -1224201423, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 183;
DELETE FROM node_tags WHERE node_id = 183;
DELETE FROM current_way_nodes WHERE node_id=183;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 183;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=183;
/* delete node 184*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (184, 377626576, -1224200511, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 184;
DELETE FROM node_tags WHERE node_id = 184;
DELETE FROM current_way_nodes WHERE node_id=184;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 184;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=184;
/* delete node 185*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (185, 377626517, -1224200464, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 185;
DELETE FROM node_tags WHERE node_id = 185;
DELETE FROM current_way_nodes WHERE node_id=185;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 185;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=185;
/* delete node 186*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (186, 377624815, -1224200275, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 186;
DELETE FROM node_tags WHERE node_id = 186;
DELETE FROM current_way_nodes WHERE node_id=186;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 186;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=186;
/* delete node 187*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (187, 377626544, -1224199491, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 187;
DELETE FROM node_tags WHERE node_id = 187;
DELETE FROM current_way_nodes WHERE node_id=187;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 187;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=187;
/* delete node 188*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (188, 377626162, -1224199444, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 188;
DELETE FROM node_tags WHERE node_id = 188;
DELETE FROM current_way_nodes WHERE node_id=188;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 188;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=188;
/* delete node 189*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (189, 377640034, -1224200820, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 189;
DELETE FROM node_tags WHERE node_id = 189;
DELETE FROM current_way_nodes WHERE node_id=189;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 189;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=189;
/* delete node 190*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (190, 377626130, -1224199498, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 190;
DELETE FROM node_tags WHERE node_id = 190;
DELETE FROM current_way_nodes WHERE node_id=190;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 190;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=190;
/* delete node 191*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (191, 377640274, -1224197179, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 191;
DELETE FROM node_tags WHERE node_id = 191;
DELETE FROM current_way_nodes WHERE node_id=191;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 191;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=191;
/* delete node 192*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (192, 377625372, -1224199431, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 192;
DELETE FROM node_tags WHERE node_id = 192;
DELETE FROM current_way_nodes WHERE node_id=192;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 192;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=192;
/* delete node 193*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (193, 377640676, -1224201229, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 193;
DELETE FROM node_tags WHERE node_id = 193;
DELETE FROM current_way_nodes WHERE node_id=193;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 193;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=193;
/* delete node 194*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (194, 377625372, -1224199377, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 194;
DELETE FROM node_tags WHERE node_id = 194;
DELETE FROM current_way_nodes WHERE node_id=194;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 194;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=194;
/* delete node 195*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (195, 377640699, -1224200890, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 195;
DELETE FROM node_tags WHERE node_id = 195;
DELETE FROM current_way_nodes WHERE node_id=195;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 195;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=195;
/* delete node 196*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (196, 377625033, -1224199290, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 196;
DELETE FROM node_tags WHERE node_id = 196;
DELETE FROM current_way_nodes WHERE node_id=196;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 196;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=196;
/* delete node 197*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (197, 377640939, -1224197247, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 197;
DELETE FROM node_tags WHERE node_id = 197;
DELETE FROM current_way_nodes WHERE node_id=197;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 197;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=197;
/* delete node 198*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (198, 377625107, -1224198593, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 198;
DELETE FROM node_tags WHERE node_id = 198;
DELETE FROM current_way_nodes WHERE node_id=198;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 198;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=198;
/* delete node 199*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (199, 377641385, -1224201305, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 199;
DELETE FROM node_tags WHERE node_id = 199;
DELETE FROM current_way_nodes WHERE node_id=199;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 199;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=199;
/* delete node 200*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (200, 377626607, -1224198680, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 200;
DELETE FROM node_tags WHERE node_id = 200;
DELETE FROM current_way_nodes WHERE node_id=200;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 200;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=200;
/* delete node 201*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (201, 377641650, -1224197308, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 201;
DELETE FROM node_tags WHERE node_id = 201;
DELETE FROM current_way_nodes WHERE node_id=201;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 201;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=201;
/* delete node 202*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (202, 377626729, -1224198774, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 202;
DELETE FROM node_tags WHERE node_id = 202;
DELETE FROM current_way_nodes WHERE node_id=202;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 202;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=202;
/* delete node 203*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (203, 377641954, -1224201365, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 203;
DELETE FROM node_tags WHERE node_id = 203;
DELETE FROM current_way_nodes WHERE node_id=203;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 203;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=203;
/* delete node 204*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (204, 377626708, -1224199035, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 204;
DELETE FROM node_tags WHERE node_id = 204;
DELETE FROM current_way_nodes WHERE node_id=204;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 204;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=204;
/* delete node 205*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (205, 377642041, -1224200053, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 205;
DELETE FROM node_tags WHERE node_id = 205;
DELETE FROM current_way_nodes WHERE node_id=205;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 205;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=205;
/* delete node 206*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (206, 377626613, -1224199062, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 206;
DELETE FROM node_tags WHERE node_id = 206;
DELETE FROM current_way_nodes WHERE node_id=206;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 206;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=206;
/* delete node 207*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (207, 377642219, -1224197369, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 207;
DELETE FROM node_tags WHERE node_id = 207;
DELETE FROM current_way_nodes WHERE node_id=207;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 207;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=207;
/* delete node 208*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (208, 377626692, -1224199156, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 208;
DELETE FROM node_tags WHERE node_id = 208;
DELETE FROM current_way_nodes WHERE node_id=208;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 208;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=208;
/* delete node 209*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (209, 377642603, -1224201530, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 209;
DELETE FROM node_tags WHERE node_id = 209;
DELETE FROM current_way_nodes WHERE node_id=209;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 209;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=209;
/* delete node 210*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (210, 377626676, -1224199391, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 210;
DELETE FROM node_tags WHERE node_id = 210;
DELETE FROM current_way_nodes WHERE node_id=210;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 210;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=210;
/* delete node 211*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (211, 377642685, -1224200121, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 211;
DELETE FROM node_tags WHERE node_id = 211;
DELETE FROM current_way_nodes WHERE node_id=211;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 211;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=211;
/* delete node 212*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (212, 377626628, -1224200374, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 212;
DELETE FROM node_tags WHERE node_id = 212;
DELETE FROM current_way_nodes WHERE node_id=212;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 212;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=212;
/* delete node 213*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (213, 377642863, -1224197437, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 213;
DELETE FROM node_tags WHERE node_id = 213;
DELETE FROM current_way_nodes WHERE node_id=213;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 213;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=213;
/* delete node 214*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (214, 377626636, -1224200139, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 214;
DELETE FROM node_tags WHERE node_id = 214;
DELETE FROM current_way_nodes WHERE node_id=214;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 214;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=214;
/* delete node 215*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (215, 377643282, -1224200993, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 215;
DELETE FROM node_tags WHERE node_id = 215;
DELETE FROM current_way_nodes WHERE node_id=215;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 215;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=215;
/* delete node 216*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (216, 377626565, -1224200054, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 216;
DELETE FROM node_tags WHERE node_id = 216;
DELETE FROM current_way_nodes WHERE node_id=216;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 216;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=216;
/* delete node 217*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (217, 377643322, -1224200339, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 217;
DELETE FROM node_tags WHERE node_id = 217;
DELETE FROM current_way_nodes WHERE node_id=217;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 217;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=217;
/* delete node 218*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (218, 377626653, -1224199985, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 218;
DELETE FROM node_tags WHERE node_id = 218;
DELETE FROM current_way_nodes WHERE node_id=218;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 218;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=218;
/* delete node 219*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (219, 377643499, -1224197491, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 219;
DELETE FROM node_tags WHERE node_id = 219;
DELETE FROM current_way_nodes WHERE node_id=219;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 219;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=219;
/* delete node 220*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (220, 377626657, -1224199740, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 220;
DELETE FROM node_tags WHERE node_id = 220;
DELETE FROM current_way_nodes WHERE node_id=220;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 220;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=220;
/* delete node 221*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (221, 377643576, -1224201626, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 221;
DELETE FROM node_tags WHERE node_id = 221;
DELETE FROM current_way_nodes WHERE node_id=221;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 221;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=221;
/* delete node 222*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (222, 377626594, -1224199665, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 222;
DELETE FROM node_tags WHERE node_id = 222;
DELETE FROM current_way_nodes WHERE node_id=222;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 222;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=222;
/* delete node 223*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (223, 377643613, -1224201026, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 223;
DELETE FROM node_tags WHERE node_id = 223;
DELETE FROM current_way_nodes WHERE node_id=223;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 223;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=223;
/* delete node 224*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (224, 377626173, -1224199622, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 224;
DELETE FROM node_tags WHERE node_id = 224;
DELETE FROM current_way_nodes WHERE node_id=224;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 224;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=224;
/* delete node 225*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (225, 377643950, -1224200795, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 225;
DELETE FROM node_tags WHERE node_id = 225;
DELETE FROM current_way_nodes WHERE node_id=225;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 225;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=225;
/* delete node 226*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (226, 377625225, -1224199511, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 226;
DELETE FROM node_tags WHERE node_id = 226;
DELETE FROM current_way_nodes WHERE node_id=226;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 226;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=226;
/* delete node 227*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (227, 377643974, -1224200403, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 227;
DELETE FROM node_tags WHERE node_id = 227;
DELETE FROM current_way_nodes WHERE node_id=227;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 227;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=227;
/* delete node 228*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (228, 377624943, -1224199511, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 228;
DELETE FROM node_tags WHERE node_id = 228;
DELETE FROM current_way_nodes WHERE node_id=228;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 228;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=228;
/* delete node 229*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (229, 377644095, -1224201110, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 229;
DELETE FROM node_tags WHERE node_id = 229;
DELETE FROM current_way_nodes WHERE node_id=229;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 229;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=229;
/* delete node 230*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (230, 377624904, -1224200285, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 230;
DELETE FROM node_tags WHERE node_id = 230;
DELETE FROM current_way_nodes WHERE node_id=230;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 230;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=230;
/* delete node 231*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (231, 377644114, -1224200811, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 231;
DELETE FROM node_tags WHERE node_id = 231;
DELETE FROM current_way_nodes WHERE node_id=231;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 231;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=231;
/* delete node 232*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (232, 377626968, -1224201411, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 232;
DELETE FROM node_tags WHERE node_id = 232;
DELETE FROM current_way_nodes WHERE node_id=232;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 232;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=232;
/* delete node 233*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (233, 377644151, -1224197556, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 233;
DELETE FROM node_tags WHERE node_id = 233;
DELETE FROM current_way_nodes WHERE node_id=233;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 233;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=233;
/* delete node 234*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (234, 377628615, -1224201571, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 234;
DELETE FROM node_tags WHERE node_id = 234;
DELETE FROM current_way_nodes WHERE node_id=234;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 234;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=234;
/* delete node 235*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (235, 377644358, -1224201136, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 235;
DELETE FROM node_tags WHERE node_id = 235;
DELETE FROM current_way_nodes WHERE node_id=235;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 235;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=235;
/* delete node 236*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (236, 377628628, -1224201443, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 236;
DELETE FROM node_tags WHERE node_id = 236;
DELETE FROM current_way_nodes WHERE node_id=236;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 236;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=236;
/* delete node 237*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (237, 377644364, -1224201039, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 237;
DELETE FROM node_tags WHERE node_id = 237;
DELETE FROM current_way_nodes WHERE node_id=237;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 237;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=237;
/* delete node 238*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (238, 377628522, -1224201433, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 238;
DELETE FROM node_tags WHERE node_id = 238;
DELETE FROM current_way_nodes WHERE node_id=238;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 238;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=238;
/* delete node 239*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (239, 377628539, -1224201102, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 239;
DELETE FROM node_tags WHERE node_id = 239;
DELETE FROM current_way_nodes WHERE node_id=239;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 239;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=239;
/* delete node 240*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (240, 377644626, -1224201065, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 240;
DELETE FROM node_tags WHERE node_id = 240;
DELETE FROM current_way_nodes WHERE node_id=240;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 240;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=240;
/* delete node 241*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (241, 377628316, -1224201071, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 241;
DELETE FROM node_tags WHERE node_id = 241;
DELETE FROM current_way_nodes WHERE node_id=241;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 241;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=241;
/* delete node 242*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (242, 377644629, -1224201029, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 242;
DELETE FROM node_tags WHERE node_id = 242;
DELETE FROM current_way_nodes WHERE node_id=242;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 242;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=242;
/* delete node 243*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (243, 377628346, -1224200660, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 243;
DELETE FROM node_tags WHERE node_id = 243;
DELETE FROM current_way_nodes WHERE node_id=243;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 243;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=243;
/* delete node 244*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (244, 377644684, -1224233483, 3, false, (now() at time zone 'utc'), 1301409555, 2);
DELETE FROM current_node_tags WHERE node_id = 244;
DELETE FROM node_tags WHERE node_id = 244;
DELETE FROM current_way_nodes WHERE node_id=244;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 244;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=244;
/* delete node 245*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (245, 377627040, -1224200522, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 245;
DELETE FROM node_tags WHERE node_id = 245;
DELETE FROM current_way_nodes WHERE node_id=245;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 245;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=245;
/* delete node 247*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (247, 377626909, -1224202285, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 247;
DELETE FROM node_tags WHERE node_id = 247;
DELETE FROM current_way_nodes WHERE node_id=247;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 247;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=247;
/* delete node 248*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (248, 377644749, -1224232421, 3, false, (now() at time zone 'utc'), 1301409555, 2);
DELETE FROM current_node_tags WHERE node_id = 248;
DELETE FROM node_tags WHERE node_id = 248;
DELETE FROM current_way_nodes WHERE node_id=248;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 248;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=248;
/* delete node 249*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (249, 377628548, -1224202450, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 249;
DELETE FROM node_tags WHERE node_id = 249;
DELETE FROM current_way_nodes WHERE node_id=249;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 249;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=249;
/* delete node 251*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (251, 377644840, -1224197624, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 251;
DELETE FROM node_tags WHERE node_id = 251;
DELETE FROM current_way_nodes WHERE node_id=251;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 251;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=251;
/* delete node 252*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (252, 377628535, -1224203361, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 252;
DELETE FROM node_tags WHERE node_id = 252;
DELETE FROM current_way_nodes WHERE node_id=252;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 252;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=252;
/* delete node 253*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (253, 377645232, -1224239576, 3, false, (now() at time zone 'utc'), 1301409555, 2);
DELETE FROM current_node_tags WHERE node_id = 253;
DELETE FROM node_tags WHERE node_id = 253;
DELETE FROM current_way_nodes WHERE node_id=253;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 253;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=253;
/* delete node 254*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (254, 377626978, -1224202292, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 254;
DELETE FROM node_tags WHERE node_id = 254;
DELETE FROM current_way_nodes WHERE node_id=254;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 254;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=254;
/* delete node 255*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (255, 377645262, -1224201092, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 255;
DELETE FROM node_tags WHERE node_id = 255;
DELETE FROM current_way_nodes WHERE node_id=255;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 255;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=255;
/* delete node 256*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (256, 377628645, -1224202482, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 256;
DELETE FROM node_tags WHERE node_id = 256;
DELETE FROM current_way_nodes WHERE node_id=256;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 256;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=256;
/* delete node 258*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (258, 377628577, -1224203239, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 258;
DELETE FROM node_tags WHERE node_id = 258;
DELETE FROM current_way_nodes WHERE node_id=258;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 258;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=258;
/* delete node 259*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (259, 377645267, -1224231473, 3, false, (now() at time zone 'utc'), 1301409555, 2);
DELETE FROM current_node_tags WHERE node_id = 259;
DELETE FROM node_tags WHERE node_id = 259;
DELETE FROM current_way_nodes WHERE node_id=259;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 259;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=259;
/* delete node 261*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (261, 377646488, -1224197150, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 261;
DELETE FROM node_tags WHERE node_id = 261;
DELETE FROM current_way_nodes WHERE node_id=261;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 261;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=261;
/* delete node 262*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (262, 377645269, -1224200974, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 262;
DELETE FROM node_tags WHERE node_id = 262;
DELETE FROM current_way_nodes WHERE node_id=262;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 262;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=262;
/* delete node 264*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (264, 377646234, -1224201241, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 264;
DELETE FROM node_tags WHERE node_id = 264;
DELETE FROM current_way_nodes WHERE node_id=264;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 264;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=264;
/* delete node 269*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (269, 377649621, -1224201577, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 269;
DELETE FROM node_tags WHERE node_id = 269;
DELETE FROM current_way_nodes WHERE node_id=269;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 269;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=269;
/* delete node 275*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (275, 377645444, -1224232489, 3, false, (now() at time zone 'utc'), 1301409555, 2);
DELETE FROM current_node_tags WHERE node_id = 275;
DELETE FROM node_tags WHERE node_id = 275;
DELETE FROM current_way_nodes WHERE node_id=275;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 275;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=275;
/* delete node 277*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (277, 377649875, -1224197486, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 277;
DELETE FROM node_tags WHERE node_id = 277;
DELETE FROM current_way_nodes WHERE node_id=277;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 277;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=277;
/* delete node 278*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (278, 377645474, -1224197687, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 278;
DELETE FROM node_tags WHERE node_id = 278;
DELETE FROM current_way_nodes WHERE node_id=278;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 278;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=278;
/* delete node 281*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (281, 377645505, -1224231496, 3, false, (now() at time zone 'utc'), 1301409555, 2);
DELETE FROM current_node_tags WHERE node_id = 281;
DELETE FROM node_tags WHERE node_id = 281;
DELETE FROM current_way_nodes WHERE node_id=281;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 281;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=281;
/* delete node 284*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (284, 377645572, -1224233712, 3, false, (now() at time zone 'utc'), 1301409555, 2);
DELETE FROM current_node_tags WHERE node_id = 284;
DELETE FROM node_tags WHERE node_id = 284;
DELETE FROM current_way_nodes WHERE node_id=284;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 284;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=284;
/* delete node 289*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (289, 377646245, -1224201071, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 289;
DELETE FROM node_tags WHERE node_id = 289;
DELETE FROM current_way_nodes WHERE node_id=289;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 289;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=289;
/* delete node 291*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (291, 377646449, -1224197784, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 291;
DELETE FROM node_tags WHERE node_id = 291;
DELETE FROM current_way_nodes WHERE node_id=291;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 291;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=291;
/* delete node 296*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (296, 377646947, -1224239735, 3, false, (now() at time zone 'utc'), 1301409555, 2);
DELETE FROM current_node_tags WHERE node_id = 296;
DELETE FROM node_tags WHERE node_id = 296;
DELETE FROM current_way_nodes WHERE node_id=296;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 296;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=296;
/* delete node 298*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (298, 377649170, -1224218859, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 298;
DELETE FROM node_tags WHERE node_id = 298;
DELETE FROM current_way_nodes WHERE node_id=298;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 298;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=298;
/* delete node 299*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (299, 377647287, -1224233871, 3, false, (now() at time zone 'utc'), 1301409555, 2);
DELETE FROM current_node_tags WHERE node_id = 299;
DELETE FROM node_tags WHERE node_id = 299;
DELETE FROM current_way_nodes WHERE node_id=299;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 299;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=299;
/* delete node 301*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (301, 377647292, -1224233739, 3, false, (now() at time zone 'utc'), 1301409555, 2);
DELETE FROM current_node_tags WHERE node_id = 301;
DELETE FROM node_tags WHERE node_id = 301;
DELETE FROM current_way_nodes WHERE node_id=301;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 301;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=301;
/* delete node 303*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (303, 377647357, -1224232677, 3, false, (now() at time zone 'utc'), 1301409555, 2);
DELETE FROM current_node_tags WHERE node_id = 303;
DELETE FROM node_tags WHERE node_id = 303;
DELETE FROM current_way_nodes WHERE node_id=303;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 303;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=303;
/* delete node 305*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (305, 377647418, -1224231684, 3, false, (now() at time zone 'utc'), 1301409555, 2);
DELETE FROM current_node_tags WHERE node_id = 305;
DELETE FROM node_tags WHERE node_id = 305;
DELETE FROM current_way_nodes WHERE node_id=305;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 305;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=305;
/* delete node 307*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (307, 377650498, -1224196888, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 307;
DELETE FROM node_tags WHERE node_id = 307;
DELETE FROM current_way_nodes WHERE node_id=307;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 307;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=307;
/* delete node 312*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (312, 377619023, -1224197942, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 312;
DELETE FROM node_tags WHERE node_id = 312;
DELETE FROM current_way_nodes WHERE node_id=312;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 312;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=312;
/* delete node 315*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (315, 377620647, -1224198101, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 315;
DELETE FROM node_tags WHERE node_id = 315;
DELETE FROM current_way_nodes WHERE node_id=315;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 315;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=315;
/* delete node 318*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (318, 377622112, -1224198245, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 318;
DELETE FROM node_tags WHERE node_id = 318;
DELETE FROM current_way_nodes WHERE node_id=318;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 318;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=318;
/* delete node 320*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (320, 377622125, -1224198039, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 320;
DELETE FROM node_tags WHERE node_id = 320;
DELETE FROM current_way_nodes WHERE node_id=320;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 320;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=320;
/* delete node 323*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (323, 377622743, -1224198345, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 323;
DELETE FROM node_tags WHERE node_id = 323;
DELETE FROM current_way_nodes WHERE node_id=323;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 323;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=323;
/* delete node 325*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (325, 377622758, -1224198101, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 325;
DELETE FROM node_tags WHERE node_id = 325;
DELETE FROM current_way_nodes WHERE node_id=325;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 325;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=325;
/* delete node 328*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (328, 377623421, -1224198619, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 328;
DELETE FROM node_tags WHERE node_id = 328;
DELETE FROM current_way_nodes WHERE node_id=328;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 328;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=328;
/* delete node 330*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (330, 377623434, -1224198413, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 330;
DELETE FROM node_tags WHERE node_id = 330;
DELETE FROM current_way_nodes WHERE node_id=330;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 330;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=330;
/* delete node 334*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (334, 377624137, -1224198689, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 334;
DELETE FROM node_tags WHERE node_id = 334;
DELETE FROM current_way_nodes WHERE node_id=334;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 334;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=334;
/* delete node 336*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (336, 377631680, -1224239099, 3, false, (now() at time zone 'utc'), 1301409554, 2);
DELETE FROM current_node_tags WHERE node_id = 336;
DELETE FROM node_tags WHERE node_id = 336;
DELETE FROM current_way_nodes WHERE node_id=336;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 336;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=336;
/* delete node 337*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (337, 377624153, -1224198430, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 337;
DELETE FROM node_tags WHERE node_id = 337;
DELETE FROM current_way_nodes WHERE node_id=337;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 337;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=337;
/* delete node 340*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (340, 377624872, -1224198500, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 340;
DELETE FROM node_tags WHERE node_id = 340;
DELETE FROM current_way_nodes WHERE node_id=340;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 340;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=340;
/* delete node 343*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (343, 377625543, -1224198566, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 343;
DELETE FROM node_tags WHERE node_id = 343;
DELETE FROM current_way_nodes WHERE node_id=343;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 343;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=343;
/* delete node 346*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (346, 377626427, -1224198653, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 346;
DELETE FROM node_tags WHERE node_id = 346;
DELETE FROM current_way_nodes WHERE node_id=346;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 346;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=346;
/* delete node 349*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (349, 377627323, -1224199595, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 349;
DELETE FROM node_tags WHERE node_id = 349;
DELETE FROM current_way_nodes WHERE node_id=349;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 349;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=349;
/* delete node 351*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (351, 377627368, -1224198802, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 351;
DELETE FROM node_tags WHERE node_id = 351;
DELETE FROM current_way_nodes WHERE node_id=351;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 351;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=351;
/* delete node 353*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (353, 377627407, -1224198105, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 353;
DELETE FROM node_tags WHERE node_id = 353;
DELETE FROM current_way_nodes WHERE node_id=353;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 353;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=353;
/* delete node 356*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (356, 377628179, -1224198876, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 356;
DELETE FROM node_tags WHERE node_id = 356;
DELETE FROM current_way_nodes WHERE node_id=356;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 356;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=356;
/* delete node 358*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (358, 377628186, -1224198750, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 358;
DELETE FROM node_tags WHERE node_id = 358;
DELETE FROM current_way_nodes WHERE node_id=358;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 358;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=358;
/* delete node 360*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (360, 377628218, -1224198178, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 360;
DELETE FROM node_tags WHERE node_id = 360;
DELETE FROM current_way_nodes WHERE node_id=360;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 360;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=360;
/* delete node 362*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (362, 377628227, -1224198018, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 362;
DELETE FROM node_tags WHERE node_id = 362;
DELETE FROM current_way_nodes WHERE node_id=362;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 362;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=362;
/* delete node 364*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (364, 377628288, -1224196936, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 364;
DELETE FROM node_tags WHERE node_id = 364;
DELETE FROM current_way_nodes WHERE node_id=364;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 364;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=364;
/* delete node 366*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (366, 377628324, -1224198027, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 366;
DELETE FROM node_tags WHERE node_id = 366;
DELETE FROM current_way_nodes WHERE node_id=366;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 366;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=366;
/* delete node 368*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (368, 377628337, -1224196067, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 368;
DELETE FROM node_tags WHERE node_id = 368;
DELETE FROM current_way_nodes WHERE node_id=368;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 368;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=368;
/* delete node 370*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (370, 377628385, -1224196945, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 370;
DELETE FROM node_tags WHERE node_id = 370;
DELETE FROM current_way_nodes WHERE node_id=370;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 370;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=370;
/* delete node 372*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (372, 377628442, -1224198899, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 372;
DELETE FROM node_tags WHERE node_id = 372;
DELETE FROM current_way_nodes WHERE node_id=372;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 372;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=372;
/* delete node 374*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (374, 377628449, -1224198774, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 374;
DELETE FROM node_tags WHERE node_id = 374;
DELETE FROM current_way_nodes WHERE node_id=374;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 374;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=374;
/* delete node 376*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (376, 377629138, -1224198962, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 376;
DELETE FROM node_tags WHERE node_id = 376;
DELETE FROM current_way_nodes WHERE node_id=376;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 376;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=376;
/* delete node 377*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (377, 377647024, -1224217173, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 377;
DELETE FROM node_tags WHERE node_id = 377;
DELETE FROM current_way_nodes WHERE node_id=377;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 377;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=377;
/* delete node 378*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (378, 377629143, -1224198868, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 378;
DELETE FROM node_tags WHERE node_id = 378;
DELETE FROM current_way_nodes WHERE node_id=378;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 378;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=378;
/* delete node 380*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (380, 377629346, -1224198132, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 380;
DELETE FROM node_tags WHERE node_id = 380;
DELETE FROM current_way_nodes WHERE node_id=380;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 380;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=380;
/* delete node 383*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (383, 377629408, -1224197049, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 383;
DELETE FROM node_tags WHERE node_id = 383;
DELETE FROM current_way_nodes WHERE node_id=383;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 383;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=383;
/* delete node 385*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (385, 377629447, -1224198896, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 385;
DELETE FROM node_tags WHERE node_id = 385;
DELETE FROM current_way_nodes WHERE node_id=385;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 385;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=385;
/* delete node 387*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (387, 377629480, -1224198315, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 387;
DELETE FROM node_tags WHERE node_id = 387;
DELETE FROM current_way_nodes WHERE node_id=387;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 387;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=387;
/* delete node 389*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (389, 377629490, -1224198145, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 389;
DELETE FROM node_tags WHERE node_id = 389;
DELETE FROM current_way_nodes WHERE node_id=389;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 389;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=389;
/* delete node 391*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (391, 377629551, -1224197062, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 391;
DELETE FROM node_tags WHERE node_id = 391;
DELETE FROM current_way_nodes WHERE node_id=391;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 391;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=391;
/* delete node 393*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (393, 377629600, -1224196181, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 393;
DELETE FROM node_tags WHERE node_id = 393;
DELETE FROM current_way_nodes WHERE node_id=393;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 393;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=393;
/* delete node 395*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (395, 377630034, -1224199839, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 395;
DELETE FROM node_tags WHERE node_id = 395;
DELETE FROM current_way_nodes WHERE node_id=395;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 395;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=395;
/* delete node 397*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (397, 377630079, -1224199047, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 397;
DELETE FROM node_tags WHERE node_id = 397;
DELETE FROM current_way_nodes WHERE node_id=397;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 397;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=397;
/* delete node 399*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (399, 377630149, -1224198376, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 399;
DELETE FROM node_tags WHERE node_id = 399;
DELETE FROM current_way_nodes WHERE node_id=399;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 399;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=399;
/* delete node 401*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (401, 377630270, -1224196241, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 401;
DELETE FROM node_tags WHERE node_id = 401;
DELETE FROM current_way_nodes WHERE node_id=401;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 401;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=401;
/* delete node 403*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (403, 377635703, -1224199105, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 403;
DELETE FROM node_tags WHERE node_id = 403;
DELETE FROM current_way_nodes WHERE node_id=403;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 403;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=403;
/* delete node 405*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (405, 377636725, -1224200355, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 405;
DELETE FROM node_tags WHERE node_id = 405;
DELETE FROM current_way_nodes WHERE node_id=405;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 405;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=405;
/* delete node 407*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (407, 377636800, -1224199220, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 407;
DELETE FROM node_tags WHERE node_id = 407;
DELETE FROM current_way_nodes WHERE node_id=407;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 407;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=407;
/* delete node 409*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (409, 377636956, -1224196858, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 409;
DELETE FROM node_tags WHERE node_id = 409;
DELETE FROM current_way_nodes WHERE node_id=409;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 409;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=409;
/* delete node 411*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (411, 377637889, -1224200477, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 411;
DELETE FROM node_tags WHERE node_id = 411;
DELETE FROM current_way_nodes WHERE node_id=411;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 411;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=411;
/* delete node 413*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (413, 377637902, -1224200283, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 413;
DELETE FROM node_tags WHERE node_id = 413;
DELETE FROM current_way_nodes WHERE node_id=413;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 413;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=413;
/* delete node 415*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (415, 377638150, -1224200309, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 415;
DELETE FROM node_tags WHERE node_id = 415;
DELETE FROM current_way_nodes WHERE node_id=415;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 415;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=415;
/* delete node 418*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (418, 377638163, -1224200107, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 418;
DELETE FROM node_tags WHERE node_id = 418;
DELETE FROM current_way_nodes WHERE node_id=418;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 418;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=418;
/* delete node 421*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (421, 377638368, -1224197007, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 421;
DELETE FROM node_tags WHERE node_id = 421;
DELETE FROM current_way_nodes WHERE node_id=421;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 421;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=421;
/* delete node 424*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (424, 377638807, -1224200216, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 424;
DELETE FROM node_tags WHERE node_id = 424;
DELETE FROM current_way_nodes WHERE node_id=424;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 424;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=424;
/* delete node 427*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (427, 377638810, -1224200175, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 427;
DELETE FROM node_tags WHERE node_id = 427;
DELETE FROM current_way_nodes WHERE node_id=427;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 427;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=427;
/* delete node 429*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (429, 377638999, -1224197070, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 429;
DELETE FROM node_tags WHERE node_id = 429;
DELETE FROM current_way_nodes WHERE node_id=429;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 429;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=429;
/* delete node 431*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (431, 377639417, -1224200280, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 431;
DELETE FROM node_tags WHERE node_id = 431;
DELETE FROM current_way_nodes WHERE node_id=431;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 431;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=431;
/* delete node 433*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (433, 377639422, -1224200217, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 433;
DELETE FROM node_tags WHERE node_id = 433;
DELETE FROM current_way_nodes WHERE node_id=433;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 433;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=433;
/* delete node 435*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (435, 377639625, -1224197136, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 435;
DELETE FROM node_tags WHERE node_id = 435;
DELETE FROM current_way_nodes WHERE node_id=435;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 435;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=435;
/* delete node 437*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (437, 377640069, -1224200285, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 437;
DELETE FROM node_tags WHERE node_id = 437;
DELETE FROM current_way_nodes WHERE node_id=437;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 437;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=437;
/* delete node 448*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (448, 377650685, -1224211805, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 448;
DELETE FROM node_tags WHERE node_id = 448;
DELETE FROM current_way_nodes WHERE node_id=448;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 448;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=448;
/* delete node 450*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (450, 377650571, -1224213438, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 450;
DELETE FROM node_tags WHERE node_id = 450;
DELETE FROM current_way_nodes WHERE node_id=450;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 450;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=450;
/* delete node 452*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (452, 377640647, -1224202133, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 452;
DELETE FROM node_tags WHERE node_id = 452;
DELETE FROM current_way_nodes WHERE node_id=452;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 452;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=452;
/* delete node 454*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (454, 377642613, -1224202243, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 454;
DELETE FROM node_tags WHERE node_id = 454;
DELETE FROM current_way_nodes WHERE node_id=454;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 454;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=454;
/* delete node 464*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (464, 377649310, -1224232383, 3, false, (now() at time zone 'utc'), 1301409555, 2);
DELETE FROM current_node_tags WHERE node_id = 464;
DELETE FROM node_tags WHERE node_id = 464;
DELETE FROM current_way_nodes WHERE node_id=464;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 464;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=464;
/* delete node 480*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (480, 377646257, -1224239611, 3, false, (now() at time zone 'utc'), 1301409555, 2);
DELETE FROM current_node_tags WHERE node_id = 480;
DELETE FROM node_tags WHERE node_id = 480;
DELETE FROM current_way_nodes WHERE node_id=480;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 480;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=480;
/* delete node 483*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (483, 377647708, -1224224622, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 483;
DELETE FROM node_tags WHERE node_id = 483;
DELETE FROM current_way_nodes WHERE node_id=483;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 483;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=483;
/* delete node 485*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (485, 377647314, -1224229830, 3, false, (now() at time zone 'utc'), 1301409555, 2);
DELETE FROM current_node_tags WHERE node_id = 485;
DELETE FROM node_tags WHERE node_id = 485;
DELETE FROM current_way_nodes WHERE node_id=485;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 485;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=485;
/* delete node 486*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (486, 377647462, -1224200305, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 486;
DELETE FROM node_tags WHERE node_id = 486;
DELETE FROM current_way_nodes WHERE node_id=486;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 486;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=486;
/* delete node 487*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (487, 377647530, -1224221129, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 487;
DELETE FROM node_tags WHERE node_id = 487;
DELETE FROM current_way_nodes WHERE node_id=487;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 487;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=487;
/* delete node 488*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (488, 377618153, -1224237098, 3, false, (now() at time zone 'utc'), 1301409554, 2);
DELETE FROM current_node_tags WHERE node_id = 488;
DELETE FROM node_tags WHERE node_id = 488;
DELETE FROM current_way_nodes WHERE node_id=488;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 488;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=488;
/* delete node 489*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (489, 377650072, -1224201691, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 489;
DELETE FROM node_tags WHERE node_id = 489;
DELETE FROM current_way_nodes WHERE node_id=489;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 489;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=489;
/* delete node 490*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (490, 377627740, -1224196291, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 490;
DELETE FROM node_tags WHERE node_id = 490;
DELETE FROM current_way_nodes WHERE node_id=490;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 490;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=490;
/* delete node 498*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (498, 377629855, -1224199709, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 498;
DELETE FROM node_tags WHERE node_id = 498;
DELETE FROM current_way_nodes WHERE node_id=498;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 498;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=498;
/* delete node 501*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (501, 377649632, -1224236142, 3, false, (now() at time zone 'utc'), 1301409555, 2);
DELETE FROM current_node_tags WHERE node_id = 501;
DELETE FROM node_tags WHERE node_id = 501;
DELETE FROM current_way_nodes WHERE node_id=501;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 501;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=501;
/* delete node 502*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (502, 377649400, -1224200276, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 502;
DELETE FROM node_tags WHERE node_id = 502;
DELETE FROM current_way_nodes WHERE node_id=502;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 502;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=502;
/* delete node 503*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (503, 377649198, -1224202861, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 503;
DELETE FROM node_tags WHERE node_id = 503;
DELETE FROM current_way_nodes WHERE node_id=503;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 503;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=503;
/* delete node 504*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (504, 377649327, -1224203539, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 504;
DELETE FROM node_tags WHERE node_id = 504;
DELETE FROM current_way_nodes WHERE node_id=504;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 504;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=504;
/* delete node 505*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (505, 377649446, -1224201696, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 505;
DELETE FROM node_tags WHERE node_id = 505;
DELETE FROM current_way_nodes WHERE node_id=505;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 505;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=505;
/* create node 506*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (506, 377639810, -1224204099, 3, true, (now() at time zone 'utc'), 1301409561, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (506, 377639810, -1224204099, 3, true, (now() at time zone 'utc'), 1301409561, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (506, 'hoot:id', '506');
INSERT INTO node_tags (node_id, k, v, version) VALUES (506, 'hoot:id', '506', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (506, 'hoot:status', '2');
INSERT INTO node_tags (node_id, k, v, version) VALUES (506, 'hoot:status', '2', 1);
/* create node 507*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (507, 377643100, -1224204399, 3, true, (now() at time zone 'utc'), 1301409561, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (507, 377643100, -1224204399, 3, true, (now() at time zone 'utc'), 1301409561, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (507, 'hoot:id', '507');
INSERT INTO node_tags (node_id, k, v, version) VALUES (507, 'hoot:id', '507', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (507, 'hoot:status', '2');
INSERT INTO node_tags (node_id, k, v, version) VALUES (507, 'hoot:status', '2', 1);
/* create node 510*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (510, 377622798, -1224229161, 3, true, (now() at time zone 'utc'), 1301409554, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (510, 377622798, -1224229161, 3, true, (now() at time zone 'utc'), 1301409554, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (510, 'hoot:id', '510');
INSERT INTO node_tags (node_id, k, v, version) VALUES (510, 'hoot:id', '510', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (510, 'hoot:status', '2');
INSERT INTO node_tags (node_id, k, v, version) VALUES (510, 'hoot:status', '2', 1);
/* create node 511*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (511, 377624550, -1224229329, 3, true, (now() at time zone 'utc'), 1301409554, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (511, 377624550, -1224229329, 3, true, (now() at time zone 'utc'), 1301409554, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (511, 'hoot:id', '511');
INSERT INTO node_tags (node_id, k, v, version) VALUES (511, 'hoot:id', '511', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (511, 'hoot:status', '2');
INSERT INTO node_tags (node_id, k, v, version) VALUES (511, 'hoot:status', '2', 1);
/* create node 512*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (512, 377624769, -1224225707, 3, true, (now() at time zone 'utc'), 1301409554, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (512, 377624769, -1224225707, 3, true, (now() at time zone 'utc'), 1301409554, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (512, 'hoot:id', '512');
INSERT INTO node_tags (node_id, k, v, version) VALUES (512, 'hoot:id', '512', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (512, 'hoot:status', '2');
INSERT INTO node_tags (node_id, k, v, version) VALUES (512, 'hoot:status', '2', 1);
/* create node 513*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (513, 377623016, -1224225539, 3, true, (now() at time zone 'utc'), 1301409554, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (513, 377623016, -1224225539, 3, true, (now() at time zone 'utc'), 1301409554, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (513, 'hoot:id', '513');
INSERT INTO node_tags (node_id, k, v, version) VALUES (513, 'hoot:id', '513', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (513, 'hoot:status', '2');
INSERT INTO node_tags (node_id, k, v, version) VALUES (513, 'hoot:status', '2', 1);
/* create node 514*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (514, 377639986, -1224201197, 3, true, (now() at time zone 'utc'), 1301409561, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (514, 377639986, -1224201197, 3, true, (now() at time zone 'utc'), 1301409561, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (514, 'hoot:id', '514');
INSERT INTO node_tags (node_id, k, v, version) VALUES (514, 'hoot:id', '514', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (514, 'hoot:status', '2');
INSERT INTO node_tags (node_id, k, v, version) VALUES (514, 'hoot:status', '2', 1);
/* create node 515*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (515, 377643276, -1224201497, 3, true, (now() at time zone 'utc'), 1301409561, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (515, 377643276, -1224201497, 3, true, (now() at time zone 'utc'), 1301409561, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (515, 'hoot:id', '515');
INSERT INTO node_tags (node_id, k, v, version) VALUES (515, 'hoot:id', '515', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (515, 'hoot:status', '2');
INSERT INTO node_tags (node_id, k, v, version) VALUES (515, 'hoot:status', '2', 1);
/* create node 516*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (516, 377642535, -1224201429, 3, true, (now() at time zone 'utc'), 1301409561, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (516, 377642535, -1224201429, 3, true, (now() at time zone 'utc'), 1301409561, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (516, 'hoot:id', '516');
INSERT INTO node_tags (node_id, k, v, version) VALUES (516, 'hoot:id', '516', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (516, 'hoot:status', '2');
INSERT INTO node_tags (node_id, k, v, version) VALUES (516, 'hoot:status', '2', 1);
/* create node 517*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (517, 377641891, -1224201371, 3, true, (now() at time zone 'utc'), 1301409561, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (517, 377641891, -1224201371, 3, true, (now() at time zone 'utc'), 1301409561, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (517, 'hoot:id', '517');
INSERT INTO node_tags (node_id, k, v, version) VALUES (517, 'hoot:id', '517', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (517, 'hoot:status', '2');
INSERT INTO node_tags (node_id, k, v, version) VALUES (517, 'hoot:status', '2', 1);
/* create node 518*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (518, 377641248, -1224201312, 3, true, (now() at time zone 'utc'), 1301409561, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (518, 377641248, -1224201312, 3, true, (now() at time zone 'utc'), 1301409561, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (518, 'hoot:id', '518');
INSERT INTO node_tags (node_id, k, v, version) VALUES (518, 'hoot:id', '518', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (518, 'hoot:status', '2');
INSERT INTO node_tags (node_id, k, v, version) VALUES (518, 'hoot:status', '2', 1);
/* create node 519*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (519, 377640604, -1224201253, 3, true, (now() at time zone 'utc'), 1301409561, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (519, 377640604, -1224201253, 3, true, (now() at time zone 'utc'), 1301409561, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (519, 'hoot:id', '519');
INSERT INTO node_tags (node_id, k, v, version) VALUES (519, 'hoot:id', '519', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (519, 'hoot:status', '2');
INSERT INTO node_tags (node_id, k, v, version) VALUES (519, 'hoot:status', '2', 1);
/* delete way 2*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (2, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 2;
DELETE FROM way_tags WHERE way_id = 2;
DELETE FROM current_way_nodes WHERE way_id=2;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 2;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=2;
/* modify way 3*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (3, 3, true, (now() at time zone 'utc'), 2);
UPDATE current_ways SET changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), version=2 WHERE id=3;
DELETE FROM current_way_tags WHERE way_id = 3;
DELETE FROM way_tags WHERE way_id = 3;
INSERT INTO current_way_tags (way_id, k, v) VALUES (3, 'gnis:feature_id', '2107373');
INSERT INTO way_tags (way_id, k, v, version) VALUES (3, 'gnis:feature_id', '2107373', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (3, 'addr:city', 'San Francisco');
INSERT INTO way_tags (way_id, k, v, version) VALUES (3, 'addr:city', 'San Francisco', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (3, 'hoot:status', '3');
INSERT INTO way_tags (way_id, k, v, version) VALUES (3, 'hoot:status', '3', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (3, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (3, 'building', 'yes', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (3, 'ele', '14');
INSERT INTO way_tags (way_id, k, v, version) VALUES (3, 'ele', '14', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (3, 'addr:housenumber', '630');
INSERT INTO way_tags (way_id, k, v, version) VALUES (3, 'addr:housenumber', '630', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (3, 'name', 'Mission Police Station');
INSERT INTO way_tags (way_id, k, v, version) VALUES (3, 'name', 'Mission Police Station', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (3, 'alt_name', 'MISSION POLICE STATION');
INSERT INTO way_tags (way_id, k, v, version) VALUES (3, 'alt_name', 'MISSION POLICE STATION', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (3, 'amenity', 'police');
INSERT INTO way_tags (way_id, k, v, version) VALUES (3, 'amenity', 'police', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (3, 'hoot:poipolygon:poismerged', '1');
INSERT INTO way_tags (way_id, k, v, version) VALUES (3, 'hoot:poipolygon:poismerged', '1', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (3, 'wheelchair', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (3, 'wheelchair', 'yes', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (3, 'gnis:county_name', 'San Francisco');
INSERT INTO way_tags (way_id, k, v, version) VALUES (3, 'gnis:county_name', 'San Francisco', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (3, 'REF1', '007037');
INSERT INTO way_tags (way_id, k, v, version) VALUES (3, 'REF1', '007037', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (3, 'REF2', '007037');
INSERT INTO way_tags (way_id, k, v, version) VALUES (3, 'REF2', '007037', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (3, 'addr:state', 'CA');
INSERT INTO way_tags (way_id, k, v, version) VALUES (3, 'addr:state', 'CA', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (3, 'addr:street', 'Valencia Street');
INSERT INTO way_tags (way_id, k, v, version) VALUES (3, 'addr:street', 'Valencia Street', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (3, 'hoot:id', '3');
INSERT INTO way_tags (way_id, k, v, version) VALUES (3, 'hoot:id', '3', 2);
DELETE FROM current_way_nodes WHERE way_id = 3;
DELETE FROM way_nodes WHERE way_id = 3;
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (3, 500, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (3, 500, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (3, 499, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (3, 499, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (3, 497, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (3, 497, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (3, 458, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (3, 458, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (3, 460, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (3, 460, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (3, 494, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (3, 494, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (3, 495, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (3, 495, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (3, 496, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (3, 496, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (3, 493, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (3, 493, 9);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (3, 492, 1, 10);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (3, 492, 10);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (3, 491, 1, 11);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (3, 491, 11);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (3, 500, 1, 12);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (3, 500, 12);
/* delete way 5*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (5, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 5;
DELETE FROM way_tags WHERE way_id = 5;
DELETE FROM current_way_nodes WHERE way_id=5;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 5;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=5;
/* delete way 6*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (6, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 6;
DELETE FROM way_tags WHERE way_id = 6;
DELETE FROM current_way_nodes WHERE way_id=6;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 6;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=6;
/* delete way 7*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (7, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 7;
DELETE FROM way_tags WHERE way_id = 7;
DELETE FROM current_way_nodes WHERE way_id=7;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 7;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=7;
/* delete way 8*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (8, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 8;
DELETE FROM way_tags WHERE way_id = 8;
DELETE FROM current_way_nodes WHERE way_id=8;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 8;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=8;
/* delete way 9*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (9, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 9;
DELETE FROM way_tags WHERE way_id = 9;
DELETE FROM current_way_nodes WHERE way_id=9;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 9;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=9;
/* delete way 10*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (10, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 10;
DELETE FROM way_tags WHERE way_id = 10;
DELETE FROM current_way_nodes WHERE way_id=10;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 10;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=10;
/* delete way 11*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (11, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 11;
DELETE FROM way_tags WHERE way_id = 11;
DELETE FROM current_way_nodes WHERE way_id=11;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 11;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=11;
/* delete way 12*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (12, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 12;
DELETE FROM way_tags WHERE way_id = 12;
DELETE FROM current_way_nodes WHERE way_id=12;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 12;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=12;
/* delete way 13*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (13, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 13;
DELETE FROM way_tags WHERE way_id = 13;
DELETE FROM current_way_nodes WHERE way_id=13;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 13;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=13;
/* delete way 14*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (14, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 14;
DELETE FROM way_tags WHERE way_id = 14;
DELETE FROM current_way_nodes WHERE way_id=14;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 14;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=14;
/* delete way 16*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (16, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 16;
DELETE FROM way_tags WHERE way_id = 16;
DELETE FROM current_way_nodes WHERE way_id=16;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 16;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=16;
/* delete way 19*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (19, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 19;
DELETE FROM way_tags WHERE way_id = 19;
DELETE FROM current_way_nodes WHERE way_id=19;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 19;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=19;
/* delete way 20*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (20, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 20;
DELETE FROM way_tags WHERE way_id = 20;
DELETE FROM current_way_nodes WHERE way_id=20;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 20;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=20;
/* delete way 23*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (23, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 23;
DELETE FROM way_tags WHERE way_id = 23;
DELETE FROM current_way_nodes WHERE way_id=23;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 23;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=23;
/* delete way 25*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (25, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 25;
DELETE FROM way_tags WHERE way_id = 25;
DELETE FROM current_way_nodes WHERE way_id=25;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 25;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=25;
/* delete way 27*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (27, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 27;
DELETE FROM way_tags WHERE way_id = 27;
DELETE FROM current_way_nodes WHERE way_id=27;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 27;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=27;
/* delete way 28*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (28, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 28;
DELETE FROM way_tags WHERE way_id = 28;
DELETE FROM current_way_nodes WHERE way_id=28;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 28;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=28;
/* delete way 29*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (29, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 29;
DELETE FROM way_tags WHERE way_id = 29;
DELETE FROM current_way_nodes WHERE way_id=29;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 29;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=29;
/* delete way 30*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (30, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 30;
DELETE FROM way_tags WHERE way_id = 30;
DELETE FROM current_way_nodes WHERE way_id=30;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 30;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=30;
/* delete way 31*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (31, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 31;
DELETE FROM way_tags WHERE way_id = 31;
DELETE FROM current_way_nodes WHERE way_id=31;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 31;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=31;
/* delete way 32*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (32, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 32;
DELETE FROM way_tags WHERE way_id = 32;
DELETE FROM current_way_nodes WHERE way_id=32;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 32;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=32;
/* delete way 33*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (33, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 33;
DELETE FROM way_tags WHERE way_id = 33;
DELETE FROM current_way_nodes WHERE way_id=33;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 33;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=33;
/* delete way 34*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (34, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 34;
DELETE FROM way_tags WHERE way_id = 34;
DELETE FROM current_way_nodes WHERE way_id=34;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 34;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=34;
/* delete way 35*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (35, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 35;
DELETE FROM way_tags WHERE way_id = 35;
DELETE FROM current_way_nodes WHERE way_id=35;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 35;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=35;
/* delete way 63*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (63, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 63;
DELETE FROM way_tags WHERE way_id = 63;
DELETE FROM current_way_nodes WHERE way_id=63;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 63;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=63;
/* delete way 64*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (64, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 64;
DELETE FROM way_tags WHERE way_id = 64;
DELETE FROM current_way_nodes WHERE way_id=64;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 64;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=64;
/* modify way 66*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (66, 3, true, (now() at time zone 'utc'), 2);
UPDATE current_ways SET changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), version=2 WHERE id=66;
DELETE FROM current_way_tags WHERE way_id = 66;
DELETE FROM way_tags WHERE way_id = 66;
INSERT INTO current_way_tags (way_id, k, v) VALUES (66, 'REF1', '00435f');
INSERT INTO way_tags (way_id, k, v, version) VALUES (66, 'REF1', '00435f', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (66, 'REF2', '00435f');
INSERT INTO way_tags (way_id, k, v, version) VALUES (66, 'REF2', '00435f', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (66, 'hoot:poipolygon:poismerged', '1');
INSERT INTO way_tags (way_id, k, v, version) VALUES (66, 'hoot:poipolygon:poismerged', '1', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (66, 'hoot:id', '66');
INSERT INTO way_tags (way_id, k, v, version) VALUES (66, 'hoot:id', '66', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (66, 'hoot:status', '3');
INSERT INTO way_tags (way_id, k, v, version) VALUES (66, 'hoot:status', '3', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (66, 'alt_name', '16TH & HOFF ST. PARKING GARAGE');
INSERT INTO way_tags (way_id, k, v, version) VALUES (66, 'alt_name', '16TH & HOFF ST. PARKING GARAGE', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (66, 'name', 'Hoff Street Garage');
INSERT INTO way_tags (way_id, k, v, version) VALUES (66, 'name', 'Hoff Street Garage', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (66, 'amenity', 'parking');
INSERT INTO way_tags (way_id, k, v, version) VALUES (66, 'amenity', 'parking', 2);
DELETE FROM current_way_nodes WHERE way_id = 66;
DELETE FROM way_nodes WHERE way_id = 66;
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (66, 438, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (66, 438, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (66, 440, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (66, 440, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (66, 55, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (66, 55, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (66, 442, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (66, 442, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (66, 444, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (66, 444, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (66, 438, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (66, 438, 6);
/* delete way 68*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (68, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 68;
DELETE FROM way_tags WHERE way_id = 68;
DELETE FROM current_way_nodes WHERE way_id=68;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 68;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=68;
/* delete way 72*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (72, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 72;
DELETE FROM way_tags WHERE way_id = 72;
DELETE FROM current_way_nodes WHERE way_id=72;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 72;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=72;
/* delete way 74*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (74, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 74;
DELETE FROM way_tags WHERE way_id = 74;
DELETE FROM current_way_nodes WHERE way_id=74;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 74;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=74;
/* delete way 76*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (76, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 76;
DELETE FROM way_tags WHERE way_id = 76;
DELETE FROM current_way_nodes WHERE way_id=76;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 76;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=76;
/* delete way 79*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (79, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 79;
DELETE FROM way_tags WHERE way_id = 79;
DELETE FROM current_way_nodes WHERE way_id=79;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 79;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=79;
/* delete way 81*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (81, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 81;
DELETE FROM way_tags WHERE way_id = 81;
DELETE FROM current_way_nodes WHERE way_id=81;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 81;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=81;
/* delete way 83*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (83, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 83;
DELETE FROM way_tags WHERE way_id = 83;
DELETE FROM current_way_nodes WHERE way_id=83;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 83;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=83;
/* delete way 84*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (84, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 84;
DELETE FROM way_tags WHERE way_id = 84;
DELETE FROM current_way_nodes WHERE way_id=84;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 84;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=84;
/* delete way 85*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (85, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 85;
DELETE FROM way_tags WHERE way_id = 85;
DELETE FROM current_way_nodes WHERE way_id=85;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 85;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=85;
/* delete way 86*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (86, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 86;
DELETE FROM way_tags WHERE way_id = 86;
DELETE FROM current_way_nodes WHERE way_id=86;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 86;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=86;
/* delete way 87*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (87, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 87;
DELETE FROM way_tags WHERE way_id = 87;
DELETE FROM current_way_nodes WHERE way_id=87;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 87;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=87;
/* delete way 88*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (88, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 88;
DELETE FROM way_tags WHERE way_id = 88;
DELETE FROM current_way_nodes WHERE way_id=88;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 88;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=88;
/* delete way 90*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (90, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 90;
DELETE FROM way_tags WHERE way_id = 90;
DELETE FROM current_way_nodes WHERE way_id=90;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 90;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=90;
/* delete way 91*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (91, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 91;
DELETE FROM way_tags WHERE way_id = 91;
DELETE FROM current_way_nodes WHERE way_id=91;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 91;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=91;
/* delete way 92*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (92, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 92;
DELETE FROM way_tags WHERE way_id = 92;
DELETE FROM current_way_nodes WHERE way_id=92;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 92;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=92;
/* delete way 93*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (93, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 93;
DELETE FROM way_tags WHERE way_id = 93;
DELETE FROM current_way_nodes WHERE way_id=93;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 93;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=93;
/* delete way 94*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (94, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 94;
DELETE FROM way_tags WHERE way_id = 94;
DELETE FROM current_way_nodes WHERE way_id=94;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 94;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=94;
/* delete way 95*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (95, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 95;
DELETE FROM way_tags WHERE way_id = 95;
DELETE FROM current_way_nodes WHERE way_id=95;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 95;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=95;
/* delete way 96*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (96, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 96;
DELETE FROM way_tags WHERE way_id = 96;
DELETE FROM current_way_nodes WHERE way_id=96;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 96;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=96;
/* delete way 97*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (97, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 97;
DELETE FROM way_tags WHERE way_id = 97;
DELETE FROM current_way_nodes WHERE way_id=97;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 97;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=97;
/* delete way 98*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (98, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 98;
DELETE FROM way_tags WHERE way_id = 98;
DELETE FROM current_way_nodes WHERE way_id=98;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 98;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=98;
/* delete way 99*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (99, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 99;
DELETE FROM way_tags WHERE way_id = 99;
DELETE FROM current_way_nodes WHERE way_id=99;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 99;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=99;
/* delete way 100*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (100, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 100;
DELETE FROM way_tags WHERE way_id = 100;
DELETE FROM current_way_nodes WHERE way_id=100;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 100;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=100;
/* delete way 101*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (101, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 101;
DELETE FROM way_tags WHERE way_id = 101;
DELETE FROM current_way_nodes WHERE way_id=101;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 101;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=101;
/* create way 102*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (102, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (102, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (102, 'REF2', 'none');
INSERT INTO way_tags (way_id, k, v, version) VALUES (102, 'REF2', 'none', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (102, 'hoot:id', '102');
INSERT INTO way_tags (way_id, k, v, version) VALUES (102, 'hoot:id', '102', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (102, 'hoot:status', '2');
INSERT INTO way_tags (way_id, k, v, version) VALUES (102, 'hoot:status', '2', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (102, 'area', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (102, 'area', 'yes', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (102, 'name', 'Kidpower Park');
INSERT INTO way_tags (way_id, k, v, version) VALUES (102, 'name', 'Kidpower Park', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (102, 514, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (102, 514, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (102, 506, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (102, 506, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (102, 507, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (102, 507, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (102, 515, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (102, 515, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (102, 516, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (102, 516, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (102, 517, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (102, 517, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (102, 518, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (102, 518, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (102, 519, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (102, 519, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (102, 514, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (102, 514, 9);
/* create way 103*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (103, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (103, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (103, 'REF2', 'none');
INSERT INTO way_tags (way_id, k, v, version) VALUES (103, 'REF2', 'none', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (103, 'hoot:id', '103');
INSERT INTO way_tags (way_id, k, v, version) VALUES (103, 'hoot:id', '103', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (103, 'hoot:status', '2');
INSERT INTO way_tags (way_id, k, v, version) VALUES (103, 'hoot:status', '2', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (103, 'area', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (103, 'area', 'yes', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (103, 'name', 'Dearborn Community Garden');
INSERT INTO way_tags (way_id, k, v, version) VALUES (103, 'name', 'Dearborn Community Garden', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (103, 510, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (103, 510, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (103, 511, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (103, 511, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (103, 512, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (103, 512, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (103, 513, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (103, 513, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (103, 510, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (103, 510, 5);
UPDATE changesets SET min_lat=377618000, max_lat=377651000, min_lon=-1224239735, max_lon=-1224195999, num_changes=334 WHERE id=3;
