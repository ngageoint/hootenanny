BEGIN TRANSACTION;


COPY changesets (id, user_id, created_at, min_lat, max_lat, min_lon, max_lon, closed_at, num_changes) FROM stdin;
2	1	2017-03-07 15:28:09.241	380000000	381000000	-776999999	-769999999	2017-03-07 15:28:09.241	20
\.



COPY current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) FROM stdin;
3	380500000	-776000000	2	t	2017-03-07 15:28:09.240	1704239955	1
4	380100000	-776500000	2	t	2017-03-07 15:28:09.240	1704239316	1
5	380100000	-775500000	2	t	2017-03-07 15:28:09.240	1704239836	1
6	381000000	-776000000	2	t	2017-03-07 15:28:09.240	1704240727	1
7	380000000	-777000000	2	t	2017-03-07 15:28:09.240	1704233711	1
8	380000000	-775000000	2	t	2017-03-07 15:28:09.240	1704241231	1
9	381000000	-774000000	2	t	2017-03-07 15:28:09.240	1704242807	1
10	380000000	-774000000	2	t	2017-03-07 15:28:09.240	1704241767	1
11	381000000	-773000000	2	t	2017-03-07 15:28:09.240	1704330453	1
12	380000000	-773000000	2	t	2017-03-07 15:28:09.240	1704329413	1
13	381000000	-772000000	2	t	2017-03-07 15:28:09.240	1704330973	1
14	380000000	-772000000	2	t	2017-03-07 15:28:09.240	1704329933	1
15	380000000	-771000000	2	t	2017-03-07 15:28:09.240	1704331493	1
16	380000000	-770000000	2	t	2017-03-07 15:28:09.240	1704332013	1
\.



COPY current_node_tags (node_id, k, v) FROM stdin;
16	building	yes
16	name	n1
\.



COPY nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version, redaction_id) FROM stdin;
3	380500000	-776000000	2	t	2017-03-07 15:28:09.240	1704239955	1	\N
4	380100000	-776500000	2	t	2017-03-07 15:28:09.240	1704239316	1	\N
5	380100000	-775500000	2	t	2017-03-07 15:28:09.240	1704239836	1	\N
6	381000000	-776000000	2	t	2017-03-07 15:28:09.240	1704240727	1	\N
7	380000000	-777000000	2	t	2017-03-07 15:28:09.240	1704233711	1	\N
8	380000000	-775000000	2	t	2017-03-07 15:28:09.240	1704241231	1	\N
9	381000000	-774000000	2	t	2017-03-07 15:28:09.240	1704242807	1	\N
10	380000000	-774000000	2	t	2017-03-07 15:28:09.240	1704241767	1	\N
11	381000000	-773000000	2	t	2017-03-07 15:28:09.240	1704330453	1	\N
12	380000000	-773000000	2	t	2017-03-07 15:28:09.240	1704329413	1	\N
13	381000000	-772000000	2	t	2017-03-07 15:28:09.240	1704330973	1	\N
14	380000000	-772000000	2	t	2017-03-07 15:28:09.240	1704329933	1	\N
15	380000000	-771000000	2	t	2017-03-07 15:28:09.240	1704331493	1	\N
16	380000000	-770000000	2	t	2017-03-07 15:28:09.240	1704332013	1	\N
\.



COPY node_tags (node_id, version, k, v) FROM stdin;
16	1	building	yes
16	1	name	n1
\.



COPY current_ways (id, changeset_id, "timestamp", visible, version) FROM stdin;
2	2	2017-03-07 15:28:09.240	t	1
3	2	2017-03-07 15:28:09.240	t	1
4	2	2017-03-07 15:28:09.240	t	1
5	2	2017-03-07 15:28:09.240	t	1
6	2	2017-03-07 15:28:09.240	t	1
\.



COPY current_way_nodes (way_id, node_id, sequence_id) FROM stdin;
2	5	1
2	4	2
2	3	3
2	5	4
3	8	1
3	7	2
3	6	3
3	8	4
4	10	1
4	9	2
5	12	1
5	11	2
6	15	1
6	14	2
6	13	3
6	15	4
\.



COPY current_way_tags (way_id, k, v) FROM stdin;
4	name	w3
4	highway	road
5	name	w2
5	highway	track
6	building	yes
6	area	yes
6	name	w1
\.



COPY ways (way_id, changeset_id, "timestamp", version, visible, redaction_id) FROM stdin;
2	2	2017-03-07 15:28:09.240	1	t	\N
3	2	2017-03-07 15:28:09.240	1	t	\N
4	2	2017-03-07 15:28:09.240	1	t	\N
5	2	2017-03-07 15:28:09.240	1	t	\N
6	2	2017-03-07 15:28:09.240	1	t	\N
\.



COPY way_nodes (way_id, node_id, version, sequence_id) FROM stdin;
2	5	1	1
2	4	1	2
2	3	1	3
2	5	1	4
3	8	1	1
3	7	1	2
3	6	1	3
3	8	1	4
4	10	1	1
4	9	1	2
5	12	1	1
5	11	1	2
6	15	1	1
6	14	1	2
6	13	1	3
6	15	1	4
\.



COPY way_tags (way_id, version, k, v) FROM stdin;
4	1	name	w3
4	1	highway	road
5	1	name	w2
5	1	highway	track
6	1	building	yes
6	1	area	yes
6	1	name	w1
\.



COPY current_relations (id, changeset_id, "timestamp", visible, version) FROM stdin;
2	2	2017-03-07 15:28:09.241	t	1
\.



COPY current_relation_members (relation_id, member_type, member_id, member_role, sequence_id) FROM stdin;
2	Way	3	outer	1
2	Way	2	inner	2
\.



COPY current_relation_tags (relation_id, k, v) FROM stdin;
2	building	yes
2	name	r1
\.



COPY relations (relation_id, changeset_id, "timestamp", version, visible, redaction_id) FROM stdin;
2	2	2017-03-07 15:28:09.241	1	t	\N
\.



COPY relation_members (relation_id, member_type, member_id, member_role, version, sequence_id) FROM stdin;
2	Way	3	outer	1	1
2	Way	2	inner	1	2
\.



COPY relation_tags (relation_id, version, k, v) FROM stdin;
2	1	building	yes
2	1	name	r1
\.



COMMIT;

