BEGIN TRANSACTION;

SELECT pg_catalog.setval('changesets_id_seq', 4);
SELECT pg_catalog.setval('current_nodes_id_seq', 14);
SELECT pg_catalog.setval('current_ways_id_seq', 5);
SELECT pg_catalog.setval('current_relations_id_seq', 1);



COPY changesets (id, user_id, created_at, min_lat, max_lat, min_lon, max_lon, closed_at, num_changes) FROM stdin;
1	1	2017-04-03 20:35:06.656	0	-9999999	0	-9999999	2017-04-03 20:35:06.656	5
2	1	2017-04-03 20:35:06.660	0	-9999999	0	-9999999	2017-04-03 20:35:06.660	5
3	1	2017-04-03 20:35:06.665	0	-9999999	0	-9999999	2017-04-03 20:35:06.665	5
4	1	2017-04-03 20:35:06.672	0	-9999999	0	-9999999	2017-04-03 20:35:06.672	5
\.



COPY current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) FROM stdin;
14	380500000	-776000000	1	t	2017-04-03 20:35:06.652	1704239955	1
13	380100000	-776500000	1	t	2017-04-03 20:35:06.653	1704239316	1
12	380100000	-775500000	1	t	2017-04-03 20:35:06.653	1704239836	1
11	381000000	-776000000	1	t	2017-04-03 20:35:06.655	1704240727	1
10	380000000	-777000000	1	t	2017-04-03 20:35:06.655	1704233711	1
9	380000000	-775000000	2	t	2017-04-03 20:35:06.656	1704241231	1
8	381000000	-774000000	2	t	2017-04-03 20:35:06.657	1704242807	1
7	380000000	-774000000	2	t	2017-04-03 20:35:06.658	1704241767	1
6	381000000	-773000000	2	t	2017-04-03 20:35:06.658	1704330453	1
5	380000000	-773000000	2	t	2017-04-03 20:35:06.659	1704329413	1
4	381000000	-772000000	3	t	2017-04-03 20:35:06.660	1704330973	1
3	380000000	-772000000	3	t	2017-04-03 20:35:06.660	1704329933	1
2	380000000	-771000000	3	t	2017-04-03 20:35:06.662	1704331493	1
1	380000000	-770000000	3	t	2017-04-03 20:35:06.662	1704332013	1
\.



COPY current_node_tags (node_id, k, v) FROM stdin;
1	building	yes
1	name	n1
\.



COPY nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version, redaction_id) FROM stdin;
14	380500000	-776000000	1	t	2017-04-03 20:35:06.652	1704239955	1	\N
13	380100000	-776500000	1	t	2017-04-03 20:35:06.653	1704239316	1	\N
12	380100000	-775500000	1	t	2017-04-03 20:35:06.653	1704239836	1	\N
11	381000000	-776000000	1	t	2017-04-03 20:35:06.655	1704240727	1	\N
10	380000000	-777000000	1	t	2017-04-03 20:35:06.655	1704233711	1	\N
9	380000000	-775000000	2	t	2017-04-03 20:35:06.656	1704241231	1	\N
8	381000000	-774000000	2	t	2017-04-03 20:35:06.657	1704242807	1	\N
7	380000000	-774000000	2	t	2017-04-03 20:35:06.658	1704241767	1	\N
6	381000000	-773000000	2	t	2017-04-03 20:35:06.658	1704330453	1	\N
5	380000000	-773000000	2	t	2017-04-03 20:35:06.659	1704329413	1	\N
4	381000000	-772000000	3	t	2017-04-03 20:35:06.660	1704330973	1	\N
3	380000000	-772000000	3	t	2017-04-03 20:35:06.660	1704329933	1	\N
2	380000000	-771000000	3	t	2017-04-03 20:35:06.662	1704331493	1	\N
1	380000000	-770000000	3	t	2017-04-03 20:35:06.662	1704332013	1	\N
\.



COPY node_tags (node_id, version, k, v) FROM stdin;
1	1	building	yes
1	1	name	n1
\.



COPY current_ways (id, changeset_id, "timestamp", visible, version) FROM stdin;
5	3	2017-04-03 20:35:06.664	t	1
4	4	2017-04-03 20:35:06.665	t	1
3	4	2017-04-03 20:35:06.666	t	1
2	4	2017-04-03 20:35:06.668	t	1
1	4	2017-04-03 20:35:06.669	t	1
\.



COPY current_way_nodes (way_id, node_id, sequence_id) FROM stdin;
5	12	1
5	13	2
5	14	3
5	12	4
4	9	1
4	10	2
4	11	3
4	9	4
3	7	1
3	8	2
2	5	1
2	6	2
1	2	1
1	3	2
1	4	3
1	2	4
\.



COPY current_way_tags (way_id, k, v) FROM stdin;
3	name	w3
3	highway	road
2	name	w2
2	highway	track
1	building	yes
1	area	yes
1	name	w1
\.



COPY ways (way_id, changeset_id, "timestamp", version, visible, redaction_id) FROM stdin;
5	3	2017-04-03 20:35:06.664	1	t	\N
4	4	2017-04-03 20:35:06.665	1	t	\N
3	4	2017-04-03 20:35:06.666	1	t	\N
2	4	2017-04-03 20:35:06.668	1	t	\N
1	4	2017-04-03 20:35:06.669	1	t	\N
\.



COPY way_nodes (way_id, node_id, version, sequence_id) FROM stdin;
5	12	1	1
5	13	1	2
5	14	1	3
5	12	1	4
4	9	1	1
4	10	1	2
4	11	1	3
4	9	1	4
3	7	1	1
3	8	1	2
2	5	1	1
2	6	1	2
1	2	1	1
1	3	1	2
1	4	1	3
1	2	1	4
\.



COPY way_tags (way_id, k, v, version) FROM stdin;
3	name	w3	1
3	highway	road	1
2	name	w2	1
2	highway	track	1
1	building	yes	1
1	area	yes	1
1	name	w1	1
\.



COPY current_relations (id, changeset_id, "timestamp", visible, version) FROM stdin;
1	4	2017-04-03 20:35:06.671	t	1
\.



COPY current_relation_members (relation_id, member_type, member_id, member_role, sequence_id) FROM stdin;
1	Way	4	outer	1
1	Way	5	inner	2
\.



COPY current_relation_tags (relation_id, k, v) FROM stdin;
1	building	yes
1	name	r1
\.



COPY relations (relation_id, changeset_id, "timestamp", version, visible, redaction_id) FROM stdin;
1	4	2017-04-03 20:35:06.671	1	t	\N
\.



COPY relation_members (relation_id, member_type, member_id, member_role, version, sequence_id) FROM stdin;
1	Way	4	outer	1	1
1	Way	5	inner	1	2
\.



COPY relation_tags (relation_id, k, v, version) FROM stdin;
1	building	yes	1
1	name	r1	1
\.



COMMIT;