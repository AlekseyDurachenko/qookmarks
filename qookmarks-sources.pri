INCLUDEPATH    +=                                                       \
    $$PWD/src                                                           \
    $$PWD/src/common                                                    \
    $$PWD/src/bookmarkimport                                            \
    $$PWD/src/project                                                   \
    $$PWD/src/network                                                   \
    $$PWD/src/manager                                                   \
    $$PWD/src/widget                                                    \
    $$PWD/src/widget/bookmarkview                                       \
    $$PWD/src/widget/navigationview                                     \
    $$PWD/src/widget/compositwidget                                     \
    $$PWD/src/widget/tageditdialog                                      \
    $$PWD/src/widget/bookmarkeditdialog                                 \


HEADERS        +=                                                       \
    $$PWD/src/bookmarkimport/bookmarkimportchromium.h                   \
    $$PWD/src/manager/cabstractbookmarkfilter.h                         \
    $$PWD/src/manager/cbookmark.h                                       \
    $$PWD/src/manager/cbookmarkfilter.h                                 \
    $$PWD/src/manager/cbookmarkfilterdatamodel.h                        \
    $$PWD/src/manager/cbookmarkitem.h                                   \
    $$PWD/src/manager/cbookmarkmgr.h                                    \
    $$PWD/src/manager/cmanager.h                                        \
    $$PWD/src/manager/ctag.h                                            \
    $$PWD/src/manager/ctagitem.h                                        \
    $$PWD/src/manager/ctagmgr.h                                         \
    $$PWD/src/widget/bookmarkview/cbookmarkfavoritesdelegate.h          \
    $$PWD/src/widget/bookmarkview/cbookmarkfilteritemmodel.h            \
    $$PWD/src/widget/bookmarkview/cbookmarkreadlaterdelegate.h          \
    $$PWD/src/widget/navigationview/cnavigationitemmodel.h              \
    $$PWD/src/consts.h                                                  \
    $$PWD/src/version.h                                                 \
    $$PWD/src/widget/navigationview/inavigationactions.h \
    $$PWD/src/widget/compositwidget/ccompositwidget.h \
    $$PWD/src/widget/navigationview/cnavigationview.h \
    $$PWD/src/widget/bookmarkview/cbookmarkview.h \
    $$PWD/src/project/cprj.h \
    $$PWD/src/project/cprjxml.h \
    $$PWD/src/cmainwindow.h \
    $$PWD/src/settings.h \
    $$PWD/src/network/cwebfiledownloader.h \
    $$PWD/src/network/cwebpagedownloader.h \
    $$PWD/src/network/cwebpagescreenshot.h \
    $$PWD/src/widget/tageditdialog/ctageditdialog.h \
    $$PWD/src/widget/bookmarkeditdialog/cbookmarkeditdialog.h \
    $$PWD/src/caboutdialog.h \
    $$PWD/src/common/cindexlist.h \
    $$PWD/src/network/cabstractdownloadreply.h \
    $$PWD/src/network/cabstractdownloadrequest.h \
    $$PWD/src/network/cdownloadtobytearrayrequest.h \
    $$PWD/src/network/cdownloadtobytearrayreply.h \
    $$PWD/src/network/cdownloadtofilerequest.h \
    $$PWD/src/network/cdownloadtofilereply.h \
    $$PWD/src/network/cdownloadmgr.h \
    $$PWD/src/network/ccheckurlrequest.h \
    $$PWD/src/network/ccheckurlreply.h


SOURCES        +=                                                       \
    $$PWD/src/bookmarkimport/bookmarkimportchromium.cpp                 \
    $$PWD/src/manager/cabstractbookmarkfilter.cpp                       \
    $$PWD/src/manager/cbookmark.cpp                                     \
    $$PWD/src/manager/cbookmarkfilter.cpp                               \
    $$PWD/src/manager/cbookmarkfilterdatamodel.cpp                      \
    $$PWD/src/manager/cbookmarkitem.cpp                                 \
    $$PWD/src/manager/cbookmarkmgr.cpp                                  \
    $$PWD/src/manager/cmanager.cpp                                      \
    $$PWD/src/manager/ctag.cpp                                          \
    $$PWD/src/manager/ctagitem.cpp                                      \
    $$PWD/src/manager/ctagmgr.cpp                                       \
    $$PWD/src/widget/bookmarkview/cbookmarkfavoritesdelegate.cpp        \
    $$PWD/src/widget/bookmarkview/cbookmarkfilteritemmodel.cpp          \
    $$PWD/src/widget/bookmarkview/cbookmarkreadlaterdelegate.cpp        \
    $$PWD/src/widget/navigationview/cnavigationitemmodel.cpp            \
    $$PWD/src/version.cpp                                               \
    $$PWD/src/widget/compositwidget/ccompositwidget.cpp \
    $$PWD/src/widget/navigationview/cnavigationview.cpp \
    $$PWD/src/widget/bookmarkview/cbookmarkview.cpp \
    $$PWD/src/project/cprj.cpp \
    $$PWD/src/project/cprjxml.cpp \
    $$PWD/src/cmainwindow.cpp \
    $$PWD/src/network/cwebfiledownloader.cpp \
    $$PWD/src/network/cwebpagedownloader.cpp \
    $$PWD/src/network/cwebpagescreenshot.cpp \
    $$PWD/src/widget/tageditdialog/ctageditdialog.cpp \
    $$PWD/src/widget/bookmarkeditdialog/cbookmarkeditdialog.cpp \
    $$PWD/src/caboutdialog.cpp \
    $$PWD/src/common/cindexlist.cpp \
    $$PWD/src/network/cabstractdownloadreply.cpp \
    $$PWD/src/network/cabstractdownloadrequest.cpp \
    $$PWD/src/network/cdownloadtobytearrayrequest.cpp \
    $$PWD/src/network/cdownloadtobytearrayreply.cpp \
    $$PWD/src/network/cdownloadtofilerequest.cpp \
    $$PWD/src/network/cdownloadtofilereply.cpp \
    $$PWD/src/network/cdownloadmgr.cpp \
    $$PWD/src/network/ccheckurlrequest.cpp \
    $$PWD/src/network/ccheckurlreply.cpp


FORMS          +=                                                       \
    $$PWD/src/cmainwindow.ui \
    $$PWD/src/widget/tageditdialog/ctageditdialog.ui \
    $$PWD/src/widget/bookmarkeditdialog/cbookmarkeditdialog.ui \
    $$PWD/src/caboutdialog.ui


RESOURCES      +=                                                       \
    $$PWD/qrc/about.qrc                                                 \
    $$PWD/qrc/icons.qrc                                                 \


OTHER_FILES    +=                                                       \
    $$PWD/ABOUT                                                         \
    $$PWD/AUTHORS                                                       \
    $$PWD/CHANGELOG                                                     \
    $$PWD/LIBRARIES                                                     \
    $$PWD/LICENSE                                                       \
    $$PWD/LICENSE.GPL-3+                                                \
    $$PWD/README.md


!contains(QT, testlib) {
    HEADERS   +=                                                        \

    SOURCES   +=                                                        \
        $$PWD/src/main.cpp                                              \

}

