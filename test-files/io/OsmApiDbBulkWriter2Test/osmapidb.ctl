#
# sample_csv.ctl -- Control file to load CSV input data
#
#    Copyright (c) 2007-2017, NIPPON TELEGRAPH AND TELEPHONE CORPORATION
#
OUTPUT = current_nodes                   # [<schema_name>.]table_name
#INPUT = /home/vagrant/hoot/test-files/io/OsmApiDbBulkWriter2Test/osmapi_test1.csv  # Input data location (absolute path)
INPUT = /fouo/hoot-tests/stress-tests.child/postgres-dump-file-writer-performance.child/tmp/rio_buildings-cropped-current-nodes.csv
TYPE = CSV                            # Input file type
QUOTE = "\""                          # Quoting character
ESCAPE = \                            # Escape character for Quoting
DELIMITER = ","                       # Delimiter
#NULL = 
#FORCE_NOT_NULL = 

#WRITER = DIRECT				#DIRECT|BUFFERED|BINARY|PARALLEL
#SKIP = 0
#LIMIT = INFINITE
#ENCODING=
#CHECK_CONSTRAINTS = NO
#PARSE_ERRORS = 0
#DUPLICATE_ERRORS = 0
#ON_DUPLICATE_KEEP = NEW
LOGFILE = /home/vagrant/pg_bulkload/bin/pg_bulkload_osmapidb.log
#PARSE_BADFILE = 
#DUPLICATE_BADFILE = 
#TRUNCATE = NO
#VERBOSE = NO
#MULTI_PROCESS = NO
