INSERT INTO changesets (id, user_id, created_at, closed_at) VALUES (3, 1, (now() at time zone 'utc'), (now() at time zone 'utc'));
INSERT INTO changeset_tags (changeset_id, k, v) VALUES (3, 'written_by', 'Hootenanny');
/* delete node 2*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2, 424847644, -714758463, 3, false, (now() at time zone 'utc'), 1710365925, 2);
DELETE FROM current_node_tags WHERE node_id = 2;
DELETE FROM node_tags WHERE node_id = 2;
DELETE FROM current_way_nodes WHERE node_id=2;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 2;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=2;
/* delete node 3*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3, 424846295, -714758307, 3, false, (now() at time zone 'utc'), 1710365925, 2);
DELETE FROM current_node_tags WHERE node_id = 3;
DELETE FROM node_tags WHERE node_id = 3;
DELETE FROM current_way_nodes WHERE node_id=3;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 3;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=3;
/* delete node 4*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (4, 424846305, -714758155, 3, false, (now() at time zone 'utc'), 1710365925, 2);
DELETE FROM current_node_tags WHERE node_id = 4;
DELETE FROM node_tags WHERE node_id = 4;
DELETE FROM current_way_nodes WHERE node_id=4;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 4;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=4;
/* delete node 5*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (5, 424845982, -714758118, 3, false, (now() at time zone 'utc'), 1710365925, 2);
DELETE FROM current_node_tags WHERE node_id = 5;
DELETE FROM node_tags WHERE node_id = 5;
DELETE FROM current_way_nodes WHERE node_id=5;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 5;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=5;
/* delete node 6*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (6, 424845968, -714758335, 3, false, (now() at time zone 'utc'), 1710365925, 2);
DELETE FROM current_node_tags WHERE node_id = 6;
DELETE FROM node_tags WHERE node_id = 6;
DELETE FROM current_way_nodes WHERE node_id=6;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 6;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=6;
/* delete node 7*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7, 424845529, -714758284, 3, false, (now() at time zone 'utc'), 1710365925, 2);
DELETE FROM current_node_tags WHERE node_id = 7;
DELETE FROM node_tags WHERE node_id = 7;
DELETE FROM current_way_nodes WHERE node_id=7;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 7;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=7;
/* delete node 8*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (8, 424845587, -714757368, 3, false, (now() at time zone 'utc'), 1710365925, 2);
DELETE FROM current_node_tags WHERE node_id = 8;
DELETE FROM node_tags WHERE node_id = 8;
DELETE FROM current_way_nodes WHERE node_id=8;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 8;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=8;
/* delete node 9*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (9, 424847244, -714757559, 3, false, (now() at time zone 'utc'), 1710365925, 2);
DELETE FROM current_node_tags WHERE node_id = 9;
DELETE FROM node_tags WHERE node_id = 9;
DELETE FROM current_way_nodes WHERE node_id=9;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 9;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=9;
/* delete node 10*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (10, 424845743, -714764294, 3, false, (now() at time zone 'utc'), 1710365925, 2);
DELETE FROM current_node_tags WHERE node_id = 10;
DELETE FROM node_tags WHERE node_id = 10;
DELETE FROM current_way_nodes WHERE node_id=10;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 10;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=10;
/* delete node 11*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11, 424847269, -714757158, 3, false, (now() at time zone 'utc'), 1710365925, 2);
DELETE FROM current_node_tags WHERE node_id = 11;
DELETE FROM node_tags WHERE node_id = 11;
DELETE FROM current_way_nodes WHERE node_id=11;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=11;
/* delete node 12*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12, 424845877, -714763328, 3, false, (now() at time zone 'utc'), 1710365925, 2);
DELETE FROM current_node_tags WHERE node_id = 12;
DELETE FROM node_tags WHERE node_id = 12;
DELETE FROM current_way_nodes WHERE node_id=12;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=12;
/* delete node 13*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13, 424847600, -714757196, 3, false, (now() at time zone 'utc'), 1710365925, 2);
DELETE FROM current_node_tags WHERE node_id = 13;
DELETE FROM node_tags WHERE node_id = 13;
DELETE FROM current_way_nodes WHERE node_id=13;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=13;
/* delete node 14*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (14, 424847371, -714763705, 3, false, (now() at time zone 'utc'), 1710365925, 2);
DELETE FROM current_node_tags WHERE node_id = 14;
DELETE FROM node_tags WHERE node_id = 14;
DELETE FROM current_way_nodes WHERE node_id=14;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 14;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=14;
/* delete node 15*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (15, 424847587, -714757401, 3, false, (now() at time zone 'utc'), 1710365925, 2);
DELETE FROM current_node_tags WHERE node_id = 15;
DELETE FROM node_tags WHERE node_id = 15;
DELETE FROM current_way_nodes WHERE node_id=15;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 15;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=15;
/* delete node 16*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (16, 424847185, -714765050, 3, false, (now() at time zone 'utc'), 1710365925, 2);
DELETE FROM current_node_tags WHERE node_id = 16;
DELETE FROM node_tags WHERE node_id = 16;
DELETE FROM current_way_nodes WHERE node_id=16;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 16;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=16;
/* delete node 17*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (17, 424847710, -714757415, 3, false, (now() at time zone 'utc'), 1710365925, 2);
DELETE FROM current_node_tags WHERE node_id = 17;
DELETE FROM node_tags WHERE node_id = 17;
DELETE FROM current_way_nodes WHERE node_id=17;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 17;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=17;
/* delete node 18*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (18, 424845723, -714764681, 3, false, (now() at time zone 'utc'), 1710365925, 2);
DELETE FROM current_node_tags WHERE node_id = 18;
DELETE FROM node_tags WHERE node_id = 18;
DELETE FROM current_way_nodes WHERE node_id=18;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 18;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=18;
/* delete node 19*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (19, 424845703, -714764826, 3, false, (now() at time zone 'utc'), 1710365925, 2);
DELETE FROM current_node_tags WHERE node_id = 19;
DELETE FROM node_tags WHERE node_id = 19;
DELETE FROM current_way_nodes WHERE node_id=19;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 19;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=19;
/* delete node 20*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (20, 424845334, -714764732, 3, false, (now() at time zone 'utc'), 1710365925, 2);
DELETE FROM current_node_tags WHERE node_id = 20;
DELETE FROM node_tags WHERE node_id = 20;
DELETE FROM current_way_nodes WHERE node_id=20;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 20;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=20;
/* delete node 21*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (21, 424845406, -714764209, 3, false, (now() at time zone 'utc'), 1710365925, 2);
DELETE FROM current_node_tags WHERE node_id = 21;
DELETE FROM node_tags WHERE node_id = 21;
DELETE FROM current_way_nodes WHERE node_id=21;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 21;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=21;
/* delete node 22*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (22, 424842139, -714769582, 3, false, (now() at time zone 'utc'), 1710365925, 2);
DELETE FROM current_node_tags WHERE node_id = 22;
DELETE FROM node_tags WHERE node_id = 22;
DELETE FROM current_way_nodes WHERE node_id=22;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 22;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=22;
/* delete node 27*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (27, 424850760, -714758523, 3, false, (now() at time zone 'utc'), 1710365936, 2);
DELETE FROM current_node_tags WHERE node_id = 27;
DELETE FROM node_tags WHERE node_id = 27;
DELETE FROM current_way_nodes WHERE node_id=27;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 27;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=27;
/* delete node 28*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (28, 424850429, -714757816, 3, false, (now() at time zone 'utc'), 1710365936, 2);
DELETE FROM current_node_tags WHERE node_id = 28;
DELETE FROM node_tags WHERE node_id = 28;
DELETE FROM current_way_nodes WHERE node_id=28;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 28;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=28;
/* delete node 29*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (29, 424850332, -714757876, 3, false, (now() at time zone 'utc'), 1710365936, 2);
DELETE FROM current_node_tags WHERE node_id = 29;
DELETE FROM node_tags WHERE node_id = 29;
DELETE FROM current_way_nodes WHERE node_id=29;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 29;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=29;
/* delete node 30*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (30, 424850214, -714757612, 3, false, (now() at time zone 'utc'), 1710365936, 2);
DELETE FROM current_node_tags WHERE node_id = 30;
DELETE FROM node_tags WHERE node_id = 30;
DELETE FROM current_way_nodes WHERE node_id=30;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 30;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=30;
/* delete node 31*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (31, 424850097, -714758569, 3, false, (now() at time zone 'utc'), 1710365936, 2);
DELETE FROM current_node_tags WHERE node_id = 31;
DELETE FROM node_tags WHERE node_id = 31;
DELETE FROM current_way_nodes WHERE node_id=31;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 31;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=31;
/* delete node 32*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (32, 424849643, -714758468, 3, false, (now() at time zone 'utc'), 1710365936, 2);
DELETE FROM current_node_tags WHERE node_id = 32;
DELETE FROM node_tags WHERE node_id = 32;
DELETE FROM current_way_nodes WHERE node_id=32;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 32;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=32;
/* delete node 33*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (33, 424849659, -714758334, 3, false, (now() at time zone 'utc'), 1710365936, 2);
DELETE FROM current_node_tags WHERE node_id = 33;
DELETE FROM node_tags WHERE node_id = 33;
DELETE FROM current_way_nodes WHERE node_id=33;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 33;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=33;
/* delete node 34*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (34, 424848649, -714758109, 3, false, (now() at time zone 'utc'), 1710365936, 2);
DELETE FROM current_node_tags WHERE node_id = 34;
DELETE FROM node_tags WHERE node_id = 34;
DELETE FROM current_way_nodes WHERE node_id=34;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 34;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=34;
/* delete node 35*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (35, 424848747, -714757246, 3, false, (now() at time zone 'utc'), 1710365936, 2);
DELETE FROM current_node_tags WHERE node_id = 35;
DELETE FROM node_tags WHERE node_id = 35;
DELETE FROM current_way_nodes WHERE node_id=35;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 35;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=35;
/* delete node 36*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (36, 424848930, -714757280, 3, false, (now() at time zone 'utc'), 1710365936, 2);
DELETE FROM current_node_tags WHERE node_id = 36;
DELETE FROM node_tags WHERE node_id = 36;
DELETE FROM current_way_nodes WHERE node_id=36;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 36;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=36;
/* delete node 37*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (37, 424848950, -714756973, 3, false, (now() at time zone 'utc'), 1710365936, 2);
DELETE FROM current_node_tags WHERE node_id = 37;
DELETE FROM node_tags WHERE node_id = 37;
DELETE FROM current_way_nodes WHERE node_id=37;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 37;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=37;
/* delete node 38*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (38, 424849220, -714757013, 3, false, (now() at time zone 'utc'), 1710365936, 2);
DELETE FROM current_node_tags WHERE node_id = 38;
DELETE FROM node_tags WHERE node_id = 38;
DELETE FROM current_way_nodes WHERE node_id=38;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 38;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=38;
/* delete node 39*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (39, 424849195, -714757325, 3, false, (now() at time zone 'utc'), 1710365936, 2);
DELETE FROM current_node_tags WHERE node_id = 39;
DELETE FROM node_tags WHERE node_id = 39;
DELETE FROM current_way_nodes WHERE node_id=39;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 39;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=39;
/* delete node 40*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (40, 424849935, -714757434, 3, false, (now() at time zone 'utc'), 1710365936, 2);
DELETE FROM current_node_tags WHERE node_id = 40;
DELETE FROM node_tags WHERE node_id = 40;
DELETE FROM current_way_nodes WHERE node_id=40;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 40;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=40;
/* delete node 41*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (41, 424843744, -714758641, 3, false, (now() at time zone 'utc'), 1710365925, 2);
DELETE FROM current_node_tags WHERE node_id = 41;
DELETE FROM node_tags WHERE node_id = 41;
DELETE FROM current_way_nodes WHERE node_id=41;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 41;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=41;
/* delete node 42*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (42, 424849974, -714756953, 3, false, (now() at time zone 'utc'), 1710365936, 2);
DELETE FROM current_node_tags WHERE node_id = 42;
DELETE FROM node_tags WHERE node_id = 42;
DELETE FROM current_way_nodes WHERE node_id=42;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 42;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=42;
/* delete node 43*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (43, 424843095, -714765214, 3, false, (now() at time zone 'utc'), 1710365925, 2);
DELETE FROM current_node_tags WHERE node_id = 43;
DELETE FROM node_tags WHERE node_id = 43;
DELETE FROM current_way_nodes WHERE node_id=43;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 43;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=43;
/* delete node 44*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (44, 424842746, -714758608, 3, false, (now() at time zone 'utc'), 1710365925, 2);
DELETE FROM current_node_tags WHERE node_id = 44;
DELETE FROM node_tags WHERE node_id = 44;
DELETE FROM current_way_nodes WHERE node_id=44;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 44;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=44;
/* delete node 45*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (45, 424850323, -714757005, 3, false, (now() at time zone 'utc'), 1710365936, 2);
DELETE FROM current_node_tags WHERE node_id = 45;
DELETE FROM node_tags WHERE node_id = 45;
DELETE FROM current_way_nodes WHERE node_id=45;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 45;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=45;
/* delete node 46*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (46, 424842495, -714765044, 3, false, (now() at time zone 'utc'), 1710365925, 2);
DELETE FROM current_node_tags WHERE node_id = 46;
DELETE FROM node_tags WHERE node_id = 46;
DELETE FROM current_way_nodes WHERE node_id=46;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 46;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=46;
/* delete node 47*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (47, 424842762, -714757700, 3, false, (now() at time zone 'utc'), 1710365925, 2);
DELETE FROM current_node_tags WHERE node_id = 47;
DELETE FROM node_tags WHERE node_id = 47;
DELETE FROM current_way_nodes WHERE node_id=47;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 47;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=47;
/* delete node 48*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (48, 424850304, -714757230, 3, false, (now() at time zone 'utc'), 1710365936, 2);
DELETE FROM current_node_tags WHERE node_id = 48;
DELETE FROM node_tags WHERE node_id = 48;
DELETE FROM current_way_nodes WHERE node_id=48;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 48;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=48;
/* delete node 49*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (49, 424842593, -714764408, 3, false, (now() at time zone 'utc'), 1710365925, 2);
DELETE FROM current_node_tags WHERE node_id = 49;
DELETE FROM node_tags WHERE node_id = 49;
DELETE FROM current_way_nodes WHERE node_id=49;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 49;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=49;
/* delete node 50*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (50, 424842544, -714757693, 3, false, (now() at time zone 'utc'), 1710365925, 2);
DELETE FROM current_node_tags WHERE node_id = 50;
DELETE FROM node_tags WHERE node_id = 50;
DELETE FROM current_way_nodes WHERE node_id=50;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 50;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=50;
/* delete node 51*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (51, 424850680, -714757285, 3, false, (now() at time zone 'utc'), 1710365936, 2);
DELETE FROM current_node_tags WHERE node_id = 51;
DELETE FROM node_tags WHERE node_id = 51;
DELETE FROM current_way_nodes WHERE node_id=51;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 51;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=51;
/* delete node 52*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (52, 424842288, -714764322, 3, false, (now() at time zone 'utc'), 1710365925, 2);
DELETE FROM current_node_tags WHERE node_id = 52;
DELETE FROM node_tags WHERE node_id = 52;
DELETE FROM current_way_nodes WHERE node_id=52;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 52;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=52;
/* delete node 53*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (53, 424842556, -714757047, 3, false, (now() at time zone 'utc'), 1710365925, 2);
DELETE FROM current_node_tags WHERE node_id = 53;
DELETE FROM node_tags WHERE node_id = 53;
DELETE FROM current_way_nodes WHERE node_id=53;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 53;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=53;
/* delete node 54*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (54, 424842485, -714763046, 3, false, (now() at time zone 'utc'), 1710365925, 2);
DELETE FROM current_node_tags WHERE node_id = 54;
DELETE FROM node_tags WHERE node_id = 54;
DELETE FROM current_way_nodes WHERE node_id=54;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 54;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=54;
/* delete node 55*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (55, 424843772, -714757087, 3, false, (now() at time zone 'utc'), 1710365925, 2);
DELETE FROM current_node_tags WHERE node_id = 55;
DELETE FROM node_tags WHERE node_id = 55;
DELETE FROM current_way_nodes WHERE node_id=55;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 55;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=55;
/* delete node 56*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (56, 424843190, -714763245, 3, false, (now() at time zone 'utc'), 1710365925, 2);
DELETE FROM current_node_tags WHERE node_id = 56;
DELETE FROM node_tags WHERE node_id = 56;
DELETE FROM current_way_nodes WHERE node_id=56;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 56;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=56;
/* delete node 57*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (57, 424842994, -714764519, 3, false, (now() at time zone 'utc'), 1710365925, 2);
DELETE FROM current_node_tags WHERE node_id = 57;
DELETE FROM node_tags WHERE node_id = 57;
DELETE FROM current_way_nodes WHERE node_id=57;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 57;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=57;
/* delete node 59*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (59, 424843194, -714764575, 3, false, (now() at time zone 'utc'), 1710365925, 2);
DELETE FROM current_node_tags WHERE node_id = 59;
DELETE FROM node_tags WHERE node_id = 59;
DELETE FROM current_way_nodes WHERE node_id=59;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 59;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=59;
/* delete node 75*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (75, 424843094, -714765444, 3, false, (now() at time zone 'utc'), 1710365925, 2);
DELETE FROM current_node_tags WHERE node_id = 75;
DELETE FROM node_tags WHERE node_id = 75;
DELETE FROM current_way_nodes WHERE node_id=75;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 75;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=75;
/* delete node 76*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (76, 424842946, -714766350, 3, false, (now() at time zone 'utc'), 1710365925, 2);
DELETE FROM current_node_tags WHERE node_id = 76;
DELETE FROM node_tags WHERE node_id = 76;
DELETE FROM current_way_nodes WHERE node_id=76;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 76;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=76;
/* delete node 77*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (77, 424842226, -714766135, 3, false, (now() at time zone 'utc'), 1710365925, 2);
DELETE FROM current_node_tags WHERE node_id = 77;
DELETE FROM node_tags WHERE node_id = 77;
DELETE FROM current_way_nodes WHERE node_id=77;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 77;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=77;
/* delete node 78*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (78, 424842374, -714765230, 3, false, (now() at time zone 'utc'), 1710365925, 2);
DELETE FROM current_node_tags WHERE node_id = 78;
DELETE FROM node_tags WHERE node_id = 78;
DELETE FROM current_way_nodes WHERE node_id=78;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 78;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=78;
/* delete node 79*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (79, 424834037, -714769809, 3, false, (now() at time zone 'utc'), 1710365925, 2);
DELETE FROM current_node_tags WHERE node_id = 79;
DELETE FROM node_tags WHERE node_id = 79;
DELETE FROM current_way_nodes WHERE node_id=79;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 79;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=79;
/* delete node 80*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (80, 424834106, -714769331, 3, false, (now() at time zone 'utc'), 1710365925, 2);
DELETE FROM current_node_tags WHERE node_id = 80;
DELETE FROM node_tags WHERE node_id = 80;
DELETE FROM current_way_nodes WHERE node_id=80;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 80;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=80;
/* delete node 81*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (81, 424833421, -714769150, 3, false, (now() at time zone 'utc'), 1710365925, 2);
DELETE FROM current_node_tags WHERE node_id = 81;
DELETE FROM node_tags WHERE node_id = 81;
DELETE FROM current_way_nodes WHERE node_id=81;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 81;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=81;
/* delete node 82*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (82, 424833549, -714768258, 3, false, (now() at time zone 'utc'), 1710365925, 2);
DELETE FROM current_node_tags WHERE node_id = 82;
DELETE FROM node_tags WHERE node_id = 82;
DELETE FROM current_way_nodes WHERE node_id=82;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 82;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=82;
/* delete node 83*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (83, 424834247, -714768442, 3, false, (now() at time zone 'utc'), 1710365925, 2);
DELETE FROM current_node_tags WHERE node_id = 83;
DELETE FROM node_tags WHERE node_id = 83;
DELETE FROM current_way_nodes WHERE node_id=83;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 83;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=83;
/* delete node 84*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (84, 424834163, -714769022, 3, false, (now() at time zone 'utc'), 1710365925, 2);
DELETE FROM current_node_tags WHERE node_id = 84;
DELETE FROM node_tags WHERE node_id = 84;
DELETE FROM current_way_nodes WHERE node_id=84;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 84;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=84;
/* delete node 85*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (85, 424834964, -714769232, 3, false, (now() at time zone 'utc'), 1710365925, 2);
DELETE FROM current_node_tags WHERE node_id = 85;
DELETE FROM node_tags WHERE node_id = 85;
DELETE FROM current_way_nodes WHERE node_id=85;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 85;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=85;
/* delete node 86*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (86, 424840529, -714759125, 3, false, (now() at time zone 'utc'), 1710365925, 2);
DELETE FROM current_node_tags WHERE node_id = 86;
DELETE FROM node_tags WHERE node_id = 86;
DELETE FROM current_way_nodes WHERE node_id=86;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 86;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=86;
/* delete node 87*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (87, 424839662, -714760567, 3, false, (now() at time zone 'utc'), 1710365925, 2);
DELETE FROM current_node_tags WHERE node_id = 87;
DELETE FROM node_tags WHERE node_id = 87;
DELETE FROM current_way_nodes WHERE node_id=87;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 87;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=87;
/* delete node 88*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (88, 424839248, -714760113, 3, false, (now() at time zone 'utc'), 1710365925, 2);
DELETE FROM current_node_tags WHERE node_id = 88;
DELETE FROM node_tags WHERE node_id = 88;
DELETE FROM current_way_nodes WHERE node_id=88;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 88;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=88;
/* delete node 89*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (89, 424839520, -714759661, 3, false, (now() at time zone 'utc'), 1710365925, 2);
DELETE FROM current_node_tags WHERE node_id = 89;
DELETE FROM node_tags WHERE node_id = 89;
DELETE FROM current_way_nodes WHERE node_id=89;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 89;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=89;
/* delete node 90*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (90, 424840046, -714758595, 3, false, (now() at time zone 'utc'), 1710365925, 2);
DELETE FROM current_node_tags WHERE node_id = 90;
DELETE FROM node_tags WHERE node_id = 90;
DELETE FROM current_way_nodes WHERE node_id=90;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 90;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=90;
/* delete node 91*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (91, 424849861, -714765015, 3, false, (now() at time zone 'utc'), 1710365936, 2);
DELETE FROM current_node_tags WHERE node_id = 91;
DELETE FROM node_tags WHERE node_id = 91;
DELETE FROM current_way_nodes WHERE node_id=91;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 91;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=91;
/* delete node 92*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (92, 424849094, -714764922, 3, false, (now() at time zone 'utc'), 1710365936, 2);
DELETE FROM current_node_tags WHERE node_id = 92;
DELETE FROM node_tags WHERE node_id = 92;
DELETE FROM current_way_nodes WHERE node_id=92;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 92;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=92;
/* delete node 93*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (93, 424832385, -714754999, 3, false, (now() at time zone 'utc'), 1710365925, 2);
DELETE FROM current_node_tags WHERE node_id = 93;
DELETE FROM node_tags WHERE node_id = 93;
DELETE FROM current_way_nodes WHERE node_id=93;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 93;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=93;
/* delete node 94*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (94, 424849104, -714764769, 3, false, (now() at time zone 'utc'), 1710365936, 2);
DELETE FROM current_node_tags WHERE node_id = 94;
DELETE FROM node_tags WHERE node_id = 94;
DELETE FROM current_way_nodes WHERE node_id=94;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 94;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=94;
/* delete node 95*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (95, 424832000, -714756755, 3, false, (now() at time zone 'utc'), 1710365925, 2);
DELETE FROM current_node_tags WHERE node_id = 95;
DELETE FROM node_tags WHERE node_id = 95;
DELETE FROM current_way_nodes WHERE node_id=95;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 95;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=95;
/* delete node 96*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (96, 424848924, -714764747, 3, false, (now() at time zone 'utc'), 1710365936, 2);
DELETE FROM current_node_tags WHERE node_id = 96;
DELETE FROM node_tags WHERE node_id = 96;
DELETE FROM current_way_nodes WHERE node_id=96;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 96;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=96;
/* delete node 98*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (98, 424848915, -714764886, 3, false, (now() at time zone 'utc'), 1710365936, 2);
DELETE FROM current_node_tags WHERE node_id = 98;
DELETE FROM node_tags WHERE node_id = 98;
DELETE FROM current_way_nodes WHERE node_id=98;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 98;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=98;
/* delete node 100*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (100, 424848204, -714764801, 3, false, (now() at time zone 'utc'), 1710365936, 2);
DELETE FROM current_node_tags WHERE node_id = 100;
DELETE FROM node_tags WHERE node_id = 100;
DELETE FROM current_way_nodes WHERE node_id=100;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 100;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=100;
/* delete node 101*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (101, 424841526, -714769999, 3, false, (now() at time zone 'utc'), 1710365925, 2);
DELETE FROM current_node_tags WHERE node_id = 101;
DELETE FROM node_tags WHERE node_id = 101;
DELETE FROM current_way_nodes WHERE node_id=101;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 101;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=101;
/* delete node 102*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (102, 424848305, -714763265, 3, false, (now() at time zone 'utc'), 1710365936, 2);
DELETE FROM current_node_tags WHERE node_id = 102;
DELETE FROM node_tags WHERE node_id = 102;
DELETE FROM current_way_nodes WHERE node_id=102;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 102;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=102;
/* delete node 103*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (103, 424842295, -714769999, 3, false, (now() at time zone 'utc'), 1710365925, 2);
DELETE FROM current_node_tags WHERE node_id = 103;
DELETE FROM node_tags WHERE node_id = 103;
DELETE FROM current_way_nodes WHERE node_id=103;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 103;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=103;
/* delete node 104*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (104, 424848733, -714763316, 3, false, (now() at time zone 'utc'), 1710365936, 2);
DELETE FROM current_node_tags WHERE node_id = 104;
DELETE FROM node_tags WHERE node_id = 104;
DELETE FROM current_way_nodes WHERE node_id=104;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 104;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=104;
/* delete node 106*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (106, 424848698, -714763845, 3, false, (now() at time zone 'utc'), 1710365936, 2);
DELETE FROM current_node_tags WHERE node_id = 106;
DELETE FROM node_tags WHERE node_id = 106;
DELETE FROM current_way_nodes WHERE node_id=106;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 106;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=106;
/* delete node 108*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (108, 424849002, -714763882, 3, false, (now() at time zone 'utc'), 1710365936, 2);
DELETE FROM current_node_tags WHERE node_id = 108;
DELETE FROM node_tags WHERE node_id = 108;
DELETE FROM current_way_nodes WHERE node_id=108;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 108;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=108;
/* delete node 109*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (109, 424851000, -714757943, 3, false, (now() at time zone 'utc'), 1710365936, 2);
DELETE FROM current_node_tags WHERE node_id = 109;
DELETE FROM node_tags WHERE node_id = 109;
DELETE FROM current_way_nodes WHERE node_id=109;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 109;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=109;
/* delete node 110*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (110, 424849039, -714763315, 3, false, (now() at time zone 'utc'), 1710365936, 2);
DELETE FROM current_node_tags WHERE node_id = 110;
DELETE FROM node_tags WHERE node_id = 110;
DELETE FROM current_way_nodes WHERE node_id=110;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 110;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=110;
/* delete node 111*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (111, 424851000, -714758301, 3, false, (now() at time zone 'utc'), 1710365936, 2);
DELETE FROM current_node_tags WHERE node_id = 111;
DELETE FROM node_tags WHERE node_id = 111;
DELETE FROM current_way_nodes WHERE node_id=111;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 111;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=111;
/* delete node 112*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (112, 424849966, -714763426, 3, false, (now() at time zone 'utc'), 1710365936, 2);
DELETE FROM current_node_tags WHERE node_id = 112;
DELETE FROM node_tags WHERE node_id = 112;
DELETE FROM current_way_nodes WHERE node_id=112;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 112;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=112;
/* delete node 113*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (113, 424834853, -714769999, 3, false, (now() at time zone 'utc'), 1710365925, 2);
DELETE FROM current_node_tags WHERE node_id = 113;
DELETE FROM node_tags WHERE node_id = 113;
DELETE FROM current_way_nodes WHERE node_id=113;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 113;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=113;
/* delete node 114*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (114, 424834759, -714769999, 3, false, (now() at time zone 'utc'), 1710365925, 2);
DELETE FROM current_node_tags WHERE node_id = 114;
DELETE FROM node_tags WHERE node_id = 114;
DELETE FROM current_way_nodes WHERE node_id=114;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 114;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=114;
/* delete node 115*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (115, 424844186, -714754999, 3, false, (now() at time zone 'utc'), 1710365925, 2);
DELETE FROM current_node_tags WHERE node_id = 115;
DELETE FROM node_tags WHERE node_id = 115;
DELETE FROM current_way_nodes WHERE node_id=115;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 115;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=115;
/* delete node 116*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (116, 424843514, -714754999, 3, false, (now() at time zone 'utc'), 1710365925, 2);
DELETE FROM current_node_tags WHERE node_id = 116;
DELETE FROM node_tags WHERE node_id = 116;
DELETE FROM current_way_nodes WHERE node_id=116;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 116;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=116;
/* delete node 117*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (117, 424844183, -714755253, 3, false, (now() at time zone 'utc'), 1710365925, 2);
DELETE FROM current_node_tags WHERE node_id = 117;
DELETE FROM node_tags WHERE node_id = 117;
DELETE FROM current_way_nodes WHERE node_id=117;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 117;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=117;
/* delete node 118*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (118, 424843512, -714755241, 3, false, (now() at time zone 'utc'), 1710365925, 2);
DELETE FROM current_node_tags WHERE node_id = 118;
DELETE FROM node_tags WHERE node_id = 118;
DELETE FROM current_way_nodes WHERE node_id=118;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 118;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=118;
/* create node 134*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (134, 424840442, -714759117, 3, true, (now() at time zone 'utc'), 1710365925, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (134, 424840442, -714759117, 3, true, (now() at time zone 'utc'), 1710365925, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (134, 'hoot:id', '134');
INSERT INTO node_tags (node_id, k, v, version) VALUES (134, 'hoot:id', '134', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (134, 'hoot:status', '2');
INSERT INTO node_tags (node_id, k, v, version) VALUES (134, 'hoot:status', '2', 1);
/* create node 142*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (142, 424839575, -714760559, 3, true, (now() at time zone 'utc'), 1710365925, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (142, 424839575, -714760559, 3, true, (now() at time zone 'utc'), 1710365925, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (142, 'hoot:id', '142');
INSERT INTO node_tags (node_id, k, v, version) VALUES (142, 'hoot:id', '142', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (142, 'hoot:status', '2');
INSERT INTO node_tags (node_id, k, v, version) VALUES (142, 'hoot:status', '2', 1);
/* create node 146*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (146, 424839433, -714759654, 3, true, (now() at time zone 'utc'), 1710365925, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (146, 424839433, -714759654, 3, true, (now() at time zone 'utc'), 1710365925, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (146, 'hoot:id', '146');
INSERT INTO node_tags (node_id, k, v, version) VALUES (146, 'hoot:id', '146', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (146, 'hoot:status', '2');
INSERT INTO node_tags (node_id, k, v, version) VALUES (146, 'hoot:status', '2', 1);
/* create node 150*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (150, 424839364, -714759578, 3, true, (now() at time zone 'utc'), 1710365925, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (150, 424839364, -714759578, 3, true, (now() at time zone 'utc'), 1710365925, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (150, 'hoot:id', '150');
INSERT INTO node_tags (node_id, k, v, version) VALUES (150, 'hoot:id', '150', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (150, 'hoot:status', '2');
INSERT INTO node_tags (node_id, k, v, version) VALUES (150, 'hoot:status', '2', 1);
/* create node 153*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (153, 424839959, -714758588, 3, true, (now() at time zone 'utc'), 1710365925, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (153, 424839959, -714758588, 3, true, (now() at time zone 'utc'), 1710365925, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (153, 'hoot:id', '153');
INSERT INTO node_tags (node_id, k, v, version) VALUES (153, 'hoot:id', '153', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (153, 'hoot:status', '2');
INSERT INTO node_tags (node_id, k, v, version) VALUES (153, 'hoot:status', '2', 1);
/* create node 159*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (159, 424839162, -714760105, 3, true, (now() at time zone 'utc'), 1710365925, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (159, 424839162, -714760105, 3, true, (now() at time zone 'utc'), 1710365925, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (159, 'hoot:id', '159');
INSERT INTO node_tags (node_id, k, v, version) VALUES (159, 'hoot:id', '159', 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (159, 'hoot:status', '2');
INSERT INTO node_tags (node_id, k, v, version) VALUES (159, 'hoot:status', '2', 1);
/* modify way 9*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (9, 3, true, (now() at time zone 'utc'), 2);
UPDATE current_ways SET changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), version=2 WHERE id=9;
DELETE FROM current_way_tags WHERE way_id = 9;
DELETE FROM way_tags WHERE way_id = 9;
INSERT INTO current_way_tags (way_id, k, v) VALUES (9, 'source:ingest:datetime', '2017-04-19T17:07:16.707Z');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9, 'source:ingest:datetime', '2017-04-19T17:07:16.707Z', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9, 'hoot:id', '9');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9, 'hoot:id', '9', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9, 'hoot:status', '3');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9, 'hoot:status', '3', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9, 'building', 'yes', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9, 'uuid', '{b40174d3-a4a4-4c78-82a1-f8092bd69d33}');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9, 'uuid', '{b40174d3-a4a4-4c78-82a1-f8092bd69d33}', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9, 'source', 'MassNh_GIS');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9, 'source', 'MassNh_GIS', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9, 'source:datetime', '2012-12-15T01:53:48Z');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9, 'source:datetime', '2012-12-15T01:53:48Z', 2);
DELETE FROM current_way_nodes WHERE way_id = 9;
DELETE FROM way_nodes WHERE way_id = 9;
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9, 146, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9, 146, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9, 159, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9, 159, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9, 142, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9, 142, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9, 134, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9, 134, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9, 153, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9, 153, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9, 150, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9, 150, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9, 146, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9, 146, 7);
/* modify way 12*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (12, 3, true, (now() at time zone 'utc'), 2);
UPDATE current_ways SET changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), version=2 WHERE id=12;
DELETE FROM current_way_tags WHERE way_id = 12;
DELETE FROM way_tags WHERE way_id = 12;
INSERT INTO current_way_tags (way_id, k, v) VALUES (12, 'width', '9.8');
INSERT INTO way_tags (way_id, k, v, version) VALUES (12, 'width', '9.8', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (12, 'source:ingest:datetime', '2017-04-19T17:06:34.105Z');
INSERT INTO way_tags (way_id, k, v, version) VALUES (12, 'source:ingest:datetime', '2017-04-19T17:06:34.105Z', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (12, 'condition', 'functional');
INSERT INTO way_tags (way_id, k, v, version) VALUES (12, 'condition', 'functional', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (12, 'width:minimum_traveled_way', '7.315200000000001');
INSERT INTO way_tags (way_id, k, v, version) VALUES (12, 'width:minimum_traveled_way', '7.315200000000001', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (12, 'hoot:id', '12');
INSERT INTO way_tags (way_id, k, v, version) VALUES (12, 'hoot:id', '12', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (12, 'hoot:status', '3');
INSERT INTO way_tags (way_id, k, v, version) VALUES (12, 'hoot:status', '3', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (12, 'uuid', '{c749fa63-2000-4490-a754-530cfdbb20c7}');
INSERT INTO way_tags (way_id, k, v, version) VALUES (12, 'uuid', '{c749fa63-2000-4490-a754-530cfdbb20c7}', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (12, 'source', 'MassNh_GIS;massgis_import_v0.1_20071009100433');
INSERT INTO way_tags (way_id, k, v, version) VALUES (12, 'source', 'MassNh_GIS;massgis_import_v0.1_20071009100433', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (12, 'name', 'Mohawk Drive');
INSERT INTO way_tags (way_id, k, v, version) VALUES (12, 'name', 'Mohawk Drive', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (12, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)');
INSERT INTO way_tags (way_id, k, v, version) VALUES (12, 'attribution', 'Office of Geographic and Environmental Information (MassGIS)', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (12, 'highway', 'tertiary');
INSERT INTO way_tags (way_id, k, v, version) VALUES (12, 'highway', 'tertiary', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (12, 'source:datetime', '2007-10-15T15:32:17Z');
INSERT INTO way_tags (way_id, k, v, version) VALUES (12, 'source:datetime', '2007-10-15T15:32:17Z', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (12, 'lanes', '2');
INSERT INTO way_tags (way_id, k, v, version) VALUES (12, 'lanes', '2', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (12, 'surface', 'asphalt');
INSERT INTO way_tags (way_id, k, v, version) VALUES (12, 'surface', 'asphalt', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (12, 'massgis:way_id', '181982');
INSERT INTO way_tags (way_id, k, v, version) VALUES (12, 'massgis:way_id', '181982', 2);
DELETE FROM current_way_nodes WHERE way_id = 12;
DELETE FROM way_nodes WHERE way_id = 12;
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (12, 99, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (12, 99, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (12, 127, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (12, 127, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (12, 126, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (12, 126, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (12, 125, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (12, 125, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (12, 124, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (12, 124, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (12, 123, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (12, 123, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (12, 122, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (12, 122, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (12, 121, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (12, 121, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (12, 120, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (12, 120, 9);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (12, 119, 1, 10);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (12, 119, 10);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (12, 97, 1, 11);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (12, 97, 11);
/* delete way 15*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (15, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 15;
DELETE FROM way_tags WHERE way_id = 15;
DELETE FROM current_way_nodes WHERE way_id=15;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 15;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=15;
/* delete way 16*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (16, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 16;
DELETE FROM way_tags WHERE way_id = 16;
DELETE FROM current_way_nodes WHERE way_id=16;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 16;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=16;
/* delete way 17*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (17, 3, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 17;
DELETE FROM way_tags WHERE way_id = 17;
DELETE FROM current_way_nodes WHERE way_id=17;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 17;
UPDATE current_ways SET changeset_id=3, visible=false, version=2 WHERE id=17;
UPDATE changesets SET min_lat=424832000, max_lat=424851000, min_lon=-714769999, max_lon=-714754999, num_changes=104 WHERE id=3;
