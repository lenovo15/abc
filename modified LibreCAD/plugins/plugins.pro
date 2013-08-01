#-------------------------------------------------
#
# Project created by QtCreator 2011-03-22T19:33:11
#
#-------------------------------------------------

TEMPLATE = subdirs
TARGET = plugins

QT += gui

SUBDIRS     = \
        asciifile \
        align \
        list \
        sameprop \
        importshp \
        tank\
        point\
        arc\
        column\
        civil\
        line


TRANSLATIONS =


# install
INSTALLDIR = ../unix/resources/plugins
win32 {
    INSTALLDIR = ../windows/resources/plugins
}
unix {
    macx { 
	INSTALLDIR = ../LibreCAD.app/Contents/Resources/plugins
    }
    else { 
	INSTALLDIR = ../unix/resources/plugins
    }
}


