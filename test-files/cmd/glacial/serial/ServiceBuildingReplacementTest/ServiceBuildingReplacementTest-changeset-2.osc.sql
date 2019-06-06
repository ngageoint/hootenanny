INSERT INTO changesets (id, user_id, created_at, closed_at) VALUES (2, 1, (now() at time zone 'utc'), (now() at time zone 'utc'));
INSERT INTO changeset_tags (changeset_id, k, v) VALUES (2, 'written_by', 'Hootenanny');
/* delete node 11271*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11271, -12095, -20355, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11271;
DELETE FROM node_tags WHERE node_id = 11271;
DELETE FROM current_way_nodes WHERE node_id=11271;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11271;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11271;
/* delete node 11274*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11274, -12256, -27035, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11274;
DELETE FROM node_tags WHERE node_id = 11274;
DELETE FROM current_way_nodes WHERE node_id=11274;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11274;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11274;
/* delete node 11275*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11275, -12038, -19683, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11275;
DELETE FROM node_tags WHERE node_id = 11275;
DELETE FROM current_way_nodes WHERE node_id=11275;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11275;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11275;
/* delete node 11298*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11298, -12015, -27713, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11298;
DELETE FROM node_tags WHERE node_id = 11298;
DELETE FROM current_way_nodes WHERE node_id=11298;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11298;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11298;
/* delete node 11315*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11315, -12095, -31285, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11315;
DELETE FROM node_tags WHERE node_id = 11315;
DELETE FROM current_way_nodes WHERE node_id=11315;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11315;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11315;
/* delete node 11321*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11321, -11668, -26155, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11321;
DELETE FROM node_tags WHERE node_id = 11321;
DELETE FROM current_way_nodes WHERE node_id=11321;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11321;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11321;
/* delete node 11326*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11326, -11485, -26702, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11326;
DELETE FROM node_tags WHERE node_id = 11326;
DELETE FROM current_way_nodes WHERE node_id=11326;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11326;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11326;
/* delete node 11328*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11328, -10959, -11864, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11328;
DELETE FROM node_tags WHERE node_id = 11328;
DELETE FROM current_way_nodes WHERE node_id=11328;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11328;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11328;
/* delete node 11330*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11330, -11785, -31790, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11330;
DELETE FROM node_tags WHERE node_id = 11330;
DELETE FROM current_way_nodes WHERE node_id=11330;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11330;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11330;
/* delete node 11331*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11331, -11217, -20407, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11331;
DELETE FROM node_tags WHERE node_id = 11331;
DELETE FROM current_way_nodes WHERE node_id=11331;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11331;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11331;
/* delete node 11333*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11333, -11162, -19669, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11333;
DELETE FROM node_tags WHERE node_id = 11333;
DELETE FROM current_way_nodes WHERE node_id=11333;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11333;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11333;
/* delete node 11334*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11334, -11169, -20028, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11334;
DELETE FROM node_tags WHERE node_id = 11334;
DELETE FROM current_way_nodes WHERE node_id=11334;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11334;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11334;
/* delete node 11336*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11336, -11334, -26011, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11336;
DELETE FROM node_tags WHERE node_id = 11336;
DELETE FROM current_way_nodes WHERE node_id=11336;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11336;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11336;
/* delete node 11338*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11338, -11311, -27169, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11338;
DELETE FROM node_tags WHERE node_id = 11338;
DELETE FROM current_way_nodes WHERE node_id=11338;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11338;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11338;
/* delete node 11339*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11339, -11296, -26127, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11339;
DELETE FROM node_tags WHERE node_id = 11339;
DELETE FROM current_way_nodes WHERE node_id=11339;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11339;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11339;
/* delete node 11341*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11341, -11234, -27373, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11341;
DELETE FROM node_tags WHERE node_id = 11341;
DELETE FROM current_way_nodes WHERE node_id=11341;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11341;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11341;
/* delete node 11342*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11342, -11217, -25933, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11342;
DELETE FROM node_tags WHERE node_id = 11342;
DELETE FROM current_way_nodes WHERE node_id=11342;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11342;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11342;
/* delete node 11345*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11345, -10745, -12278, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11345;
DELETE FROM node_tags WHERE node_id = 11345;
DELETE FROM current_way_nodes WHERE node_id=11345;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11345;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11345;
/* delete node 11346*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11346, -11172, -26073, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11346;
DELETE FROM node_tags WHERE node_id = 11346;
DELETE FROM current_way_nodes WHERE node_id=11346;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11346;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11346;
/* delete node 11352*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11352, -10908, -20025, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11352;
DELETE FROM node_tags WHERE node_id = 11352;
DELETE FROM current_way_nodes WHERE node_id=11352;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11352;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11352;
/* delete node 11354*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11354, -11369, -31483, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11354;
DELETE FROM node_tags WHERE node_id = 11354;
DELETE FROM current_way_nodes WHERE node_id=11354;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11354;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11354;
/* delete node 11355*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11355, -10589, -12171, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11355;
DELETE FROM node_tags WHERE node_id = 11355;
DELETE FROM current_way_nodes WHERE node_id=11355;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11355;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11355;
/* delete node 11356*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11356, -11013, -25845, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11356;
DELETE FROM node_tags WHERE node_id = 11356;
DELETE FROM current_way_nodes WHERE node_id=11356;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11356;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11356;
/* delete node 11360*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11360, -10818, -21165, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11360;
DELETE FROM node_tags WHERE node_id = 11360;
DELETE FROM current_way_nodes WHERE node_id=11360;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11360;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11360;
/* delete node 11361*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11361, -11309, -31582, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11361;
DELETE FROM node_tags WHERE node_id = 11361;
DELETE FROM current_way_nodes WHERE node_id=11361;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11361;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11361;
/* delete node 11363*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11363, -10953, -26026, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11363;
DELETE FROM node_tags WHERE node_id = 11363;
DELETE FROM current_way_nodes WHERE node_id=11363;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11363;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11363;
/* delete node 11366*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11366, -10735, -20351, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11366;
DELETE FROM node_tags WHERE node_id = 11366;
DELETE FROM current_way_nodes WHERE node_id=11366;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11366;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11366;
/* delete node 11367*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11367, -10440, -12462, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11367;
DELETE FROM node_tags WHERE node_id = 11367;
DELETE FROM current_way_nodes WHERE node_id=11367;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11367;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11367;
/* delete node 11369*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11369, -10845, -25979, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11369;
DELETE FROM node_tags WHERE node_id = 11369;
DELETE FROM current_way_nodes WHERE node_id=11369;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11369;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11369;
/* delete node 11370*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11370, -10607, -20265, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11370;
DELETE FROM node_tags WHERE node_id = 11370;
DELETE FROM current_way_nodes WHERE node_id=11370;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11370;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11370;
/* delete node 11372*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11372, -10737, -26269, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11372;
DELETE FROM node_tags WHERE node_id = 11372;
DELETE FROM current_way_nodes WHERE node_id=11372;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11372;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11372;
/* delete node 11374*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11374, -10204, -11095, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11374;
DELETE FROM node_tags WHERE node_id = 11374;
DELETE FROM current_way_nodes WHERE node_id=11374;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11374;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11374;
/* delete node 11376*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11376, -10650, -26231, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11376;
DELETE FROM node_tags WHERE node_id = 11376;
DELETE FROM current_way_nodes WHERE node_id=11376;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11376;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11376;
/* delete node 11383*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11383, -10111, -11276, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11383;
DELETE FROM node_tags WHERE node_id = 11383;
DELETE FROM current_way_nodes WHERE node_id=11383;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11383;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11383;
/* delete node 11386*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11386, -10310, -20826, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11386;
DELETE FROM node_tags WHERE node_id = 11386;
DELETE FROM current_way_nodes WHERE node_id=11386;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11386;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11386;
/* delete node 11387*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11387, -10442, -26791, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11387;
DELETE FROM node_tags WHERE node_id = 11387;
DELETE FROM current_way_nodes WHERE node_id=11387;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11387;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11387;
/* delete node 11394*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11394, -10179, -24607, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11394;
DELETE FROM node_tags WHERE node_id = 11394;
DELETE FROM current_way_nodes WHERE node_id=11394;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11394;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11394;
/* delete node 11403*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11403, -9529, -10626, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11403;
DELETE FROM node_tags WHERE node_id = 11403;
DELETE FROM current_way_nodes WHERE node_id=11403;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11403;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11403;
/* delete node 11406*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11406, -10102, -30021, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11406;
DELETE FROM node_tags WHERE node_id = 11406;
DELETE FROM current_way_nodes WHERE node_id=11406;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11406;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11406;
/* delete node 11407*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11407, -9893, -25199, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11407;
DELETE FROM node_tags WHERE node_id = 11407;
DELETE FROM current_way_nodes WHERE node_id=11407;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11407;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11407;
/* delete node 11416*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11416, -9766, -25124, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11416;
DELETE FROM node_tags WHERE node_id = 11416;
DELETE FROM current_way_nodes WHERE node_id=11416;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11416;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11416;
/* delete node 11424*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11424, -9807, -30555, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11424;
DELETE FROM node_tags WHERE node_id = 11424;
DELETE FROM current_way_nodes WHERE node_id=11424;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11424;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11424;
/* delete node 11426*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11426, -9122, -11412, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11426;
DELETE FROM node_tags WHERE node_id = 11426;
DELETE FROM current_way_nodes WHERE node_id=11426;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11426;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11426;
/* delete node 11434*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11434, -9314, -25999, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11434;
DELETE FROM node_tags WHERE node_id = 11434;
DELETE FROM current_way_nodes WHERE node_id=11434;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11434;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11434;
/* delete node 11441*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11441, -9136, -23994, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11441;
DELETE FROM node_tags WHERE node_id = 11441;
DELETE FROM current_way_nodes WHERE node_id=11441;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11441;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11441;
/* delete node 11456*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11456, -8908, -24660, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11456;
DELETE FROM node_tags WHERE node_id = 11456;
DELETE FROM current_way_nodes WHERE node_id=11456;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11456;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11456;
/* delete node 11458*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11458, -8609, -15626, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11458;
DELETE FROM node_tags WHERE node_id = 11458;
DELETE FROM current_way_nodes WHERE node_id=11458;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11458;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11458;
/* delete node 11463*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11463, -8818, -24608, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11463;
DELETE FROM node_tags WHERE node_id = 11463;
DELETE FROM current_way_nodes WHERE node_id=11463;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11463;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11463;
/* delete node 11465*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11465, -8753, -25276, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11465;
DELETE FROM node_tags WHERE node_id = 11465;
DELETE FROM current_way_nodes WHERE node_id=11465;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11465;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11465;
/* delete node 11471*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11471, -8628, -25203, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11471;
DELETE FROM node_tags WHERE node_id = 11471;
DELETE FROM current_way_nodes WHERE node_id=11471;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11471;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11471;
/* delete node 11473*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11473, -8597, -25578, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11473;
DELETE FROM node_tags WHERE node_id = 11473;
DELETE FROM current_way_nodes WHERE node_id=11473;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11473;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11473;
/* delete node 11485*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11485, -8260, -15949, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11485;
DELETE FROM node_tags WHERE node_id = 11485;
DELETE FROM current_way_nodes WHERE node_id=11485;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11485;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11485;
/* delete node 11498*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11498, -8147, -15795, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11498;
DELETE FROM node_tags WHERE node_id = 11498;
DELETE FROM current_way_nodes WHERE node_id=11498;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11498;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11498;
/* delete node 11512*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11512, -7984, -16213, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11512;
DELETE FROM node_tags WHERE node_id = 11512;
DELETE FROM current_way_nodes WHERE node_id=11512;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11512;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11512;
/* delete node 11517*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11517, -7868, -16053, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11517;
DELETE FROM node_tags WHERE node_id = 11517;
DELETE FROM current_way_nodes WHERE node_id=11517;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11517;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11517;
/* delete node 11519*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11519, -7764, -14453, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11519;
DELETE FROM node_tags WHERE node_id = 11519;
DELETE FROM current_way_nodes WHERE node_id=11519;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11519;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11519;
/* delete node 11529*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11529, -7661, -16510, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11529;
DELETE FROM node_tags WHERE node_id = 11529;
DELETE FROM current_way_nodes WHERE node_id=11529;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11529;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11529;
/* delete node 11531*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11531, -7584, -15341, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11531;
DELETE FROM node_tags WHERE node_id = 11531;
DELETE FROM current_way_nodes WHERE node_id=11531;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11531;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11531;
/* delete node 11538*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11538, -7863, -29325, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11538;
DELETE FROM node_tags WHERE node_id = 11538;
DELETE FROM current_way_nodes WHERE node_id=11538;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11538;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11538;
/* delete node 11539*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11539, -7537, -16337, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11539;
DELETE FROM node_tags WHERE node_id = 11539;
DELETE FROM current_way_nodes WHERE node_id=11539;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11539;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11539;
/* delete node 11552*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11552, -7240, -9201, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11552;
DELETE FROM node_tags WHERE node_id = 11552;
DELETE FROM current_way_nodes WHERE node_id=11552;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11552;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11552;
/* delete node 11553*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11553, -7271, -14908, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11553;
DELETE FROM node_tags WHERE node_id = 11553;
DELETE FROM current_way_nodes WHERE node_id=11553;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11553;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11553;
/* delete node 11555*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11555, -7259, -16592, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11555;
DELETE FROM node_tags WHERE node_id = 11555;
DELETE FROM current_way_nodes WHERE node_id=11555;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11555;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11555;
/* delete node 11560*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11560, -7366, -20510, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11560;
DELETE FROM node_tags WHERE node_id = 11560;
DELETE FROM current_way_nodes WHERE node_id=11560;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11560;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11560;
/* delete node 11565*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11565, -7456, -29107, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11565;
DELETE FROM node_tags WHERE node_id = 11565;
DELETE FROM current_way_nodes WHERE node_id=11565;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11565;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11565;
/* delete node 11572*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11572, -6909, -9890, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11572;
DELETE FROM node_tags WHERE node_id = 11572;
DELETE FROM current_way_nodes WHERE node_id=11572;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11572;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11572;
/* delete node 11578*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11578, -7325, -29436, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11578;
DELETE FROM node_tags WHERE node_id = 11578;
DELETE FROM current_way_nodes WHERE node_id=11578;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11578;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11578;
/* delete node 11582*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11582, -6849, -8485, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11582;
DELETE FROM node_tags WHERE node_id = 11582;
DELETE FROM current_way_nodes WHERE node_id=11582;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11582;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11582;
/* delete node 11583*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11583, -6847, -16022, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11583;
DELETE FROM node_tags WHERE node_id = 11583;
DELETE FROM current_way_nodes WHERE node_id=11583;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11583;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11583;
/* delete node 11585*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11585, -7287, -30781, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11585;
DELETE FROM node_tags WHERE node_id = 11585;
DELETE FROM current_way_nodes WHERE node_id=11585;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11585;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11585;
/* delete node 11588*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11588, -7041, -29282, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11588;
DELETE FROM node_tags WHERE node_id = 11588;
DELETE FROM current_way_nodes WHERE node_id=11588;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11588;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11588;
/* delete node 11592*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11592, -6664, -8869, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11592;
DELETE FROM node_tags WHERE node_id = 11592;
DELETE FROM current_way_nodes WHERE node_id=11592;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11592;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11592;
/* delete node 11594*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11594, -6538, -9677, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11594;
DELETE FROM node_tags WHERE node_id = 11594;
DELETE FROM current_way_nodes WHERE node_id=11594;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11594;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11594;
/* delete node 11595*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11595, -6838, -21570, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11595;
DELETE FROM node_tags WHERE node_id = 11595;
DELETE FROM current_way_nodes WHERE node_id=11595;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11595;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11595;
/* delete node 11596*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11596, -6490, -9778, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11596;
DELETE FROM node_tags WHERE node_id = 11596;
DELETE FROM current_way_nodes WHERE node_id=11596;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11596;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11596;
/* delete node 11599*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11599, -6676, -20076, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11599;
DELETE FROM node_tags WHERE node_id = 11599;
DELETE FROM current_way_nodes WHERE node_id=11599;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11599;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11599;
/* delete node 11601*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11601, -6684, -21472, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11601;
DELETE FROM node_tags WHERE node_id = 11601;
DELETE FROM current_way_nodes WHERE node_id=11601;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11601;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11601;
/* delete node 11602*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11602, -6468, -8264, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11602;
DELETE FROM node_tags WHERE node_id = 11602;
DELETE FROM current_way_nodes WHERE node_id=11602;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11602;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11602;
/* delete node 11607*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11607, -6268, -8680, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11607;
DELETE FROM node_tags WHERE node_id = 11607;
DELETE FROM current_way_nodes WHERE node_id=11607;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11607;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11607;
/* delete node 11612*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11612, -6418, -21954, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11612;
DELETE FROM node_tags WHERE node_id = 11612;
DELETE FROM current_way_nodes WHERE node_id=11612;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11612;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11612;
/* delete node 11614*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11614, -6593, -30410, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11614;
DELETE FROM node_tags WHERE node_id = 11614;
DELETE FROM current_way_nodes WHERE node_id=11614;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11614;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11614;
/* delete node 11616*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11616, -5951, -9468, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11616;
DELETE FROM node_tags WHERE node_id = 11616;
DELETE FROM current_way_nodes WHERE node_id=11616;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11616;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11616;
/* delete node 11619*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11619, -5888, -9601, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11619;
DELETE FROM node_tags WHERE node_id = 11619;
DELETE FROM current_way_nodes WHERE node_id=11619;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11619;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11619;
/* delete node 11629*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11629, -5922, -21640, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11629;
DELETE FROM node_tags WHERE node_id = 11629;
DELETE FROM current_way_nodes WHERE node_id=11629;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11629;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11629;
/* delete node 11638*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11638, -5623, -8309, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11638;
DELETE FROM node_tags WHERE node_id = 11638;
DELETE FROM current_way_nodes WHERE node_id=11638;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11638;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11638;
/* delete node 11654*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11654, -5192, -9202, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11654;
DELETE FROM node_tags WHERE node_id = 11654;
DELETE FROM current_way_nodes WHERE node_id=11654;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11654;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11654;
/* delete node 11659*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11659, -5339, -20193, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11659;
DELETE FROM node_tags WHERE node_id = 11659;
DELETE FROM current_way_nodes WHERE node_id=11659;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11659;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11659;
/* delete node 11682*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11682, -4736, -12956, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11682;
DELETE FROM node_tags WHERE node_id = 11682;
DELETE FROM current_way_nodes WHERE node_id=11682;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11682;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11682;
/* delete node 11685*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11685, -4934, -20813, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11685;
DELETE FROM node_tags WHERE node_id = 11685;
DELETE FROM current_way_nodes WHERE node_id=11685;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11685;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11685;
/* delete node 11691*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11691, -5377, -1634, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11691;
DELETE FROM node_tags WHERE node_id = 11691;
DELETE FROM current_way_nodes WHERE node_id=11691;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11691;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11691;
/* delete node 11696*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11696, -4729, -19573, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11696;
DELETE FROM node_tags WHERE node_id = 11696;
DELETE FROM current_way_nodes WHERE node_id=11696;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11696;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11696;
/* delete node 11705*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11705, -4450, -13702, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11705;
DELETE FROM node_tags WHERE node_id = 11705;
DELETE FROM current_way_nodes WHERE node_id=11705;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11705;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11705;
/* delete node 11706*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11706, -4623, -19731, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11706;
DELETE FROM node_tags WHERE node_id = 11706;
DELETE FROM current_way_nodes WHERE node_id=11706;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11706;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11706;
/* delete node 11710*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11710, -4452, -24509, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11710;
DELETE FROM node_tags WHERE node_id = 11710;
DELETE FROM current_way_nodes WHERE node_id=11710;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11710;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11710;
/* delete node 11718*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11718, -4341, -20123, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11718;
DELETE FROM node_tags WHERE node_id = 11718;
DELETE FROM current_way_nodes WHERE node_id=11718;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11718;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11718;
/* delete node 11719*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11719, -4992, -993, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11719;
DELETE FROM node_tags WHERE node_id = 11719;
DELETE FROM current_way_nodes WHERE node_id=11719;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11719;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11719;
/* delete node 11724*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11724, -4309, -24841, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11724;
DELETE FROM node_tags WHERE node_id = 11724;
DELETE FROM current_way_nodes WHERE node_id=11724;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11724;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11724;
/* delete node 11736*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11736, -4165, -20386, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11736;
DELETE FROM node_tags WHERE node_id = 11736;
DELETE FROM current_way_nodes WHERE node_id=11736;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11736;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11736;
/* delete node 11741*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11741, -4081, -27136, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11741;
DELETE FROM node_tags WHERE node_id = 11741;
DELETE FROM current_way_nodes WHERE node_id=11741;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11741;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11741;
/* delete node 11753*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11753, -4682, -434, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11753;
DELETE FROM node_tags WHERE node_id = 11753;
DELETE FROM current_way_nodes WHERE node_id=11753;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11753;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11753;
/* delete node 11755*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11755, -3963, -24728, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11755;
DELETE FROM node_tags WHERE node_id = 11755;
DELETE FROM current_way_nodes WHERE node_id=11755;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11755;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11755;
/* delete node 11759*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11759, -3876, -19063, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11759;
DELETE FROM node_tags WHERE node_id = 11759;
DELETE FROM current_way_nodes WHERE node_id=11759;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11759;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11759;
/* delete node 11765*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11765, -3800, -25056, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11765;
DELETE FROM node_tags WHERE node_id = 11765;
DELETE FROM current_way_nodes WHERE node_id=11765;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11765;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11765;
/* delete node 11767*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11767, -4314, -2383, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11767;
DELETE FROM node_tags WHERE node_id = 11767;
DELETE FROM current_way_nodes WHERE node_id=11767;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11767;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11767;
/* delete node 11775*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11775, -3650, -27886, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11775;
DELETE FROM node_tags WHERE node_id = 11775;
DELETE FROM current_way_nodes WHERE node_id=11775;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11775;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11775;
/* delete node 11777*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11777, -4206, -1482, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11777;
DELETE FROM node_tags WHERE node_id = 11777;
DELETE FROM current_way_nodes WHERE node_id=11777;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11777;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11777;
/* delete node 11782*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11782, -3514, -7499, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11782;
DELETE FROM node_tags WHERE node_id = 11782;
DELETE FROM current_way_nodes WHERE node_id=11782;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11782;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11782;
/* delete node 11783*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11783, -3500, -19655, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11783;
DELETE FROM node_tags WHERE node_id = 11783;
DELETE FROM current_way_nodes WHERE node_id=11783;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11783;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11783;
/* delete node 11790*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11790, -3418, -26810, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11790;
DELETE FROM node_tags WHERE node_id = 11790;
DELETE FROM current_way_nodes WHERE node_id=11790;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11790;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11790;
/* delete node 11794*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11794, -3368, -27746, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11794;
DELETE FROM node_tags WHERE node_id = 11794;
DELETE FROM current_way_nodes WHERE node_id=11794;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11794;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11794;
/* delete node 11795*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11795, -3933, -1674, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11795;
DELETE FROM node_tags WHERE node_id = 11795;
DELETE FROM current_way_nodes WHERE node_id=11795;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11795;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11795;
/* delete node 11799*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11799, -3272, -24126, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11799;
DELETE FROM node_tags WHERE node_id = 11799;
DELETE FROM current_way_nodes WHERE node_id=11799;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11799;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11799;
/* delete node 11801*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11801, -3926, -962, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11801;
DELETE FROM node_tags WHERE node_id = 11801;
DELETE FROM current_way_nodes WHERE node_id=11801;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11801;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11801;
/* delete node 11802*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11802, -3238, -24207, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11802;
DELETE FROM node_tags WHERE node_id = 11802;
DELETE FROM current_way_nodes WHERE node_id=11802;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11802;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11802;
/* delete node 11806*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11806, -2987, -12295, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11806;
DELETE FROM node_tags WHERE node_id = 11806;
DELETE FROM current_way_nodes WHERE node_id=11806;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11806;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11806;
/* delete node 11809*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11809, -3172, -28095, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11809;
DELETE FROM node_tags WHERE node_id = 11809;
DELETE FROM current_way_nodes WHERE node_id=11809;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11809;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11809;
/* delete node 11812*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11812, -2998, -8512, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11812;
DELETE FROM node_tags WHERE node_id = 11812;
DELETE FROM current_way_nodes WHERE node_id=11812;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11812;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11812;
/* delete node 11817*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11817, -3124, -29038, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11817;
DELETE FROM node_tags WHERE node_id = 11817;
DELETE FROM current_way_nodes WHERE node_id=11817;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11817;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11817;
/* delete node 11818*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11818, -2985, -24791, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11818;
DELETE FROM node_tags WHERE node_id = 11818;
DELETE FROM current_way_nodes WHERE node_id=11818;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11818;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11818;
/* delete node 11819*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11819, -3019, -27516, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11819;
DELETE FROM node_tags WHERE node_id = 11819;
DELETE FROM current_way_nodes WHERE node_id=11819;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11819;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11819;
/* delete node 11824*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11824, -2947, -7247, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11824;
DELETE FROM node_tags WHERE node_id = 11824;
DELETE FROM current_way_nodes WHERE node_id=11824;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11824;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11824;
/* delete node 11825*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11825, -3107, -30004, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11825;
DELETE FROM node_tags WHERE node_id = 11825;
DELETE FROM current_way_nodes WHERE node_id=11825;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11825;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11825;
/* delete node 11826*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11826, -2701, -13041, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11826;
DELETE FROM node_tags WHERE node_id = 11826;
DELETE FROM current_way_nodes WHERE node_id=11826;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11826;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11826;
/* delete node 11830*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11830, -2829, -7476, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11830;
DELETE FROM node_tags WHERE node_id = 11830;
DELETE FROM current_way_nodes WHERE node_id=11830;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11830;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11830;
/* delete node 11833*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11833, -2735, -18831, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11833;
DELETE FROM node_tags WHERE node_id = 11833;
DELETE FROM current_way_nodes WHERE node_id=11833;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11833;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11833;
/* delete node 11834*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11834, -2778, -27396, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11834;
DELETE FROM node_tags WHERE node_id = 11834;
DELETE FROM current_way_nodes WHERE node_id=11834;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11834;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11834;
/* delete node 11840*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11840, -2862, -29931, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11840;
DELETE FROM node_tags WHERE node_id = 11840;
DELETE FROM current_way_nodes WHERE node_id=11840;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11840;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11840;
/* delete node 11845*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11845, -2865, -30830, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11845;
DELETE FROM node_tags WHERE node_id = 11845;
DELETE FROM current_way_nodes WHERE node_id=11845;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11845;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11845;
/* delete node 11847*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11847, -2558, -27789, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11847;
DELETE FROM node_tags WHERE node_id = 11847;
DELETE FROM current_way_nodes WHERE node_id=11847;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11847;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11847;
/* delete node 11848*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11848, -2456, -16329, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11848;
DELETE FROM node_tags WHERE node_id = 11848;
DELETE FROM current_way_nodes WHERE node_id=11848;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11848;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11848;
/* delete node 11850*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11850, -2478, -23948, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11850;
DELETE FROM node_tags WHERE node_id = 11850;
DELETE FROM current_way_nodes WHERE node_id=11850;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11850;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11850;
/* delete node 11854*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11854, -2360, -8236, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11854;
DELETE FROM node_tags WHERE node_id = 11854;
DELETE FROM current_way_nodes WHERE node_id=11854;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11854;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11854;
/* delete node 11856*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11856, -2492, -28851, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11856;
DELETE FROM node_tags WHERE node_id = 11856;
DELETE FROM current_way_nodes WHERE node_id=11856;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11856;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11856;
/* delete node 11858*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11858, -2288, -8384, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11858;
DELETE FROM node_tags WHERE node_id = 11858;
DELETE FROM current_way_nodes WHERE node_id=11858;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11858;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11858;
/* delete node 11861*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11861, -2318, -18654, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11861;
DELETE FROM node_tags WHERE node_id = 11861;
DELETE FROM current_way_nodes WHERE node_id=11861;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11861;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11861;
/* delete node 11863*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11863, -2303, -19732, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11863;
DELETE FROM node_tags WHERE node_id = 11863;
DELETE FROM current_way_nodes WHERE node_id=11863;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11863;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11863;
/* delete node 11865*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11865, -2247, -16661, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11865;
DELETE FROM node_tags WHERE node_id = 11865;
DELETE FROM current_way_nodes WHERE node_id=11865;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11865;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11865;
/* delete node 11866*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11866, -2261, -24510, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11866;
DELETE FROM node_tags WHERE node_id = 11866;
DELETE FROM current_way_nodes WHERE node_id=11866;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11866;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11866;
/* delete node 11869*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11869, -2461, -29785, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11869;
DELETE FROM node_tags WHERE node_id = 11869;
DELETE FROM current_way_nodes WHERE node_id=11869;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11869;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11869;
/* delete node 11873*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11873, -2153, -19000, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11873;
DELETE FROM node_tags WHERE node_id = 11873;
DELETE FROM current_way_nodes WHERE node_id=11873;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11873;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11873;
/* delete node 11877*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11877, -2041, -16055, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11877;
DELETE FROM node_tags WHERE node_id = 11877;
DELETE FROM current_way_nodes WHERE node_id=11877;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11877;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11877;
/* delete node 11880*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11880, -2238, -29719, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11880;
DELETE FROM node_tags WHERE node_id = 11880;
DELETE FROM current_way_nodes WHERE node_id=11880;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11880;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11880;
/* delete node 11887*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11887, -2211, -30636, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11887;
DELETE FROM node_tags WHERE node_id = 11887;
DELETE FROM current_way_nodes WHERE node_id=11887;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11887;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11887;
/* delete node 11890*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11890, -1833, -16386, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11890;
DELETE FROM node_tags WHERE node_id = 11890;
DELETE FROM current_way_nodes WHERE node_id=11890;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11890;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11890;
/* delete node 11893*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11893, -1788, -19514, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11893;
DELETE FROM node_tags WHERE node_id = 11893;
DELETE FROM current_way_nodes WHERE node_id=11893;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11893;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11893;
/* delete node 11895*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11895, -1847, -7041, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11895;
DELETE FROM node_tags WHERE node_id = 11895;
DELETE FROM current_way_nodes WHERE node_id=11895;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11895;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11895;
/* delete node 11901*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11901, -1682, -19736, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11901;
DELETE FROM node_tags WHERE node_id = 11901;
DELETE FROM current_way_nodes WHERE node_id=11901;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11901;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11901;
/* delete node 11909*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11909, -1327, -11576, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11909;
DELETE FROM node_tags WHERE node_id = 11909;
DELETE FROM current_way_nodes WHERE node_id=11909;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11909;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11909;
/* delete node 11917*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11917, -1389, -7996, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11917;
DELETE FROM node_tags WHERE node_id = 11917;
DELETE FROM current_way_nodes WHERE node_id=11917;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11917;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11917;
/* delete node 11923*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11923, -1120, -11262, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11923;
DELETE FROM node_tags WHERE node_id = 11923;
DELETE FROM current_way_nodes WHERE node_id=11923;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11923;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11923;
/* delete node 11924*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11924, -1273, -18628, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11924;
DELETE FROM node_tags WHERE node_id = 11924;
DELETE FROM current_way_nodes WHERE node_id=11924;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11924;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11924;
/* delete node 11926*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11926, -1119, -12070, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11926;
DELETE FROM node_tags WHERE node_id = 11926;
DELETE FROM current_way_nodes WHERE node_id=11926;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11926;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11926;
/* delete node 11930*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11930, -1039, -11455, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11930;
DELETE FROM node_tags WHERE node_id = 11930;
DELETE FROM current_way_nodes WHERE node_id=11930;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11930;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11930;
/* delete node 11945*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11945, -818, -11942, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11945;
DELETE FROM node_tags WHERE node_id = 11945;
DELETE FROM current_way_nodes WHERE node_id=11945;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11945;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11945;
/* delete node 11946*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11946, -759, -10881, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11946;
DELETE FROM node_tags WHERE node_id = 11946;
DELETE FROM current_way_nodes WHERE node_id=11946;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11946;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11946;
/* delete node 11947*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11947, -901, -19406, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11947;
DELETE FROM node_tags WHERE node_id = 11947;
DELETE FROM current_way_nodes WHERE node_id=11947;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11947;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11947;
/* delete node 11948*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11948, -775, -12045, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11948;
DELETE FROM node_tags WHERE node_id = 11948;
DELETE FROM current_way_nodes WHERE node_id=11948;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11948;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11948;
/* delete node 11953*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11953, -678, -11075, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11953;
DELETE FROM node_tags WHERE node_id = 11953;
DELETE FROM current_way_nodes WHERE node_id=11953;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11953;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11953;
/* delete node 11955*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11955, -621, -10822, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11955;
DELETE FROM node_tags WHERE node_id = 11955;
DELETE FROM current_way_nodes WHERE node_id=11955;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11955;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11955;
/* delete node 11957*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11957, -762, -23471, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11957;
DELETE FROM node_tags WHERE node_id = 11957;
DELETE FROM current_way_nodes WHERE node_id=11957;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11957;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11957;
/* delete node 11960*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11960, -535, -11028, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11960;
DELETE FROM node_tags WHERE node_id = 11960;
DELETE FROM current_way_nodes WHERE node_id=11960;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11960;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11960;
/* delete node 11964*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11964, -1082, -31622, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11964;
DELETE FROM node_tags WHERE node_id = 11964;
DELETE FROM current_way_nodes WHERE node_id=11964;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11964;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11964;
/* delete node 11981*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11981, -381, -24185, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11981;
DELETE FROM node_tags WHERE node_id = 11981;
DELETE FROM current_way_nodes WHERE node_id=11981;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11981;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11981;
/* delete node 11982*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11982, -336, -18179, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11982;
DELETE FROM node_tags WHERE node_id = 11982;
DELETE FROM current_way_nodes WHERE node_id=11982;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11982;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11982;
/* delete node 11992*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11992, -236, -17652, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11992;
DELETE FROM node_tags WHERE node_id = 11992;
DELETE FROM current_way_nodes WHERE node_id=11992;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11992;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11992;
/* delete node 11993*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (11993, -796, -32473, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 11993;
DELETE FROM node_tags WHERE node_id = 11993;
DELETE FROM current_way_nodes WHERE node_id=11993;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 11993;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=11993;
/* delete node 12001*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12001, 59, -10777, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12001;
DELETE FROM node_tags WHERE node_id = 12001;
DELETE FROM current_way_nodes WHERE node_id=12001;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12001;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12001;
/* delete node 12004*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12004, -876, -359, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 12004;
DELETE FROM node_tags WHERE node_id = 12004;
DELETE FROM current_way_nodes WHERE node_id=12004;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12004;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12004;
/* delete node 12006*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12006, -765, -1298, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 12006;
DELETE FROM node_tags WHERE node_id = 12006;
DELETE FROM current_way_nodes WHERE node_id=12006;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12006;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12006;
/* delete node 12007*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12007, -17, -17968, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 12007;
DELETE FROM node_tags WHERE node_id = 12007;
DELETE FROM current_way_nodes WHERE node_id=12007;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12007;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12007;
/* delete node 12010*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12010, 66, -18762, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12010;
DELETE FROM node_tags WHERE node_id = 12010;
DELETE FROM current_way_nodes WHERE node_id=12010;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12010;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12010;
/* delete node 12012*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12012, 80, -17441, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12012;
DELETE FROM node_tags WHERE node_id = 12012;
DELETE FROM current_way_nodes WHERE node_id=12012;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12012;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12012;
/* delete node 12018*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12018, 155, -23083, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12018;
DELETE FROM node_tags WHERE node_id = 12018;
DELETE FROM current_way_nodes WHERE node_id=12018;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12018;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12018;
/* delete node 12019*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12019, -516, -1853, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 12019;
DELETE FROM node_tags WHERE node_id = 12019;
DELETE FROM current_way_nodes WHERE node_id=12019;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12019;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12019;
/* delete node 12021*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12021, 232, -17660, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12021;
DELETE FROM node_tags WHERE node_id = 12021;
DELETE FROM current_way_nodes WHERE node_id=12021;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12021;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12021;
/* delete node 12024*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12024, -505, -1204, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 12024;
DELETE FROM node_tags WHERE node_id = 12024;
DELETE FROM current_way_nodes WHERE node_id=12024;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12024;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12024;
/* delete node 12027*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12027, 386, -11556, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12027;
DELETE FROM node_tags WHERE node_id = 12027;
DELETE FROM current_way_nodes WHERE node_id=12027;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12027;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12027;
/* delete node 12031*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12031, 311, -23375, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12031;
DELETE FROM node_tags WHERE node_id = 12031;
DELETE FROM current_way_nodes WHERE node_id=12031;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12031;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12031;
/* delete node 12032*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12032, 327, -17596, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12032;
DELETE FROM node_tags WHERE node_id = 12032;
DELETE FROM current_way_nodes WHERE node_id=12032;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12032;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12032;
/* delete node 12034*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12034, 85, -5972, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12034;
DELETE FROM node_tags WHERE node_id = 12034;
DELETE FROM current_way_nodes WHERE node_id=12034;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12034;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12034;
/* delete node 12036*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12036, 400, -17700, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12036;
DELETE FROM node_tags WHERE node_id = 12036;
DELETE FROM current_way_nodes WHERE node_id=12036;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12036;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12036;
/* delete node 12044*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12044, -248, -1757, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 12044;
DELETE FROM node_tags WHERE node_id = 12044;
DELETE FROM current_way_nodes WHERE node_id=12044;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12044;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12044;
/* delete node 12046*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12046, 482, -23137, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12046;
DELETE FROM node_tags WHERE node_id = 12046;
DELETE FROM current_way_nodes WHERE node_id=12046;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12046;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12046;
/* delete node 12050*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12050, 555, -23272, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12050;
DELETE FROM node_tags WHERE node_id = 12050;
DELETE FROM current_way_nodes WHERE node_id=12050;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12050;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12050;
/* delete node 12051*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12051, -100, -2085, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 12051;
DELETE FROM node_tags WHERE node_id = 12051;
DELETE FROM current_way_nodes WHERE node_id=12051;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12051;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12051;
/* delete node 12052*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12052, -251, -146, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 12052;
DELETE FROM node_tags WHERE node_id = 12052;
DELETE FROM current_way_nodes WHERE node_id=12052;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12052;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12052;
/* delete node 12058*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12058, 658, -17527, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12058;
DELETE FROM node_tags WHERE node_id = 12058;
DELETE FROM current_way_nodes WHERE node_id=12058;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12058;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12058;
/* delete node 12068*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12068, 229, -31910, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12068;
DELETE FROM node_tags WHERE node_id = 12068;
DELETE FROM current_way_nodes WHERE node_id=12068;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12068;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12068;
/* delete node 12074*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12074, -5, -716, 2, false, (now() at time zone 'utc'), 1073741823, 2);
DELETE FROM current_node_tags WHERE node_id = 12074;
DELETE FROM node_tags WHERE node_id = 12074;
DELETE FROM current_way_nodes WHERE node_id=12074;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12074;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12074;
/* delete node 12077*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12077, 843, -18245, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12077;
DELETE FROM node_tags WHERE node_id = 12077;
DELETE FROM current_way_nodes WHERE node_id=12077;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12077;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12077;
/* delete node 12079*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12079, 295, -32106, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12079;
DELETE FROM node_tags WHERE node_id = 12079;
DELETE FROM current_way_nodes WHERE node_id=12079;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12079;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12079;
/* delete node 12080*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12080, 850, -17807, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12080;
DELETE FROM node_tags WHERE node_id = 12080;
DELETE FROM current_way_nodes WHERE node_id=12080;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12080;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12080;
/* delete node 12092*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12092, 584, -31063, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12092;
DELETE FROM node_tags WHERE node_id = 12092;
DELETE FROM current_way_nodes WHERE node_id=12092;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12092;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12092;
/* delete node 12093*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12093, 1028, -22906, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12093;
DELETE FROM node_tags WHERE node_id = 12093;
DELETE FROM current_way_nodes WHERE node_id=12093;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12093;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12093;
/* delete node 12094*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12094, 202, -645, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12094;
DELETE FROM node_tags WHERE node_id = 12094;
DELETE FROM current_way_nodes WHERE node_id=12094;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12094;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12094;
/* delete node 12096*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12096, 1058, -18557, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12096;
DELETE FROM node_tags WHERE node_id = 12096;
DELETE FROM current_way_nodes WHERE node_id=12096;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12096;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12096;
/* delete node 12105*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12105, 957, -6701, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12105;
DELETE FROM node_tags WHERE node_id = 12105;
DELETE FROM current_way_nodes WHERE node_id=12105;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12105;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12105;
/* delete node 12107*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12107, 1255, -18426, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12107;
DELETE FROM node_tags WHERE node_id = 12107;
DELETE FROM current_way_nodes WHERE node_id=12107;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12107;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12107;
/* delete node 12110*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12110, 864, -4699, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12110;
DELETE FROM node_tags WHERE node_id = 12110;
DELETE FROM current_way_nodes WHERE node_id=12110;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12110;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12110;
/* delete node 12113*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12113, 1317, -18514, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12113;
DELETE FROM node_tags WHERE node_id = 12113;
DELETE FROM current_way_nodes WHERE node_id=12113;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12113;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12113;
/* delete node 12114*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12114, 804, -31716, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12114;
DELETE FROM node_tags WHERE node_id = 12114;
DELETE FROM current_way_nodes WHERE node_id=12114;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12114;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12114;
/* delete node 12115*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12115, 1306, -23458, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12115;
DELETE FROM node_tags WHERE node_id = 12115;
DELETE FROM current_way_nodes WHERE node_id=12115;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12115;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12115;
/* delete node 12120*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12120, 713, -1791, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12120;
DELETE FROM node_tags WHERE node_id = 12120;
DELETE FROM current_way_nodes WHERE node_id=12120;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12120;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12120;
/* delete node 12121*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12121, 1463, -17399, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12121;
DELETE FROM node_tags WHERE node_id = 12121;
DELETE FROM current_way_nodes WHERE node_id=12121;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12121;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12121;
/* delete node 12134*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12134, 1721, -18125, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12134;
DELETE FROM node_tags WHERE node_id = 12134;
DELETE FROM current_way_nodes WHERE node_id=12134;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12134;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12134;
/* delete node 12136*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12136, 1777, -18208, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12136;
DELETE FROM node_tags WHERE node_id = 12136;
DELETE FROM current_way_nodes WHERE node_id=12136;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12136;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12136;
/* delete node 12137*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12137, 1798, -21908, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12137;
DELETE FROM node_tags WHERE node_id = 12137;
DELETE FROM current_way_nodes WHERE node_id=12137;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12137;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12137;
/* delete node 12140*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12140, 1475, -5796, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12140;
DELETE FROM node_tags WHERE node_id = 12140;
DELETE FROM current_way_nodes WHERE node_id=12140;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12140;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12140;
/* delete node 12141*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12141, 1888, -18014, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12141;
DELETE FROM node_tags WHERE node_id = 12141;
DELETE FROM current_way_nodes WHERE node_id=12141;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12141;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12141;
/* delete node 12143*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12143, 1564, -5880, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12143;
DELETE FROM node_tags WHERE node_id = 12143;
DELETE FROM current_way_nodes WHERE node_id=12143;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12143;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12143;
/* delete node 12148*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12148, 1689, -5494, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12148;
DELETE FROM node_tags WHERE node_id = 12148;
DELETE FROM current_way_nodes WHERE node_id=12148;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12148;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12148;
/* delete node 12152*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12152, 2167, -22896, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12152;
DELETE FROM node_tags WHERE node_id = 12152;
DELETE FROM current_way_nodes WHERE node_id=12152;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12152;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12152;
/* delete node 12158*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12158, 2319, -15530, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12158;
DELETE FROM node_tags WHERE node_id = 12158;
DELETE FROM current_way_nodes WHERE node_id=12158;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12158;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12158;
/* delete node 12163*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12163, 2445, -16446, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12163;
DELETE FROM node_tags WHERE node_id = 12163;
DELETE FROM current_way_nodes WHERE node_id=12163;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12163;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12163;
/* delete node 12169*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12169, 2142, -30533, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12169;
DELETE FROM node_tags WHERE node_id = 12169;
DELETE FROM current_way_nodes WHERE node_id=12169;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12169;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12169;
/* delete node 12174*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12174, 2654, -21662, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12174;
DELETE FROM node_tags WHERE node_id = 12174;
DELETE FROM current_way_nodes WHERE node_id=12174;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12174;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12174;
/* delete node 12177*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12177, 2614, -24242, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12177;
DELETE FROM node_tags WHERE node_id = 12177;
DELETE FROM current_way_nodes WHERE node_id=12177;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12177;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12177;
/* delete node 12178*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12178, 2728, -16311, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12178;
DELETE FROM node_tags WHERE node_id = 12178;
DELETE FROM current_way_nodes WHERE node_id=12178;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12178;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12178;
/* delete node 12182*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12182, 2702, -24561, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12182;
DELETE FROM node_tags WHERE node_id = 12182;
DELETE FROM current_way_nodes WHERE node_id=12182;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12182;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12182;
/* delete node 12183*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12183, 2804, -22076, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12183;
DELETE FROM node_tags WHERE node_id = 12183;
DELETE FROM current_way_nodes WHERE node_id=12183;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12183;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12183;
/* delete node 12184*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12184, 2788, -22711, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12184;
DELETE FROM node_tags WHERE node_id = 12184;
DELETE FROM current_way_nodes WHERE node_id=12184;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12184;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12184;
/* delete node 12186*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12186, 2831, -22834, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12186;
DELETE FROM node_tags WHERE node_id = 12186;
DELETE FROM current_way_nodes WHERE node_id=12186;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12186;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12186;
/* delete node 12188*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12188, 2484, -30355, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12188;
DELETE FROM node_tags WHERE node_id = 12188;
DELETE FROM current_way_nodes WHERE node_id=12188;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12188;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12188;
/* delete node 12191*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12191, 2938, -15236, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12191;
DELETE FROM node_tags WHERE node_id = 12191;
DELETE FROM current_way_nodes WHERE node_id=12191;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12191;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12191;
/* delete node 12193*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12193, 2945, -16235, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12193;
DELETE FROM node_tags WHERE node_id = 12193;
DELETE FROM current_way_nodes WHERE node_id=12193;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12193;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12193;
/* delete node 12197*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12197, 2984, -17475, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12197;
DELETE FROM node_tags WHERE node_id = 12197;
DELETE FROM current_way_nodes WHERE node_id=12197;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12197;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12197;
/* delete node 12198*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12198, 2567, -30485, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12198;
DELETE FROM node_tags WHERE node_id = 12198;
DELETE FROM current_way_nodes WHERE node_id=12198;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12198;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12198;
/* delete node 12200*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12200, 2551, -31188, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12200;
DELETE FROM node_tags WHERE node_id = 12200;
DELETE FROM current_way_nodes WHERE node_id=12200;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12200;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12200;
/* delete node 12202*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12202, 3051, -21879, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12202;
DELETE FROM node_tags WHERE node_id = 12202;
DELETE FROM current_way_nodes WHERE node_id=12202;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12202;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12202;
/* delete node 12206*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12206, 2734, -29994, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12206;
DELETE FROM node_tags WHERE node_id = 12206;
DELETE FROM current_way_nodes WHERE node_id=12206;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12206;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12206;
/* delete node 12207*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12207, 3091, -21991, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12207;
DELETE FROM node_tags WHERE node_id = 12207;
DELETE FROM current_way_nodes WHERE node_id=12207;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12207;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12207;
/* delete node 12208*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12208, 3037, -24147, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12208;
DELETE FROM node_tags WHERE node_id = 12208;
DELETE FROM current_way_nodes WHERE node_id=12208;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12208;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12208;
/* delete node 12212*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12212, 3177, -16677, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12212;
DELETE FROM node_tags WHERE node_id = 12212;
DELETE FROM current_way_nodes WHERE node_id=12212;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12212;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12212;
/* delete node 12215*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12215, 3126, -24466, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12215;
DELETE FROM node_tags WHERE node_id = 12215;
DELETE FROM current_way_nodes WHERE node_id=12215;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12215;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12215;
/* delete node 12220*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12220, 3290, -16622, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12220;
DELETE FROM node_tags WHERE node_id = 12220;
DELETE FROM current_way_nodes WHERE node_id=12220;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12220;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12220;
/* delete node 12221*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12221, 3254, -22471, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12221;
DELETE FROM node_tags WHERE node_id = 12221;
DELETE FROM current_way_nodes WHERE node_id=12221;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12221;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12221;
/* delete node 12222*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12222, 2917, -30301, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12222;
DELETE FROM node_tags WHERE node_id = 12222;
DELETE FROM current_way_nodes WHERE node_id=12222;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12222;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12222;
/* delete node 12227*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12227, 3359, -16039, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12227;
DELETE FROM node_tags WHERE node_id = 12227;
DELETE FROM current_way_nodes WHERE node_id=12227;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12227;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12227;
/* delete node 12228*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12228, 3330, -22686, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12228;
DELETE FROM node_tags WHERE node_id = 12228;
DELETE FROM current_way_nodes WHERE node_id=12228;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12228;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12228;
/* delete node 12232*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12232, 2998, -30764, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12232;
DELETE FROM node_tags WHERE node_id = 12232;
DELETE FROM current_way_nodes WHERE node_id=12232;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12232;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12232;
/* delete node 12239*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12239, 3088, -30907, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12239;
DELETE FROM node_tags WHERE node_id = 12239;
DELETE FROM current_way_nodes WHERE node_id=12239;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12239;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12239;
/* delete node 12240*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12240, 3111, -30705, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12240;
DELETE FROM node_tags WHERE node_id = 12240;
DELETE FROM current_way_nodes WHERE node_id=12240;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12240;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12240;
/* delete node 12241*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12241, 3587, -17189, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12241;
DELETE FROM node_tags WHERE node_id = 12241;
DELETE FROM current_way_nodes WHERE node_id=12241;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12241;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12241;
/* delete node 12244*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12244, 3620, -21710, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12244;
DELETE FROM node_tags WHERE node_id = 12244;
DELETE FROM current_way_nodes WHERE node_id=12244;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12244;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12244;
/* delete node 12246*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12246, 3220, -30877, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12246;
DELETE FROM node_tags WHERE node_id = 12246;
DELETE FROM current_way_nodes WHERE node_id=12246;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12246;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12246;
/* delete node 12254*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12254, 3510, -29592, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12254;
DELETE FROM node_tags WHERE node_id = 12254;
DELETE FROM current_way_nodes WHERE node_id=12254;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12254;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12254;
/* delete node 12255*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12255, 3830, -22300, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12255;
DELETE FROM node_tags WHERE node_id = 12255;
DELETE FROM current_way_nodes WHERE node_id=12255;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12255;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12255;
/* delete node 12265*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12265, 3690, -29877, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12265;
DELETE FROM node_tags WHERE node_id = 12265;
DELETE FROM current_way_nodes WHERE node_id=12265;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12265;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12265;
/* delete node 12269*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12269, 3666, -30643, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12269;
DELETE FROM node_tags WHERE node_id = 12269;
DELETE FROM current_way_nodes WHERE node_id=12269;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12269;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12269;
/* delete node 12273*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12273, 3711, -30715, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12273;
DELETE FROM node_tags WHERE node_id = 12273;
DELETE FROM current_way_nodes WHERE node_id=12273;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12273;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12273;
/* delete node 12275*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12275, 4139, -21086, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12275;
DELETE FROM node_tags WHERE node_id = 12275;
DELETE FROM current_way_nodes WHERE node_id=12275;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12275;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12275;
/* delete node 12282*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12282, 3954, -29734, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12282;
DELETE FROM node_tags WHERE node_id = 12282;
DELETE FROM current_way_nodes WHERE node_id=12282;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12282;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12282;
/* delete node 12288*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12288, 4337, -20843, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12288;
DELETE FROM node_tags WHERE node_id = 12288;
DELETE FROM current_way_nodes WHERE node_id=12288;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12288;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12288;
/* delete node 12293*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12293, 4334, -21612, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12293;
DELETE FROM node_tags WHERE node_id = 12293;
DELETE FROM current_way_nodes WHERE node_id=12293;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12293;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12293;
/* delete node 12296*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12296, 4290, -9994, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12296;
DELETE FROM node_tags WHERE node_id = 12296;
DELETE FROM current_way_nodes WHERE node_id=12296;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12296;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12296;
/* delete node 12298*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12298, 4391, -20987, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12298;
DELETE FROM node_tags WHERE node_id = 12298;
DELETE FROM current_way_nodes WHERE node_id=12298;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12298;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12298;
/* delete node 12314*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12314, 4337, -30332, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12314;
DELETE FROM node_tags WHERE node_id = 12314;
DELETE FROM current_way_nodes WHERE node_id=12314;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12314;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12314;
/* delete node 12332*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12332, 4804, -8397, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12332;
DELETE FROM node_tags WHERE node_id = 12332;
DELETE FROM current_way_nodes WHERE node_id=12332;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12332;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12332;
/* delete node 12336*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12336, 5135, -20468, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12336;
DELETE FROM node_tags WHERE node_id = 12336;
DELETE FROM current_way_nodes WHERE node_id=12336;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12336;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12336;
/* delete node 12338*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12338, 5165, -20550, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12338;
DELETE FROM node_tags WHERE node_id = 12338;
DELETE FROM current_way_nodes WHERE node_id=12338;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12338;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12338;
/* delete node 12341*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12341, 5242, -13529, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12341;
DELETE FROM node_tags WHERE node_id = 12341;
DELETE FROM current_way_nodes WHERE node_id=12341;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12341;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12341;
/* delete node 12345*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12345, 5078, -29241, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12345;
DELETE FROM node_tags WHERE node_id = 12345;
DELETE FROM current_way_nodes WHERE node_id=12345;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12345;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12345;
/* delete node 12347*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12347, 4653, -3034, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12347;
DELETE FROM node_tags WHERE node_id = 12347;
DELETE FROM current_way_nodes WHERE node_id=12347;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12347;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12347;
/* delete node 12351*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12351, 4766, -3634, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12351;
DELETE FROM node_tags WHERE node_id = 12351;
DELETE FROM current_way_nodes WHERE node_id=12351;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12351;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12351;
/* delete node 12356*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12356, 5369, -9928, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12356;
DELETE FROM node_tags WHERE node_id = 12356;
DELETE FROM current_way_nodes WHERE node_id=12356;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12356;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12356;
/* delete node 12361*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12361, 5504, -10587, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12361;
DELETE FROM node_tags WHERE node_id = 12361;
DELETE FROM current_way_nodes WHERE node_id=12361;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12361;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12361;
/* delete node 12362*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12362, 4874, -3579, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12362;
DELETE FROM node_tags WHERE node_id = 12362;
DELETE FROM current_way_nodes WHERE node_id=12362;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12362;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12362;
/* delete node 12368*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12368, 5643, -14347, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12368;
DELETE FROM node_tags WHERE node_id = 12368;
DELETE FROM current_way_nodes WHERE node_id=12368;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12368;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12368;
/* delete node 12369*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12369, 5350, -29675, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12369;
DELETE FROM node_tags WHERE node_id = 12369;
DELETE FROM current_way_nodes WHERE node_id=12369;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12369;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12369;
/* delete node 12373*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12373, 5734, -20284, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12373;
DELETE FROM node_tags WHERE node_id = 12373;
DELETE FROM current_way_nodes WHERE node_id=12373;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12373;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12373;
/* delete node 12377*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12377, 5666, -10074, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12377;
DELETE FROM node_tags WHERE node_id = 12377;
DELETE FROM current_way_nodes WHERE node_id=12377;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12377;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12377;
/* delete node 12382*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12382, 5332, -5035, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12382;
DELETE FROM node_tags WHERE node_id = 12382;
DELETE FROM current_way_nodes WHERE node_id=12382;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12382;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12382;
/* delete node 12384*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12384, 5697, -8889, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12384;
DELETE FROM node_tags WHERE node_id = 12384;
DELETE FROM current_way_nodes WHERE node_id=12384;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12384;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12384;
/* delete node 12386*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12386, 5908, -13105, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12386;
DELETE FROM node_tags WHERE node_id = 12386;
DELETE FROM current_way_nodes WHERE node_id=12386;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12386;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12386;
/* delete node 12387*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12387, 5170, -2772, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12387;
DELETE FROM node_tags WHERE node_id = 12387;
DELETE FROM current_way_nodes WHERE node_id=12387;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12387;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12387;
/* delete node 12388*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12388, 5647, -29490, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12388;
DELETE FROM node_tags WHERE node_id = 12388;
DELETE FROM current_way_nodes WHERE node_id=12388;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12388;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12388;
/* delete node 12389*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12389, 5919, -14171, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12389;
DELETE FROM node_tags WHERE node_id = 12389;
DELETE FROM current_way_nodes WHERE node_id=12389;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12389;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12389;
/* delete node 12390*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12390, 5949, -20855, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12390;
DELETE FROM node_tags WHERE node_id = 12390;
DELETE FROM current_way_nodes WHERE node_id=12390;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12390;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12390;
/* delete node 12393*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12393, 5964, -14562, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12393;
DELETE FROM node_tags WHERE node_id = 12393;
DELETE FROM current_way_nodes WHERE node_id=12393;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12393;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12393;
/* delete node 12397*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12397, 5748, -29652, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12397;
DELETE FROM node_tags WHERE node_id = 12397;
DELETE FROM current_way_nodes WHERE node_id=12397;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12397;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12397;
/* delete node 12401*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12401, 6076, -14491, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12401;
DELETE FROM node_tags WHERE node_id = 12401;
DELETE FROM current_way_nodes WHERE node_id=12401;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12401;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12401;
/* delete node 12402*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12402, 5384, -3301, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12402;
DELETE FROM node_tags WHERE node_id = 12402;
DELETE FROM current_way_nodes WHERE node_id=12402;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12402;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12402;
/* delete node 12405*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12405, 5702, -32582, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12405;
DELETE FROM node_tags WHERE node_id = 12405;
DELETE FROM current_way_nodes WHERE node_id=12405;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12405;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12405;
/* delete node 12408*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12408, 5621, -4286, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12408;
DELETE FROM node_tags WHERE node_id = 12408;
DELETE FROM current_way_nodes WHERE node_id=12408;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12408;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12408;
/* delete node 12409*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12409, 6231, -15107, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12409;
DELETE FROM node_tags WHERE node_id = 12409;
DELETE FROM current_way_nodes WHERE node_id=12409;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12409;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12409;
/* delete node 12410*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12410, 6233, -14010, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12410;
DELETE FROM node_tags WHERE node_id = 12410;
DELETE FROM current_way_nodes WHERE node_id=12410;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12410;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12410;
/* delete node 12413*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12413, 6085, -28573, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12413;
DELETE FROM node_tags WHERE node_id = 12413;
DELETE FROM current_way_nodes WHERE node_id=12413;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12413;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12413;
/* delete node 12414*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12414, 6323, -13953, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12414;
DELETE FROM node_tags WHERE node_id = 12414;
DELETE FROM current_way_nodes WHERE node_id=12414;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12414;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12414;
/* delete node 12418*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12418, 6370, -14292, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12418;
DELETE FROM node_tags WHERE node_id = 12418;
DELETE FROM current_way_nodes WHERE node_id=12418;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12418;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12418;
/* delete node 12420*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12420, 5822, -4785, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12420;
DELETE FROM node_tags WHERE node_id = 12420;
DELETE FROM current_way_nodes WHERE node_id=12420;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12420;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12420;
/* delete node 12429*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12429, 5790, -3114, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12429;
DELETE FROM node_tags WHERE node_id = 12429;
DELETE FROM current_way_nodes WHERE node_id=12429;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12429;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12429;
/* delete node 12430*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12430, 5975, -33150, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12430;
DELETE FROM node_tags WHERE node_id = 12430;
DELETE FROM current_way_nodes WHERE node_id=12430;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12430;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12430;
/* delete node 12432*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12432, 6528, -14192, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12432;
DELETE FROM node_tags WHERE node_id = 12432;
DELETE FROM current_way_nodes WHERE node_id=12432;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12432;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12432;
/* delete node 12440*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12440, 6484, -29193, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12440;
DELETE FROM node_tags WHERE node_id = 12440;
DELETE FROM current_way_nodes WHERE node_id=12440;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12440;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12440;
/* delete node 12446*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12446, 6152, -4014, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12446;
DELETE FROM node_tags WHERE node_id = 12446;
DELETE FROM current_way_nodes WHERE node_id=12446;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12446;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12446;
/* delete node 12448*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12448, 6799, -14746, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12448;
DELETE FROM node_tags WHERE node_id = 12448;
DELETE FROM current_way_nodes WHERE node_id=12448;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12448;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12448;
/* delete node 12451*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12451, 6894, -20984, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12451;
DELETE FROM node_tags WHERE node_id = 12451;
DELETE FROM current_way_nodes WHERE node_id=12451;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12451;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12451;
/* delete node 12466*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12466, 6715, -32057, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12466;
DELETE FROM node_tags WHERE node_id = 12466;
DELETE FROM current_way_nodes WHERE node_id=12466;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12466;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12466;
/* delete node 12478*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12478, 7312, -20550, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12478;
DELETE FROM node_tags WHERE node_id = 12478;
DELETE FROM current_way_nodes WHERE node_id=12478;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12478;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12478;
/* delete node 12480*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12480, 7313, -21818, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12480;
DELETE FROM node_tags WHERE node_id = 12480;
DELETE FROM current_way_nodes WHERE node_id=12480;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12480;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12480;
/* delete node 12482*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12482, 7385, -20695, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12482;
DELETE FROM node_tags WHERE node_id = 12482;
DELETE FROM current_way_nodes WHERE node_id=12482;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12482;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12482;
/* delete node 12487*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12487, 6987, -32625, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12487;
DELETE FROM node_tags WHERE node_id = 12487;
DELETE FROM current_way_nodes WHERE node_id=12487;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12487;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12487;
/* delete node 12494*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12494, 7683, -20137, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12494;
DELETE FROM node_tags WHERE node_id = 12494;
DELETE FROM current_way_nodes WHERE node_id=12494;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12494;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12494;
/* delete node 12498*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12498, 7696, -21211, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12498;
DELETE FROM node_tags WHERE node_id = 12498;
DELETE FROM current_way_nodes WHERE node_id=12498;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12498;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12498;
/* delete node 12499*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12499, 7760, -20288, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12499;
DELETE FROM node_tags WHERE node_id = 12499;
DELETE FROM current_way_nodes WHERE node_id=12499;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12499;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12499;
/* delete node 12503*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12503, 7845, -21506, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12503;
DELETE FROM node_tags WHERE node_id = 12503;
DELETE FROM current_way_nodes WHERE node_id=12503;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12503;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12503;
/* delete node 12505*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12505, 7892, -14190, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12505;
DELETE FROM node_tags WHERE node_id = 12505;
DELETE FROM current_way_nodes WHERE node_id=12505;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12505;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12505;
/* delete node 12512*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12512, 7589, -31685, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12512;
DELETE FROM node_tags WHERE node_id = 12512;
DELETE FROM current_way_nodes WHERE node_id=12512;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12512;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12512;
/* delete node 12517*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12517, 7889, -27624, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12517;
DELETE FROM node_tags WHERE node_id = 12517;
DELETE FROM current_way_nodes WHERE node_id=12517;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12517;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12517;
/* delete node 12524*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12524, 8230, -19793, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12524;
DELETE FROM node_tags WHERE node_id = 12524;
DELETE FROM current_way_nodes WHERE node_id=12524;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12524;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12524;
/* delete node 12528*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12528, 7821, -32207, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12528;
DELETE FROM node_tags WHERE node_id = 12528;
DELETE FROM current_way_nodes WHERE node_id=12528;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12528;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12528;
/* delete node 12530*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12530, 8107, -28054, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12530;
DELETE FROM node_tags WHERE node_id = 12530;
DELETE FROM current_way_nodes WHERE node_id=12530;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12530;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12530;
/* delete node 12533*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12533, 8275, -14829, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12533;
DELETE FROM node_tags WHERE node_id = 12533;
DELETE FROM current_way_nodes WHERE node_id=12533;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12533;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12533;
/* delete node 12536*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12536, 8373, -13824, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12536;
DELETE FROM node_tags WHERE node_id = 12536;
DELETE FROM current_way_nodes WHERE node_id=12536;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12536;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12536;
/* delete node 12537*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12537, 8257, -27971, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12537;
DELETE FROM node_tags WHERE node_id = 12537;
DELETE FROM current_way_nodes WHERE node_id=12537;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12537;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12537;
/* delete node 12544*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12544, 8105, -31385, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12544;
DELETE FROM node_tags WHERE node_id = 12544;
DELETE FROM current_way_nodes WHERE node_id=12544;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12544;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12544;
/* delete node 12545*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12545, 8438, -14705, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12545;
DELETE FROM node_tags WHERE node_id = 12545;
DELETE FROM current_way_nodes WHERE node_id=12545;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12545;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12545;
/* delete node 12547*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12547, 8482, -14779, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12547;
DELETE FROM node_tags WHERE node_id = 12547;
DELETE FROM current_way_nodes WHERE node_id=12547;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12547;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12547;
/* delete node 12548*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12548, 8374, -28202, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12548;
DELETE FROM node_tags WHERE node_id = 12548;
DELETE FROM current_way_nodes WHERE node_id=12548;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12548;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12548;
/* delete node 12550*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12550, 8505, -14042, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12550;
DELETE FROM node_tags WHERE node_id = 12550;
DELETE FROM current_way_nodes WHERE node_id=12550;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12550;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12550;
/* delete node 12553*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12553, 8239, -31660, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12553;
DELETE FROM node_tags WHERE node_id = 12553;
DELETE FROM current_way_nodes WHERE node_id=12553;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12553;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12553;
/* delete node 12554*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12554, 8664, -20642, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12554;
DELETE FROM node_tags WHERE node_id = 12554;
DELETE FROM current_way_nodes WHERE node_id=12554;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12554;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12554;
/* delete node 12556*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12556, 8303, -31303, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12556;
DELETE FROM node_tags WHERE node_id = 12556;
DELETE FROM current_way_nodes WHERE node_id=12556;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12556;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12556;
/* delete node 12557*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12557, 8569, -28094, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12557;
DELETE FROM node_tags WHERE node_id = 12557;
DELETE FROM current_way_nodes WHERE node_id=12557;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12557;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12557;
/* delete node 12559*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12559, 8628, -28210, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12559;
DELETE FROM node_tags WHERE node_id = 12559;
DELETE FROM current_way_nodes WHERE node_id=12559;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12559;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12559;
/* delete node 12564*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12564, 8429, -31561, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12564;
DELETE FROM node_tags WHERE node_id = 12564;
DELETE FROM current_way_nodes WHERE node_id=12564;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12564;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12564;
/* delete node 12578*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12578, 8957, -14416, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12578;
DELETE FROM node_tags WHERE node_id = 12578;
DELETE FROM current_way_nodes WHERE node_id=12578;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12578;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12578;
/* delete node 12580*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12580, 8922, -27924, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12580;
DELETE FROM node_tags WHERE node_id = 12580;
DELETE FROM current_way_nodes WHERE node_id=12580;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12580;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12580;
/* delete node 12581*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12581, 8936, -27046, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12581;
DELETE FROM node_tags WHERE node_id = 12581;
DELETE FROM current_way_nodes WHERE node_id=12581;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12581;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12581;
/* delete node 12583*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12583, 9043, -14560, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12583;
DELETE FROM node_tags WHERE node_id = 12583;
DELETE FROM current_way_nodes WHERE node_id=12583;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12583;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12583;
/* delete node 12585*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12585, 8970, -28021, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12585;
DELETE FROM node_tags WHERE node_id = 12585;
DELETE FROM current_way_nodes WHERE node_id=12585;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12585;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12585;
/* delete node 12592*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12592, 9332, -18061, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12592;
DELETE FROM node_tags WHERE node_id = 12592;
DELETE FROM current_way_nodes WHERE node_id=12592;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12592;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12592;
/* delete node 12593*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12593, 8940, -31739, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12593;
DELETE FROM node_tags WHERE node_id = 12593;
DELETE FROM current_way_nodes WHERE node_id=12593;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12593;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12593;
/* delete node 12596*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12596, 8999, -31908, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12596;
DELETE FROM node_tags WHERE node_id = 12596;
DELETE FROM current_way_nodes WHERE node_id=12596;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12596;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12596;
/* delete node 12599*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12599, 9282, -27727, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12599;
DELETE FROM node_tags WHERE node_id = 12599;
DELETE FROM current_way_nodes WHERE node_id=12599;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12599;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12599;
/* delete node 12602*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12602, 9398, -13362, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12602;
DELETE FROM node_tags WHERE node_id = 12602;
DELETE FROM current_way_nodes WHERE node_id=12602;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12602;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12602;
/* delete node 12604*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12604, 9219, -30825, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12604;
DELETE FROM node_tags WHERE node_id = 12604;
DELETE FROM current_way_nodes WHERE node_id=12604;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12604;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12604;
/* delete node 12608*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12608, 9648, -18703, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12608;
DELETE FROM node_tags WHERE node_id = 12608;
DELETE FROM current_way_nodes WHERE node_id=12608;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12608;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12608;
/* delete node 12617*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12617, 9781, -13999, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12617;
DELETE FROM node_tags WHERE node_id = 12617;
DELETE FROM current_way_nodes WHERE node_id=12617;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12617;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12617;
/* delete node 12619*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12619, 9523, -31687, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12619;
DELETE FROM node_tags WHERE node_id = 12619;
DELETE FROM current_way_nodes WHERE node_id=12619;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12619;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12619;
/* delete node 12628*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12628, 10050, -17630, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12628;
DELETE FROM node_tags WHERE node_id = 12628;
DELETE FROM current_way_nodes WHERE node_id=12628;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12628;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12628;
/* delete node 12634*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12634, 10178, -17890, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12634;
DELETE FROM node_tags WHERE node_id = 12634;
DELETE FROM current_way_nodes WHERE node_id=12634;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12634;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12634;
/* delete node 12636*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12636, 10259, -17524, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12636;
DELETE FROM node_tags WHERE node_id = 12636;
DELETE FROM current_way_nodes WHERE node_id=12636;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12636;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12636;
/* delete node 12639*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12639, 10204, -26504, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12639;
DELETE FROM node_tags WHERE node_id = 12639;
DELETE FROM current_way_nodes WHERE node_id=12639;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12639;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12639;
/* delete node 12640*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12640, 10047, -30427, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12640;
DELETE FROM node_tags WHERE node_id = 12640;
DELETE FROM current_way_nodes WHERE node_id=12640;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12640;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12640;
/* delete node 12641*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12641, 10330, -18293, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12641;
DELETE FROM node_tags WHERE node_id = 12641;
DELETE FROM current_way_nodes WHERE node_id=12641;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12641;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12641;
/* delete node 12642*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12642, 10253, -26368, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12642;
DELETE FROM node_tags WHERE node_id = 12642;
DELETE FROM current_way_nodes WHERE node_id=12642;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12642;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12642;
/* delete node 12643*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12643, 10379, -17769, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12643;
DELETE FROM node_tags WHERE node_id = 12643;
DELETE FROM current_way_nodes WHERE node_id=12643;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12643;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12643;
/* delete node 12644*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12644, 10296, -26453, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12644;
DELETE FROM node_tags WHERE node_id = 12644;
DELETE FROM current_way_nodes WHERE node_id=12644;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12644;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12644;
/* delete node 12645*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12645, 10415, -18467, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12645;
DELETE FROM node_tags WHERE node_id = 12645;
DELETE FROM current_way_nodes WHERE node_id=12645;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12645;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12645;
/* delete node 12653*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12653, 10450, -26977, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12653;
DELETE FROM node_tags WHERE node_id = 12653;
DELETE FROM current_way_nodes WHERE node_id=12653;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12653;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12653;
/* delete node 12658*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12658, 10605, -18083, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12658;
DELETE FROM node_tags WHERE node_id = 12658;
DELETE FROM current_way_nodes WHERE node_id=12658;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12658;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12658;
/* delete node 12660*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12660, 10528, -12507, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12660;
DELETE FROM node_tags WHERE node_id = 12660;
DELETE FROM current_way_nodes WHERE node_id=12660;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12660;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12660;
/* delete node 12662*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12662, 10627, -17303, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12662;
DELETE FROM node_tags WHERE node_id = 12662;
DELETE FROM current_way_nodes WHERE node_id=12662;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12662;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12662;
/* delete node 12663*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12663, 10540, -26925, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12663;
DELETE FROM node_tags WHERE node_id = 12663;
DELETE FROM current_way_nodes WHERE node_id=12663;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12663;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12663;
/* delete node 12664*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12664, 10589, -27017, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12664;
DELETE FROM node_tags WHERE node_id = 12664;
DELETE FROM current_way_nodes WHERE node_id=12664;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12664;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12664;
/* delete node 12665*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12665, 10707, -18292, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12665;
DELETE FROM node_tags WHERE node_id = 12665;
DELETE FROM current_way_nodes WHERE node_id=12665;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12665;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12665;
/* delete node 12666*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12666, 10437, -31250, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12666;
DELETE FROM node_tags WHERE node_id = 12666;
DELETE FROM current_way_nodes WHERE node_id=12666;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12666;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12666;
/* delete node 12670*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12670, 10918, -17895, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12670;
DELETE FROM node_tags WHERE node_id = 12670;
DELETE FROM current_way_nodes WHERE node_id=12670;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12670;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12670;
/* delete node 12674*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12674, 10888, -12287, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12674;
DELETE FROM node_tags WHERE node_id = 12674;
DELETE FROM current_way_nodes WHERE node_id=12674;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12674;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12674;
/* delete node 12677*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12677, 11055, -13587, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12677;
DELETE FROM node_tags WHERE node_id = 12677;
DELETE FROM current_way_nodes WHERE node_id=12677;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12677;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12677;
/* delete node 12679*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12679, 11056, -12640, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12679;
DELETE FROM node_tags WHERE node_id = 12679;
DELETE FROM current_way_nodes WHERE node_id=12679;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12679;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12679;
/* delete node 12684*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12684, 10917, -31002, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12684;
DELETE FROM node_tags WHERE node_id = 12684;
DELETE FROM current_way_nodes WHERE node_id=12684;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12684;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12684;
/* delete node 12685*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12685, 10885, -31836, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12685;
DELETE FROM node_tags WHERE node_id = 12685;
DELETE FROM current_way_nodes WHERE node_id=12685;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12685;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12685;
/* delete node 12686*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12686, 10970, -31115, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12686;
DELETE FROM node_tags WHERE node_id = 12686;
DELETE FROM current_way_nodes WHERE node_id=12686;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12686;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12686;
/* delete node 12688*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12688, 11297, -16961, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12688;
DELETE FROM node_tags WHERE node_id = 12688;
DELETE FROM current_way_nodes WHERE node_id=12688;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12688;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12688;
/* delete node 12690*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12690, 11263, -25796, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12690;
DELETE FROM node_tags WHERE node_id = 12690;
DELETE FROM current_way_nodes WHERE node_id=12690;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12690;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12690;
/* delete node 12695*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12695, 11079, -32190, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12695;
DELETE FROM node_tags WHERE node_id = 12695;
DELETE FROM current_way_nodes WHERE node_id=12695;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12695;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12695;
/* delete node 12697*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12697, 11393, -11565, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12697;
DELETE FROM node_tags WHERE node_id = 12697;
DELETE FROM current_way_nodes WHERE node_id=12697;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12697;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12697;
/* delete node 12698*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12698, 11153, -31682, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12698;
DELETE FROM node_tags WHERE node_id = 12698;
DELETE FROM current_way_nodes WHERE node_id=12698;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12698;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12698;
/* delete node 12702*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12702, 11468, -29696, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12702;
DELETE FROM node_tags WHERE node_id = 12702;
DELETE FROM current_way_nodes WHERE node_id=12702;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12702;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12702;
/* delete node 12703*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12703, 11600, -26445, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12703;
DELETE FROM node_tags WHERE node_id = 12703;
DELETE FROM current_way_nodes WHERE node_id=12703;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12703;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12703;
/* delete node 12704*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12704, 11396, -30894, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12704;
DELETE FROM node_tags WHERE node_id = 12704;
DELETE FROM current_way_nodes WHERE node_id=12704;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12704;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12704;
/* delete node 12705*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12705, 11601, -12793, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12705;
DELETE FROM node_tags WHERE node_id = 12705;
DELETE FROM current_way_nodes WHERE node_id=12705;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12705;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12705;
/* delete node 12708*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12708, 11706, -17632, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12708;
DELETE FROM node_tags WHERE node_id = 12708;
DELETE FROM current_way_nodes WHERE node_id=12708;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12708;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12708;
/* delete node 12709*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12709, 11432, -30972, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12709;
DELETE FROM node_tags WHERE node_id = 12709;
DELETE FROM current_way_nodes WHERE node_id=12709;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12709;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12709;
/* delete node 12710*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12710, 11348, -32036, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12710;
DELETE FROM node_tags WHERE node_id = 12710;
DELETE FROM current_way_nodes WHERE node_id=12710;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12710;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12710;
/* delete node 12711*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12711, 11636, -12122, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12711;
DELETE FROM node_tags WHERE node_id = 12711;
DELETE FROM current_way_nodes WHERE node_id=12711;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12711;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12711;
/* delete node 12714*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12714, 11552, -30439, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12714;
DELETE FROM node_tags WHERE node_id = 12714;
DELETE FROM current_way_nodes WHERE node_id=12714;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12714;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12714;
/* delete node 12717*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12717, 11737, -12225, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12717;
DELETE FROM node_tags WHERE node_id = 12717;
DELETE FROM current_way_nodes WHERE node_id=12717;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12717;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12717;
/* delete node 12719*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12719, 11778, -13146, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12719;
DELETE FROM node_tags WHERE node_id = 12719;
DELETE FROM current_way_nodes WHERE node_id=12719;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12719;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12719;
/* delete node 12724*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12724, 11768, -30328, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12724;
DELETE FROM node_tags WHERE node_id = 12724;
DELETE FROM current_way_nodes WHERE node_id=12724;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12724;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12724;
/* delete node 12725*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12725, 11731, -30817, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12725;
DELETE FROM node_tags WHERE node_id = 12725;
DELETE FROM current_way_nodes WHERE node_id=12725;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12725;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12725;
/* delete node 12726*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12726, 11922, -12598, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12726;
DELETE FROM node_tags WHERE node_id = 12726;
DELETE FROM current_way_nodes WHERE node_id=12726;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12726;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12726;
/* delete node 12727*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12727, 11941, -11259, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12727;
DELETE FROM node_tags WHERE node_id = 12727;
DELETE FROM current_way_nodes WHERE node_id=12727;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12727;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12727;
/* delete node 12736*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12736, 12198, -25464, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12736;
DELETE FROM node_tags WHERE node_id = 12736;
DELETE FROM current_way_nodes WHERE node_id=12736;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12736;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12736;
/* delete node 12738*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12738, 12190, -11813, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12738;
DELETE FROM node_tags WHERE node_id = 12738;
DELETE FROM current_way_nodes WHERE node_id=12738;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12738;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12738;
/* delete node 12739*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12739, 12282, -16242, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12739;
DELETE FROM node_tags WHERE node_id = 12739;
DELETE FROM current_way_nodes WHERE node_id=12739;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12739;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12739;
/* delete node 12743*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12743, 12370, -17148, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12743;
DELETE FROM node_tags WHERE node_id = 12743;
DELETE FROM current_way_nodes WHERE node_id=12743;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12743;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12743;
/* delete node 12747*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12747, 12325, -10290, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12747;
DELETE FROM node_tags WHERE node_id = 12747;
DELETE FROM current_way_nodes WHERE node_id=12747;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12747;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12747;
/* delete node 12748*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12748, 12450, -24775, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12748;
DELETE FROM node_tags WHERE node_id = 12748;
DELETE FROM current_way_nodes WHERE node_id=12748;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12748;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12748;
/* delete node 12750*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12750, 12492, -17348, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12750;
DELETE FROM node_tags WHERE node_id = 12750;
DELETE FROM current_way_nodes WHERE node_id=12750;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12750;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12750;
/* delete node 12751*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12751, 12448, -25981, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12751;
DELETE FROM node_tags WHERE node_id = 12751;
DELETE FROM current_way_nodes WHERE node_id=12751;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12751;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12751;
/* delete node 12753*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12753, 12515, -17026, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12753;
DELETE FROM node_tags WHERE node_id = 12753;
DELETE FROM current_way_nodes WHERE node_id=12753;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12753;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12753;
/* delete node 12761*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12761, 12644, -17237, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12761;
DELETE FROM node_tags WHERE node_id = 12761;
DELETE FROM current_way_nodes WHERE node_id=12761;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12761;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12761;
/* delete node 12765*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12765, 12684, -16903, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12765;
DELETE FROM node_tags WHERE node_id = 12765;
DELETE FROM current_way_nodes WHERE node_id=12765;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12765;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12765;
/* delete node 12766*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12766, 12663, -25216, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12766;
DELETE FROM node_tags WHERE node_id = 12766;
DELETE FROM current_way_nodes WHERE node_id=12766;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12766;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12766;
/* delete node 12767*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12767, 12685, -16723, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12767;
DELETE FROM node_tags WHERE node_id = 12767;
DELETE FROM current_way_nodes WHERE node_id=12767;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12767;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12767;
/* delete node 12769*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12769, 12596, -10911, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12769;
DELETE FROM node_tags WHERE node_id = 12769;
DELETE FROM current_way_nodes WHERE node_id=12769;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12769;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12769;
/* delete node 12770*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12770, 12602, -29176, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12770;
DELETE FROM node_tags WHERE node_id = 12770;
DELETE FROM current_way_nodes WHERE node_id=12770;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12770;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12770;
/* delete node 12777*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12777, 12836, -24570, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12777;
DELETE FROM node_tags WHERE node_id = 12777;
DELETE FROM current_way_nodes WHERE node_id=12777;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12777;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12777;
/* delete node 12786*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12786, 12976, -24857, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12786;
DELETE FROM node_tags WHERE node_id = 12786;
DELETE FROM current_way_nodes WHERE node_id=12786;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12786;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12786;
/* delete node 12788*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12788, 13016, -17285, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12788;
DELETE FROM node_tags WHERE node_id = 12788;
DELETE FROM current_way_nodes WHERE node_id=12788;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12788;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12788;
/* delete node 12789*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12789, 12858, -29710, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12789;
DELETE FROM node_tags WHERE node_id = 12789;
DELETE FROM current_way_nodes WHERE node_id=12789;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12789;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12789;
/* delete node 12793*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12793, 12908, -9948, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12793;
DELETE FROM node_tags WHERE node_id = 12793;
DELETE FROM current_way_nodes WHERE node_id=12793;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12793;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12793;
/* delete node 12795*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12795, 13077, -16447, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12795;
DELETE FROM node_tags WHERE node_id = 12795;
DELETE FROM current_way_nodes WHERE node_id=12795;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12795;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12795;
/* delete node 12798*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12798, 13003, -10576, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12798;
DELETE FROM node_tags WHERE node_id = 12798;
DELETE FROM current_way_nodes WHERE node_id=12798;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12798;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12798;
/* delete node 12801*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12801, 13044, -10669, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12801;
DELETE FROM node_tags WHERE node_id = 12801;
DELETE FROM current_way_nodes WHERE node_id=12801;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12801;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12801;
/* delete node 12804*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12804, 13123, -10912, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12804;
DELETE FROM node_tags WHERE node_id = 12804;
DELETE FROM current_way_nodes WHERE node_id=12804;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12804;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12804;
/* delete node 12808*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12808, 13186, -28749, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12808;
DELETE FROM node_tags WHERE node_id = 12808;
DELETE FROM current_way_nodes WHERE node_id=12808;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12808;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12808;
/* delete node 12810*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12810, 13268, -24693, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12810;
DELETE FROM node_tags WHERE node_id = 12810;
DELETE FROM current_way_nodes WHERE node_id=12810;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12810;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12810;
/* delete node 12815*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12815, 13173, -10483, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12815;
DELETE FROM node_tags WHERE node_id = 12815;
DELETE FROM current_way_nodes WHERE node_id=12815;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12815;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12815;
/* delete node 12818*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12818, 13226, -28830, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12818;
DELETE FROM node_tags WHERE node_id = 12818;
DELETE FROM current_way_nodes WHERE node_id=12818;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12818;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12818;
/* delete node 12823*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12823, 13408, -17010, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12823;
DELETE FROM node_tags WHERE node_id = 12823;
DELETE FROM current_way_nodes WHERE node_id=12823;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12823;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12823;
/* delete node 12824*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12824, 13409, -24982, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12824;
DELETE FROM node_tags WHERE node_id = 12824;
DELETE FROM current_way_nodes WHERE node_id=12824;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12824;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12824;
/* delete node 12827*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12827, 13402, -11562, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12827;
DELETE FROM node_tags WHERE node_id = 12827;
DELETE FROM current_way_nodes WHERE node_id=12827;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12827;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12827;
/* delete node 12828*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12828, 13517, -24921, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12828;
DELETE FROM node_tags WHERE node_id = 12828;
DELETE FROM current_way_nodes WHERE node_id=12828;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12828;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12828;
/* delete node 12829*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12829, 13511, -15878, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12829;
DELETE FROM node_tags WHERE node_id = 12829;
DELETE FROM current_way_nodes WHERE node_id=12829;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12829;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12829;
/* delete node 12838*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12838, 13559, -29328, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12838;
DELETE FROM node_tags WHERE node_id = 12838;
DELETE FROM current_way_nodes WHERE node_id=12838;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12838;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12838;
/* delete node 12841*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12841, 13698, -25291, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12841;
DELETE FROM node_tags WHERE node_id = 12841;
DELETE FROM current_way_nodes WHERE node_id=12841;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12841;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12841;
/* delete node 12842*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12842, 13578, -10230, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12842;
DELETE FROM node_tags WHERE node_id = 12842;
DELETE FROM current_way_nodes WHERE node_id=12842;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12842;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12842;
/* delete node 12844*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12844, 13691, -28462, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12844;
DELETE FROM node_tags WHERE node_id = 12844;
DELETE FROM current_way_nodes WHERE node_id=12844;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12844;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12844;
/* delete node 12847*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12847, 13788, -16466, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12847;
DELETE FROM node_tags WHERE node_id = 12847;
DELETE FROM current_way_nodes WHERE node_id=12847;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12847;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12847;
/* delete node 12848*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12848, 13786, -15445, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12848;
DELETE FROM node_tags WHERE node_id = 12848;
DELETE FROM current_way_nodes WHERE node_id=12848;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12848;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12848;
/* delete node 12852*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12852, 13741, -10585, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12852;
DELETE FROM node_tags WHERE node_id = 12852;
DELETE FROM current_way_nodes WHERE node_id=12852;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12852;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12852;
/* delete node 12855*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12855, 13889, -15662, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12855;
DELETE FROM node_tags WHERE node_id = 12855;
DELETE FROM current_way_nodes WHERE node_id=12855;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12855;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12855;
/* delete node 12858*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12858, 13856, -28803, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12858;
DELETE FROM node_tags WHERE node_id = 12858;
DELETE FROM current_way_nodes WHERE node_id=12858;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12858;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12858;
/* delete node 12862*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12862, 13836, -10083, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12862;
DELETE FROM node_tags WHERE node_id = 12862;
DELETE FROM current_way_nodes WHERE node_id=12862;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12862;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12862;
/* delete node 12870*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12870, 13873, -30006, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12870;
DELETE FROM node_tags WHERE node_id = 12870;
DELETE FROM current_way_nodes WHERE node_id=12870;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12870;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12870;
/* delete node 12873*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12873, 14068, -16064, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12873;
DELETE FROM node_tags WHERE node_id = 12873;
DELETE FROM current_way_nodes WHERE node_id=12873;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12873;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12873;
/* delete node 12876*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12876, 14057, -29210, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12876;
DELETE FROM node_tags WHERE node_id = 12876;
DELETE FROM current_way_nodes WHERE node_id=12876;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12876;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12876;
/* delete node 12877*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12877, 14158, -16254, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12877;
DELETE FROM node_tags WHERE node_id = 12877;
DELETE FROM current_way_nodes WHERE node_id=12877;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12877;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12877;
/* delete node 12883*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12883, 14237, -28585, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12883;
DELETE FROM node_tags WHERE node_id = 12883;
DELETE FROM current_way_nodes WHERE node_id=12883;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12883;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12883;
/* delete node 12886*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12886, 14291, -11093, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12886;
DELETE FROM node_tags WHERE node_id = 12886;
DELETE FROM current_way_nodes WHERE node_id=12886;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12886;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12886;
/* delete node 12891*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12891, 14322, -29750, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12891;
DELETE FROM node_tags WHERE node_id = 12891;
DELETE FROM current_way_nodes WHERE node_id=12891;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12891;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12891;
/* delete node 12895*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12895, 14437, -28994, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12895;
DELETE FROM node_tags WHERE node_id = 12895;
DELETE FROM current_way_nodes WHERE node_id=12895;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12895;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12895;
/* delete node 12902*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12902, 14546, -15010, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12902;
DELETE FROM node_tags WHERE node_id = 12902;
DELETE FROM current_way_nodes WHERE node_id=12902;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12902;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12902;
/* delete node 12909*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12909, 14672, -15279, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12909;
DELETE FROM node_tags WHERE node_id = 12909;
DELETE FROM current_way_nodes WHERE node_id=12909;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12909;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12909;
/* delete node 12911*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12911, 14538, -30085, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12911;
DELETE FROM node_tags WHERE node_id = 12911;
DELETE FROM current_way_nodes WHERE node_id=12911;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12911;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12911;
/* delete node 12916*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12916, 14821, -15632, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12916;
DELETE FROM node_tags WHERE node_id = 12916;
DELETE FROM current_way_nodes WHERE node_id=12916;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12916;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12916;
/* delete node 12921*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12921, 14939, -15883, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12921;
DELETE FROM node_tags WHERE node_id = 12921;
DELETE FROM current_way_nodes WHERE node_id=12921;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12921;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12921;
/* delete node 12924*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12924, 14797, -30715, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12924;
DELETE FROM node_tags WHERE node_id = 12924;
DELETE FROM current_way_nodes WHERE node_id=12924;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12924;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12924;
/* delete node 12926*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12926, 14886, -9732, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12926;
DELETE FROM node_tags WHERE node_id = 12926;
DELETE FROM current_way_nodes WHERE node_id=12926;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12926;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12926;
/* delete node 12928*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12928, 15122, -23607, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12928;
DELETE FROM node_tags WHERE node_id = 12928;
DELETE FROM current_way_nodes WHERE node_id=12928;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12928;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12928;
/* delete node 12942*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12942, 15259, -27876, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12942;
DELETE FROM node_tags WHERE node_id = 12942;
DELETE FROM current_way_nodes WHERE node_id=12942;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12942;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12942;
/* delete node 12943*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12943, 15192, -29748, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12943;
DELETE FROM node_tags WHERE node_id = 12943;
DELETE FROM current_way_nodes WHERE node_id=12943;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12943;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12943;
/* delete node 12945*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12945, 15374, -24262, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12945;
DELETE FROM node_tags WHERE node_id = 12945;
DELETE FROM current_way_nodes WHERE node_id=12945;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12945;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12945;
/* delete node 12950*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12950, 15311, -10438, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12950;
DELETE FROM node_tags WHERE node_id = 12950;
DELETE FROM current_way_nodes WHERE node_id=12950;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12950;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12950;
/* delete node 12952*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12952, 15439, -14839, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12952;
DELETE FROM node_tags WHERE node_id = 12952;
DELETE FROM current_way_nodes WHERE node_id=12952;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12952;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12952;
/* delete node 12957*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12957, 15492, -28449, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12957;
DELETE FROM node_tags WHERE node_id = 12957;
DELETE FROM current_way_nodes WHERE node_id=12957;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12957;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12957;
/* delete node 12961*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12961, 15496, -10318, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12961;
DELETE FROM node_tags WHERE node_id = 12961;
DELETE FROM current_way_nodes WHERE node_id=12961;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12961;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12961;
/* delete node 12962*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12962, 15451, -30380, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12962;
DELETE FROM node_tags WHERE node_id = 12962;
DELETE FROM current_way_nodes WHERE node_id=12962;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12962;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12962;
/* delete node 12968*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12968, 15614, -10536, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12968;
DELETE FROM node_tags WHERE node_id = 12968;
DELETE FROM current_way_nodes WHERE node_id=12968;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12968;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12968;
/* delete node 12971*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12971, 15720, -15435, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12971;
DELETE FROM node_tags WHERE node_id = 12971;
DELETE FROM current_way_nodes WHERE node_id=12971;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12971;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12971;
/* delete node 12980*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12980, 15935, -23227, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12980;
DELETE FROM node_tags WHERE node_id = 12980;
DELETE FROM current_way_nodes WHERE node_id=12980;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12980;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12980;
/* delete node 12981*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12981, 15904, -28249, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12981;
DELETE FROM node_tags WHERE node_id = 12981;
DELETE FROM current_way_nodes WHERE node_id=12981;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12981;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12981;
/* delete node 12985*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12985, 15996, -28498, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12985;
DELETE FROM node_tags WHERE node_id = 12985;
DELETE FROM current_way_nodes WHERE node_id=12985;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12985;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12985;
/* delete node 12990*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12990, 16051, -14258, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12990;
DELETE FROM node_tags WHERE node_id = 12990;
DELETE FROM current_way_nodes WHERE node_id=12990;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12990;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12990;
/* delete node 12995*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (12995, 16186, -23882, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 12995;
DELETE FROM node_tags WHERE node_id = 12995;
DELETE FROM current_way_nodes WHERE node_id=12995;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 12995;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=12995;
/* delete node 13001*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13001, 16301, -27348, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13001;
DELETE FROM node_tags WHERE node_id = 13001;
DELETE FROM current_way_nodes WHERE node_id=13001;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13001;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13001;
/* delete node 13002*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13002, 16045, -8915, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13002;
DELETE FROM node_tags WHERE node_id = 13002;
DELETE FROM current_way_nodes WHERE node_id=13002;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13002;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13002;
/* delete node 13004*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13004, 16323, -13809, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13004;
DELETE FROM node_tags WHERE node_id = 13004;
DELETE FROM current_way_nodes WHERE node_id=13004;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13004;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13004;
/* delete node 13005*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13005, 16418, -27622, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13005;
DELETE FROM node_tags WHERE node_id = 13005;
DELETE FROM current_way_nodes WHERE node_id=13005;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13005;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13005;
/* delete node 13006*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13006, 16408, -14893, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13006;
DELETE FROM node_tags WHERE node_id = 13006;
DELETE FROM current_way_nodes WHERE node_id=13006;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13006;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13006;
/* delete node 13012*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13012, 16310, -9793, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13012;
DELETE FROM node_tags WHERE node_id = 13012;
DELETE FROM current_way_nodes WHERE node_id=13012;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13012;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13012;
/* delete node 13014*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13014, 16431, -14002, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13014;
DELETE FROM node_tags WHERE node_id = 13014;
DELETE FROM current_way_nodes WHERE node_id=13014;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13014;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13014;
/* delete node 13020*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13020, 16432, -9988, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13020;
DELETE FROM node_tags WHERE node_id = 13020;
DELETE FROM current_way_nodes WHERE node_id=13020;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13020;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13020;
/* delete node 13024*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13024, 16508, -9655, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13024;
DELETE FROM node_tags WHERE node_id = 13024;
DELETE FROM current_way_nodes WHERE node_id=13024;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13024;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13024;
/* delete node 13027*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13027, 16730, -27464, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13027;
DELETE FROM node_tags WHERE node_id = 13027;
DELETE FROM current_way_nodes WHERE node_id=13027;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13027;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13027;
/* delete node 13030*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13030, 16759, -14657, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13030;
DELETE FROM node_tags WHERE node_id = 13030;
DELETE FROM current_way_nodes WHERE node_id=13030;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13030;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13030;
/* delete node 13041*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13041, 16898, -14906, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13041;
DELETE FROM node_tags WHERE node_id = 13041;
DELETE FROM current_way_nodes WHERE node_id=13041;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13041;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13041;
/* delete node 13042*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13042, 16971, -28026, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13042;
DELETE FROM node_tags WHERE node_id = 13042;
DELETE FROM current_way_nodes WHERE node_id=13042;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13042;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13042;
/* delete node 13050*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13050, 17014, -13345, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13050;
DELETE FROM node_tags WHERE node_id = 13050;
DELETE FROM current_way_nodes WHERE node_id=13050;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13050;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13050;
/* delete node 13061*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13061, 17188, -13655, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13061;
DELETE FROM node_tags WHERE node_id = 13061;
DELETE FROM current_way_nodes WHERE node_id=13061;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13061;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13061;
/* delete node 13071*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13071, 17302, -14635, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13071;
DELETE FROM node_tags WHERE node_id = 13071;
DELETE FROM current_way_nodes WHERE node_id=13071;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13071;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13071;
/* delete node 13083*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13083, 17399, -14795, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13083;
DELETE FROM node_tags WHERE node_id = 13083;
DELETE FROM current_way_nodes WHERE node_id=13083;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13083;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13083;
/* delete node 13088*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13088, 17453, -14152, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13088;
DELETE FROM node_tags WHERE node_id = 13088;
DELETE FROM current_way_nodes WHERE node_id=13088;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13088;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13088;
/* delete node 13091*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13091, 17188, -8158, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13091;
DELETE FROM node_tags WHERE node_id = 13091;
DELETE FROM current_way_nodes WHERE node_id=13091;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13091;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13091;
/* delete node 13097*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13097, 17538, -13418, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13097;
DELETE FROM node_tags WHERE node_id = 13097;
DELETE FROM current_way_nodes WHERE node_id=13097;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13097;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13097;
/* delete node 13099*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13099, 16561, -1257, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13099;
DELETE FROM node_tags WHERE node_id = 13099;
DELETE FROM current_way_nodes WHERE node_id=13099;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13099;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13099;
/* delete node 13110*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13110, 17708, -14584, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13110;
DELETE FROM node_tags WHERE node_id = 13110;
DELETE FROM current_way_nodes WHERE node_id=13110;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13110;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13110;
/* delete node 13114*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13114, 17822, -26762, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13114;
DELETE FROM node_tags WHERE node_id = 13114;
DELETE FROM current_way_nodes WHERE node_id=13114;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13114;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13114;
/* delete node 13115*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13115, 17822, -27037, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13115;
DELETE FROM node_tags WHERE node_id = 13115;
DELETE FROM current_way_nodes WHERE node_id=13115;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13115;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13115;
/* delete node 13117*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13117, 16658, -269, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13117;
DELETE FROM node_tags WHERE node_id = 13117;
DELETE FROM current_way_nodes WHERE node_id=13117;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13117;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13117;
/* delete node 13120*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13120, 17815, -13909, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13120;
DELETE FROM node_tags WHERE node_id = 13120;
DELETE FROM current_way_nodes WHERE node_id=13120;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13120;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13120;
/* delete node 13124*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13124, 17914, -26992, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13124;
DELETE FROM node_tags WHERE node_id = 13124;
DELETE FROM current_way_nodes WHERE node_id=13124;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13124;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13124;
/* delete node 13126*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13126, 17926, -26532, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13126;
DELETE FROM node_tags WHERE node_id = 13126;
DELETE FROM current_way_nodes WHERE node_id=13126;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13126;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13126;
/* delete node 13130*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13130, 17977, -27421, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13130;
DELETE FROM node_tags WHERE node_id = 13130;
DELETE FROM current_way_nodes WHERE node_id=13130;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13130;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13130;
/* delete node 13131*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13131, 17987, -26680, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13131;
DELETE FROM node_tags WHERE node_id = 13131;
DELETE FROM current_way_nodes WHERE node_id=13131;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13131;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13131;
/* delete node 13133*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13133, 17728, -9003, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13133;
DELETE FROM node_tags WHERE node_id = 13133;
DELETE FROM current_way_nodes WHERE node_id=13133;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13133;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13133;
/* delete node 13134*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13134, 17608, -7855, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13134;
DELETE FROM node_tags WHERE node_id = 13134;
DELETE FROM current_way_nodes WHERE node_id=13134;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13134;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13134;
/* delete node 13142*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13142, 18091, -27365, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13142;
DELETE FROM node_tags WHERE node_id = 13142;
DELETE FROM current_way_nodes WHERE node_id=13142;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13142;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13142;
/* delete node 13147*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13147, 17135, -2174, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13147;
DELETE FROM node_tags WHERE node_id = 13147;
DELETE FROM current_way_nodes WHERE node_id=13147;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13147;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13147;
/* delete node 13150*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13150, 17805, -8164, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13150;
DELETE FROM node_tags WHERE node_id = 13150;
DELETE FROM current_way_nodes WHERE node_id=13150;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13150;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13150;
/* delete node 13151*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13151, 17045, -888, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13151;
DELETE FROM node_tags WHERE node_id = 13151;
DELETE FROM current_way_nodes WHERE node_id=13151;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13151;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13151;
/* delete node 13153*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13153, 18199, -27631, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13153;
DELETE FROM node_tags WHERE node_id = 13153;
DELETE FROM current_way_nodes WHERE node_id=13153;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13153;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13153;
/* delete node 13164*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13164, 18019, -8795, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13164;
DELETE FROM node_tags WHERE node_id = 13164;
DELETE FROM current_way_nodes WHERE node_id=13164;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13164;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13164;
/* delete node 13171*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13171, 18109, -8938, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13171;
DELETE FROM node_tags WHERE node_id = 13171;
DELETE FROM current_way_nodes WHERE node_id=13171;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13171;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13171;
/* delete node 13177*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13177, 17241, 115, 2, false, (now() at time zone 'utc'), 3221225472, 2);
DELETE FROM current_node_tags WHERE node_id = 13177;
DELETE FROM node_tags WHERE node_id = 13177;
DELETE FROM current_way_nodes WHERE node_id=13177;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13177;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13177;
/* delete node 13186*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13186, 18519, -26242, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13186;
DELETE FROM node_tags WHERE node_id = 13186;
DELETE FROM current_way_nodes WHERE node_id=13186;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13186;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13186;
/* delete node 13189*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13189, 17501, -1664, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13189;
DELETE FROM node_tags WHERE node_id = 13189;
DELETE FROM current_way_nodes WHERE node_id=13189;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13189;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13189;
/* delete node 13190*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13190, 18461, -12805, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13190;
DELETE FROM node_tags WHERE node_id = 13190;
DELETE FROM current_way_nodes WHERE node_id=13190;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13190;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13190;
/* delete node 13191*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13191, 18585, -26405, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13191;
DELETE FROM node_tags WHERE node_id = 13191;
DELETE FROM current_way_nodes WHERE node_id=13191;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13191;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13191;
/* delete node 13194*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13194, 17599, -1820, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13194;
DELETE FROM node_tags WHERE node_id = 13194;
DELETE FROM current_way_nodes WHERE node_id=13194;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13194;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13194;
/* delete node 13200*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13200, 18598, -12296, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13200;
DELETE FROM node_tags WHERE node_id = 13200;
DELETE FROM current_way_nodes WHERE node_id=13200;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13200;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13200;
/* delete node 13202*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13202, 18413, -8718, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13202;
DELETE FROM node_tags WHERE node_id = 13202;
DELETE FROM current_way_nodes WHERE node_id=13202;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13202;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13202;
/* delete node 13204*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13204, 18770, -27080, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13204;
DELETE FROM node_tags WHERE node_id = 13204;
DELETE FROM current_way_nodes WHERE node_id=13204;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13204;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13204;
/* delete node 13208*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13208, 18733, -13313, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13208;
DELETE FROM node_tags WHERE node_id = 13208;
DELETE FROM current_way_nodes WHERE node_id=13208;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13208;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13208;
/* delete node 13209*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13209, 18551, -8935, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13209;
DELETE FROM node_tags WHERE node_id = 13209;
DELETE FROM current_way_nodes WHERE node_id=13209;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13209;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13209;
/* delete node 13213*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13213, 18862, -27308, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13213;
DELETE FROM node_tags WHERE node_id = 13213;
DELETE FROM current_way_nodes WHERE node_id=13213;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13213;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13213;
/* delete node 13214*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13214, 18765, -12615, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13214;
DELETE FROM node_tags WHERE node_id = 13214;
DELETE FROM current_way_nodes WHERE node_id=13214;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13214;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13214;
/* delete node 13221*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13221, 18897, -12949, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13221;
DELETE FROM node_tags WHERE node_id = 13221;
DELETE FROM current_way_nodes WHERE node_id=13221;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13221;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13221;
/* delete node 13228*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13228, 18635, -7562, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13228;
DELETE FROM node_tags WHERE node_id = 13228;
DELETE FROM current_way_nodes WHERE node_id=13228;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13228;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13228;
/* delete node 13229*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13229, 19088, -26159, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13229;
DELETE FROM node_tags WHERE node_id = 13229;
DELETE FROM current_way_nodes WHERE node_id=13229;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13229;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13229;
/* delete node 13230*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13230, 18765, -8516, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13230;
DELETE FROM node_tags WHERE node_id = 13230;
DELETE FROM current_way_nodes WHERE node_id=13230;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13230;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13230;
/* delete node 13236*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13236, 19005, -13140, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13236;
DELETE FROM node_tags WHERE node_id = 13236;
DELETE FROM current_way_nodes WHERE node_id=13236;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13236;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13236;
/* delete node 13242*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13242, 18880, -8697, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13242;
DELETE FROM node_tags WHERE node_id = 13242;
DELETE FROM current_way_nodes WHERE node_id=13242;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13242;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13242;
/* delete node 13244*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13244, 18081, -1221, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13244;
DELETE FROM node_tags WHERE node_id = 13244;
DELETE FROM current_way_nodes WHERE node_id=13244;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13244;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13244;
/* delete node 13252*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13252, 19349, -26797, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13252;
DELETE FROM node_tags WHERE node_id = 13252;
DELETE FROM current_way_nodes WHERE node_id=13252;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13252;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13252;
/* delete node 13259*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13259, 19093, -8280, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13259;
DELETE FROM node_tags WHERE node_id = 13259;
DELETE FROM current_way_nodes WHERE node_id=13259;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13259;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13259;
/* delete node 13260*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13260, 19337, -11833, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13260;
DELETE FROM node_tags WHERE node_id = 13260;
DELETE FROM current_way_nodes WHERE node_id=13260;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13260;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13260;
/* delete node 13281*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13281, 19673, -12457, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13281;
DELETE FROM node_tags WHERE node_id = 13281;
DELETE FROM current_way_nodes WHERE node_id=13281;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13281;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13281;
/* delete node 13302*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13302, 19777, -7027, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13302;
DELETE FROM node_tags WHERE node_id = 13302;
DELETE FROM current_way_nodes WHERE node_id=13302;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13302;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13302;
/* delete node 13307*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13307, 19802, -6777, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13307;
DELETE FROM node_tags WHERE node_id = 13307;
DELETE FROM current_way_nodes WHERE node_id=13307;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13307;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13307;
/* delete node 13310*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13310, 19902, -6937, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13310;
DELETE FROM node_tags WHERE node_id = 13310;
DELETE FROM current_way_nodes WHERE node_id=13310;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13310;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13310;
/* delete node 13318*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13318, 20693, -20123, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13318;
DELETE FROM node_tags WHERE node_id = 13318;
DELETE FROM current_way_nodes WHERE node_id=13318;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13318;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13318;
/* delete node 13319*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13319, 20173, -7654, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13319;
DELETE FROM node_tags WHERE node_id = 13319;
DELETE FROM current_way_nodes WHERE node_id=13319;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13319;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13319;
/* delete node 13322*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13322, 20109, -6557, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13322;
DELETE FROM node_tags WHERE node_id = 13322;
DELETE FROM current_way_nodes WHERE node_id=13322;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13322;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13322;
/* delete node 13323*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13323, 20771, -19369, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13323;
DELETE FROM node_tags WHERE node_id = 13323;
DELETE FROM current_way_nodes WHERE node_id=13323;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13323;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13323;
/* delete node 13324*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13324, 20190, -6686, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13324;
DELETE FROM node_tags WHERE node_id = 13324;
DELETE FROM current_way_nodes WHERE node_id=13324;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13324;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13324;
/* delete node 13330*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13330, 20784, -26010, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13330;
DELETE FROM node_tags WHERE node_id = 13330;
DELETE FROM current_way_nodes WHERE node_id=13330;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13330;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13330;
/* delete node 13331*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13331, 20195, -6178, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13331;
DELETE FROM node_tags WHERE node_id = 13331;
DELETE FROM current_way_nodes WHERE node_id=13331;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13331;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13331;
/* delete node 13334*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13334, 20959, -26890, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13334;
DELETE FROM node_tags WHERE node_id = 13334;
DELETE FROM current_way_nodes WHERE node_id=13334;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13334;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13334;
/* delete node 13337*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13337, 20413, -6526, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13337;
DELETE FROM node_tags WHERE node_id = 13337;
DELETE FROM current_way_nodes WHERE node_id=13337;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13337;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13337;
/* delete node 13338*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13338, 20528, -7401, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13338;
DELETE FROM node_tags WHERE node_id = 13338;
DELETE FROM current_way_nodes WHERE node_id=13338;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13338;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13338;
/* delete node 13341*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13341, 20386, -5684, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13341;
DELETE FROM node_tags WHERE node_id = 13341;
DELETE FROM current_way_nodes WHERE node_id=13341;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13341;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13341;
/* delete node 13342*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13342, 20907, -10861, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13342;
DELETE FROM node_tags WHERE node_id = 13342;
DELETE FROM current_way_nodes WHERE node_id=13342;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13342;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13342;
/* delete node 13346*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13346, 20664, -7616, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13346;
DELETE FROM node_tags WHERE node_id = 13346;
DELETE FROM current_way_nodes WHERE node_id=13346;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13346;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13346;
/* delete node 13351*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13351, 20543, -5930, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13351;
DELETE FROM node_tags WHERE node_id = 13351;
DELETE FROM current_way_nodes WHERE node_id=13351;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13351;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13351;
/* delete node 13357*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13357, 20643, -5500, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13357;
DELETE FROM node_tags WHERE node_id = 13357;
DELETE FROM current_way_nodes WHERE node_id=13357;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13357;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13357;
/* delete node 13359*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13359, 21221, -11428, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13359;
DELETE FROM node_tags WHERE node_id = 13359;
DELETE FROM current_way_nodes WHERE node_id=13359;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13359;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13359;
/* delete node 13365*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13365, 21558, -20091, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13365;
DELETE FROM node_tags WHERE node_id = 13365;
DELETE FROM current_way_nodes WHERE node_id=13365;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13365;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13365;
/* delete node 13370*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13370, 21600, -19699, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13370;
DELETE FROM node_tags WHERE node_id = 13370;
DELETE FROM current_way_nodes WHERE node_id=13370;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13370;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13370;
/* delete node 13372*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13372, 21096, -6885, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13372;
DELETE FROM node_tags WHERE node_id = 13372;
DELETE FROM current_way_nodes WHERE node_id=13372;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13372;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13372;
/* delete node 13378*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13378, 21284, -7190, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13378;
DELETE FROM node_tags WHERE node_id = 13378;
DELETE FROM current_way_nodes WHERE node_id=13378;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13378;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13378;
/* delete node 13381*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13381, 21934, -19376, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13381;
DELETE FROM node_tags WHERE node_id = 13381;
DELETE FROM current_way_nodes WHERE node_id=13381;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13381;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13381;
/* delete node 13383*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13383, 21665, -10412, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13383;
DELETE FROM node_tags WHERE node_id = 13383;
DELETE FROM current_way_nodes WHERE node_id=13383;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13383;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13383;
/* delete node 13387*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13387, 21998, -18695, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13387;
DELETE FROM node_tags WHERE node_id = 13387;
DELETE FROM current_way_nodes WHERE node_id=13387;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13387;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13387;
/* delete node 13388*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13388, 21944, -25695, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13388;
DELETE FROM node_tags WHERE node_id = 13388;
DELETE FROM current_way_nodes WHERE node_id=13388;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13388;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13388;
/* delete node 13390*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13390, 21779, -11118, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13390;
DELETE FROM node_tags WHERE node_id = 13390;
DELETE FROM current_way_nodes WHERE node_id=13390;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13390;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13390;
/* delete node 13391*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13391, 21403, -6713, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13391;
DELETE FROM node_tags WHERE node_id = 13391;
DELETE FROM current_way_nodes WHERE node_id=13391;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13391;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13391;
/* delete node 13392*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13392, 21783, -10633, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13392;
DELETE FROM node_tags WHERE node_id = 13392;
DELETE FROM current_way_nodes WHERE node_id=13392;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13392;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13392;
/* delete node 13395*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13395, 22024, -26101, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13395;
DELETE FROM node_tags WHERE node_id = 13395;
DELETE FROM current_way_nodes WHERE node_id=13395;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13395;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13395;
/* delete node 13399*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13399, 22104, -26614, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13399;
DELETE FROM node_tags WHERE node_id = 13399;
DELETE FROM current_way_nodes WHERE node_id=13399;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13399;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13399;
/* delete node 13400*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13400, 21974, -11483, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13400;
DELETE FROM node_tags WHERE node_id = 13400;
DELETE FROM current_way_nodes WHERE node_id=13400;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13400;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13400;
/* delete node 13401*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13401, 22138, -26792, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13401;
DELETE FROM node_tags WHERE node_id = 13401;
DELETE FROM current_way_nodes WHERE node_id=13401;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13401;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13401;
/* delete node 13416*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13416, 22420, -26056, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13416;
DELETE FROM node_tags WHERE node_id = 13416;
DELETE FROM current_way_nodes WHERE node_id=13416;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13416;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13416;
/* delete node 13421*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13421, 22559, -19814, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13421;
DELETE FROM node_tags WHERE node_id = 13421;
DELETE FROM current_way_nodes WHERE node_id=13421;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13421;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13421;
/* delete node 13422*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13422, 22325, -11157, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13422;
DELETE FROM node_tags WHERE node_id = 13422;
DELETE FROM current_way_nodes WHERE node_id=13422;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13422;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13422;
/* delete node 13423*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13423, 22395, -11285, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13423;
DELETE FROM node_tags WHERE node_id = 13423;
DELETE FROM current_way_nodes WHERE node_id=13423;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13423;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13423;
/* delete node 13425*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13425, 22550, -26748, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13425;
DELETE FROM node_tags WHERE node_id = 13425;
DELETE FROM current_way_nodes WHERE node_id=13425;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13425;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13425;
/* delete node 13429*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13429, 22663, -18803, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13429;
DELETE FROM node_tags WHERE node_id = 13429;
DELETE FROM current_way_nodes WHERE node_id=13429;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13429;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13429;
/* delete node 13433*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13433, 22435, -10325, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13433;
DELETE FROM node_tags WHERE node_id = 13433;
DELETE FROM current_way_nodes WHERE node_id=13433;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13433;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13433;
/* delete node 13434*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13434, 21964, -5654, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13434;
DELETE FROM node_tags WHERE node_id = 13434;
DELETE FROM current_way_nodes WHERE node_id=13434;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13434;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13434;
/* delete node 13435*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13435, 21975, -5343, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13435;
DELETE FROM node_tags WHERE node_id = 13435;
DELETE FROM current_way_nodes WHERE node_id=13435;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13435;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13435;
/* delete node 13448*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13448, 22116, -5575, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13448;
DELETE FROM node_tags WHERE node_id = 13448;
DELETE FROM current_way_nodes WHERE node_id=13448;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13448;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13448;
/* delete node 13462*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13462, 22768, -10949, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13462;
DELETE FROM node_tags WHERE node_id = 13462;
DELETE FROM current_way_nodes WHERE node_id=13462;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13462;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13462;
/* delete node 13465*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13465, 22265, -5194, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13465;
DELETE FROM node_tags WHERE node_id = 13465;
DELETE FROM current_way_nodes WHERE node_id=13465;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13465;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13465;
/* delete node 13466*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13466, 22397, -6364, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13466;
DELETE FROM node_tags WHERE node_id = 13466;
DELETE FROM current_way_nodes WHERE node_id=13466;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13466;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13466;
/* delete node 13474*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13474, 22442, -5484, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13474;
DELETE FROM node_tags WHERE node_id = 13474;
DELETE FROM current_way_nodes WHERE node_id=13474;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13474;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13474;
/* delete node 13490*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13490, 22812, -6150, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13490;
DELETE FROM node_tags WHERE node_id = 13490;
DELETE FROM current_way_nodes WHERE node_id=13490;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13490;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13490;
/* delete node 13502*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13502, 23013, -6479, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13502;
DELETE FROM node_tags WHERE node_id = 13502;
DELETE FROM current_way_nodes WHERE node_id=13502;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13502;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13502;
/* delete node 13516*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13516, 23557, -10339, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13516;
DELETE FROM node_tags WHERE node_id = 13516;
DELETE FROM current_way_nodes WHERE node_id=13516;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13516;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13516;
/* delete node 13520*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13520, 23085, -5154, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13520;
DELETE FROM node_tags WHERE node_id = 13520;
DELETE FROM current_way_nodes WHERE node_id=13520;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13520;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13520;
/* delete node 13524*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13524, 23837, -25736, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13524;
DELETE FROM node_tags WHERE node_id = 13524;
DELETE FROM current_way_nodes WHERE node_id=13524;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13524;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13524;
/* delete node 13539*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13539, 23907, -26879, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13539;
DELETE FROM node_tags WHERE node_id = 13539;
DELETE FROM current_way_nodes WHERE node_id=13539;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13539;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13539;
/* delete node 13550*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13550, 23682, -6135, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13550;
DELETE FROM node_tags WHERE node_id = 13550;
DELETE FROM current_way_nodes WHERE node_id=13550;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13550;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13550;
/* delete node 13554*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13554, 24278, -26570, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13554;
DELETE FROM node_tags WHERE node_id = 13554;
DELETE FROM current_way_nodes WHERE node_id=13554;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13554;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13554;
/* delete node 13555*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13555, 24235, -11706, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13555;
DELETE FROM node_tags WHERE node_id = 13555;
DELETE FROM current_way_nodes WHERE node_id=13555;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13555;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13555;
/* delete node 13558*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13558, 24296, -26872, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13558;
DELETE FROM node_tags WHERE node_id = 13558;
DELETE FROM current_way_nodes WHERE node_id=13558;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13558;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13558;
/* delete node 13564*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13564, 24295, -10022, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13564;
DELETE FROM node_tags WHERE node_id = 13564;
DELETE FROM current_way_nodes WHERE node_id=13564;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13564;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13564;
/* delete node 13586*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13586, 24850, -25719, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13586;
DELETE FROM node_tags WHERE node_id = 13586;
DELETE FROM current_way_nodes WHERE node_id=13586;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13586;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13586;
/* delete node 13598*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13598, 24902, -26560, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13598;
DELETE FROM node_tags WHERE node_id = 13598;
DELETE FROM current_way_nodes WHERE node_id=13598;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13598;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13598;
/* delete node 13603*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (13603, 24972, -11388, 2, false, (now() at time zone 'utc'), 1789569706, 2);
DELETE FROM current_node_tags WHERE node_id = 13603;
DELETE FROM node_tags WHERE node_id = 13603;
DELETE FROM current_way_nodes WHERE node_id=13603;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 13603;
UPDATE current_nodes SET changeset_id=2, visible=false, version=2 WHERE id=13603;
/* create node 2067713030*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067713030, 424865169, -714688236, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067713030, 424865169, -714688236, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067713231*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067713231, 424865491, -714689048, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067713231, 424865491, -714689048, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067713334*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067713334, 424865628, -714688949, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067713334, 424865628, -714688949, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067713561*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067713561, 424866114, -714690171, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067713561, 424866114, -714690171, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067713617*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067713617, 424866256, -714657364, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067713617, 424866256, -714657364, 2, true, (now() at time zone 'utc'), 1710365944, 1);
/* create node 2067713628*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067713628, 424866298, -714687417, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067713628, 424866298, -714687417, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067713768*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067713768, 424866519, -714656882, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067713768, 424866519, -714656882, 2, true, (now() at time zone 'utc'), 1710365944, 1);
/* create node 2067713779*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067713779, 424866544, -714688344, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067713779, 424866544, -714688344, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067713791*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067713791, 424866588, -714676313, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067713791, 424866588, -714676313, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067713820*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067713820, 424866639, -714688275, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067713820, 424866639, -714688275, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067713841*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067713841, 424866711, -714689196, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067713841, 424866711, -714689196, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067713952*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067713952, 424866766, -714657128, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067713952, 424866766, -714657128, 2, true, (now() at time zone 'utc'), 1710365944, 1);
/* create node 2067714004*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067714004, 424866844, -714689100, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067714004, 424866844, -714689100, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067714014*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067714014, 424866879, -714689617, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067714014, 424866879, -714689617, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067714095*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067714095, 424866985, -714676737, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067714095, 424866985, -714676737, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067714171*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067714171, 424867103, -714676537, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067714171, 424867103, -714676537, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067714215*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067714215, 424867175, -714658278, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067714215, 424867175, -714658278, 2, true, (now() at time zone 'utc'), 1710365944, 1);
/* create node 2067714248*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067714248, 424867243, -714656253, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067714248, 424867243, -714656253, 2, true, (now() at time zone 'utc'), 1710365944, 1);
/* create node 2067714272*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067714272, 424867299, -714677083, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067714272, 424867299, -714677083, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067714301*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067714301, 424867356, -714656365, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067714301, 424867356, -714656365, 2, true, (now() at time zone 'utc'), 1710365944, 1);
/* create node 2067714327*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067714327, 424867420, -714676876, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067714327, 424867420, -714676876, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067714349*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067714349, 424867475, -714674794, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067714349, 424867475, -714674794, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067714420*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067714420, 424867664, -714677472, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067714420, 424867664, -714677472, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067714457*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067714457, 424867706, -714675952, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067714457, 424867706, -714675952, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067714492*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067714492, 424867757, -714655628, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067714492, 424867757, -714655628, 2, true, (now() at time zone 'utc'), 1710365944, 1);
/* create node 2067714500*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067714500, 424867779, -714694817, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067714500, 424867779, -714694817, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067714510*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067714510, 424867795, -714677248, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067714510, 424867795, -714677248, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067714540*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067714540, 424867869, -714657005, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067714540, 424867869, -714657005, 2, true, (now() at time zone 'utc'), 1710365944, 1);
/* create node 2067714657*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067714657, 424867991, -714657126, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067714657, 424867991, -714657126, 2, true, (now() at time zone 'utc'), 1710365944, 1);
/* create node 2067714677*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067714677, 424868014, -714667933, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067714677, 424868014, -714667933, 2, true, (now() at time zone 'utc'), 1710365944, 1);
/* create node 2067714684*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067714684, 424868034, -714675391, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067714684, 424868034, -714675391, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067714709*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067714709, 424868110, -714677584, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067714709, 424868110, -714677584, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067714746*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067714746, 424868134, -714682728, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067714746, 424868134, -714682728, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067714783*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067714783, 424868189, -714694527, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067714783, 424868189, -714694527, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067714830*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067714830, 424868299, -714668813, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067714830, 424868299, -714668813, 2, true, (now() at time zone 'utc'), 1710365944, 1);
/* create node 2067714861*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067714861, 424868369, -714694991, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067714861, 424868369, -714694991, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067714894*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067714894, 424868438, -714656306, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067714894, 424868438, -714656306, 2, true, (now() at time zone 'utc'), 1710365944, 1);
/* create node 2067714936*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067714936, 424868525, -714667044, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067714936, 424868525, -714667044, 2, true, (now() at time zone 'utc'), 1710365944, 1);
/* create node 2067714938*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067714938, 424868542, -714676844, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067714938, 424868542, -714676844, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067714949*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067714949, 424868571, -714696865, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067714949, 424868571, -714696865, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067714998*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067714998, 424868656, -714694788, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067714998, 424868656, -714694788, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067715017*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067715017, 424868684, -714667536, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067715017, 424868684, -714667536, 2, true, (now() at time zone 'utc'), 1710365944, 1);
/* create node 2067715035*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067715035, 424868730, -714668559, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067715035, 424868730, -714668559, 2, true, (now() at time zone 'utc'), 1710365944, 1);
/* create node 2067715051*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067715051, 424868747, -714684167, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067715051, 424868747, -714684167, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067715061*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067715061, 424868771, -714668687, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067715061, 424868771, -714668687, 2, true, (now() at time zone 'utc'), 1710365944, 1);
/* create node 2067715111*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067715111, 424868879, -714682149, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067715111, 424868879, -714682149, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067715125*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067715125, 424868912, -714684038, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067715125, 424868912, -714684038, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067715142*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067715142, 424868969, -714666781, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067715142, 424868969, -714666781, 2, true, (now() at time zone 'utc'), 1710365944, 1);
/* create node 2067715177*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067715177, 424869141, -714667311, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067715177, 424869141, -714667311, 2, true, (now() at time zone 'utc'), 1710365944, 1);
/* create node 2067715222*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067715222, 424869198, -714684710, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067715222, 424869198, -714684710, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067715241*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067715241, 424869269, -714696373, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067715241, 424869269, -714696373, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067715265*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067715265, 424869397, -714668317, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067715265, 424869397, -714668317, 2, true, (now() at time zone 'utc'), 1710365944, 1);
/* create node 2067715283*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067715283, 424869452, -714668487, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067715283, 424869452, -714668487, 2, true, (now() at time zone 'utc'), 1710365944, 1);
/* create node 2067715410*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067715410, 424869728, -714684298, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067715410, 424869728, -714684298, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067715500*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067715500, 424869891, -714666868, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067715500, 424869891, -714666868, 2, true, (now() at time zone 'utc'), 1710365944, 1);
/* create node 2067715605*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067715605, 424870261, -714668009, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067715605, 424870261, -714668009, 2, true, (now() at time zone 'utc'), 1710365944, 1);
/* create node 2067715648*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067715648, 424870346, -714682327, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067715648, 424870346, -714682327, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067715861*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067715861, 424870751, -714672870, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067715861, 424870751, -714672870, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067715876*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067715876, 424870777, -714683187, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067715876, 424870777, -714683187, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067715926*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067715926, 424870908, -714658446, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067715926, 424870908, -714658446, 2, true, (now() at time zone 'utc'), 1710365944, 1);
/* create node 2067715964*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067715964, 424870971, -714681456, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067715964, 424870971, -714681456, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067716011*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716011, 424871062, -714673809, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716011, 424871062, -714673809, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067716015*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716015, 424871074, -714681662, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716015, 424871074, -714681662, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067716083*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716083, 424871254, -714688215, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716083, 424871254, -714688215, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067716108*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716108, 424871349, -714682167, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716108, 424871349, -714682167, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067716114*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716114, 424871360, -714657606, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716114, 424871360, -714657606, 2, true, (now() at time zone 'utc'), 1710365944, 1);
/* create node 2067716130*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716130, 424871394, -714688654, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716130, 424871394, -714688654, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067716198*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716198, 424871520, -714682509, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716198, 424871520, -714682509, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067716225*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716225, 424871622, -714691785, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716225, 424871622, -714691785, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067716283*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716283, 424871722, -714656868, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716283, 424871722, -714656868, 2, true, (now() at time zone 'utc'), 1710365944, 1);
/* create node 2067716289*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716289, 424871729, -714688459, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716289, 424871729, -714688459, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067716312*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716312, 424871799, -714680700, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716312, 424871799, -714680700, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067716338*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716338, 424871887, -714688890, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716338, 424871887, -714688890, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067716347*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716347, 424871890, -714659328, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716347, 424871890, -714659328, 2, true, (now() at time zone 'utc'), 1710365944, 1);
/* create node 2067716389*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716389, 424872064, -714692770, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716389, 424872064, -714692770, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067716395*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716395, 424872092, -714658176, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716395, 424872092, -714658176, 2, true, (now() at time zone 'utc'), 1710365944, 1);
/* create node 2067716412*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716412, 424872153, -714665778, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716412, 424872153, -714665778, 2, true, (now() at time zone 'utc'), 1710365944, 1);
/* create node 2067716414*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716414, 424872162, -714681425, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716414, 424872162, -714681425, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067716456*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716456, 424872264, -714691259, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716456, 424872264, -714691259, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067716493*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716493, 424872340, -714692544, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716493, 424872340, -714692544, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067716496*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716496, 424872344, -714658402, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716496, 424872344, -714658402, 2, true, (now() at time zone 'utc'), 1710365944, 1);
/* create node 2067716510*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716510, 424872396, -714687550, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716510, 424872396, -714687550, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067716518*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716518, 424872425, -714657497, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716518, 424872425, -714657497, 2, true, (now() at time zone 'utc'), 1710365944, 1);
/* create node 2067716520*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716520, 424872430, -714687656, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716520, 424872430, -714687656, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067716540*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716540, 424872482, -714671824, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716540, 424872482, -714671824, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067716569*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716569, 424872547, -714693005, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716569, 424872547, -714693005, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067716575*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716575, 424872566, -714667023, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716575, 424872566, -714667023, 2, true, (now() at time zone 'utc'), 1710365944, 1);
/* create node 2067716616*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716616, 424872665, -714694296, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716616, 424872665, -714694296, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067716621*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716621, 424872677, -714688430, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716621, 424872677, -714688430, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067716624*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716624, 424872680, -714692187, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716624, 424872680, -714692187, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067716637*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716637, 424872744, -714665420, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716637, 424872744, -714665420, 2, true, (now() at time zone 'utc'), 1710365944, 1);
/* create node 2067716641*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716641, 424872785, -714695619, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716641, 424872785, -714695619, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067716643*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716643, 424872793, -714672763, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716643, 424872793, -714672763, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067716682*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716682, 424872839, -714665704, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716682, 424872839, -714665704, 2, true, (now() at time zone 'utc'), 1710365944, 1);
/* create node 2067716694*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716694, 424872905, -714680274, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716694, 424872905, -714680274, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067716703*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716703, 424872916, -714691994, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716703, 424872916, -714691994, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067716737*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716737, 424873020, -714695492, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716737, 424873020, -714695492, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067716772*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716772, 424873113, -714696725, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716772, 424873113, -714696725, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067716784*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716784, 424873149, -714692512, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716784, 424873149, -714692512, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067716787*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716787, 424873150, -714677002, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716787, 424873150, -714677002, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067716793*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716793, 424873165, -714687211, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716793, 424873165, -714687211, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067716821*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716821, 424873237, -714666616, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716821, 424873237, -714666616, 2, true, (now() at time zone 'utc'), 1710365944, 1);
/* create node 2067716829*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716829, 424873271, -714693968, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716829, 424873271, -714693968, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067716840*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716840, 424873295, -714666792, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716840, 424873295, -714666792, 2, true, (now() at time zone 'utc'), 1710365944, 1);
/* create node 2067716849*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716849, 424873310, -714679999, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716849, 424873310, -714679999, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067716855*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716855, 424873322, -714681392, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716855, 424873322, -714681392, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067716866*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716866, 424873370, -714677410, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716866, 424873370, -714677410, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067716868*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716868, 424873377, -714687958, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716868, 424873377, -714687958, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067716875*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716875, 424873402, -714695247, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716875, 424873402, -714695247, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067716906*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716906, 424873469, -714680428, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716906, 424873469, -714680428, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067716936*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716936, 424873556, -714676602, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716936, 424873556, -714676602, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067716948*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716948, 424873616, -714695131, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716948, 424873616, -714695131, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067716981*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716981, 424873740, -714696386, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716981, 424873740, -714696386, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067716990*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716990, 424873776, -714677009, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067716990, 424873776, -714677009, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067717004*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717004, 424873821, -714681053, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717004, 424873821, -714681053, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067717016*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717016, 424873864, -714665083, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717016, 424873864, -714665083, 2, true, (now() at time zone 'utc'), 1710365944, 1);
/* create node 2067717037*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717037, 424873924, -714681329, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717037, 424873924, -714681329, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067717160*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717160, 424874121, -714670713, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717160, 424874121, -714670713, 2, true, (now() at time zone 'utc'), 1710365944, 1);
/* create node 2067717193*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717193, 424874241, -714666219, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717193, 424874241, -714666219, 2, true, (now() at time zone 'utc'), 1710365944, 1);
/* create node 2067717221*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717221, 424874318, -714670284, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717221, 424874318, -714670284, 2, true, (now() at time zone 'utc'), 1710365944, 1);
/* create node 2067717223*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717223, 424874322, -714679849, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717223, 424874322, -714679849, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067717227*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717227, 424874346, -714671331, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717227, 424874346, -714671331, 2, true, (now() at time zone 'utc'), 1710365944, 1);
/* create node 2067717244*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717244, 424874406, -714670525, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717244, 424874406, -714670525, 2, true, (now() at time zone 'utc'), 1710365944, 1);
/* create node 2067717353*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717353, 424874644, -714671134, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717353, 424874644, -714671134, 2, true, (now() at time zone 'utc'), 1710365944, 1);
/* create node 2067717362*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717362, 424874666, -714669750, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717362, 424874666, -714669750, 2, true, (now() at time zone 'utc'), 1710365944, 1);
/* create node 2067717377*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717377, 424874681, -714680814, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717377, 424874681, -714680814, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067717381*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717381, 424874691, -714671262, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717381, 424874691, -714671262, 2, true, (now() at time zone 'utc'), 1710365944, 1);
/* create node 2067717414*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717414, 424874755, -714669994, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717414, 424874755, -714669994, 2, true, (now() at time zone 'utc'), 1710365944, 1);
/* create node 2067717429*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717429, 424874803, -714669659, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717429, 424874803, -714669659, 2, true, (now() at time zone 'utc'), 1710365944, 1);
/* create node 2067717435*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717435, 424874825, -714686353, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717435, 424874825, -714686353, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067717468*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717468, 424874896, -714669917, 2, true, (now() at time zone 'utc'), 1710365944, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717468, 424874896, -714669917, 2, true, (now() at time zone 'utc'), 1710365944, 1);
/* create node 2067717493*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717493, 424874965, -714697611, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717493, 424874965, -714697611, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067717582*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717582, 424875197, -714687289, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717582, 424875197, -714687289, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067717591*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717591, 424875231, -714679164, 2, true, (now() at time zone 'utc'), 1710365938, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717591, 424875231, -714679164, 2, true, (now() at time zone 'utc'), 1710365938, 1);
/* create node 2067717643*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717643, 424875329, -714678471, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717643, 424875329, -714678471, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067717645*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717645, 424875338, -714698746, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717645, 424875338, -714698746, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067717686*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717686, 424875485, -714669527, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717686, 424875485, -714669527, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067717698*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717698, 424875515, -714656512, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717698, 424875515, -714656512, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067717707*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717707, 424875540, -714657717, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717707, 424875540, -714657717, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067717710*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717710, 424875540, -714678855, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717710, 424875540, -714678855, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067717740*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717740, 424875621, -714679873, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717740, 424875621, -714679873, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067717749*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717749, 424875637, -714678163, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717749, 424875637, -714678163, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067717780*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717780, 424875714, -714685710, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717780, 424875714, -714685710, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067717785*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717785, 424875731, -714658406, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717785, 424875731, -714658406, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067717799*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717799, 424875783, -714678429, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717799, 424875783, -714678429, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067717811*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717811, 424875810, -714657580, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717811, 424875810, -714657580, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067717823*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717823, 424875838, -714670502, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717823, 424875838, -714670502, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067717838*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717838, 424875866, -714686094, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717838, 424875866, -714686094, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067717841*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717841, 424875876, -714678336, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717841, 424875876, -714678336, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067717859*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717859, 424875919, -714663561, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717859, 424875919, -714663561, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067717874*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717874, 424875946, -714678462, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717874, 424875946, -714678462, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067717917*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717917, 424876008, -714658266, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717917, 424876008, -714658266, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067717922*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717922, 424876031, -714685747, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717922, 424876031, -714685747, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067717938*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717938, 424876101, -714685923, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717938, 424876101, -714685923, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067717947*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717947, 424876121, -714658673, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717947, 424876121, -714658673, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067717959*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717959, 424876153, -714656189, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717959, 424876153, -714656189, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067717983*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717983, 424876196, -714678211, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067717983, 424876196, -714678211, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067718047*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718047, 424876295, -714697858, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718047, 424876295, -714697858, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067718070*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718070, 424876356, -714656922, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718070, 424876356, -714656922, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067718079*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718079, 424876374, -714679118, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718079, 424876374, -714679118, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067718088*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718088, 424876381, -714698121, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718088, 424876381, -714698121, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067718091*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718091, 424876382, -714678551, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718091, 424876382, -714678551, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067718166*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718166, 424876556, -714696656, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718166, 424876556, -714696656, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067718169*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718169, 424876559, -714685365, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718169, 424876559, -714685365, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067718172*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718172, 424876568, -714656815, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718172, 424876568, -714656815, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067718176*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718176, 424876582, -714679499, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718176, 424876582, -714679499, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067718239*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718239, 424876736, -714664374, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718239, 424876736, -714664374, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067718262*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718262, 424876773, -714679307, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718262, 424876773, -714679307, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067718277*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718277, 424876822, -714661905, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718277, 424876822, -714661905, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067718292*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718292, 424876833, -714679415, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718292, 424876833, -714679415, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067718295*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718295, 424876843, -714697529, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718295, 424876843, -714697529, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067718304*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718304, 424876848, -714686094, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718304, 424876848, -714686094, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067718348*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718348, 424876964, -714658246, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718348, 424876964, -714658246, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067718354*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718354, 424876976, -714677955, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718354, 424876976, -714677955, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067718439*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718439, 424877225, -714678866, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718439, 424877225, -714678866, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067718455*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718455, 424877280, -714678967, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718455, 424877280, -714678967, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067718458*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718458, 424877302, -714683900, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718458, 424877302, -714683900, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067718479*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718479, 424877342, -714663200, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718479, 424877342, -714663200, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067718491*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718491, 424877387, -714678704, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718491, 424877387, -714678704, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067718506*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718506, 424877424, -714663295, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718506, 424877424, -714663295, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067718561*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718561, 424877585, -714662805, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718561, 424877585, -714662805, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067718695*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718695, 424877686, -714685220, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718695, 424877686, -714685220, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067718738*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718738, 424877810, -714675442, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718738, 424877810, -714675442, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067718775*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718775, 424877930, -714676613, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718775, 424877930, -714676613, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067718821*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718821, 424878043, -714695753, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718821, 424878043, -714695753, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067718853*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718853, 424878130, -714683461, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718853, 424878130, -714683461, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067718868*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718868, 424878179, -714687028, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718868, 424878179, -714687028, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067718875*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718875, 424878205, -714676407, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718875, 424878205, -714676407, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067718893*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718893, 424878279, -714687458, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718893, 424878279, -714687458, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067718899*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718899, 424878292, -714684016, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718899, 424878292, -714684016, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067718902*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718902, 424878298, -714684895, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718902, 424878298, -714684895, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067718911*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718911, 424878346, -714685059, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718911, 424878346, -714685059, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067718917*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718917, 424878363, -714695474, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718917, 424878363, -714695474, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067718941*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718941, 424878411, -714674992, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718941, 424878411, -714674992, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067718947*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718947, 424878416, -714676284, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718947, 424878416, -714676284, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067718962*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718962, 424878450, -714677885, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718962, 424878450, -714677885, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067718965*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718965, 424878452, -714695660, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718965, 424878452, -714695660, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067718983*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718983, 424878490, -714696690, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718983, 424878490, -714696690, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067718995*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718995, 424878531, -714683715, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067718995, 424878531, -714683715, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067719016*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719016, 424878572, -714694948, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719016, 424878572, -714694948, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067719019*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719019, 424878575, -714683866, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719019, 424878575, -714683866, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067719022*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719022, 424878598, -714686849, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719022, 424878598, -714686849, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067719049*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719049, 424878639, -714676830, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719049, 424878639, -714676830, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067719067*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719067, 424878698, -714687279, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719067, 424878698, -714687279, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067719117*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719117, 424878749, -714676747, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719117, 424878749, -714676747, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067719120*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719120, 424878754, -714684509, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719120, 424878754, -714684509, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067719126*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719126, 424878777, -714695377, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719126, 424878777, -714695377, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067719148*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719148, 424878817, -714675984, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719148, 424878817, -714675984, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067719152*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719152, 424878838, -714684798, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719152, 424878838, -714684798, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067719182*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719182, 424878890, -714696052, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719182, 424878890, -714696052, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067719224*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719224, 424878988, -714696257, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719224, 424878988, -714696257, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067719235*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719235, 424878995, -714695960, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719235, 424878995, -714695960, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067719243*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719243, 424879036, -714677448, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719243, 424879036, -714677448, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067719296*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719296, 424879092, -714683417, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719296, 424879092, -714683417, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067719310*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719310, 424879113, -714696207, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719310, 424879113, -714696207, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067719384*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719384, 424879296, -714694317, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719384, 424879296, -714694317, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067719387*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719387, 424879322, -714684208, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719387, 424879322, -714684208, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067719426*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719426, 424879491, -714694726, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719426, 424879491, -714694726, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067719461*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719461, 424879526, -714695848, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719461, 424879526, -714695848, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067719482*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719482, 424879575, -714695951, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719482, 424879575, -714695951, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067719488*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719488, 424879587, -714682497, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719488, 424879587, -714682497, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067719527*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719527, 424879737, -714694512, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719527, 424879737, -714694512, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067719553*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719553, 424879779, -714682153, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719553, 424879779, -714682153, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067719581*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719581, 424879804, -714683220, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719581, 424879804, -714683220, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067719596*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719596, 424879817, -714668130, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719596, 424879817, -714668130, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067719605*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719605, 424879841, -714682358, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719605, 424879841, -714682358, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067719690*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719690, 424880175, -714695429, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719690, 424880175, -714695429, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067719795*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719795, 424880514, -714666222, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719795, 424880514, -714666222, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067719818*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719818, 424880595, -714681707, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719818, 424880595, -714681707, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067719834*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719834, 424880629, -714681823, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719834, 424880629, -714681823, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067719871*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719871, 424880730, -714672710, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719871, 424880730, -714672710, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067719907*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719907, 424880843, -714693892, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719907, 424880843, -714693892, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067719916*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719916, 424880863, -714659659, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719916, 424880863, -714659659, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067719938*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719938, 424880925, -714660401, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719938, 424880925, -714660401, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067719955*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719955, 424880964, -714668135, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719955, 424880964, -714668135, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067719985*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719985, 424881038, -714668944, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719985, 424881038, -714668944, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067719988*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719988, 424881045, -714660343, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067719988, 424881045, -714660343, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067720044*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720044, 424881152, -714673800, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720044, 424881152, -714673800, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067720047*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720047, 424881154, -714694558, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720047, 424881154, -714694558, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067720065*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720065, 424881211, -714681505, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720065, 424881211, -714681505, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067720089*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720089, 424881261, -714668334, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720089, 424881261, -714668334, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067720130*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720130, 424881393, -714662160, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720130, 424881393, -714662160, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067720136*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720136, 424881415, -714666899, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720136, 424881415, -714666899, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067720142*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720142, 424881434, -714672213, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720142, 424881434, -714672213, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067720145*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720145, 424881435, -714659382, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720145, 424881435, -714659382, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067720148*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720148, 424881442, -714694313, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720148, 424881442, -714694313, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067720151*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720151, 424881444, -714673594, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720151, 424881444, -714673594, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067720154*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720154, 424881455, -714682317, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720154, 424881455, -714682317, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067720171*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720171, 424881491, -714674103, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720171, 424881491, -714674103, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067720185*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720185, 424881558, -714694562, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720185, 424881558, -714694562, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067720202*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720202, 424881608, -714674020, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720202, 424881608, -714674020, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067720204*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720204, 424881612, -714660046, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720204, 424881612, -714660046, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067720348*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720348, 424881770, -714661270, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720348, 424881770, -714661270, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067720351*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720351, 424881772, -714674829, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720351, 424881772, -714674829, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067720354*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720354, 424881775, -714673410, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720354, 424881775, -714673410, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067720372*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720372, 424881842, -714693040, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720372, 424881842, -714693040, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067720375*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720375, 424881871, -714673342, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720375, 424881871, -714673342, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067720402*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720402, 424881920, -714673785, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720402, 424881920, -714673785, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067720408*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720408, 424881936, -714661896, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720408, 424881936, -714661896, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067720437*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720437, 424882060, -714659853, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720437, 424882060, -714659853, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067720446*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720446, 424882086, -714673668, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720446, 424882086, -714673668, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067720495*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720495, 424882270, -714693955, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720495, 424882270, -714693955, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067720525*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720525, 424882360, -714660983, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720525, 424882360, -714660983, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067720534*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720534, 424882371, -714674405, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720534, 424882371, -714674405, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067720551*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720551, 424882441, -714682583, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720551, 424882441, -714682583, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067720713*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720713, 424882861, -714682020, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720713, 424882861, -714682020, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067720732*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720732, 424882909, -714683780, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720732, 424882909, -714683780, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067720738*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720738, 424882941, -714682227, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720738, 424882941, -714682227, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067720819*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720819, 424883231, -714681481, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720819, 424883231, -714681481, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067720840*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720840, 424883284, -714682972, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720840, 424883284, -714682972, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067720845*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720845, 424883316, -714681697, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720845, 424883316, -714681697, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067720893*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720893, 424883449, -714683395, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720893, 424883449, -714683395, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067720902*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720902, 424883526, -714673770, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720902, 424883526, -714673770, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067720939*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720939, 424883596, -714697731, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720939, 424883596, -714697731, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067720962*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720962, 424883660, -714691886, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067720962, 424883660, -714691886, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067721001*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721001, 424883801, -714681075, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721001, 424883801, -714681075, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067721022*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721022, 424883874, -714698523, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721022, 424883874, -714698523, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067721032*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721032, 424883886, -714692502, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721032, 424883886, -714692502, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067721041*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721041, 424883929, -714674626, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721041, 424883929, -714674626, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067721067*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721067, 424884035, -714673333, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721067, 424884035, -714673333, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067721076*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721076, 424884037, -714692400, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721076, 424884037, -714692400, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067721106*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721106, 424884098, -714697340, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721106, 424884098, -714697340, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067721112*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721112, 424884101, -714674478, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721112, 424884101, -714674478, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067721141*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721141, 424884147, -714674577, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721141, 424884147, -714674577, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067721147*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721147, 424884159, -714692731, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721147, 424884159, -714692731, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067721161*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721161, 424884173, -714673626, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721161, 424884173, -714673626, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067721216*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721216, 424884256, -714697757, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721216, 424884256, -714697757, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067721219*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721219, 424884268, -714682271, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721219, 424884268, -714682271, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067721234*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721234, 424884293, -714697238, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721234, 424884293, -714697238, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067721270*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721270, 424884356, -714692599, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721270, 424884356, -714692599, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067721298*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721298, 424884416, -714692765, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721298, 424884416, -714692765, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067721314*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721314, 424884441, -714697629, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721314, 424884441, -714697629, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067721421*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721421, 424884650, -714674145, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721421, 424884650, -714674145, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067721436*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721436, 424884711, -714692395, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721436, 424884711, -714692395, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067721438*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721438, 424884714, -714691179, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721438, 424884714, -714691179, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067721446*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721446, 424884741, -714674338, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721446, 424884741, -714674338, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067721449*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721449, 424884761, -714692533, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721449, 424884761, -714692533, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067721546*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721546, 424884976, -714678917, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721546, 424884976, -714678917, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067721549*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721549, 424884976, -714697940, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721549, 424884976, -714697940, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067721560*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721560, 424885050, -714698194, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721560, 424885050, -714698194, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067721570*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721570, 424885073, -714692153, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721570, 424885073, -714692153, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067721590*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721590, 424885117, -714672815, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721590, 424885117, -714672815, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067721631*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721631, 424885188, -714696620, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721631, 424885188, -714696620, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067721683*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721683, 424885291, -714679780, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721683, 424885291, -714679780, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067721774*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721774, 424885519, -714673669, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721774, 424885519, -714673669, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067721786*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721786, 424885566, -714697920, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721786, 424885566, -714697920, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067721832*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721832, 424885728, -714678418, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721832, 424885728, -714678418, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067721967*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721967, 424885855, -714678767, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721967, 424885855, -714678767, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067721990*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721990, 424885946, -714678297, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067721990, 424885946, -714678297, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067722012*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722012, 424885993, -714690542, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722012, 424885993, -714690542, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067722018*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722018, 424885999, -714696113, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722018, 424885999, -714696113, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067722024*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722024, 424886005, -714679305, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722024, 424886005, -714679305, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067722036*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722036, 424886041, -714690357, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722036, 424886041, -714690357, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067722048*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722048, 424886066, -714678627, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722048, 424886066, -714678627, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067722060*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722060, 424886086, -714690478, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722060, 424886086, -714690478, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067722064*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722064, 424886089, -714679538, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722064, 424886089, -714679538, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067722140*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722140, 424886249, -714691219, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722140, 424886249, -714691219, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067722160*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722160, 424886293, -714679054, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722160, 424886293, -714679054, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067722169*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722169, 424886312, -714671796, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722169, 424886312, -714671796, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067722181*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722181, 424886331, -714678041, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722181, 424886331, -714678041, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067722192*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722192, 424886340, -714691156, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722192, 424886340, -714691156, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067722198*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722198, 424886390, -714691287, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722198, 424886390, -714691287, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067722209*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722209, 424886395, -714679335, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722209, 424886395, -714679335, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067722227*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722227, 424886465, -714697368, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722227, 424886465, -714697368, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067722253*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722253, 424886621, -714678837, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722253, 424886621, -714678837, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067722271*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722271, 424886692, -714671540, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722271, 424886692, -714671540, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067722301*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722301, 424886846, -714673242, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722301, 424886846, -714673242, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067722317*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722317, 424886865, -714672010, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722317, 424886865, -714672010, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067722350*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722350, 424886935, -714697050, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722350, 424886935, -714697050, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067722361*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722361, 424886999, -714697223, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722361, 424886999, -714697223, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067722367*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722367, 424887032, -714677654, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722367, 424887032, -714677654, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067722371*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722371, 424887058, -714689655, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722371, 424887058, -714689655, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067722417*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722417, 424887227, -714670644, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722417, 424887227, -714670644, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067722487*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722487, 424887388, -714695172, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722487, 424887388, -714695172, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067722493*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722493, 424887409, -714690584, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722493, 424887409, -714690584, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067722496*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722496, 424887416, -714696941, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722496, 424887416, -714696941, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067722499*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722499, 424887427, -714672256, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722499, 424887427, -714672256, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067722509*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722509, 424887442, -714678562, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722509, 424887442, -714678562, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067722511*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722511, 424887460, -714697060, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722511, 424887460, -714697060, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067722515*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722515, 424887476, -714671386, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722515, 424887476, -714671386, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067722540*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722540, 424887537, -714696282, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722540, 424887537, -714696282, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067722559*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722559, 424887578, -714671530, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722559, 424887578, -714671530, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067722583*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722583, 424887603, -714672732, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722583, 424887603, -714672732, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067722637*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722637, 424887748, -714696140, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722637, 424887748, -714696140, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067722640*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722640, 424887752, -714696862, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722640, 424887752, -714696862, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067722646*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722646, 424887762, -714672030, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722646, 424887762, -714672030, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067722649*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722649, 424887806, -714670290, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722649, 424887806, -714670290, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067722701*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722701, 424888002, -714689278, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722701, 424888002, -714689278, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067722722*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722722, 424888055, -714671031, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722722, 424888055, -714671031, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067722728*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722728, 424888066, -714676803, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722728, 424888066, -714676803, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067722754*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722754, 424888139, -714677989, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722754, 424888139, -714677989, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067722789*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722789, 424888228, -714669077, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722789, 424888228, -714669077, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067722803*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722803, 424888249, -714688346, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722803, 424888249, -714688346, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067722809*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722809, 424888262, -714678260, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722809, 424888262, -714678260, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067722811*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722811, 424888263, -714690017, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722811, 424888263, -714690017, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067722820*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722820, 424888292, -714677843, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722820, 424888292, -714677843, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067722863*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722863, 424888421, -714678129, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722863, 424888421, -714678129, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067722881*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722881, 424888469, -714677697, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722881, 424888469, -714677697, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067722884*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722884, 424888471, -714688976, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722884, 424888471, -714688976, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067722887*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722887, 424888474, -714677464, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722887, 424888474, -714677464, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067722901*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722901, 424888493, -714669893, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722901, 424888493, -714669893, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067722910*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722910, 424888504, -714694518, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722910, 424888504, -714694518, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067722959*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722959, 424888637, -714688096, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067722959, 424888637, -714688096, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067723004*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723004, 424888784, -714688511, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723004, 424888784, -714688511, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067723016*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723016, 424888805, -714678223, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723016, 424888805, -714678223, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067723023*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723023, 424888812, -714695346, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723023, 424888812, -714695346, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067723036*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723036, 424888873, -714668696, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723036, 424888873, -714668696, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067723053*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723053, 424888884, -714677138, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723053, 424888884, -714677138, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067723068*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723068, 424888919, -714669491, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723068, 424888919, -714669491, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067723078*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723078, 424888960, -714669616, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723078, 424888960, -714669616, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067723093*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723093, 424889037, -714669939, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723093, 424889037, -714669939, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067723101*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723101, 424889067, -714693976, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723101, 424889067, -714693976, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067723104*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723104, 424889078, -714688321, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723104, 424889078, -714688321, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067723114*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723114, 424889097, -714669385, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723114, 424889097, -714669385, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067723122*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723122, 424889114, -714694104, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723122, 424889114, -714694104, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067723143*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723143, 424889215, -714677898, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723143, 424889215, -714677898, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067723146*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723146, 424889226, -714688740, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723146, 424889226, -714688740, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067723154*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723154, 424889313, -714670808, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723154, 424889313, -714670808, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067723156*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723156, 424889335, -714688669, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723156, 424889335, -714688669, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067723158*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723158, 424889342, -714676434, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723158, 424889342, -714676434, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067723189*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723189, 424889499, -714694908, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723189, 424889499, -714694908, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067723195*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723195, 424889524, -714689205, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723195, 424889524, -714689205, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067723197*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723197, 424889530, -714669094, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723197, 424889530, -714669094, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067723200*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723200, 424889559, -714693642, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723200, 424889559, -714693642, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067723215*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723215, 424889617, -714677223, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723215, 424889617, -714677223, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067723220*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723220, 424889634, -714675894, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723220, 424889634, -714675894, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067723273*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723273, 424889680, -714669566, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723273, 424889680, -714669566, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067723290*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723290, 424889735, -714676184, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723290, 424889735, -714676184, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067723300*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723300, 424889758, -714694180, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723300, 424889758, -714694180, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067723321*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723321, 424889812, -714668930, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723321, 424889812, -714668930, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067723347*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723347, 424889878, -714695977, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723347, 424889878, -714695977, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067723359*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723359, 424889913, -714676723, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723359, 424889913, -714676723, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067723405*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723405, 424890000, -714694825, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723405, 424890000, -714694825, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067723409*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723409, 424890002, -714676978, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723409, 424890002, -714676978, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067723540*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723540, 424890131, -714693927, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723540, 424890131, -714693927, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067723590*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723590, 424890238, -714670272, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723590, 424890238, -714670272, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067723626*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723626, 424890317, -714695679, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723626, 424890317, -714695679, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067723641*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723641, 424890371, -714694573, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723641, 424890371, -714694573, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067723661*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723661, 424890425, -714675392, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723661, 424890425, -714675392, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067723715*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723715, 424890550, -714675752, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723715, 424890550, -714675752, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067723726*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723726, 424890567, -714696217, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723726, 424890567, -714696217, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067723761*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723761, 424890697, -714676224, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723761, 424890697, -714676224, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067723797*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723797, 424890814, -714676561, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723797, 424890814, -714676561, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067723816*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723816, 424890887, -714697206, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723816, 424890887, -714697206, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067723825*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723825, 424890927, -714668573, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723825, 424890927, -714668573, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067723833*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723833, 424890951, -714687053, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723833, 424890951, -714687053, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067723916*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723916, 424891145, -714693081, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723916, 424891145, -714693081, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067723923*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723923, 424891209, -714695837, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723923, 424891209, -714695837, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067723929*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723929, 424891216, -714687994, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723929, 424891216, -714687994, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067723945*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723945, 424891301, -714669507, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723945, 424891301, -714669507, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067723961*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723961, 424891349, -714675245, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723961, 424891349, -714675245, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067723975*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723975, 424891406, -714693956, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723975, 424891406, -714693956, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067723999*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723999, 424891494, -714669366, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067723999, 424891494, -714669366, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067724004*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724004, 424891529, -714696827, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724004, 424891529, -714696827, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067724050*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724050, 424891612, -714669659, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724050, 424891612, -714669659, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067724062*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724062, 424891627, -714676044, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724062, 424891627, -714676044, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067724099*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724099, 424891773, -714686631, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724099, 424891773, -714686631, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067724101*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724101, 424891812, -714693735, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724101, 424891812, -714693735, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067724115*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724115, 424891928, -714694123, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724115, 424891928, -714694123, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067724131*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724131, 424891991, -714674540, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724131, 424891991, -714674540, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067724151*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724151, 424892037, -714687572, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724151, 424892037, -714687572, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067724198*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724198, 424892198, -714692509, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724198, 424892198, -714692509, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067724201*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724201, 424892208, -714667636, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724201, 424892208, -714667636, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067724210*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724210, 424892280, -714673979, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724210, 424892280, -714673979, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067724219*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724219, 424892321, -714692923, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724219, 424892321, -714692923, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067724225*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724225, 424892347, -714675397, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724225, 424892347, -714675397, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067724254*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724254, 424892379, -714668766, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724254, 424892379, -714668766, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067724260*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724260, 424892388, -714674239, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724260, 424892388, -714674239, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067724295*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724295, 424892482, -714669023, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724295, 424892482, -714669023, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067724330*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724330, 424892597, -714668607, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724330, 424892597, -714668607, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067724342*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724342, 424892635, -714692753, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724342, 424892635, -714692753, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067724366*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724366, 424892713, -714675120, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724366, 424892713, -714675120, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067724428*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724428, 424892852, -714675454, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724428, 424892852, -714675454, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067724437*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724437, 424892888, -714693601, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724437, 424892888, -714693601, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067724511*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724511, 424893001, -714673433, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724511, 424893001, -714673433, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067724556*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724556, 424893174, -714673851, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724556, 424893174, -714673851, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067724637*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724637, 424893273, -714675136, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724637, 424893273, -714675136, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067724676*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724676, 424893366, -714675360, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724676, 424893366, -714675360, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067724734*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724734, 424893438, -714674521, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724734, 424893438, -714674521, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067724751*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724751, 424893465, -714666773, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724751, 424893465, -714666773, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067724790*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724790, 424893540, -714673573, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724790, 424893540, -714673573, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067724801*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724801, 424893560, -714658193, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724801, 424893560, -714658193, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067724862*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724862, 424893687, -714675117, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724862, 424893687, -714675117, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067724890*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724890, 424893737, -714691941, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724890, 424893737, -714691941, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067724893*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724893, 424893741, -714692335, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724893, 424893741, -714692335, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067724907*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724907, 424893759, -714656984, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724907, 424893759, -714656984, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067724933*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724933, 424893815, -714674235, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724933, 424893815, -714674235, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067724943*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724943, 424893834, -714692287, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724943, 424893834, -714692287, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067724946*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724946, 424893841, -714691630, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724946, 424893841, -714691630, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067724968*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724968, 424893904, -714692913, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067724968, 424893904, -714692913, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067725054*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725054, 424893905, -714691854, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725054, 424893905, -714691854, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067725060*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725060, 424893922, -714667885, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725060, 424893922, -714667885, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067725063*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725063, 424893929, -714666424, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725063, 424893929, -714666424, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067725123*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725123, 424894018, -714692854, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725123, 424894018, -714692854, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067725153*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725153, 424894063, -714659322, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725153, 424894063, -714659322, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067725180*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725180, 424894096, -714666830, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725180, 424894096, -714666830, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067725182*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725182, 424894101, -714657753, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725182, 424894101, -714657753, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067725190*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725190, 424894132, -714693255, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725190, 424894132, -714693255, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067725227*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725227, 424894243, -714667645, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725227, 424894243, -714667645, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067725274*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725274, 424894320, -714667833, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725274, 424894320, -714667833, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067725307*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725307, 424894385, -714656475, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725307, 424894385, -714656475, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067725330*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725330, 424894440, -714691321, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725330, 424894440, -714691321, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067725343*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725343, 424894497, -714658708, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725343, 424894497, -714658708, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067725345*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725345, 424894503, -714672852, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725345, 424894503, -714672852, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067725351*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725351, 424894509, -714691565, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725351, 424894509, -714691565, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067725372*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725372, 424894582, -714658900, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725372, 424894582, -714658900, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067725399*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725399, 424894654, -714672201, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725399, 424894654, -714672201, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067725415*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725415, 424894656, -714667581, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725415, 424894656, -714667581, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067725433*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725433, 424894705, -714692566, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725433, 424894705, -714692566, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067725458*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725458, 424894769, -714673546, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725458, 424894769, -714673546, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067725460*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725460, 424894773, -714667866, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725460, 424894773, -714667866, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067725470*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725470, 424894802, -714692909, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725470, 424894802, -714692909, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067725474*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725474, 424894819, -714672631, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725474, 424894819, -714672631, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067725511*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725511, 424894948, -714673081, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725511, 424894948, -714673081, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067725558*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725558, 424895016, -714666139, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725558, 424895016, -714666139, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067725564*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725564, 424895018, -714691303, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725564, 424895018, -714691303, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067725576*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725576, 424895040, -714667354, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725576, 424895040, -714667354, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067725594*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725594, 424895050, -714673349, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725594, 424895050, -714673349, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067725628*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725628, 424895137, -714667592, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725628, 424895137, -714667592, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067725636*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725636, 424895145, -714658181, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725636, 424895145, -714658181, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067725696*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725696, 424895290, -714692265, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725696, 424895290, -714692265, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067725730*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725730, 424895403, -714667082, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725730, 424895403, -714667082, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067725732*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725732, 424895423, -714671662, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725732, 424895423, -714671662, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067725842*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725842, 424895751, -714672519, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067725842, 424895751, -714672519, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067726020*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726020, 424896246, -714665556, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726020, 424896246, -714665556, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067726041*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726041, 424896300, -714665242, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726041, 424896300, -714665242, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067726083*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726083, 424896385, -714665453, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726083, 424896385, -714665453, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067726138*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726138, 424896574, -714682940, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726138, 424896574, -714682940, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067726144*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726144, 424896581, -714666381, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726144, 424896581, -714666381, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067726170*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726170, 424896640, -714664990, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726170, 424896640, -714664990, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067726181*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726181, 424896644, -714681904, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726181, 424896644, -714681904, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067726191*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726191, 424896709, -714665159, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726191, 424896709, -714665159, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067726221*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726221, 424896740, -714691393, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726221, 424896740, -714691393, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067726225*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726225, 424896771, -714664519, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726225, 424896771, -714664519, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067726303*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726303, 424896929, -714692686, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726303, 424896929, -714692686, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067726313*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726313, 424896956, -714664976, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726313, 424896956, -714664976, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067726315*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726315, 424896973, -714666091, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726315, 424896973, -714666091, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067726336*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726336, 424897024, -714663909, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726336, 424897024, -714663909, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067726345*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726345, 424897055, -714670542, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726345, 424897055, -714670542, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067726366*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726366, 424897087, -714666373, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726366, 424897087, -714666373, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067726405*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726405, 424897156, -714664234, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726405, 424897156, -714664234, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067726467*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726467, 424897307, -714663699, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726467, 424897307, -714663699, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067726483*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726483, 424897354, -714671337, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726483, 424897354, -714671337, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067726623*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726623, 424897453, -714683048, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726623, 424897453, -714683048, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067726647*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726647, 424897491, -714682494, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726647, 424897491, -714682494, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067726678*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726678, 424897613, -714665485, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726678, 424897613, -714665485, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067726714*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726714, 424897768, -714665868, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726714, 424897768, -714665868, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067726735*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726735, 424897823, -714682048, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726735, 424897823, -714682048, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067726744*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726744, 424897834, -714670007, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726744, 424897834, -714670007, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067726786*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726786, 424897887, -714681099, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726786, 424897887, -714681099, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067726793*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726793, 424897910, -714691081, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726793, 424897910, -714691081, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067726799*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726799, 424897922, -714670947, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726799, 424897922, -714670947, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067726808*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726808, 424897935, -714665246, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726808, 424897935, -714665246, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067726814*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726814, 424897942, -714670295, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726814, 424897942, -714670295, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067726831*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726831, 424897993, -714691654, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726831, 424897993, -714691654, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067726853*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726853, 424898079, -714692380, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726853, 424898079, -714692380, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067726868*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726868, 424898102, -714671426, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726868, 424898102, -714671426, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067726871*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726871, 424898115, -714692631, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726871, 424898115, -714692631, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067726973*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726973, 424898384, -714691550, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067726973, 424898384, -714691550, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067727012*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067727012, 424898447, -714682612, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067727012, 424898447, -714682612, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067727015*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067727015, 424898459, -714670969, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067727015, 424898459, -714670969, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067727038*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067727038, 424898523, -714671137, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067727038, 424898523, -714671137, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067727044*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067727044, 424898525, -714692522, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067727044, 424898525, -714692522, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067727065*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067727065, 424898544, -714681180, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067727065, 424898544, -714681180, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067727095*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067727095, 424898595, -714669847, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067727095, 424898595, -714669847, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067727098*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067727098, 424898598, -714663838, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067727098, 424898598, -714663838, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067727101*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067727101, 424898643, -714663438, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067727101, 424898643, -714663438, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067727171*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067727171, 424898756, -714663723, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067727171, 424898756, -714663723, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067727249*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067727249, 424898903, -714670664, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067727249, 424898903, -714670664, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067727257*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067727257, 424898943, -714663221, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067727257, 424898943, -714663221, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067727259*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067727259, 424898944, -714664709, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067727259, 424898944, -714664709, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067727284*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067727284, 424899084, -714663576, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067727284, 424899084, -714663576, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067727382*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067727382, 424899374, -714664397, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067727382, 424899374, -714664397, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067727446*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067727446, 424899534, -714664801, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067727446, 424899534, -714664801, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067727525*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067727525, 424899700, -714669802, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067727525, 424899700, -714669802, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067727545*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067727545, 424899748, -714663095, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067727545, 424899748, -714663095, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067727565*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067727565, 424899792, -714690938, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067727565, 424899792, -714690938, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067727645*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067727645, 424899951, -714692457, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067727645, 424899951, -714692457, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067727710*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067727710, 424900226, -714664299, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067727710, 424900226, -714664299, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067727753*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067727753, 424900315, -714691977, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067727753, 424900315, -714691977, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067727755*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067727755, 424900322, -714671598, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067727755, 424900322, -714671598, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067727761*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067727761, 424900357, -714692379, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067727761, 424900357, -714692379, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067727789*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067727789, 424900437, -714669336, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067727789, 424900437, -714669336, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067727945*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067727945, 424900848, -714690736, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067727945, 424900848, -714690736, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067728076*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067728076, 424900965, -714691853, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067728076, 424900965, -714691853, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067728109*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067728109, 424901059, -714671131, 2, true, (now() at time zone 'utc'), 1710365945, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067728109, 424901059, -714671131, 2, true, (now() at time zone 'utc'), 1710365945, 1);
/* create node 2067728423*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067728423, 424901841, -714684486, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067728423, 424901841, -714684486, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067728429*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067728429, 424901874, -714675960, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067728429, 424901874, -714675960, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067728435*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067728435, 424901883, -714683674, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067728435, 424901883, -714683674, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067728450*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067728450, 424901907, -714677053, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067728450, 424901907, -714677053, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067728504*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067728504, 424902085, -714677583, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067728504, 424902085, -714677583, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067728523*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067728523, 424902163, -714684052, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067728523, 424902163, -714684052, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067728543*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067728543, 424902184, -714676882, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067728543, 424902184, -714676882, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067728550*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067728550, 424902191, -714683258, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067728550, 424902191, -714683258, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067728599*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067728599, 424902379, -714677402, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067728599, 424902379, -714677402, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067728609*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067728609, 424902436, -714685291, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067728609, 424902436, -714685291, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067728620*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067728620, 424902454, -714683614, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067728620, 424902454, -714683614, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067728655*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067728655, 424902625, -714682924, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067728655, 424902625, -714682924, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067728673*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067728673, 424902675, -714675467, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067728673, 424902675, -714675467, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067728678*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067728678, 424902702, -714678361, 2, true, (now() at time zone 'utc'), 1710365939, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067728678, 424902702, -714678361, 2, true, (now() at time zone 'utc'), 1710365939, 1);
/* create node 2067728717*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067728717, 424902795, -714683153, 2, true, (now() at time zone 'utc'), 1710365942, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067728717, 424902795, -714683153, 2, true, (now() at time zone 'utc'), 1710365942, 1);
/* create node 2067728759*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067728759, 424902980, -714683976, 2, true, (now() at time zone 'utc'), 1710365942, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067728759, 424902980, -714683976, 2, true, (now() at time zone 'utc'), 1710365942, 1);
/* create node 2067728775*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067728775, 424903012, -714677185, 2, true, (now() at time zone 'utc'), 1710365942, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067728775, 424903012, -714677185, 2, true, (now() at time zone 'utc'), 1710365942, 1);
/* create node 2067728807*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067728807, 424903089, -714682297, 2, true, (now() at time zone 'utc'), 1710365942, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067728807, 424903089, -714682297, 2, true, (now() at time zone 'utc'), 1710365942, 1);
/* create node 2067728823*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067728823, 424903195, -714684267, 2, true, (now() at time zone 'utc'), 1710365942, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067728823, 424903195, -714684267, 2, true, (now() at time zone 'utc'), 1710365942, 1);
/* create node 2067728832*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067728832, 424903212, -714677062, 2, true, (now() at time zone 'utc'), 1710365942, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067728832, 424903212, -714677062, 2, true, (now() at time zone 'utc'), 1710365942, 1);
/* create node 2067728865*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067728865, 424903287, -714678001, 2, true, (now() at time zone 'utc'), 1710365942, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067728865, 424903287, -714678001, 2, true, (now() at time zone 'utc'), 1710365942, 1);
/* create node 2067729016*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067729016, 424903656, -714683063, 2, true, (now() at time zone 'utc'), 1710365942, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (2067729016, 424903656, -714683063, 2, true, (now() at time zone 'utc'), 1710365942, 1);
/* delete way 322*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (322, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 322;
DELETE FROM way_tags WHERE way_id = 322;
DELETE FROM current_way_nodes WHERE way_id=322;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 322;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=322;
/* delete way 357*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (357, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 357;
DELETE FROM way_tags WHERE way_id = 357;
DELETE FROM current_way_nodes WHERE way_id=357;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 357;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=357;
/* delete way 389*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (389, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 389;
DELETE FROM way_tags WHERE way_id = 389;
DELETE FROM current_way_nodes WHERE way_id=389;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 389;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=389;
/* delete way 391*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (391, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 391;
DELETE FROM way_tags WHERE way_id = 391;
DELETE FROM current_way_nodes WHERE way_id=391;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 391;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=391;
/* delete way 429*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (429, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 429;
DELETE FROM way_tags WHERE way_id = 429;
DELETE FROM current_way_nodes WHERE way_id=429;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 429;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=429;
/* delete way 440*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (440, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 440;
DELETE FROM way_tags WHERE way_id = 440;
DELETE FROM current_way_nodes WHERE way_id=440;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 440;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=440;
/* delete way 448*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (448, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 448;
DELETE FROM way_tags WHERE way_id = 448;
DELETE FROM current_way_nodes WHERE way_id=448;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 448;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=448;
/* delete way 464*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (464, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 464;
DELETE FROM way_tags WHERE way_id = 464;
DELETE FROM current_way_nodes WHERE way_id=464;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 464;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=464;
/* delete way 467*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (467, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 467;
DELETE FROM way_tags WHERE way_id = 467;
DELETE FROM current_way_nodes WHERE way_id=467;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 467;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=467;
/* delete way 479*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (479, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 479;
DELETE FROM way_tags WHERE way_id = 479;
DELETE FROM current_way_nodes WHERE way_id=479;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 479;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=479;
/* delete way 495*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (495, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 495;
DELETE FROM way_tags WHERE way_id = 495;
DELETE FROM current_way_nodes WHERE way_id=495;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 495;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=495;
/* delete way 505*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (505, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 505;
DELETE FROM way_tags WHERE way_id = 505;
DELETE FROM current_way_nodes WHERE way_id=505;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 505;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=505;
/* delete way 516*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (516, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 516;
DELETE FROM way_tags WHERE way_id = 516;
DELETE FROM current_way_nodes WHERE way_id=516;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 516;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=516;
/* delete way 551*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (551, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 551;
DELETE FROM way_tags WHERE way_id = 551;
DELETE FROM current_way_nodes WHERE way_id=551;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 551;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=551;
/* delete way 556*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (556, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 556;
DELETE FROM way_tags WHERE way_id = 556;
DELETE FROM current_way_nodes WHERE way_id=556;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 556;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=556;
/* delete way 572*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (572, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 572;
DELETE FROM way_tags WHERE way_id = 572;
DELETE FROM current_way_nodes WHERE way_id=572;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 572;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=572;
/* delete way 595*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (595, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 595;
DELETE FROM way_tags WHERE way_id = 595;
DELETE FROM current_way_nodes WHERE way_id=595;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 595;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=595;
/* delete way 598*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (598, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 598;
DELETE FROM way_tags WHERE way_id = 598;
DELETE FROM current_way_nodes WHERE way_id=598;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 598;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=598;
/* delete way 621*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (621, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 621;
DELETE FROM way_tags WHERE way_id = 621;
DELETE FROM current_way_nodes WHERE way_id=621;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 621;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=621;
/* delete way 640*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (640, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 640;
DELETE FROM way_tags WHERE way_id = 640;
DELETE FROM current_way_nodes WHERE way_id=640;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 640;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=640;
/* delete way 662*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (662, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 662;
DELETE FROM way_tags WHERE way_id = 662;
DELETE FROM current_way_nodes WHERE way_id=662;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 662;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=662;
/* delete way 667*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (667, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 667;
DELETE FROM way_tags WHERE way_id = 667;
DELETE FROM current_way_nodes WHERE way_id=667;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 667;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=667;
/* delete way 769*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (769, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 769;
DELETE FROM way_tags WHERE way_id = 769;
DELETE FROM current_way_nodes WHERE way_id=769;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 769;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=769;
/* delete way 770*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (770, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 770;
DELETE FROM way_tags WHERE way_id = 770;
DELETE FROM current_way_nodes WHERE way_id=770;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 770;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=770;
/* delete way 817*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (817, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 817;
DELETE FROM way_tags WHERE way_id = 817;
DELETE FROM current_way_nodes WHERE way_id=817;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 817;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=817;
/* delete way 819*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (819, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 819;
DELETE FROM way_tags WHERE way_id = 819;
DELETE FROM current_way_nodes WHERE way_id=819;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 819;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=819;
/* delete way 825*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (825, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 825;
DELETE FROM way_tags WHERE way_id = 825;
DELETE FROM current_way_nodes WHERE way_id=825;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 825;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=825;
/* delete way 855*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (855, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 855;
DELETE FROM way_tags WHERE way_id = 855;
DELETE FROM current_way_nodes WHERE way_id=855;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 855;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=855;
/* delete way 899*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (899, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 899;
DELETE FROM way_tags WHERE way_id = 899;
DELETE FROM current_way_nodes WHERE way_id=899;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 899;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=899;
/* delete way 926*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (926, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 926;
DELETE FROM way_tags WHERE way_id = 926;
DELETE FROM current_way_nodes WHERE way_id=926;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 926;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=926;
/* delete way 980*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (980, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 980;
DELETE FROM way_tags WHERE way_id = 980;
DELETE FROM current_way_nodes WHERE way_id=980;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 980;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=980;
/* delete way 990*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (990, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 990;
DELETE FROM way_tags WHERE way_id = 990;
DELETE FROM current_way_nodes WHERE way_id=990;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 990;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=990;
/* delete way 1024*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (1024, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 1024;
DELETE FROM way_tags WHERE way_id = 1024;
DELETE FROM current_way_nodes WHERE way_id=1024;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 1024;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=1024;
/* delete way 1047*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (1047, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 1047;
DELETE FROM way_tags WHERE way_id = 1047;
DELETE FROM current_way_nodes WHERE way_id=1047;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 1047;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=1047;
/* delete way 1050*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (1050, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 1050;
DELETE FROM way_tags WHERE way_id = 1050;
DELETE FROM current_way_nodes WHERE way_id=1050;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 1050;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=1050;
/* delete way 1132*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (1132, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 1132;
DELETE FROM way_tags WHERE way_id = 1132;
DELETE FROM current_way_nodes WHERE way_id=1132;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 1132;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=1132;
/* delete way 1183*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (1183, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 1183;
DELETE FROM way_tags WHERE way_id = 1183;
DELETE FROM current_way_nodes WHERE way_id=1183;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 1183;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=1183;
/* delete way 1208*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (1208, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 1208;
DELETE FROM way_tags WHERE way_id = 1208;
DELETE FROM current_way_nodes WHERE way_id=1208;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 1208;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=1208;
/* delete way 1291*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (1291, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 1291;
DELETE FROM way_tags WHERE way_id = 1291;
DELETE FROM current_way_nodes WHERE way_id=1291;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 1291;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=1291;
/* delete way 1301*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (1301, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 1301;
DELETE FROM way_tags WHERE way_id = 1301;
DELETE FROM current_way_nodes WHERE way_id=1301;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 1301;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=1301;
/* delete way 1333*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (1333, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 1333;
DELETE FROM way_tags WHERE way_id = 1333;
DELETE FROM current_way_nodes WHERE way_id=1333;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 1333;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=1333;
/* delete way 1388*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (1388, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 1388;
DELETE FROM way_tags WHERE way_id = 1388;
DELETE FROM current_way_nodes WHERE way_id=1388;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 1388;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=1388;
/* delete way 1407*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (1407, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 1407;
DELETE FROM way_tags WHERE way_id = 1407;
DELETE FROM current_way_nodes WHERE way_id=1407;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 1407;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=1407;
/* delete way 1423*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (1423, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 1423;
DELETE FROM way_tags WHERE way_id = 1423;
DELETE FROM current_way_nodes WHERE way_id=1423;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 1423;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=1423;
/* delete way 1466*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (1466, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 1466;
DELETE FROM way_tags WHERE way_id = 1466;
DELETE FROM current_way_nodes WHERE way_id=1466;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 1466;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=1466;
/* delete way 1480*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (1480, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 1480;
DELETE FROM way_tags WHERE way_id = 1480;
DELETE FROM current_way_nodes WHERE way_id=1480;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 1480;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=1480;
/* delete way 1498*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (1498, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 1498;
DELETE FROM way_tags WHERE way_id = 1498;
DELETE FROM current_way_nodes WHERE way_id=1498;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 1498;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=1498;
/* delete way 1501*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (1501, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 1501;
DELETE FROM way_tags WHERE way_id = 1501;
DELETE FROM current_way_nodes WHERE way_id=1501;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 1501;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=1501;
/* delete way 1521*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (1521, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 1521;
DELETE FROM way_tags WHERE way_id = 1521;
DELETE FROM current_way_nodes WHERE way_id=1521;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 1521;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=1521;
/* delete way 1528*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (1528, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 1528;
DELETE FROM way_tags WHERE way_id = 1528;
DELETE FROM current_way_nodes WHERE way_id=1528;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 1528;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=1528;
/* delete way 1546*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (1546, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 1546;
DELETE FROM way_tags WHERE way_id = 1546;
DELETE FROM current_way_nodes WHERE way_id=1546;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 1546;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=1546;
/* delete way 1554*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (1554, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 1554;
DELETE FROM way_tags WHERE way_id = 1554;
DELETE FROM current_way_nodes WHERE way_id=1554;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 1554;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=1554;
/* delete way 1559*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (1559, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 1559;
DELETE FROM way_tags WHERE way_id = 1559;
DELETE FROM current_way_nodes WHERE way_id=1559;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 1559;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=1559;
/* delete way 1586*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (1586, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 1586;
DELETE FROM way_tags WHERE way_id = 1586;
DELETE FROM current_way_nodes WHERE way_id=1586;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 1586;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=1586;
/* delete way 1604*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (1604, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 1604;
DELETE FROM way_tags WHERE way_id = 1604;
DELETE FROM current_way_nodes WHERE way_id=1604;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 1604;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=1604;
/* delete way 1607*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (1607, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 1607;
DELETE FROM way_tags WHERE way_id = 1607;
DELETE FROM current_way_nodes WHERE way_id=1607;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 1607;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=1607;
/* delete way 1642*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (1642, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 1642;
DELETE FROM way_tags WHERE way_id = 1642;
DELETE FROM current_way_nodes WHERE way_id=1642;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 1642;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=1642;
/* delete way 1671*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (1671, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 1671;
DELETE FROM way_tags WHERE way_id = 1671;
DELETE FROM current_way_nodes WHERE way_id=1671;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 1671;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=1671;
/* delete way 1710*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (1710, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 1710;
DELETE FROM way_tags WHERE way_id = 1710;
DELETE FROM current_way_nodes WHERE way_id=1710;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 1710;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=1710;
/* delete way 1727*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (1727, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 1727;
DELETE FROM way_tags WHERE way_id = 1727;
DELETE FROM current_way_nodes WHERE way_id=1727;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 1727;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=1727;
/* delete way 1736*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (1736, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 1736;
DELETE FROM way_tags WHERE way_id = 1736;
DELETE FROM current_way_nodes WHERE way_id=1736;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 1736;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=1736;
/* delete way 1737*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (1737, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 1737;
DELETE FROM way_tags WHERE way_id = 1737;
DELETE FROM current_way_nodes WHERE way_id=1737;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 1737;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=1737;
/* delete way 1767*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (1767, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 1767;
DELETE FROM way_tags WHERE way_id = 1767;
DELETE FROM current_way_nodes WHERE way_id=1767;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 1767;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=1767;
/* delete way 1785*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (1785, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 1785;
DELETE FROM way_tags WHERE way_id = 1785;
DELETE FROM current_way_nodes WHERE way_id=1785;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 1785;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=1785;
/* delete way 1813*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (1813, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 1813;
DELETE FROM way_tags WHERE way_id = 1813;
DELETE FROM current_way_nodes WHERE way_id=1813;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 1813;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=1813;
/* delete way 1842*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (1842, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 1842;
DELETE FROM way_tags WHERE way_id = 1842;
DELETE FROM current_way_nodes WHERE way_id=1842;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 1842;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=1842;
/* delete way 1858*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (1858, 2, false, (now() at time zone 'utc'), 2);
DELETE FROM current_way_tags WHERE way_id = 1858;
DELETE FROM way_tags WHERE way_id = 1858;
DELETE FROM current_way_nodes WHERE way_id=1858;
DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = 1858;
UPDATE current_ways SET changeset_id=2, visible=false, version=2 WHERE id=1858;
/* create way 196423735*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (196423735, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (196423735, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (196423735, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (196423735, 'building', 'yes', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196423735, 2067713628, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196423735, 2067713628, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196423735, 2067713030, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196423735, 2067713030, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196423735, 2067713231, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196423735, 2067713231, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196423735, 2067713334, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196423735, 2067713334, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196423735, 2067713561, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196423735, 2067713561, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196423735, 2067714014, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196423735, 2067714014, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196423735, 2067713841, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196423735, 2067713841, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196423735, 2067714004, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196423735, 2067714004, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196423735, 2067713779, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196423735, 2067713779, 9);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196423735, 2067713820, 1, 10);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196423735, 2067713820, 10);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196423735, 2067713628, 1, 11);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196423735, 2067713628, 11);
/* create way 196424014*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (196424014, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (196424014, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (196424014, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (196424014, 'building', 'yes', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424014, 2067726799, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424014, 2067726799, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424014, 2067726868, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424014, 2067726868, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424014, 2067727038, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424014, 2067727038, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424014, 2067727015, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424014, 2067727015, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424014, 2067727249, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424014, 2067727249, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424014, 2067727095, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424014, 2067727095, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424014, 2067726814, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424014, 2067726814, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424014, 2067726744, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424014, 2067726744, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424014, 2067726345, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424014, 2067726345, 9);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424014, 2067726483, 1, 10);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424014, 2067726483, 10);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424014, 2067726799, 1, 11);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424014, 2067726799, 11);
/* create way 196424034*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (196424034, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (196424034, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (196424034, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (196424034, 'building', 'yes', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424034, 2067722227, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424034, 2067722227, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424034, 2067722350, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424034, 2067722350, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424034, 2067722361, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424034, 2067722361, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424034, 2067722496, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424034, 2067722496, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424034, 2067722511, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424034, 2067722511, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424034, 2067722640, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424034, 2067722640, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424034, 2067722540, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424034, 2067722540, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424034, 2067722637, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424034, 2067722637, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424034, 2067722487, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424034, 2067722487, 9);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424034, 2067722018, 1, 10);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424034, 2067722018, 10);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424034, 2067722227, 1, 11);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424034, 2067722227, 11);
/* create way 196424353*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (196424353, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (196424353, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (196424353, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (196424353, 'building', 'yes', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424353, 2067719938, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424353, 2067719938, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424353, 2067720130, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424353, 2067720130, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424353, 2067720408, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424353, 2067720408, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424353, 2067720348, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424353, 2067720348, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424353, 2067720525, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424353, 2067720525, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424353, 2067720437, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424353, 2067720437, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424353, 2067720204, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424353, 2067720204, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424353, 2067720145, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424353, 2067720145, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424353, 2067719916, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424353, 2067719916, 9);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424353, 2067719988, 1, 10);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424353, 2067719988, 10);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424353, 2067719938, 1, 11);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424353, 2067719938, 11);
/* create way 196424416*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (196424416, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (196424416, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (196424416, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (196424416, 'building', 'yes', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424416, 2067723195, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424416, 2067723195, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424416, 2067723156, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424416, 2067723156, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424416, 2067723146, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424416, 2067723146, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424416, 2067723104, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424416, 2067723104, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424416, 2067723004, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424416, 2067723004, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424416, 2067722959, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424416, 2067722959, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424416, 2067722803, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424416, 2067722803, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424416, 2067722884, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424416, 2067722884, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424416, 2067722701, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424416, 2067722701, 9);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424416, 2067722811, 1, 10);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424416, 2067722811, 10);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424416, 2067723195, 1, 11);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424416, 2067723195, 11);
/* create way 196424458*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (196424458, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (196424458, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (196424458, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (196424458, 'building', 'yes', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424458, 2067720551, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424458, 2067720551, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424458, 2067720732, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424458, 2067720732, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424458, 2067720893, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424458, 2067720893, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424458, 2067720840, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424458, 2067720840, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424458, 2067721219, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424458, 2067721219, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424458, 2067721001, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424458, 2067721001, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424458, 2067720819, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424458, 2067720819, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424458, 2067720845, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424458, 2067720845, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424458, 2067720713, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424458, 2067720713, 9);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424458, 2067720738, 1, 10);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424458, 2067720738, 10);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424458, 2067720551, 1, 11);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424458, 2067720551, 11);
/* create way 196424625*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (196424625, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (196424625, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (196424625, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (196424625, 'building', 'yes', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424625, 2067721270, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424625, 2067721270, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424625, 2067721298, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424625, 2067721298, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424625, 2067721449, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424625, 2067721449, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424625, 2067721436, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424625, 2067721436, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424625, 2067721570, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424625, 2067721570, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424625, 2067721438, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424625, 2067721438, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424625, 2067720962, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424625, 2067720962, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424625, 2067721032, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424625, 2067721032, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424625, 2067721076, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424625, 2067721076, 9);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424625, 2067721147, 1, 10);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424625, 2067721147, 10);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424625, 2067721270, 1, 11);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424625, 2067721270, 11);
/* create way 196424637*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (196424637, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (196424637, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (196424637, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (196424637, 'building', 'yes', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424637, 2067727382, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424637, 2067727382, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424637, 2067727446, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424637, 2067727446, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424637, 2067727710, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424637, 2067727710, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424637, 2067727545, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424637, 2067727545, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424637, 2067727284, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424637, 2067727284, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424637, 2067727257, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424637, 2067727257, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424637, 2067727101, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424637, 2067727101, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424637, 2067727171, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424637, 2067727171, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424637, 2067727098, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424637, 2067727098, 9);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424637, 2067727259, 1, 10);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424637, 2067727259, 10);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424637, 2067727382, 1, 11);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424637, 2067727382, 11);
/* create way 196424653*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (196424653, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (196424653, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (196424653, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (196424653, 'building', 'yes', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424653, 2067728543, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424653, 2067728543, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424653, 2067728450, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424653, 2067728450, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424653, 2067728504, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424653, 2067728504, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424653, 2067728599, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424653, 2067728599, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424653, 2067728678, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424653, 2067728678, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424653, 2067728865, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424653, 2067728865, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424653, 2067728775, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424653, 2067728775, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424653, 2067728832, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424653, 2067728832, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424653, 2067728673, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424653, 2067728673, 9);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424653, 2067728429, 1, 10);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424653, 2067728429, 10);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424653, 2067728543, 1, 11);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424653, 2067728543, 11);
/* create way 196424702*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (196424702, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (196424702, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (196424702, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (196424702, 'building', 'yes', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424702, 2067720939, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424702, 2067720939, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424702, 2067721022, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424702, 2067721022, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424702, 2067721549, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424702, 2067721549, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424702, 2067721560, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424702, 2067721560, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424702, 2067721786, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424702, 2067721786, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424702, 2067721631, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424702, 2067721631, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424702, 2067721234, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424702, 2067721234, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424702, 2067721314, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424702, 2067721314, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424702, 2067721216, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424702, 2067721216, 9);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424702, 2067721106, 1, 10);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424702, 2067721106, 10);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424702, 2067720939, 1, 11);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424702, 2067720939, 11);
/* create way 196424857*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (196424857, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (196424857, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (196424857, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (196424857, 'building', 'yes', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424857, 2067718875, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424857, 2067718875, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424857, 2067718775, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424857, 2067718775, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424857, 2067718962, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424857, 2067718962, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424857, 2067719243, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424857, 2067719243, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424857, 2067719117, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424857, 2067719117, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424857, 2067719049, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424857, 2067719049, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424857, 2067718947, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424857, 2067718947, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424857, 2067719148, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424857, 2067719148, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424857, 2067718941, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424857, 2067718941, 9);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424857, 2067718738, 1, 10);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424857, 2067718738, 10);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424857, 2067718875, 1, 11);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424857, 2067718875, 11);
/* create way 196424954*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (196424954, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (196424954, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (196424954, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (196424954, 'building', 'yes', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424954, 2067714894, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424954, 2067714894, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424954, 2067714492, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424954, 2067714492, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424954, 2067714301, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424954, 2067714301, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424954, 2067714248, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424954, 2067714248, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424954, 2067713952, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424954, 2067713952, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424954, 2067713768, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424954, 2067713768, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424954, 2067713617, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424954, 2067713617, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424954, 2067714215, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424954, 2067714215, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424954, 2067714540, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424954, 2067714540, 9);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424954, 2067714657, 1, 10);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424954, 2067714657, 10);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424954, 2067714894, 1, 11);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424954, 2067714894, 11);
/* create way 196424956*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (196424956, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (196424956, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (196424956, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (196424956, 'building', 'yes', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424956, 2067717917, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424956, 2067717917, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424956, 2067717947, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424956, 2067717947, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424956, 2067718348, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424956, 2067718348, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424956, 2067718172, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424956, 2067718172, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424956, 2067718070, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424956, 2067718070, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424956, 2067717959, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424956, 2067717959, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424956, 2067717698, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424956, 2067717698, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424956, 2067717811, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424956, 2067717811, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424956, 2067717707, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424956, 2067717707, 9);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424956, 2067717785, 1, 10);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424956, 2067717785, 10);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196424956, 2067717917, 1, 11);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196424956, 2067717917, 11);
/* create way 196425086*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (196425086, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (196425086, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (196425086, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (196425086, 'building', 'yes', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196425086, 2067721421, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196425086, 2067721421, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196425086, 2067721446, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196425086, 2067721446, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196425086, 2067721774, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196425086, 2067721774, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196425086, 2067721590, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196425086, 2067721590, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196425086, 2067721161, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196425086, 2067721161, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196425086, 2067721067, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196425086, 2067721067, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196425086, 2067720902, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196425086, 2067720902, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196425086, 2067721041, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196425086, 2067721041, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196425086, 2067721112, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196425086, 2067721112, 9);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196425086, 2067721141, 1, 10);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196425086, 2067721141, 10);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196425086, 2067721421, 1, 11);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196425086, 2067721421, 11);
/* create way 196425511*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (196425511, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (196425511, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (196425511, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (196425511, 'building', 'yes', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196425511, 2067723200, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196425511, 2067723200, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196425511, 2067723101, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196425511, 2067723101, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196425511, 2067723122, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196425511, 2067723122, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196425511, 2067722910, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196425511, 2067722910, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196425511, 2067723023, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196425511, 2067723023, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196425511, 2067723189, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196425511, 2067723189, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196425511, 2067723347, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196425511, 2067723347, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196425511, 2067723626, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196425511, 2067723626, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196425511, 2067723405, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196425511, 2067723405, 9);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196425511, 2067723641, 1, 10);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196425511, 2067723641, 10);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196425511, 2067723540, 1, 11);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196425511, 2067723540, 11);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196425511, 2067723300, 1, 12);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196425511, 2067723300, 12);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196425511, 2067723200, 1, 13);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196425511, 2067723200, 13);
/* create way 196425548*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (196425548, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (196425548, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (196425548, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (196425548, 'building', 'yes', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196425548, 2067723290, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196425548, 2067723290, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196425548, 2067723158, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196425548, 2067723158, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196425548, 2067723215, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196425548, 2067723215, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196425548, 2067723409, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196425548, 2067723409, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196425548, 2067723359, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196425548, 2067723359, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196425548, 2067723761, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196425548, 2067723761, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196425548, 2067723797, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196425548, 2067723797, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196425548, 2067724062, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196425548, 2067724062, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196425548, 2067723961, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196425548, 2067723961, 9);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196425548, 2067723715, 1, 10);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196425548, 2067723715, 10);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196425548, 2067723661, 1, 11);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196425548, 2067723661, 11);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196425548, 2067723220, 1, 12);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196425548, 2067723220, 12);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196425548, 2067723290, 1, 13);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196425548, 2067723290, 13);
/* create way 196425930*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (196425930, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (196425930, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (196425930, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (196425930, 'building', 'yes', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196425930, 2067722048, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196425930, 2067722048, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196425930, 2067721967, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196425930, 2067721967, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196425930, 2067721832, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196425930, 2067721832, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196425930, 2067721546, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196425930, 2067721546, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196425930, 2067721683, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196425930, 2067721683, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196425930, 2067722024, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196425930, 2067722024, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196425930, 2067722064, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196425930, 2067722064, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196425930, 2067722209, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196425930, 2067722209, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196425930, 2067722160, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196425930, 2067722160, 9);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196425930, 2067722253, 1, 10);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196425930, 2067722253, 10);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196425930, 2067722181, 1, 11);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196425930, 2067722181, 11);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196425930, 2067721990, 1, 12);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196425930, 2067721990, 12);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196425930, 2067722048, 1, 13);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196425930, 2067722048, 13);
/* create way 196426174*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (196426174, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (196426174, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (196426174, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (196426174, 'building', 'yes', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426174, 2067725180, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426174, 2067725180, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426174, 2067725063, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426174, 2067725063, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426174, 2067724751, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426174, 2067724751, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426174, 2067725060, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426174, 2067725060, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426174, 2067725227, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426174, 2067725227, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426174, 2067725274, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426174, 2067725274, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426174, 2067725415, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426174, 2067725415, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426174, 2067725460, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426174, 2067725460, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426174, 2067725628, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426174, 2067725628, 9);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426174, 2067725576, 1, 10);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426174, 2067725576, 10);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426174, 2067725730, 1, 11);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426174, 2067725730, 11);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426174, 2067725558, 1, 12);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426174, 2067725558, 12);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426174, 2067725180, 1, 13);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426174, 2067725180, 13);
/* create way 196426203*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (196426203, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (196426203, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (196426203, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (196426203, 'building', 'yes', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426203, 2067715017, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426203, 2067715017, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426203, 2067714677, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426203, 2067714677, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426203, 2067714830, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426203, 2067714830, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426203, 2067715035, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426203, 2067715035, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426203, 2067715061, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426203, 2067715061, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426203, 2067715265, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426203, 2067715265, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426203, 2067715283, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426203, 2067715283, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426203, 2067715605, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426203, 2067715605, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426203, 2067715500, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426203, 2067715500, 9);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426203, 2067715177, 1, 10);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426203, 2067715177, 10);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426203, 2067715142, 1, 11);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426203, 2067715142, 11);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426203, 2067714936, 1, 12);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426203, 2067714936, 12);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426203, 2067715017, 1, 13);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426203, 2067715017, 13);
/* create way 196426316*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (196426316, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (196426316, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (196426316, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (196426316, 'building', 'yes', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426316, 2067728717, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426316, 2067728717, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426316, 2067728620, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426316, 2067728620, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426316, 2067728550, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426316, 2067728550, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426316, 2067728435, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426316, 2067728435, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426316, 2067728523, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426316, 2067728523, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426316, 2067728423, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426316, 2067728423, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426316, 2067728609, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426316, 2067728609, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426316, 2067728823, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426316, 2067728823, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426316, 2067728759, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426316, 2067728759, 9);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426316, 2067729016, 1, 10);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426316, 2067729016, 10);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426316, 2067728807, 1, 11);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426316, 2067728807, 11);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426316, 2067728655, 1, 12);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426316, 2067728655, 12);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426316, 2067728717, 1, 13);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426316, 2067728717, 13);
/* create way 196426495*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (196426495, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (196426495, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (196426495, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (196426495, 'building', 'yes', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426495, 2067720375, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426495, 2067720375, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426495, 2067720142, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426495, 2067720142, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426495, 2067719871, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426495, 2067719871, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426495, 2067720044, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426495, 2067720044, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426495, 2067720151, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426495, 2067720151, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426495, 2067720202, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426495, 2067720202, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426495, 2067720171, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426495, 2067720171, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426495, 2067720351, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426495, 2067720351, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426495, 2067720534, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426495, 2067720534, 9);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426495, 2067720446, 1, 10);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426495, 2067720446, 10);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426495, 2067720402, 1, 11);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426495, 2067720402, 11);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426495, 2067720354, 1, 12);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426495, 2067720354, 12);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426495, 2067720375, 1, 13);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426495, 2067720375, 13);
/* create way 196426726*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (196426726, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (196426726, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (196426726, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (196426726, 'building', 'yes', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426726, 2067719387, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426726, 2067719387, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426726, 2067719296, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426726, 2067719296, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426726, 2067718995, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426726, 2067718995, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426726, 2067719019, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426726, 2067719019, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426726, 2067718899, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426726, 2067718899, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426726, 2067718853, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426726, 2067718853, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426726, 2067718458, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426726, 2067718458, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426726, 2067718695, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426726, 2067718695, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426726, 2067718902, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426726, 2067718902, 9);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426726, 2067718911, 1, 10);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426726, 2067718911, 10);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426726, 2067719152, 1, 11);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426726, 2067719152, 11);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426726, 2067719120, 1, 12);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426726, 2067719120, 12);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426726, 2067719387, 1, 13);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426726, 2067719387, 13);
/* create way 196426913*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (196426913, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (196426913, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (196426913, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (196426913, 'building', 'yes', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426913, 2067717414, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426913, 2067717414, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426913, 2067717221, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426913, 2067717221, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426913, 2067717244, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426913, 2067717244, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426913, 2067717160, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426913, 2067717160, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426913, 2067717227, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426913, 2067717227, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426913, 2067717353, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426913, 2067717353, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426913, 2067717381, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426913, 2067717381, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426913, 2067717823, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426913, 2067717823, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426913, 2067717686, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426913, 2067717686, 9);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426913, 2067717468, 1, 10);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426913, 2067717468, 10);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426913, 2067717429, 1, 11);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426913, 2067717429, 11);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426913, 2067717362, 1, 12);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426913, 2067717362, 12);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426913, 2067717414, 1, 13);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426913, 2067717414, 13);
/* create way 196426940*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (196426940, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (196426940, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (196426940, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (196426940, 'building', 'yes', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426940, 2067714684, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426940, 2067714684, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426940, 2067714349, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426940, 2067714349, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426940, 2067713791, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426940, 2067713791, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426940, 2067714095, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426940, 2067714095, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426940, 2067714171, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426940, 2067714171, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426940, 2067714327, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426940, 2067714327, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426940, 2067714272, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426940, 2067714272, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426940, 2067714420, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426940, 2067714420, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426940, 2067714510, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426940, 2067714510, 9);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426940, 2067714709, 1, 10);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426940, 2067714709, 10);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426940, 2067714938, 1, 11);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426940, 2067714938, 11);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426940, 2067714457, 1, 12);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426940, 2067714457, 12);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196426940, 2067714684, 1, 13);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196426940, 2067714684, 13);
/* create way 196428017*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (196428017, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (196428017, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (196428017, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (196428017, 'building', 'yes', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196428017, 2067724511, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196428017, 2067724511, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196428017, 2067724210, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196428017, 2067724210, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196428017, 2067724260, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196428017, 2067724260, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196428017, 2067724131, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196428017, 2067724131, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196428017, 2067724225, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196428017, 2067724225, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196428017, 2067724366, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196428017, 2067724366, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196428017, 2067724428, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196428017, 2067724428, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196428017, 2067724637, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196428017, 2067724637, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196428017, 2067724676, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196428017, 2067724676, 9);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196428017, 2067724862, 1, 10);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196428017, 2067724862, 10);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196428017, 2067724734, 1, 11);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196428017, 2067724734, 11);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196428017, 2067724933, 1, 12);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196428017, 2067724933, 12);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196428017, 2067724790, 1, 13);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196428017, 2067724790, 13);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196428017, 2067724556, 1, 14);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196428017, 2067724556, 14);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196428017, 2067724511, 1, 15);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196428017, 2067724511, 15);
/* create way 196428038*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (196428038, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (196428038, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (196428038, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (196428038, 'building', 'yes', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196428038, 2067725330, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196428038, 2067725330, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196428038, 2067724946, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196428038, 2067724946, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196428038, 2067725054, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196428038, 2067725054, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196428038, 2067724890, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196428038, 2067724890, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196428038, 2067724943, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196428038, 2067724943, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196428038, 2067724893, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196428038, 2067724893, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196428038, 2067724968, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196428038, 2067724968, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196428038, 2067725123, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196428038, 2067725123, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196428038, 2067725190, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196428038, 2067725190, 9);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196428038, 2067725470, 1, 10);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196428038, 2067725470, 10);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196428038, 2067725433, 1, 11);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196428038, 2067725433, 11);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196428038, 2067725696, 1, 12);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196428038, 2067725696, 12);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196428038, 2067725564, 1, 13);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196428038, 2067725564, 13);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196428038, 2067725351, 1, 14);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196428038, 2067725351, 14);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196428038, 2067725330, 1, 15);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196428038, 2067725330, 15);
/* create way 196428858*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (196428858, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (196428858, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (196428858, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (196428858, 'building', 'yes', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196428858, 2067718821, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196428858, 2067718821, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196428858, 2067718983, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196428858, 2067718983, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196428858, 2067719224, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196428858, 2067719224, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196428858, 2067719182, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196428858, 2067719182, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196428858, 2067719235, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196428858, 2067719235, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196428858, 2067719310, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196428858, 2067719310, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196428858, 2067719461, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196428858, 2067719461, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196428858, 2067719482, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196428858, 2067719482, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196428858, 2067719690, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196428858, 2067719690, 9);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196428858, 2067719527, 1, 10);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196428858, 2067719527, 10);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196428858, 2067719426, 1, 11);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196428858, 2067719426, 11);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196428858, 2067719384, 1, 12);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196428858, 2067719384, 12);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196428858, 2067719016, 1, 13);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196428858, 2067719016, 13);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196428858, 2067719126, 1, 14);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196428858, 2067719126, 14);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196428858, 2067718965, 1, 15);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196428858, 2067718965, 15);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196428858, 2067718917, 1, 16);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196428858, 2067718917, 16);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196428858, 2067718821, 1, 17);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196428858, 2067718821, 17);
/* create way 196428937*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (196428937, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (196428937, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (196428937, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (196428937, 'building', 'yes', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196428937, 2067726313, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196428937, 2067726313, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196428937, 2067726191, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196428937, 2067726191, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196428937, 2067726170, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196428937, 2067726170, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196428937, 2067726041, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196428937, 2067726041, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196428937, 2067726083, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196428937, 2067726083, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196428937, 2067726020, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196428937, 2067726020, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196428937, 2067726144, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196428937, 2067726144, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196428937, 2067726315, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196428937, 2067726315, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196428937, 2067726366, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196428937, 2067726366, 9);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196428937, 2067726714, 1, 10);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196428937, 2067726714, 10);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196428937, 2067726678, 1, 11);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196428937, 2067726678, 11);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196428937, 2067726808, 1, 12);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196428937, 2067726808, 12);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196428937, 2067726467, 1, 13);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196428937, 2067726467, 13);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196428937, 2067726336, 1, 14);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196428937, 2067726336, 14);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196428937, 2067726405, 1, 15);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196428937, 2067726405, 15);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196428937, 2067726225, 1, 16);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196428937, 2067726225, 16);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196428937, 2067726313, 1, 17);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196428937, 2067726313, 17);
/* create way 196429463*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (196429463, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (196429463, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (196429463, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (196429463, 'building', 'yes', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196429463, 2067718091, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196429463, 2067718091, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196429463, 2067717983, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196429463, 2067717983, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196429463, 2067717874, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196429463, 2067717874, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196429463, 2067717841, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196429463, 2067717841, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196429463, 2067717799, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196429463, 2067717799, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196429463, 2067717749, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196429463, 2067717749, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196429463, 2067717643, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196429463, 2067717643, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196429463, 2067717710, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196429463, 2067717710, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196429463, 2067717591, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196429463, 2067717591, 9);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196429463, 2067717740, 1, 10);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196429463, 2067717740, 10);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196429463, 2067718079, 1, 11);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196429463, 2067718079, 11);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196429463, 2067718176, 1, 12);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196429463, 2067718176, 12);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196429463, 2067718262, 1, 13);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196429463, 2067718262, 13);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196429463, 2067718292, 1, 14);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196429463, 2067718292, 14);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196429463, 2067718455, 1, 15);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196429463, 2067718455, 15);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196429463, 2067718439, 1, 16);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196429463, 2067718439, 16);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196429463, 2067718491, 1, 17);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196429463, 2067718491, 17);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196429463, 2067718354, 1, 18);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196429463, 2067718354, 18);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196429463, 2067718091, 1, 19);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196429463, 2067718091, 19);
/* create way 196430751*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (196430751, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (196430751, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (196430751, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (196430751, 'building', 'yes', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196430751, 2067723016, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196430751, 2067723016, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196430751, 2067723143, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196430751, 2067723143, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196430751, 2067723053, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196430751, 2067723053, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196430751, 2067722887, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196430751, 2067722887, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196430751, 2067723016, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196430751, 2067723016, 5);
/* create way 196431239*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (196431239, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (196431239, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (196431239, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (196431239, 'building', 'yes', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196431239, 2067716011, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196431239, 2067716011, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196431239, 2067716643, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196431239, 2067716643, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196431239, 2067716540, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196431239, 2067716540, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196431239, 2067715861, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196431239, 2067715861, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196431239, 2067716011, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196431239, 2067716011, 5);
/* create way 196431312*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (196431312, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (196431312, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (196431312, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (196431312, 'building', 'yes', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196431312, 2067727755, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196431312, 2067727755, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196431312, 2067728109, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196431312, 2067728109, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196431312, 2067727789, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196431312, 2067727789, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196431312, 2067727525, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196431312, 2067727525, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196431312, 2067727755, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196431312, 2067727755, 5);
/* create way 196431555*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (196431555, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (196431555, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (196431555, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (196431555, 'building', 'yes', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196431555, 2067723929, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196431555, 2067723929, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196431555, 2067724151, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196431555, 2067724151, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196431555, 2067724099, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196431555, 2067724099, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196431555, 2067723833, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196431555, 2067723833, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196431555, 2067723929, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196431555, 2067723929, 5);
/* create way 196431786*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (196431786, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (196431786, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (196431786, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (196431786, 'building', 'yes', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196431786, 2067716866, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196431786, 2067716866, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196431786, 2067716990, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196431786, 2067716990, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196431786, 2067716936, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196431786, 2067716936, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196431786, 2067716787, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196431786, 2067716787, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196431786, 2067716866, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196431786, 2067716866, 5);
/* create way 196431854*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (196431854, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (196431854, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (196431854, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (196431854, 'building', 'yes', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196431854, 2067718893, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196431854, 2067718893, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196431854, 2067719067, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196431854, 2067719067, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196431854, 2067719022, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196431854, 2067719022, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196431854, 2067718868, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196431854, 2067718868, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196431854, 2067718893, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196431854, 2067718893, 5);
/* create way 196433157*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (196433157, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (196433157, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (196433157, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (196433157, 'building', 'yes', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196433157, 2067722515, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196433157, 2067722515, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196433157, 2067722722, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196433157, 2067722722, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196433157, 2067722649, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196433157, 2067722649, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196433157, 2067722417, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196433157, 2067722417, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196433157, 2067722515, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196433157, 2067722515, 5);
/* create way 196433345*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (196433345, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (196433345, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (196433345, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (196433345, 'building', 'yes', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196433345, 2067723816, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196433345, 2067723816, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196433345, 2067724004, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196433345, 2067724004, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196433345, 2067723923, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196433345, 2067723923, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196433345, 2067723726, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196433345, 2067723726, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196433345, 2067723816, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196433345, 2067723816, 5);
/* create way 196434296*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (196434296, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (196434296, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (196434296, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (196434296, 'building', 'yes', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196434296, 2067722789, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196434296, 2067722789, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196434296, 2067722901, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196434296, 2067722901, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196434296, 2067723078, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196434296, 2067723078, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196434296, 2067723068, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196434296, 2067723068, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196434296, 2067723114, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196434296, 2067723114, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196434296, 2067723036, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196434296, 2067723036, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196434296, 2067722789, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196434296, 2067722789, 7);
/* create way 196434623*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (196434623, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (196434623, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (196434623, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (196434623, 'building', 'yes', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196434623, 2067717859, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196434623, 2067717859, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196434623, 2067718239, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196434623, 2067718239, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196434623, 2067718506, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196434623, 2067718506, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196434623, 2067718479, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196434623, 2067718479, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196434623, 2067718561, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196434623, 2067718561, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196434623, 2067718277, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196434623, 2067718277, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196434623, 2067717859, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196434623, 2067717859, 7);
/* create way 196435160*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (196435160, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (196435160, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (196435160, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (196435160, 'building', 'yes', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196435160, 2067723273, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196435160, 2067723273, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196435160, 2067723093, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196435160, 2067723093, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196435160, 2067723154, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196435160, 2067723154, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196435160, 2067723590, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196435160, 2067723590, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196435160, 2067723321, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196435160, 2067723321, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196435160, 2067723197, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196435160, 2067723197, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196435160, 2067723273, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196435160, 2067723273, 7);
/* create way 196435322*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (196435322, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (196435322, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (196435322, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (196435322, 'building', 'yes', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196435322, 2067717645, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196435322, 2067717645, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196435322, 2067718088, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196435322, 2067718088, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196435322, 2067718047, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196435322, 2067718047, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196435322, 2067718295, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196435322, 2067718295, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196435322, 2067718166, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196435322, 2067718166, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196435322, 2067717493, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196435322, 2067717493, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196435322, 2067717645, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196435322, 2067717645, 7);
/* create way 196435455*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (196435455, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (196435455, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (196435455, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (196435455, 'building', 'yes', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196435455, 2067719596, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196435455, 2067719596, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196435455, 2067719985, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196435455, 2067719985, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196435455, 2067720089, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196435455, 2067720089, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196435455, 2067719955, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196435455, 2067719955, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196435455, 2067720136, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196435455, 2067720136, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196435455, 2067719795, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196435455, 2067719795, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196435455, 2067719596, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196435455, 2067719596, 7);
/* create way 196435896*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (196435896, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (196435896, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (196435896, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (196435896, 'building', 'yes', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196435896, 2067714949, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196435896, 2067714949, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196435896, 2067715241, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196435896, 2067715241, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196435896, 2067714998, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196435896, 2067714998, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196435896, 2067714861, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196435896, 2067714861, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196435896, 2067714783, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196435896, 2067714783, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196435896, 2067714500, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196435896, 2067714500, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196435896, 2067714949, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196435896, 2067714949, 7);
/* create way 196436005*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (196436005, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (196436005, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (196436005, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (196436005, 'building', 'yes', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436005, 2067727645, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436005, 2067727645, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436005, 2067727761, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436005, 2067727761, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436005, 2067727753, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436005, 2067727753, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436005, 2067728076, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436005, 2067728076, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436005, 2067727945, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436005, 2067727945, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436005, 2067727565, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436005, 2067727565, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436005, 2067727645, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436005, 2067727645, 7);
/* create way 196436186*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (196436186, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (196436186, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (196436186, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (196436186, 'building', 'yes', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436186, 2067715051, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436186, 2067715051, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436186, 2067715125, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436186, 2067715125, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436186, 2067715222, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436186, 2067715222, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436186, 2067715410, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436186, 2067715410, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436186, 2067715111, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436186, 2067715111, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436186, 2067714746, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436186, 2067714746, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436186, 2067715051, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436186, 2067715051, 7);
/* create way 196436211*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (196436211, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (196436211, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (196436211, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (196436211, 'building', 'yes', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436211, 2067720047, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436211, 2067720047, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436211, 2067720148, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436211, 2067720148, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436211, 2067720185, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436211, 2067720185, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436211, 2067720495, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436211, 2067720495, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436211, 2067720372, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436211, 2067720372, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436211, 2067719907, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436211, 2067719907, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436211, 2067720047, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436211, 2067720047, 7);
/* create way 196436454*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (196436454, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (196436454, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (196436454, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (196436454, 'building', 'yes', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436454, 2067716496, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436454, 2067716496, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436454, 2067716395, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436454, 2067716395, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436454, 2067716518, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436454, 2067716518, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436454, 2067716283, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436454, 2067716283, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436454, 2067716114, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436454, 2067716114, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436454, 2067715926, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436454, 2067715926, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436454, 2067716347, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436454, 2067716347, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436454, 2067716496, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436454, 2067716496, 8);
/* create way 196436641*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (196436641, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (196436641, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (196436641, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (196436641, 'building', 'yes', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436641, 2067723825, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436641, 2067723825, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436641, 2067723945, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436641, 2067723945, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436641, 2067723999, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436641, 2067723999, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436641, 2067724050, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436641, 2067724050, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436641, 2067724295, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436641, 2067724295, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436641, 2067724254, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436641, 2067724254, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436641, 2067724330, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436641, 2067724330, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436641, 2067724201, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436641, 2067724201, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436641, 2067723825, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436641, 2067723825, 9);
/* create way 196436685*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (196436685, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (196436685, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (196436685, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (196436685, 'building', 'yes', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436685, 2067726623, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436685, 2067726623, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436685, 2067726647, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436685, 2067726647, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436685, 2067727012, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436685, 2067727012, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436685, 2067727065, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436685, 2067727065, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436685, 2067726786, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436685, 2067726786, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436685, 2067726735, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436685, 2067726735, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436685, 2067726181, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436685, 2067726181, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436685, 2067726138, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436685, 2067726138, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436685, 2067726623, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436685, 2067726623, 9);
/* create way 196436712*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (196436712, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (196436712, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (196436712, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (196436712, 'building', 'yes', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436712, 2067726303, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436712, 2067726303, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436712, 2067726853, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436712, 2067726853, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436712, 2067726871, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436712, 2067726871, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436712, 2067727044, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436712, 2067727044, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436712, 2067726973, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436712, 2067726973, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436712, 2067726831, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436712, 2067726831, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436712, 2067726793, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436712, 2067726793, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436712, 2067726221, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436712, 2067726221, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436712, 2067726303, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436712, 2067726303, 9);
/* create way 196436981*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (196436981, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (196436981, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (196436981, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (196436981, 'building', 'yes', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436981, 2067716389, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436981, 2067716389, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436981, 2067716493, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436981, 2067716493, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436981, 2067716569, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436981, 2067716569, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436981, 2067716784, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436981, 2067716784, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436981, 2067716703, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436981, 2067716703, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436981, 2067716624, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436981, 2067716624, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436981, 2067716456, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436981, 2067716456, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436981, 2067716225, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436981, 2067716225, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196436981, 2067716389, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196436981, 2067716389, 9);
/* create way 196437126*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (196437126, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (196437126, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (196437126, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (196437126, 'building', 'yes', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196437126, 2067722060, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196437126, 2067722060, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196437126, 2067722012, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196437126, 2067722012, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196437126, 2067722140, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196437126, 2067722140, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196437126, 2067722192, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196437126, 2067722192, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196437126, 2067722198, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196437126, 2067722198, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196437126, 2067722493, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196437126, 2067722493, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196437126, 2067722371, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196437126, 2067722371, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196437126, 2067722036, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196437126, 2067722036, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196437126, 2067722060, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196437126, 2067722060, 9);
/* create way 196437138*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (196437138, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (196437138, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (196437138, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (196437138, 'building', 'yes', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196437138, 2067724907, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196437138, 2067724907, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196437138, 2067725182, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196437138, 2067725182, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196437138, 2067724801, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196437138, 2067724801, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196437138, 2067725153, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196437138, 2067725153, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196437138, 2067725372, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196437138, 2067725372, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196437138, 2067725343, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196437138, 2067725343, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196437138, 2067725636, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196437138, 2067725636, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196437138, 2067725307, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196437138, 2067725307, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196437138, 2067724907, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196437138, 2067724907, 9);
/* create way 196437438*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (196437438, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (196437438, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (196437438, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (196437438, 'building', 'yes', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196437438, 2067717938, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196437438, 2067717938, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196437438, 2067717838, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196437438, 2067717838, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196437438, 2067717780, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196437438, 2067717780, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196437438, 2067717435, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196437438, 2067717435, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196437438, 2067717582, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196437438, 2067717582, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196437438, 2067718304, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196437438, 2067718304, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196437438, 2067718169, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196437438, 2067718169, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196437438, 2067717922, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196437438, 2067717922, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196437438, 2067717938, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196437438, 2067717938, 9);
/* create way 196437642*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (196437642, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (196437642, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (196437642, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (196437642, 'building', 'yes', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196437642, 2067716948, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196437642, 2067716948, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196437642, 2067716829, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196437642, 2067716829, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196437642, 2067716616, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196437642, 2067716616, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196437642, 2067716737, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196437642, 2067716737, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196437642, 2067716641, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196437642, 2067716641, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196437642, 2067716772, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196437642, 2067716772, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196437642, 2067716981, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196437642, 2067716981, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196437642, 2067716875, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196437642, 2067716875, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196437642, 2067716948, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196437642, 2067716948, 9);
/* create way 196437982*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (196437982, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (196437982, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (196437982, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (196437982, 'building', 'yes', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196437982, 2067716015, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196437982, 2067716015, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196437982, 2067715648, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196437982, 2067715648, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196437982, 2067715876, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196437982, 2067715876, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196437982, 2067716198, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196437982, 2067716198, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196437982, 2067716108, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196437982, 2067716108, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196437982, 2067716414, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196437982, 2067716414, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196437982, 2067716312, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196437982, 2067716312, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196437982, 2067715964, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196437982, 2067715964, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196437982, 2067716015, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196437982, 2067716015, 9);
/* create way 196438155*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (196438155, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (196438155, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (196438155, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (196438155, 'building', 'yes', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196438155, 2067723975, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196438155, 2067723975, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196438155, 2067724101, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196438155, 2067724101, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196438155, 2067724115, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196438155, 2067724115, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196438155, 2067724437, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196438155, 2067724437, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196438155, 2067724342, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196438155, 2067724342, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196438155, 2067724219, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196438155, 2067724219, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196438155, 2067724198, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196438155, 2067724198, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196438155, 2067723916, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196438155, 2067723916, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196438155, 2067723975, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196438155, 2067723975, 9);
/* create way 196438262*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (196438262, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (196438262, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (196438262, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (196438262, 'building', 'yes', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196438262, 2067716575, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196438262, 2067716575, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196438262, 2067716821, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196438262, 2067716821, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196438262, 2067716840, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196438262, 2067716840, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196438262, 2067717193, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196438262, 2067717193, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196438262, 2067717016, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196438262, 2067717016, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196438262, 2067716682, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196438262, 2067716682, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196438262, 2067716637, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196438262, 2067716637, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196438262, 2067716412, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196438262, 2067716412, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196438262, 2067716575, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196438262, 2067716575, 9);
/* create way 196438291*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (196438291, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (196438291, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (196438291, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (196438291, 'building', 'yes', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196438291, 2067722271, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196438291, 2067722271, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196438291, 2067722169, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196438291, 2067722169, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196438291, 2067722301, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196438291, 2067722301, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196438291, 2067722583, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196438291, 2067722583, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196438291, 2067722499, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196438291, 2067722499, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196438291, 2067722646, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196438291, 2067722646, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196438291, 2067722559, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196438291, 2067722559, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196438291, 2067722317, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196438291, 2067722317, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196438291, 2067722271, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196438291, 2067722271, 9);
/* create way 196438551*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (196438551, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (196438551, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (196438551, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (196438551, 'building', 'yes', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196438551, 2067719605, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196438551, 2067719605, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196438551, 2067719488, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196438551, 2067719488, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196438551, 2067719581, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196438551, 2067719581, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196438551, 2067720154, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196438551, 2067720154, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196438551, 2067720065, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196438551, 2067720065, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196438551, 2067719834, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196438551, 2067719834, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196438551, 2067719818, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196438551, 2067719818, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196438551, 2067719553, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196438551, 2067719553, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196438551, 2067719605, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196438551, 2067719605, 9);
/* create way 196438685*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (196438685, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (196438685, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (196438685, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (196438685, 'building', 'yes', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196438685, 2067725474, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196438685, 2067725474, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196438685, 2067725345, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196438685, 2067725345, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196438685, 2067725458, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196438685, 2067725458, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196438685, 2067725594, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196438685, 2067725594, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196438685, 2067725511, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196438685, 2067725511, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196438685, 2067725842, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196438685, 2067725842, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196438685, 2067725732, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196438685, 2067725732, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196438685, 2067725399, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196438685, 2067725399, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196438685, 2067725474, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196438685, 2067725474, 9);
/* create way 196438899*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (196438899, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (196438899, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (196438899, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (196438899, 'building', 'yes', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196438899, 2067716855, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196438899, 2067716855, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196438899, 2067717004, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196438899, 2067717004, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196438899, 2067717037, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196438899, 2067717037, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196438899, 2067717377, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196438899, 2067717377, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196438899, 2067717223, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196438899, 2067717223, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196438899, 2067716906, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196438899, 2067716906, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196438899, 2067716849, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196438899, 2067716849, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196438899, 2067716694, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196438899, 2067716694, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196438899, 2067716855, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196438899, 2067716855, 9);
/* create way 196439188*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (196439188, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (196439188, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (196439188, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (196439188, 'building', 'yes', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196439188, 2067722367, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196439188, 2067722367, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196439188, 2067722509, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196439188, 2067722509, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196439188, 2067722754, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196439188, 2067722754, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196439188, 2067722809, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196439188, 2067722809, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196439188, 2067722863, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196439188, 2067722863, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196439188, 2067722820, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196439188, 2067722820, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196439188, 2067722881, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196439188, 2067722881, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196439188, 2067722728, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196439188, 2067722728, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196439188, 2067722367, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196439188, 2067722367, 9);
/* create way 196439435*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (196439435, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (196439435, 2, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (196439435, 'building', 'yes');
INSERT INTO way_tags (way_id, k, v, version) VALUES (196439435, 'building', 'yes', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196439435, 2067716130, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196439435, 2067716130, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196439435, 2067716289, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196439435, 2067716289, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196439435, 2067716338, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196439435, 2067716338, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196439435, 2067716621, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196439435, 2067716621, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196439435, 2067716868, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196439435, 2067716868, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196439435, 2067716793, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196439435, 2067716793, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196439435, 2067716520, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196439435, 2067716520, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196439435, 2067716510, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196439435, 2067716510, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196439435, 2067716083, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196439435, 2067716083, 9);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (196439435, 2067716130, 1, 10);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (196439435, 2067716130, 10);
UPDATE changesets SET min_lat=-12256, max_lat=424903656, min_lon=-714698746, max_lon=115, num_changes=1283 WHERE id=2;
