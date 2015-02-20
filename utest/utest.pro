include(../qmake/config.pri)

TEMPLATE = app

CONFIG(debug, debug|release) {
  TARGET_SUFFIX = _d
} else {
  TARGET_SUFFIX =
}
TARGET = utest$$TARGET_SUFFIX

CONFIG += console
QT += testlib  gui script

HEADERS += \
    $$PWD/test_cpptools.h \
    $$PWD/test_qttools_core.h \
    $$PWD/test_qttools_gui.h \
    $$PWD/test_qttools_script.h \
    \
    $$PWD/../src/qttools/core/qlocale_tools.h \
    $$PWD/../src/qttools/core/wait_loop.h \
    $$PWD/../src/qttools/core/wait_loop_stop_condition.h \
    $$PWD/../src/qttools/core/internal/wait_loop_p.h \
    $$PWD/../src/qttools/core/internal/wait_loop_time_out_stop_condition.h \
    \
    $$PWD/../src/qttools/gui/qstandard_item_explorer.h \
    \
    $$PWD/../src/qttools/script/calculator.h

SOURCES += \
    $$PWD/main.cpp \
    $$PWD/test_cpptools.cpp \
    $$PWD/test_qttools_core.cpp \
    $$PWD/test_qttools_gui.cpp \
    $$PWD/test_qttools_script.cpp \
    \
    $$PWD/../src/qttools/core/qlocale_tools.cpp \
    $$PWD/../src/qttools/core/wait_loop.cpp \
    $$PWD/../src/qttools/core/wait_loop_stop_condition.cpp \
    $$PWD/../src/qttools/core/internal/wait_loop_p.cpp \
    $$PWD/../src/qttools/core/internal/wait_loop_time_out_stop_condition.cpp \
    \
    $$PWD/../src/qttools/gui/qstandard_item_explorer.cpp \
    \
    $$PWD/../src/qttools/script/calculator.cpp

occtools {
    DEFINES += FOUGTOOLS_HAVE_OCCTOOLS

    include(../src/occtools/occ.pri)

    HEADERS += \
        $$PWD/test_occtools.h \
        $$PWD/../src/occtools/io.h

    SOURCES += \
        $$PWD/test_occtools.cpp \
        $$PWD/../src/occtools/io.cpp

    LIBS += -lTKBRep -lTKernel -lTKG2d -lTKG3d -lTKGeomAlgo -lTKGeomBase \
          -lTKIGES -lTKMath -lTKPrim -lTKService -lTKShHealing \
          -lTKSTEP -lTKSTEPAttr -lTKSTEPBase -lTKSTEP209 -lTKSTL -lTKTopAlgo \
          -lTKXSBase
}
