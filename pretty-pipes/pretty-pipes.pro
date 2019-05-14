
exists ("../Configure.pri") {
    include("../Configure.pri")
}

TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS = \
    pp-lib \
    pp-bin \
    example \

