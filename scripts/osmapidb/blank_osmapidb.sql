--
-- PostgreSQL database dump
--

SET statement_timeout = 0;
-- SET lock_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SET check_function_bodies = false;
SET client_min_messages = warning;

--
-- Name: plpgsql; Type: EXTENSION; Schema: -; Owner: 
--

CREATE EXTENSION IF NOT EXISTS plpgsql WITH SCHEMA pg_catalog;


--
-- Name: EXTENSION plpgsql; Type: COMMENT; Schema: -; Owner: 
--

COMMENT ON EXTENSION plpgsql IS 'PL/pgSQL procedural language';


--
-- Name: btree_gist; Type: EXTENSION; Schema: -; Owner: 
--

CREATE EXTENSION IF NOT EXISTS btree_gist WITH SCHEMA public;


--
-- Name: EXTENSION btree_gist; Type: COMMENT; Schema: -; Owner: 
--

COMMENT ON EXTENSION btree_gist IS 'support for indexing common datatypes in GiST';


SET search_path = public, pg_catalog;

--
-- Name: format_enum; Type: TYPE; Schema: public; Owner: hoot
--

CREATE TYPE format_enum AS ENUM (
    'html',
    'markdown',
    'text'
);


ALTER TYPE public.format_enum OWNER TO hoot;

--
-- Name: gpx_visibility_enum; Type: TYPE; Schema: public; Owner: hoot
--

CREATE TYPE gpx_visibility_enum AS ENUM (
    'private',
    'public',
    'trackable',
    'identifiable'
);


ALTER TYPE public.gpx_visibility_enum OWNER TO hoot;

--
-- Name: note_event_enum; Type: TYPE; Schema: public; Owner: hoot
--

CREATE TYPE note_event_enum AS ENUM (
    'opened',
    'closed',
    'reopened',
    'commented',
    'hidden'
);


ALTER TYPE public.note_event_enum OWNER TO hoot;

--
-- Name: note_status_enum; Type: TYPE; Schema: public; Owner: hoot
--

CREATE TYPE note_status_enum AS ENUM (
    'open',
    'closed',
    'hidden'
);


ALTER TYPE public.note_status_enum OWNER TO hoot;

--
-- Name: nwr_enum; Type: TYPE; Schema: public; Owner: hoot
--

CREATE TYPE nwr_enum AS ENUM (
    'Node',
    'Way',
    'Relation'
);


ALTER TYPE public.nwr_enum OWNER TO hoot;

--
-- Name: user_role_enum; Type: TYPE; Schema: public; Owner: hoot
--

CREATE TYPE user_role_enum AS ENUM (
    'administrator',
    'moderator'
);


ALTER TYPE public.user_role_enum OWNER TO hoot;

--
-- Name: user_status_enum; Type: TYPE; Schema: public; Owner: hoot
--

CREATE TYPE user_status_enum AS ENUM (
    'pending',
    'active',
    'confirmed',
    'suspended',
    'deleted'
);


ALTER TYPE public.user_status_enum OWNER TO hoot;

--
-- Name: maptile_for_point(bigint, bigint, integer); Type: FUNCTION; Schema: public; Owner: postgres
--

-- CREATE FUNCTION maptile_for_point(bigint, bigint, integer) RETURNS integer
--    LANGUAGE c STRICT
--    AS '/tmp/libpgosm.so', 'maptile_for_point';


-- ALTER FUNCTION public.maptile_for_point(bigint, bigint, integer) OWNER TO postgres;

--
-- Name: tile_for_point(integer, integer); Type: FUNCTION; Schema: public; Owner: postgres
--

-- CREATE FUNCTION tile_for_point(integer, integer) RETURNS bigint
--     LANGUAGE c STRICT
--    AS '/tmp/libpgosm.so', 'tile_for_point';


-- ALTER FUNCTION public.tile_for_point(integer, integer) OWNER TO postgres;

--
-- Name: xid_to_int4(xid); Type: FUNCTION; Schema: public; Owner: postgres
--

-- CREATE FUNCTION xid_to_int4(xid) RETURNS integer
--     LANGUAGE c STRICT
--    AS '/tmp/libpgosm.so', 'xid_to_int4';


-- ALTER FUNCTION public.xid_to_int4(xid) OWNER TO postgres;

SET default_tablespace = '';

SET default_with_oids = false;

--
-- Name: acls; Type: TABLE; Schema: public; Owner: hoot; Tablespace: 
--

CREATE TABLE acls (
    id integer NOT NULL,
    address inet,
    k character varying NOT NULL,
    v character varying,
    domain character varying
);


ALTER TABLE public.acls OWNER TO hoot;

--
-- Name: acls_id_seq; Type: SEQUENCE; Schema: public; Owner: hoot
--

CREATE SEQUENCE acls_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.acls_id_seq OWNER TO hoot;

--
-- Name: acls_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: hoot
--

ALTER SEQUENCE acls_id_seq OWNED BY acls.id;


--
-- Name: changeset_comments; Type: TABLE; Schema: public; Owner: hoot; Tablespace: 
--

CREATE TABLE changeset_comments (
    id integer NOT NULL,
    changeset_id bigint NOT NULL,
    author_id bigint NOT NULL,
    body text NOT NULL,
    created_at timestamp without time zone NOT NULL,
    visible boolean NOT NULL
);


ALTER TABLE public.changeset_comments OWNER TO hoot;

--
-- Name: changeset_comments_id_seq; Type: SEQUENCE; Schema: public; Owner: hoot
--

CREATE SEQUENCE changeset_comments_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.changeset_comments_id_seq OWNER TO hoot;

--
-- Name: changeset_comments_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: hoot
--

ALTER SEQUENCE changeset_comments_id_seq OWNED BY changeset_comments.id;


--
-- Name: changeset_tags; Type: TABLE; Schema: public; Owner: hoot; Tablespace: 
--

CREATE TABLE changeset_tags (
    changeset_id bigint NOT NULL,
    k character varying DEFAULT ''::character varying NOT NULL,
    v character varying DEFAULT ''::character varying NOT NULL
);


ALTER TABLE public.changeset_tags OWNER TO hoot;

--
-- Name: changesets; Type: TABLE; Schema: public; Owner: hoot; Tablespace: 
--

