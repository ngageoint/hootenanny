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
500	384000000	-1065000000	1	t	2015-10-20 07:11:05.015861	1328832880	1
\.


--
-- Data for Name: current_node_tags; Type: TABLE DATA; Schema: public; Owner: hoot
--

COPY current_node_tags (node_id, k, v) FROM stdin;
500	body_of_water	haitis river
500	accuracy1	3
\.


--
-- Name: current_nodes_id_seq; Type: SEQUENCE SET; Schema: public; Owner: hoot
--

SELECT pg_catalog.setval('current_nodes_id_seq', 1, true);


--
-- Data for Name: nodes; Type: TABLE DATA; Schema: public; Owner: hoot
--

COPY nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version, redaction_id) FROM stdin;
500	384000000	-1065000000	1	t	2015-10-20 07:11:05.015861	1328832880	1	\N
\.


--
-- Data for Name: node_tags; Type: TABLE DATA; Schema: public; Owner: hoot
--

COPY node_tags (node_id, version, k, v) FROM stdin;
500	1	accuracy1	3
500	1	body_of_water	haitis river
\.


--
-- PostgreSQL database dump complete
--

