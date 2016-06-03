INSERT INTO changesets (id, user_id, created_at, closed_at) VALUES (1, 1, (now() at time zone 'utc'), (now() at time zone 'utc'));
INSERT INTO changeset_tags (changeset_id, k, v) VALUES (1, 'written_by', 'Hootenanny');
/* delete node -1669793*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (-1669793, 388549321, -1048979049, 1, false, (now() at time zone 'utc'), 1329019012, 4);
DELETE FROM current_node_tags WHERE node_id = -1669793;
DELETE FROM node_tags WHERE node_id = -1669793;
DELETE FROM current_way_nodes WHERE node_id=-1669793;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = -1669793;
UPDATE current_nodes SET changeset_id=1, visible=false, version=4 WHERE id=-1669793;
/* delete node -1669791*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (-1669791, 388549524, -1048987388, 1, false, (now() at time zone 'utc'), 1329019012, 2);
DELETE FROM current_node_tags WHERE node_id = -1669791;
DELETE FROM node_tags WHERE node_id = -1669791;
DELETE FROM current_way_nodes WHERE node_id=-1669791;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = -1669791;
UPDATE current_nodes SET changeset_id=1, visible=false, version=2 WHERE id=-1669791;
/* delete node -1669789*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (-1669789, 388540541, -1049024315, 1, false, (now() at time zone 'utc'), 1329018926, 2);
DELETE FROM current_node_tags WHERE node_id = -1669789;
DELETE FROM node_tags WHERE node_id = -1669789;
DELETE FROM current_way_nodes WHERE node_id=-1669789;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = -1669789;
UPDATE current_nodes SET changeset_id=1, visible=false, version=2 WHERE id=-1669789;
/* delete node -1669787*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (-1669787, 388541299, -1049023064, 1, false, (now() at time zone 'utc'), 1329018926, 4);
DELETE FROM current_node_tags WHERE node_id = -1669787;
DELETE FROM node_tags WHERE node_id = -1669787;
DELETE FROM current_way_nodes WHERE node_id=-1669787;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = -1669787;
UPDATE current_nodes SET changeset_id=1, visible=false, version=4 WHERE id=-1669787;
/* delete node -1669785*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (-1669785, 388543319, -1049017876, 1, false, (now() at time zone 'utc'), 1329018926, 3);
DELETE FROM current_node_tags WHERE node_id = -1669785;
DELETE FROM node_tags WHERE node_id = -1669785;
DELETE FROM current_way_nodes WHERE node_id=-1669785;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = -1669785;
UPDATE current_nodes SET changeset_id=1, visible=false, version=3 WHERE id=-1669785;
/* delete node -1669783*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (-1669783, 388548207, -1049008078, 1, false, (now() at time zone 'utc'), 1329018926, 2);
DELETE FROM current_node_tags WHERE node_id = -1669783;
DELETE FROM node_tags WHERE node_id = -1669783;
DELETE FROM current_way_nodes WHERE node_id=-1669783;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = -1669783;
UPDATE current_nodes SET changeset_id=1, visible=false, version=2 WHERE id=-1669783;
/* delete node -1669781*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (-1669781, 388549290, -1049005252, 1, false, (now() at time zone 'utc'), 1329018926, 2);
DELETE FROM current_node_tags WHERE node_id = -1669781;
DELETE FROM node_tags WHERE node_id = -1669781;
DELETE FROM current_way_nodes WHERE node_id=-1669781;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = -1669781;
UPDATE current_nodes SET changeset_id=1, visible=false, version=2 WHERE id=-1669781;
/* delete node -1669779*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (-1669779, 388548514, -1049005692, 1, false, (now() at time zone 'utc'), 1329018926, 2);
DELETE FROM current_node_tags WHERE node_id = -1669779;
DELETE FROM node_tags WHERE node_id = -1669779;
DELETE FROM current_way_nodes WHERE node_id=-1669779;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = -1669779;
UPDATE current_nodes SET changeset_id=1, visible=false, version=2 WHERE id=-1669779;
/* delete node -1669777*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (-1669777, 388549073, -1048961822, 1, false, (now() at time zone 'utc'), 1329019012, 3);
DELETE FROM current_node_tags WHERE node_id = -1669777;
DELETE FROM node_tags WHERE node_id = -1669777;
DELETE FROM current_way_nodes WHERE node_id=-1669777;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = -1669777;
UPDATE current_nodes SET changeset_id=1, visible=false, version=3 WHERE id=-1669777;
/* delete node -1669775*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (-1669775, 388549019, -1048964393, 1, false, (now() at time zone 'utc'), 1329019012, 2);
DELETE FROM current_node_tags WHERE node_id = -1669775;
DELETE FROM node_tags WHERE node_id = -1669775;
DELETE FROM current_way_nodes WHERE node_id=-1669775;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = -1669775;
UPDATE current_nodes SET changeset_id=1, visible=false, version=2 WHERE id=-1669775;
/* delete node -1669773*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (-1669773, 388548604, -1048968586, 1, false, (now() at time zone 'utc'), 1329019012, 2);
DELETE FROM current_node_tags WHERE node_id = -1669773;
DELETE FROM node_tags WHERE node_id = -1669773;
DELETE FROM current_way_nodes WHERE node_id=-1669773;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = -1669773;
UPDATE current_nodes SET changeset_id=1, visible=false, version=2 WHERE id=-1669773;
/* delete node -1669771*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (-1669771, 388547197, -1048973218, 1, false, (now() at time zone 'utc'), 1329019012, 2);
DELETE FROM current_node_tags WHERE node_id = -1669771;
DELETE FROM node_tags WHERE node_id = -1669771;
DELETE FROM current_way_nodes WHERE node_id=-1669771;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = -1669771;
UPDATE current_nodes SET changeset_id=1, visible=false, version=2 WHERE id=-1669771;
/* delete node -1669769*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (-1669769, 388546043, -1048977758, 1, false, (now() at time zone 'utc'), 1329019012, 3);
DELETE FROM current_node_tags WHERE node_id = -1669769;
DELETE FROM node_tags WHERE node_id = -1669769;
DELETE FROM current_way_nodes WHERE node_id=-1669769;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = -1669769;
UPDATE current_nodes SET changeset_id=1, visible=false, version=3 WHERE id=-1669769;
/* delete node -1669767*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (-1669767, 388544293, -1048980653, 1, false, (now() at time zone 'utc'), 1329019012, 2);
DELETE FROM current_node_tags WHERE node_id = -1669767;
DELETE FROM node_tags WHERE node_id = -1669767;
DELETE FROM current_way_nodes WHERE node_id=-1669767;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = -1669767;
UPDATE current_nodes SET changeset_id=1, visible=false, version=2 WHERE id=-1669767;
/* delete node -1669765*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (-1669765, 388542327, -1048983109, 1, false, (now() at time zone 'utc'), 1329019012, 2);
DELETE FROM current_node_tags WHERE node_id = -1669765;
DELETE FROM node_tags WHERE node_id = -1669765;
DELETE FROM current_way_nodes WHERE node_id=-1669765;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = -1669765;
UPDATE current_nodes SET changeset_id=1, visible=false, version=2 WHERE id=-1669765;
/* delete node -1669763*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (-1669763, 388541768, -1048987069, 1, false, (now() at time zone 'utc'), 1329019012, 3);
DELETE FROM current_node_tags WHERE node_id = -1669763;
DELETE FROM node_tags WHERE node_id = -1669763;
DELETE FROM current_way_nodes WHERE node_id=-1669763;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = -1669763;
UPDATE current_nodes SET changeset_id=1, visible=false, version=3 WHERE id=-1669763;
/* delete node -1669761*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (-1669761, 388541335, -1048988899, 1, false, (now() at time zone 'utc'), 1329019012, 2);
DELETE FROM current_node_tags WHERE node_id = -1669761;
DELETE FROM node_tags WHERE node_id = -1669761;
DELETE FROM current_way_nodes WHERE node_id=-1669761;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = -1669761;
UPDATE current_nodes SET changeset_id=1, visible=false, version=2 WHERE id=-1669761;
/* delete node -1669759*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (-1669759, 388539585, -1048989154, 1, false, (now() at time zone 'utc'), 1329019012, 2);
DELETE FROM current_node_tags WHERE node_id = -1669759;
DELETE FROM node_tags WHERE node_id = -1669759;
DELETE FROM current_way_nodes WHERE node_id=-1669759;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = -1669759;
UPDATE current_nodes SET changeset_id=1, visible=false, version=2 WHERE id=-1669759;
/* delete node -1669757*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (-1669757, 388537204, -1048987232, 1, false, (now() at time zone 'utc'), 1329019009, 5);
DELETE FROM current_node_tags WHERE node_id = -1669757;
DELETE FROM node_tags WHERE node_id = -1669757;
DELETE FROM current_way_nodes WHERE node_id=-1669757;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = -1669757;
UPDATE current_nodes SET changeset_id=1, visible=false, version=5 WHERE id=-1669757;
/* delete node -1669755*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (-1669755, 388536194, -1048987069, 1, false, (now() at time zone 'utc'), 1329019009, 2);
DELETE FROM current_node_tags WHERE node_id = -1669755;
DELETE FROM node_tags WHERE node_id = -1669755;
DELETE FROM current_way_nodes WHERE node_id=-1669755;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = -1669755;
UPDATE current_nodes SET changeset_id=1, visible=false, version=2 WHERE id=-1669755;
/* delete node -1669753*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (-1669753, 388535509, -1048988320, 1, false, (now() at time zone 'utc'), 1329019009, 2);
DELETE FROM current_node_tags WHERE node_id = -1669753;
DELETE FROM node_tags WHERE node_id = -1669753;
DELETE FROM current_way_nodes WHERE node_id=-1669753;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = -1669753;
UPDATE current_nodes SET changeset_id=1, visible=false, version=2 WHERE id=-1669753;
/* delete node -1669751*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (-1669751, 388535689, -1048990567, 1, false, (now() at time zone 'utc'), 1329019009, 2);
DELETE FROM current_node_tags WHERE node_id = -1669751;
DELETE FROM node_tags WHERE node_id = -1669751;
DELETE FROM current_way_nodes WHERE node_id=-1669751;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = -1669751;
UPDATE current_nodes SET changeset_id=1, visible=false, version=2 WHERE id=-1669751;
/* delete node -1669749*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (-1669749, 388535166, -1048992698, 1, false, (now() at time zone 'utc'), 1329018923, 2);
DELETE FROM current_node_tags WHERE node_id = -1669749;
DELETE FROM node_tags WHERE node_id = -1669749;
DELETE FROM current_way_nodes WHERE node_id=-1669749;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = -1669749;
UPDATE current_nodes SET changeset_id=1, visible=false, version=2 WHERE id=-1669749;
/* delete node -1669747*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (-1669747, 388533470, -1048993671, 1, false, (now() at time zone 'utc'), 1329018923, 5);
DELETE FROM current_node_tags WHERE node_id = -1669747;
DELETE FROM node_tags WHERE node_id = -1669747;
DELETE FROM current_way_nodes WHERE node_id=-1669747;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = -1669747;
UPDATE current_nodes SET changeset_id=1, visible=false, version=5 WHERE id=-1669747;
/* delete node -1669745*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (-1669745, 388533001, -1048994528, 1, false, (now() at time zone 'utc'), 1329018923, 2);
DELETE FROM current_node_tags WHERE node_id = -1669745;
DELETE FROM node_tags WHERE node_id = -1669745;
DELETE FROM current_way_nodes WHERE node_id=-1669745;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = -1669745;
UPDATE current_nodes SET changeset_id=1, visible=false, version=2 WHERE id=-1669745;
/* delete node -1669743*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (-1669743, 388532424, -1048996867, 1, false, (now() at time zone 'utc'), 1329018923, 6);
DELETE FROM current_node_tags WHERE node_id = -1669743;
DELETE FROM node_tags WHERE node_id = -1669743;
DELETE FROM current_way_nodes WHERE node_id=-1669743;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = -1669743;
UPDATE current_nodes SET changeset_id=1, visible=false, version=6 WHERE id=-1669743;
/* delete node -1669741*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (-1669741, 388532583, -1048997542, 1, false, (now() at time zone 'utc'), 1329018923, 2);
DELETE FROM current_node_tags WHERE node_id = -1669741;
DELETE FROM node_tags WHERE node_id = -1669741;
DELETE FROM current_way_nodes WHERE node_id=-1669741;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = -1669741;
UPDATE current_nodes SET changeset_id=1, visible=false, version=2 WHERE id=-1669741;
/* delete node -1669739*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (-1669739, 388536248, -1048996934, 1, false, (now() at time zone 'utc'), 1329018923, 4);
DELETE FROM current_node_tags WHERE node_id = -1669739;
DELETE FROM node_tags WHERE node_id = -1669739;
DELETE FROM current_way_nodes WHERE node_id=-1669739;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = -1669739;
UPDATE current_nodes SET changeset_id=1, visible=false, version=4 WHERE id=-1669739;
/* delete node -1669737*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (-1669737, 388539526, -1048996839, 1, false, (now() at time zone 'utc'), 1329018926, 2);
DELETE FROM current_node_tags WHERE node_id = -1669737;
DELETE FROM node_tags WHERE node_id = -1669737;
DELETE FROM current_way_nodes WHERE node_id=-1669737;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = -1669737;
UPDATE current_nodes SET changeset_id=1, visible=false, version=2 WHERE id=-1669737;
/* delete node -1669735*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (-1669735, 388542618, -1048997170, 1, false, (now() at time zone 'utc'), 1329018926, 2);
DELETE FROM current_node_tags WHERE node_id = -1669735;
DELETE FROM node_tags WHERE node_id = -1669735;
DELETE FROM current_way_nodes WHERE node_id=-1669735;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = -1669735;
UPDATE current_nodes SET changeset_id=1, visible=false, version=2 WHERE id=-1669735;
/* delete node -1669733*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (-1669733, 388545785, -1048997170, 1, false, (now() at time zone 'utc'), 1329018926, 2);
DELETE FROM current_node_tags WHERE node_id = -1669733;
DELETE FROM node_tags WHERE node_id = -1669733;
DELETE FROM current_way_nodes WHERE node_id=-1669733;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = -1669733;
UPDATE current_nodes SET changeset_id=1, visible=false, version=2 WHERE id=-1669733;
/* delete node -1669731*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (-1669731, 388549614, -1048997123, 1, false, (now() at time zone 'utc'), 1329018926, 2);
DELETE FROM current_node_tags WHERE node_id = -1669731;
DELETE FROM node_tags WHERE node_id = -1669731;
DELETE FROM current_way_nodes WHERE node_id=-1669731;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = -1669731;
UPDATE current_nodes SET changeset_id=1, visible=false, version=2 WHERE id=-1669731;
/* delete node -1669729*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (-1669729, 388542600, -1049005794, 1, false, (now() at time zone 'utc'), 1329018926, 3);
DELETE FROM current_node_tags WHERE node_id = -1669729;
DELETE FROM node_tags WHERE node_id = -1669729;
DELETE FROM current_way_nodes WHERE node_id=-1669729;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = -1669729;
UPDATE current_nodes SET changeset_id=1, visible=false, version=3 WHERE id=-1669729;
/* delete node -1669727*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (-1669727, 388542471, -1049010788, 1, false, (now() at time zone 'utc'), 1329018926, 2);
DELETE FROM current_node_tags WHERE node_id = -1669727;
DELETE FROM node_tags WHERE node_id = -1669727;
DELETE FROM current_way_nodes WHERE node_id=-1669727;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = -1669727;
UPDATE current_nodes SET changeset_id=1, visible=false, version=2 WHERE id=-1669727;
/* delete node -1669725*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (-1669725, 388540851, -1049014476, 1, false, (now() at time zone 'utc'), 1329018926, 3);
DELETE FROM current_node_tags WHERE node_id = -1669725;
DELETE FROM node_tags WHERE node_id = -1669725;
DELETE FROM current_way_nodes WHERE node_id=-1669725;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = -1669725;
UPDATE current_nodes SET changeset_id=1, visible=false, version=3 WHERE id=-1669725;
/* delete node -1669723*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (-1669723, 388541670, -1048997069, 1, false, (now() at time zone 'utc'), 1329018926, 2);
DELETE FROM current_node_tags WHERE node_id = -1669723;
DELETE FROM node_tags WHERE node_id = -1669723;
DELETE FROM current_way_nodes WHERE node_id=-1669723;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = -1669723;
UPDATE current_nodes SET changeset_id=1, visible=false, version=2 WHERE id=-1669723;
/* create node 1*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1, 388532642, -1048997532, 1, true, (now() at time zone 'utc'), 1329018923, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1, 388532642, -1048997532, 1, true, (now() at time zone 'utc'), 1329018923, 1);
/* create node 2*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2, 388541670, -1048997069, 1, true, (now() at time zone 'utc'), 1329018926, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2, 388541670, -1048997069, 1, true, (now() at time zone 'utc'), 1329018926, 1);
/* create node 3*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3, 388540851, -1049014476, 1, true, (now() at time zone 'utc'), 1329018926, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3, 388540851, -1049014476, 1, true, (now() at time zone 'utc'), 1329018926, 1);
/* create node 4*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (4, 388542471, -1049010788, 1, true, (now() at time zone 'utc'), 1329018926, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (4, 388542471, -1049010788, 1, true, (now() at time zone 'utc'), 1329018926, 1);
/* create node 5*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (5, 388542600, -1049005794, 1, true, (now() at time zone 'utc'), 1329018926, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (5, 388542600, -1049005794, 1, true, (now() at time zone 'utc'), 1329018926, 1);
/* create node 6*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (6, 388549614, -1048997123, 1, true, (now() at time zone 'utc'), 1329018926, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (6, 388549614, -1048997123, 1, true, (now() at time zone 'utc'), 1329018926, 1);
/* create node 7*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7, 388545785, -1048997170, 1, true, (now() at time zone 'utc'), 1329018926, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (7, 388545785, -1048997170, 1, true, (now() at time zone 'utc'), 1329018926, 1);
/* create node 8*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (8, 388542618, -1048997170, 1, true, (now() at time zone 'utc'), 1329018926, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (8, 388542618, -1048997170, 1, true, (now() at time zone 'utc'), 1329018926, 1);
/* create node 9*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (9, 388539525, -1048996839, 1, true, (now() at time zone 'utc'), 1329018926, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (9, 388539525, -1048996839, 1, true, (now() at time zone 'utc'), 1329018926, 1);
/* create node 10*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (10, 388536248, -1048996934, 1, true, (now() at time zone 'utc'), 1329018923, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (10, 388536248, -1048996934, 1, true, (now() at time zone 'utc'), 1329018923, 1);
/* create node 11*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11, 388532583, -1048997542, 1, true, (now() at time zone 'utc'), 1329018923, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11, 388532583, -1048997542, 1, true, (now() at time zone 'utc'), 1329018923, 1);
/* create node 12*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12, 388532424, -1048996867, 1, true, (now() at time zone 'utc'), 1329018923, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12, 388532424, -1048996867, 1, true, (now() at time zone 'utc'), 1329018923, 1);
/* create node 13*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13, 388533001, -1048994528, 1, true, (now() at time zone 'utc'), 1329018923, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13, 388533001, -1048994528, 1, true, (now() at time zone 'utc'), 1329018923, 1);
/* create node 14*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (14, 388533470, -1048993671, 1, true, (now() at time zone 'utc'), 1329018923, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (14, 388533470, -1048993671, 1, true, (now() at time zone 'utc'), 1329018923, 1);
/* create node 15*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (15, 388535166, -1048992698, 1, true, (now() at time zone 'utc'), 1329018923, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (15, 388535166, -1048992698, 1, true, (now() at time zone 'utc'), 1329018923, 1);
/* create node 16*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (16, 388535689, -1048990567, 1, true, (now() at time zone 'utc'), 1329019009, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (16, 388535689, -1048990567, 1, true, (now() at time zone 'utc'), 1329019009, 1);
/* create node 17*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (17, 388535509, -1048988320, 1, true, (now() at time zone 'utc'), 1329019009, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (17, 388535509, -1048988320, 1, true, (now() at time zone 'utc'), 1329019009, 1);
/* create node 18*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (18, 388536194, -1048987069, 1, true, (now() at time zone 'utc'), 1329019009, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (18, 388536194, -1048987069, 1, true, (now() at time zone 'utc'), 1329019009, 1);
/* create node 19*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (19, 388537204, -1048987232, 1, true, (now() at time zone 'utc'), 1329019009, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (19, 388537204, -1048987232, 1, true, (now() at time zone 'utc'), 1329019009, 1);
/* create node 20*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (20, 388539585, -1048989154, 1, true, (now() at time zone 'utc'), 1329019012, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (20, 388539585, -1048989154, 1, true, (now() at time zone 'utc'), 1329019012, 1);
/* create node 21*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (21, 388541335, -1048988899, 1, true, (now() at time zone 'utc'), 1329019012, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (21, 388541335, -1048988899, 1, true, (now() at time zone 'utc'), 1329019012, 1);
/* create node 22*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (22, 388541768, -1048987069, 1, true, (now() at time zone 'utc'), 1329019012, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (22, 388541768, -1048987069, 1, true, (now() at time zone 'utc'), 1329019012, 1);
/* create node 23*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (23, 388542327, -1048983109, 1, true, (now() at time zone 'utc'), 1329019012, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (23, 388542327, -1048983109, 1, true, (now() at time zone 'utc'), 1329019012, 1);
/* create node 24*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (24, 388544293, -1048980653, 1, true, (now() at time zone 'utc'), 1329019012, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (24, 388544293, -1048980653, 1, true, (now() at time zone 'utc'), 1329019012, 1);
/* create node 25*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (25, 388546043, -1048977758, 1, true, (now() at time zone 'utc'), 1329019012, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (25, 388546043, -1048977758, 1, true, (now() at time zone 'utc'), 1329019012, 1);
/* create node 26*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (26, 388547197, -1048973218, 1, true, (now() at time zone 'utc'), 1329019012, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (26, 388547197, -1048973218, 1, true, (now() at time zone 'utc'), 1329019012, 1);
/* create node 27*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (27, 388548604, -1048968586, 1, true, (now() at time zone 'utc'), 1329019012, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (27, 388548604, -1048968586, 1, true, (now() at time zone 'utc'), 1329019012, 1);
/* create node 28*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (28, 388549019, -1048964393, 1, true, (now() at time zone 'utc'), 1329019012, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (28, 388549019, -1048964393, 1, true, (now() at time zone 'utc'), 1329019012, 1);
/* create node 29*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (29, 388549073, -1048961822, 1, true, (now() at time zone 'utc'), 1329019012, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (29, 388549073, -1048961822, 1, true, (now() at time zone 'utc'), 1329019012, 1);
/* create node 30*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (30, 388548514, -1049005692, 1, true, (now() at time zone 'utc'), 1329018926, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (30, 388548514, -1049005692, 1, true, (now() at time zone 'utc'), 1329018926, 1);
/* create node 31*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (31, 388549290, -1049005252, 1, true, (now() at time zone 'utc'), 1329018926, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (31, 388549290, -1049005252, 1, true, (now() at time zone 'utc'), 1329018926, 1);
/* create node 32*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (32, 388548207, -1049008078, 1, true, (now() at time zone 'utc'), 1329018926, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (32, 388548207, -1049008078, 1, true, (now() at time zone 'utc'), 1329018926, 1);
/* create node 33*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (33, 388543319, -1049017876, 1, true, (now() at time zone 'utc'), 1329018926, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (33, 388543319, -1049017876, 1, true, (now() at time zone 'utc'), 1329018926, 1);
/* create node 34*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (34, 388541299, -1049023064, 1, true, (now() at time zone 'utc'), 1329018926, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (34, 388541299, -1049023064, 1, true, (now() at time zone 'utc'), 1329018926, 1);
/* create node 35*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (35, 388540541, -1049024315, 1, true, (now() at time zone 'utc'), 1329018926, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (35, 388540541, -1049024315, 1, true, (now() at time zone 'utc'), 1329018926, 1);
/* create node 36*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (36, 388549524, -1048987388, 1, true, (now() at time zone 'utc'), 1329019012, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (36, 388549524, -1048987388, 1, true, (now() at time zone 'utc'), 1329019012, 1);
/* create node 37*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (37, 388549321, -1048979049, 1, true, (now() at time zone 'utc'), 1329019012, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (37, 388549321, -1048979049, 1, true, (now() at time zone 'utc'), 1329019012, 1);
/* delete way -1669801*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (-1669801, 1, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = -1669801;
DELETE FROM way_tags WHERE way_id = -1669801;
DELETE FROM current_way_nodes WHERE way_id=-1669801;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = -1669801;
UPDATE current_ways SET changeset_id=1, visible=false, version=2 WHERE id=-1669801;
/* delete way -1669799*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (-1669799, 1, false, (now() at time zone 'utc'), 4);
DELETE FROM current_way_tags WHERE way_id = -1669799;
DELETE FROM way_tags WHERE way_id = -1669799;
DELETE FROM current_way_nodes WHERE way_id=-1669799;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = -1669799;
UPDATE current_ways SET changeset_id=1, visible=false, version=4 WHERE id=-1669799;
/* delete way -1669797*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (-1669797, 1, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = -1669797;
DELETE FROM way_tags WHERE way_id = -1669797;
DELETE FROM current_way_nodes WHERE way_id=-1669797;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = -1669797;
UPDATE current_ways SET changeset_id=1, visible=false, version=2 WHERE id=-1669797;
/* delete way -1669795*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (-1669795, 1, false, (now() at time zone 'utc'), 3);
DELETE FROM current_way_tags WHERE way_id = -1669795;
DELETE FROM way_tags WHERE way_id = -1669795;
DELETE FROM current_way_nodes WHERE way_id=-1669795;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = -1669795;
UPDATE current_ways SET changeset_id=1, visible=false, version=3 WHERE id=-1669795;
/* create way 1*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (1, 1, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (1, 1, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (1, 'note', '1');
INSERT INTO way_tags (way_id, k, v, version) VALUES (1, 'note', '1', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (1, 'highway', 'road');
INSERT INTO way_tags (way_id, k, v, version) VALUES (1, 'highway', 'road', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (1, -32, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (1, -32, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (1, -2, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (1, -2, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (1, -1, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (1, -1, 3);
/* create way 2*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (2, 1, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (2, 1, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (2, 'note', '0');
INSERT INTO way_tags (way_id, k, v, version) VALUES (2, 'note', '0', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (2, 'highway', 'road');
INSERT INTO way_tags (way_id, k, v, version) VALUES (2, 'highway', 'road', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (2, -165, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (2, -165, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (2, -27, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (2, -27, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (2, -26, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (2, -26, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (2, -25, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (2, -25, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (2, -24, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (2, -24, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (2, -23, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (2, -23, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (2, -22, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (2, -22, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (2, -21, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (2, -21, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (2, -20, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (2, -20, 9);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (2, -19, 1, 10);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (2, -19, 10);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (2, -18, 1, 11);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (2, -18, 11);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (2, -17, 1, 12);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (2, -17, 12);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (2, -16, 1, 13);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (2, -16, 13);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (2, -15, 1, 14);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (2, -15, 14);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (2, -14, 1, 15);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (2, -14, 15);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (2, -13, 1, 16);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (2, -13, 16);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (2, -12, 1, 17);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (2, -12, 17);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (2, -11, 1, 18);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (2, -11, 18);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (2, -10, 1, 19);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (2, -10, 19);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (2, -9, 1, 20);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (2, -9, 20);
/* create way 3*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (3, 1, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (3, 1, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (3, 'note', '0');
INSERT INTO way_tags (way_id, k, v, version) VALUES (3, 'note', '0', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (3, 'highway', 'road');
INSERT INTO way_tags (way_id, k, v, version) VALUES (3, 'highway', 'road', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (3, -30, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (3, -30, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (3, -36, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (3, -36, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (3, -29, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (3, -29, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (3, -28, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (3, -28, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (3, -165, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (3, -165, 5);
/* create way 4*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (4, 1, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (4, 1, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (4, 'note', '0');
INSERT INTO way_tags (way_id, k, v, version) VALUES (4, 'note', '0', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (4, 'highway', 'road');
INSERT INTO way_tags (way_id, k, v, version) VALUES (4, 'highway', 'road', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (4, -3, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (4, -3, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (4, -4, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (4, -4, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (4, -5, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (4, -5, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (4, -6, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (4, -6, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (4, -7, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (4, -7, 5);
/* create way 5*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (5, 1, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (5, 1, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (5, 'note', '0');
INSERT INTO way_tags (way_id, k, v, version) VALUES (5, 'note', '0', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (5, 'highway', 'road');
INSERT INTO way_tags (way_id, k, v, version) VALUES (5, 'highway', 'road', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (5, -7, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (5, -7, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (5, -32, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (5, -32, 2);
/* create way 6*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (6, 1, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (6, 1, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (6, 'note', '0');
INSERT INTO way_tags (way_id, k, v, version) VALUES (6, 'note', '0', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (6, 'highway', 'road');
INSERT INTO way_tags (way_id, k, v, version) VALUES (6, 'highway', 'road', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (6, -32, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (6, -32, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (6, -31, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (6, -31, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (6, -30, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (6, -30, 3);
/* create way 7*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (7, 1, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (7, 1, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (7, 'note', '2');
INSERT INTO way_tags (way_id, k, v, version) VALUES (7, 'note', '2', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (7, 'highway', 'road');
INSERT INTO way_tags (way_id, k, v, version) VALUES (7, 'highway', 'road', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (7, -35, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (7, -35, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (7, -34, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (7, -34, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (7, -33, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (7, -33, 3);
/* create way 8*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (8, 1, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (8, 1, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (8, 'note', '2');
INSERT INTO way_tags (way_id, k, v, version) VALUES (8, 'note', '2', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (8, 'highway', 'road');
INSERT INTO way_tags (way_id, k, v, version) VALUES (8, 'highway', 'road', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (8, -33, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (8, -33, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (8, -30, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (8, -30, 2);
/* create way 9*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (9, 1, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (9, 1, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9, 'note', '3');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9, 'note', '3', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (9, 'highway', 'road');
INSERT INTO way_tags (way_id, k, v, version) VALUES (9, 'highway', 'road', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9, -33, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9, -33, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9, -8, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9, -8, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (9, -7, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (9, -7, 3);
UPDATE changesets SET min_lat=388532424, max_lat=388549614, min_lon=-1049024315, max_lon=-1048961822, num_changes=86 WHERE id=1;
