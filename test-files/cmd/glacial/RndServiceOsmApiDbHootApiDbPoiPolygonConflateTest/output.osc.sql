INSERT INTO changesets (id, user_id, created_at, closed_at) VALUES (3, 1, (now() at time zone 'utc'), (now() at time zone 'utc'));
INSERT INTO changeset_tags (changeset_id, k, v) VALUES (3, 'written_by', 'Hootenanny');
/* delete node 5*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (5, 377627526, -1224195999, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 5;
DELETE FROM node_tags WHERE node_id = 5;
DELETE FROM current_way_nodes WHERE node_id=5;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 5;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=5;
/* delete node 6*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (6, 377627592, -1224195999, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 6;
DELETE FROM node_tags WHERE node_id = 6;
DELETE FROM current_way_nodes WHERE node_id=6;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 6;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=6;
/* delete node 7*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7, 377624302, -1224195999, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 7;
DELETE FROM node_tags WHERE node_id = 7;
DELETE FROM current_way_nodes WHERE node_id=7;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 7;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=7;
/* delete node 8*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (8, 377623582, -1224195999, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 8;
DELETE FROM node_tags WHERE node_id = 8;
DELETE FROM current_way_nodes WHERE node_id=8;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 8;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=8;
/* delete node 9*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (9, 377623582, -1224195999, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 9;
DELETE FROM node_tags WHERE node_id = 9;
DELETE FROM current_way_nodes WHERE node_id=9;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 9;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=9;
/* delete node 10*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (10, 377622886, -1224195999, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 10;
DELETE FROM node_tags WHERE node_id = 10;
DELETE FROM current_way_nodes WHERE node_id=10;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 10;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=10;
/* delete node 11*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11, 377622250, -1224195999, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 11;
DELETE FROM node_tags WHERE node_id = 11;
DELETE FROM current_way_nodes WHERE node_id=11;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=11;
/* delete node 12*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12, 377620776, -1224195999, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 12;
DELETE FROM node_tags WHERE node_id = 12;
DELETE FROM current_way_nodes WHERE node_id=12;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=12;
/* delete node 13*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13, 377625025, -1224195999, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 13;
DELETE FROM node_tags WHERE node_id = 13;
DELETE FROM current_way_nodes WHERE node_id=13;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=13;
/* delete node 14*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (14, 377624302, -1224195999, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 14;
DELETE FROM node_tags WHERE node_id = 14;
DELETE FROM current_way_nodes WHERE node_id=14;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 14;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=14;
/* delete node 15*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (15, 377620776, -1224195999, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 15;
DELETE FROM node_tags WHERE node_id = 15;
DELETE FROM current_way_nodes WHERE node_id=15;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 15;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=15;
/* delete node 16*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (16, 377619142, -1224195999, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 16;
DELETE FROM node_tags WHERE node_id = 16;
DELETE FROM current_way_nodes WHERE node_id=16;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 16;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=16;
/* delete node 17*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (17, 377622250, -1224195999, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 17;
DELETE FROM node_tags WHERE node_id = 17;
DELETE FROM current_way_nodes WHERE node_id=17;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 17;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=17;
/* delete node 18*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (18, 377622886, -1224195999, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 18;
DELETE FROM node_tags WHERE node_id = 18;
DELETE FROM current_way_nodes WHERE node_id=18;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 18;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=18;
/* delete node 19*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (19, 377626590, -1224195999, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 19;
DELETE FROM node_tags WHERE node_id = 19;
DELETE FROM current_way_nodes WHERE node_id=19;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 19;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=19;
/* delete node 20*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (20, 377625700, -1224195999, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 20;
DELETE FROM node_tags WHERE node_id = 20;
DELETE FROM current_way_nodes WHERE node_id=20;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 20;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=20;
/* delete node 21*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (21, 377625700, -1224195999, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 21;
DELETE FROM node_tags WHERE node_id = 21;
DELETE FROM current_way_nodes WHERE node_id=21;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 21;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=21;
/* delete node 22*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (22, 377625025, -1224195999, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 22;
DELETE FROM node_tags WHERE node_id = 22;
DELETE FROM current_way_nodes WHERE node_id=22;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 22;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=22;
/* delete node 27*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (27, 377651000, -1224213486, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 27;
DELETE FROM node_tags WHERE node_id = 27;
DELETE FROM current_way_nodes WHERE node_id=27;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 27;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=27;
/* delete node 28*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (28, 377651000, -1224211840, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 28;
DELETE FROM node_tags WHERE node_id = 28;
DELETE FROM current_way_nodes WHERE node_id=28;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 28;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=28;
/* delete node 29*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (29, 377618000, -1224195999, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 29;
DELETE FROM node_tags WHERE node_id = 29;
DELETE FROM current_way_nodes WHERE node_id=29;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 29;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=29;
/* delete node 30*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (30, 377650414, -1224195999, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 30;
DELETE FROM node_tags WHERE node_id = 30;
DELETE FROM current_way_nodes WHERE node_id=30;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 30;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=30;
/* delete node 31*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (31, 377618000, -1224197813, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 31;
DELETE FROM node_tags WHERE node_id = 31;
DELETE FROM current_way_nodes WHERE node_id=31;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 31;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=31;
/* delete node 34*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (34, 377649170, -1224218859, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 34;
DELETE FROM node_tags WHERE node_id = 34;
DELETE FROM current_way_nodes WHERE node_id=34;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 34;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=34;
/* delete node 35*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (35, 377650498, -1224196888, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 35;
DELETE FROM node_tags WHERE node_id = 35;
DELETE FROM current_way_nodes WHERE node_id=35;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 35;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=35;
/* delete node 37*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (37, 377631680, -1224239099, 3, false, (now() at time zone 'utc'), 1301409554, 2);
DELETE FROM current_node_tags WHERE node_id = 37;
DELETE FROM node_tags WHERE node_id = 37;
DELETE FROM current_way_nodes WHERE node_id=37;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 37;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=37;
/* delete node 46*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (46, 377650685, -1224211805, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 46;
DELETE FROM node_tags WHERE node_id = 46;
DELETE FROM current_way_nodes WHERE node_id=46;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 46;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=46;
/* delete node 47*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (47, 377650571, -1224213438, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 47;
DELETE FROM node_tags WHERE node_id = 47;
DELETE FROM current_way_nodes WHERE node_id=47;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 47;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=47;
/* delete node 48*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (48, 377640647, -1224202133, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 48;
DELETE FROM node_tags WHERE node_id = 48;
DELETE FROM current_way_nodes WHERE node_id=48;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 48;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=48;
/* delete node 49*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (49, 377642613, -1224202243, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 49;
DELETE FROM node_tags WHERE node_id = 49;
DELETE FROM current_way_nodes WHERE node_id=49;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 49;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=49;
/* delete node 54*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (54, 377649310, -1224232383, 3, false, (now() at time zone 'utc'), 1301409555, 2);
DELETE FROM current_node_tags WHERE node_id = 54;
DELETE FROM node_tags WHERE node_id = 54;
DELETE FROM current_way_nodes WHERE node_id=54;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 54;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=54;
/* delete node 56*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (56, 377646257, -1224239611, 3, false, (now() at time zone 'utc'), 1301409555, 2);
DELETE FROM current_node_tags WHERE node_id = 56;
DELETE FROM node_tags WHERE node_id = 56;
DELETE FROM current_way_nodes WHERE node_id=56;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 56;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=56;
/* delete node 59*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (59, 377647708, -1224224622, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 59;
DELETE FROM node_tags WHERE node_id = 59;
DELETE FROM current_way_nodes WHERE node_id=59;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 59;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=59;
/* delete node 61*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (61, 377647314, -1224229830, 3, false, (now() at time zone 'utc'), 1301409555, 2);
DELETE FROM current_node_tags WHERE node_id = 61;
DELETE FROM node_tags WHERE node_id = 61;
DELETE FROM current_way_nodes WHERE node_id=61;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 61;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=61;
/* delete node 62*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (62, 377647462, -1224200305, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 62;
DELETE FROM node_tags WHERE node_id = 62;
DELETE FROM current_way_nodes WHERE node_id=62;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 62;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=62;
/* delete node 63*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (63, 377650072, -1224201691, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 63;
DELETE FROM node_tags WHERE node_id = 63;
DELETE FROM current_way_nodes WHERE node_id=63;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 63;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=63;
/* delete node 67*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (67, 377629855, -1224199709, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 67;
DELETE FROM node_tags WHERE node_id = 67;
DELETE FROM current_way_nodes WHERE node_id=67;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 67;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=67;
/* delete node 68*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (68, 377649632, -1224236142, 3, false, (now() at time zone 'utc'), 1301409555, 2);
DELETE FROM current_node_tags WHERE node_id = 68;
DELETE FROM node_tags WHERE node_id = 68;
DELETE FROM current_way_nodes WHERE node_id=68;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 68;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=68;
/* delete node 69*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (69, 377649400, -1224200276, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 69;
DELETE FROM node_tags WHERE node_id = 69;
DELETE FROM current_way_nodes WHERE node_id=69;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 69;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=69;
/* delete node 70*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (70, 377649198, -1224202861, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 70;
DELETE FROM node_tags WHERE node_id = 70;
DELETE FROM current_way_nodes WHERE node_id=70;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 70;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=70;
/* delete node 71*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (71, 377649327, -1224203539, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 71;
DELETE FROM node_tags WHERE node_id = 71;
DELETE FROM current_way_nodes WHERE node_id=71;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 71;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=71;
/* delete node 72*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (72, 377649446, -1224201696, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 72;
DELETE FROM node_tags WHERE node_id = 72;
DELETE FROM current_way_nodes WHERE node_id=72;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 72;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=72;
/* delete node 73*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (73, 377646765, -1224201418, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 73;
DELETE FROM node_tags WHERE node_id = 73;
DELETE FROM current_way_nodes WHERE node_id=73;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 73;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=73;
/* delete node 74*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (74, 377646646, -1224203261, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 74;
DELETE FROM node_tags WHERE node_id = 74;
DELETE FROM current_way_nodes WHERE node_id=74;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 74;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=74;
/* delete node 75*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (75, 377647569, -1224217023, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 75;
DELETE FROM node_tags WHERE node_id = 75;
DELETE FROM current_way_nodes WHERE node_id=75;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 75;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=75;
/* delete node 77*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (77, 377648614, -1224210767, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 77;
DELETE FROM node_tags WHERE node_id = 77;
DELETE FROM current_way_nodes WHERE node_id=77;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 77;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=77;
/* delete node 79*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (79, 377635994, -1224237890, 3, false, (now() at time zone 'utc'), 1301409555, 2);
DELETE FROM current_node_tags WHERE node_id = 79;
DELETE FROM node_tags WHERE node_id = 79;
DELETE FROM current_way_nodes WHERE node_id=79;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 79;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=79;
/* delete node 80*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (80, 377649400, -1224199504, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 80;
DELETE FROM node_tags WHERE node_id = 80;
DELETE FROM current_way_nodes WHERE node_id=80;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 80;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=80;
/* delete node 81*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (81, 377648246, -1224197530, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 81;
DELETE FROM node_tags WHERE node_id = 81;
DELETE FROM current_way_nodes WHERE node_id=81;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 81;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=81;
/* delete node 82*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (82, 377618662, -1224197444, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 82;
DELETE FROM node_tags WHERE node_id = 82;
DELETE FROM current_way_nodes WHERE node_id=82;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 82;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=82;
/* delete node 83*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (83, 377650919, -1224216926, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 83;
DELETE FROM node_tags WHERE node_id = 83;
DELETE FROM current_way_nodes WHERE node_id=83;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 83;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=83;
/* delete node 85*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (85, 377650583, -1224220700, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 85;
DELETE FROM node_tags WHERE node_id = 85;
DELETE FROM current_way_nodes WHERE node_id=85;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 85;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=85;
/* delete node 88*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (88, 377647640, -1224227283, 3, false, (now() at time zone 'utc'), 1301409555, 2);
DELETE FROM current_node_tags WHERE node_id = 88;
DELETE FROM node_tags WHERE node_id = 88;
DELETE FROM current_way_nodes WHERE node_id=88;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 88;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=88;
/* delete node 90*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (90, 377649404, -1224239385, 3, false, (now() at time zone 'utc'), 1301409555, 2);
DELETE FROM current_node_tags WHERE node_id = 90;
DELETE FROM node_tags WHERE node_id = 90;
DELETE FROM current_way_nodes WHERE node_id=90;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 90;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=90;
/* delete node 91*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (91, 377649564, -1224237344, 3, false, (now() at time zone 'utc'), 1301409555, 2);
DELETE FROM current_node_tags WHERE node_id = 91;
DELETE FROM node_tags WHERE node_id = 91;
DELETE FROM current_way_nodes WHERE node_id=91;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 91;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=91;
/* delete node 92*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (92, 377646987, -1224235800, 3, false, (now() at time zone 'utc'), 1301409555, 2);
DELETE FROM current_node_tags WHERE node_id = 92;
DELETE FROM node_tags WHERE node_id = 92;
DELETE FROM current_way_nodes WHERE node_id=92;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 92;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=92;
/* delete node 93*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (93, 377647122, -1224233224, 3, false, (now() at time zone 'utc'), 1301409555, 2);
DELETE FROM current_node_tags WHERE node_id = 93;
DELETE FROM node_tags WHERE node_id = 93;
DELETE FROM current_way_nodes WHERE node_id=93;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 93;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=93;
/* delete node 94*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (94, 377632581, -1224196732, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 94;
DELETE FROM node_tags WHERE node_id = 94;
DELETE FROM current_way_nodes WHERE node_id=94;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 94;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=94;
/* delete node 95*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (95, 377648278, -1224222608, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 95;
DELETE FROM node_tags WHERE node_id = 95;
DELETE FROM current_way_nodes WHERE node_id=95;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 95;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=95;
/* delete node 96*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (96, 377649940, -1224226909, 3, false, (now() at time zone 'utc'), 1301409555, 2);
DELETE FROM current_node_tags WHERE node_id = 96;
DELETE FROM node_tags WHERE node_id = 96;
DELETE FROM current_way_nodes WHERE node_id=96;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 96;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=96;
/* delete node 97*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (97, 377649810, -1224223199, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 97;
DELETE FROM node_tags WHERE node_id = 97;
DELETE FROM current_way_nodes WHERE node_id=97;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 97;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=97;
/* delete node 98*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (98, 377647407, -1224220141, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 98;
DELETE FROM node_tags WHERE node_id = 98;
DELETE FROM current_way_nodes WHERE node_id=98;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 98;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=98;
/* delete node 100*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (100, 377646488, -1224197150, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 100;
DELETE FROM node_tags WHERE node_id = 100;
DELETE FROM current_way_nodes WHERE node_id=100;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 100;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=100;
/* delete node 101*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (101, 377646234, -1224201241, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 101;
DELETE FROM node_tags WHERE node_id = 101;
DELETE FROM current_way_nodes WHERE node_id=101;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 101;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=101;
/* delete node 102*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (102, 377649621, -1224201577, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 102;
DELETE FROM node_tags WHERE node_id = 102;
DELETE FROM current_way_nodes WHERE node_id=102;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 102;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=102;
/* delete node 103*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (103, 377649875, -1224197486, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 103;
DELETE FROM node_tags WHERE node_id = 103;
DELETE FROM current_way_nodes WHERE node_id=103;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 103;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=103;
/* delete node 107*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (107, 377630525, -1224199611, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 107;
DELETE FROM node_tags WHERE node_id = 107;
DELETE FROM current_way_nodes WHERE node_id=107;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 107;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=107;
/* delete node 108*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (108, 377630734, -1224196305, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 108;
DELETE FROM node_tags WHERE node_id = 108;
DELETE FROM current_way_nodes WHERE node_id=108;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 108;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=108;
/* delete node 109*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (109, 377631140, -1224198902, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 109;
DELETE FROM node_tags WHERE node_id = 109;
DELETE FROM current_way_nodes WHERE node_id=109;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 109;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=109;
/* delete node 110*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (110, 377631193, -1224198058, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 110;
DELETE FROM node_tags WHERE node_id = 110;
DELETE FROM current_way_nodes WHERE node_id=110;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 110;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=110;
/* delete node 111*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (111, 377631302, -1224199690, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 111;
DELETE FROM node_tags WHERE node_id = 111;
DELETE FROM current_way_nodes WHERE node_id=111;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 111;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=111;
/* delete node 112*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (112, 377631314, -1224199503, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 112;
DELETE FROM node_tags WHERE node_id = 112;
DELETE FROM current_way_nodes WHERE node_id=112;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 112;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=112;
/* delete node 113*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (113, 377631350, -1224198924, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 113;
DELETE FROM node_tags WHERE node_id = 113;
DELETE FROM current_way_nodes WHERE node_id=113;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 113;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=113;
/* delete node 114*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (114, 377631362, -1224197418, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 114;
DELETE FROM node_tags WHERE node_id = 114;
DELETE FROM current_way_nodes WHERE node_id=114;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 114;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=114;
/* delete node 115*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (115, 377631390, -1224196973, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 115;
DELETE FROM node_tags WHERE node_id = 115;
DELETE FROM current_way_nodes WHERE node_id=115;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 115;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=115;
/* delete node 116*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (116, 377631404, -1224198080, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 116;
DELETE FROM node_tags WHERE node_id = 116;
DELETE FROM current_way_nodes WHERE node_id=116;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 116;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=116;
/* delete node 117*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (117, 377631445, -1224197427, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 117;
DELETE FROM node_tags WHERE node_id = 117;
DELETE FROM current_way_nodes WHERE node_id=117;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 117;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=117;
/* delete node 118*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (118, 377631473, -1224196981, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 118;
DELETE FROM node_tags WHERE node_id = 118;
DELETE FROM current_way_nodes WHERE node_id=118;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 118;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=118;
/* delete node 119*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (119, 377631511, -1224196383, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 119;
DELETE FROM node_tags WHERE node_id = 119;
DELETE FROM current_way_nodes WHERE node_id=119;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 119;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=119;
/* delete node 120*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (120, 377633209, -1224199695, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 120;
DELETE FROM node_tags WHERE node_id = 120;
DELETE FROM current_way_nodes WHERE node_id=120;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 120;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=120;
/* delete node 121*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (121, 377633406, -1224196575, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 121;
DELETE FROM node_tags WHERE node_id = 121;
DELETE FROM current_way_nodes WHERE node_id=121;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 121;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=121;
/* delete node 122*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (122, 377633924, -1224199658, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 122;
DELETE FROM node_tags WHERE node_id = 122;
DELETE FROM current_way_nodes WHERE node_id=122;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 122;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=122;
/* delete node 123*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (123, 377634138, -1224200163, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 123;
DELETE FROM node_tags WHERE node_id = 123;
DELETE FROM current_way_nodes WHERE node_id=123;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 123;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=123;
/* delete node 124*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (124, 377634965, -1224199292, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 124;
DELETE FROM node_tags WHERE node_id = 124;
DELETE FROM current_way_nodes WHERE node_id=124;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 124;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=124;
/* delete node 125*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (125, 377635119, -1224196664, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 125;
DELETE FROM node_tags WHERE node_id = 125;
DELETE FROM current_way_nodes WHERE node_id=125;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 125;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=125;
/* delete node 126*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (126, 377635606, -1224199355, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 126;
DELETE FROM node_tags WHERE node_id = 126;
DELETE FROM current_way_nodes WHERE node_id=126;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 126;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=126;
/* delete node 127*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (127, 377635626, -1224198994, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 127;
DELETE FROM node_tags WHERE node_id = 127;
DELETE FROM current_way_nodes WHERE node_id=127;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 127;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=127;
/* delete node 128*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (128, 377635710, -1224199003, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 128;
DELETE FROM node_tags WHERE node_id = 128;
DELETE FROM current_way_nodes WHERE node_id=128;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 128;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=128;
/* delete node 129*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (129, 377635859, -1224196742, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 129;
DELETE FROM node_tags WHERE node_id = 129;
DELETE FROM current_way_nodes WHERE node_id=129;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 129;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=129;
/* delete node 130*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (130, 377640034, -1224200820, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 130;
DELETE FROM node_tags WHERE node_id = 130;
DELETE FROM current_way_nodes WHERE node_id=130;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 130;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=130;
/* delete node 131*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (131, 377640274, -1224197179, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 131;
DELETE FROM node_tags WHERE node_id = 131;
DELETE FROM current_way_nodes WHERE node_id=131;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 131;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=131;
/* delete node 132*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (132, 377640676, -1224201229, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 132;
DELETE FROM node_tags WHERE node_id = 132;
DELETE FROM current_way_nodes WHERE node_id=132;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 132;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=132;
/* delete node 133*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (133, 377640699, -1224200890, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 133;
DELETE FROM node_tags WHERE node_id = 133;
DELETE FROM current_way_nodes WHERE node_id=133;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 133;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=133;
/* delete node 134*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (134, 377640939, -1224197247, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 134;
DELETE FROM node_tags WHERE node_id = 134;
DELETE FROM current_way_nodes WHERE node_id=134;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 134;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=134;
/* delete node 135*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (135, 377641385, -1224201305, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 135;
DELETE FROM node_tags WHERE node_id = 135;
DELETE FROM current_way_nodes WHERE node_id=135;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 135;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=135;
/* delete node 136*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (136, 377641650, -1224197308, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 136;
DELETE FROM node_tags WHERE node_id = 136;
DELETE FROM current_way_nodes WHERE node_id=136;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 136;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=136;
/* delete node 137*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (137, 377641954, -1224201365, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 137;
DELETE FROM node_tags WHERE node_id = 137;
DELETE FROM current_way_nodes WHERE node_id=137;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 137;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=137;
/* delete node 138*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (138, 377642041, -1224200053, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 138;
DELETE FROM node_tags WHERE node_id = 138;
DELETE FROM current_way_nodes WHERE node_id=138;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 138;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=138;
/* delete node 139*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (139, 377642219, -1224197369, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 139;
DELETE FROM node_tags WHERE node_id = 139;
DELETE FROM current_way_nodes WHERE node_id=139;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 139;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=139;
/* delete node 140*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (140, 377642603, -1224201530, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 140;
DELETE FROM node_tags WHERE node_id = 140;
DELETE FROM current_way_nodes WHERE node_id=140;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 140;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=140;
/* delete node 141*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (141, 377642685, -1224200121, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 141;
DELETE FROM node_tags WHERE node_id = 141;
DELETE FROM current_way_nodes WHERE node_id=141;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 141;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=141;
/* delete node 142*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (142, 377642863, -1224197437, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 142;
DELETE FROM node_tags WHERE node_id = 142;
DELETE FROM current_way_nodes WHERE node_id=142;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 142;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=142;
/* delete node 143*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (143, 377643282, -1224200993, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 143;
DELETE FROM node_tags WHERE node_id = 143;
DELETE FROM current_way_nodes WHERE node_id=143;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 143;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=143;
/* delete node 144*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (144, 377643322, -1224200339, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 144;
DELETE FROM node_tags WHERE node_id = 144;
DELETE FROM current_way_nodes WHERE node_id=144;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 144;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=144;
/* delete node 145*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (145, 377643499, -1224197491, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 145;
DELETE FROM node_tags WHERE node_id = 145;
DELETE FROM current_way_nodes WHERE node_id=145;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 145;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=145;
/* delete node 146*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (146, 377643576, -1224201626, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 146;
DELETE FROM node_tags WHERE node_id = 146;
DELETE FROM current_way_nodes WHERE node_id=146;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 146;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=146;
/* delete node 147*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (147, 377643613, -1224201026, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 147;
DELETE FROM node_tags WHERE node_id = 147;
DELETE FROM current_way_nodes WHERE node_id=147;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 147;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=147;
/* delete node 148*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (148, 377643950, -1224200795, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 148;
DELETE FROM node_tags WHERE node_id = 148;
DELETE FROM current_way_nodes WHERE node_id=148;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 148;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=148;
/* delete node 149*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (149, 377643974, -1224200403, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 149;
DELETE FROM node_tags WHERE node_id = 149;
DELETE FROM current_way_nodes WHERE node_id=149;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 149;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=149;
/* delete node 150*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (150, 377644095, -1224201110, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 150;
DELETE FROM node_tags WHERE node_id = 150;
DELETE FROM current_way_nodes WHERE node_id=150;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 150;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=150;
/* delete node 151*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (151, 377644114, -1224200811, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 151;
DELETE FROM node_tags WHERE node_id = 151;
DELETE FROM current_way_nodes WHERE node_id=151;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 151;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=151;
/* delete node 152*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (152, 377644151, -1224197556, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 152;
DELETE FROM node_tags WHERE node_id = 152;
DELETE FROM current_way_nodes WHERE node_id=152;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 152;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=152;
/* delete node 153*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (153, 377644358, -1224201136, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 153;
DELETE FROM node_tags WHERE node_id = 153;
DELETE FROM current_way_nodes WHERE node_id=153;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 153;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=153;
/* delete node 154*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (154, 377644364, -1224201039, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 154;
DELETE FROM node_tags WHERE node_id = 154;
DELETE FROM current_way_nodes WHERE node_id=154;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 154;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=154;
/* delete node 155*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (155, 377644626, -1224201065, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 155;
DELETE FROM node_tags WHERE node_id = 155;
DELETE FROM current_way_nodes WHERE node_id=155;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 155;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=155;
/* delete node 156*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (156, 377644629, -1224201029, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 156;
DELETE FROM node_tags WHERE node_id = 156;
DELETE FROM current_way_nodes WHERE node_id=156;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 156;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=156;
/* delete node 157*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (157, 377644684, -1224233483, 3, false, (now() at time zone 'utc'), 1301409555, 2);
DELETE FROM current_node_tags WHERE node_id = 157;
DELETE FROM node_tags WHERE node_id = 157;
DELETE FROM current_way_nodes WHERE node_id=157;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 157;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=157;
/* delete node 159*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (159, 377644749, -1224232421, 3, false, (now() at time zone 'utc'), 1301409555, 2);
DELETE FROM current_node_tags WHERE node_id = 159;
DELETE FROM node_tags WHERE node_id = 159;
DELETE FROM current_way_nodes WHERE node_id=159;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 159;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=159;
/* delete node 160*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (160, 377644840, -1224197624, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 160;
DELETE FROM node_tags WHERE node_id = 160;
DELETE FROM current_way_nodes WHERE node_id=160;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 160;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=160;
/* delete node 161*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (161, 377645232, -1224239576, 3, false, (now() at time zone 'utc'), 1301409555, 2);
DELETE FROM current_node_tags WHERE node_id = 161;
DELETE FROM node_tags WHERE node_id = 161;
DELETE FROM current_way_nodes WHERE node_id=161;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 161;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=161;
/* delete node 162*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (162, 377645262, -1224201092, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 162;
DELETE FROM node_tags WHERE node_id = 162;
DELETE FROM current_way_nodes WHERE node_id=162;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 162;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=162;
/* delete node 164*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (164, 377645267, -1224231473, 3, false, (now() at time zone 'utc'), 1301409555, 2);
DELETE FROM current_node_tags WHERE node_id = 164;
DELETE FROM node_tags WHERE node_id = 164;
DELETE FROM current_way_nodes WHERE node_id=164;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 164;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=164;
/* delete node 165*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (165, 377645269, -1224200974, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 165;
DELETE FROM node_tags WHERE node_id = 165;
DELETE FROM current_way_nodes WHERE node_id=165;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 165;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=165;
/* delete node 169*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (169, 377645444, -1224232489, 3, false, (now() at time zone 'utc'), 1301409555, 2);
DELETE FROM current_node_tags WHERE node_id = 169;
DELETE FROM node_tags WHERE node_id = 169;
DELETE FROM current_way_nodes WHERE node_id=169;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 169;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=169;
/* delete node 170*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (170, 377645474, -1224197687, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 170;
DELETE FROM node_tags WHERE node_id = 170;
DELETE FROM current_way_nodes WHERE node_id=170;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 170;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=170;
/* delete node 171*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (171, 377645505, -1224231496, 3, false, (now() at time zone 'utc'), 1301409555, 2);
DELETE FROM current_node_tags WHERE node_id = 171;
DELETE FROM node_tags WHERE node_id = 171;
DELETE FROM current_way_nodes WHERE node_id=171;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 171;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=171;
/* delete node 172*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (172, 377645572, -1224233712, 3, false, (now() at time zone 'utc'), 1301409555, 2);
DELETE FROM current_node_tags WHERE node_id = 172;
DELETE FROM node_tags WHERE node_id = 172;
DELETE FROM current_way_nodes WHERE node_id=172;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 172;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=172;
/* delete node 174*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (174, 377646245, -1224201071, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 174;
DELETE FROM node_tags WHERE node_id = 174;
DELETE FROM current_way_nodes WHERE node_id=174;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 174;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=174;
/* delete node 175*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (175, 377646449, -1224197784, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 175;
DELETE FROM node_tags WHERE node_id = 175;
DELETE FROM current_way_nodes WHERE node_id=175;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 175;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=175;
/* delete node 177*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (177, 377646947, -1224239735, 3, false, (now() at time zone 'utc'), 1301409555, 2);
DELETE FROM current_node_tags WHERE node_id = 177;
DELETE FROM node_tags WHERE node_id = 177;
DELETE FROM current_way_nodes WHERE node_id=177;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 177;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=177;
/* delete node 178*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (178, 377647287, -1224233871, 3, false, (now() at time zone 'utc'), 1301409555, 2);
DELETE FROM current_node_tags WHERE node_id = 178;
DELETE FROM node_tags WHERE node_id = 178;
DELETE FROM current_way_nodes WHERE node_id=178;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 178;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=178;
/* delete node 179*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (179, 377647292, -1224233739, 3, false, (now() at time zone 'utc'), 1301409555, 2);
DELETE FROM current_node_tags WHERE node_id = 179;
DELETE FROM node_tags WHERE node_id = 179;
DELETE FROM current_way_nodes WHERE node_id=179;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 179;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=179;
/* delete node 180*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (180, 377647357, -1224232677, 3, false, (now() at time zone 'utc'), 1301409555, 2);
DELETE FROM current_node_tags WHERE node_id = 180;
DELETE FROM node_tags WHERE node_id = 180;
DELETE FROM current_way_nodes WHERE node_id=180;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 180;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=180;
/* delete node 181*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (181, 377647418, -1224231684, 3, false, (now() at time zone 'utc'), 1301409555, 2);
DELETE FROM current_node_tags WHERE node_id = 181;
DELETE FROM node_tags WHERE node_id = 181;
DELETE FROM current_way_nodes WHERE node_id=181;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 181;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=181;
/* delete node 184*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (184, 377619023, -1224197942, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 184;
DELETE FROM node_tags WHERE node_id = 184;
DELETE FROM current_way_nodes WHERE node_id=184;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 184;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=184;
/* delete node 185*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (185, 377620647, -1224198101, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 185;
DELETE FROM node_tags WHERE node_id = 185;
DELETE FROM current_way_nodes WHERE node_id=185;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 185;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=185;
/* delete node 186*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (186, 377622112, -1224198245, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 186;
DELETE FROM node_tags WHERE node_id = 186;
DELETE FROM current_way_nodes WHERE node_id=186;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 186;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=186;
/* delete node 187*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (187, 377622125, -1224198039, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 187;
DELETE FROM node_tags WHERE node_id = 187;
DELETE FROM current_way_nodes WHERE node_id=187;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 187;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=187;
/* delete node 188*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (188, 377622743, -1224198345, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 188;
DELETE FROM node_tags WHERE node_id = 188;
DELETE FROM current_way_nodes WHERE node_id=188;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 188;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=188;
/* delete node 189*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (189, 377622758, -1224198101, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 189;
DELETE FROM node_tags WHERE node_id = 189;
DELETE FROM current_way_nodes WHERE node_id=189;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 189;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=189;
/* delete node 190*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (190, 377623421, -1224198619, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 190;
DELETE FROM node_tags WHERE node_id = 190;
DELETE FROM current_way_nodes WHERE node_id=190;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 190;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=190;
/* delete node 191*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (191, 377623434, -1224198413, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 191;
DELETE FROM node_tags WHERE node_id = 191;
DELETE FROM current_way_nodes WHERE node_id=191;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 191;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=191;
/* delete node 192*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (192, 377624137, -1224198689, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 192;
DELETE FROM node_tags WHERE node_id = 192;
DELETE FROM current_way_nodes WHERE node_id=192;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 192;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=192;
/* delete node 193*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (193, 377624153, -1224198430, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 193;
DELETE FROM node_tags WHERE node_id = 193;
DELETE FROM current_way_nodes WHERE node_id=193;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 193;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=193;
/* delete node 194*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (194, 377624872, -1224198500, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 194;
DELETE FROM node_tags WHERE node_id = 194;
DELETE FROM current_way_nodes WHERE node_id=194;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 194;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=194;
/* delete node 195*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (195, 377625543, -1224198566, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 195;
DELETE FROM node_tags WHERE node_id = 195;
DELETE FROM current_way_nodes WHERE node_id=195;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 195;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=195;
/* delete node 196*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (196, 377626427, -1224198653, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 196;
DELETE FROM node_tags WHERE node_id = 196;
DELETE FROM current_way_nodes WHERE node_id=196;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 196;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=196;
/* delete node 197*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (197, 377627323, -1224199595, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 197;
DELETE FROM node_tags WHERE node_id = 197;
DELETE FROM current_way_nodes WHERE node_id=197;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 197;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=197;
/* delete node 198*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (198, 377627368, -1224198802, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 198;
DELETE FROM node_tags WHERE node_id = 198;
DELETE FROM current_way_nodes WHERE node_id=198;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 198;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=198;
/* delete node 199*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (199, 377627407, -1224198105, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 199;
DELETE FROM node_tags WHERE node_id = 199;
DELETE FROM current_way_nodes WHERE node_id=199;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 199;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=199;
/* delete node 200*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (200, 377628179, -1224198876, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 200;
DELETE FROM node_tags WHERE node_id = 200;
DELETE FROM current_way_nodes WHERE node_id=200;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 200;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=200;
/* delete node 201*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (201, 377628186, -1224198750, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 201;
DELETE FROM node_tags WHERE node_id = 201;
DELETE FROM current_way_nodes WHERE node_id=201;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 201;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=201;
/* delete node 202*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (202, 377628218, -1224198178, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 202;
DELETE FROM node_tags WHERE node_id = 202;
DELETE FROM current_way_nodes WHERE node_id=202;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 202;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=202;
/* delete node 203*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (203, 377628227, -1224198018, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 203;
DELETE FROM node_tags WHERE node_id = 203;
DELETE FROM current_way_nodes WHERE node_id=203;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 203;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=203;
/* delete node 204*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (204, 377628288, -1224196936, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 204;
DELETE FROM node_tags WHERE node_id = 204;
DELETE FROM current_way_nodes WHERE node_id=204;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 204;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=204;
/* delete node 205*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (205, 377628324, -1224198027, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 205;
DELETE FROM node_tags WHERE node_id = 205;
DELETE FROM current_way_nodes WHERE node_id=205;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 205;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=205;
/* delete node 206*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (206, 377628337, -1224196067, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 206;
DELETE FROM node_tags WHERE node_id = 206;
DELETE FROM current_way_nodes WHERE node_id=206;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 206;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=206;
/* delete node 207*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (207, 377628385, -1224196945, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 207;
DELETE FROM node_tags WHERE node_id = 207;
DELETE FROM current_way_nodes WHERE node_id=207;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 207;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=207;
/* delete node 208*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (208, 377628442, -1224198899, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 208;
DELETE FROM node_tags WHERE node_id = 208;
DELETE FROM current_way_nodes WHERE node_id=208;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 208;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=208;
/* delete node 209*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (209, 377628449, -1224198774, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 209;
DELETE FROM node_tags WHERE node_id = 209;
DELETE FROM current_way_nodes WHERE node_id=209;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 209;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=209;
/* delete node 210*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (210, 377629138, -1224198962, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 210;
DELETE FROM node_tags WHERE node_id = 210;
DELETE FROM current_way_nodes WHERE node_id=210;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 210;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=210;
/* delete node 211*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (211, 377629143, -1224198868, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 211;
DELETE FROM node_tags WHERE node_id = 211;
DELETE FROM current_way_nodes WHERE node_id=211;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 211;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=211;
/* delete node 212*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (212, 377629346, -1224198132, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 212;
DELETE FROM node_tags WHERE node_id = 212;
DELETE FROM current_way_nodes WHERE node_id=212;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 212;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=212;
/* delete node 213*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (213, 377629408, -1224197049, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 213;
DELETE FROM node_tags WHERE node_id = 213;
DELETE FROM current_way_nodes WHERE node_id=213;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 213;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=213;
/* delete node 214*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (214, 377629447, -1224198896, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 214;
DELETE FROM node_tags WHERE node_id = 214;
DELETE FROM current_way_nodes WHERE node_id=214;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 214;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=214;
/* delete node 215*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (215, 377629480, -1224198315, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 215;
DELETE FROM node_tags WHERE node_id = 215;
DELETE FROM current_way_nodes WHERE node_id=215;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 215;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=215;
/* delete node 216*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (216, 377629490, -1224198145, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 216;
DELETE FROM node_tags WHERE node_id = 216;
DELETE FROM current_way_nodes WHERE node_id=216;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 216;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=216;
/* delete node 217*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (217, 377629551, -1224197062, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 217;
DELETE FROM node_tags WHERE node_id = 217;
DELETE FROM current_way_nodes WHERE node_id=217;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 217;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=217;
/* delete node 218*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (218, 377629600, -1224196181, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 218;
DELETE FROM node_tags WHERE node_id = 218;
DELETE FROM current_way_nodes WHERE node_id=218;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 218;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=218;
/* delete node 219*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (219, 377630034, -1224199839, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 219;
DELETE FROM node_tags WHERE node_id = 219;
DELETE FROM current_way_nodes WHERE node_id=219;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 219;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=219;
/* delete node 220*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (220, 377630079, -1224199047, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 220;
DELETE FROM node_tags WHERE node_id = 220;
DELETE FROM current_way_nodes WHERE node_id=220;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 220;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=220;
/* delete node 221*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (221, 377630149, -1224198376, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 221;
DELETE FROM node_tags WHERE node_id = 221;
DELETE FROM current_way_nodes WHERE node_id=221;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 221;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=221;
/* delete node 222*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (222, 377630270, -1224196241, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 222;
DELETE FROM node_tags WHERE node_id = 222;
DELETE FROM current_way_nodes WHERE node_id=222;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 222;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=222;
/* delete node 223*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (223, 377635703, -1224199105, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 223;
DELETE FROM node_tags WHERE node_id = 223;
DELETE FROM current_way_nodes WHERE node_id=223;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 223;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=223;
/* delete node 224*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (224, 377636725, -1224200355, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 224;
DELETE FROM node_tags WHERE node_id = 224;
DELETE FROM current_way_nodes WHERE node_id=224;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 224;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=224;
/* delete node 225*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (225, 377636800, -1224199220, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 225;
DELETE FROM node_tags WHERE node_id = 225;
DELETE FROM current_way_nodes WHERE node_id=225;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 225;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=225;
/* delete node 226*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (226, 377636956, -1224196858, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 226;
DELETE FROM node_tags WHERE node_id = 226;
DELETE FROM current_way_nodes WHERE node_id=226;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 226;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=226;
/* delete node 227*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (227, 377637889, -1224200477, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 227;
DELETE FROM node_tags WHERE node_id = 227;
DELETE FROM current_way_nodes WHERE node_id=227;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 227;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=227;
/* delete node 228*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (228, 377637902, -1224200283, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 228;
DELETE FROM node_tags WHERE node_id = 228;
DELETE FROM current_way_nodes WHERE node_id=228;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 228;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=228;
/* delete node 229*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (229, 377638150, -1224200309, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 229;
DELETE FROM node_tags WHERE node_id = 229;
DELETE FROM current_way_nodes WHERE node_id=229;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 229;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=229;
/* delete node 230*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (230, 377638163, -1224200107, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 230;
DELETE FROM node_tags WHERE node_id = 230;
DELETE FROM current_way_nodes WHERE node_id=230;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 230;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=230;
/* delete node 231*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (231, 377638368, -1224197007, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 231;
DELETE FROM node_tags WHERE node_id = 231;
DELETE FROM current_way_nodes WHERE node_id=231;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 231;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=231;
/* delete node 232*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (232, 377638807, -1224200216, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 232;
DELETE FROM node_tags WHERE node_id = 232;
DELETE FROM current_way_nodes WHERE node_id=232;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 232;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=232;
/* delete node 233*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (233, 377638810, -1224200175, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 233;
DELETE FROM node_tags WHERE node_id = 233;
DELETE FROM current_way_nodes WHERE node_id=233;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 233;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=233;
/* delete node 234*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (234, 377638999, -1224197070, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 234;
DELETE FROM node_tags WHERE node_id = 234;
DELETE FROM current_way_nodes WHERE node_id=234;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 234;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=234;
/* delete node 235*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (235, 377639417, -1224200280, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 235;
DELETE FROM node_tags WHERE node_id = 235;
DELETE FROM current_way_nodes WHERE node_id=235;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 235;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=235;
/* delete node 236*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (236, 377639422, -1224200217, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 236;
DELETE FROM node_tags WHERE node_id = 236;
DELETE FROM current_way_nodes WHERE node_id=236;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 236;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=236;
/* delete node 237*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (237, 377639625, -1224197136, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 237;
DELETE FROM node_tags WHERE node_id = 237;
DELETE FROM current_way_nodes WHERE node_id=237;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 237;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=237;
/* delete node 238*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (238, 377640069, -1224200285, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 238;
DELETE FROM node_tags WHERE node_id = 238;
DELETE FROM current_way_nodes WHERE node_id=238;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 238;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=238;
/* delete node 247*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (247, 377635113, -1224199872, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 247;
DELETE FROM node_tags WHERE node_id = 247;
DELETE FROM current_way_nodes WHERE node_id=247;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 247;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=247;
/* delete node 249*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (249, 377634830, -1224201147, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 249;
DELETE FROM node_tags WHERE node_id = 249;
DELETE FROM current_way_nodes WHERE node_id=249;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 249;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=249;
/* delete node 250*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (250, 377634905, -1224200277, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 250;
DELETE FROM node_tags WHERE node_id = 250;
DELETE FROM current_way_nodes WHERE node_id=250;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 250;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=250;
/* delete node 251*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (251, 377636560, -1224200439, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 251;
DELETE FROM node_tags WHERE node_id = 251;
DELETE FROM current_way_nodes WHERE node_id=251;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 251;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=251;
/* delete node 252*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (252, 377636620, -1224199454, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 252;
DELETE FROM node_tags WHERE node_id = 252;
DELETE FROM current_way_nodes WHERE node_id=252;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 252;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=252;
/* delete node 253*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (253, 377637399, -1224201437, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 253;
DELETE FROM node_tags WHERE node_id = 253;
DELETE FROM current_way_nodes WHERE node_id=253;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 253;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=253;
/* delete node 254*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (254, 377637461, -1224200555, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 254;
DELETE FROM node_tags WHERE node_id = 254;
DELETE FROM current_way_nodes WHERE node_id=254;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 254;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=254;
/* delete node 255*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (255, 377647569, -1224223631, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 255;
DELETE FROM node_tags WHERE node_id = 255;
DELETE FROM current_way_nodes WHERE node_id=255;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 255;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=255;
/* delete node 261*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (261, 377650804, -1224196567, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 261;
DELETE FROM node_tags WHERE node_id = 261;
DELETE FROM current_way_nodes WHERE node_id=261;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 261;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=261;
/* delete node 268*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (268, 377621084, -1224200980, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 268;
DELETE FROM node_tags WHERE node_id = 268;
DELETE FROM current_way_nodes WHERE node_id=268;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 268;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=268;
/* delete node 269*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (269, 377621211, -1224199089, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 269;
DELETE FROM node_tags WHERE node_id = 269;
DELETE FROM current_way_nodes WHERE node_id=269;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 269;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=269;
/* delete node 270*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (270, 377618942, -1224198901, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 270;
DELETE FROM node_tags WHERE node_id = 270;
DELETE FROM current_way_nodes WHERE node_id=270;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 270;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=270;
/* delete node 271*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (271, 377618815, -1224200792, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 271;
DELETE FROM node_tags WHERE node_id = 271;
DELETE FROM current_way_nodes WHERE node_id=271;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 271;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=271;
/* delete node 272*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (272, 377621020, -1224198901, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 272;
DELETE FROM node_tags WHERE node_id = 272;
DELETE FROM current_way_nodes WHERE node_id=272;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 272;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=272;
/* delete node 273*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (273, 377621041, -1224198727, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 273;
DELETE FROM node_tags WHERE node_id = 273;
DELETE FROM current_way_nodes WHERE node_id=273;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 273;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=273;
/* delete node 274*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (274, 377620967, -1224198539, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 274;
DELETE FROM node_tags WHERE node_id = 274;
DELETE FROM current_way_nodes WHERE node_id=274;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 274;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=274;
/* delete node 275*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (275, 377620919, -1224198151, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 275;
DELETE FROM node_tags WHERE node_id = 275;
DELETE FROM current_way_nodes WHERE node_id=275;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 275;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=275;
/* delete node 276*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (276, 377620850, -1224199062, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 276;
DELETE FROM node_tags WHERE node_id = 276;
DELETE FROM current_way_nodes WHERE node_id=276;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 276;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=276;
/* delete node 277*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (277, 377622748, -1224201141, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 277;
DELETE FROM node_tags WHERE node_id = 277;
DELETE FROM current_way_nodes WHERE node_id=277;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 277;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=277;
/* delete node 278*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (278, 377621810, -1224201081, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 278;
DELETE FROM node_tags WHERE node_id = 278;
DELETE FROM current_way_nodes WHERE node_id=278;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 278;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=278;
/* delete node 279*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (279, 377621974, -1224198881, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 279;
DELETE FROM node_tags WHERE node_id = 279;
DELETE FROM current_way_nodes WHERE node_id=279;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 279;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=279;
/* delete node 280*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (280, 377622907, -1224198901, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 280;
DELETE FROM node_tags WHERE node_id = 280;
DELETE FROM current_way_nodes WHERE node_id=280;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 280;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=280;
/* delete node 281*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (281, 377623384, -1224201181, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 281;
DELETE FROM node_tags WHERE node_id = 281;
DELETE FROM current_way_nodes WHERE node_id=281;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 281;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=281;
/* delete node 282*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (282, 377623522, -1224198982, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 282;
DELETE FROM node_tags WHERE node_id = 282;
DELETE FROM current_way_nodes WHERE node_id=282;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 282;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=282;
/* delete node 283*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (283, 377624752, -1224201356, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 283;
DELETE FROM node_tags WHERE node_id = 283;
DELETE FROM current_way_nodes WHERE node_id=283;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 283;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=283;
/* delete node 284*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (284, 377623925, -1224201262, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 284;
DELETE FROM node_tags WHERE node_id = 284;
DELETE FROM current_way_nodes WHERE node_id=284;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 284;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=284;
/* delete node 285*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (285, 377624058, -1224198955, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 285;
DELETE FROM node_tags WHERE node_id = 285;
DELETE FROM current_way_nodes WHERE node_id=285;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 285;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=285;
/* delete node 286*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (286, 377624890, -1224199009, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 286;
DELETE FROM node_tags WHERE node_id = 286;
DELETE FROM current_way_nodes WHERE node_id=286;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 286;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=286;
/* delete node 287*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (287, 377626369, -1224201570, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 287;
DELETE FROM node_tags WHERE node_id = 287;
DELETE FROM current_way_nodes WHERE node_id=287;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 287;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=287;
/* delete node 288*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (288, 377626491, -1224201423, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 288;
DELETE FROM node_tags WHERE node_id = 288;
DELETE FROM current_way_nodes WHERE node_id=288;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 288;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=288;
/* delete node 289*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (289, 377626576, -1224200511, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 289;
DELETE FROM node_tags WHERE node_id = 289;
DELETE FROM current_way_nodes WHERE node_id=289;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 289;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=289;
/* delete node 290*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (290, 377626517, -1224200464, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 290;
DELETE FROM node_tags WHERE node_id = 290;
DELETE FROM current_way_nodes WHERE node_id=290;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 290;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=290;
/* delete node 291*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (291, 377624815, -1224200275, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 291;
DELETE FROM node_tags WHERE node_id = 291;
DELETE FROM current_way_nodes WHERE node_id=291;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 291;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=291;
/* delete node 292*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (292, 377626544, -1224199491, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 292;
DELETE FROM node_tags WHERE node_id = 292;
DELETE FROM current_way_nodes WHERE node_id=292;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 292;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=292;
/* delete node 293*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (293, 377626162, -1224199444, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 293;
DELETE FROM node_tags WHERE node_id = 293;
DELETE FROM current_way_nodes WHERE node_id=293;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 293;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=293;
/* delete node 294*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (294, 377626130, -1224199498, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 294;
DELETE FROM node_tags WHERE node_id = 294;
DELETE FROM current_way_nodes WHERE node_id=294;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 294;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=294;
/* delete node 295*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (295, 377625372, -1224199431, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 295;
DELETE FROM node_tags WHERE node_id = 295;
DELETE FROM current_way_nodes WHERE node_id=295;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 295;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=295;
/* delete node 296*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (296, 377625372, -1224199377, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 296;
DELETE FROM node_tags WHERE node_id = 296;
DELETE FROM current_way_nodes WHERE node_id=296;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 296;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=296;
/* delete node 297*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (297, 377625033, -1224199290, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 297;
DELETE FROM node_tags WHERE node_id = 297;
DELETE FROM current_way_nodes WHERE node_id=297;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 297;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=297;
/* delete node 298*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (298, 377625107, -1224198593, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 298;
DELETE FROM node_tags WHERE node_id = 298;
DELETE FROM current_way_nodes WHERE node_id=298;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 298;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=298;
/* delete node 299*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (299, 377626607, -1224198680, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 299;
DELETE FROM node_tags WHERE node_id = 299;
DELETE FROM current_way_nodes WHERE node_id=299;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 299;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=299;
/* delete node 300*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (300, 377626729, -1224198774, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 300;
DELETE FROM node_tags WHERE node_id = 300;
DELETE FROM current_way_nodes WHERE node_id=300;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 300;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=300;
/* delete node 301*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (301, 377626708, -1224199035, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 301;
DELETE FROM node_tags WHERE node_id = 301;
DELETE FROM current_way_nodes WHERE node_id=301;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 301;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=301;
/* delete node 302*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (302, 377626613, -1224199062, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 302;
DELETE FROM node_tags WHERE node_id = 302;
DELETE FROM current_way_nodes WHERE node_id=302;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 302;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=302;
/* delete node 303*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (303, 377626692, -1224199156, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 303;
DELETE FROM node_tags WHERE node_id = 303;
DELETE FROM current_way_nodes WHERE node_id=303;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 303;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=303;
/* delete node 304*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (304, 377626676, -1224199391, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 304;
DELETE FROM node_tags WHERE node_id = 304;
DELETE FROM current_way_nodes WHERE node_id=304;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 304;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=304;
/* delete node 305*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (305, 377626628, -1224200374, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 305;
DELETE FROM node_tags WHERE node_id = 305;
DELETE FROM current_way_nodes WHERE node_id=305;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 305;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=305;
/* delete node 306*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (306, 377626636, -1224200139, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 306;
DELETE FROM node_tags WHERE node_id = 306;
DELETE FROM current_way_nodes WHERE node_id=306;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 306;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=306;
/* delete node 307*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (307, 377626565, -1224200054, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 307;
DELETE FROM node_tags WHERE node_id = 307;
DELETE FROM current_way_nodes WHERE node_id=307;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 307;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=307;
/* delete node 308*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (308, 377626653, -1224199985, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 308;
DELETE FROM node_tags WHERE node_id = 308;
DELETE FROM current_way_nodes WHERE node_id=308;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 308;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=308;
/* delete node 309*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (309, 377626657, -1224199740, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 309;
DELETE FROM node_tags WHERE node_id = 309;
DELETE FROM current_way_nodes WHERE node_id=309;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 309;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=309;
/* delete node 310*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (310, 377626594, -1224199665, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 310;
DELETE FROM node_tags WHERE node_id = 310;
DELETE FROM current_way_nodes WHERE node_id=310;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 310;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=310;
/* delete node 311*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (311, 377626173, -1224199622, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 311;
DELETE FROM node_tags WHERE node_id = 311;
DELETE FROM current_way_nodes WHERE node_id=311;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 311;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=311;
/* delete node 312*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (312, 377625225, -1224199511, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 312;
DELETE FROM node_tags WHERE node_id = 312;
DELETE FROM current_way_nodes WHERE node_id=312;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 312;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=312;
/* delete node 313*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (313, 377624943, -1224199511, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 313;
DELETE FROM node_tags WHERE node_id = 313;
DELETE FROM current_way_nodes WHERE node_id=313;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 313;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=313;
/* delete node 314*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (314, 377624904, -1224200285, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 314;
DELETE FROM node_tags WHERE node_id = 314;
DELETE FROM current_way_nodes WHERE node_id=314;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 314;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=314;
/* delete node 315*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (315, 377626968, -1224201411, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 315;
DELETE FROM node_tags WHERE node_id = 315;
DELETE FROM current_way_nodes WHERE node_id=315;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 315;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=315;
/* delete node 316*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (316, 377628615, -1224201571, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 316;
DELETE FROM node_tags WHERE node_id = 316;
DELETE FROM current_way_nodes WHERE node_id=316;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 316;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=316;
/* delete node 317*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (317, 377628628, -1224201443, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 317;
DELETE FROM node_tags WHERE node_id = 317;
DELETE FROM current_way_nodes WHERE node_id=317;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 317;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=317;
/* delete node 318*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (318, 377628522, -1224201433, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 318;
DELETE FROM node_tags WHERE node_id = 318;
DELETE FROM current_way_nodes WHERE node_id=318;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 318;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=318;
/* delete node 319*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (319, 377628539, -1224201102, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 319;
DELETE FROM node_tags WHERE node_id = 319;
DELETE FROM current_way_nodes WHERE node_id=319;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 319;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=319;
/* delete node 320*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (320, 377628316, -1224201071, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 320;
DELETE FROM node_tags WHERE node_id = 320;
DELETE FROM current_way_nodes WHERE node_id=320;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 320;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=320;
/* delete node 321*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (321, 377628346, -1224200660, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 321;
DELETE FROM node_tags WHERE node_id = 321;
DELETE FROM current_way_nodes WHERE node_id=321;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 321;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=321;
/* delete node 322*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (322, 377627040, -1224200522, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 322;
DELETE FROM node_tags WHERE node_id = 322;
DELETE FROM current_way_nodes WHERE node_id=322;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 322;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=322;
/* delete node 323*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (323, 377626909, -1224202285, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 323;
DELETE FROM node_tags WHERE node_id = 323;
DELETE FROM current_way_nodes WHERE node_id=323;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 323;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=323;
/* delete node 324*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (324, 377628548, -1224202450, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 324;
DELETE FROM node_tags WHERE node_id = 324;
DELETE FROM current_way_nodes WHERE node_id=324;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 324;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=324;
/* delete node 325*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (325, 377628535, -1224203361, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 325;
DELETE FROM node_tags WHERE node_id = 325;
DELETE FROM current_way_nodes WHERE node_id=325;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 325;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=325;
/* delete node 326*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (326, 377626978, -1224202292, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 326;
DELETE FROM node_tags WHERE node_id = 326;
DELETE FROM current_way_nodes WHERE node_id=326;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 326;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=326;
/* delete node 327*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (327, 377628645, -1224202482, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 327;
DELETE FROM node_tags WHERE node_id = 327;
DELETE FROM current_way_nodes WHERE node_id=327;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 327;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=327;
/* delete node 328*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (328, 377628577, -1224203239, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 328;
DELETE FROM node_tags WHERE node_id = 328;
DELETE FROM current_way_nodes WHERE node_id=328;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 328;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=328;
/* delete node 388*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (388, 377647024, -1224217173, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 388;
DELETE FROM node_tags WHERE node_id = 388;
DELETE FROM current_way_nodes WHERE node_id=388;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 388;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=388;
/* delete node 445*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (445, 377647530, -1224221129, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 445;
DELETE FROM node_tags WHERE node_id = 445;
DELETE FROM current_way_nodes WHERE node_id=445;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 445;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=445;
/* delete node 446*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (446, 377618153, -1224237098, 3, false, (now() at time zone 'utc'), 1301409554, 2);
DELETE FROM current_node_tags WHERE node_id = 446;
DELETE FROM node_tags WHERE node_id = 446;
DELETE FROM current_way_nodes WHERE node_id=446;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 446;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=446;
/* delete node 447*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (447, 377627740, -1224196291, 3, false, (now() at time zone 'utc'), 1301409560, 2);
DELETE FROM current_node_tags WHERE node_id = 447;
DELETE FROM node_tags WHERE node_id = 447;
DELETE FROM current_way_nodes WHERE node_id=447;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 447;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=447;
/* delete node 486*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (486, 377634905, -1224200277, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 486;
DELETE FROM node_tags WHERE node_id = 486;
DELETE FROM current_way_nodes WHERE node_id=486;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 486;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=486;
/* delete node 487*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (487, 377634965, -1224199292, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 487;
DELETE FROM node_tags WHERE node_id = 487;
DELETE FROM current_way_nodes WHERE node_id=487;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 487;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=487;
/* delete node 488*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (488, 377635606, -1224199355, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 488;
DELETE FROM node_tags WHERE node_id = 488;
DELETE FROM current_way_nodes WHERE node_id=488;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 488;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=488;
/* delete node 489*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (489, 377636560, -1224200439, 3, false, (now() at time zone 'utc'), 1301409561, 2);
DELETE FROM current_node_tags WHERE node_id = 489;
DELETE FROM node_tags WHERE node_id = 489;
DELETE FROM current_way_nodes WHERE node_id=489;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 489;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=489;
/* create node 506*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (506, 377626807, -1224219696, 3, true, (now() at time zone 'utc'), 1301409560, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (506, 377626807, -1224219696, 3, true, (now() at time zone 'utc'), 1301409560, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (506, 'REF2', '007037');
INSERT INTO node_tags (node_id, k, v, version) VALUES (506, 'REF2', '007037', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (506, 'hoot:id', '506');
INSERT INTO node_tags (node_id, k, v, version) VALUES (506, 'hoot:id', '506', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (506, 'hoot:status', '2');
INSERT INTO node_tags (node_id, k, v, version) VALUES (506, 'hoot:status', '2', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (506, 'name', 'MISSION POLICE STATION');
INSERT INTO node_tags (node_id, k, v, version) VALUES (506, 'name', 'MISSION POLICE STATION', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (506, 'amenity', 'police');
INSERT INTO node_tags (node_id, k, v, version) VALUES (506, 'amenity', 'police', 1);
/* delete way 3*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (3, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 3;
DELETE FROM way_tags WHERE way_id = 3;
DELETE FROM current_way_nodes WHERE way_id=3;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 3;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=3;
/* delete way 4*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (4, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 4;
DELETE FROM way_tags WHERE way_id = 4;
DELETE FROM current_way_nodes WHERE way_id=4;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 4;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=4;
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
/* delete way 14*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (14, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 14;
DELETE FROM way_tags WHERE way_id = 14;
DELETE FROM current_way_nodes WHERE way_id=14;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 14;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=14;
/* delete way 15*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (15, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 15;
DELETE FROM way_tags WHERE way_id = 15;
DELETE FROM current_way_nodes WHERE way_id=15;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 15;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=15;
/* modify way 16*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (16, 3, true, (now() at time zone 'utc'), 2);
UPDATE current_ways SET changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), version=2 WHERE id=16;
DELETE FROM current_way_tags WHERE way_id = 16;
DELETE FROM way_tags WHERE way_id = 16;
INSERT INTO current_way_tags (way_id, k, v) VALUES (16, 'REF1', '009232');
INSERT INTO way_tags (way_id, k, v, version) VALUES (16, 'REF1', '009232', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (16, 'REF2', 'none');
INSERT INTO way_tags (way_id, k, v, version) VALUES (16, 'REF2', 'none', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (16, 'created_by', 'Potlatch 0.9c');
INSERT INTO way_tags (way_id, k, v, version) VALUES (16, 'created_by', 'Potlatch 0.9c', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (16, 'hoot:id', '16');
INSERT INTO way_tags (way_id, k, v, version) VALUES (16, 'hoot:id', '16', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (16, 'leisure', 'park');
INSERT INTO way_tags (way_id, k, v, version) VALUES (16, 'leisure', 'park', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (16, 'hoot:status', '3');
INSERT INTO way_tags (way_id, k, v, version) VALUES (16, 'hoot:status', '3', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (16, 'alt_name', 'Kidpower Park');
INSERT INTO way_tags (way_id, k, v, version) VALUES (16, 'alt_name', 'Kidpower Park', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (16, 'area', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (16, 'area', 'yes', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (16, 'name', 'Kids Power Park');
INSERT INTO way_tags (way_id, k, v, version) VALUES (16, 'name', 'Kids Power Park', 2);
DELETE FROM current_way_nodes WHERE way_id = 16;
DELETE FROM way_nodes WHERE way_id = 16;
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (16, 38, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (16, 38, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (16, 39, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (16, 39, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (16, 40, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (16, 40, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (16, 41, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (16, 41, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (16, 38, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (16, 38, 5);
/* modify way 17*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (17, 3, true, (now() at time zone 'utc'), 2);
UPDATE current_ways SET changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), version=2 WHERE id=17;
DELETE FROM current_way_tags WHERE way_id = 17;
DELETE FROM way_tags WHERE way_id = 17;
INSERT INTO current_way_tags (way_id, k, v) VALUES (17, 'REF1', '00435f');
INSERT INTO way_tags (way_id, k, v, version) VALUES (17, 'REF1', '00435f', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (17, 'REF2', '00435f');
INSERT INTO way_tags (way_id, k, v, version) VALUES (17, 'REF2', '00435f', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (17, 'hoot:poipolygon:poismerged', '1');
INSERT INTO way_tags (way_id, k, v, version) VALUES (17, 'hoot:poipolygon:poismerged', '1', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (17, 'hoot:id', '17');
INSERT INTO way_tags (way_id, k, v, version) VALUES (17, 'hoot:id', '17', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (17, 'hoot:status', '3');
INSERT INTO way_tags (way_id, k, v, version) VALUES (17, 'hoot:status', '3', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (17, 'alt_name', '16TH & HOFF ST. PARKING GARAGE');
INSERT INTO way_tags (way_id, k, v, version) VALUES (17, 'alt_name', '16TH & HOFF ST. PARKING GARAGE', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (17, 'name', 'Hoff Street Garage');
INSERT INTO way_tags (way_id, k, v, version) VALUES (17, 'name', 'Hoff Street Garage', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (17, 'amenity', 'parking');
INSERT INTO way_tags (way_id, k, v, version) VALUES (17, 'amenity', 'parking', 2);
DELETE FROM current_way_nodes WHERE way_id = 17;
DELETE FROM way_nodes WHERE way_id = 17;
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (17, 42, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (17, 42, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (17, 43, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (17, 43, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (17, 87, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (17, 87, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (17, 44, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (17, 44, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (17, 45, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (17, 45, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (17, 42, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (17, 42, 6);
/* modify way 18*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (18, 3, true, (now() at time zone 'utc'), 2);
UPDATE current_ways SET changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), version=2 WHERE id=18;
DELETE FROM current_way_tags WHERE way_id = 18;
DELETE FROM way_tags WHERE way_id = 18;
INSERT INTO current_way_tags (way_id, k, v) VALUES (18, 'REF1', '00439d');
INSERT INTO way_tags (way_id, k, v, version) VALUES (18, 'REF1', '00439d', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (18, 'REF2', 'none');
INSERT INTO way_tags (way_id, k, v, version) VALUES (18, 'REF2', 'none', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (18, 'hoot:id', '18');
INSERT INTO way_tags (way_id, k, v, version) VALUES (18, 'hoot:id', '18', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (18, 'leisure', 'garden');
INSERT INTO way_tags (way_id, k, v, version) VALUES (18, 'leisure', 'garden', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (18, 'hoot:status', '3');
INSERT INTO way_tags (way_id, k, v, version) VALUES (18, 'hoot:status', '3', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (18, 'area', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (18, 'area', 'yes', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (18, 'name', 'Dearborn Community Garden');
INSERT INTO way_tags (way_id, k, v, version) VALUES (18, 'name', 'Dearborn Community Garden', 2);
DELETE FROM current_way_nodes WHERE way_id = 18;
DELETE FROM way_nodes WHERE way_id = 18;
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (18, 50, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (18, 50, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (18, 51, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (18, 51, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (18, 52, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (18, 52, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (18, 53, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (18, 53, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (18, 50, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (18, 50, 5);
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
/* delete way 21*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (21, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 21;
DELETE FROM way_tags WHERE way_id = 21;
DELETE FROM current_way_nodes WHERE way_id=21;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 21;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=21;
/* delete way 22*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (22, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 22;
DELETE FROM way_tags WHERE way_id = 22;
DELETE FROM current_way_nodes WHERE way_id=22;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 22;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=22;
/* delete way 23*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (23, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 23;
DELETE FROM way_tags WHERE way_id = 23;
DELETE FROM current_way_nodes WHERE way_id=23;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 23;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=23;
/* delete way 24*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (24, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 24;
DELETE FROM way_tags WHERE way_id = 24;
DELETE FROM current_way_nodes WHERE way_id=24;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 24;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=24;
/* delete way 25*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (25, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 25;
DELETE FROM way_tags WHERE way_id = 25;
DELETE FROM current_way_nodes WHERE way_id=25;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 25;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=25;
/* delete way 26*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (26, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 26;
DELETE FROM way_tags WHERE way_id = 26;
DELETE FROM current_way_nodes WHERE way_id=26;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 26;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=26;
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
/* delete way 35*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (35, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 35;
DELETE FROM way_tags WHERE way_id = 35;
DELETE FROM current_way_nodes WHERE way_id=35;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 35;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=35;
/* delete way 36*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (36, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 36;
DELETE FROM way_tags WHERE way_id = 36;
DELETE FROM current_way_nodes WHERE way_id=36;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 36;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=36;
/* delete way 37*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (37, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 37;
DELETE FROM way_tags WHERE way_id = 37;
DELETE FROM current_way_nodes WHERE way_id=37;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 37;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=37;
/* delete way 38*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (38, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 38;
DELETE FROM way_tags WHERE way_id = 38;
DELETE FROM current_way_nodes WHERE way_id=38;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 38;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=38;
/* delete way 39*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (39, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 39;
DELETE FROM way_tags WHERE way_id = 39;
DELETE FROM current_way_nodes WHERE way_id=39;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 39;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=39;
/* delete way 40*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (40, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 40;
DELETE FROM way_tags WHERE way_id = 40;
DELETE FROM current_way_nodes WHERE way_id=40;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 40;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=40;
/* delete way 41*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (41, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 41;
DELETE FROM way_tags WHERE way_id = 41;
DELETE FROM current_way_nodes WHERE way_id=41;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 41;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=41;
/* delete way 42*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (42, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 42;
DELETE FROM way_tags WHERE way_id = 42;
DELETE FROM current_way_nodes WHERE way_id=42;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 42;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=42;
/* delete way 43*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (43, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 43;
DELETE FROM way_tags WHERE way_id = 43;
DELETE FROM current_way_nodes WHERE way_id=43;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 43;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=43;
/* delete way 44*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (44, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 44;
DELETE FROM way_tags WHERE way_id = 44;
DELETE FROM current_way_nodes WHERE way_id=44;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 44;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=44;
/* delete way 45*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (45, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 45;
DELETE FROM way_tags WHERE way_id = 45;
DELETE FROM current_way_nodes WHERE way_id=45;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 45;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=45;
/* delete way 46*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (46, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 46;
DELETE FROM way_tags WHERE way_id = 46;
DELETE FROM current_way_nodes WHERE way_id=46;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 46;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=46;
/* delete way 48*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (48, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 48;
DELETE FROM way_tags WHERE way_id = 48;
DELETE FROM current_way_nodes WHERE way_id=48;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 48;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=48;
/* delete way 49*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (49, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 49;
DELETE FROM way_tags WHERE way_id = 49;
DELETE FROM current_way_nodes WHERE way_id=49;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 49;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=49;
/* delete way 52*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (52, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 52;
DELETE FROM way_tags WHERE way_id = 52;
DELETE FROM current_way_nodes WHERE way_id=52;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 52;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=52;
/* delete way 53*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (53, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 53;
DELETE FROM way_tags WHERE way_id = 53;
DELETE FROM current_way_nodes WHERE way_id=53;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 53;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=53;
/* delete way 54*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (54, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 54;
DELETE FROM way_tags WHERE way_id = 54;
DELETE FROM current_way_nodes WHERE way_id=54;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 54;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=54;
/* delete way 55*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (55, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 55;
DELETE FROM way_tags WHERE way_id = 55;
DELETE FROM current_way_nodes WHERE way_id=55;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 55;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=55;
/* delete way 56*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (56, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 56;
DELETE FROM way_tags WHERE way_id = 56;
DELETE FROM current_way_nodes WHERE way_id=56;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 56;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=56;
/* delete way 57*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (57, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 57;
DELETE FROM way_tags WHERE way_id = 57;
DELETE FROM current_way_nodes WHERE way_id=57;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 57;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=57;
/* delete way 58*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (58, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 58;
DELETE FROM way_tags WHERE way_id = 58;
DELETE FROM current_way_nodes WHERE way_id=58;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 58;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=58;
/* delete way 59*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (59, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 59;
DELETE FROM way_tags WHERE way_id = 59;
DELETE FROM current_way_nodes WHERE way_id=59;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 59;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=59;
/* delete way 60*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (60, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 60;
DELETE FROM way_tags WHERE way_id = 60;
DELETE FROM current_way_nodes WHERE way_id=60;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 60;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=60;
/* delete way 61*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (61, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 61;
DELETE FROM way_tags WHERE way_id = 61;
DELETE FROM current_way_nodes WHERE way_id=61;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 61;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=61;
/* delete way 62*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (62, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 62;
DELETE FROM way_tags WHERE way_id = 62;
DELETE FROM current_way_nodes WHERE way_id=62;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 62;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=62;
UPDATE changesets SET min_lat=377618000, max_lat=377651000, min_lon=-1224239735, max_lon=-1224195999, num_changes=322 WHERE id=3;
