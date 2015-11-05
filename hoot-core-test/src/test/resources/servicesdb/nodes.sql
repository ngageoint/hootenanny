--
-- PostgreSQL database dump
--

SET statement_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SET check_function_bodies = false;
SET client_min_messages = warning;

SET search_path = public, pg_catalog;

--
-- Data for Name: current_nodes; Type: TABLE DATA; Schema: public; Owner: hoot
--

COPY current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) FROM stdin;
1	384000000	-1065000000	1	t	2015-10-20 07:11:05.015861	1328832880	1
2	380000000	-1040000000	1	t	2015-10-20 07:13:23.128109	1329332431	1
\.


--
-- Data for Name: current_node_tags; Type: TABLE DATA; Schema: public; Owner: hoot
--

COPY current_node_tags (node_id, k, v) FROM stdin;
1	highway	road
1	accuracy1	5
2	foo	bar
\.


--
-- Name: current_nodes_id_seq; Type: SEQUENCE SET; Schema: public; Owner: hoot
--

SELECT pg_catalog.setval('current_nodes_id_seq', 2, true);


--
-- Data for Name: nodes; Type: TABLE DATA; Schema: public; Owner: hoot
--

COPY nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version, redaction_id) FROM stdin;
1	384000000	-1065000000	1	t	2015-10-20 07:11:05.015861	1328832880	1	\N
2	380000000	-1040000000	1	t	2015-10-20 07:13:23.128109	1329332431	1	\N
\.


--
-- Data for Name: node_tags; Type: TABLE DATA; Schema: public; Owner: hoot
--

COPY node_tags (node_id, version, k, v) FROM stdin;
1	1	highway	road
1	1	accuracy1	5
2	1	foo	bar
\.


--
-- PostgreSQL database dump complete
--


