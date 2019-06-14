INSERT INTO changesets (id, user_id, created_at, closed_at) VALUES (2, 1, (now() at time zone 'utc'), (now() at time zone 'utc'));
INSERT INTO changeset_tags (changeset_id, k, v) VALUES (2, 'written_by', 'Hootenanny');
/* delete node 27*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (27, 43316, -12426, 2, false, (now() at time zone 'utc'), 1789569707, 2);
DELETE FROM current_node_tags WHERE node_id = 27;
DELETE FROM node_tags WHERE node_id = 27;
DELETE FROM current_way_nodes WHERE node_id=27;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 27;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=27;
/* delete node 31*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (31, 17885, -55328, 2, false, (now() at time zone 'utc'), 1789569704, 2);
DELETE FROM current_node_tags WHERE node_id = 31;
DELETE FROM node_tags WHERE node_id = 31;
DELETE FROM current_way_nodes WHERE node_id=31;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 31;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=31;
/* delete node 32*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (32, 43401, -40372, 2, false, (now() at time zone 'utc'), 1789569707, 2);
DELETE FROM current_node_tags WHERE node_id = 32;
DELETE FROM node_tags WHERE node_id = 32;
DELETE FROM current_way_nodes WHERE node_id=32;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 32;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=32;
/* delete node 52*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (52, 69608, 6070, 2, false, (now() at time zone 'utc'), 3221225476, 2);
DELETE FROM current_node_tags WHERE node_id = 52;
DELETE FROM node_tags WHERE node_id = 52;
DELETE FROM current_way_nodes WHERE node_id=52;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 52;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=52;
/* delete node 53*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (53, 67272, -5887, 2, false, (now() at time zone 'utc'), 1789569710, 2);
DELETE FROM current_node_tags WHERE node_id = 53;
DELETE FROM node_tags WHERE node_id = 53;
DELETE FROM current_way_nodes WHERE node_id=53;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 53;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=53;
/* delete node 80*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (80, 8178, -60285, 2, false, (now() at time zone 'utc'), 1789569704, 2);
DELETE FROM current_node_tags WHERE node_id = 80;
DELETE FROM node_tags WHERE node_id = 80;
DELETE FROM current_way_nodes WHERE node_id=80;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 80;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=80;
/* delete node 86*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (86, 31160, -48749, 2, false, (now() at time zone 'utc'), 1789569707, 2);
DELETE FROM current_node_tags WHERE node_id = 86;
DELETE FROM node_tags WHERE node_id = 86;
DELETE FROM current_way_nodes WHERE node_id=86;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 86;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=86;
/* delete node 99*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (99, 41845, -62756, 2, false, (now() at time zone 'utc'), 1789569705, 2);
DELETE FROM current_node_tags WHERE node_id = 99;
DELETE FROM node_tags WHERE node_id = 99;
DELETE FROM current_way_nodes WHERE node_id=99;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 99;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=99;
/* delete node 100*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (100, 80562, -47607, 2, false, (now() at time zone 'utc'), 1789569710, 2);
DELETE FROM current_node_tags WHERE node_id = 100;
DELETE FROM node_tags WHERE node_id = 100;
DELETE FROM current_way_nodes WHERE node_id=100;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 100;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=100;
/* delete node 114*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (114, 68267, -50843, 2, false, (now() at time zone 'utc'), 1789569710, 2);
DELETE FROM current_node_tags WHERE node_id = 114;
DELETE FROM node_tags WHERE node_id = 114;
DELETE FROM current_way_nodes WHERE node_id=114;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 114;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=114;
/* delete node 115*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (115, 66716, -13551, 2, false, (now() at time zone 'utc'), 1789569710, 2);
DELETE FROM current_node_tags WHERE node_id = 115;
DELETE FROM node_tags WHERE node_id = 115;
DELETE FROM current_way_nodes WHERE node_id=115;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 115;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=115;
/* delete node 123*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (123, 15981, -63241, 2, false, (now() at time zone 'utc'), 1789569704, 2);
DELETE FROM current_node_tags WHERE node_id = 123;
DELETE FROM node_tags WHERE node_id = 123;
DELETE FROM current_way_nodes WHERE node_id=123;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 123;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=123;
/* delete node 129*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (129, 38570, -64589, 2, false, (now() at time zone 'utc'), 1789569705, 2);
DELETE FROM current_node_tags WHERE node_id = 129;
DELETE FROM node_tags WHERE node_id = 129;
DELETE FROM current_way_nodes WHERE node_id=129;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 129;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=129;
/* delete node 142*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (142, 81243, -44034, 2, false, (now() at time zone 'utc'), 1789569710, 2);
DELETE FROM current_node_tags WHERE node_id = 142;
DELETE FROM node_tags WHERE node_id = 142;
DELETE FROM current_way_nodes WHERE node_id=142;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 142;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=142;
/* delete node 161*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (161, 31469, -68167, 2, false, (now() at time zone 'utc'), 1789569705, 2);
DELETE FROM current_node_tags WHERE node_id = 161;
DELETE FROM node_tags WHERE node_id = 161;
DELETE FROM current_way_nodes WHERE node_id=161;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 161;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=161;
/* delete node 162*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (162, 81443, -46586, 2, false, (now() at time zone 'utc'), 1789569710, 2);
DELETE FROM current_node_tags WHERE node_id = 162;
DELETE FROM node_tags WHERE node_id = 162;
DELETE FROM current_way_nodes WHERE node_id=162;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 162;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=162;
/* delete node 186*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (186, 54175, -34429, 2, false, (now() at time zone 'utc'), 1789569707, 2);
DELETE FROM current_node_tags WHERE node_id = 186;
DELETE FROM node_tags WHERE node_id = 186;
DELETE FROM current_way_nodes WHERE node_id=186;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 186;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=186;
/* delete node 190*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (190, 13348, -64377, 2, false, (now() at time zone 'utc'), 1789569704, 2);
DELETE FROM current_node_tags WHERE node_id = 190;
DELETE FROM node_tags WHERE node_id = 190;
DELETE FROM current_way_nodes WHERE node_id=190;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 190;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=190;
/* delete node 202*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (202, 59209, -80741, 2, false, (now() at time zone 'utc'), 1789569708, 2);
DELETE FROM current_node_tags WHERE node_id = 202;
DELETE FROM node_tags WHERE node_id = 202;
DELETE FROM current_way_nodes WHERE node_id=202;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 202;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=202;
/* delete node 206*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (206, 79559, -40397, 2, false, (now() at time zone 'utc'), 1789569710, 2);
DELETE FROM current_node_tags WHERE node_id = 206;
DELETE FROM node_tags WHERE node_id = 206;
DELETE FROM current_way_nodes WHERE node_id=206;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 206;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=206;
/* delete node 220*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (220, 56261, -75557, 2, false, (now() at time zone 'utc'), 1789569708, 2);
DELETE FROM current_node_tags WHERE node_id = 220;
DELETE FROM node_tags WHERE node_id = 220;
DELETE FROM current_way_nodes WHERE node_id=220;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 220;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=220;
/* delete node 229*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (229, 76486, -31363, 2, false, (now() at time zone 'utc'), 1789569710, 2);
DELETE FROM current_node_tags WHERE node_id = 229;
DELETE FROM node_tags WHERE node_id = 229;
DELETE FROM current_way_nodes WHERE node_id=229;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 229;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=229;
/* delete node 232*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (232, 71580, -26249, 2, false, (now() at time zone 'utc'), 1789569710, 2);
DELETE FROM current_node_tags WHERE node_id = 232;
DELETE FROM node_tags WHERE node_id = 232;
DELETE FROM current_way_nodes WHERE node_id=232;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 232;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=232;
/* delete node 236*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (236, 52133, -35195, 2, false, (now() at time zone 'utc'), 1789569707, 2);
DELETE FROM current_node_tags WHERE node_id = 236;
DELETE FROM node_tags WHERE node_id = 236;
DELETE FROM current_way_nodes WHERE node_id=236;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 236;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=236;
/* delete node 248*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (248, 73153, -32846, 2, false, (now() at time zone 'utc'), 1789569710, 2);
DELETE FROM current_node_tags WHERE node_id = 248;
DELETE FROM node_tags WHERE node_id = 248;
DELETE FROM current_way_nodes WHERE node_id=248;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 248;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=248;
/* delete node 265*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (265, 11029, -61848, 2, false, (now() at time zone 'utc'), 1789569704, 2);
DELETE FROM current_node_tags WHERE node_id = 265;
DELETE FROM node_tags WHERE node_id = 265;
DELETE FROM current_way_nodes WHERE node_id=265;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 265;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=265;
/* delete node 267*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (267, 14696, -64020, 2, false, (now() at time zone 'utc'), 1789569704, 2);
DELETE FROM current_node_tags WHERE node_id = 267;
DELETE FROM node_tags WHERE node_id = 267;
DELETE FROM current_way_nodes WHERE node_id=267;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 267;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=267;
/* delete node 305*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (305, 69291, -22672, 2, false, (now() at time zone 'utc'), 1789569710, 2);
DELETE FROM current_node_tags WHERE node_id = 305;
DELETE FROM node_tags WHERE node_id = 305;
DELETE FROM current_way_nodes WHERE node_id=305;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 305;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=305;
/* delete node 310*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (310, 6352, -59946, 2, false, (now() at time zone 'utc'), 1789569704, 2);
DELETE FROM current_node_tags WHERE node_id = 310;
DELETE FROM node_tags WHERE node_id = 310;
DELETE FROM current_way_nodes WHERE node_id=310;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 310;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=310;
/* delete node 313*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (313, 38667, -10757, 2, false, (now() at time zone 'utc'), 1789569707, 2);
DELETE FROM current_node_tags WHERE node_id = 313;
DELETE FROM node_tags WHERE node_id = 313;
DELETE FROM current_way_nodes WHERE node_id=313;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 313;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=313;
/* delete node 332*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (332, 53367, -27935, 2, false, (now() at time zone 'utc'), 1789569707, 2);
DELETE FROM current_node_tags WHERE node_id = 332;
DELETE FROM node_tags WHERE node_id = 332;
DELETE FROM current_way_nodes WHERE node_id=332;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 332;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=332;
/* delete node 345*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (345, 67928, 11096, 2, false, (now() at time zone 'utc'), 3221225476, 2);
DELETE FROM current_node_tags WHERE node_id = 345;
DELETE FROM node_tags WHERE node_id = 345;
DELETE FROM current_way_nodes WHERE node_id=345;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 345;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=345;
/* delete node 353*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (353, 26999, -50989, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 353;
DELETE FROM node_tags WHERE node_id = 353;
DELETE FROM current_way_nodes WHERE node_id=353;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 353;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=353;
/* delete node 366*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (366, 66374, -8629, 2, false, (now() at time zone 'utc'), 1789569710, 2);
DELETE FROM current_node_tags WHERE node_id = 366;
DELETE FROM node_tags WHERE node_id = 366;
DELETE FROM current_way_nodes WHERE node_id=366;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 366;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=366;
/* delete node 385*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (385, 53275, -34975, 2, false, (now() at time zone 'utc'), 1789569707, 2);
DELETE FROM current_node_tags WHERE node_id = 385;
DELETE FROM node_tags WHERE node_id = 385;
DELETE FROM current_way_nodes WHERE node_id=385;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 385;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=385;
/* delete node 395*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (395, 45619, -52652, 2, false, (now() at time zone 'utc'), 1789569707, 2);
DELETE FROM current_node_tags WHERE node_id = 395;
DELETE FROM node_tags WHERE node_id = 395;
DELETE FROM current_way_nodes WHERE node_id=395;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 395;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=395;
/* delete node 406*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (406, 47120, -15343, 2, false, (now() at time zone 'utc'), 1789569707, 2);
DELETE FROM current_node_tags WHERE node_id = 406;
DELETE FROM node_tags WHERE node_id = 406;
DELETE FROM current_way_nodes WHERE node_id=406;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 406;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=406;
/* delete node 420*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (420, 68913, -4, 2, false, (now() at time zone 'utc'), 1789569710, 2);
DELETE FROM current_node_tags WHERE node_id = 420;
DELETE FROM node_tags WHERE node_id = 420;
DELETE FROM current_way_nodes WHERE node_id=420;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 420;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=420;
/* delete node 451*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (451, 54752, -73684, 2, false, (now() at time zone 'utc'), 1789569705, 2);
DELETE FROM current_node_tags WHERE node_id = 451;
DELETE FROM node_tags WHERE node_id = 451;
DELETE FROM current_way_nodes WHERE node_id=451;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 451;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=451;
/* delete node 472*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (472, 69673, 1994, 2, false, (now() at time zone 'utc'), 3221225476, 2);
DELETE FROM current_node_tags WHERE node_id = 472;
DELETE FROM node_tags WHERE node_id = 472;
DELETE FROM current_way_nodes WHERE node_id=472;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 472;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=472;
/* delete node 474*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (474, 40551, -14959, 2, false, (now() at time zone 'utc'), 1789569707, 2);
DELETE FROM current_node_tags WHERE node_id = 474;
DELETE FROM node_tags WHERE node_id = 474;
DELETE FROM current_way_nodes WHERE node_id=474;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 474;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=474;
/* delete node 477*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (477, 80715, -42947, 2, false, (now() at time zone 'utc'), 1789569710, 2);
DELETE FROM current_node_tags WHERE node_id = 477;
DELETE FROM node_tags WHERE node_id = 477;
DELETE FROM current_way_nodes WHERE node_id=477;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 477;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=477;
/* delete node 480*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (480, 58052, -78530, 2, false, (now() at time zone 'utc'), 1789569708, 2);
DELETE FROM current_node_tags WHERE node_id = 480;
DELETE FROM node_tags WHERE node_id = 480;
DELETE FROM current_way_nodes WHERE node_id=480;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 480;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=480;
/* delete node 487*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (487, 68735, 8972, 2, false, (now() at time zone 'utc'), 3221225476, 2);
DELETE FROM current_node_tags WHERE node_id = 487;
DELETE FROM node_tags WHERE node_id = 487;
DELETE FROM current_way_nodes WHERE node_id=487;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 487;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=487;
/* delete node 497*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (497, 52695, -70953, 2, false, (now() at time zone 'utc'), 1789569705, 2);
DELETE FROM current_node_tags WHERE node_id = 497;
DELETE FROM node_tags WHERE node_id = 497;
DELETE FROM current_way_nodes WHERE node_id=497;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 497;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=497;
/* delete node 505*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (505, 74421, -48981, 2, false, (now() at time zone 'utc'), 1789569710, 2);
DELETE FROM current_node_tags WHERE node_id = 505;
DELETE FROM node_tags WHERE node_id = 505;
DELETE FROM current_way_nodes WHERE node_id=505;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 505;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=505;
/* delete node 515*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (515, 76099, -49134, 2, false, (now() at time zone 'utc'), 1789569710, 2);
DELETE FROM current_node_tags WHERE node_id = 515;
DELETE FROM node_tags WHERE node_id = 515;
DELETE FROM current_way_nodes WHERE node_id=515;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 515;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=515;
/* delete node 527*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (527, 23527, -61870, 2, false, (now() at time zone 'utc'), 1789569704, 2);
DELETE FROM current_node_tags WHERE node_id = 527;
DELETE FROM node_tags WHERE node_id = 527;
DELETE FROM current_way_nodes WHERE node_id=527;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 527;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=527;
/* delete node 530*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (530, 45680, -52799, 2, false, (now() at time zone 'utc'), 1789569707, 2);
DELETE FROM current_node_tags WHERE node_id = 530;
DELETE FROM node_tags WHERE node_id = 530;
DELETE FROM current_way_nodes WHERE node_id=530;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 530;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=530;
/* delete node 546*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (546, 68244, -2322, 2, false, (now() at time zone 'utc'), 1789569710, 2);
DELETE FROM current_node_tags WHERE node_id = 546;
DELETE FROM node_tags WHERE node_id = 546;
DELETE FROM current_way_nodes WHERE node_id=546;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 546;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=546;
/* delete node 563*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (563, 75455, -31823, 2, false, (now() at time zone 'utc'), 1789569710, 2);
DELETE FROM current_node_tags WHERE node_id = 563;
DELETE FROM node_tags WHERE node_id = 563;
DELETE FROM current_way_nodes WHERE node_id=563;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 563;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=563;
/* delete node 595*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (595, 18072, -61992, 2, false, (now() at time zone 'utc'), 1789569704, 2);
DELETE FROM current_node_tags WHERE node_id = 595;
DELETE FROM node_tags WHERE node_id = 595;
DELETE FROM current_way_nodes WHERE node_id=595;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 595;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=595;
/* delete node 602*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (602, 55528, -39325, 2, false, (now() at time zone 'utc'), 1789569710, 2);
DELETE FROM current_node_tags WHERE node_id = 602;
DELETE FROM node_tags WHERE node_id = 602;
DELETE FROM current_way_nodes WHERE node_id=602;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 602;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=602;
/* delete node 603*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (603, 40519, -43368, 2, false, (now() at time zone 'utc'), 1789569707, 2);
DELETE FROM current_node_tags WHERE node_id = 603;
DELETE FROM node_tags WHERE node_id = 603;
DELETE FROM current_way_nodes WHERE node_id=603;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 603;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=603;
/* delete node 605*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (605, 51330, -60063, 2, false, (now() at time zone 'utc'), 1789569705, 2);
DELETE FROM current_node_tags WHERE node_id = 605;
DELETE FROM node_tags WHERE node_id = 605;
DELETE FROM current_way_nodes WHERE node_id=605;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 605;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=605;
/* delete node 608*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (608, 8767, -60610, 2, false, (now() at time zone 'utc'), 1789569704, 2);
DELETE FROM current_node_tags WHERE node_id = 608;
DELETE FROM node_tags WHERE node_id = 608;
DELETE FROM current_way_nodes WHERE node_id=608;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 608;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=608;
/* delete node 622*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (622, 50590, -35852, 2, false, (now() at time zone 'utc'), 1789569707, 2);
DELETE FROM current_node_tags WHERE node_id = 622;
DELETE FROM node_tags WHERE node_id = 622;
DELETE FROM current_way_nodes WHERE node_id=622;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 622;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=622;
/* delete node 634*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (634, 69811, 4941, 2, false, (now() at time zone 'utc'), 3221225476, 2);
DELETE FROM current_node_tags WHERE node_id = 634;
DELETE FROM node_tags WHERE node_id = 634;
DELETE FROM current_way_nodes WHERE node_id=634;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 634;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=634;
/* delete node 654*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (654, 47844, -17254, 2, false, (now() at time zone 'utc'), 1789569707, 2);
DELETE FROM current_node_tags WHERE node_id = 654;
DELETE FROM node_tags WHERE node_id = 654;
DELETE FROM current_way_nodes WHERE node_id=654;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 654;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=654;
/* delete node 668*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (668, 41556, -14287, 2, false, (now() at time zone 'utc'), 1789569707, 2);
DELETE FROM current_node_tags WHERE node_id = 668;
DELETE FROM node_tags WHERE node_id = 668;
DELETE FROM current_way_nodes WHERE node_id=668;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 668;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=668;
/* delete node 694*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (694, 66720, -51970, 2, false, (now() at time zone 'utc'), 1789569710, 2);
DELETE FROM current_node_tags WHERE node_id = 694;
DELETE FROM node_tags WHERE node_id = 694;
DELETE FROM current_way_nodes WHERE node_id=694;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 694;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=694;
/* delete node 703*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (703, 13963, -64302, 2, false, (now() at time zone 'utc'), 1789569704, 2);
DELETE FROM current_node_tags WHERE node_id = 703;
DELETE FROM node_tags WHERE node_id = 703;
DELETE FROM current_way_nodes WHERE node_id=703;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 703;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=703;
/* delete node 721*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (721, 2794, -60110, 2, false, (now() at time zone 'utc'), 1789569704, 2);
DELETE FROM current_node_tags WHERE node_id = 721;
DELETE FROM node_tags WHERE node_id = 721;
DELETE FROM current_way_nodes WHERE node_id=721;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 721;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=721;
/* delete node 778*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (778, 77894, -37125, 2, false, (now() at time zone 'utc'), 1789569710, 2);
DELETE FROM current_node_tags WHERE node_id = 778;
DELETE FROM node_tags WHERE node_id = 778;
DELETE FROM current_way_nodes WHERE node_id=778;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 778;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=778;
/* delete node 780*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (780, 25194, -61710, 2, false, (now() at time zone 'utc'), 1789569704, 2);
DELETE FROM current_node_tags WHERE node_id = 780;
DELETE FROM node_tags WHERE node_id = 780;
DELETE FROM current_way_nodes WHERE node_id=780;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 780;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=780;
/* delete node 784*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (784, 69286, 7477, 2, false, (now() at time zone 'utc'), 3221225476, 2);
DELETE FROM current_node_tags WHERE node_id = 784;
DELETE FROM node_tags WHERE node_id = 784;
DELETE FROM current_way_nodes WHERE node_id=784;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 784;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=784;
/* delete node 794*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (794, 81712, -46145, 2, false, (now() at time zone 'utc'), 1789569710, 2);
DELETE FROM current_node_tags WHERE node_id = 794;
DELETE FROM node_tags WHERE node_id = 794;
DELETE FROM current_way_nodes WHERE node_id=794;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 794;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=794;
/* delete node 801*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (801, 70142, -24029, 2, false, (now() at time zone 'utc'), 1789569710, 2);
DELETE FROM current_node_tags WHERE node_id = 801;
DELETE FROM node_tags WHERE node_id = 801;
DELETE FROM current_way_nodes WHERE node_id=801;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 801;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=801;
/* delete node 821*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (821, 53818, -72652, 2, false, (now() at time zone 'utc'), 1789569705, 2);
DELETE FROM current_node_tags WHERE node_id = 821;
DELETE FROM node_tags WHERE node_id = 821;
DELETE FROM current_way_nodes WHERE node_id=821;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 821;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=821;
/* delete node 835*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (835, 25079, -51881, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 835;
DELETE FROM node_tags WHERE node_id = 835;
DELETE FROM current_way_nodes WHERE node_id=835;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 835;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=835;
/* delete node 847*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (847, 78314, -48722, 2, false, (now() at time zone 'utc'), 1789569710, 2);
DELETE FROM current_node_tags WHERE node_id = 847;
DELETE FROM node_tags WHERE node_id = 847;
DELETE FROM current_way_nodes WHERE node_id=847;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 847;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=847;
/* delete node 849*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (849, 47090, -60959, 2, false, (now() at time zone 'utc'), 1789569705, 2);
DELETE FROM current_node_tags WHERE node_id = 849;
DELETE FROM node_tags WHERE node_id = 849;
DELETE FROM current_way_nodes WHERE node_id=849;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 849;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=849;
/* delete node 888*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (888, 12864, -64240, 2, false, (now() at time zone 'utc'), 1789569704, 2);
DELETE FROM current_node_tags WHERE node_id = 888;
DELETE FROM node_tags WHERE node_id = 888;
DELETE FROM current_way_nodes WHERE node_id=888;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 888;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=888;
/* delete node 899*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (899, 13115, -58047, 2, false, (now() at time zone 'utc'), 1789569704, 2);
DELETE FROM current_node_tags WHERE node_id = 899;
DELETE FROM node_tags WHERE node_id = 899;
DELETE FROM current_way_nodes WHERE node_id=899;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 899;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=899;
/* delete node 909*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (909, 30336, -59841, 2, false, (now() at time zone 'utc'), 1789569705, 2);
DELETE FROM current_node_tags WHERE node_id = 909;
DELETE FROM node_tags WHERE node_id = 909;
DELETE FROM current_way_nodes WHERE node_id=909;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 909;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=909;
/* delete node 913*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (913, 69364, -50242, 2, false, (now() at time zone 'utc'), 1789569710, 2);
DELETE FROM current_node_tags WHERE node_id = 913;
DELETE FROM node_tags WHERE node_id = 913;
DELETE FROM current_way_nodes WHERE node_id=913;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 913;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=913;
/* delete node 922*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (922, 47813, -62794, 2, false, (now() at time zone 'utc'), 1789569705, 2);
DELETE FROM current_node_tags WHERE node_id = 922;
DELETE FROM node_tags WHERE node_id = 922;
DELETE FROM current_way_nodes WHERE node_id=922;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 922;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=922;
/* delete node 937*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (937, 81681, -45248, 2, false, (now() at time zone 'utc'), 1789569710, 2);
DELETE FROM current_node_tags WHERE node_id = 937;
DELETE FROM node_tags WHERE node_id = 937;
DELETE FROM current_way_nodes WHERE node_id=937;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 937;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=937;
/* delete node 962*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (962, 77003, -49153, 2, false, (now() at time zone 'utc'), 1789569710, 2);
DELETE FROM current_node_tags WHERE node_id = 962;
DELETE FROM node_tags WHERE node_id = 962;
DELETE FROM current_way_nodes WHERE node_id=962;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 962;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=962;
/* delete node 968*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (968, 44861, -12435, 2, false, (now() at time zone 'utc'), 1789569707, 2);
DELETE FROM current_node_tags WHERE node_id = 968;
DELETE FROM node_tags WHERE node_id = 968;
DELETE FROM current_way_nodes WHERE node_id=968;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 968;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=968;
/* delete node 998*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (998, 9832, -60902, 2, false, (now() at time zone 'utc'), 1789569704, 2);
DELETE FROM current_node_tags WHERE node_id = 998;
DELETE FROM node_tags WHERE node_id = 998;
DELETE FROM current_way_nodes WHERE node_id=998;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 998;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=998;
/* delete node 1004*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1004, 11815, -63229, 2, false, (now() at time zone 'utc'), 1789569704, 2);
DELETE FROM current_node_tags WHERE node_id = 1004;
DELETE FROM node_tags WHERE node_id = 1004;
DELETE FROM current_way_nodes WHERE node_id=1004;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 1004;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=1004;
/* delete node 1006*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1006, 71191, -33956, 2, false, (now() at time zone 'utc'), 1789569710, 2);
DELETE FROM current_node_tags WHERE node_id = 1006;
DELETE FROM node_tags WHERE node_id = 1006;
DELETE FROM current_way_nodes WHERE node_id=1006;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 1006;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=1006;
/* delete node 1018*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1018, 71633, -49234, 2, false, (now() at time zone 'utc'), 1789569710, 2);
DELETE FROM current_node_tags WHERE node_id = 1018;
DELETE FROM node_tags WHERE node_id = 1018;
DELETE FROM current_way_nodes WHERE node_id=1018;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 1018;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=1018;
/* delete node 1032*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1032, 47156, -37246, 2, false, (now() at time zone 'utc'), 1789569707, 2);
DELETE FROM current_node_tags WHERE node_id = 1032;
DELETE FROM node_tags WHERE node_id = 1032;
DELETE FROM current_way_nodes WHERE node_id=1032;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 1032;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=1032;
/* delete node 1041*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1041, 79589, -48208, 2, false, (now() at time zone 'utc'), 1789569710, 2);
DELETE FROM current_node_tags WHERE node_id = 1041;
DELETE FROM node_tags WHERE node_id = 1041;
DELETE FROM current_way_nodes WHERE node_id=1041;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 1041;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=1041;
/* delete node 1044*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1044, 23651, -52675, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 1044;
DELETE FROM node_tags WHERE node_id = 1044;
DELETE FROM current_way_nodes WHERE node_id=1044;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 1044;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=1044;
/* delete node 1092*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1092, 36459, -57591, 2, false, (now() at time zone 'utc'), 1789569705, 2);
DELETE FROM current_node_tags WHERE node_id = 1092;
DELETE FROM node_tags WHERE node_id = 1092;
DELETE FROM current_way_nodes WHERE node_id=1092;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 1092;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=1092;
/* delete node 1096*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1096, 63762, -40946, 2, false, (now() at time zone 'utc'), 1789569710, 2);
DELETE FROM current_node_tags WHERE node_id = 1096;
DELETE FROM node_tags WHERE node_id = 1096;
DELETE FROM current_way_nodes WHERE node_id=1096;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 1096;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=1096;
/* delete node 1107*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1107, 44747, -50446, 2, false, (now() at time zone 'utc'), 1789569707, 2);
DELETE FROM current_node_tags WHERE node_id = 1107;
DELETE FROM node_tags WHERE node_id = 1107;
DELETE FROM current_way_nodes WHERE node_id=1107;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 1107;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=1107;
/* delete node 1120*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1120, 40854, -14999, 2, false, (now() at time zone 'utc'), 1789569707, 2);
DELETE FROM current_node_tags WHERE node_id = 1120;
DELETE FROM node_tags WHERE node_id = 1120;
DELETE FROM current_way_nodes WHERE node_id=1120;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 1120;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=1120;
/* delete node 1121*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1121, 54527, -30624, 2, false, (now() at time zone 'utc'), 1789569707, 2);
DELETE FROM current_node_tags WHERE node_id = 1121;
DELETE FROM node_tags WHERE node_id = 1121;
DELETE FROM current_way_nodes WHERE node_id=1121;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 1121;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=1121;
/* delete node 1135*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1135, 54606, -34009, 2, false, (now() at time zone 'utc'), 1789569707, 2);
DELETE FROM current_node_tags WHERE node_id = 1135;
DELETE FROM node_tags WHERE node_id = 1135;
DELETE FROM current_way_nodes WHERE node_id=1135;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 1135;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=1135;
/* delete node 1151*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1151, 54701, -31923, 2, false, (now() at time zone 'utc'), 1789569707, 2);
DELETE FROM current_node_tags WHERE node_id = 1151;
DELETE FROM node_tags WHERE node_id = 1151;
DELETE FROM current_way_nodes WHERE node_id=1151;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 1151;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=1151;
/* delete node 1153*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1153, 67368, -16542, 2, false, (now() at time zone 'utc'), 1789569710, 2);
DELETE FROM current_node_tags WHERE node_id = 1153;
DELETE FROM node_tags WHERE node_id = 1153;
DELETE FROM current_way_nodes WHERE node_id=1153;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 1153;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=1153;
/* delete node 1173*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1173, 66668, -12938, 2, false, (now() at time zone 'utc'), 1789569710, 2);
DELETE FROM current_node_tags WHERE node_id = 1173;
DELETE FROM node_tags WHERE node_id = 1173;
DELETE FROM current_way_nodes WHERE node_id=1173;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 1173;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=1173;
/* delete node 1185*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1185, 44416, -49617, 2, false, (now() at time zone 'utc'), 1789569707, 2);
DELETE FROM current_node_tags WHERE node_id = 1185;
DELETE FROM node_tags WHERE node_id = 1185;
DELETE FROM current_way_nodes WHERE node_id=1185;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 1185;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=1185;
/* delete node 1204*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1204, 40303, -56067, 2, false, (now() at time zone 'utc'), 1789569705, 2);
DELETE FROM current_node_tags WHERE node_id = 1204;
DELETE FROM node_tags WHERE node_id = 1204;
DELETE FROM current_way_nodes WHERE node_id=1204;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 1204;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=1204;
/* delete node 1215*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1215, 50765, -67898, 2, false, (now() at time zone 'utc'), 1789569705, 2);
DELETE FROM current_node_tags WHERE node_id = 1215;
DELETE FROM node_tags WHERE node_id = 1215;
DELETE FROM current_way_nodes WHERE node_id=1215;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 1215;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=1215;
/* delete node 1218*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1218, 9663, -59783, 2, false, (now() at time zone 'utc'), 1789569704, 2);
DELETE FROM current_node_tags WHERE node_id = 1218;
DELETE FROM node_tags WHERE node_id = 1218;
DELETE FROM current_way_nodes WHERE node_id=1218;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 1218;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=1218;
/* delete node 1274*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1274, 46796, -58936, 2, false, (now() at time zone 'utc'), 1789569705, 2);
DELETE FROM current_node_tags WHERE node_id = 1274;
DELETE FROM node_tags WHERE node_id = 1274;
DELETE FROM current_way_nodes WHERE node_id=1274;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 1274;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=1274;
/* delete node 1301*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1301, 90400, -24032, 2, false, (now() at time zone 'utc'), 1789569711, 2);
DELETE FROM current_node_tags WHERE node_id = 1301;
DELETE FROM node_tags WHERE node_id = 1301;
DELETE FROM current_way_nodes WHERE node_id=1301;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 1301;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=1301;
/* delete node 1320*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1320, 53557, -47932, 2, false, (now() at time zone 'utc'), 1789569707, 2);
DELETE FROM current_node_tags WHERE node_id = 1320;
DELETE FROM node_tags WHERE node_id = 1320;
DELETE FROM current_way_nodes WHERE node_id=1320;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 1320;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=1320;
/* delete node 1327*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1327, 59407, -56790, 2, false, (now() at time zone 'utc'), 1789569708, 2);
DELETE FROM current_node_tags WHERE node_id = 1327;
DELETE FROM node_tags WHERE node_id = 1327;
DELETE FROM current_way_nodes WHERE node_id=1327;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 1327;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=1327;
/* delete node 1347*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1347, 49288, -48483, 2, false, (now() at time zone 'utc'), 1789569707, 2);
DELETE FROM current_node_tags WHERE node_id = 1347;
DELETE FROM node_tags WHERE node_id = 1347;
DELETE FROM current_way_nodes WHERE node_id=1347;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 1347;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=1347;
/* delete node 1350*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1350, 40214, -13240, 2, false, (now() at time zone 'utc'), 1789569707, 2);
DELETE FROM current_node_tags WHERE node_id = 1350;
DELETE FROM node_tags WHERE node_id = 1350;
DELETE FROM current_way_nodes WHERE node_id=1350;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 1350;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=1350;
/* delete node 1358*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1358, 67634, -37351, 2, false, (now() at time zone 'utc'), 1789569710, 2);
DELETE FROM current_node_tags WHERE node_id = 1358;
DELETE FROM node_tags WHERE node_id = 1358;
DELETE FROM current_way_nodes WHERE node_id=1358;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 1358;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=1358;
/* delete node 1374*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1374, 66838, -7296, 2, false, (now() at time zone 'utc'), 1789569710, 2);
DELETE FROM current_node_tags WHERE node_id = 1374;
DELETE FROM node_tags WHERE node_id = 1374;
DELETE FROM current_way_nodes WHERE node_id=1374;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 1374;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=1374;
/* delete node 1426*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1426, 42171, -41727, 2, false, (now() at time zone 'utc'), 1789569707, 2);
DELETE FROM current_node_tags WHERE node_id = 1426;
DELETE FROM node_tags WHERE node_id = 1426;
DELETE FROM current_way_nodes WHERE node_id=1426;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 1426;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=1426;
/* delete node 1433*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1433, 42836, -42713, 2, false, (now() at time zone 'utc'), 1789569707, 2);
DELETE FROM current_node_tags WHERE node_id = 1433;
DELETE FROM node_tags WHERE node_id = 1433;
DELETE FROM current_way_nodes WHERE node_id=1433;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 1433;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=1433;
/* delete node 1439*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1439, 40415, -14189, 2, false, (now() at time zone 'utc'), 1789569707, 2);
DELETE FROM current_node_tags WHERE node_id = 1439;
DELETE FROM node_tags WHERE node_id = 1439;
DELETE FROM current_way_nodes WHERE node_id=1439;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 1439;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=1439;
/* delete node 1474*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1474, 69980, 4048, 2, false, (now() at time zone 'utc'), 3221225476, 2);
DELETE FROM current_node_tags WHERE node_id = 1474;
DELETE FROM node_tags WHERE node_id = 1474;
DELETE FROM current_way_nodes WHERE node_id=1474;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 1474;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=1474;
/* delete node 1477*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1477, 43532, -45880, 2, false, (now() at time zone 'utc'), 1789569707, 2);
DELETE FROM current_node_tags WHERE node_id = 1477;
DELETE FROM node_tags WHERE node_id = 1477;
DELETE FROM current_way_nodes WHERE node_id=1477;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 1477;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=1477;
/* delete node 1479*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1479, 45017, -38906, 2, false, (now() at time zone 'utc'), 1789569707, 2);
DELETE FROM current_node_tags WHERE node_id = 1479;
DELETE FROM node_tags WHERE node_id = 1479;
DELETE FROM current_way_nodes WHERE node_id=1479;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 1479;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=1479;
/* delete node 1515*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1515, 26354, -70267, 2, false, (now() at time zone 'utc'), 1789569704, 2);
DELETE FROM current_node_tags WHERE node_id = 1515;
DELETE FROM node_tags WHERE node_id = 1515;
DELETE FROM current_way_nodes WHERE node_id=1515;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 1515;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=1515;
/* delete node 1527*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1527, 82636, -28070, 2, false, (now() at time zone 'utc'), 1789569711, 2);
DELETE FROM current_node_tags WHERE node_id = 1527;
DELETE FROM node_tags WHERE node_id = 1527;
DELETE FROM current_way_nodes WHERE node_id=1527;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 1527;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=1527;
/* delete node 1540*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1540, 42314, -13339, 2, false, (now() at time zone 'utc'), 1789569707, 2);
DELETE FROM current_node_tags WHERE node_id = 1540;
DELETE FROM node_tags WHERE node_id = 1540;
DELETE FROM current_way_nodes WHERE node_id=1540;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 1540;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=1540;
/* delete node 1544*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1544, 10439, -61260, 2, false, (now() at time zone 'utc'), 1789569704, 2);
DELETE FROM current_node_tags WHERE node_id = 1544;
DELETE FROM node_tags WHERE node_id = 1544;
DELETE FROM current_way_nodes WHERE node_id=1544;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 1544;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=1544;
/* delete node 1549*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1549, 27590, -60915, 2, false, (now() at time zone 'utc'), 1789569705, 2);
DELETE FROM current_node_tags WHERE node_id = 1549;
DELETE FROM node_tags WHERE node_id = 1549;
DELETE FROM current_way_nodes WHERE node_id=1549;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 1549;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=1549;
/* delete node 1583*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1583, 54834, -33494, 2, false, (now() at time zone 'utc'), 1789569707, 2);
DELETE FROM current_node_tags WHERE node_id = 1583;
DELETE FROM node_tags WHERE node_id = 1583;
DELETE FROM current_way_nodes WHERE node_id=1583;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 1583;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=1583;
/* delete node 1584*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1584, 66492, -10735, 2, false, (now() at time zone 'utc'), 1789569710, 2);
DELETE FROM current_node_tags WHERE node_id = 1584;
DELETE FROM node_tags WHERE node_id = 1584;
DELETE FROM current_way_nodes WHERE node_id=1584;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 1584;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=1584;
/* delete node 1597*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1597, -9748, -62478, 2, false, (now() at time zone 'utc'), 1073741821, 2);
DELETE FROM current_node_tags WHERE node_id = 1597;
DELETE FROM node_tags WHERE node_id = 1597;
DELETE FROM current_way_nodes WHERE node_id=1597;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 1597;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=1597;
/* delete node 1609*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1609, 12307, -63976, 2, false, (now() at time zone 'utc'), 1789569704, 2);
DELETE FROM current_node_tags WHERE node_id = 1609;
DELETE FROM node_tags WHERE node_id = 1609;
DELETE FROM current_way_nodes WHERE node_id=1609;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 1609;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=1609;
/* delete node 1622*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1622, 48890, -19906, 2, false, (now() at time zone 'utc'), 1789569707, 2);
DELETE FROM current_node_tags WHERE node_id = 1622;
DELETE FROM node_tags WHERE node_id = 1622;
DELETE FROM current_way_nodes WHERE node_id=1622;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 1622;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=1622;
/* delete node 1638*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1638, 45294, -61212, 2, false, (now() at time zone 'utc'), 1789569705, 2);
DELETE FROM current_node_tags WHERE node_id = 1638;
DELETE FROM node_tags WHERE node_id = 1638;
DELETE FROM current_way_nodes WHERE node_id=1638;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 1638;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=1638;
/* delete node 1639*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1639, 46558, -13971, 2, false, (now() at time zone 'utc'), 1789569707, 2);
DELETE FROM current_node_tags WHERE node_id = 1639;
DELETE FROM node_tags WHERE node_id = 1639;
DELETE FROM current_way_nodes WHERE node_id=1639;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 1639;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=1639;
/* delete node 1683*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1683, 69407, 1343, 2, false, (now() at time zone 'utc'), 3221225476, 2);
DELETE FROM current_node_tags WHERE node_id = 1683;
DELETE FROM node_tags WHERE node_id = 1683;
DELETE FROM current_way_nodes WHERE node_id=1683;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 1683;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=1683;
/* delete node 1687*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1687, 36985, -8373, 2, false, (now() at time zone 'utc'), 1789569707, 2);
DELETE FROM current_node_tags WHERE node_id = 1687;
DELETE FROM node_tags WHERE node_id = 1687;
DELETE FROM current_way_nodes WHERE node_id=1687;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 1687;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=1687;
/* delete node 1695*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1695, 400, -60364, 2, false, (now() at time zone 'utc'), 1789569704, 2);
DELETE FROM current_node_tags WHERE node_id = 1695;
DELETE FROM node_tags WHERE node_id = 1695;
DELETE FROM current_way_nodes WHERE node_id=1695;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 1695;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=1695;
/* delete node 1700*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1700, 42501, -41365, 2, false, (now() at time zone 'utc'), 1789569707, 2);
DELETE FROM current_node_tags WHERE node_id = 1700;
DELETE FROM node_tags WHERE node_id = 1700;
DELETE FROM current_way_nodes WHERE node_id=1700;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 1700;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=1700;
/* delete node 1711*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1711, 75676, -33111, 2, false, (now() at time zone 'utc'), 1789569710, 2);
DELETE FROM current_node_tags WHERE node_id = 1711;
DELETE FROM node_tags WHERE node_id = 1711;
DELETE FROM current_way_nodes WHERE node_id=1711;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 1711;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=1711;
/* delete node 1741*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1741, 78416, -30411, 2, false, (now() at time zone 'utc'), 1789569710, 2);
DELETE FROM current_node_tags WHERE node_id = 1741;
DELETE FROM node_tags WHERE node_id = 1741;
DELETE FROM current_way_nodes WHERE node_id=1741;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 1741;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=1741;
/* delete node 1849*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1849, 53481, -34849, 2, false, (now() at time zone 'utc'), 1789569707, 2);
DELETE FROM current_node_tags WHERE node_id = 1849;
DELETE FROM node_tags WHERE node_id = 1849;
DELETE FROM current_way_nodes WHERE node_id=1849;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 1849;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=1849;
/* delete node 1851*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1851, 66371, -9507, 2, false, (now() at time zone 'utc'), 1789569710, 2);
DELETE FROM current_node_tags WHERE node_id = 1851;
DELETE FROM node_tags WHERE node_id = 1851;
DELETE FROM current_way_nodes WHERE node_id=1851;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 1851;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=1851;
/* delete node 1856*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1856, 45814, -12898, 2, false, (now() at time zone 'utc'), 1789569707, 2);
DELETE FROM current_node_tags WHERE node_id = 1856;
DELETE FROM node_tags WHERE node_id = 1856;
DELETE FROM current_way_nodes WHERE node_id=1856;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 1856;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=1856;
/* delete node 1887*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1887, 51770, -25159, 2, false, (now() at time zone 'utc'), 1789569707, 2);
DELETE FROM current_node_tags WHERE node_id = 1887;
DELETE FROM node_tags WHERE node_id = 1887;
DELETE FROM current_way_nodes WHERE node_id=1887;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 1887;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=1887;
/* delete node 1920*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1920, 43903, -12279, 2, false, (now() at time zone 'utc'), 1789569707, 2);
DELETE FROM current_node_tags WHERE node_id = 1920;
DELETE FROM node_tags WHERE node_id = 1920;
DELETE FROM current_way_nodes WHERE node_id=1920;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 1920;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=1920;
/* delete node 1927*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1927, 11378, -62516, 2, false, (now() at time zone 'utc'), 1789569704, 2);
DELETE FROM current_node_tags WHERE node_id = 1927;
DELETE FROM node_tags WHERE node_id = 1927;
DELETE FROM current_way_nodes WHERE node_id=1927;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 1927;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=1927;
/* delete node 1935*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1935, 67483, 12528, 2, false, (now() at time zone 'utc'), 3221225476, 2);
DELETE FROM current_node_tags WHERE node_id = 1935;
DELETE FROM node_tags WHERE node_id = 1935;
DELETE FROM current_way_nodes WHERE node_id=1935;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 1935;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=1935;
/* delete node 1940*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1940, 4924, -59840, 2, false, (now() at time zone 'utc'), 1789569704, 2);
DELETE FROM current_node_tags WHERE node_id = 1940;
DELETE FROM node_tags WHERE node_id = 1940;
DELETE FROM current_way_nodes WHERE node_id=1940;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 1940;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=1940;
/* delete node 1947*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1947, 56920, -76477, 2, false, (now() at time zone 'utc'), 1789569708, 2);
DELETE FROM current_node_tags WHERE node_id = 1947;
DELETE FROM node_tags WHERE node_id = 1947;
DELETE FROM current_way_nodes WHERE node_id=1947;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 1947;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=1947;
/* delete node 1950*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1950, 57946, -46021, 2, false, (now() at time zone 'utc'), 1789569710, 2);
DELETE FROM current_node_tags WHERE node_id = 1950;
DELETE FROM node_tags WHERE node_id = 1950;
DELETE FROM current_way_nodes WHERE node_id=1950;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 1950;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=1950;
/* delete node 1962*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1962, 21081, -53757, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 1962;
DELETE FROM node_tags WHERE node_id = 1962;
DELETE FROM current_way_nodes WHERE node_id=1962;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 1962;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=1962;
/* create node 73191556*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73191556, 424893800, -714717199, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73191556, 424893800, -714717199, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73191556, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73191556, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73191556, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73191556, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73193517*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73193517, 424868870, -714642359, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73193517, 424868870, -714642359, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73193517, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73193517, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73193517, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73193517, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73193693*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73193693, 424842080, -714693009, 2, true, (now() at time zone 'utc'), 1710365927, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73193693, 424842080, -714693009, 2, true, (now() at time zone 'utc'), 1710365927, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73193693, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73193693, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73193693, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73193693, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73193706*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73193706, 424864620, -714674559, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73193706, 424864620, -714674559, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73193706, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73193706, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73193706, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73193706, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73194897*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73194897, 424897250, -714613819, 2, true, (now() at time zone 'utc'), 1710365947, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73194897, 424897250, -714613819, 2, true, (now() at time zone 'utc'), 1710365947, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73194897, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73194897, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73194897, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73194897, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73194926*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73194926, 424896430, -714631059, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73194926, 424896430, -714631059, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73194926, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73194926, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73194926, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73194926, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73196627*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73196627, 424834540, -714695829, 2, true, (now() at time zone 'utc'), 1710365927, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73196627, 424834540, -714695829, 2, true, (now() at time zone 'utc'), 1710365927, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73196627, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73196627, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73196627, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73196627, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73196784*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73196784, 424892700, -714726659, 2, true, (now() at time zone 'utc'), 1710365937, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73196784, 424892700, -714726659, 2, true, (now() at time zone 'utc'), 1710365937, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73196784, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73196784, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73196784, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73196784, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73196869*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73196869, 424854740, -714684199, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73196869, 424854740, -714684199, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73196869, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73196869, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73196869, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73196869, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73197523*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73197523, 424868280, -714705649, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73197523, 424868280, -714705649, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73197523, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73197523, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73197523, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73197523, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73197564*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73197564, 424905180, -714684479, 2, true, (now() at time zone 'utc'), 1710365942, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73197564, 424905180, -714684479, 2, true, (now() at time zone 'utc'), 1710365942, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73197564, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73197564, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73197564, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73197564, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73198417*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73198417, 424892760, -714689659, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73198417, 424892760, -714689659, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73198417, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73198417, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73198417, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73198417, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73198531*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73198531, 424897080, -714642989, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73198531, 424897080, -714642989, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73198531, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73198531, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73198531, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73198531, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73198827*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73198827, 424842060, -714705169, 2, true, (now() at time zone 'utc'), 1710365927, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73198827, 424842060, -714705169, 2, true, (now() at time zone 'utc'), 1710365927, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73198827, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73198827, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73198827, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73198827, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73199101*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73199101, 424864020, -714708299, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73199101, 424864020, -714708299, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73199101, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73199101, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73199101, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73199101, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73200077*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73200077, 424905740, -714679369, 2, true, (now() at time zone 'utc'), 1710365942, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73200077, 424905740, -714679369, 2, true, (now() at time zone 'utc'), 1710365942, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73200077, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73200077, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73200077, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73200077, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73200871*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73200871, 424816600, -714676459, 2, true, (now() at time zone 'utc'), 1710365926, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73200871, 424816600, -714676459, 2, true, (now() at time zone 'utc'), 1710365926, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73200871, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73200871, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73200871, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73200871, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73201078*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73201078, 424857360, -714712859, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73201078, 424857360, -714712859, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73201078, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73201078, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73201078, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73201078, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73201203*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73201203, 424905870, -714682709, 2, true, (now() at time zone 'utc'), 1710365942, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73201203, 424905870, -714682709, 2, true, (now() at time zone 'utc'), 1710365942, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73201203, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73201203, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73201203, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73201203, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73202592*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73202592, 424877810, -714666039, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73202592, 424877810, -714666039, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73202592, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73202592, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73202592, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73202592, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73203149*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73203149, 424839760, -714705139, 2, true, (now() at time zone 'utc'), 1710365927, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73203149, 424839760, -714705139, 2, true, (now() at time zone 'utc'), 1710365927, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73203149, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73203149, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73203149, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73203149, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73204211*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73204211, 424885350, -714735879, 2, true, (now() at time zone 'utc'), 1710365937, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73204211, 424885350, -714735879, 2, true, (now() at time zone 'utc'), 1710365937, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73204211, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73204211, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73204211, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73204211, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73204436*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73204436, 424904350, -714675399, 2, true, (now() at time zone 'utc'), 1710365942, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73204436, 424904350, -714675399, 2, true, (now() at time zone 'utc'), 1710365942, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73204436, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73204436, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73204436, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73204436, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73205517*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73205517, 424882440, -714728019, 2, true, (now() at time zone 'utc'), 1710365937, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73205517, 424882440, -714728019, 2, true, (now() at time zone 'utc'), 1710365937, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73205517, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73205517, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73205517, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73205517, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73206274*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73206274, 424903030, -714664449, 2, true, (now() at time zone 'utc'), 1710365948, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73206274, 424903030, -714664449, 2, true, (now() at time zone 'utc'), 1710365948, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73206274, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73206274, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73206274, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73206274, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73206411*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73206411, 424899820, -714659399, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73206411, 424899820, -714659399, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73206411, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73206411, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73206411, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73206411, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73206799*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73206799, 424875350, -714667499, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73206799, 424875350, -714667499, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73206799, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73206799, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73206799, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73206799, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73207842*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73207842, 424899250, -714666919, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73207842, 424899250, -714666919, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73207842, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73207842, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73207842, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73207842, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73208896*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73208896, 424837090, -714699339, 2, true, (now() at time zone 'utc'), 1710365927, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73208896, 424837090, -714699339, 2, true, (now() at time zone 'utc'), 1710365927, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73208896, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73208896, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73208896, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73208896, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73208963*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73208963, 424841000, -714705559, 2, true, (now() at time zone 'utc'), 1710365927, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73208963, 424841000, -714705559, 2, true, (now() at time zone 'utc'), 1710365927, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73208963, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73208963, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73208963, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73208963, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73209956*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73209956, 424915090, -714694719, 2, true, (now() at time zone 'utc'), 1710365942, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73209956, 424915090, -714694719, 2, true, (now() at time zone 'utc'), 1710365942, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73209956, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73209956, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73209956, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73209956, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73210632*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73210632, 424842720, -714652849, 2, true, (now() at time zone 'utc'), 1710365933, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73210632, 424842720, -714652849, 2, true, (now() at time zone 'utc'), 1710365933, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73210632, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73210632, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73210632, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73210632, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73210843*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73210843, 424873407, -714623290, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73210843, 424873407, -714623290, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73210843, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73210843, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73210843, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73210843, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73210979*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73210979, 424898680, -714655269, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73210979, 424898680, -714655269, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73210979, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73210979, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73210979, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73210979, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73211079*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73211079, 424856680, -714638129, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73211079, 424856680, -714638129, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73211079, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73211079, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73211079, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73211079, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73211196*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73211196, 424929670, -714686969, 2, true, (now() at time zone 'utc'), 1710365942, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73211196, 424929670, -714686969, 2, true, (now() at time zone 'utc'), 1710365942, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73211196, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73211196, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73211196, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73211196, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73211260*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73211260, 424832790, -714696239, 2, true, (now() at time zone 'utc'), 1710365927, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73211260, 424832790, -714696239, 2, true, (now() at time zone 'utc'), 1710365927, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73211260, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73211260, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73211260, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73211260, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73211503*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73211503, 424860710, -714639229, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73211503, 424860710, -714639229, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73211503, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73211503, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73211503, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73211503, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73211935*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73211935, 424864780, -714630889, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73211935, 424864780, -714630889, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73211935, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73211935, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73211935, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73211935, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73212202*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73212202, 424878680, -714659219, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73212202, 424878680, -714659219, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73212202, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73212202, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73212202, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73212202, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73212758*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73212758, 424810820, -714682849, 2, true, (now() at time zone 'utc'), 1710365926, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73212758, 424810820, -714682849, 2, true, (now() at time zone 'utc'), 1710365926, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73212758, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73212758, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73212758, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73212758, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73212829*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73212829, 424894280, -714607569, 2, true, (now() at time zone 'utc'), 1710365947, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73212829, 424894280, -714607569, 2, true, (now() at time zone 'utc'), 1710365947, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73212829, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73212829, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73212829, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73212829, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73213112*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73213112, 424851290, -714687059, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73213112, 424851290, -714687059, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73213112, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73213112, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73213112, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73213112, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73213119*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73213119, 424903830, -714601189, 2, true, (now() at time zone 'utc'), 1710365950, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73213119, 424903830, -714601189, 2, true, (now() at time zone 'utc'), 1710365950, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73213119, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73213119, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73213119, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73213119, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73213247*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73213247, 424927980, -714689279, 2, true, (now() at time zone 'utc'), 1710365942, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73213247, 424927980, -714689279, 2, true, (now() at time zone 'utc'), 1710365942, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73213247, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73213247, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73213247, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73213247, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73213616*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73213616, 424896090, -714635029, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73213616, 424896090, -714635029, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73213616, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73213616, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73213616, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73213616, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73214146*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73214146, 424876670, -714666909, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73214146, 424876670, -714666909, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73214146, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73214146, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73214146, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73214146, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73214266*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73214266, 424925030, -714691589, 2, true, (now() at time zone 'utc'), 1710365942, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73214266, 424925030, -714691589, 2, true, (now() at time zone 'utc'), 1710365942, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73214266, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73214266, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73214266, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73214266, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73214397*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73214397, 424869040, -714690979, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73214397, 424869040, -714690979, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73214397, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73214397, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73214397, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73214397, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73214598*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73214598, 424850010, -714645399, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73214598, 424850010, -714645399, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73214598, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73214598, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73214598, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73214598, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73214740*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73214740, 424874680, -714645209, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73214740, 424874680, -714645209, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73214740, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73214740, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73214740, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73214740, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73215196*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73215196, 424897690, -714622679, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73215196, 424897690, -714622679, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73215196, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73215196, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73215196, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73215196, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73216349*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73216349, 424922880, -714692609, 2, true, (now() at time zone 'utc'), 1710365942, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73216349, 424922880, -714692609, 2, true, (now() at time zone 'utc'), 1710365942, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73216349, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73216349, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73216349, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73216349, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73216364*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73216364, 424881210, -714616169, 2, true, (now() at time zone 'utc'), 1710365947, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73216364, 424881210, -714616169, 2, true, (now() at time zone 'utc'), 1710365947, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73216364, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73216364, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73216364, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73216364, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73216653*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73216653, 424880460, -714722399, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73216653, 424880460, -714722399, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73216653, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73216653, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73216653, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73216653, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73216703*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73216703, 424889690, -714609379, 2, true, (now() at time zone 'utc'), 1710365947, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73216703, 424889690, -714609379, 2, true, (now() at time zone 'utc'), 1710365947, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73216703, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73216703, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73216703, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73216703, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73217679*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73217679, 424790489, -714705583, 2, true, (now() at time zone 'utc'), 1710365923, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73217679, 424790489, -714705583, 2, true, (now() at time zone 'utc'), 1710365923, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73217679, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73217679, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73217679, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73217679, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73218277*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73218277, 424898050, -714619719, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73218277, 424898050, -714619719, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73218277, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73218277, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73218277, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73218277, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73218364*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73218364, 424864120, -714645139, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73218364, 424864120, -714645139, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73218364, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73218364, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73218364, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73218364, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73218667*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73218667, 424905300, -714678189, 2, true, (now() at time zone 'utc'), 1710365942, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73218667, 424905300, -714678189, 2, true, (now() at time zone 'utc'), 1710365942, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73218667, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73218667, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73218667, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73218667, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73218801*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73218801, 424884350, -714732999, 2, true, (now() at time zone 'utc'), 1710365937, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73218801, 424884350, -714732999, 2, true, (now() at time zone 'utc'), 1710365937, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73218801, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73218801, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73218801, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73218801, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73219227*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73219227, 424895490, -714610029, 2, true, (now() at time zone 'utc'), 1710365947, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73219227, 424895490, -714610029, 2, true, (now() at time zone 'utc'), 1710365947, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73219227, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73219227, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73219227, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73219227, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73219714*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73219714, 424878440, -714716649, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73219714, 424878440, -714716649, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73219714, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73219714, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73219714, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73219714, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73220389*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73220389, 424899160, -714689319, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73220389, 424899160, -714689319, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73220389, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73220389, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73220389, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73220389, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73221280*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73221280, 424900790, -714688939, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73221280, 424900790, -714688939, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73221280, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73221280, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73221280, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73221280, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73221959*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73221959, 424946510, -714672499, 2, true, (now() at time zone 'utc'), 1710365943, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73221959, 424946510, -714672499, 2, true, (now() at time zone 'utc'), 1710365943, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73221959, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73221959, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73221959, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73221959, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73222135*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73222135, 424850270, -714702659, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73222135, 424850270, -714702659, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73222135, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73222135, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73222135, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73222135, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73222194*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73222194, 424869120, -714691199, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73222194, 424869120, -714691199, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73222194, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73222194, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73222194, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73222194, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73222820*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73222820, 424908370, -714599259, 2, true, (now() at time zone 'utc'), 1710365950, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73222820, 424908370, -714599259, 2, true, (now() at time zone 'utc'), 1710365950, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73222820, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73222820, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73222820, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73222820, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73223344*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73223344, 424897170, -714625989, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73223344, 424897170, -714625989, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73223344, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73223344, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73223344, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73223344, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73223635*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73223635, 424798400, -714696509, 2, true, (now() at time zone 'utc'), 1710365926, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73223635, 424798400, -714696509, 2, true, (now() at time zone 'utc'), 1710365926, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73223635, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73223635, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73223635, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73223635, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73224493*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73224493, 424901860, -714665209, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73224493, 424901860, -714665209, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73224493, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73224493, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73224493, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73224493, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73224877*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73224877, 424930570, -714685579, 2, true, (now() at time zone 'utc'), 1710365943, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73224877, 424930570, -714685579, 2, true, (now() at time zone 'utc'), 1710365943, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73224877, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73224877, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73224877, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73224877, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73225591*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73225591, 424899940, -714710419, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73225591, 424899940, -714710419, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73225591, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73225591, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73225591, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73225591, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73226144*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73226144, 424885950, -714612239, 2, true, (now() at time zone 'utc'), 1710365947, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73226144, 424885950, -714612239, 2, true, (now() at time zone 'utc'), 1710365947, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73226144, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73226144, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73226144, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73226144, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73226653*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73226653, 424843790, -714704549, 2, true, (now() at time zone 'utc'), 1710365927, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73226653, 424843790, -714704549, 2, true, (now() at time zone 'utc'), 1710365927, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73226653, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73226653, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73226653, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73226653, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73227003*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73227003, 424878540, -714671069, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73227003, 424878540, -714671069, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73227003, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73227003, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73227003, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73227003, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73227041*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73227041, 424861290, -714678009, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73227041, 424861290, -714678009, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73227041, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73227041, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73227041, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73227041, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73227169*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73227169, 424876680, -714700039, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73227169, 424876680, -714700039, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73227169, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73227169, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73227169, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73227169, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73227427*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73227427, 424835090, -714696349, 2, true, (now() at time zone 'utc'), 1710365927, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73227427, 424835090, -714696349, 2, true, (now() at time zone 'utc'), 1710365927, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73227427, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73227427, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73227427, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73227427, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73228507*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73228507, 424873470, -714668689, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73228507, 424873470, -714668689, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73228507, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73228507, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73228507, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73228507, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73229104*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73229104, 424897740, -714615349, 2, true, (now() at time zone 'utc'), 1710365947, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73229104, 424897740, -714615349, 2, true, (now() at time zone 'utc'), 1710365947, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73229104, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73229104, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73229104, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73229104, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73229161*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73229161, 424933760, -714682279, 2, true, (now() at time zone 'utc'), 1710365943, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73229161, 424933760, -714682279, 2, true, (now() at time zone 'utc'), 1710365943, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73229161, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73229161, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73229161, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73229161, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73230808*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73230808, 424875280, -714647329, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73230808, 424875280, -714647329, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73230808, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73230808, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73230808, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73230808, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73231682*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73231682, 424866060, -714644479, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73231682, 424866060, -714644479, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73231682, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73231682, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73231682, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73231682, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73232252*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73232252, 424912290, -714696619, 2, true, (now() at time zone 'utc'), 1710365942, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73232252, 424912290, -714696619, 2, true, (now() at time zone 'utc'), 1710365942, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73232252, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73232252, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73232252, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73232252, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73233275*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73233275, 424891140, -714690429, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73233275, 424891140, -714690429, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73233275, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73233275, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73233275, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73233275, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73233749*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73233749, 424840350, -714705489, 2, true, (now() at time zone 'utc'), 1710365927, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73233749, 424840350, -714705489, 2, true, (now() at time zone 'utc'), 1710365927, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73233749, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73233749, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73233749, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73233749, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73234433*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73234433, 424909070, -714700249, 2, true, (now() at time zone 'utc'), 1710365942, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73234433, 424909070, -714700249, 2, true, (now() at time zone 'utc'), 1710365942, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73234433, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73234433, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73234433, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73234433, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73235438*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73235438, 424829360, -714697879, 2, true, (now() at time zone 'utc'), 1710365927, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73235438, 424829360, -714697879, 2, true, (now() at time zone 'utc'), 1710365927, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73235438, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73235438, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73235438, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73235438, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73235656*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73235656, 424822040, -714671329, 2, true, (now() at time zone 'utc'), 1710365933, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73235656, 424822040, -714671329, 2, true, (now() at time zone 'utc'), 1710365933, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73235656, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73235656, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73235656, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73235656, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73237048*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73237048, 424896290, -714714289, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73237048, 424896290, -714714289, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73237048, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73237048, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73237048, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73237048, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73239072*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73239072, 424903010, -714671639, 2, true, (now() at time zone 'utc'), 1710365942, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73239072, 424903010, -714671639, 2, true, (now() at time zone 'utc'), 1710365942, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73239072, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73239072, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73239072, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73239072, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73239140*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73239140, 424852230, -714701899, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73239140, 424852230, -714701899, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73239140, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73239140, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73239140, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73239140, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73239324*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73239324, 424896490, -714611969, 2, true, (now() at time zone 'utc'), 1710365947, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73239324, 424896490, -714611969, 2, true, (now() at time zone 'utc'), 1710365947, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73239324, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73239324, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73239324, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73239324, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73239736*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73239736, 424906080, -714681949, 2, true, (now() at time zone 'utc'), 1710365942, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73239736, 424906080, -714681949, 2, true, (now() at time zone 'utc'), 1710365942, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73239736, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73239736, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73239736, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73239736, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73240216*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73240216, 424899120, -714657109, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73240216, 424899120, -714657109, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73240216, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73240216, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73240216, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73240216, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73241114*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73241114, 424879400, -714719399, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73241114, 424879400, -714719399, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73241114, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73241114, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73241114, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73241114, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73242177*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73242177, 424849200, -714688589, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73242177, 424849200, -714688589, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73242177, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73242177, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73242177, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73242177, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73242272*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73242272, 424891710, -714719569, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73242272, 424891710, -714719569, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73242272, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73242272, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73242272, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73242272, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73242650*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73242650, 424919480, -714693319, 2, true, (now() at time zone 'utc'), 1710365942, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73242650, 424919480, -714693319, 2, true, (now() at time zone 'utc'), 1710365942, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73242650, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73242650, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73242650, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73242650, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73242809*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73242809, 424903030, -714687199, 2, true, (now() at time zone 'utc'), 1710365942, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73242809, 424903030, -714687199, 2, true, (now() at time zone 'utc'), 1710365942, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73242809, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73242809, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73242809, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73242809, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73242965*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73242965, 424873190, -714702289, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73242965, 424873190, -714702289, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73242965, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73242965, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73242965, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73242965, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73245161*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73245161, 424839240, -714704509, 2, true, (now() at time zone 'utc'), 1710365927, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73245161, 424839240, -714704509, 2, true, (now() at time zone 'utc'), 1710365927, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73245161, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73245161, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73245161, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73245161, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73245707*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73245707, 424911180, -714697909, 2, true, (now() at time zone 'utc'), 1710365942, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73245707, 424911180, -714697909, 2, true, (now() at time zone 'utc'), 1710365942, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73245707, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73245707, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73245707, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73245707, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73245948*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73245948, 424838400, -714694649, 2, true, (now() at time zone 'utc'), 1710365927, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73245948, 424838400, -714694649, 2, true, (now() at time zone 'utc'), 1710365927, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73245948, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73245948, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73245948, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73245948, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73246449*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73246449, 424855820, -714699629, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73246449, 424855820, -714699629, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73246449, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73246449, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73246449, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73246449, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73246580*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73246580, 424893920, -714689449, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73246580, 424893920, -714689449, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73246580, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73246580, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73246580, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73246580, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73246975*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73246975, 424874360, -714705389, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73246975, 424874360, -714705389, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73246975, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73246975, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73246975, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73246975, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73247268*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73247268, 424940020, -714677419, 2, true, (now() at time zone 'utc'), 1710365943, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73247268, 424940020, -714677419, 2, true, (now() at time zone 'utc'), 1710365943, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73247268, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73247268, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73247268, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73247268, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73247741*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73247741, 424906080, -714680729, 2, true, (now() at time zone 'utc'), 1710365942, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73247741, 424906080, -714680729, 2, true, (now() at time zone 'utc'), 1710365942, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73247741, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73247741, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73247741, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73247741, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73248893*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73248893, 424901720, -714688519, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73248893, 424901720, -714688519, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73248893, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73248893, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73248893, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73248893, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73249339*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73249339, 424871450, -714642279, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73249339, 424871450, -714642279, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73249339, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73249339, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73249339, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73249339, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73250382*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73250382, 424839300, -714655819, 2, true, (now() at time zone 'utc'), 1710365933, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73250382, 424839300, -714655819, 2, true, (now() at time zone 'utc'), 1710365933, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73250382, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73250382, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73250382, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73250382, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73250455*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73250455, 424926490, -714690809, 2, true, (now() at time zone 'utc'), 1710365942, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73250455, 424926490, -714690809, 2, true, (now() at time zone 'utc'), 1710365942, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73250455, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73250455, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73250455, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73250455, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73250523*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73250523, 424836000, -714697319, 2, true, (now() at time zone 'utc'), 1710365927, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73250523, 424836000, -714697319, 2, true, (now() at time zone 'utc'), 1710365927, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73250523, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73250523, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73250523, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73250523, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73250848*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73250848, 424837910, -714701849, 2, true, (now() at time zone 'utc'), 1710365927, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73250848, 424837910, -714701849, 2, true, (now() at time zone 'utc'), 1710365927, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73250848, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73250848, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73250848, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73250848, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73250952*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73250952, 424896920, -714668409, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73250952, 424896920, -714668409, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73250952, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73250952, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73250952, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73250952, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73251655*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73251655, 424896330, -714689409, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73251655, 424896330, -714689409, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73251655, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73251655, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73251655, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73251655, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73252208*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73252208, 424869660, -714671129, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73252208, 424869660, -714671129, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73252208, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73252208, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73252208, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73252208, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73252408*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73252408, 424904280, -714685799, 2, true, (now() at time zone 'utc'), 1710365942, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73252408, 424904280, -714685799, 2, true, (now() at time zone 'utc'), 1710365942, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73252408, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73252408, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73252408, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73252408, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73252524*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73252524, 424847660, -714689879, 2, true, (now() at time zone 'utc'), 1710365927, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73252524, 424847660, -714689879, 2, true, (now() at time zone 'utc'), 1710365927, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73252524, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73252524, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73252524, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73252524, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73252638*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73252638, 424853210, -714641569, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73252638, 424853210, -714641569, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73252638, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73252638, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73252638, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73252638, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73255331*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73255331, 424917940, -714693699, 2, true, (now() at time zone 'utc'), 1710365942, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73255331, 424917940, -714693699, 2, true, (now() at time zone 'utc'), 1710365942, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73255331, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73255331, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73255331, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73255331, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73256253*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73256253, 424859840, -714697049, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73256253, 424859840, -714697049, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73256253, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73256253, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73256253, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73256253, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73256457*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73256457, 424887990, -714674489, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73256457, 424887990, -714674489, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73256457, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73256457, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73256457, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73256457, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73257221*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73257221, 424867880, -714687679, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73257221, 424867880, -714687679, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73257221, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73257221, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73257221, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73257221, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73258145*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73258145, 424864820, -714645249, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73258145, 424864820, -714645249, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73258145, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73258145, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73258145, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73258145, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73258459*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73258459, 424879230, -714661789, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73258459, 424879230, -714661789, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73258459, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73258459, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73258459, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73258459, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73259551*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73259551, 424878400, -714665449, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73259551, 424878400, -714665449, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73259551, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73259551, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73259551, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73259551, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73260428*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73260428, 424879070, -714663149, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73260428, 424879070, -714663149, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73260428, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73260428, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73260428, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73260428, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73260456*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73260456, 424897650, -714647339, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73260456, 424897650, -714647339, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73260456, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73260456, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73260456, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73260456, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73261702*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73261702, 424896950, -714641999, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73261702, 424896950, -714641999, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73261702, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73261702, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73261702, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73261702, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73262290*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73262290, 424932290, -714683749, 2, true, (now() at time zone 'utc'), 1710365943, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73262290, 424932290, -714683749, 2, true, (now() at time zone 'utc'), 1710365943, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73262290, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73262290, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73262290, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73262290, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73262376*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73262376, 424867440, -714686439, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73262376, 424867440, -714686439, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73262376, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73262376, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73262376, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73262376, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73263578*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73263578, 424862740, -714695169, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73263578, 424862740, -714695169, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73263578, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73263578, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73263578, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73263578, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73263987*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73263987, 424877020, -714712899, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73263987, 424877020, -714712899, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73263987, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73263987, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73263987, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73263987, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73264086*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73264086, 424835740, -714695549, 2, true, (now() at time zone 'utc'), 1710365927, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73264086, 424835740, -714695549, 2, true, (now() at time zone 'utc'), 1710365927, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73264086, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73264086, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73264086, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73264086, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73264165*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73264165, 424883180, -714614569, 2, true, (now() at time zone 'utc'), 1710365947, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73264165, 424883180, -714614569, 2, true, (now() at time zone 'utc'), 1710365947, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73264165, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73264165, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73264165, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73264165, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73264220*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73264220, 424903910, -714705969, 2, true, (now() at time zone 'utc'), 1710365942, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73264220, 424903910, -714705969, 2, true, (now() at time zone 'utc'), 1710365942, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73264220, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73264220, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73264220, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73264220, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73264246*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73264246, 424935780, -714680639, 2, true, (now() at time zone 'utc'), 1710365943, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73264246, 424935780, -714680639, 2, true, (now() at time zone 'utc'), 1710365943, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73264246, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73264246, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73264246, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73264246, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73267560*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73267560, 424872220, -714699719, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73267560, 424872220, -714699719, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73267560, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73267560, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73267560, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73267560, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73269044*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73269044, 424890960, -714721849, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73269044, 424890960, -714721849, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73269044, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73269044, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73269044, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73269044, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73269234*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73269234, 424915510, -714656269, 2, true, (now() at time zone 'utc'), 1710365948, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73269234, 424915510, -714656269, 2, true, (now() at time zone 'utc'), 1710365948, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73269234, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73269234, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73269234, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73269234, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73270072*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73270072, 424876700, -714681899, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73270072, 424876700, -714681899, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73270072, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73270072, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73270072, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73270072, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73270248*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73270248, 424796560, -714698529, 2, true, (now() at time zone 'utc'), 1710365926, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73270248, 424796560, -714698529, 2, true, (now() at time zone 'utc'), 1710365926, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73270248, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73270248, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73270248, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73270248, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73270366*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73270366, 424883790, -714695159, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73270366, 424883790, -714695159, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73270366, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73270366, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73270366, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73270366, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73270673*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73270673, 424845820, -714649909, 2, true, (now() at time zone 'utc'), 1710365933, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73270673, 424845820, -714649909, 2, true, (now() at time zone 'utc'), 1710365933, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73270673, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73270673, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73270673, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73270673, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73271318*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73271318, 424872830, -714684319, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73271318, 424872830, -714684319, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73271318, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73271318, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73271318, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73271318, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73271427*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73271427, 424862570, -714642779, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73271427, 424862570, -714642779, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73271427, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73271427, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73271427, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73271427, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73271679*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73271679, 424892530, -714671319, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73271679, 424892530, -714671319, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73271679, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73271679, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73271679, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73271679, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73272659*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73272659, 424896140, -714633079, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73272659, 424896140, -714633079, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73272659, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73272659, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73272659, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73272659, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73273000*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73273000, 424916050, -714694209, 2, true, (now() at time zone 'utc'), 1710365942, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73273000, 424916050, -714694209, 2, true, (now() at time zone 'utc'), 1710365942, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73273000, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73273000, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73273000, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73273000, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73274442*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73274442, 424891110, -714722589, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73274442, 424891110, -714722589, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73274442, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73274442, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73274442, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73274442, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73276164*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73276164, 424863020, -714676099, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73276164, 424863020, -714676099, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73276164, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73276164, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73276164, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73276164, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73276926*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73276926, 424864160, -714677479, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73276926, 424864160, -714677479, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73276926, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73276926, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73276926, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73276926, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73277351*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73277351, 424863350, -714644099, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73277351, 424863350, -714644099, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73277351, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73277351, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73277351, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73277351, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73278839*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73278839, 424891060, -714721069, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73278839, 424891060, -714721069, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73278839, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73278839, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73278839, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73278839, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73279205*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73279205, 424898000, -714616599, 2, true, (now() at time zone 'utc'), 1710365947, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73279205, 424898000, -714616599, 2, true, (now() at time zone 'utc'), 1710365947, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73279205, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73279205, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73279205, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73279205, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73279284*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73279284, 424865660, -714681339, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73279284, 424865660, -714681339, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73279284, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73279284, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73279284, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73279284, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73279399*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73279399, 424866770, -714672939, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73279399, 424866770, -714672939, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73279399, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73279399, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73279399, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73279399, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73281776*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73281776, 424853300, -714715439, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73281776, 424853300, -714715439, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73281776, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73281776, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73281776, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73281776, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73282312*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73282312, 424909140, -714660259, 2, true, (now() at time zone 'utc'), 1710365948, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73282312, 424909140, -714660259, 2, true, (now() at time zone 'utc'), 1710365948, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73282312, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73282312, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73282312, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73282312, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73282640*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73282640, 424803290, -714691149, 2, true, (now() at time zone 'utc'), 1710365926, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73282640, 424803290, -714691149, 2, true, (now() at time zone 'utc'), 1710365926, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73282640, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73282640, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73282640, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73282640, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73283089*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73283089, 424905620, -714600469, 2, true, (now() at time zone 'utc'), 1710365950, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73283089, 424905620, -714600469, 2, true, (now() at time zone 'utc'), 1710365950, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73283089, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73283089, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73283089, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73283089, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73283381*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73283381, 424867170, -714643369, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73283381, 424867170, -714643369, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73283381, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73283381, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73283381, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73283381, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73283794*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73283794, 424891590, -714724089, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73283794, 424891590, -714724089, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73283794, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73283794, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73283794, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73283794, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73283816*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73283816, 424836540, -714698159, 2, true, (now() at time zone 'utc'), 1710365927, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73283816, 424836540, -714698159, 2, true, (now() at time zone 'utc'), 1710365927, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73283816, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73283816, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73283816, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73283816, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73284174*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73284174, 424854030, -714700849, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73284174, 424854030, -714700849, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73284174, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73284174, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73284174, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73284174, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73285556*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73285556, 424858820, -714636039, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73285556, 424858820, -714636039, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73285556, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73285556, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73285556, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73285556, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73285767*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73285767, 424914670, -714695009, 2, true, (now() at time zone 'utc'), 1710365942, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73285767, 424914670, -714695009, 2, true, (now() at time zone 'utc'), 1710365942, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73285767, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73285767, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73285767, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73285767, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73286391*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73286391, 424878790, -714664819, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73286391, 424878790, -714664819, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73286391, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73286391, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73286391, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73286391, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73286439*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73286439, 424896480, -714638439, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73286439, 424896480, -714638439, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73286439, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73286439, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73286439, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73286439, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73286780*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73286780, 424806680, -714687429, 2, true, (now() at time zone 'utc'), 1710365926, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73286780, 424806680, -714687429, 2, true, (now() at time zone 'utc'), 1710365926, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73286780, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73286780, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73286780, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73286780, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73286854*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73286854, 424816350, -714676739, 2, true, (now() at time zone 'utc'), 1710365926, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73286854, 424816350, -714676739, 2, true, (now() at time zone 'utc'), 1710365926, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73286854, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73286854, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73286854, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73286854, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73286936*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73286936, 424818560, -714704539, 2, true, (now() at time zone 'utc'), 1710365926, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73286936, 424818560, -714704539, 2, true, (now() at time zone 'utc'), 1710365926, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73286936, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73286936, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73286936, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73286936, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73287250*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73287250, 424825610, -714668159, 2, true, (now() at time zone 'utc'), 1710365933, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73287250, 424825610, -714668159, 2, true, (now() at time zone 'utc'), 1710365933, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73287250, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73287250, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73287250, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73287250, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73287411*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73287411, 424892320, -714607359, 2, true, (now() at time zone 'utc'), 1710365947, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73287411, 424892320, -714607359, 2, true, (now() at time zone 'utc'), 1710365947, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73287411, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73287411, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73287411, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73287411, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73287452*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73287452, 424838610, -714703589, 2, true, (now() at time zone 'utc'), 1710365927, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73287452, 424838610, -714703589, 2, true, (now() at time zone 'utc'), 1710365927, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73287452, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73287452, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73287452, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73287452, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73288294*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73288294, 424876100, -714650289, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73288294, 424876100, -714650289, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73288294, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73288294, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73288294, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73288294, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73289382*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73289382, 424871640, -714703279, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73289382, 424871640, -714703279, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73289382, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73289382, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73289382, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73289382, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73289430*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73289430, 424873700, -714643639, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73289430, 424873700, -714643639, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73289430, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73289430, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73289430, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73289430, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73291062*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73291062, 424866380, -714629459, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73291062, 424866380, -714629459, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73291062, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73291062, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73291062, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73291062, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73291308*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73291308, 424897930, -714620739, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73291308, 424897930, -714620739, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73291308, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73291308, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73291308, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73291308, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73291671*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73291671, 424859120, -714635779, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73291671, 424859120, -714635779, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73291671, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73291671, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73291671, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73291671, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73292388*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73292388, 424827080, -714699209, 2, true, (now() at time zone 'utc'), 1710365927, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73292388, 424827080, -714699209, 2, true, (now() at time zone 'utc'), 1710365927, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73292388, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73292388, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73292388, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73292388, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73292960*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73292960, 424887580, -714611019, 2, true, (now() at time zone 'utc'), 1710365947, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73292960, 424887580, -714611019, 2, true, (now() at time zone 'utc'), 1710365947, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73292960, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73292960, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73292960, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73292960, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73292997*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73292997, 424863450, -714675689, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73292997, 424863450, -714675689, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73292997, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73292997, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73292997, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73292997, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73293749*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73293749, 424898370, -714603659, 2, true, (now() at time zone 'utc'), 1710365947, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73293749, 424898370, -714603659, 2, true, (now() at time zone 'utc'), 1710365947, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73293749, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73293749, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73293749, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73293749, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73293894*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73293894, 424901680, -714666809, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73293894, 424901680, -714666809, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73293894, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73293894, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73293894, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73293894, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73294708*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73294708, 424921050, -714693019, 2, true, (now() at time zone 'utc'), 1710365942, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73294708, 424921050, -714693019, 2, true, (now() at time zone 'utc'), 1710365942, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73294708, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73294708, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73294708, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73294708, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73294892*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73294892, 424893910, -714729509, 2, true, (now() at time zone 'utc'), 1710365937, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73294892, 424893910, -714729509, 2, true, (now() at time zone 'utc'), 1710365937, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73294892, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73294892, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73294892, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73294892, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73295702*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73295702, 424842100, -714653439, 2, true, (now() at time zone 'utc'), 1710365933, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73295702, 424842100, -714653439, 2, true, (now() at time zone 'utc'), 1710365933, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73295702, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73295702, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73295702, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73295702, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73296123*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73296123, 424905250, -714662909, 2, true, (now() at time zone 'utc'), 1710365948, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73296123, 424905250, -714662909, 2, true, (now() at time zone 'utc'), 1710365948, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73296123, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73296123, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73296123, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73296123, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73301829*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73301829, 424896140, -714604779, 2, true, (now() at time zone 'utc'), 1710365947, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73301829, 424896140, -714604779, 2, true, (now() at time zone 'utc'), 1710365947, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73301829, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73301829, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73301829, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73301829, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73304035*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73304035, 424853110, -714641689, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73304035, 424853110, -714641689, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73304035, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73304035, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73304035, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73304035, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73304338*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73304338, 424876930, -714666709, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73304338, 424876930, -714666709, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73304338, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73304338, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73304338, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73304338, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73304656*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73304656, 424896190, -714636459, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73304656, 424896190, -714636459, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73304656, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73304656, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73304656, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73304656, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73304917*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73304917, 424872510, -714642489, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73304917, 424872510, -714642489, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73304917, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73304917, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73304917, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73304917, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73305363*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73305363, 424854970, -714639589, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73305363, 424854970, -714639589, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73305363, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73305363, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73305363, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73305363, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73305792*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73305792, 424937000, -714679649, 2, true, (now() at time zone 'utc'), 1710365943, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73305792, 424937000, -714679649, 2, true, (now() at time zone 'utc'), 1710365943, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73305792, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73305792, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73305792, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73305792, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73306906*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73306906, 424877810, -714656329, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73306906, 424877810, -714656329, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73306906, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73306906, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73306906, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73306906, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73309070*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73309070, 424817930, -714675209, 2, true, (now() at time zone 'utc'), 1710365926, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73309070, 424817930, -714675209, 2, true, (now() at time zone 'utc'), 1710365926, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73309070, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73309070, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73309070, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73309070, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73309605*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73309605, 424891280, -714720419, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73309605, 424891280, -714720419, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73309605, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73309605, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73309605, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73309605, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73309877*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73309877, 424870130, -714642289, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73309877, 424870130, -714642289, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73309877, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73309877, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73309877, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73309877, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73310263*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73310263, 424837450, -714700519, 2, true, (now() at time zone 'utc'), 1710365927, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73310263, 424837450, -714700519, 2, true, (now() at time zone 'utc'), 1710365927, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73310263, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73310263, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73310263, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73310263, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73310821*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73310821, 424893640, -714606319, 2, true, (now() at time zone 'utc'), 1710365947, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73310821, 424893640, -714606319, 2, true, (now() at time zone 'utc'), 1710365947, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73310821, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73310821, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73310821, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73310821, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73311056*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73311056, 424899780, -714603019, 2, true, (now() at time zone 'utc'), 1710365947, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73311056, 424899780, -714603019, 2, true, (now() at time zone 'utc'), 1710365947, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73311056, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73311056, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73311056, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73311056, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73311090*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73311090, 424831380, -714696629, 2, true, (now() at time zone 'utc'), 1710365927, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73311090, 424831380, -714696629, 2, true, (now() at time zone 'utc'), 1710365927, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73311090, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73311090, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73311090, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73311090, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73311384*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73311384, 424883320, -714730319, 2, true, (now() at time zone 'utc'), 1710365937, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73311384, 424883320, -714730319, 2, true, (now() at time zone 'utc'), 1710365937, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73311384, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73311384, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73311384, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73311384, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73311591*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73311591, 424881050, -714678829, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73311591, 424881050, -714678829, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73311591, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73311591, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73311591, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73311591, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 73312096*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73312096, 424844870, -714691759, 2, true, (now() at time zone 'utc'), 1710365927, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73312096, 424844870, -714691759, 2, true, (now() at time zone 'utc'), 1710365927, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73312096, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73312096, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (73312096, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO node_tags (node_id, k, v, version) VALUES (73312096, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
/* create node 3306483744*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3306483744, 424875190, -714621663, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3306483744, 424875190, -714621663, 2, true, (now() at time zone 'utc'), 1710365944, 1);
/* delete way 19*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (19, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 19;
DELETE FROM way_tags WHERE way_id = 19;
DELETE FROM current_way_nodes WHERE way_id=19;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 19;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=19;
/* delete way 21*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (21, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 21;
DELETE FROM way_tags WHERE way_id = 21;
DELETE FROM current_way_nodes WHERE way_id=21;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 21;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=21;
/* delete way 25*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (25, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 25;
DELETE FROM way_tags WHERE way_id = 25;
DELETE FROM current_way_nodes WHERE way_id=25;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 25;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=25;
/* delete way 26*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (26, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 26;
DELETE FROM way_tags WHERE way_id = 26;
DELETE FROM current_way_nodes WHERE way_id=26;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 26;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=26;
/* delete way 28*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (28, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 28;
DELETE FROM way_tags WHERE way_id = 28;
DELETE FROM current_way_nodes WHERE way_id=28;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 28;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=28;
/* delete way 37*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (37, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 37;
DELETE FROM way_tags WHERE way_id = 37;
DELETE FROM current_way_nodes WHERE way_id=37;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 37;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=37;
/* delete way 113*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (113, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 113;
DELETE FROM way_tags WHERE way_id = 113;
DELETE FROM current_way_nodes WHERE way_id=113;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 113;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=113;
/* create way 9515048*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (9515048, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (9515048, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515048, 'name', 'Agawam Road');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515048, 'name', 'Agawam Road', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515048, 'condition', 'fair');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515048, 'condition', 'fair', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515048, 'lanes', '2');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515048, 'lanes', '2', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515048, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515048, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515048, 'width', '11.0');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515048, 'width', '11.0', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515048, 'massgis:way_id', '166413');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515048, 'massgis:way_id', '166413', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515048, 'highway', 'residential');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515048, 'highway', 'residential', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515048, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515048, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515048, 73286936, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515048, 73286936, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515048, 73292388, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515048, 73292388, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515048, 73235438, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515048, 73235438, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515048, 73311090, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515048, 73311090, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515048, 73211260, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515048, 73211260, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515048, 73196627, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515048, 73196627, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515048, 73264086, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515048, 73264086, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515048, 73245948, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515048, 73245948, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515048, 73193693, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515048, 73193693, 9);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515048, 73312096, 1, 10);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515048, 73312096, 10);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515048, 73252524, 1, 11);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515048, 73252524, 11);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515048, 73242177, 1, 12);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515048, 73242177, 12);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515048, 73213112, 1, 13);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515048, 73213112, 13);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515048, 73196869, 1, 14);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515048, 73196869, 14);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515048, 73227041, 1, 15);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515048, 73227041, 15);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515048, 73276164, 1, 16);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515048, 73276164, 16);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515048, 73292997, 1, 17);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515048, 73292997, 17);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515048, 73193706, 1, 18);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515048, 73193706, 18);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515048, 73279399, 1, 19);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515048, 73279399, 19);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515048, 73252208, 1, 20);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515048, 73252208, 20);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515048, 73228507, 1, 21);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515048, 73228507, 21);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515048, 73206799, 1, 22);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515048, 73206799, 22);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515048, 73214146, 1, 23);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515048, 73214146, 23);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515048, 73304338, 1, 24);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515048, 73304338, 24);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515048, 73202592, 1, 25);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515048, 73202592, 25);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515048, 73259551, 1, 26);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515048, 73259551, 26);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515048, 73286391, 1, 27);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515048, 73286391, 27);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515048, 73260428, 1, 28);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515048, 73260428, 28);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515048, 73258459, 1, 29);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515048, 73258459, 29);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515048, 73212202, 1, 30);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515048, 73212202, 30);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515048, 73306906, 1, 31);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515048, 73306906, 31);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515048, 73288294, 1, 32);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515048, 73288294, 32);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515048, 73230808, 1, 33);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515048, 73230808, 33);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515048, 73214740, 1, 34);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515048, 73214740, 34);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515048, 73289430, 1, 35);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515048, 73289430, 35);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515048, 73304917, 1, 36);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515048, 73304917, 36);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515048, 73249339, 1, 37);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515048, 73249339, 37);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515048, 73309877, 1, 38);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515048, 73309877, 38);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515048, 73193517, 1, 39);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515048, 73193517, 39);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515048, 73283381, 1, 40);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515048, 73283381, 40);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515048, 73231682, 1, 41);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515048, 73231682, 41);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515048, 73258145, 1, 42);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515048, 73258145, 42);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515048, 73218364, 1, 43);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515048, 73218364, 43);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515048, 73277351, 1, 44);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515048, 73277351, 44);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515048, 73271427, 1, 45);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515048, 73271427, 45);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515048, 73211503, 1, 46);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515048, 73211503, 46);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515048, 73291671, 1, 47);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515048, 73291671, 47);
/* create way 9515064*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (9515064, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (9515064, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515064, 'name', 'Mohegan Road');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515064, 'name', 'Mohegan Road', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515064, 'condition', 'fair');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515064, 'condition', 'fair', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515064, 'lanes', '2');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515064, 'lanes', '2', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515064, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515064, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515064, 'width', '9.1');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515064, 'width', '9.1', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515064, 'massgis:way_id', '157763');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515064, 'massgis:way_id', '157763', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515064, 'highway', 'residential');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515064, 'highway', 'residential', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515064, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515064, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515064, 73257221, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515064, 73257221, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515064, 73271318, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515064, 73271318, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515064, 73270072, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515064, 73270072, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515064, 73311591, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515064, 73311591, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515064, 73256457, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515064, 73256457, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515064, 73271679, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515064, 73271679, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515064, 73250952, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515064, 73250952, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515064, 73207842, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515064, 73207842, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515064, 73224493, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515064, 73224493, 9);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515064, 73206274, 1, 10);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515064, 73206274, 10);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515064, 73296123, 1, 11);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515064, 73296123, 11);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515064, 73282312, 1, 12);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515064, 73282312, 12);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515064, 73269234, 1, 13);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515064, 73269234, 13);
/* create way 9515077*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (9515077, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (9515077, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515077, 'name', 'Freedom Farme Road');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515077, 'name', 'Freedom Farme Road', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515077, 'source:maxspeed', 'massgis');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515077, 'source:maxspeed', 'massgis', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515077, 'condition', 'fair');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515077, 'condition', 'fair', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515077, 'lanes', '2');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515077, 'lanes', '2', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515077, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515077, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515077, 'width', '15.2');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515077, 'width', '15.2', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515077, 'massgis:way_id', '191604');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515077, 'massgis:way_id', '191604', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515077, 'highway', 'residential');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515077, 'highway', 'residential', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515077, 'maxspeed', '25 mph');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515077, 'maxspeed', '25 mph', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515077, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515077, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515077, 73224493, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515077, 73224493, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515077, 73206411, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515077, 73206411, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515077, 73240216, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515077, 73240216, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515077, 73210979, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515077, 73210979, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515077, 73260456, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515077, 73260456, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515077, 73198531, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515077, 73198531, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515077, 73261702, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515077, 73261702, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515077, 73286439, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515077, 73286439, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515077, 73304656, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515077, 73304656, 9);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515077, 73213616, 1, 10);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515077, 73213616, 10);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515077, 73272659, 1, 11);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515077, 73272659, 11);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515077, 73194926, 1, 12);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515077, 73194926, 12);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515077, 73223344, 1, 13);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515077, 73223344, 13);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515077, 73215196, 1, 14);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515077, 73215196, 14);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515077, 73291308, 1, 15);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515077, 73291308, 15);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515077, 73218277, 1, 16);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515077, 73218277, 16);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515077, 73279205, 1, 17);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515077, 73279205, 17);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515077, 73229104, 1, 18);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515077, 73229104, 18);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515077, 73194897, 1, 19);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515077, 73194897, 19);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515077, 73239324, 1, 20);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515077, 73239324, 20);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515077, 73219227, 1, 21);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515077, 73219227, 21);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515077, 73212829, 1, 22);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515077, 73212829, 22);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515077, 73310821, 1, 23);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515077, 73310821, 23);
/* create way 9515080*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (9515080, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (9515080, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515080, 'name', 'Seminole Road');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515080, 'name', 'Seminole Road', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515080, 'condition', 'fair');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515080, 'condition', 'fair', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515080, 'lanes', '2');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515080, 'lanes', '2', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515080, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515080, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515080, 'width', '9.8');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515080, 'width', '9.8', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515080, 'massgis:way_id', '142239');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515080, 'massgis:way_id', '142239', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515080, 'highway', 'residential');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515080, 'highway', 'residential', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515080, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515080, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515080, 73281776, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515080, 73281776, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515080, 73201078, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515080, 73201078, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515080, 73199101, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515080, 73199101, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515080, 73197523, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515080, 73197523, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515080, 73289382, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515080, 73289382, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515080, 73242965, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515080, 73242965, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515080, 73227169, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515080, 73227169, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515080, 73270366, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515080, 73270366, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515080, 73233275, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515080, 73233275, 9);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515080, 73198417, 1, 10);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515080, 73198417, 10);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515080, 73246580, 1, 11);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515080, 73246580, 11);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515080, 73251655, 1, 12);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515080, 73251655, 12);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515080, 73220389, 1, 13);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515080, 73220389, 13);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515080, 73221280, 1, 14);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515080, 73221280, 14);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515080, 73248893, 1, 15);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515080, 73248893, 15);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515080, 73242809, 1, 16);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515080, 73242809, 16);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515080, 73252408, 1, 17);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515080, 73252408, 17);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515080, 73197564, 1, 18);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515080, 73197564, 18);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515080, 73201203, 1, 19);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515080, 73201203, 19);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515080, 73239736, 1, 20);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515080, 73239736, 20);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515080, 73247741, 1, 21);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515080, 73247741, 21);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515080, 73200077, 1, 22);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515080, 73200077, 22);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515080, 73218667, 1, 23);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515080, 73218667, 23);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515080, 73204436, 1, 24);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515080, 73204436, 24);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515080, 73239072, 1, 25);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515080, 73239072, 25);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515080, 73293894, 1, 26);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515080, 73293894, 26);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515080, 73224493, 1, 27);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515080, 73224493, 27);
/* create way 9515089*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (9515089, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (9515089, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515089, 'name', 'Seneca Road');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515089, 'name', 'Seneca Road', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515089, 'condition', 'fair');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515089, 'condition', 'fair', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515089, 'lanes', '2');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515089, 'lanes', '2', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515089, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515089, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515089, 'width', '9.1');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515089, 'width', '9.1', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515089, 'massgis:way_id', '130764');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515089, 'massgis:way_id', '130764', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515089, 'highway', 'residential');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515089, 'highway', 'residential', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515089, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515089, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515089, 73204211, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515089, 73204211, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515089, 73218801, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515089, 73218801, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515089, 73311384, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515089, 73311384, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515089, 73205517, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515089, 73205517, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515089, 73216653, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515089, 73216653, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515089, 73241114, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515089, 73241114, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515089, 73219714, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515089, 73219714, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515089, 73263987, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515089, 73263987, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515089, 73246975, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515089, 73246975, 9);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515089, 73242965, 1, 10);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515089, 73242965, 10);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515089, 73267560, 1, 11);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515089, 73267560, 11);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515089, 73222194, 1, 12);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515089, 73222194, 12);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515089, 73214397, 1, 13);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515089, 73214397, 13);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515089, 73257221, 1, 14);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515089, 73257221, 14);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515089, 73262376, 1, 15);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515089, 73262376, 15);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515089, 73279284, 1, 16);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515089, 73279284, 16);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515089, 73276926, 1, 17);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515089, 73276926, 17);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515089, 73292997, 1, 18);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515089, 73292997, 18);
/* create way 9515150*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (9515150, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (9515150, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515150, 'name', 'Quaboag Road');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515150, 'name', 'Quaboag Road', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515150, 'condition', 'fair');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515150, 'condition', 'fair', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515150, 'lanes', '2');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515150, 'lanes', '2', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515150, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515150, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515150, 'width', '9.8');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515150, 'width', '9.8', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515150, 'massgis:way_id', '152803');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515150, 'massgis:way_id', '152803', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515150, 'highway', 'residential');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515150, 'highway', 'residential', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515150, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515150, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515150, 73196627, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515150, 73196627, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515150, 73227427, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515150, 73227427, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515150, 73250523, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515150, 73250523, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515150, 73283816, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515150, 73283816, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515150, 73208896, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515150, 73208896, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515150, 73310263, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515150, 73310263, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515150, 73250848, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515150, 73250848, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515150, 73287452, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515150, 73287452, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515150, 73245161, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515150, 73245161, 9);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515150, 73203149, 1, 10);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515150, 73203149, 10);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515150, 73233749, 1, 11);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515150, 73233749, 11);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515150, 73208963, 1, 12);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515150, 73208963, 12);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515150, 73198827, 1, 13);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515150, 73198827, 13);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515150, 73226653, 1, 14);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515150, 73226653, 14);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515150, 73222135, 1, 15);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515150, 73222135, 15);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515150, 73239140, 1, 16);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515150, 73239140, 16);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515150, 73284174, 1, 17);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515150, 73284174, 17);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515150, 73246449, 1, 18);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515150, 73246449, 18);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515150, 73256253, 1, 19);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515150, 73256253, 19);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515150, 73263578, 1, 20);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515150, 73263578, 20);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515150, 73214397, 1, 21);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515150, 73214397, 21);
/* create way 9515925*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (9515925, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (9515925, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515925, 'name', 'Sioux Road');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515925, 'name', 'Sioux Road', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515925, 'condition', 'fair');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515925, 'condition', 'fair', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515925, 'lanes', '2');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515925, 'lanes', '2', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515925, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515925, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515925, 'width', '12.2');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515925, 'width', '12.2', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515925, 'massgis:way_id', '187072');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515925, 'massgis:way_id', '187072', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515925, 'highway', 'residential');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515925, 'highway', 'residential', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515925, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515925, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515925, 73311591, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515925, 73311591, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515925, 73227003, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515925, 73227003, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515925, 73304338, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515925, 73304338, 3);
/* create way 9515954*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (9515954, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (9515954, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515954, 'name', 'Oneida Road');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515954, 'name', 'Oneida Road', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515954, 'condition', 'fair');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515954, 'condition', 'fair', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515954, 'lanes', '2');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515954, 'lanes', '2', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515954, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515954, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515954, 'width', '9.8');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515954, 'width', '9.8', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515954, 'massgis:way_id', '127721');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515954, 'massgis:way_id', '127721', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515954, 'highway', 'residential');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515954, 'highway', 'residential', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9515954, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9515954, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515954, 73294892, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515954, 73294892, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515954, 73196784, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515954, 73196784, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515954, 73283794, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515954, 73283794, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515954, 73274442, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515954, 73274442, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515954, 73269044, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515954, 73269044, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515954, 73278839, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515954, 73278839, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515954, 73309605, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515954, 73309605, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515954, 73242272, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515954, 73242272, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515954, 73191556, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515954, 73191556, 9);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515954, 73237048, 1, 10);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515954, 73237048, 10);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515954, 73225591, 1, 11);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515954, 73225591, 11);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515954, 73264220, 1, 12);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515954, 73264220, 12);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515954, 73234433, 1, 13);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515954, 73234433, 13);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515954, 73245707, 1, 14);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515954, 73245707, 14);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515954, 73232252, 1, 15);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515954, 73232252, 15);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515954, 73285767, 1, 16);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515954, 73285767, 16);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515954, 73209956, 1, 17);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515954, 73209956, 17);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515954, 73273000, 1, 18);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515954, 73273000, 18);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515954, 73255331, 1, 19);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515954, 73255331, 19);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515954, 73242650, 1, 20);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515954, 73242650, 20);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515954, 73294708, 1, 21);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515954, 73294708, 21);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515954, 73216349, 1, 22);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515954, 73216349, 22);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515954, 73214266, 1, 23);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515954, 73214266, 23);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515954, 73250455, 1, 24);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515954, 73250455, 24);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515954, 73213247, 1, 25);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515954, 73213247, 25);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515954, 73211196, 1, 26);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515954, 73211196, 26);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515954, 73224877, 1, 27);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515954, 73224877, 27);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515954, 73262290, 1, 28);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515954, 73262290, 28);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515954, 73229161, 1, 29);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515954, 73229161, 29);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515954, 73264246, 1, 30);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515954, 73264246, 30);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515954, 73305792, 1, 31);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515954, 73305792, 31);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515954, 73247268, 1, 32);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515954, 73247268, 32);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9515954, 73221959, 1, 33);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9515954, 73221959, 33);
/* create way 313033877*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (313033877, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (313033877, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (313033877, 'name', 'Arlington Street');
INSERT INTO way_tags (way_id, k, v, version) VALUES (313033877, 'name', 'Arlington Street', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (313033877, 'condition', 'fair');
INSERT INTO way_tags (way_id, k, v, version) VALUES (313033877, 'condition', 'fair', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (313033877, 'lanes', '2');
INSERT INTO way_tags (way_id, k, v, version) VALUES (313033877, 'lanes', '2', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (313033877, 'source', 'massgis_import_v0.1_20071009100433');
INSERT INTO way_tags (way_id, k, v, version) VALUES (313033877, 'source', 'massgis_import_v0.1_20071009100433', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (313033877, 'width', '10.4');
INSERT INTO way_tags (way_id, k, v, version) VALUES (313033877, 'width', '10.4', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (313033877, 'massgis:way_id', '154676');
INSERT INTO way_tags (way_id, k, v, version) VALUES (313033877, 'massgis:way_id', '154676', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (313033877, 'highway', 'tertiary');
INSERT INTO way_tags (way_id, k, v, version) VALUES (313033877, 'highway', 'tertiary', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (313033877, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO way_tags (way_id, k, v, version) VALUES (313033877, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (313033877, 73217679, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (313033877, 73217679, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (313033877, 73270248, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (313033877, 73270248, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (313033877, 73223635, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (313033877, 73223635, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (313033877, 73282640, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (313033877, 73282640, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (313033877, 73286780, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (313033877, 73286780, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (313033877, 73212758, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (313033877, 73212758, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (313033877, 73286854, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (313033877, 73286854, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (313033877, 73200871, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (313033877, 73200871, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (313033877, 73309070, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (313033877, 73309070, 9);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (313033877, 73235656, 1, 10);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (313033877, 73235656, 10);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (313033877, 73287250, 1, 11);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (313033877, 73287250, 11);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (313033877, 73250382, 1, 12);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (313033877, 73250382, 12);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (313033877, 73295702, 1, 13);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (313033877, 73295702, 13);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (313033877, 73210632, 1, 14);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (313033877, 73210632, 14);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (313033877, 73270673, 1, 15);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (313033877, 73270673, 15);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (313033877, 73214598, 1, 16);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (313033877, 73214598, 16);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (313033877, 73304035, 1, 17);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (313033877, 73304035, 17);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (313033877, 73252638, 1, 18);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (313033877, 73252638, 18);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (313033877, 73305363, 1, 19);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (313033877, 73305363, 19);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (313033877, 73211079, 1, 20);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (313033877, 73211079, 20);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (313033877, 73285556, 1, 21);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (313033877, 73285556, 21);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (313033877, 73291671, 1, 22);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (313033877, 73291671, 22);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (313033877, 73211935, 1, 23);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (313033877, 73211935, 23);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (313033877, 73291062, 1, 24);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (313033877, 73291062, 24);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (313033877, 73210843, 1, 25);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (313033877, 73210843, 25);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (313033877, 3306483744, 1, 26);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (313033877, 3306483744, 26);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (313033877, 73216364, 1, 27);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (313033877, 73216364, 27);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (313033877, 73264165, 1, 28);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (313033877, 73264165, 28);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (313033877, 73226144, 1, 29);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (313033877, 73226144, 29);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (313033877, 73292960, 1, 30);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (313033877, 73292960, 30);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (313033877, 73216703, 1, 31);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (313033877, 73216703, 31);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (313033877, 73287411, 1, 32);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (313033877, 73287411, 32);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (313033877, 73310821, 1, 33);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (313033877, 73310821, 33);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (313033877, 73301829, 1, 34);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (313033877, 73301829, 34);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (313033877, 73293749, 1, 35);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (313033877, 73293749, 35);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (313033877, 73311056, 1, 36);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (313033877, 73311056, 36);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (313033877, 73213119, 1, 37);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (313033877, 73213119, 37);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (313033877, 73283089, 1, 38);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (313033877, 73283089, 38);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (313033877, 73222820, 1, 39);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (313033877, 73222820, 39);
UPDATE changesets SET min_lat=-9748, max_lat=424946510, min_lon=-714735879, max_lon=12528, num_changes=372 WHERE id=2;
