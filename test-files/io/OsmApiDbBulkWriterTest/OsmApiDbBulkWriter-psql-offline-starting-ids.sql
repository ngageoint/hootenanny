BEGIN TRANSACTION;

SELECT pg_catalog.setval('changesets_id_seq', 4);
SELECT pg_catalog.setval('current_nodes_id_seq', 16);
SELECT pg_catalog.setval('current_ways_id_seq', 8);
SELECT pg_catalog.setval('current_relations_id_seq', 5);



COPY changesets (id, user_id, created_at, min_lat, max_lat, min_lon, max_lon, closed_at, num_changes) FROM stdin;
1	1	2017-03-27 19:05:21.698	0	-9999999	0	-9999999	2017-03-27 19:05:21.698	5
2	1	2017-03-27 19:05:21.699	0	-9999999	0	-9999999	2017-03-27 19:05:21.699	5
3	1	2017-03-27 19:05:21.699	0	-9999999	0	-9999999	2017-03-27 19:05:21.699	5
4	1	2017-03-27 19:05:21.700	0	-9999999	0	-9999999	2017-03-27 19:05:21.700	5
\.



COPY current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) FROM stdin;
3	380500000	-776000000	1	t	2017-03-27 19:05:21.698	1704239955	1
4	380100000	-776500000	1	t	2017-03-27 19:05:21.698	1704239316	1
5	380100000	-775500000	1	t	2017-03-27 19:05:21.698	1704239836	1
6	381000000	-776000000	1	t	2017-03-27 19:05:21.698	1704240727	1
7	380000000	-777000000	1	t	2017-03-27 19:05:21.698	1704233711	1
8	380000000	-775000000	2	t	2017-03-27 19:05:21.698	1704241231	1
9	381000000	-774000000	2	t	2017-03-27 19:05:21.699	1704242807	1
10	380000000	-774000000	2	t	2017-03-27 19:05:21.699	1704241767	1
11	381000000	-773000000	2	t	2017-03-27 19:05:21.699	1704330453	1
12	380000000	-773000000	2	t	2017-03-27 19:05:21.699	1704329413	1
13	381000000	-772000000	3	t	2017-03-27 19:05:21.699	1704330973	1
14	380000000	-772000000	3	t	2017-03-27 19:05:21.699	1704329933	1
15	380000000	-771000000	3	t	2017-03-27 19:05:21.699	1704331493	1
16	380000000	-770000000	3	t	2017-03-27 19:05:21.699	1704332013	1
\.



COPY current_node_tags (node_id, k, v) FROM stdin;
16	building	yes
16	name	n1
\.



COPY nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version, redaction_id) FROM stdin;
3	380500000	-776000000	1	t	2017-03-27 19:05:21.698	1704239955	1	\N
4	380100000	-776500000	1	t	2017-03-27 19:05:21.698	1704239316	1	\N
5	380100000	-775500000	1	t	2017-03-27 19:05:21.698	1704239836	1	\N
6	381000000	-776000000	1	t	2017-03-27 19:05:21.698	1704240727	1	\N
7	380000000	-777000000	1	t	2017-03-27 19:05:21.698	1704233711	1	\N
8	380000000	-775000000	2	t	2017-03-27 19:05:21.698	1704241231	1	\N
9	381000000	-774000000	2	t	2017-03-27 19:05:21.699	1704242807	1	\N
10	380000000	-774000000	2	t	2017-03-27 19:05:21.699	1704241767	1	\N
11	381000000	-773000000	2	t	2017-03-27 19:05:21.699	1704330453	1	\N
12	380000000	-773000000	2	t	2017-03-27 19:05:21.699	1704329413	1	\N
13	381000000	-772000000	3	t	2017-03-27 19:05:21.699	1704330973	1	\N
14	380000000	-772000000	3	t	2017-03-27 19:05:21.699	1704329933	1	\N
15	380000000	-771000000	3	t	2017-03-27 19:05:21.699	1704331493	1	\N
16	380000000	-770000000	3	t	2017-03-27 19:05:21.699	1704332013	1	\N
\.



COPY node_tags (node_id, version, k, v) FROM stdin;
16	1	building	yes
16	1	name	n1
\.



COPY current_ways (id, changeset_id, "timestamp", visible, version) FROM stdin;
4	3	2017-03-27 19:05:21.699	t	1
5	4	2017-03-27 19:05:21.699	t	1
6	4	2017-03-27 19:05:21.699	t	1
7	4	2017-03-27 19:05:21.699	t	1
8	4	2017-03-27 19:05:21.699	t	1
\.



COPY current_way_nodes (way_id, node_id, sequence_id) FROM stdin;
4	5	1
4	4	2
4	3	3
4	5	4
5	8	1
5	7	2
5	6	3
5	8	4
6	10	1
6	9	2
7	12	1
7	11	2
8	15	1
8	14	2
8	13	3
8	15	4
\.



COPY current_way_tags (way_id, k, v) FROM stdin;
6	name	w3
6	highway	road
7	name	w2
7	highway	track
8	building	yes
8	area	yes
8	name	w1
\.



COPY ways (way_id, changeset_id, "timestamp", version, visible, redaction_id) FROM stdin;
4	3	2017-03-27 19:05:21.699	1	t	\N
5	4	2017-03-27 19:05:21.699	1	t	\N
6	4	2017-03-27 19:05:21.699	1	t	\N
7	4	2017-03-27 19:05:21.699	1	t	\N
8	4	2017-03-27 19:05:21.699	1	t	\N
\.



COPY way_nodes (way_id, node_id, version, sequence_id) FROM stdin;
4	5	1	1
4	4	1	2
4	3	1	3
4	5	1	4
5	8	1	1
5	7	1	2
5	6	1	3
5	8	1	4
6	10	1	1
6	9	1	2
7	12	1	1
7	11	1	2
8	15	1	1
8	14	1	2
8	13	1	3
8	15	1	4
\.



COPY way_tags (way_id, k, v, version) FROM stdin;
6	name	w3	1
6	highway	road	1
7	name	w2	1
7	highway	track	1
8	building	yes	1
8	area	yes	1
8	name	w1	1
\.



COPY current_relations (id, changeset_id, "timestamp", visible, version) FROM stdin;
5	4	2017-03-27 19:05:21.700	t	1
\.



COPY current_relation_members (relation_id, member_type, member_id, member_role, sequence_id) FROM stdin;
5	Way	5	outer	1
5	Way	4	inner	2
\.



COPY current_relation_tags (relation_id, k, v) FROM stdin;
5	building	yes
5	name	r1
\.



COPY relations (relation_id, changeset_id, "timestamp", version, visible, redaction_id) FROM stdin;
5	4	2017-03-27 19:05:21.700	1	t	\N
\.



COPY relation_members (relation_id, member_type, member_id, member_role, version, sequence_id) FROM stdin;
5	Way	5	outer	1	1
5	Way	4	inner	1	2
\.



COPY relation_tags (relation_id, k, v, version) FROM stdin;
5	building	yes	1
5	name	r1	1
\.



COMMIT;