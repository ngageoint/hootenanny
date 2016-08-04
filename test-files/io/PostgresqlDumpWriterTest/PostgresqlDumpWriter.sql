
SELECT pg_catalog.setval('changesets_id_seq', 2);
SELECT pg_catalog.setval('current_nodes_id_seq', 15);
SELECT pg_catalog.setval('current_ways_id_seq', 6);
SELECT pg_catalog.setval('current_relations_id_seq', 2);


COPY changesets (id, user_id, created_at, min_lat, max_lat, min_lon, max_lon, closed_at, num_changes) FROM stdin;
1	1	2016-08-02 18:38:46.625	380000000	381000000	-776999999	-769999999	2016-08-02 18:38:46.625	20
\.


COPY current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) FROM stdin;
1	380500000	-776000000	1	t	2016-08-02 18:38:46.623	1704239955	1
2	380100000	-776500000	1	t	2016-08-02 18:38:46.624	1704239316	1
3	380100000	-775500000	1	t	2016-08-02 18:38:46.624	1704239836	1
4	381000000	-776000000	1	t	2016-08-02 18:38:46.624	1704240727	1
5	380000000	-777000000	1	t	2016-08-02 18:38:46.624	1704233711	1
6	380000000	-775000000	1	t	2016-08-02 18:38:46.624	1704241231	1
7	381000000	-774000000	1	t	2016-08-02 18:38:46.624	1704242807	1
8	380000000	-774000000	1	t	2016-08-02 18:38:46.624	1704241767	1
9	381000000	-773000000	1	t	2016-08-02 18:38:46.624	1704330453	1
10	380000000	-773000000	1	t	2016-08-02 18:38:46.624	1704329413	1
11	381000000	-772000000	1	t	2016-08-02 18:38:46.624	1704330973	1
12	380000000	-772000000	1	t	2016-08-02 18:38:46.624	1704329933	1
13	380000000	-771000000	1	t	2016-08-02 18:38:46.624	1704331493	1
14	380000000	-770000000	1	t	2016-08-02 18:38:46.624	1704332013	1
\.


COPY current_node_tags (node_id, k, v) FROM stdin;
14	building	yes
14	name	n1
\.


COPY nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version, redaction_id) FROM stdin;
1	380500000	-776000000	1	t	2016-08-02 18:38:46.623	1704239955	1	\N
2	380100000	-776500000	1	t	2016-08-02 18:38:46.624	1704239316	1	\N
3	380100000	-775500000	1	t	2016-08-02 18:38:46.624	1704239836	1	\N
4	381000000	-776000000	1	t	2016-08-02 18:38:46.624	1704240727	1	\N
5	380000000	-777000000	1	t	2016-08-02 18:38:46.624	1704233711	1	\N
6	380000000	-775000000	1	t	2016-08-02 18:38:46.624	1704241231	1	\N
7	381000000	-774000000	1	t	2016-08-02 18:38:46.624	1704242807	1	\N
8	380000000	-774000000	1	t	2016-08-02 18:38:46.624	1704241767	1	\N
9	381000000	-773000000	1	t	2016-08-02 18:38:46.624	1704330453	1	\N
10	380000000	-773000000	1	t	2016-08-02 18:38:46.624	1704329413	1	\N
11	381000000	-772000000	1	t	2016-08-02 18:38:46.624	1704330973	1	\N
12	380000000	-772000000	1	t	2016-08-02 18:38:46.624	1704329933	1	\N
13	380000000	-771000000	1	t	2016-08-02 18:38:46.624	1704331493	1	\N
14	380000000	-770000000	1	t	2016-08-02 18:38:46.624	1704332013	1	\N
\.


COPY node_tags (node_id, version, k, v) FROM stdin;
14	1	building	yes
14	1	name	n1
\.


COPY current_ways (id, changeset_id, "timestamp", visible, version) FROM stdin;
1	1	2016-08-02 18:38:46.624	t	1
2	1	2016-08-02 18:38:46.625	t	1
3	1	2016-08-02 18:38:46.625	t	1
4	1	2016-08-02 18:38:46.625	t	1
5	1	2016-08-02 18:38:46.625	t	1
\.


COPY current_way_nodes (way_id, node_id, sequence_id) FROM stdin;
1	3	1
1	2	2
1	1	3
1	3	4
2	6	1
2	5	2
2	4	3
2	6	4
3	8	1
3	7	2
4	10	1
4	9	2
5	13	1
5	12	2
5	11	3
5	13	4
\.


COPY current_way_tags (way_id, k, v) FROM stdin;
3	name	w3
3	highway	road
4	name	w2
4	highway	track
5	building	yes
5	area	yes
5	name	w1
\.


COPY ways (way_id, changeset_id, "timestamp", version, visible, redaction_id) FROM stdin;
1	1	2016-08-02 18:38:46.624	1	t	\N
2	1	2016-08-02 18:38:46.625	1	t	\N
3	1	2016-08-02 18:38:46.625	1	t	\N
4	1	2016-08-02 18:38:46.625	1	t	\N
5	1	2016-08-02 18:38:46.625	1	t	\N
\.


COPY way_nodes (way_id, node_id, version, sequence_id) FROM stdin;
1	3	1	1
1	2	1	2
1	1	1	3
1	3	1	4
2	6	1	1
2	5	1	2
2	4	1	3
2	6	1	4
3	8	1	1
3	7	1	2
4	10	1	1
4	9	1	2
5	13	1	1
5	12	1	2
5	11	1	3
5	13	1	4
\.


COPY way_tags (way_id, version, k, v) FROM stdin;
3	1	name	w3
3	1	highway	road
4	1	name	w2
4	1	highway	track
5	1	building	yes
5	1	area	yes
5	1	name	w1
\.


COPY current_relations (id, changeset_id, "timestamp", visible, version) FROM stdin;
1	1	2016-08-02 18:38:46.625	t	1
\.


COPY current_relation_members (relation_id, member_type, member_id, member_role, sequence_id) FROM stdin;
1	Way	2	outer	1
1	Way	1	inner	2
\.


COPY current_relation_tags (relation_id, k, v) FROM stdin;
1	building	yes
1	name	r1
\.


COPY relations (relation_id, changeset_id, "timestamp", version, visible, redaction_id) FROM stdin;
1	1	2016-08-02 18:38:46.625	1	t	\N
\.


COPY relation_members (relation_id, member_type, member_id, member_role, version, sequence_id) FROM stdin;
1	Way	2	outer	1	1
1	Way	1	inner	1	2
\.


COPY relation_tags (relation_id, version, k, v) FROM stdin;
1	1	building	yes
1	1	name	r1
\.