CREATE TABLE changesets (
    id bigint NOT NULL,
    user_id bigint NOT NULL,
    created_at timestamp without time zone NOT NULL,
    min_lat integer,
    max_lat integer,
    min_lon integer,
    max_lon integer,
    closed_at timestamp without time zone NOT NULL,
    num_changes integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.changesets OWNER TO hoot;

--
-- Name: changesets_id_seq; Type: SEQUENCE; Schema: public; Owner: hoot
--

CREATE SEQUENCE changesets_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.changesets_id_seq OWNER TO hoot;

--
-- Name: changesets_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: hoot
--

ALTER SEQUENCE changesets_id_seq OWNED BY changesets.id;


--
-- Name: changesets_subscribers; Type: TABLE; Schema: public; Owner: hoot; Tablespace: 
--

CREATE TABLE changesets_subscribers (
    subscriber_id bigint NOT NULL,
    changeset_id bigint NOT NULL
);


ALTER TABLE public.changesets_subscribers OWNER TO hoot;

--
-- Name: client_applications; Type: TABLE; Schema: public; Owner: hoot; Tablespace: 
--

CREATE TABLE client_applications (
    id integer NOT NULL,
    name character varying,
    url character varying,
    support_url character varying,
    callback_url character varying,
    key character varying(50),
    secret character varying(50),
    user_id integer,
    created_at timestamp without time zone,
    updated_at timestamp without time zone,
    allow_read_prefs boolean DEFAULT false NOT NULL,
    allow_write_prefs boolean DEFAULT false NOT NULL,
    allow_write_diary boolean DEFAULT false NOT NULL,
    allow_write_api boolean DEFAULT false NOT NULL,
    allow_read_gpx boolean DEFAULT false NOT NULL,
    allow_write_gpx boolean DEFAULT false NOT NULL,
    allow_write_notes boolean DEFAULT false NOT NULL
);


ALTER TABLE public.client_applications OWNER TO hoot;

--
-- Name: client_applications_id_seq; Type: SEQUENCE; Schema: public; Owner: hoot
--

CREATE SEQUENCE client_applications_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.client_applications_id_seq OWNER TO hoot;

--
-- Name: client_applications_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: hoot
--

ALTER SEQUENCE client_applications_id_seq OWNED BY client_applications.id;


--
-- Name: current_node_tags; Type: TABLE; Schema: public; Owner: hoot; Tablespace: 
--

CREATE TABLE current_node_tags (
    node_id bigint NOT NULL,
    k character varying DEFAULT ''::character varying NOT NULL,
    v character varying DEFAULT ''::character varying NOT NULL
);


ALTER TABLE public.current_node_tags OWNER TO hoot;

--
-- Name: current_nodes; Type: TABLE; Schema: public; Owner: hoot; Tablespace: 
--

CREATE TABLE current_nodes (
    id bigint NOT NULL,
    latitude integer NOT NULL,
    longitude integer NOT NULL,
    changeset_id bigint NOT NULL,
    visible boolean NOT NULL,
    "timestamp" timestamp without time zone NOT NULL,
    tile bigint NOT NULL,
    version bigint NOT NULL
);


ALTER TABLE public.current_nodes OWNER TO hoot;

--
-- Name: current_nodes_id_seq; Type: SEQUENCE; Schema: public; Owner: hoot
--

CREATE SEQUENCE current_nodes_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.current_nodes_id_seq OWNER TO hoot;

--
-- Name: current_nodes_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: hoot
--

ALTER SEQUENCE current_nodes_id_seq OWNED BY current_nodes.id;


--
-- Name: current_relation_members; Type: TABLE; Schema: public; Owner: hoot; Tablespace: 
--

CREATE TABLE current_relation_members (
    relation_id bigint NOT NULL,
    member_type nwr_enum NOT NULL,
    member_id bigint NOT NULL,
    member_role character varying NOT NULL,
    sequence_id integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.current_relation_members OWNER TO hoot;

--
-- Name: current_relation_tags; Type: TABLE; Schema: public; Owner: hoot; Tablespace: 
--

CREATE TABLE current_relation_tags (
    relation_id bigint NOT NULL,
    k character varying DEFAULT ''::character varying NOT NULL,
    v character varying DEFAULT ''::character varying NOT NULL
);


ALTER TABLE public.current_relation_tags OWNER TO hoot;

--
-- Name: current_relations; Type: TABLE; Schema: public; Owner: hoot; Tablespace: 
--

CREATE TABLE current_relations (
    id bigint NOT NULL,
    changeset_id bigint NOT NULL,
    "timestamp" timestamp without time zone NOT NULL,
    visible boolean NOT NULL,
    version bigint NOT NULL
);


ALTER TABLE public.current_relations OWNER TO hoot;

--
-- Name: current_relations_id_seq; Type: SEQUENCE; Schema: public; Owner: hoot
--

CREATE SEQUENCE current_relations_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.current_relations_id_seq OWNER TO hoot;

--
-- Name: current_relations_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: hoot
--

ALTER SEQUENCE current_relations_id_seq OWNED BY current_relations.id;


--
-- Name: current_way_nodes; Type: TABLE; Schema: public; Owner: hoot; Tablespace: 
--

CREATE TABLE current_way_nodes (
    way_id bigint NOT NULL,
    node_id bigint NOT NULL,
    sequence_id bigint NOT NULL
);


ALTER TABLE public.current_way_nodes OWNER TO hoot;

--
-- Name: current_way_tags; Type: TABLE; Schema: public; Owner: hoot; Tablespace: 
--

CREATE TABLE current_way_tags (
    way_id bigint NOT NULL,
    k character varying DEFAULT ''::character varying NOT NULL,
    v character varying DEFAULT ''::character varying NOT NULL
);


ALTER TABLE public.current_way_tags OWNER TO hoot;

--
-- Name: current_ways; Type: TABLE; Schema: public; Owner: hoot; Tablespace: 
--

CREATE TABLE current_ways (
    id bigint NOT NULL,
    changeset_id bigint NOT NULL,
    "timestamp" timestamp without time zone NOT NULL,
    visible boolean NOT NULL,
    version bigint NOT NULL
);


ALTER TABLE public.current_ways OWNER TO hoot;

--
-- Name: current_ways_id_seq; Type: SEQUENCE; Schema: public; Owner: hoot
--

CREATE SEQUENCE current_ways_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.current_ways_id_seq OWNER TO hoot;

--
-- Name: current_ways_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: hoot
--

ALTER SEQUENCE current_ways_id_seq OWNED BY current_ways.id;


--
-- Name: diary_comments; Type: TABLE; Schema: public; Owner: hoot; Tablespace: 
--

CREATE TABLE diary_comments (
    id bigint NOT NULL,
    diary_entry_id bigint NOT NULL,
    user_id bigint NOT NULL,
    body text NOT NULL,
    created_at timestamp without time zone NOT NULL,
    updated_at timestamp without time zone NOT NULL,
    visible boolean DEFAULT true NOT NULL,
    body_format format_enum DEFAULT 'markdown'::format_enum NOT NULL
);


ALTER TABLE public.diary_comments OWNER TO hoot;

--
-- Name: diary_comments_id_seq; Type: SEQUENCE; Schema: public; Owner: hoot
--

CREATE SEQUENCE diary_comments_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.diary_comments_id_seq OWNER TO hoot;

--
-- Name: diary_comments_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: hoot
--

ALTER SEQUENCE diary_comments_id_seq OWNED BY diary_comments.id;


--
-- Name: diary_entries; Type: TABLE; Schema: public; Owner: hoot; Tablespace: 
--

CREATE TABLE diary_entries (
    id bigint NOT NULL,
    user_id bigint NOT NULL,
    title character varying NOT NULL,
    body text NOT NULL,
    created_at timestamp without time zone NOT NULL,
    updated_at timestamp without time zone NOT NULL,
    latitude double precision,
    longitude double precision,
    language_code character varying DEFAULT 'en'::character varying NOT NULL,
    visible boolean DEFAULT true NOT NULL,
    body_format format_enum DEFAULT 'markdown'::format_enum NOT NULL
);


ALTER TABLE public.diary_entries OWNER TO hoot;

--
-- Name: diary_entries_id_seq; Type: SEQUENCE; Schema: public; Owner: hoot
--

CREATE SEQUENCE diary_entries_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.diary_entries_id_seq OWNER TO hoot;

--
-- Name: diary_entries_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: hoot
--

ALTER SEQUENCE diary_entries_id_seq OWNED BY diary_entries.id;


--
-- Name: friends; Type: TABLE; Schema: public; Owner: hoot; Tablespace: 
--

CREATE TABLE friends (
    id bigint NOT NULL,
    user_id bigint NOT NULL,
    friend_user_id bigint NOT NULL
);


ALTER TABLE public.friends OWNER TO hoot;

--
-- Name: friends_id_seq; Type: SEQUENCE; Schema: public; Owner: hoot
--

CREATE SEQUENCE friends_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.friends_id_seq OWNER TO hoot;

--
-- Name: friends_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: hoot
--

ALTER SEQUENCE friends_id_seq OWNED BY friends.id;


--
-- Name: gps_points; Type: TABLE; Schema: public; Owner: hoot; Tablespace: 
--

CREATE TABLE gps_points (
    altitude double precision,
    trackid integer NOT NULL,
    latitude integer NOT NULL,
    longitude integer NOT NULL,
    gpx_id bigint NOT NULL,
    "timestamp" timestamp without time zone,
    tile bigint
);


ALTER TABLE public.gps_points OWNER TO hoot;

--
-- Name: gpx_file_tags; Type: TABLE; Schema: public; Owner: hoot; Tablespace: 
--

CREATE TABLE gpx_file_tags (
    gpx_id bigint DEFAULT 0 NOT NULL,
    tag character varying NOT NULL,
    id bigint NOT NULL
);


ALTER TABLE public.gpx_file_tags OWNER TO hoot;

--
-- Name: gpx_file_tags_id_seq; Type: SEQUENCE; Schema: public; Owner: hoot
--

CREATE SEQUENCE gpx_file_tags_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.gpx_file_tags_id_seq OWNER TO hoot;

--
-- Name: gpx_file_tags_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: hoot
--

ALTER SEQUENCE gpx_file_tags_id_seq OWNED BY gpx_file_tags.id;


--
-- Name: gpx_files; Type: TABLE; Schema: public; Owner: hoot; Tablespace: 
--

CREATE TABLE gpx_files (
    id bigint NOT NULL,
    user_id bigint NOT NULL,
    visible boolean DEFAULT true NOT NULL,
    name character varying DEFAULT ''::character varying NOT NULL,
    size bigint,
    latitude double precision,
    longitude double precision,
    "timestamp" timestamp without time zone NOT NULL,
    description character varying DEFAULT ''::character varying NOT NULL,
    inserted boolean NOT NULL,
    visibility gpx_visibility_enum DEFAULT 'public'::gpx_visibility_enum NOT NULL
);


ALTER TABLE public.gpx_files OWNER TO hoot;

--
-- Name: gpx_files_id_seq; Type: SEQUENCE; Schema: public; Owner: hoot
--

CREATE SEQUENCE gpx_files_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.gpx_files_id_seq OWNER TO hoot;

--
-- Name: gpx_files_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: hoot
--

ALTER SEQUENCE gpx_files_id_seq OWNED BY gpx_files.id;


--
-- Name: languages; Type: TABLE; Schema: public; Owner: hoot; Tablespace: 
--

CREATE TABLE languages (
    code character varying NOT NULL,
    english_name character varying NOT NULL,
    native_name character varying
);


ALTER TABLE public.languages OWNER TO hoot;

--
-- Name: messages; Type: TABLE; Schema: public; Owner: hoot; Tablespace: 
--

CREATE TABLE messages (
    id bigint NOT NULL,
    from_user_id bigint NOT NULL,
    title character varying NOT NULL,
    body text NOT NULL,
    sent_on timestamp without time zone NOT NULL,
    message_read boolean DEFAULT false NOT NULL,
    to_user_id bigint NOT NULL,
    to_user_visible boolean DEFAULT true NOT NULL,
    from_user_visible boolean DEFAULT true NOT NULL,
    body_format format_enum DEFAULT 'markdown'::format_enum NOT NULL
);


ALTER TABLE public.messages OWNER TO hoot;

--
-- Name: messages_id_seq; Type: SEQUENCE; Schema: public; Owner: hoot
--

CREATE SEQUENCE messages_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.messages_id_seq OWNER TO hoot;

--
-- Name: messages_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: hoot
--

ALTER SEQUENCE messages_id_seq OWNED BY messages.id;


--
-- Name: node_tags; Type: TABLE; Schema: public; Owner: hoot; Tablespace: 
--

CREATE TABLE node_tags (
    node_id bigint NOT NULL,
    version bigint NOT NULL,
    k character varying DEFAULT ''::character varying NOT NULL,
    v character varying DEFAULT ''::character varying NOT NULL
);


ALTER TABLE public.node_tags OWNER TO hoot;

--
-- Name: nodes; Type: TABLE; Schema: public; Owner: hoot; Tablespace: 
--

CREATE TABLE nodes (
    node_id bigint NOT NULL,
    latitude integer NOT NULL,
    longitude integer NOT NULL,
    changeset_id bigint NOT NULL,
    visible boolean NOT NULL,
    "timestamp" timestamp without time zone NOT NULL,
    tile bigint NOT NULL,
    version bigint NOT NULL,
    redaction_id integer
);


ALTER TABLE public.nodes OWNER TO hoot;

--
-- Name: note_comments; Type: TABLE; Schema: public; Owner: hoot; Tablespace: 
--

CREATE TABLE note_comments (
    id bigint NOT NULL,
    note_id bigint NOT NULL,
    visible boolean NOT NULL,
    created_at timestamp without time zone NOT NULL,
    author_ip inet,
    author_id bigint,
    body text,
    event note_event_enum
);


ALTER TABLE public.note_comments OWNER TO hoot;

--
-- Name: note_comments_id_seq; Type: SEQUENCE; Schema: public; Owner: hoot
--

CREATE SEQUENCE note_comments_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.note_comments_id_seq OWNER TO hoot;

--
-- Name: note_comments_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: hoot
--

ALTER SEQUENCE note_comments_id_seq OWNED BY note_comments.id;


--
-- Name: notes; Type: TABLE; Schema: public; Owner: hoot; Tablespace: 
--

CREATE TABLE notes (
    id bigint NOT NULL,
    latitude integer NOT NULL,
    longitude integer NOT NULL,
    tile bigint NOT NULL,
    updated_at timestamp without time zone NOT NULL,
    created_at timestamp without time zone NOT NULL,
    status note_status_enum NOT NULL,
    closed_at timestamp without time zone
);


ALTER TABLE public.notes OWNER TO hoot;

--
-- Name: notes_id_seq; Type: SEQUENCE; Schema: public; Owner: hoot
--

CREATE SEQUENCE notes_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.notes_id_seq OWNER TO hoot;

--
-- Name: notes_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: hoot
--

ALTER SEQUENCE notes_id_seq OWNED BY notes.id;


--
-- Name: oauth_nonces; Type: TABLE; Schema: public; Owner: hoot; Tablespace: 
--

CREATE TABLE oauth_nonces (
    id integer NOT NULL,
    nonce character varying,
    "timestamp" integer,
    created_at timestamp without time zone,
    updated_at timestamp without time zone
);


ALTER TABLE public.oauth_nonces OWNER TO hoot;

--
-- Name: oauth_nonces_id_seq; Type: SEQUENCE; Schema: public; Owner: hoot
--

CREATE SEQUENCE oauth_nonces_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.oauth_nonces_id_seq OWNER TO hoot;

--
-- Name: oauth_nonces_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: hoot
--

ALTER SEQUENCE oauth_nonces_id_seq OWNED BY oauth_nonces.id;


--
-- Name: oauth_tokens; Type: TABLE; Schema: public; Owner: hoot; Tablespace: 
--

CREATE TABLE oauth_tokens (
    id integer NOT NULL,
    user_id integer,
    type character varying(20),
    client_application_id integer,
    token character varying(50),
    secret character varying(50),
    authorized_at timestamp without time zone,
    invalidated_at timestamp without time zone,
    created_at timestamp without time zone,
    updated_at timestamp without time zone,
    allow_read_prefs boolean DEFAULT false NOT NULL,
    allow_write_prefs boolean DEFAULT false NOT NULL,
    allow_write_diary boolean DEFAULT false NOT NULL,
    allow_write_api boolean DEFAULT false NOT NULL,
    allow_read_gpx boolean DEFAULT false NOT NULL,
    allow_write_gpx boolean DEFAULT false NOT NULL,
    callback_url character varying,
    verifier character varying(20),
    scope character varying,
    valid_to timestamp without time zone,
    allow_write_notes boolean DEFAULT false NOT NULL
);


ALTER TABLE public.oauth_tokens OWNER TO hoot;

--
-- Name: oauth_tokens_id_seq; Type: SEQUENCE; Schema: public; Owner: hoot
--

CREATE SEQUENCE oauth_tokens_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.oauth_tokens_id_seq OWNER TO hoot;

--
-- Name: oauth_tokens_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: hoot
--

ALTER SEQUENCE oauth_tokens_id_seq OWNED BY oauth_tokens.id;


--
-- Name: redactions; Type: TABLE; Schema: public; Owner: hoot; Tablespace: 
--

CREATE TABLE redactions (
    id integer NOT NULL,
    title character varying,
    description text,
    created_at timestamp without time zone,
    updated_at timestamp without time zone,
    user_id bigint NOT NULL,
    description_format format_enum DEFAULT 'markdown'::format_enum NOT NULL
);


ALTER TABLE public.redactions OWNER TO hoot;

--
-- Name: redactions_id_seq; Type: SEQUENCE; Schema: public; Owner: hoot
--

CREATE SEQUENCE redactions_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.redactions_id_seq OWNER TO hoot;

--
-- Name: redactions_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: hoot
--

ALTER SEQUENCE redactions_id_seq OWNED BY redactions.id;


--
-- Name: relation_members; Type: TABLE; Schema: public; Owner: hoot; Tablespace: 
--

CREATE TABLE relation_members (
    relation_id bigint DEFAULT 0 NOT NULL,
    member_type nwr_enum NOT NULL,
    member_id bigint NOT NULL,
    member_role character varying NOT NULL,
    version bigint DEFAULT 0 NOT NULL,
    sequence_id integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.relation_members OWNER TO hoot;

--
-- Name: relation_tags; Type: TABLE; Schema: public; Owner: hoot; Tablespace: 
--

CREATE TABLE relation_tags (
    relation_id bigint DEFAULT 0 NOT NULL,
    k character varying DEFAULT ''::character varying NOT NULL,
    v character varying DEFAULT ''::character varying NOT NULL,
    version bigint NOT NULL
);


ALTER TABLE public.relation_tags OWNER TO hoot;

--
-- Name: relations; Type: TABLE; Schema: public; Owner: hoot; Tablespace: 
--

CREATE TABLE relations (
    relation_id bigint DEFAULT 0 NOT NULL,
    changeset_id bigint NOT NULL,
    "timestamp" timestamp without time zone NOT NULL,
    version bigint NOT NULL,
    visible boolean DEFAULT true NOT NULL,
    redaction_id integer
);


ALTER TABLE public.relations OWNER TO hoot;

--
-- Name: schema_migrations; Type: TABLE; Schema: public; Owner: hoot; Tablespace: 
--

CREATE TABLE schema_migrations (
    version character varying NOT NULL
);


ALTER TABLE public.schema_migrations OWNER TO hoot;

--
-- Name: user_blocks; Type: TABLE; Schema: public; Owner: hoot; Tablespace: 
--

CREATE TABLE user_blocks (
    id integer NOT NULL,
    user_id bigint NOT NULL,
    creator_id bigint NOT NULL,
    reason text NOT NULL,
    ends_at timestamp without time zone NOT NULL,
    needs_view boolean DEFAULT false NOT NULL,
    revoker_id bigint,
    created_at timestamp without time zone,
    updated_at timestamp without time zone,
    reason_format format_enum DEFAULT 'markdown'::format_enum NOT NULL
);


ALTER TABLE public.user_blocks OWNER TO hoot;

--
-- Name: user_blocks_id_seq; Type: SEQUENCE; Schema: public; Owner: hoot
--

CREATE SEQUENCE user_blocks_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.user_blocks_id_seq OWNER TO hoot;

--
-- Name: user_blocks_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: hoot
--

ALTER SEQUENCE user_blocks_id_seq OWNED BY user_blocks.id;


--
-- Name: user_preferences; Type: TABLE; Schema: public; Owner: hoot; Tablespace: 
--

CREATE TABLE user_preferences (
    user_id bigint NOT NULL,
    k character varying NOT NULL,
    v character varying NOT NULL
);


ALTER TABLE public.user_preferences OWNER TO hoot;

--
-- Name: user_roles; Type: TABLE; Schema: public; Owner: hoot; Tablespace: 
--

CREATE TABLE user_roles (
    id integer NOT NULL,
    user_id bigint NOT NULL,
    role user_role_enum NOT NULL,
    created_at timestamp without time zone,
    updated_at timestamp without time zone,
    granter_id bigint NOT NULL
);


ALTER TABLE public.user_roles OWNER TO hoot;

--
-- Name: user_roles_id_seq; Type: SEQUENCE; Schema: public; Owner: hoot
--

CREATE SEQUENCE user_roles_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.user_roles_id_seq OWNER TO hoot;

--
-- Name: user_roles_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: hoot
--

ALTER SEQUENCE user_roles_id_seq OWNED BY user_roles.id;


--
-- Name: user_tokens; Type: TABLE; Schema: public; Owner: hoot; Tablespace: 
--

CREATE TABLE user_tokens (
    id bigint NOT NULL,
    user_id bigint NOT NULL,
    token character varying NOT NULL,
    expiry timestamp without time zone NOT NULL,
    referer text
);


ALTER TABLE public.user_tokens OWNER TO hoot;

--
-- Name: user_tokens_id_seq; Type: SEQUENCE; Schema: public; Owner: hoot
--

CREATE SEQUENCE user_tokens_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.user_tokens_id_seq OWNER TO hoot;

--
-- Name: user_tokens_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: hoot
--

ALTER SEQUENCE user_tokens_id_seq OWNED BY user_tokens.id;


--
-- Name: users; Type: TABLE; Schema: public; Owner: hoot; Tablespace: 
--

CREATE TABLE users (
    email character varying NOT NULL,
    id bigint NOT NULL,
    pass_crypt character varying NOT NULL,
    creation_time timestamp without time zone NOT NULL,
    display_name character varying DEFAULT ''::character varying NOT NULL,
    data_public boolean DEFAULT false NOT NULL,
    description text DEFAULT ''::text NOT NULL,
    home_lat double precision,
    home_lon double precision,
    home_zoom smallint DEFAULT 3,
    nearby integer DEFAULT 50,
    pass_salt character varying,
    image_file_name text,
    email_valid boolean DEFAULT false NOT NULL,
    new_email character varying,
    creation_ip character varying,
    languages character varying,
    status user_status_enum DEFAULT 'pending'::user_status_enum NOT NULL,
    terms_agreed timestamp without time zone,
    consider_pd boolean DEFAULT false NOT NULL,
    openid_url character varying,
    preferred_editor character varying,
    terms_seen boolean DEFAULT false NOT NULL,
    description_format format_enum DEFAULT 'markdown'::format_enum NOT NULL,
    image_fingerprint character varying,
    changesets_count integer DEFAULT 0 NOT NULL,
    traces_count integer DEFAULT 0 NOT NULL,
    diary_entries_count integer DEFAULT 0 NOT NULL,
    image_use_gravatar boolean DEFAULT true NOT NULL,
    image_content_type character varying
);


ALTER TABLE public.users OWNER TO hoot;

--
-- Name: users_id_seq; Type: SEQUENCE; Schema: public; Owner: hoot
--

CREATE SEQUENCE users_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.users_id_seq OWNER TO hoot;

--
-- Name: users_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: hoot
--

ALTER SEQUENCE users_id_seq OWNED BY users.id;


--
-- Name: way_nodes; Type: TABLE; Schema: public; Owner: hoot; Tablespace: 
--

CREATE TABLE way_nodes (
    way_id bigint NOT NULL,
    node_id bigint NOT NULL,
    version bigint NOT NULL,
    sequence_id bigint NOT NULL
);


ALTER TABLE public.way_nodes OWNER TO hoot;

--
-- Name: way_tags; Type: TABLE; Schema: public; Owner: hoot; Tablespace: 
--

CREATE TABLE way_tags (
    way_id bigint DEFAULT 0 NOT NULL,
    k character varying NOT NULL,
    v character varying NOT NULL,
    version bigint NOT NULL
);


ALTER TABLE public.way_tags OWNER TO hoot;

--
-- Name: ways; Type: TABLE; Schema: public; Owner: hoot; Tablespace: 
--

CREATE TABLE ways (
    way_id bigint DEFAULT 0 NOT NULL,
    changeset_id bigint NOT NULL,
    "timestamp" timestamp without time zone NOT NULL,
    version bigint NOT NULL,
    visible boolean DEFAULT true NOT NULL,
    redaction_id integer
);


ALTER TABLE public.ways OWNER TO hoot;

--
-- Name: id; Type: DEFAULT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY acls ALTER COLUMN id SET DEFAULT nextval('acls_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY changeset_comments ALTER COLUMN id SET DEFAULT nextval('changeset_comments_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY changesets ALTER COLUMN id SET DEFAULT nextval('changesets_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY client_applications ALTER COLUMN id SET DEFAULT nextval('client_applications_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY current_nodes ALTER COLUMN id SET DEFAULT nextval('current_nodes_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY current_relations ALTER COLUMN id SET DEFAULT nextval('current_relations_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY current_ways ALTER COLUMN id SET DEFAULT nextval('current_ways_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY diary_comments ALTER COLUMN id SET DEFAULT nextval('diary_comments_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY diary_entries ALTER COLUMN id SET DEFAULT nextval('diary_entries_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY friends ALTER COLUMN id SET DEFAULT nextval('friends_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY gpx_file_tags ALTER COLUMN id SET DEFAULT nextval('gpx_file_tags_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY gpx_files ALTER COLUMN id SET DEFAULT nextval('gpx_files_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY messages ALTER COLUMN id SET DEFAULT nextval('messages_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY note_comments ALTER COLUMN id SET DEFAULT nextval('note_comments_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY notes ALTER COLUMN id SET DEFAULT nextval('notes_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY oauth_nonces ALTER COLUMN id SET DEFAULT nextval('oauth_nonces_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY oauth_tokens ALTER COLUMN id SET DEFAULT nextval('oauth_tokens_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY redactions ALTER COLUMN id SET DEFAULT nextval('redactions_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY user_blocks ALTER COLUMN id SET DEFAULT nextval('user_blocks_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY user_roles ALTER COLUMN id SET DEFAULT nextval('user_roles_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY user_tokens ALTER COLUMN id SET DEFAULT nextval('user_tokens_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY users ALTER COLUMN id SET DEFAULT nextval('users_id_seq'::regclass);


--
-- Data for Name: acls; Type: TABLE DATA; Schema: public; Owner: hoot
--

COPY acls (id, address, k, v, domain) FROM stdin;
\.


--
-- Name: acls_id_seq; Type: SEQUENCE SET; Schema: public; Owner: hoot
--

SELECT pg_catalog.setval('acls_id_seq', 1, false);


--
-- Data for Name: changeset_comments; Type: TABLE DATA; Schema: public; Owner: hoot
--

COPY changeset_comments (id, changeset_id, author_id, body, created_at, visible) FROM stdin;
\.


--
-- Name: changeset_comments_id_seq; Type: SEQUENCE SET; Schema: public; Owner: hoot
--

SELECT pg_catalog.setval('changeset_comments_id_seq', 1, false);


--
-- Data for Name: changeset_tags; Type: TABLE DATA; Schema: public; Owner: hoot
--

COPY changeset_tags (changeset_id, k, v) FROM stdin;
\.


--
-- Data for Name: changesets; Type: TABLE DATA; Schema: public; Owner: hoot
--

COPY changesets (id, user_id, created_at, min_lat, max_lat, min_lon, max_lon, closed_at, num_changes) FROM stdin;
\.


--
-- Name: changesets_id_seq; Type: SEQUENCE SET; Schema: public; Owner: hoot
--

SELECT pg_catalog.setval('changesets_id_seq', 1, false);


--
-- Data for Name: changesets_subscribers; Type: TABLE DATA; Schema: public; Owner: hoot
--

COPY changesets_subscribers (subscriber_id, changeset_id) FROM stdin;
\.


--
-- Data for Name: client_applications; Type: TABLE DATA; Schema: public; Owner: hoot
--

COPY client_applications (id, name, url, support_url, callback_url, key, secret, user_id, created_at, updated_at, allow_read_prefs, allow_write_prefs, allow_write_diary, allow_write_api, allow_read_gpx, allow_write_gpx, allow_write_notes) FROM stdin;
\.


--
-- Name: client_applications_id_seq; Type: SEQUENCE SET; Schema: public; Owner: hoot
--

SELECT pg_catalog.setval('client_applications_id_seq', 1, false);


--
-- Data for Name: current_node_tags; Type: TABLE DATA; Schema: public; Owner: hoot
--

COPY current_node_tags (node_id, k, v) FROM stdin;
\.


--
-- Data for Name: current_nodes; Type: TABLE DATA; Schema: public; Owner: hoot
--

COPY current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) FROM stdin;
\.


--
-- Name: current_nodes_id_seq; Type: SEQUENCE SET; Schema: public; Owner: hoot
--

SELECT pg_catalog.setval('current_nodes_id_seq', 1, false);


--
-- Data for Name: current_relation_members; Type: TABLE DATA; Schema: public; Owner: hoot
--

COPY current_relation_members (relation_id, member_type, member_id, member_role, sequence_id) FROM stdin;
\.


--
-- Data for Name: current_relation_tags; Type: TABLE DATA; Schema: public; Owner: hoot
--

COPY current_relation_tags (relation_id, k, v) FROM stdin;
\.


--
-- Data for Name: current_relations; Type: TABLE DATA; Schema: public; Owner: hoot
--

COPY current_relations (id, changeset_id, "timestamp", visible, version) FROM stdin;
\.


--
-- Name: current_relations_id_seq; Type: SEQUENCE SET; Schema: public; Owner: hoot
--

SELECT pg_catalog.setval('current_relations_id_seq', 1, false);


--
-- Data for Name: current_way_nodes; Type: TABLE DATA; Schema: public; Owner: hoot
--

COPY current_way_nodes (way_id, node_id, sequence_id) FROM stdin;
\.


--
-- Data for Name: current_way_tags; Type: TABLE DATA; Schema: public; Owner: hoot
--

COPY current_way_tags (way_id, k, v) FROM stdin;
\.


--
-- Data for Name: current_ways; Type: TABLE DATA; Schema: public; Owner: hoot
--

COPY current_ways (id, changeset_id, "timestamp", visible, version) FROM stdin;
\.


--
-- Name: current_ways_id_seq; Type: SEQUENCE SET; Schema: public; Owner: hoot
--

SELECT pg_catalog.setval('current_ways_id_seq', 1, false);


--
-- Data for Name: diary_comments; Type: TABLE DATA; Schema: public; Owner: hoot
--

COPY diary_comments (id, diary_entry_id, user_id, body, created_at, updated_at, visible, body_format) FROM stdin;
\.


--
-- Name: diary_comments_id_seq; Type: SEQUENCE SET; Schema: public; Owner: hoot
--

SELECT pg_catalog.setval('diary_comments_id_seq', 1, false);


--
-- Data for Name: diary_entries; Type: TABLE DATA; Schema: public; Owner: hoot
--

COPY diary_entries (id, user_id, title, body, created_at, updated_at, latitude, longitude, language_code, visible, body_format) FROM stdin;
\.


--
-- Name: diary_entries_id_seq; Type: SEQUENCE SET; Schema: public; Owner: hoot
--

SELECT pg_catalog.setval('diary_entries_id_seq', 1, false);


--
-- Data for Name: friends; Type: TABLE DATA; Schema: public; Owner: hoot
--

COPY friends (id, user_id, friend_user_id) FROM stdin;
\.


--
-- Name: friends_id_seq; Type: SEQUENCE SET; Schema: public; Owner: hoot
--

SELECT pg_catalog.setval('friends_id_seq', 1, false);


--
-- Data for Name: gps_points; Type: TABLE DATA; Schema: public; Owner: hoot
--

COPY gps_points (altitude, trackid, latitude, longitude, gpx_id, "timestamp", tile) FROM stdin;
\.


--
-- Data for Name: gpx_file_tags; Type: TABLE DATA; Schema: public; Owner: hoot
--

COPY gpx_file_tags (gpx_id, tag, id) FROM stdin;
\.


--
-- Name: gpx_file_tags_id_seq; Type: SEQUENCE SET; Schema: public; Owner: hoot
--

SELECT pg_catalog.setval('gpx_file_tags_id_seq', 1, false);


--
-- Data for Name: gpx_files; Type: TABLE DATA; Schema: public; Owner: hoot
--

COPY gpx_files (id, user_id, visible, name, size, latitude, longitude, "timestamp", description, inserted, visibility) FROM stdin;
\.


--
-- Name: gpx_files_id_seq; Type: SEQUENCE SET; Schema: public; Owner: hoot
--

SELECT pg_catalog.setval('gpx_files_id_seq', 1, false);


--
-- Data for Name: languages; Type: TABLE DATA; Schema: public; Owner: hoot
--

COPY languages (code, english_name, native_name) FROM stdin;
aa	Afar	Afaraf
ab	Abkhazian	Аҧсуа
ae	Avestan	avesta
af	Afrikaans	Afrikaans
ak	Akan	Akan
am	Amharic	አማርኛ
an	Aragonese	Aragonés
ar	Arabic	العربية
as	Assamese	অসমীয়া
av	Avaric	Авар
ay	Aymara	Aymar aru
az	Azerbaijani	Azərbaycan
ba	Bashkir	Башҡорт
be	Belarusian	Беларуская
be-Tarask	Belarusian (Taraškievica orthography)	беларуская (тарашкевіца)
bg	Bulgarian	Български
bh	Bihari	भोजपुरी
bi	Bislama	Bislama
bm	Bambara	Bamanankan
bn	Bengali	বাংলা
bo	Tibetan	བོད་ཡིག
br	Breton	Brezhoneg
bs	Bosnian	Bosanski
ca	Catalan	Català
ce	Chechen	Нохчийн
ch	Chamorro	Chamoru
co	Corsican	Corsu
cr	Cree	ᓀᐦᐃᔭᐍᐏᐣ
cs	Czech	Česky
cu	Church Slavic	Словѣ́ньскъ / ⰔⰎⰑⰂⰡⰐⰠⰔⰍⰟ
cv	Chuvash	Чӑвашла
cy	Welsh	Cymraeg
da	Danish	Dansk
de	German	Deutsch
dsb	Lower Sorbian	Dolnoserbski
dv	Divehi	ދިވެހިބަސް
dz	Dzongkha	ཇོང་ཁ
ee	Ewe	Eʋegbe
el	Greek	Ελληνικά
en	English	English
eo	Esperanto	Esperanto
es	Spanish	Español
et	Estonian	Eesti
eu	Basque	Euskara
fa	Persian	فارسی
ff	Fulah	Fulfulde
fi	Finnish	Suomi
fj	Fijian	Na Vosa Vakaviti
fo	Faroese	Føroyskt
fr	French	Français
fur	Friulian	Furlan
fy	Western Frisian	Frysk
ga	Irish	Gaeilge
gd	Scottish Gaelic	Gàidhlig
gl	Galician	Galego
gn	Guarani	Avañe'ẽ
gsw	Swiss German	Alemannisch
gu	Gujarati	ગુજરાતી
gv	Manx	Gaelg
ha	Hausa	هَوُسَ
he	Hebrew	עברית
hi	Hindi	हिन्दी
ho	Hiri Motu	Hiri Motu
hr	Croatian	Hrvatski
hsb	Upper Sorbian	Hornjoserbsce
ht	Haitian	Kreyòl ayisyen
hu	Hungarian	Magyar
hy	Armenian	Հայերեն
hz	Herero	Otjiherero
ia	Interlingua	Interlingua
id	Indonesian	Bahasa Indonesia
ie	Interlingue	Interlingue
ig	Igbo	Igbo
ii	Sichuan Yi	ꆇꉙ
ik	Inupiaq	Iñupiak
io	Ido	Ido
is	Icelandic	Íslenska
it	Italian	Italiano
iu	Inuktitut	ᐃᓄᒃᑎᑐᑦ/inuktitut
ja	Japanese	日本語
jv	Javanese	Basa Jawa
ka	Georgian	ქართული
kg	Kongo	Kongo
ki	Kikuyu	Gĩkũyũ
kj	Kwanyama	Kuanyama
kk	Kazakh	Қазақша
kl	Kalaallisut	Kalaallisut
km	Khmer	ភាសាខ្មែរ
kn	Kannada	ಕನ್ನಡ
ko	Korean	한국어
kr	Kanuri	Kanuri
ks	Kashmiri	(كشميري)
ksh	Ripoarisch	Ripoarisch
ku	Kurdish	Kurdî / كوردی
kv	Komi	Коми
kw	Cornish	Kernowek
ky	Kirghiz	Кыргызча
la	Latin	Latina
lb	Luxembourgish	Lëtzebuergesch
lg	Ganda	Luganda
li	Limburgish	Limburgs
ln	Lingala	Lingála
lo	Lao	ລາວ
lt	Lithuanian	Lietuvių
lu	Luba-Katanga	\N
lv	Latvian	Latviešu
mg	Malagasy	Malagasy
mh	Marshallese	Kajin M̧ajeļ
mi	Maori	Māori
mk	Macedonian	Македонски
ml	Malayalam	മലയാളം
mn	Mongolian	Монгол
mo	Moldavian	Молдовеняскэ
mr	Marathi	मराठी
ms	Malay	Bahasa Melayu
mt	Maltese	Malti
my	Burmese	မြန်မာဘာသာ
na	Nauru	Dorerin Naoero
nb	Norwegian Bokmål	‪Norsk (bokmål)‬
nd	North Ndebele	isiNdebele
nds	Low German	Plattdüütsch
ne	Nepali	नेपाली
ng	Ndonga	Owambo
nl	Dutch	Nederlands
nn	Norwegian Nynorsk	‪Norsk (nynorsk)‬
no	Norwegian (bokmål)‬	‪Norsk (bokmål)‬
nr	South Ndebele	isiNdebele
nv	Navajo	Diné bizaad
ny	Nyanja	Chi-Chewa
oc	Occitan	Occitan
oj	Ojibwa	ᐊᓂᔑᓈᐯᒧᐎᓐ
om	Oromo	Oromoo
or	Oriya	ଓଡ଼ିଆ
os	Ossetic	Иронау
pa	Punjabi	ਪੰਜਾਬੀ
pi	Pali	पािऴ
pl	Polish	Polski
ps	Pashto	پښتو
pt	Portuguese	Português
pt-BR	Brazilian Portuguese	Português do Brasil
qu	Quechua	Runa Simi
rm	Rhaeto-Romance	Rumantsch
rn	Kirundi	kiRundi
ro	Romanian	Română
ru	Russian	Русский
rw	Kinyarwanda	Ikinyarwanda
sa	Sanskrit	संस्कृत
sc	Sardinian	Sardu
sd	Sindhi	سنڌي
se	Northern Sami	Sámegiella
sg	Sango	Sängö
sh	Serbo-Croatian	Srpskohrvatski / Српскохрватски
si	Sinhala	සිංහල
sk	Slovak	Slovenčina
sl	Slovenian	Slovenščina
sm	Samoan	Gagana Samoa
sn	Shona	chiShona
so	Somali	Soomaaliga
sq	Albanian	Shqip
sr	Serbian	Српски
sr-Latn	Serbian (Latin script)	srpski (latinica)
ss	Swati	SiSwati
st	Southern Sotho	Sesotho
su	Sundanese	Basa Sunda
sv	Swedish	Svenska
sw	Swahili	Kiswahili
ta	Tamil	தமிழ்
te	Telugu	తెలుగు
tg	Tajik	Тоҷикӣ
th	Thai	ไทย
ti	Tigrinya	ትግርኛ
tk	Turkmen	Türkmençe
tl	Tagalog	Tagalog
tn	Tswana	Setswana
to	Tonga	lea faka-Tonga
tr	Turkish	Türkçe
ts	Tsonga	Xitsonga
tt	Tatar	Татарча/Tatarça
tw	Twi	Twi
ty	Tahitian	Reo Mā`ohi
ug	Uighur	Uyghurche‎ / ئۇيغۇرچە
uk	Ukrainian	Українська
ur	Urdu	اردو
uz	Uzbek	O'zbek
ve	Venda	Tshivenda
vi	Vietnamese	Tiếng Việt
vo	Volapük	Volapük
wa	Walloon	Walon
wo	Wolof	Wolof
xh	Xhosa	isiXhosa
yi	Yiddish	ייִדיש
yo	Yoruba	Yorùbá
za	Zhuang	Vahcuengh
zh	Chinese	中文
zh-CN	Chinese (China)	‪中文(中国大陆)‬
zh-TW	Chinese (Taiwan)	‪中文(台灣)‬
zu	Zulu	isiZulu
\.


--
-- Data for Name: messages; Type: TABLE DATA; Schema: public; Owner: hoot
--

COPY messages (id, from_user_id, title, body, sent_on, message_read, to_user_id, to_user_visible, from_user_visible, body_format) FROM stdin;
\.


--
-- Name: messages_id_seq; Type: SEQUENCE SET; Schema: public; Owner: hoot
--

SELECT pg_catalog.setval('messages_id_seq', 1, false);


--
-- Data for Name: node_tags; Type: TABLE DATA; Schema: public; Owner: hoot
--

COPY node_tags (node_id, version, k, v) FROM stdin;
\.


--
-- Data for Name: nodes; Type: TABLE DATA; Schema: public; Owner: hoot
--

COPY nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version, redaction_id) FROM stdin;
\.


--
-- Data for Name: note_comments; Type: TABLE DATA; Schema: public; Owner: hoot
--

COPY note_comments (id, note_id, visible, created_at, author_ip, author_id, body, event) FROM stdin;
\.


--
-- Name: note_comments_id_seq; Type: SEQUENCE SET; Schema: public; Owner: hoot
--

SELECT pg_catalog.setval('note_comments_id_seq', 1, false);


--
-- Data for Name: notes; Type: TABLE DATA; Schema: public; Owner: hoot
--

COPY notes (id, latitude, longitude, tile, updated_at, created_at, status, closed_at) FROM stdin;
\.


--
-- Name: notes_id_seq; Type: SEQUENCE SET; Schema: public; Owner: hoot
--

SELECT pg_catalog.setval('notes_id_seq', 1, false);


--
-- Data for Name: oauth_nonces; Type: TABLE DATA; Schema: public; Owner: hoot
--

COPY oauth_nonces (id, nonce, "timestamp", created_at, updated_at) FROM stdin;
\.


--
-- Name: oauth_nonces_id_seq; Type: SEQUENCE SET; Schema: public; Owner: hoot
--

SELECT pg_catalog.setval('oauth_nonces_id_seq', 1, false);


--
-- Data for Name: oauth_tokens; Type: TABLE DATA; Schema: public; Owner: hoot
--

COPY oauth_tokens (id, user_id, type, client_application_id, token, secret, authorized_at, invalidated_at, created_at, updated_at, allow_read_prefs, allow_write_prefs, allow_write_diary, allow_write_api, allow_read_gpx, allow_write_gpx, callback_url, verifier, scope, valid_to, allow_write_notes) FROM stdin;
\.


--
-- Name: oauth_tokens_id_seq; Type: SEQUENCE SET; Schema: public; Owner: hoot
--

SELECT pg_catalog.setval('oauth_tokens_id_seq', 1, false);


--
-- Data for Name: redactions; Type: TABLE DATA; Schema: public; Owner: hoot
--

COPY redactions (id, title, description, created_at, updated_at, user_id, description_format) FROM stdin;
\.


--
-- Name: redactions_id_seq; Type: SEQUENCE SET; Schema: public; Owner: hoot
--

SELECT pg_catalog.setval('redactions_id_seq', 1, false);


--
-- Data for Name: relation_members; Type: TABLE DATA; Schema: public; Owner: hoot
--

COPY relation_members (relation_id, member_type, member_id, member_role, version, sequence_id) FROM stdin;
\.


--
-- Data for Name: relation_tags; Type: TABLE DATA; Schema: public; Owner: hoot
--

COPY relation_tags (relation_id, k, v, version) FROM stdin;
\.


--
-- Data for Name: relations; Type: TABLE DATA; Schema: public; Owner: hoot
--

COPY relations (relation_id, changeset_id, "timestamp", version, visible, redaction_id) FROM stdin;
\.


--
-- Data for Name: schema_migrations; Type: TABLE DATA; Schema: public; Owner: hoot
--

COPY schema_migrations (version) FROM stdin;
1
2
3
4
5
6
7
8
9
10
11
12
13
14
15
16
17
18
19
20
21
22
23
24
25
26
27
28
29
30
31
32
33
34
35
36
37
38
39
40
41
42
43
44
45
46
47
48
49
50
51
52
53
54
55
56
57
20100513171259
20100516124737
20100910084426
20101114011429
20110322001319
20110508145337
20110521142405
20110925112722
20111116184519
20111212183945
20120123184321
20120208122334
20120208194454
20120214210114
20120219161649
20120318201948
20120328090602
20120404205604
20120808231205
20121005195010
20121012044047
20121119165817
20121202155309
20121203124841
20130328184137
20131212124700
20140115192822
20140117185510
20140210003018
20140507110937
20140519141742
20150110152606
20150111192335
\.


--
-- Data for Name: user_blocks; Type: TABLE DATA; Schema: public; Owner: hoot
--

COPY user_blocks (id, user_id, creator_id, reason, ends_at, needs_view, revoker_id, created_at, updated_at, reason_format) FROM stdin;
\.


--
-- Name: user_blocks_id_seq; Type: SEQUENCE SET; Schema: public; Owner: hoot
--

SELECT pg_catalog.setval('user_blocks_id_seq', 1, false);


--
-- Data for Name: user_preferences; Type: TABLE DATA; Schema: public; Owner: hoot
--

COPY user_preferences (user_id, k, v) FROM stdin;
\.


--
-- Data for Name: user_roles; Type: TABLE DATA; Schema: public; Owner: hoot
--

COPY user_roles (id, user_id, role, created_at, updated_at, granter_id) FROM stdin;
\.


--
-- Name: user_roles_id_seq; Type: SEQUENCE SET; Schema: public; Owner: hoot
--

SELECT pg_catalog.setval('user_roles_id_seq', 1, false);


--
-- Data for Name: user_tokens; Type: TABLE DATA; Schema: public; Owner: hoot
--

COPY user_tokens (id, user_id, token, expiry, referer) FROM stdin;
\.


--
-- Name: user_tokens_id_seq; Type: SEQUENCE SET; Schema: public; Owner: hoot
--

SELECT pg_catalog.setval('user_tokens_id_seq', 1, false);


--
-- Data for Name: users; Type: TABLE DATA; Schema: public; Owner: hoot
--

COPY users (email, id, pass_crypt, creation_time, display_name, data_public, description, home_lat, home_lon, home_zoom, nearby, pass_salt, image_file_name, email_valid, new_email, creation_ip, languages, status, terms_agreed, consider_pd, openid_url, preferred_editor, terms_seen, description_format, image_fingerprint, changesets_count, traces_count, diary_entries_count, image_use_gravatar, image_content_type) FROM stdin;
\.


--
-- Name: users_id_seq; Type: SEQUENCE SET; Schema: public; Owner: hoot
--

SELECT pg_catalog.setval('users_id_seq', 1, false);


--
-- Data for Name: way_nodes; Type: TABLE DATA; Schema: public; Owner: hoot
--

COPY way_nodes (way_id, node_id, version, sequence_id) FROM stdin;
\.


--
-- Data for Name: way_tags; Type: TABLE DATA; Schema: public; Owner: hoot
--

COPY way_tags (way_id, k, v, version) FROM stdin;
\.


--
-- Data for Name: ways; Type: TABLE DATA; Schema: public; Owner: hoot
--

COPY ways (way_id, changeset_id, "timestamp", version, visible, redaction_id) FROM stdin;
\.


--
-- Name: acls_pkey; Type: CONSTRAINT; Schema: public; Owner: hoot; Tablespace: 
--

ALTER TABLE ONLY acls
    ADD CONSTRAINT acls_pkey PRIMARY KEY (id);


--
-- Name: changeset_comments_pkey; Type: CONSTRAINT; Schema: public; Owner: hoot; Tablespace: 
--

ALTER TABLE ONLY changeset_comments
    ADD CONSTRAINT changeset_comments_pkey PRIMARY KEY (id);


--
-- Name: changesets_pkey; Type: CONSTRAINT; Schema: public; Owner: hoot; Tablespace: 
--

ALTER TABLE ONLY changesets
    ADD CONSTRAINT changesets_pkey PRIMARY KEY (id);


--
-- Name: client_applications_pkey; Type: CONSTRAINT; Schema: public; Owner: hoot; Tablespace: 
--

ALTER TABLE ONLY client_applications
    ADD CONSTRAINT client_applications_pkey PRIMARY KEY (id);


--
-- Name: current_node_tags_pkey; Type: CONSTRAINT; Schema: public; Owner: hoot; Tablespace: 
--

ALTER TABLE ONLY current_node_tags
    ADD CONSTRAINT current_node_tags_pkey PRIMARY KEY (node_id, k);


--
-- Name: current_nodes_pkey1; Type: CONSTRAINT; Schema: public; Owner: hoot; Tablespace: 
--

ALTER TABLE ONLY current_nodes
    ADD CONSTRAINT current_nodes_pkey1 PRIMARY KEY (id);


--
-- Name: current_relation_members_pkey; Type: CONSTRAINT; Schema: public; Owner: hoot; Tablespace: 
--

ALTER TABLE ONLY current_relation_members
    ADD CONSTRAINT current_relation_members_pkey PRIMARY KEY (relation_id, member_type, member_id, member_role, sequence_id);


--
-- Name: current_relation_tags_pkey; Type: CONSTRAINT; Schema: public; Owner: hoot; Tablespace: 
--

ALTER TABLE ONLY current_relation_tags
    ADD CONSTRAINT current_relation_tags_pkey PRIMARY KEY (relation_id, k);


--
-- Name: current_relations_pkey; Type: CONSTRAINT; Schema: public; Owner: hoot; Tablespace: 
--

ALTER TABLE ONLY current_relations
    ADD CONSTRAINT current_relations_pkey PRIMARY KEY (id);


--
-- Name: current_way_nodes_pkey; Type: CONSTRAINT; Schema: public; Owner: hoot; Tablespace: 
--

ALTER TABLE ONLY current_way_nodes
    ADD CONSTRAINT current_way_nodes_pkey PRIMARY KEY (way_id, sequence_id);


--
-- Name: current_way_tags_pkey; Type: CONSTRAINT; Schema: public; Owner: hoot; Tablespace: 
--

ALTER TABLE ONLY current_way_tags
    ADD CONSTRAINT current_way_tags_pkey PRIMARY KEY (way_id, k);


--
-- Name: current_ways_pkey; Type: CONSTRAINT; Schema: public; Owner: hoot; Tablespace: 
--

ALTER TABLE ONLY current_ways
    ADD CONSTRAINT current_ways_pkey PRIMARY KEY (id);


--
-- Name: diary_comments_pkey; Type: CONSTRAINT; Schema: public; Owner: hoot; Tablespace: 
--

ALTER TABLE ONLY diary_comments
    ADD CONSTRAINT diary_comments_pkey PRIMARY KEY (id);


--
-- Name: diary_entries_pkey; Type: CONSTRAINT; Schema: public; Owner: hoot; Tablespace: 
--

ALTER TABLE ONLY diary_entries
    ADD CONSTRAINT diary_entries_pkey PRIMARY KEY (id);


--
-- Name: friends_pkey; Type: CONSTRAINT; Schema: public; Owner: hoot; Tablespace: 
--

ALTER TABLE ONLY friends
    ADD CONSTRAINT friends_pkey PRIMARY KEY (id);


--
-- Name: gpx_file_tags_pkey; Type: CONSTRAINT; Schema: public; Owner: hoot; Tablespace: 
--

ALTER TABLE ONLY gpx_file_tags
    ADD CONSTRAINT gpx_file_tags_pkey PRIMARY KEY (id);


--
-- Name: gpx_files_pkey; Type: CONSTRAINT; Schema: public; Owner: hoot; Tablespace: 
--

ALTER TABLE ONLY gpx_files
    ADD CONSTRAINT gpx_files_pkey PRIMARY KEY (id);


--
-- Name: languages_pkey; Type: CONSTRAINT; Schema: public; Owner: hoot; Tablespace: 
--

ALTER TABLE ONLY languages
    ADD CONSTRAINT languages_pkey PRIMARY KEY (code);


--
-- Name: messages_pkey; Type: CONSTRAINT; Schema: public; Owner: hoot; Tablespace: 
--

ALTER TABLE ONLY messages
    ADD CONSTRAINT messages_pkey PRIMARY KEY (id);


--
-- Name: node_tags_pkey; Type: CONSTRAINT; Schema: public; Owner: hoot; Tablespace: 
--

ALTER TABLE ONLY node_tags
    ADD CONSTRAINT node_tags_pkey PRIMARY KEY (node_id, version, k);


--
-- Name: nodes_pkey; Type: CONSTRAINT; Schema: public; Owner: hoot; Tablespace: 
--

ALTER TABLE ONLY nodes
    ADD CONSTRAINT nodes_pkey PRIMARY KEY (node_id, version);


--
-- Name: note_comments_pkey; Type: CONSTRAINT; Schema: public; Owner: hoot; Tablespace: 
--

ALTER TABLE ONLY note_comments
    ADD CONSTRAINT note_comments_pkey PRIMARY KEY (id);


--
-- Name: notes_pkey; Type: CONSTRAINT; Schema: public; Owner: hoot; Tablespace: 
--

ALTER TABLE ONLY notes
    ADD CONSTRAINT notes_pkey PRIMARY KEY (id);


--
-- Name: oauth_nonces_pkey; Type: CONSTRAINT; Schema: public; Owner: hoot; Tablespace: 
--

ALTER TABLE ONLY oauth_nonces
    ADD CONSTRAINT oauth_nonces_pkey PRIMARY KEY (id);


--
-- Name: oauth_tokens_pkey; Type: CONSTRAINT; Schema: public; Owner: hoot; Tablespace: 
--

ALTER TABLE ONLY oauth_tokens
    ADD CONSTRAINT oauth_tokens_pkey PRIMARY KEY (id);


--
-- Name: redactions_pkey; Type: CONSTRAINT; Schema: public; Owner: hoot; Tablespace: 
--

ALTER TABLE ONLY redactions
    ADD CONSTRAINT redactions_pkey PRIMARY KEY (id);


--
-- Name: relation_members_pkey; Type: CONSTRAINT; Schema: public; Owner: hoot; Tablespace: 
--

ALTER TABLE ONLY relation_members
    ADD CONSTRAINT relation_members_pkey PRIMARY KEY (relation_id, version, member_type, member_id, member_role, sequence_id);


--
-- Name: relation_tags_pkey; Type: CONSTRAINT; Schema: public; Owner: hoot; Tablespace: 
--

ALTER TABLE ONLY relation_tags
    ADD CONSTRAINT relation_tags_pkey PRIMARY KEY (relation_id, version, k);


--
-- Name: relations_pkey; Type: CONSTRAINT; Schema: public; Owner: hoot; Tablespace: 
--

ALTER TABLE ONLY relations
    ADD CONSTRAINT relations_pkey PRIMARY KEY (relation_id, version);


--
-- Name: user_blocks_pkey; Type: CONSTRAINT; Schema: public; Owner: hoot; Tablespace: 
--

ALTER TABLE ONLY user_blocks
    ADD CONSTRAINT user_blocks_pkey PRIMARY KEY (id);


--
-- Name: user_preferences_pkey; Type: CONSTRAINT; Schema: public; Owner: hoot; Tablespace: 
--

ALTER TABLE ONLY user_preferences
    ADD CONSTRAINT user_preferences_pkey PRIMARY KEY (user_id, k);


--
-- Name: user_roles_pkey; Type: CONSTRAINT; Schema: public; Owner: hoot; Tablespace: 
--

ALTER TABLE ONLY user_roles
    ADD CONSTRAINT user_roles_pkey PRIMARY KEY (id);


--
-- Name: user_tokens_pkey; Type: CONSTRAINT; Schema: public; Owner: hoot; Tablespace: 
--

ALTER TABLE ONLY user_tokens
    ADD CONSTRAINT user_tokens_pkey PRIMARY KEY (id);


--
-- Name: users_pkey; Type: CONSTRAINT; Schema: public; Owner: hoot; Tablespace: 
--

ALTER TABLE ONLY users
    ADD CONSTRAINT users_pkey PRIMARY KEY (id);


--
-- Name: way_nodes_pkey; Type: CONSTRAINT; Schema: public; Owner: hoot; Tablespace: 
--

ALTER TABLE ONLY way_nodes
    ADD CONSTRAINT way_nodes_pkey PRIMARY KEY (way_id, version, sequence_id);


--
-- Name: way_tags_pkey; Type: CONSTRAINT; Schema: public; Owner: hoot; Tablespace: 
--

ALTER TABLE ONLY way_tags
    ADD CONSTRAINT way_tags_pkey PRIMARY KEY (way_id, version, k);


--
-- Name: ways_pkey; Type: CONSTRAINT; Schema: public; Owner: hoot; Tablespace: 
--

ALTER TABLE ONLY ways
    ADD CONSTRAINT ways_pkey PRIMARY KEY (way_id, version);


--
-- Name: acls_k_idx; Type: INDEX; Schema: public; Owner: hoot; Tablespace: 
--

CREATE INDEX acls_k_idx ON acls USING btree (k);


--
-- Name: changeset_tags_id_idx; Type: INDEX; Schema: public; Owner: hoot; Tablespace: 
--

CREATE INDEX changeset_tags_id_idx ON changeset_tags USING btree (changeset_id);


--
-- Name: changesets_bbox_idx; Type: INDEX; Schema: public; Owner: hoot; Tablespace: 
--

CREATE INDEX changesets_bbox_idx ON changesets USING gist (min_lat, max_lat, min_lon, max_lon);


--
-- Name: changesets_closed_at_idx; Type: INDEX; Schema: public; Owner: hoot; Tablespace: 
--

CREATE INDEX changesets_closed_at_idx ON changesets USING btree (closed_at);


--
-- Name: changesets_created_at_idx; Type: INDEX; Schema: public; Owner: hoot; Tablespace: 
--

CREATE INDEX changesets_created_at_idx ON changesets USING btree (created_at);


--
-- Name: changesets_user_id_created_at_idx; Type: INDEX; Schema: public; Owner: hoot; Tablespace: 
--

CREATE INDEX changesets_user_id_created_at_idx ON changesets USING btree (user_id, created_at);


--
-- Name: changesets_user_id_id_idx; Type: INDEX; Schema: public; Owner: hoot; Tablespace: 
--

CREATE INDEX changesets_user_id_id_idx ON changesets USING btree (user_id, id);


--
-- Name: current_nodes_tile_idx; Type: INDEX; Schema: public; Owner: hoot; Tablespace: 
--

CREATE INDEX current_nodes_tile_idx ON current_nodes USING btree (tile);


--
-- Name: current_nodes_timestamp_idx; Type: INDEX; Schema: public; Owner: hoot; Tablespace: 
--

CREATE INDEX current_nodes_timestamp_idx ON current_nodes USING btree ("timestamp");


--
-- Name: current_relation_members_member_idx; Type: INDEX; Schema: public; Owner: hoot; Tablespace: 
--

CREATE INDEX current_relation_members_member_idx ON current_relation_members USING btree (member_type, member_id);


--
-- Name: current_relations_timestamp_idx; Type: INDEX; Schema: public; Owner: hoot; Tablespace: 
--

CREATE INDEX current_relations_timestamp_idx ON current_relations USING btree ("timestamp");


--
-- Name: current_way_nodes_node_idx; Type: INDEX; Schema: public; Owner: hoot; Tablespace: 
--

CREATE INDEX current_way_nodes_node_idx ON current_way_nodes USING btree (node_id);


--
-- Name: current_ways_timestamp_idx; Type: INDEX; Schema: public; Owner: hoot; Tablespace: 
--

CREATE INDEX current_ways_timestamp_idx ON current_ways USING btree ("timestamp");


--
-- Name: diary_comment_user_id_created_at_index; Type: INDEX; Schema: public; Owner: hoot; Tablespace: 
--

CREATE INDEX diary_comment_user_id_created_at_index ON diary_comments USING btree (user_id, created_at);


--
-- Name: diary_comments_entry_id_idx; Type: INDEX; Schema: public; Owner: hoot; Tablespace: 
--

CREATE UNIQUE INDEX diary_comments_entry_id_idx ON diary_comments USING btree (diary_entry_id, id);


--
-- Name: diary_entry_created_at_index; Type: INDEX; Schema: public; Owner: hoot; Tablespace: 
--

CREATE INDEX diary_entry_created_at_index ON diary_entries USING btree (created_at);


--
-- Name: diary_entry_language_code_created_at_index; Type: INDEX; Schema: public; Owner: hoot; Tablespace: 
--

CREATE INDEX diary_entry_language_code_created_at_index ON diary_entries USING btree (language_code, created_at);


--
-- Name: diary_entry_user_id_created_at_index; Type: INDEX; Schema: public; Owner: hoot; Tablespace: 
--

CREATE INDEX diary_entry_user_id_created_at_index ON diary_entries USING btree (user_id, created_at);


--
-- Name: friends_user_id_idx; Type: INDEX; Schema: public; Owner: hoot; Tablespace: 
--

CREATE INDEX friends_user_id_idx ON friends USING btree (user_id);


--
-- Name: gpx_file_tags_gpxid_idx; Type: INDEX; Schema: public; Owner: hoot; Tablespace: 
--

CREATE INDEX gpx_file_tags_gpxid_idx ON gpx_file_tags USING btree (gpx_id);


--
-- Name: gpx_file_tags_tag_idx; Type: INDEX; Schema: public; Owner: hoot; Tablespace: 
--

CREATE INDEX gpx_file_tags_tag_idx ON gpx_file_tags USING btree (tag);


--
-- Name: gpx_files_timestamp_idx; Type: INDEX; Schema: public; Owner: hoot; Tablespace: 
--

CREATE INDEX gpx_files_timestamp_idx ON gpx_files USING btree ("timestamp");


--
-- Name: gpx_files_user_id_idx; Type: INDEX; Schema: public; Owner: hoot; Tablespace: 
--

CREATE INDEX gpx_files_user_id_idx ON gpx_files USING btree (user_id);


--
-- Name: gpx_files_visible_visibility_idx; Type: INDEX; Schema: public; Owner: hoot; Tablespace: 
--

CREATE INDEX gpx_files_visible_visibility_idx ON gpx_files USING btree (visible, visibility);


--
-- Name: index_changeset_comments_on_created_at; Type: INDEX; Schema: public; Owner: hoot; Tablespace: 
--

CREATE INDEX index_changeset_comments_on_created_at ON changeset_comments USING btree (created_at);


--
-- Name: index_changesets_subscribers_on_changeset_id; Type: INDEX; Schema: public; Owner: hoot; Tablespace: 
--

CREATE INDEX index_changesets_subscribers_on_changeset_id ON changesets_subscribers USING btree (changeset_id);


--
-- Name: index_changesets_subscribers_on_subscriber_id_and_changeset_id; Type: INDEX; Schema: public; Owner: hoot; Tablespace: 
--

CREATE UNIQUE INDEX index_changesets_subscribers_on_subscriber_id_and_changeset_id ON changesets_subscribers USING btree (subscriber_id, changeset_id);


--
-- Name: index_client_applications_on_key; Type: INDEX; Schema: public; Owner: hoot; Tablespace: 
--

CREATE UNIQUE INDEX index_client_applications_on_key ON client_applications USING btree (key);


--
-- Name: index_note_comments_on_body; Type: INDEX; Schema: public; Owner: hoot; Tablespace: 
--

CREATE INDEX index_note_comments_on_body ON note_comments USING gin (to_tsvector('english'::regconfig, body));


--
-- Name: index_note_comments_on_created_at; Type: INDEX; Schema: public; Owner: hoot; Tablespace: 
--

CREATE INDEX index_note_comments_on_created_at ON note_comments USING btree (created_at);


--
-- Name: index_oauth_nonces_on_nonce_and_timestamp; Type: INDEX; Schema: public; Owner: hoot; Tablespace: 
--

CREATE UNIQUE INDEX index_oauth_nonces_on_nonce_and_timestamp ON oauth_nonces USING btree (nonce, "timestamp");


--
-- Name: index_oauth_tokens_on_token; Type: INDEX; Schema: public; Owner: hoot; Tablespace: 
--

CREATE UNIQUE INDEX index_oauth_tokens_on_token ON oauth_tokens USING btree (token);


--
-- Name: index_user_blocks_on_user_id; Type: INDEX; Schema: public; Owner: hoot; Tablespace: 
--

CREATE INDEX index_user_blocks_on_user_id ON user_blocks USING btree (user_id);


--
-- Name: messages_from_user_id_idx; Type: INDEX; Schema: public; Owner: hoot; Tablespace: 
--

CREATE INDEX messages_from_user_id_idx ON messages USING btree (from_user_id);


--
-- Name: messages_to_user_id_idx; Type: INDEX; Schema: public; Owner: hoot; Tablespace: 
--

CREATE INDEX messages_to_user_id_idx ON messages USING btree (to_user_id);


--
-- Name: nodes_changeset_id_idx; Type: INDEX; Schema: public; Owner: hoot; Tablespace: 
--

CREATE INDEX nodes_changeset_id_idx ON nodes USING btree (changeset_id);


--
-- Name: nodes_tile_idx; Type: INDEX; Schema: public; Owner: hoot; Tablespace: 
--

CREATE INDEX nodes_tile_idx ON nodes USING btree (tile);


--
-- Name: nodes_timestamp_idx; Type: INDEX; Schema: public; Owner: hoot; Tablespace: 
--

CREATE INDEX nodes_timestamp_idx ON nodes USING btree ("timestamp");


--
-- Name: note_comments_note_id_idx; Type: INDEX; Schema: public; Owner: hoot; Tablespace: 
--

CREATE INDEX note_comments_note_id_idx ON note_comments USING btree (note_id);


--
-- Name: notes_created_at_idx; Type: INDEX; Schema: public; Owner: hoot; Tablespace: 
--

CREATE INDEX notes_created_at_idx ON notes USING btree (created_at);


--
-- Name: notes_tile_status_idx; Type: INDEX; Schema: public; Owner: hoot; Tablespace: 
--

CREATE INDEX notes_tile_status_idx ON notes USING btree (tile, status);


--
-- Name: notes_updated_at_idx; Type: INDEX; Schema: public; Owner: hoot; Tablespace: 
--

CREATE INDEX notes_updated_at_idx ON notes USING btree (updated_at);


--
-- Name: points_gpxid_idx; Type: INDEX; Schema: public; Owner: hoot; Tablespace: 
--

CREATE INDEX points_gpxid_idx ON gps_points USING btree (gpx_id);


--
-- Name: points_tile_idx; Type: INDEX; Schema: public; Owner: hoot; Tablespace: 
--

CREATE INDEX points_tile_idx ON gps_points USING btree (tile);


--
-- Name: relation_members_member_idx; Type: INDEX; Schema: public; Owner: hoot; Tablespace: 
--

CREATE INDEX relation_members_member_idx ON relation_members USING btree (member_type, member_id);


--
-- Name: relations_changeset_id_idx; Type: INDEX; Schema: public; Owner: hoot; Tablespace: 
--

CREATE INDEX relations_changeset_id_idx ON relations USING btree (changeset_id);


--
-- Name: relations_timestamp_idx; Type: INDEX; Schema: public; Owner: hoot; Tablespace: 
--

CREATE INDEX relations_timestamp_idx ON relations USING btree ("timestamp");


--
-- Name: unique_schema_migrations; Type: INDEX; Schema: public; Owner: hoot; Tablespace: 
--

CREATE UNIQUE INDEX unique_schema_migrations ON schema_migrations USING btree (version);


--
-- Name: user_id_idx; Type: INDEX; Schema: public; Owner: hoot; Tablespace: 
--

CREATE INDEX user_id_idx ON friends USING btree (friend_user_id);


--
-- Name: user_openid_url_idx; Type: INDEX; Schema: public; Owner: hoot; Tablespace: 
--

CREATE UNIQUE INDEX user_openid_url_idx ON users USING btree (openid_url);


--
-- Name: user_roles_id_role_unique; Type: INDEX; Schema: public; Owner: hoot; Tablespace: 
--

CREATE UNIQUE INDEX user_roles_id_role_unique ON user_roles USING btree (user_id, role);


--
-- Name: user_tokens_token_idx; Type: INDEX; Schema: public; Owner: hoot; Tablespace: 
--

CREATE UNIQUE INDEX user_tokens_token_idx ON user_tokens USING btree (token);


--
-- Name: user_tokens_user_id_idx; Type: INDEX; Schema: public; Owner: hoot; Tablespace: 
--

CREATE INDEX user_tokens_user_id_idx ON user_tokens USING btree (user_id);


--
-- Name: users_display_name_idx; Type: INDEX; Schema: public; Owner: hoot; Tablespace: 
--

CREATE UNIQUE INDEX users_display_name_idx ON users USING btree (display_name);


--
-- Name: users_display_name_lower_idx; Type: INDEX; Schema: public; Owner: hoot; Tablespace: 
--

CREATE INDEX users_display_name_lower_idx ON users USING btree (lower((display_name)::text));


--
-- Name: users_email_idx; Type: INDEX; Schema: public; Owner: hoot; Tablespace: 
--

CREATE UNIQUE INDEX users_email_idx ON users USING btree (email);


--
-- Name: users_email_lower_idx; Type: INDEX; Schema: public; Owner: hoot; Tablespace: 
--

CREATE INDEX users_email_lower_idx ON users USING btree (lower((email)::text));


--
-- Name: way_nodes_node_idx; Type: INDEX; Schema: public; Owner: hoot; Tablespace: 
--

CREATE INDEX way_nodes_node_idx ON way_nodes USING btree (node_id);


--
-- Name: ways_changeset_id_idx; Type: INDEX; Schema: public; Owner: hoot; Tablespace: 
--

CREATE INDEX ways_changeset_id_idx ON ways USING btree (changeset_id);


--
-- Name: ways_timestamp_idx; Type: INDEX; Schema: public; Owner: hoot; Tablespace: 
--

CREATE INDEX ways_timestamp_idx ON ways USING btree ("timestamp");


--
-- Name: changeset_comments_author_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY changeset_comments
    ADD CONSTRAINT changeset_comments_author_id_fkey FOREIGN KEY (author_id) REFERENCES users(id);


--
-- Name: changeset_comments_changeset_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY changeset_comments
    ADD CONSTRAINT changeset_comments_changeset_id_fkey FOREIGN KEY (changeset_id) REFERENCES changesets(id);


--
-- Name: changeset_tags_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY changeset_tags
    ADD CONSTRAINT changeset_tags_id_fkey FOREIGN KEY (changeset_id) REFERENCES changesets(id);


--
-- Name: changesets_subscribers_changeset_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY changesets_subscribers
    ADD CONSTRAINT changesets_subscribers_changeset_id_fkey FOREIGN KEY (changeset_id) REFERENCES changesets(id);


--
-- Name: changesets_subscribers_subscriber_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY changesets_subscribers
    ADD CONSTRAINT changesets_subscribers_subscriber_id_fkey FOREIGN KEY (subscriber_id) REFERENCES users(id);


--
-- Name: changesets_user_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY changesets
    ADD CONSTRAINT changesets_user_id_fkey FOREIGN KEY (user_id) REFERENCES users(id);


--
-- Name: client_applications_user_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY client_applications
    ADD CONSTRAINT client_applications_user_id_fkey FOREIGN KEY (user_id) REFERENCES users(id);


--
-- Name: current_node_tags_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY current_node_tags
    ADD CONSTRAINT current_node_tags_id_fkey FOREIGN KEY (node_id) REFERENCES current_nodes(id);


--
-- Name: current_nodes_changeset_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY current_nodes
    ADD CONSTRAINT current_nodes_changeset_id_fkey FOREIGN KEY (changeset_id) REFERENCES changesets(id);


--
-- Name: current_relation_members_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY current_relation_members
    ADD CONSTRAINT current_relation_members_id_fkey FOREIGN KEY (relation_id) REFERENCES current_relations(id);


--
-- Name: current_relation_tags_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY current_relation_tags
    ADD CONSTRAINT current_relation_tags_id_fkey FOREIGN KEY (relation_id) REFERENCES current_relations(id);


--
-- Name: current_relations_changeset_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY current_relations
    ADD CONSTRAINT current_relations_changeset_id_fkey FOREIGN KEY (changeset_id) REFERENCES changesets(id);


--
-- Name: current_way_nodes_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY current_way_nodes
    ADD CONSTRAINT current_way_nodes_id_fkey FOREIGN KEY (way_id) REFERENCES current_ways(id);


--
-- Name: current_way_nodes_node_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY current_way_nodes
    ADD CONSTRAINT current_way_nodes_node_id_fkey FOREIGN KEY (node_id) REFERENCES current_nodes(id);


--
-- Name: current_way_tags_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY current_way_tags
    ADD CONSTRAINT current_way_tags_id_fkey FOREIGN KEY (way_id) REFERENCES current_ways(id);


--
-- Name: current_ways_changeset_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY current_ways
    ADD CONSTRAINT current_ways_changeset_id_fkey FOREIGN KEY (changeset_id) REFERENCES changesets(id);


--
-- Name: diary_comments_diary_entry_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY diary_comments
    ADD CONSTRAINT diary_comments_diary_entry_id_fkey FOREIGN KEY (diary_entry_id) REFERENCES diary_entries(id);


--
-- Name: diary_comments_user_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY diary_comments
    ADD CONSTRAINT diary_comments_user_id_fkey FOREIGN KEY (user_id) REFERENCES users(id);


--
-- Name: diary_entries_language_code_fkey; Type: FK CONSTRAINT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY diary_entries
    ADD CONSTRAINT diary_entries_language_code_fkey FOREIGN KEY (language_code) REFERENCES languages(code);


--
-- Name: diary_entries_user_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY diary_entries
    ADD CONSTRAINT diary_entries_user_id_fkey FOREIGN KEY (user_id) REFERENCES users(id);


--
-- Name: friends_friend_user_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY friends
    ADD CONSTRAINT friends_friend_user_id_fkey FOREIGN KEY (friend_user_id) REFERENCES users(id);


--
-- Name: friends_user_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY friends
    ADD CONSTRAINT friends_user_id_fkey FOREIGN KEY (user_id) REFERENCES users(id);


--
-- Name: gps_points_gpx_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY gps_points
    ADD CONSTRAINT gps_points_gpx_id_fkey FOREIGN KEY (gpx_id) REFERENCES gpx_files(id);


--
-- Name: gpx_file_tags_gpx_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY gpx_file_tags
    ADD CONSTRAINT gpx_file_tags_gpx_id_fkey FOREIGN KEY (gpx_id) REFERENCES gpx_files(id);


--
-- Name: gpx_files_user_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY gpx_files
    ADD CONSTRAINT gpx_files_user_id_fkey FOREIGN KEY (user_id) REFERENCES users(id);


--
-- Name: messages_from_user_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY messages
    ADD CONSTRAINT messages_from_user_id_fkey FOREIGN KEY (from_user_id) REFERENCES users(id);


--
-- Name: messages_to_user_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY messages
    ADD CONSTRAINT messages_to_user_id_fkey FOREIGN KEY (to_user_id) REFERENCES users(id);


--
-- Name: node_tags_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY node_tags
    ADD CONSTRAINT node_tags_id_fkey FOREIGN KEY (node_id, version) REFERENCES nodes(node_id, version);


--
-- Name: nodes_changeset_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY nodes
    ADD CONSTRAINT nodes_changeset_id_fkey FOREIGN KEY (changeset_id) REFERENCES changesets(id);


--
-- Name: nodes_redaction_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY nodes
    ADD CONSTRAINT nodes_redaction_id_fkey FOREIGN KEY (redaction_id) REFERENCES redactions(id);


--
-- Name: note_comments_author_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY note_comments
    ADD CONSTRAINT note_comments_author_id_fkey FOREIGN KEY (author_id) REFERENCES users(id);


--
-- Name: note_comments_note_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY note_comments
    ADD CONSTRAINT note_comments_note_id_fkey FOREIGN KEY (note_id) REFERENCES notes(id);


--
-- Name: oauth_tokens_client_application_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY oauth_tokens
    ADD CONSTRAINT oauth_tokens_client_application_id_fkey FOREIGN KEY (client_application_id) REFERENCES client_applications(id);


--
-- Name: oauth_tokens_user_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY oauth_tokens
    ADD CONSTRAINT oauth_tokens_user_id_fkey FOREIGN KEY (user_id) REFERENCES users(id);


--
-- Name: redactions_user_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY redactions
    ADD CONSTRAINT redactions_user_id_fkey FOREIGN KEY (user_id) REFERENCES users(id);


--
-- Name: relation_members_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY relation_members
    ADD CONSTRAINT relation_members_id_fkey FOREIGN KEY (relation_id, version) REFERENCES relations(relation_id, version);


--
-- Name: relation_tags_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY relation_tags
    ADD CONSTRAINT relation_tags_id_fkey FOREIGN KEY (relation_id, version) REFERENCES relations(relation_id, version);


--
-- Name: relations_changeset_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY relations
    ADD CONSTRAINT relations_changeset_id_fkey FOREIGN KEY (changeset_id) REFERENCES changesets(id);


--
-- Name: relations_redaction_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY relations
    ADD CONSTRAINT relations_redaction_id_fkey FOREIGN KEY (redaction_id) REFERENCES redactions(id);


--
-- Name: user_blocks_moderator_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY user_blocks
    ADD CONSTRAINT user_blocks_moderator_id_fkey FOREIGN KEY (creator_id) REFERENCES users(id);


--
-- Name: user_blocks_revoker_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY user_blocks
    ADD CONSTRAINT user_blocks_revoker_id_fkey FOREIGN KEY (revoker_id) REFERENCES users(id);


--
-- Name: user_blocks_user_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY user_blocks
    ADD CONSTRAINT user_blocks_user_id_fkey FOREIGN KEY (user_id) REFERENCES users(id);


--
-- Name: user_preferences_user_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY user_preferences
    ADD CONSTRAINT user_preferences_user_id_fkey FOREIGN KEY (user_id) REFERENCES users(id);


--
-- Name: user_roles_granter_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY user_roles
    ADD CONSTRAINT user_roles_granter_id_fkey FOREIGN KEY (granter_id) REFERENCES users(id);


--
-- Name: user_roles_user_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY user_roles
    ADD CONSTRAINT user_roles_user_id_fkey FOREIGN KEY (user_id) REFERENCES users(id);


--
-- Name: user_tokens_user_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY user_tokens
    ADD CONSTRAINT user_tokens_user_id_fkey FOREIGN KEY (user_id) REFERENCES users(id);


--
-- Name: way_nodes_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY way_nodes
    ADD CONSTRAINT way_nodes_id_fkey FOREIGN KEY (way_id, version) REFERENCES ways(way_id, version);


--
-- Name: way_tags_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY way_tags
    ADD CONSTRAINT way_tags_id_fkey FOREIGN KEY (way_id, version) REFERENCES ways(way_id, version);


--
-- Name: ways_changeset_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY ways
    ADD CONSTRAINT ways_changeset_id_fkey FOREIGN KEY (changeset_id) REFERENCES changesets(id);


--
-- Name: ways_redaction_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: hoot
--

ALTER TABLE ONLY ways
    ADD CONSTRAINT ways_redaction_id_fkey FOREIGN KEY (redaction_id) REFERENCES redactions(id);


--
-- Name: public; Type: ACL; Schema: -; Owner: postgres
--

REVOKE ALL ON SCHEMA public FROM PUBLIC;
REVOKE ALL ON SCHEMA public FROM postgres;
GRANT ALL ON SCHEMA public TO postgres;
GRANT ALL ON SCHEMA public TO PUBLIC;


--
-- PostgreSQL database dump complete
--

