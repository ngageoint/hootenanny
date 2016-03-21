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
-- Data for Name: current_ways; Type: TABLE DATA; Schema: public; Owner: hoot
--

COPY current_ways (id, changeset_id, "timestamp", visible, version) FROM stdin;
1	1	2015-10-20 07:50:04.174847	t	1
\.


--
-- Data for Name: current_way_nodes; Type: TABLE DATA; Schema: public; Owner: hoot
--

COPY current_way_nodes (way_id, node_id, sequence_id) FROM stdin;
1	1	1
1	2	2
\.


--
-- Data for Name: current_way_tags; Type: TABLE DATA; Schema: public; Owner: hoot
--

COPY current_way_tags (way_id, k, v) FROM stdin;
1	highway	primary
\.


--
-- Name: current_ways_id_seq; Type: SEQUENCE SET; Schema: public; Owner: hoot
--

SELECT pg_catalog.setval('current_ways_id_seq', 1, true);


--
-- Data for Name: ways; Type: TABLE DATA; Schema: public; Owner: hoot
--

COPY ways (way_id, changeset_id, "timestamp", version, visible, redaction_id) FROM stdin;
1	1	2015-10-20 07:49:09.557115	1	t	\N
\.


--
-- Data for Name: way_nodes; Type: TABLE DATA; Schema: public; Owner: hoot
--

COPY way_nodes (way_id, node_id, version, sequence_id) FROM stdin;
1	1	1	1
1	2	1	2
\.


--
-- Data for Name: way_tags; Type: TABLE DATA; Schema: public; Owner: hoot
--

COPY way_tags (way_id, k, v, version) FROM stdin;
1	highway	primary	1
\.


--
-- PostgreSQL database dump complete
--

