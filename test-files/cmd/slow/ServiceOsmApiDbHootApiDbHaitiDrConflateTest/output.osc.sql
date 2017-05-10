INSERT INTO changesets (id, user_id, created_at, closed_at) VALUES (3, 1, (now() at time zone 'utc'), (now() at time zone 'utc'));
INSERT INTO changeset_tags (changeset_id, k, v) VALUES (3, 'written_by', 'Hootenanny');
/* modify node 439*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (439, 185256801, -724795746, 3, true, (now() at time zone 'utc'), 1642347362, 2);
UPDATE current_nodes SET latitude=185256801, longitude=-724795746, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347362, version=2 WHERE id=439;
DELETE FROM current_node_tags WHERE node_id = 439;
DELETE FROM node_tags WHERE node_id = 439;
INSERT INTO current_node_tags (node_id, k, v) VALUES (439, 'hoot:id', '439');
INSERT INTO node_tags (node_id, k, v, version) VALUES (439, 'hoot:id', '439', 2);
/* modify node 445*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (445, 185241214, -724784817, 3, true, (now() at time zone 'utc'), 1642347362, 2);
UPDATE current_nodes SET latitude=185241214, longitude=-724784817, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347362, version=2 WHERE id=445;
DELETE FROM current_node_tags WHERE node_id = 445;
DELETE FROM node_tags WHERE node_id = 445;
INSERT INTO current_node_tags (node_id, k, v) VALUES (445, 'hoot:id', '445');
INSERT INTO node_tags (node_id, k, v, version) VALUES (445, 'hoot:id', '445', 2);
/* modify node 452*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (452, 185242427, -724786043, 3, true, (now() at time zone 'utc'), 1642347362, 2);
UPDATE current_nodes SET latitude=185242427, longitude=-724786043, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347362, version=2 WHERE id=452;
DELETE FROM current_node_tags WHERE node_id = 452;
DELETE FROM node_tags WHERE node_id = 452;
INSERT INTO current_node_tags (node_id, k, v) VALUES (452, 'hoot:id', '452');
INSERT INTO node_tags (node_id, k, v, version) VALUES (452, 'hoot:id', '452', 2);
/* modify node 466*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (466, 185243507, -724786014, 3, true, (now() at time zone 'utc'), 1642347362, 2);
UPDATE current_nodes SET latitude=185243507, longitude=-724786014, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347362, version=2 WHERE id=466;
DELETE FROM current_node_tags WHERE node_id = 466;
DELETE FROM node_tags WHERE node_id = 466;
INSERT INTO current_node_tags (node_id, k, v) VALUES (466, 'hoot:id', '466');
INSERT INTO node_tags (node_id, k, v, version) VALUES (466, 'hoot:id', '466', 2);
/* modify node 467*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (467, 185244345, -724784932, 3, true, (now() at time zone 'utc'), 1642347362, 2);
UPDATE current_nodes SET latitude=185244345, longitude=-724784932, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347362, version=2 WHERE id=467;
DELETE FROM current_node_tags WHERE node_id = 467;
DELETE FROM node_tags WHERE node_id = 467;
INSERT INTO current_node_tags (node_id, k, v) VALUES (467, 'hoot:id', '467');
INSERT INTO node_tags (node_id, k, v, version) VALUES (467, 'hoot:id', '467', 2);
/* modify node 469*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (469, 185245696, -724784960, 3, true, (now() at time zone 'utc'), 1642347362, 2);
UPDATE current_nodes SET latitude=185245696, longitude=-724784960, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347362, version=2 WHERE id=469;
DELETE FROM current_node_tags WHERE node_id = 469;
DELETE FROM node_tags WHERE node_id = 469;
INSERT INTO current_node_tags (node_id, k, v) VALUES (469, 'hoot:id', '469');
INSERT INTO node_tags (node_id, k, v, version) VALUES (469, 'hoot:id', '469', 2);
/* modify node 470*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (470, 185246588, -724786328, 3, true, (now() at time zone 'utc'), 1642347362, 2);
UPDATE current_nodes SET latitude=185246588, longitude=-724786328, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347362, version=2 WHERE id=470;
DELETE FROM current_node_tags WHERE node_id = 470;
DELETE FROM node_tags WHERE node_id = 470;
INSERT INTO current_node_tags (node_id, k, v) VALUES (470, 'hoot:id', '470');
INSERT INTO node_tags (node_id, k, v, version) VALUES (470, 'hoot:id', '470', 2);
/* modify node 472*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (472, 185247479, -724789348, 3, true, (now() at time zone 'utc'), 1642347362, 2);
UPDATE current_nodes SET latitude=185247479, longitude=-724789348, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347362, version=2 WHERE id=472;
DELETE FROM current_node_tags WHERE node_id = 472;
DELETE FROM node_tags WHERE node_id = 472;
INSERT INTO current_node_tags (node_id, k, v) VALUES (472, 'hoot:id', '472');
INSERT INTO node_tags (node_id, k, v, version) VALUES (472, 'hoot:id', '472', 2);
/* modify node 474*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (474, 185248128, -724789975, 3, true, (now() at time zone 'utc'), 1642347362, 2);
UPDATE current_nodes SET latitude=185248128, longitude=-724789975, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347362, version=2 WHERE id=474;
DELETE FROM current_node_tags WHERE node_id = 474;
DELETE FROM node_tags WHERE node_id = 474;
INSERT INTO current_node_tags (node_id, k, v) VALUES (474, 'hoot:id', '474');
INSERT INTO node_tags (node_id, k, v, version) VALUES (474, 'hoot:id', '474', 2);
/* modify node 475*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (475, 185249033, -724789334, 3, true, (now() at time zone 'utc'), 1642347362, 2);
UPDATE current_nodes SET latitude=185249033, longitude=-724789334, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347362, version=2 WHERE id=475;
DELETE FROM current_node_tags WHERE node_id = 475;
DELETE FROM node_tags WHERE node_id = 475;
INSERT INTO current_node_tags (node_id, k, v) VALUES (475, 'hoot:id', '475');
INSERT INTO node_tags (node_id, k, v, version) VALUES (475, 'hoot:id', '475', 2);
/* modify node 477*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (477, 185249675, -724789441, 3, true, (now() at time zone 'utc'), 1642347362, 2);
UPDATE current_nodes SET latitude=185249675, longitude=-724789441, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347362, version=2 WHERE id=477;
DELETE FROM current_node_tags WHERE node_id = 477;
DELETE FROM node_tags WHERE node_id = 477;
INSERT INTO current_node_tags (node_id, k, v) VALUES (477, 'hoot:id', '477');
INSERT INTO node_tags (node_id, k, v, version) VALUES (477, 'hoot:id', '477', 2);
/* modify node 478*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (478, 185250553, -724792148, 3, true, (now() at time zone 'utc'), 1642347362, 2);
UPDATE current_nodes SET latitude=185250553, longitude=-724792148, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347362, version=2 WHERE id=478;
DELETE FROM current_node_tags WHERE node_id = 478;
DELETE FROM node_tags WHERE node_id = 478;
INSERT INTO current_node_tags (node_id, k, v) VALUES (478, 'hoot:id', '478');
INSERT INTO node_tags (node_id, k, v, version) VALUES (478, 'hoot:id', '478', 2);
/* modify node 480*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (480, 185250890, -724794143, 3, true, (now() at time zone 'utc'), 1642347362, 2);
UPDATE current_nodes SET latitude=185250890, longitude=-724794143, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347362, version=2 WHERE id=480;
DELETE FROM current_node_tags WHERE node_id = 480;
DELETE FROM node_tags WHERE node_id = 480;
INSERT INTO current_node_tags (node_id, k, v) VALUES (480, 'hoot:id', '480');
INSERT INTO node_tags (node_id, k, v, version) VALUES (480, 'hoot:id', '480', 2);
/* modify node 482*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (482, 185251836, -724794891, 3, true, (now() at time zone 'utc'), 1642347362, 2);
UPDATE current_nodes SET latitude=185251836, longitude=-724794891, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347362, version=2 WHERE id=482;
DELETE FROM current_node_tags WHERE node_id = 482;
DELETE FROM node_tags WHERE node_id = 482;
INSERT INTO current_node_tags (node_id, k, v) VALUES (482, 'hoot:id', '482');
INSERT INTO node_tags (node_id, k, v, version) VALUES (482, 'hoot:id', '482', 2);
/* modify node 483*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (483, 185252309, -724795496, 3, true, (now() at time zone 'utc'), 1642347362, 2);
UPDATE current_nodes SET latitude=185252309, longitude=-724795496, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347362, version=2 WHERE id=483;
DELETE FROM current_node_tags WHERE node_id = 483;
DELETE FROM node_tags WHERE node_id = 483;
INSERT INTO current_node_tags (node_id, k, v) VALUES (483, 'hoot:id', '483');
INSERT INTO node_tags (node_id, k, v, version) VALUES (483, 'hoot:id', '483', 2);
/* modify node 485*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (485, 185255213, -724795318, 3, true, (now() at time zone 'utc'), 1642347362, 2);
UPDATE current_nodes SET latitude=185255213, longitude=-724795318, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347362, version=2 WHERE id=485;
DELETE FROM current_node_tags WHERE node_id = 485;
DELETE FROM node_tags WHERE node_id = 485;
INSERT INTO current_node_tags (node_id, k, v) VALUES (485, 'hoot:id', '485');
INSERT INTO node_tags (node_id, k, v, version) VALUES (485, 'hoot:id', '485', 2);
/* modify node 487*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (487, 185256632, -724794926, 3, true, (now() at time zone 'utc'), 1642347362, 2);
UPDATE current_nodes SET latitude=185256632, longitude=-724794926, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347362, version=2 WHERE id=487;
DELETE FROM current_node_tags WHERE node_id = 487;
DELETE FROM node_tags WHERE node_id = 487;
INSERT INTO current_node_tags (node_id, k, v) VALUES (487, 'hoot:id', '487');
INSERT INTO node_tags (node_id, k, v, version) VALUES (487, 'hoot:id', '487', 2);
/* modify node 489*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (489, 185258861, -724796422, 3, true, (now() at time zone 'utc'), 1642347362, 2);
UPDATE current_nodes SET latitude=185258861, longitude=-724796422, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347362, version=2 WHERE id=489;
DELETE FROM current_node_tags WHERE node_id = 489;
DELETE FROM node_tags WHERE node_id = 489;
INSERT INTO current_node_tags (node_id, k, v) VALUES (489, 'hoot:id', '489');
INSERT INTO node_tags (node_id, k, v, version) VALUES (489, 'hoot:id', '489', 2);
/* modify node 490*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (490, 185259368, -724799877, 3, true, (now() at time zone 'utc'), 1642347362, 2);
UPDATE current_nodes SET latitude=185259368, longitude=-724799877, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347362, version=2 WHERE id=490;
DELETE FROM current_node_tags WHERE node_id = 490;
DELETE FROM node_tags WHERE node_id = 490;
INSERT INTO current_node_tags (node_id, k, v) VALUES (490, 'hoot:id', '490');
INSERT INTO node_tags (node_id, k, v, version) VALUES (490, 'hoot:id', '490', 2);
/* modify node 493*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (493, 185260887, -724803297, 3, true, (now() at time zone 'utc'), 1642347363, 2);
UPDATE current_nodes SET latitude=185260887, longitude=-724803297, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347363, version=2 WHERE id=493;
DELETE FROM current_node_tags WHERE node_id = 493;
DELETE FROM node_tags WHERE node_id = 493;
INSERT INTO current_node_tags (node_id, k, v) VALUES (493, 'hoot:id', '493');
INSERT INTO node_tags (node_id, k, v, version) VALUES (493, 'hoot:id', '493', 2);
/* modify node 494*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (494, 185263184, -724805577, 3, true, (now() at time zone 'utc'), 1642347363, 2);
UPDATE current_nodes SET latitude=185263184, longitude=-724805577, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347363, version=2 WHERE id=494;
DELETE FROM current_node_tags WHERE node_id = 494;
DELETE FROM node_tags WHERE node_id = 494;
INSERT INTO current_node_tags (node_id, k, v) VALUES (494, 'hoot:id', '494');
INSERT INTO node_tags (node_id, k, v, version) VALUES (494, 'hoot:id', '494', 2);
/* modify node 496*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (496, 185266359, -724806111, 3, true, (now() at time zone 'utc'), 1642347363, 2);
UPDATE current_nodes SET latitude=185266359, longitude=-724806111, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347363, version=2 WHERE id=496;
DELETE FROM current_node_tags WHERE node_id = 496;
DELETE FROM node_tags WHERE node_id = 496;
INSERT INTO current_node_tags (node_id, k, v) VALUES (496, 'hoot:id', '496');
INSERT INTO node_tags (node_id, k, v, version) VALUES (496, 'hoot:id', '496', 2);
/* modify node 497*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (497, 185267068, -724806111, 3, true, (now() at time zone 'utc'), 1642347363, 2);
UPDATE current_nodes SET latitude=185267068, longitude=-724806111, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347363, version=2 WHERE id=497;
DELETE FROM current_node_tags WHERE node_id = 497;
DELETE FROM node_tags WHERE node_id = 497;
INSERT INTO current_node_tags (node_id, k, v) VALUES (497, 'hoot:id', '497');
INSERT INTO node_tags (node_id, k, v, version) VALUES (497, 'hoot:id', '497', 2);
/* modify node 499*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (499, 185267980, -724806894, 3, true, (now() at time zone 'utc'), 1642347363, 2);
UPDATE current_nodes SET latitude=185267980, longitude=-724806894, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347363, version=2 WHERE id=499;
DELETE FROM current_node_tags WHERE node_id = 499;
DELETE FROM node_tags WHERE node_id = 499;
INSERT INTO current_node_tags (node_id, k, v) VALUES (499, 'hoot:id', '499');
INSERT INTO node_tags (node_id, k, v, version) VALUES (499, 'hoot:id', '499', 2);
/* modify node 501*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (501, 185269196, -724806966, 3, true, (now() at time zone 'utc'), 1642347363, 2);
UPDATE current_nodes SET latitude=185269196, longitude=-724806966, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347363, version=2 WHERE id=501;
DELETE FROM current_node_tags WHERE node_id = 501;
DELETE FROM node_tags WHERE node_id = 501;
INSERT INTO current_node_tags (node_id, k, v) VALUES (501, 'hoot:id', '501');
INSERT INTO node_tags (node_id, k, v, version) VALUES (501, 'hoot:id', '501', 2);
/* modify node 503*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (503, 185269871, -724806894, 3, true, (now() at time zone 'utc'), 1642347363, 2);
UPDATE current_nodes SET latitude=185269871, longitude=-724806894, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347363, version=2 WHERE id=503;
DELETE FROM current_node_tags WHERE node_id = 503;
DELETE FROM node_tags WHERE node_id = 503;
INSERT INTO current_node_tags (node_id, k, v) VALUES (503, 'hoot:id', '503');
INSERT INTO node_tags (node_id, k, v, version) VALUES (503, 'hoot:id', '503', 2);
/* modify node 504*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (504, 185272100, -724807678, 3, true, (now() at time zone 'utc'), 1642347363, 2);
UPDATE current_nodes SET latitude=185272100, longitude=-724807678, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347363, version=2 WHERE id=504;
DELETE FROM current_node_tags WHERE node_id = 504;
DELETE FROM node_tags WHERE node_id = 504;
INSERT INTO current_node_tags (node_id, k, v) VALUES (504, 'hoot:id', '504');
INSERT INTO node_tags (node_id, k, v, version) VALUES (504, 'hoot:id', '504', 2);
/* modify node 506*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (506, 185273181, -724807536, 3, true, (now() at time zone 'utc'), 1642347363, 2);
UPDATE current_nodes SET latitude=185273181, longitude=-724807536, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347363, version=2 WHERE id=506;
DELETE FROM current_node_tags WHERE node_id = 506;
DELETE FROM node_tags WHERE node_id = 506;
INSERT INTO current_node_tags (node_id, k, v) VALUES (506, 'hoot:id', '506');
INSERT INTO node_tags (node_id, k, v, version) VALUES (506, 'hoot:id', '506', 2);
/* modify node 507*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (507, 185274281, -724805585, 3, true, (now() at time zone 'utc'), 1642347363, 2);
UPDATE current_nodes SET latitude=185274281, longitude=-724805585, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347363, version=2 WHERE id=507;
DELETE FROM current_node_tags WHERE node_id = 507;
DELETE FROM node_tags WHERE node_id = 507;
INSERT INTO current_node_tags (node_id, k, v) VALUES (507, 'hoot:id', '507');
INSERT INTO node_tags (node_id, k, v, version) VALUES (507, 'hoot:id', '507', 2);
/* modify node 510*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (510, 185277118, -724804588, 3, true, (now() at time zone 'utc'), 1642347363, 2);
UPDATE current_nodes SET latitude=185277118, longitude=-724804588, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347363, version=2 WHERE id=510;
DELETE FROM current_node_tags WHERE node_id = 510;
DELETE FROM node_tags WHERE node_id = 510;
INSERT INTO current_node_tags (node_id, k, v) VALUES (510, 'hoot:id', '510');
INSERT INTO node_tags (node_id, k, v, version) VALUES (510, 'hoot:id', '510', 2);
/* modify node 511*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (511, 185278739, -724803377, 3, true, (now() at time zone 'utc'), 1642347363, 2);
UPDATE current_nodes SET latitude=185278739, longitude=-724803377, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347363, version=2 WHERE id=511;
DELETE FROM current_node_tags WHERE node_id = 511;
DELETE FROM node_tags WHERE node_id = 511;
INSERT INTO current_node_tags (node_id, k, v) VALUES (511, 'hoot:id', '511');
INSERT INTO node_tags (node_id, k, v, version) VALUES (511, 'hoot:id', '511', 2);
/* modify node 513*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (513, 185281238, -724802843, 3, true, (now() at time zone 'utc'), 1642347363, 2);
UPDATE current_nodes SET latitude=185281238, longitude=-724802843, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347363, version=2 WHERE id=513;
DELETE FROM current_node_tags WHERE node_id = 513;
DELETE FROM node_tags WHERE node_id = 513;
INSERT INTO current_node_tags (node_id, k, v) VALUES (513, 'hoot:id', '513');
INSERT INTO node_tags (node_id, k, v, version) VALUES (513, 'hoot:id', '513', 2);
/* modify node 514*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (514, 185281744, -724802843, 3, true, (now() at time zone 'utc'), 1642347363, 2);
UPDATE current_nodes SET latitude=185281744, longitude=-724802843, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347363, version=2 WHERE id=514;
DELETE FROM current_node_tags WHERE node_id = 514;
DELETE FROM node_tags WHERE node_id = 514;
INSERT INTO current_node_tags (node_id, k, v) VALUES (514, 'hoot:id', '514');
INSERT INTO node_tags (node_id, k, v, version) VALUES (514, 'hoot:id', '514', 2);
/* modify node 516*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (516, 185282238, -724802458, 3, true, (now() at time zone 'utc'), 1642347363, 2);
UPDATE current_nodes SET latitude=185282238, longitude=-724802458, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347363, version=2 WHERE id=516;
DELETE FROM current_node_tags WHERE node_id = 516;
DELETE FROM node_tags WHERE node_id = 516;
INSERT INTO current_node_tags (node_id, k, v) VALUES (516, 'hoot:id', '516');
INSERT INTO node_tags (node_id, k, v, version) VALUES (516, 'hoot:id', '516', 2);
/* modify node 518*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (518, 185283129, -724803142, 3, true, (now() at time zone 'utc'), 1642347363, 2);
UPDATE current_nodes SET latitude=185283129, longitude=-724803142, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347363, version=2 WHERE id=518;
DELETE FROM current_node_tags WHERE node_id = 518;
DELETE FROM node_tags WHERE node_id = 518;
INSERT INTO current_node_tags (node_id, k, v) VALUES (518, 'hoot:id', '518');
INSERT INTO node_tags (node_id, k, v, version) VALUES (518, 'hoot:id', '518', 2);
/* modify node 519*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (519, 185283805, -724802230, 3, true, (now() at time zone 'utc'), 1642347363, 2);
UPDATE current_nodes SET latitude=185283805, longitude=-724802230, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347363, version=2 WHERE id=519;
DELETE FROM current_node_tags WHERE node_id = 519;
DELETE FROM node_tags WHERE node_id = 519;
INSERT INTO current_node_tags (node_id, k, v) VALUES (519, 'hoot:id', '519');
INSERT INTO node_tags (node_id, k, v, version) VALUES (519, 'hoot:id', '519', 2);
/* modify node 521*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (521, 185258253, -724795033, 3, true, (now() at time zone 'utc'), 1642347362, 2);
UPDATE current_nodes SET latitude=185258253, longitude=-724795033, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347362, version=2 WHERE id=521;
DELETE FROM current_node_tags WHERE node_id = 521;
DELETE FROM node_tags WHERE node_id = 521;
INSERT INTO current_node_tags (node_id, k, v) VALUES (521, 'hoot:id', '521');
INSERT INTO node_tags (node_id, k, v, version) VALUES (521, 'hoot:id', '521', 2);
/* modify node 540*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (540, 185284453, -724802715, 3, true, (now() at time zone 'utc'), 1642347363, 2);
UPDATE current_nodes SET latitude=185284453, longitude=-724802715, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347363, version=2 WHERE id=540;
DELETE FROM current_node_tags WHERE node_id = 540;
DELETE FROM node_tags WHERE node_id = 540;
INSERT INTO current_node_tags (node_id, k, v) VALUES (540, 'hoot:id', '540');
INSERT INTO node_tags (node_id, k, v, version) VALUES (540, 'hoot:id', '540', 2);
/* modify node 541*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (541, 185285048, -724802031, 3, true, (now() at time zone 'utc'), 1642347363, 2);
UPDATE current_nodes SET latitude=185285048, longitude=-724802031, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347363, version=2 WHERE id=541;
DELETE FROM current_node_tags WHERE node_id = 541;
DELETE FROM node_tags WHERE node_id = 541;
INSERT INTO current_node_tags (node_id, k, v) VALUES (541, 'hoot:id', '541');
INSERT INTO node_tags (node_id, k, v, version) VALUES (541, 'hoot:id', '541', 2);
/* modify node 542*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (542, 185285561, -724802373, 3, true, (now() at time zone 'utc'), 1642347363, 2);
UPDATE current_nodes SET latitude=185285561, longitude=-724802373, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347363, version=2 WHERE id=542;
DELETE FROM current_node_tags WHERE node_id = 542;
DELETE FROM node_tags WHERE node_id = 542;
INSERT INTO current_node_tags (node_id, k, v) VALUES (542, 'hoot:id', '542');
INSERT INTO node_tags (node_id, k, v, version) VALUES (542, 'hoot:id', '542', 2);
/* modify node 543*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (543, 185286209, -724801717, 3, true, (now() at time zone 'utc'), 1642347363, 2);
UPDATE current_nodes SET latitude=185286209, longitude=-724801717, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347363, version=2 WHERE id=543;
DELETE FROM current_node_tags WHERE node_id = 543;
DELETE FROM node_tags WHERE node_id = 543;
INSERT INTO current_node_tags (node_id, k, v) VALUES (543, 'hoot:id', '543');
INSERT INTO node_tags (node_id, k, v, version) VALUES (543, 'hoot:id', '543', 2);
/* modify node 544*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (544, 185286588, -724802059, 3, true, (now() at time zone 'utc'), 1642347363, 2);
UPDATE current_nodes SET latitude=185286588, longitude=-724802059, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347363, version=2 WHERE id=544;
DELETE FROM current_node_tags WHERE node_id = 544;
DELETE FROM node_tags WHERE node_id = 544;
INSERT INTO current_node_tags (node_id, k, v) VALUES (544, 'hoot:id', '544');
INSERT INTO node_tags (node_id, k, v, version) VALUES (544, 'hoot:id', '544', 2);
/* modify node 545*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (545, 185287830, -724801204, 3, true, (now() at time zone 'utc'), 1642347366, 2);
UPDATE current_nodes SET latitude=185287830, longitude=-724801204, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347366, version=2 WHERE id=545;
DELETE FROM current_node_tags WHERE node_id = 545;
DELETE FROM node_tags WHERE node_id = 545;
INSERT INTO current_node_tags (node_id, k, v) VALUES (545, 'hoot:id', '545');
INSERT INTO node_tags (node_id, k, v, version) VALUES (545, 'hoot:id', '545', 2);
/* modify node 546*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (546, 185288371, -724799808, 3, true, (now() at time zone 'utc'), 1642347366, 2);
UPDATE current_nodes SET latitude=185288371, longitude=-724799808, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347366, version=2 WHERE id=546;
DELETE FROM current_node_tags WHERE node_id = 546;
DELETE FROM node_tags WHERE node_id = 546;
INSERT INTO current_node_tags (node_id, k, v) VALUES (546, 'hoot:id', '546');
INSERT INTO node_tags (node_id, k, v, version) VALUES (546, 'hoot:id', '546', 2);
/* modify node 547*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (547, 185289235, -724800093, 3, true, (now() at time zone 'utc'), 1642347366, 2);
UPDATE current_nodes SET latitude=185289235, longitude=-724800093, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347366, version=2 WHERE id=547;
DELETE FROM current_node_tags WHERE node_id = 547;
DELETE FROM node_tags WHERE node_id = 547;
INSERT INTO current_node_tags (node_id, k, v) VALUES (547, 'hoot:id', '547');
INSERT INTO node_tags (node_id, k, v, version) VALUES (547, 'hoot:id', '547', 2);
/* modify node 548*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (548, 185289479, -724798269, 3, true, (now() at time zone 'utc'), 1642347366, 2);
UPDATE current_nodes SET latitude=185289479, longitude=-724798269, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347366, version=2 WHERE id=548;
DELETE FROM current_node_tags WHERE node_id = 548;
DELETE FROM node_tags WHERE node_id = 548;
INSERT INTO current_node_tags (node_id, k, v) VALUES (548, 'hoot:id', '548');
INSERT INTO node_tags (node_id, k, v, version) VALUES (548, 'hoot:id', '548', 2);
/* modify node 549*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (549, 185290640, -724796246, 3, true, (now() at time zone 'utc'), 1642347366, 2);
UPDATE current_nodes SET latitude=185290640, longitude=-724796246, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347366, version=2 WHERE id=549;
DELETE FROM current_node_tags WHERE node_id = 549;
DELETE FROM node_tags WHERE node_id = 549;
INSERT INTO current_node_tags (node_id, k, v) VALUES (549, 'hoot:id', '549');
INSERT INTO node_tags (node_id, k, v, version) VALUES (549, 'hoot:id', '549', 2);
/* modify node 550*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (550, 185291478, -724795762, 3, true, (now() at time zone 'utc'), 1642347366, 2);
UPDATE current_nodes SET latitude=185291478, longitude=-724795762, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347366, version=2 WHERE id=550;
DELETE FROM current_node_tags WHERE node_id = 550;
DELETE FROM node_tags WHERE node_id = 550;
INSERT INTO current_node_tags (node_id, k, v) VALUES (550, 'hoot:id', '550');
INSERT INTO node_tags (node_id, k, v, version) VALUES (550, 'hoot:id', '550', 2);
/* modify node 551*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (551, 185292532, -724793824, 3, true, (now() at time zone 'utc'), 1642347366, 2);
UPDATE current_nodes SET latitude=185292532, longitude=-724793824, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347366, version=2 WHERE id=551;
DELETE FROM current_node_tags WHERE node_id = 551;
DELETE FROM node_tags WHERE node_id = 551;
INSERT INTO current_node_tags (node_id, k, v) VALUES (551, 'hoot:id', '551');
INSERT INTO node_tags (node_id, k, v, version) VALUES (551, 'hoot:id', '551', 2);
/* modify node 552*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (552, 185293045, -724791972, 3, true, (now() at time zone 'utc'), 1642347366, 2);
UPDATE current_nodes SET latitude=185293045, longitude=-724791972, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347366, version=2 WHERE id=552;
DELETE FROM current_node_tags WHERE node_id = 552;
DELETE FROM node_tags WHERE node_id = 552;
INSERT INTO current_node_tags (node_id, k, v) VALUES (552, 'hoot:id', '552');
INSERT INTO node_tags (node_id, k, v, version) VALUES (552, 'hoot:id', '552', 2);
/* modify node 553*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (553, 185292964, -724790946, 3, true, (now() at time zone 'utc'), 1642347366, 2);
UPDATE current_nodes SET latitude=185292964, longitude=-724790946, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347366, version=2 WHERE id=553;
DELETE FROM current_node_tags WHERE node_id = 553;
DELETE FROM node_tags WHERE node_id = 553;
INSERT INTO current_node_tags (node_id, k, v) VALUES (553, 'hoot:id', '553');
INSERT INTO node_tags (node_id, k, v, version) VALUES (553, 'hoot:id', '553', 2);
/* modify node 554*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (554, 185293504, -724791003, 3, true, (now() at time zone 'utc'), 1642347366, 2);
UPDATE current_nodes SET latitude=185293504, longitude=-724791003, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347366, version=2 WHERE id=554;
DELETE FROM current_node_tags WHERE node_id = 554;
DELETE FROM node_tags WHERE node_id = 554;
INSERT INTO current_node_tags (node_id, k, v) VALUES (554, 'hoot:id', '554');
INSERT INTO node_tags (node_id, k, v, version) VALUES (554, 'hoot:id', '554', 2);
/* modify node 555*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (555, 185293477, -724789493, 3, true, (now() at time zone 'utc'), 1642347366, 2);
UPDATE current_nodes SET latitude=185293477, longitude=-724789493, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347366, version=2 WHERE id=555;
DELETE FROM current_node_tags WHERE node_id = 555;
DELETE FROM node_tags WHERE node_id = 555;
INSERT INTO current_node_tags (node_id, k, v) VALUES (555, 'hoot:id', '555');
INSERT INTO node_tags (node_id, k, v, version) VALUES (555, 'hoot:id', '555', 2);
/* modify node 556*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (556, 185294099, -724788153, 3, true, (now() at time zone 'utc'), 1642347366, 2);
UPDATE current_nodes SET latitude=185294099, longitude=-724788153, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347366, version=2 WHERE id=556;
DELETE FROM current_node_tags WHERE node_id = 556;
DELETE FROM node_tags WHERE node_id = 556;
INSERT INTO current_node_tags (node_id, k, v) VALUES (556, 'hoot:id', '556');
INSERT INTO node_tags (node_id, k, v, version) VALUES (556, 'hoot:id', '556', 2);
/* modify node 557*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (557, 185294963, -724785931, 3, true, (now() at time zone 'utc'), 1642347366, 2);
UPDATE current_nodes SET latitude=185294963, longitude=-724785931, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347366, version=2 WHERE id=557;
DELETE FROM current_node_tags WHERE node_id = 557;
DELETE FROM node_tags WHERE node_id = 557;
INSERT INTO current_node_tags (node_id, k, v) VALUES (557, 'hoot:id', '557');
INSERT INTO node_tags (node_id, k, v, version) VALUES (557, 'hoot:id', '557', 2);
/* modify node 558*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (558, 185294855, -724784420, 3, true, (now() at time zone 'utc'), 1642347366, 2);
UPDATE current_nodes SET latitude=185294855, longitude=-724784420, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347366, version=2 WHERE id=558;
DELETE FROM current_node_tags WHERE node_id = 558;
DELETE FROM node_tags WHERE node_id = 558;
INSERT INTO current_node_tags (node_id, k, v) VALUES (558, 'hoot:id', '558');
INSERT INTO node_tags (node_id, k, v, version) VALUES (558, 'hoot:id', '558', 2);
/* modify node 559*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (559, 185296125, -724785047, 3, true, (now() at time zone 'utc'), 1642347366, 2);
UPDATE current_nodes SET latitude=185296125, longitude=-724785047, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347366, version=2 WHERE id=559;
DELETE FROM current_node_tags WHERE node_id = 559;
DELETE FROM node_tags WHERE node_id = 559;
INSERT INTO current_node_tags (node_id, k, v) VALUES (559, 'hoot:id', '559');
INSERT INTO node_tags (node_id, k, v, version) VALUES (559, 'hoot:id', '559', 2);
/* modify node 560*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (560, 185296855, -724786016, 3, true, (now() at time zone 'utc'), 1642347366, 2);
UPDATE current_nodes SET latitude=185296855, longitude=-724786016, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347366, version=2 WHERE id=560;
DELETE FROM current_node_tags WHERE node_id = 560;
DELETE FROM node_tags WHERE node_id = 560;
INSERT INTO current_node_tags (node_id, k, v) VALUES (560, 'hoot:id', '560');
INSERT INTO node_tags (node_id, k, v, version) VALUES (560, 'hoot:id', '560', 2);
/* modify node 561*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (561, 185298989, -724787954, 3, true, (now() at time zone 'utc'), 1642347366, 2);
UPDATE current_nodes SET latitude=185298989, longitude=-724787954, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347366, version=2 WHERE id=561;
DELETE FROM current_node_tags WHERE node_id = 561;
DELETE FROM node_tags WHERE node_id = 561;
INSERT INTO current_node_tags (node_id, k, v) VALUES (561, 'hoot:id', '561');
INSERT INTO node_tags (node_id, k, v, version) VALUES (561, 'hoot:id', '561', 2);
/* modify node 562*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (562, 185300205, -724787498, 3, true, (now() at time zone 'utc'), 1642347366, 2);
UPDATE current_nodes SET latitude=185300205, longitude=-724787498, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347366, version=2 WHERE id=562;
DELETE FROM current_node_tags WHERE node_id = 562;
DELETE FROM node_tags WHERE node_id = 562;
INSERT INTO current_node_tags (node_id, k, v) VALUES (562, 'hoot:id', '562');
INSERT INTO node_tags (node_id, k, v, version) VALUES (562, 'hoot:id', '562', 2);
/* modify node 563*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (563, 185301231, -724787669, 3, true, (now() at time zone 'utc'), 1642347366, 2);
UPDATE current_nodes SET latitude=185301231, longitude=-724787669, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347366, version=2 WHERE id=563;
DELETE FROM current_node_tags WHERE node_id = 563;
DELETE FROM node_tags WHERE node_id = 563;
INSERT INTO current_node_tags (node_id, k, v) VALUES (563, 'hoot:id', '563');
INSERT INTO node_tags (node_id, k, v, version) VALUES (563, 'hoot:id', '563', 2);
/* modify node 564*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (564, 185302285, -724788353, 3, true, (now() at time zone 'utc'), 1642347366, 2);
UPDATE current_nodes SET latitude=185302285, longitude=-724788353, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347366, version=2 WHERE id=564;
DELETE FROM current_node_tags WHERE node_id = 564;
DELETE FROM node_tags WHERE node_id = 564;
INSERT INTO current_node_tags (node_id, k, v) VALUES (564, 'hoot:id', '564');
INSERT INTO node_tags (node_id, k, v, version) VALUES (564, 'hoot:id', '564', 2);
/* modify node 565*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (565, 185305068, -724788381, 3, true, (now() at time zone 'utc'), 1642347366, 2);
UPDATE current_nodes SET latitude=185305068, longitude=-724788381, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347366, version=2 WHERE id=565;
DELETE FROM current_node_tags WHERE node_id = 565;
DELETE FROM node_tags WHERE node_id = 565;
INSERT INTO current_node_tags (node_id, k, v) VALUES (565, 'hoot:id', '565');
INSERT INTO node_tags (node_id, k, v, version) VALUES (565, 'hoot:id', '565', 2);
/* modify node 566*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (566, 185306554, -724787868, 3, true, (now() at time zone 'utc'), 1642347366, 2);
UPDATE current_nodes SET latitude=185306554, longitude=-724787868, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347366, version=2 WHERE id=566;
DELETE FROM current_node_tags WHERE node_id = 566;
DELETE FROM node_tags WHERE node_id = 566;
INSERT INTO current_node_tags (node_id, k, v) VALUES (566, 'hoot:id', '566');
INSERT INTO node_tags (node_id, k, v, version) VALUES (566, 'hoot:id', '566', 2);
/* modify node 567*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (567, 185307581, -724787412, 3, true, (now() at time zone 'utc'), 1642347366, 2);
UPDATE current_nodes SET latitude=185307581, longitude=-724787412, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347366, version=2 WHERE id=567;
DELETE FROM current_node_tags WHERE node_id = 567;
DELETE FROM node_tags WHERE node_id = 567;
INSERT INTO current_node_tags (node_id, k, v) VALUES (567, 'hoot:id', '567');
INSERT INTO node_tags (node_id, k, v, version) VALUES (567, 'hoot:id', '567', 2);
/* modify node 568*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (568, 185308607, -724787071, 3, true, (now() at time zone 'utc'), 1642347366, 2);
UPDATE current_nodes SET latitude=185308607, longitude=-724787071, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347366, version=2 WHERE id=568;
DELETE FROM current_node_tags WHERE node_id = 568;
DELETE FROM node_tags WHERE node_id = 568;
INSERT INTO current_node_tags (node_id, k, v) VALUES (568, 'hoot:id', '568');
INSERT INTO node_tags (node_id, k, v, version) VALUES (568, 'hoot:id', '568', 2);
/* modify node 569*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (569, 185309175, -724785190, 3, true, (now() at time zone 'utc'), 1642347366, 2);
UPDATE current_nodes SET latitude=185309175, longitude=-724785190, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347366, version=2 WHERE id=569;
DELETE FROM current_node_tags WHERE node_id = 569;
DELETE FROM node_tags WHERE node_id = 569;
INSERT INTO current_node_tags (node_id, k, v) VALUES (569, 'hoot:id', '569');
INSERT INTO node_tags (node_id, k, v, version) VALUES (569, 'hoot:id', '569', 2);
/* modify node 570*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (570, 185310553, -724785218, 3, true, (now() at time zone 'utc'), 1642347366, 2);
UPDATE current_nodes SET latitude=185310553, longitude=-724785218, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347366, version=2 WHERE id=570;
DELETE FROM current_node_tags WHERE node_id = 570;
DELETE FROM node_tags WHERE node_id = 570;
INSERT INTO current_node_tags (node_id, k, v) VALUES (570, 'hoot:id', '570');
INSERT INTO node_tags (node_id, k, v, version) VALUES (570, 'hoot:id', '570', 2);
/* modify node 571*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (571, 185311498, -724783423, 3, true, (now() at time zone 'utc'), 1642347366, 2);
UPDATE current_nodes SET latitude=185311498, longitude=-724783423, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347366, version=2 WHERE id=571;
DELETE FROM current_node_tags WHERE node_id = 571;
DELETE FROM node_tags WHERE node_id = 571;
INSERT INTO current_node_tags (node_id, k, v) VALUES (571, 'hoot:id', '571');
INSERT INTO node_tags (node_id, k, v, version) VALUES (571, 'hoot:id', '571', 2);
/* modify node 572*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (572, 185312822, -724782882, 3, true, (now() at time zone 'utc'), 1642347366, 2);
UPDATE current_nodes SET latitude=185312822, longitude=-724782882, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347366, version=2 WHERE id=572;
DELETE FROM current_node_tags WHERE node_id = 572;
DELETE FROM node_tags WHERE node_id = 572;
INSERT INTO current_node_tags (node_id, k, v) VALUES (572, 'hoot:id', '572');
INSERT INTO node_tags (node_id, k, v, version) VALUES (572, 'hoot:id', '572', 2);
/* modify node 573*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (573, 185315146, -724779719, 3, true, (now() at time zone 'utc'), 1642347367, 2);
UPDATE current_nodes SET latitude=185315146, longitude=-724779719, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347367, version=2 WHERE id=573;
DELETE FROM current_node_tags WHERE node_id = 573;
DELETE FROM node_tags WHERE node_id = 573;
INSERT INTO current_node_tags (node_id, k, v) VALUES (573, 'hoot:id', '573');
INSERT INTO node_tags (node_id, k, v, version) VALUES (573, 'hoot:id', '573', 2);
/* modify node 574*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (574, 185316389, -724779434, 3, true, (now() at time zone 'utc'), 1642347367, 2);
UPDATE current_nodes SET latitude=185316389, longitude=-724779434, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347367, version=2 WHERE id=574;
DELETE FROM current_node_tags WHERE node_id = 574;
DELETE FROM node_tags WHERE node_id = 574;
INSERT INTO current_node_tags (node_id, k, v) VALUES (574, 'hoot:id', '574');
INSERT INTO node_tags (node_id, k, v, version) VALUES (574, 'hoot:id', '574', 2);
/* modify node 575*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (575, 185317604, -724778721, 3, true, (now() at time zone 'utc'), 1642347373, 2);
UPDATE current_nodes SET latitude=185317604, longitude=-724778721, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347373, version=2 WHERE id=575;
DELETE FROM current_node_tags WHERE node_id = 575;
DELETE FROM node_tags WHERE node_id = 575;
INSERT INTO current_node_tags (node_id, k, v) VALUES (575, 'hoot:id', '575');
INSERT INTO node_tags (node_id, k, v, version) VALUES (575, 'hoot:id', '575', 2);
/* modify node 576*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (576, 185318442, -724778493, 3, true, (now() at time zone 'utc'), 1642347373, 2);
UPDATE current_nodes SET latitude=185318442, longitude=-724778493, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347373, version=2 WHERE id=576;
DELETE FROM current_node_tags WHERE node_id = 576;
DELETE FROM node_tags WHERE node_id = 576;
INSERT INTO current_node_tags (node_id, k, v) VALUES (576, 'hoot:id', '576');
INSERT INTO node_tags (node_id, k, v, version) VALUES (576, 'hoot:id', '576', 2);
/* modify node 577*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (577, 185319262, -724777297, 3, true, (now() at time zone 'utc'), 1642347373, 2);
UPDATE current_nodes SET latitude=185319262, longitude=-724777297, changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), tile=1642347373, version=2 WHERE id=577;
DELETE FROM current_node_tags WHERE node_id = 577;
DELETE FROM node_tags WHERE node_id = 577;
INSERT INTO current_node_tags (node_id, k, v) VALUES (577, 'hoot:id', '577');
INSERT INTO node_tags (node_id, k, v, version) VALUES (577, 'hoot:id', '577', 2);
/* delete node 578*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (578, 185319198, -724776214, 3, false, (now() at time zone 'utc'), 1642347373, 2);
DELETE FROM current_node_tags WHERE node_id = 578;
DELETE FROM node_tags WHERE node_id = 578;
DELETE FROM current_way_nodes WHERE node_id=578;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 578;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=578;
/* delete node 579*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (579, 185319982, -724775188, 3, false, (now() at time zone 'utc'), 1642347373, 2);
DELETE FROM current_node_tags WHERE node_id = 579;
DELETE FROM node_tags WHERE node_id = 579;
DELETE FROM current_way_nodes WHERE node_id=579;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 579;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=579;
/* delete node 580*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (580, 185322224, -724774105, 3, false, (now() at time zone 'utc'), 1642347373, 2);
DELETE FROM current_node_tags WHERE node_id = 580;
DELETE FROM node_tags WHERE node_id = 580;
DELETE FROM current_way_nodes WHERE node_id=580;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 580;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=580;
/* delete node 581*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (581, 185323818, -724774191, 3, false, (now() at time zone 'utc'), 1642347373, 2);
DELETE FROM current_node_tags WHERE node_id = 581;
DELETE FROM node_tags WHERE node_id = 581;
DELETE FROM current_way_nodes WHERE node_id=581;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 581;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=581;
/* delete node 582*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (582, 185324871, -724773104, 3, false, (now() at time zone 'utc'), 1642347373, 2);
DELETE FROM current_node_tags WHERE node_id = 582;
DELETE FROM node_tags WHERE node_id = 582;
DELETE FROM current_way_nodes WHERE node_id=582;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 582;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=582;
/* delete node 583*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (583, 185325597, -724773998, 3, false, (now() at time zone 'utc'), 1642347373, 2);
DELETE FROM current_node_tags WHERE node_id = 583;
DELETE FROM node_tags WHERE node_id = 583;
DELETE FROM current_way_nodes WHERE node_id=583;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 583;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=583;
/* delete node 584*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (584, 185327517, -724772302, 3, false, (now() at time zone 'utc'), 1642347373, 2);
DELETE FROM current_node_tags WHERE node_id = 584;
DELETE FROM node_tags WHERE node_id = 584;
DELETE FROM current_way_nodes WHERE node_id=584;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 584;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=584;
/* delete node 585*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (585, 185328843, -724769620, 3, false, (now() at time zone 'utc'), 1642347373, 2);
DELETE FROM current_node_tags WHERE node_id = 585;
DELETE FROM node_tags WHERE node_id = 585;
DELETE FROM current_way_nodes WHERE node_id=585;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 585;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=585;
/* delete node 586*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (586, 185329194, -724768199, 3, false, (now() at time zone 'utc'), 1642347373, 2);
DELETE FROM current_node_tags WHERE node_id = 586;
DELETE FROM node_tags WHERE node_id = 586;
DELETE FROM current_way_nodes WHERE node_id=586;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 586;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=586;
/* delete node 608*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (608, 185329644, -724766703, 3, false, (now() at time zone 'utc'), 1642347373, 2);
DELETE FROM current_node_tags WHERE node_id = 608;
DELETE FROM node_tags WHERE node_id = 608;
DELETE FROM current_way_nodes WHERE node_id=608;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 608;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=608;
/* delete node 609*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (609, 185330439, -724766047, 3, false, (now() at time zone 'utc'), 1642347373, 2);
DELETE FROM current_node_tags WHERE node_id = 609;
DELETE FROM node_tags WHERE node_id = 609;
DELETE FROM current_way_nodes WHERE node_id=609;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 609;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=609;
/* delete node 610*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (610, 185331822, -724764387, 3, false, (now() at time zone 'utc'), 1642347373, 2);
DELETE FROM current_node_tags WHERE node_id = 610;
DELETE FROM node_tags WHERE node_id = 610;
DELETE FROM current_way_nodes WHERE node_id=610;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 610;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=610;
/* delete node 611*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (611, 185334954, -724758876, 3, false, (now() at time zone 'utc'), 1642347373, 2);
DELETE FROM current_node_tags WHERE node_id = 611;
DELETE FROM node_tags WHERE node_id = 611;
DELETE FROM current_way_nodes WHERE node_id=611;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 611;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=611;
/* delete node 612*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (612, 185332669, -724763202, 3, false, (now() at time zone 'utc'), 1642347373, 2);
DELETE FROM current_node_tags WHERE node_id = 612;
DELETE FROM node_tags WHERE node_id = 612;
DELETE FROM current_way_nodes WHERE node_id=612;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 612;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=612;
/* delete node 613*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (613, 185333793, -724763110, 3, false, (now() at time zone 'utc'), 1642347373, 2);
DELETE FROM current_node_tags WHERE node_id = 613;
DELETE FROM node_tags WHERE node_id = 613;
DELETE FROM current_way_nodes WHERE node_id=613;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 613;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=613;
/* delete node 614*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (614, 185334796, -724761907, 3, false, (now() at time zone 'utc'), 1642347373, 2);
DELETE FROM current_node_tags WHERE node_id = 614;
DELETE FROM node_tags WHERE node_id = 614;
DELETE FROM current_way_nodes WHERE node_id=614;
DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = 614;
UPDATE current_nodes SET changeset_id=3, visible=false, version=2 WHERE id=614;
/* create node 3826*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3826, 185274010, -724805894, 3, true, (now() at time zone 'utc'), 1642347363, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3826, 185274010, -724805894, 3, true, (now() at time zone 'utc'), 1642347363, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (3826, 'hoot:id', '3826');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3826, 'hoot:id', '3826', 1);
/* create node 3963*/
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3963, 185274081, -724805939, 3, true, (now() at time zone 'utc'), 1642347363, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (3963, 185274081, -724805939, 3, true, (now() at time zone 'utc'), 1642347363, 1);
INSERT INTO current_node_tags (node_id, k, v) VALUES (3963, 'hoot:id', '-6');
INSERT INTO node_tags (node_id, k, v, version) VALUES (3963, 'hoot:id', '-6', 1);
/* modify way 268*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (268, 3, true, (now() at time zone 'utc'), 2);
UPDATE current_ways SET changeset_id=3, visible=true, "timestamp"=(now() at time zone 'utc'), version=2 WHERE id=268;
DELETE FROM current_way_tags WHERE way_id = 268;
DELETE FROM way_tags WHERE way_id = 268;
INSERT INTO current_way_tags (way_id, k, v) VALUES (268, 'source', 'geoeye');
INSERT INTO way_tags (way_id, k, v, version) VALUES (268, 'source', 'geoeye', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (268, 'source:horiz_accuracy_cat', 'accurate');
INSERT INTO way_tags (way_id, k, v, version) VALUES (268, 'source:horiz_accuracy_cat', 'accurate', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (268, 'status', 'functional');
INSERT INTO way_tags (way_id, k, v, version) VALUES (268, 'status', 'functional', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (268, 'highway:min_width', '3');
INSERT INTO way_tags (way_id, k, v, version) VALUES (268, 'highway:min_width', '3', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (268, 'source:date_time', '2010-01-13');
INSERT INTO way_tags (way_id, k, v, version) VALUES (268, 'source:date_time', '2010-01-13', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (268, 'lanes', '1');
INSERT INTO way_tags (way_id, k, v, version) VALUES (268, 'lanes', '1', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (268, 'seasonal', 'no');
INSERT INTO way_tags (way_id, k, v, version) VALUES (268, 'seasonal', 'no', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (268, 'source:vertical_source_category', 'no_elevations');
INSERT INTO way_tags (way_id, k, v, version) VALUES (268, 'source:vertical_source_category', 'no_elevations', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (268, 'source:name', 'very_high_resolution_commercial_monoscopic_imagery');
INSERT INTO way_tags (way_id, k, v, version) VALUES (268, 'source:name', 'very_high_resolution_commercial_monoscopic_imagery', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (268, 'surface', 'unpaved');
INSERT INTO way_tags (way_id, k, v, version) VALUES (268, 'surface', 'unpaved', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (268, 'source:abs_horiz_accuracy', '25');
INSERT INTO way_tags (way_id, k, v, version) VALUES (268, 'source:abs_horiz_accuracy', '25', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (268, 'median', 'no');
INSERT INTO way_tags (way_id, k, v, version) VALUES (268, 'median', 'no', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (268, 'source:copyright', 'Â© 2010. Her Majesty the Queen in Right of Canada.');
INSERT INTO way_tags (way_id, k, v, version) VALUES (268, 'source:copyright', 'Â© 2010. Her Majesty the Queen in Right of Canada.', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (268, 'source:abs_vert_accuracy_eval', 'NA');
INSERT INTO way_tags (way_id, k, v, version) VALUES (268, 'source:abs_vert_accuracy_eval', 'NA', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (268, 'source:description', 'IKONOS Imagery');
INSERT INTO way_tags (way_id, k, v, version) VALUES (268, 'source:description', 'IKONOS Imagery', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (268, 'source:review_source_type', 'NA');
INSERT INTO way_tags (way_id, k, v, version) VALUES (268, 'source:review_source_type', 'NA', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (268, 'REF2', 'todo');
INSERT INTO way_tags (way_id, k, v, version) VALUES (268, 'REF2', 'todo', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (268, 'highway', 'path');
INSERT INTO way_tags (way_id, k, v, version) VALUES (268, 'highway', 'path', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (268, 'source:datetime', '2010-01-18T18:46:23.000Z;2016-09-29T18:41:47Z');
INSERT INTO way_tags (way_id, k, v, version) VALUES (268, 'source:datetime', '2010-01-18T18:46:23.000Z;2016-09-29T18:41:47Z', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (268, 'source:abs_horiz_accuracy_eval', 'emc_product_specification');
INSERT INTO way_tags (way_id, k, v, version) VALUES (268, 'source:abs_horiz_accuracy_eval', 'emc_product_specification', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (268, 'location', 'surface');
INSERT INTO way_tags (way_id, k, v, version) VALUES (268, 'location', 'surface', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (268, 'source:abs_vert_accuracy', '-32765');
INSERT INTO way_tags (way_id, k, v, version) VALUES (268, 'source:abs_vert_accuracy', '-32765', 2);
INSERT INTO current_way_tags (way_id, k, v) VALUES (268, 'hoot:id', '268');
INSERT INTO way_tags (way_id, k, v, version) VALUES (268, 'hoot:id', '268', 2);
DELETE FROM current_way_nodes WHERE way_id = 268;
DELETE FROM way_nodes WHERE way_id = 268;
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (268, 3963, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (268, 3963, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (268, 507, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (268, 507, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (268, 510, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (268, 510, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (268, 511, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (268, 511, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (268, 513, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (268, 513, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (268, 514, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (268, 514, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (268, 516, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (268, 516, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (268, 518, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (268, 518, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (268, 519, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (268, 519, 9);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (268, 540, 1, 10);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (268, 540, 10);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (268, 541, 1, 11);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (268, 541, 11);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (268, 542, 1, 12);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (268, 542, 12);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (268, 543, 1, 13);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (268, 543, 13);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (268, 544, 1, 14);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (268, 544, 14);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (268, 545, 1, 15);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (268, 545, 15);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (268, 546, 1, 16);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (268, 546, 16);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (268, 547, 1, 17);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (268, 547, 17);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (268, 548, 1, 18);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (268, 548, 18);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (268, 549, 1, 19);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (268, 549, 19);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (268, 550, 1, 20);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (268, 550, 20);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (268, 551, 1, 21);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (268, 551, 21);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (268, 552, 1, 22);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (268, 552, 22);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (268, 553, 1, 23);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (268, 553, 23);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (268, 554, 1, 24);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (268, 554, 24);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (268, 555, 1, 25);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (268, 555, 25);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (268, 556, 1, 26);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (268, 556, 26);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (268, 557, 1, 27);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (268, 557, 27);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (268, 558, 1, 28);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (268, 558, 28);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (268, 559, 1, 29);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (268, 559, 29);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (268, 560, 1, 30);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (268, 560, 30);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (268, 561, 1, 31);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (268, 561, 31);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (268, 562, 1, 32);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (268, 562, 32);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (268, 563, 1, 33);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (268, 563, 33);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (268, 564, 1, 34);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (268, 564, 34);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (268, 565, 1, 35);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (268, 565, 35);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (268, 566, 1, 36);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (268, 566, 36);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (268, 567, 1, 37);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (268, 567, 37);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (268, 568, 1, 38);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (268, 568, 38);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (268, 569, 1, 39);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (268, 569, 39);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (268, 570, 1, 40);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (268, 570, 40);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (268, 571, 1, 41);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (268, 571, 41);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (268, 572, 1, 42);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (268, 572, 42);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (268, 573, 1, 43);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (268, 573, 43);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (268, 574, 1, 44);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (268, 574, 44);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (268, 575, 1, 45);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (268, 575, 45);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (268, 576, 1, 46);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (268, 576, 46);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (268, 577, 1, 47);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (268, 577, 47);
/* create way 383*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (383, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (383, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (383, 'source:horiz_accuracy_cat', 'accurate');
INSERT INTO way_tags (way_id, k, v, version) VALUES (383, 'source:horiz_accuracy_cat', 'accurate', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (383, 'status', 'functional');
INSERT INTO way_tags (way_id, k, v, version) VALUES (383, 'status', 'functional', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (383, 'highway:min_width', '3');
INSERT INTO way_tags (way_id, k, v, version) VALUES (383, 'highway:min_width', '3', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (383, 'source:date_time', '2010-01-13');
INSERT INTO way_tags (way_id, k, v, version) VALUES (383, 'source:date_time', '2010-01-13', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (383, 'lanes', '1');
INSERT INTO way_tags (way_id, k, v, version) VALUES (383, 'lanes', '1', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (383, 'seasonal', 'no');
INSERT INTO way_tags (way_id, k, v, version) VALUES (383, 'seasonal', 'no', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (383, 'source:vertical_source_category', 'no_elevations');
INSERT INTO way_tags (way_id, k, v, version) VALUES (383, 'source:vertical_source_category', 'no_elevations', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (383, 'source:name', 'very_high_resolution_commercial_monoscopic_imagery');
INSERT INTO way_tags (way_id, k, v, version) VALUES (383, 'source:name', 'very_high_resolution_commercial_monoscopic_imagery', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (383, 'surface', 'unpaved');
INSERT INTO way_tags (way_id, k, v, version) VALUES (383, 'surface', 'unpaved', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (383, 'source:abs_horiz_accuracy', '25');
INSERT INTO way_tags (way_id, k, v, version) VALUES (383, 'source:abs_horiz_accuracy', '25', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (383, 'median', 'no');
INSERT INTO way_tags (way_id, k, v, version) VALUES (383, 'median', 'no', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (383, 'source:copyright', 'Â© 2010. Her Majesty the Queen in Right of Canada.');
INSERT INTO way_tags (way_id, k, v, version) VALUES (383, 'source:copyright', 'Â© 2010. Her Majesty the Queen in Right of Canada.', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (383, 'source:abs_vert_accuracy_eval', 'NA');
INSERT INTO way_tags (way_id, k, v, version) VALUES (383, 'source:abs_vert_accuracy_eval', 'NA', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (383, 'source:description', 'IKONOS Imagery');
INSERT INTO way_tags (way_id, k, v, version) VALUES (383, 'source:description', 'IKONOS Imagery', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (383, 'source:review_source_type', 'NA');
INSERT INTO way_tags (way_id, k, v, version) VALUES (383, 'source:review_source_type', 'NA', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (383, 'REF2', 'todo');
INSERT INTO way_tags (way_id, k, v, version) VALUES (383, 'REF2', 'todo', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (383, 'highway', 'road');
INSERT INTO way_tags (way_id, k, v, version) VALUES (383, 'highway', 'road', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (383, 'source:datetime', '2016-09-29T18:41:47Z');
INSERT INTO way_tags (way_id, k, v, version) VALUES (383, 'source:datetime', '2016-09-29T18:41:47Z', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (383, 'source:abs_horiz_accuracy_eval', 'emc_product_specification');
INSERT INTO way_tags (way_id, k, v, version) VALUES (383, 'source:abs_horiz_accuracy_eval', 'emc_product_specification', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (383, 'location', 'surface');
INSERT INTO way_tags (way_id, k, v, version) VALUES (383, 'location', 'surface', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (383, 'source:abs_vert_accuracy', '-32765');
INSERT INTO way_tags (way_id, k, v, version) VALUES (383, 'source:abs_vert_accuracy', '-32765', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (383, 'hoot:id', '-14');
INSERT INTO way_tags (way_id, k, v, version) VALUES (383, 'hoot:id', '-14', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (383, 3963, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (383, 3963, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (383, 3826, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (383, 3826, 2);
/* create way 386*/
INSERT INTO ways (way_id, changeset_id, visible, "timestamp", version) VALUES (386, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_ways (id, changeset_id, visible, "timestamp", version) VALUES (386, 3, true, (now() at time zone 'utc'), 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (386, 'hoot:id', '-10');
INSERT INTO way_tags (way_id, k, v, version) VALUES (386, 'hoot:id', '-10', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (386, 'source', 'geoeye');
INSERT INTO way_tags (way_id, k, v, version) VALUES (386, 'source', 'geoeye', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (386, 'highway', 'path');
INSERT INTO way_tags (way_id, k, v, version) VALUES (386, 'highway', 'path', 1);
INSERT INTO current_way_tags (way_id, k, v) VALUES (386, 'source:datetime', '2010-01-18T18:46:23.000Z');
INSERT INTO way_tags (way_id, k, v, version) VALUES (386, 'source:datetime', '2010-01-18T18:46:23.000Z', 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (386, 445, 1, 1);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (386, 445, 1);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (386, 452, 1, 2);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (386, 452, 2);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (386, 466, 1, 3);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (386, 466, 3);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (386, 467, 1, 4);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (386, 467, 4);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (386, 469, 1, 5);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (386, 469, 5);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (386, 470, 1, 6);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (386, 470, 6);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (386, 472, 1, 7);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (386, 472, 7);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (386, 474, 1, 8);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (386, 474, 8);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (386, 475, 1, 9);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (386, 475, 9);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (386, 477, 1, 10);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (386, 477, 10);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (386, 478, 1, 11);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (386, 478, 11);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (386, 480, 1, 12);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (386, 480, 12);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (386, 482, 1, 13);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (386, 482, 13);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (386, 483, 1, 14);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (386, 483, 14);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (386, 485, 1, 15);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (386, 485, 15);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (386, 487, 1, 16);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (386, 487, 16);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (386, 439, 1, 17);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (386, 439, 17);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (386, 521, 1, 18);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (386, 521, 18);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (386, 489, 1, 19);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (386, 489, 19);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (386, 490, 1, 20);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (386, 490, 20);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (386, 493, 1, 21);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (386, 493, 21);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (386, 494, 1, 22);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (386, 494, 22);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (386, 496, 1, 23);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (386, 496, 23);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (386, 497, 1, 24);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (386, 497, 24);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (386, 499, 1, 25);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (386, 499, 25);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (386, 501, 1, 26);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (386, 501, 26);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (386, 503, 1, 27);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (386, 503, 27);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (386, 504, 1, 28);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (386, 504, 28);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (386, 506, 1, 29);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (386, 506, 29);
INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES (386, 3963, 1, 30);
INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES (386, 3963, 30);
UPDATE changesets SET min_lat=185241214, max_lat=185334954, min_lon=-724807678, max_lon=-724758876, num_changes=96 WHERE id=3;
