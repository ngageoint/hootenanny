
MAKEFILE = Makefile.qmake

TEMPLATE = subdirs

CONFIG += ordered

include(Configure.pri)

SUBDIRS += \
    tbs \
    tgs \
    hoot-core \
    hoot-core-test \

nodejs {
SUBDIRS += \
    hoot-js
}

hadoop {
SUBDIRS += \
    pretty-pipes \
    hoot-hadoop \
}

swig {
SUBDIRS += \
    hoot-swig
}

rnd {
SUBDIRS += hoot-rnd
}

SUBDIRS += \
    hoot-cmd \

cppunit {
SUBDIRS += \
    hoot-test \
}
