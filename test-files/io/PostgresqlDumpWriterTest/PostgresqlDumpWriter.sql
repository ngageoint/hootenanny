SELECT pg_catalog.setval('changesets_id_seq', 2);
SELECT pg_catalog.setval('current_nodes_id_seq', 15);
SELECT pg_catalog.setval('current_ways_id_seq', 6);
SELECT pg_catalog.setval('current_relations_id_seq', 2);


COPY changesets (id, user_id, created_at, closed_at, num_changes) FROM stdin;
1	1	2016-05-18 17:07:25.182	2016-05-18 17:07:25.182	20
\.


COPY current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) FROM stdin;
1	500000	6000000	1	t	2016-05-18 17:07:25.171	3221236134	1
2	100000	6500000	1	t	2016-05-18 17:07:25.171	3221236269	1
3	100000	5500000	1	t	2016-05-18 17:07:25.171	3221235749	1
4	1000000	6000000	1	t	2016-05-18 17:07:25.171	3221236914	1
5	0	7000000	1	t	2016-05-18 17:07:25.171	3221236394	1
6	0	5000000	1	t	2016-05-18 17:07:25.171	3221234314	1
7	1000000	4000000	1	t	2016-05-18 17:07:25.171	3221234832	1
8	0	4000000	1	t	2016-05-18 17:07:25.171	3221233792	1
9	1000000	3000000	1	t	2016-05-18 17:07:25.171	3221229112	1
10	0	3000000	1	t	2016-05-18 17:07:25.171	3221228072	1
11	1000000	2000000	1	t	2016-05-18 17:07:25.171	3221228592	1
12	0	2000000	1	t	2016-05-18 17:07:25.171	3221227552	1
13	0	1000000	1	t	2016-05-18 17:07:25.171	3221225992	1
14	0	0	1	t	2016-05-18 17:07:25.171	3221225472	1
\.


COPY current_node_tags (node_id, k, v) FROM stdin;
1	error:circular	10
2	error:circular	10
3	error:circular	10
4	error:circular	10
5	error:circular	10
6	error:circular	10
7	error:circular	10
8	error:circular	10
9	error:circular	10
10	error:circular	10
11	error:circular	10
12	error:circular	10
13	error:circular	10
14	error:circular	10
14	building	yes
14	name	n1
\.


COPY nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version, redaction_id) FROM stdin;
1	500000	6000000	1	t	2016-05-18 17:07:25.171	3221236134	1	\N
2	100000	6500000	1	t	2016-05-18 17:07:25.171	3221236269	1	\N
3	100000	5500000	1	t	2016-05-18 17:07:25.171	3221235749	1	\N
4	1000000	6000000	1	t	2016-05-18 17:07:25.171	3221236914	1	\N
5	0	7000000	1	t	2016-05-18 17:07:25.171	3221236394	1	\N
6	0	5000000	1	t	2016-05-18 17:07:25.171	3221234314	1	\N
7	1000000	4000000	1	t	2016-05-18 17:07:25.171	3221234832	1	\N
8	0	4000000	1	t	2016-05-18 17:07:25.171	3221233792	1	\N
9	1000000	3000000	1	t	2016-05-18 17:07:25.171	3221229112	1	\N
10	0	3000000	1	t	2016-05-18 17:07:25.171	3221228072	1	\N
11	1000000	2000000	1	t	2016-05-18 17:07:25.171	3221228592	1	\N
12	0	2000000	1	t	2016-05-18 17:07:25.171	3221227552	1	\N
13	0	1000000	1	t	2016-05-18 17:07:25.171	3221225992	1	\N
14	0	0	1	t	2016-05-18 17:07:25.171	3221225472	1	\N
\.


COPY node_tags (node_id, version, k, v) FROM stdin;
1	1	error:circular	10
2	1	error:circular	10
3	1	error:circular	10
4	1	error:circular	10
5	1	error:circular	10
6	1	error:circular	10
7	1	error:circular	10
8	1	error:circular	10
9	1	error:circular	10
10	1	error:circular	10
11	1	error:circular	10
12	1	error:circular	10
13	1	error:circular	10
14	1	error:circular	10
14	1	building	yes
14	1	name	n1
\.


COPY current_ways (id, changeset_id, "timestamp", visible, version) FROM stdin;
1	1	2016-05-18 17:07:25.171	t	1
2	1	2016-05-18 17:07:25.171	t	1
3	1	2016-05-18 17:07:25.172	t	1
4	1	2016-05-18 17:07:25.172	t	1
5	1	2016-05-18 17:07:25.172	t	1
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
1	error:circular	13
2	error:circular	13
3	error:circular	13
3	name	w3
3	highway	road
4	error:circular	13
4	name	w2
4	highway	track
5	error:circular	13
5	building	yes
5	area	yes
5	name	w1
\.


COPY ways (way_id, changeset_id, "timestamp", version, visible, redaction_id) FROM stdin;
1	1	2016-05-18 17:07:25.171	1	t	\N
2	1	2016-05-18 17:07:25.171	1	t	\N
3	1	2016-05-18 17:07:25.172	1	t	\N
4	1	2016-05-18 17:07:25.172	1	t	\N
5	1	2016-05-18 17:07:25.172	1	t	\N
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
1	1	error:circular	13
2	1	error:circular	13
3	1	error:circular	13
3	1	name	w3
3	1	highway	road
4	1	error:circular	13
4	1	name	w2
4	1	highway	track
5	1	error:circular	13
5	1	building	yes
5	1	area	yes
5	1	name	w1
\.


COPY current_relations (id, changeset_id, "timestamp", visible, version) FROM stdin;
1	1	2016-05-18 17:07:25.172	t	1
\.


COPY current_relation_members (relation_id, member_type, member_id, member_role, sequence_id) FROM stdin;
1	Way	2	outer	1
1	Way	1	inner	2
\.


COPY current_relation_tags (relation_id, k, v) FROM stdin;
1	error:circular	15
1	building	yes
1	name	r1
\.


COPY relations (relation_id, changeset_id, "timestamp", version, visible, redaction_id) FROM stdin;
1	1	2016-05-18 17:07:25.172	1	t	\N
\.


COPY relation_members (relation_id, member_type, member_id, member_role, version, sequence_id) FROM stdin;
1	Way	2	outer	1	1
1	Way	1	inner	1	2
\.


COPY relation_tags (relation_id, version, k, v) FROM stdin;
1	1	error:circular	15
1	1	building	yes
1	1	name	r1
\.


