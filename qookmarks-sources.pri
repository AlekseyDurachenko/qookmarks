INCLUDEPATH    +=                                                       \
    $$PWD/src                                                           \
    $$PWD/src/common                                                    \
    $$PWD/src/bookmarkimport                                            \
    $$PWD/src/bookmarkimportwizard                                      \
    $$PWD/src/project                                                   \
    $$PWD/src/network                                                   \
    $$PWD/src/manager                                                   \
    $$PWD/src/browser                                                   \
    $$PWD/src/icontheme                                                 \
    $$PWD/src/widget                                                    \
    $$PWD/src/widget/aboutdialog                                        \
    $$PWD/src/widget/bookmarkview                                       \
    $$PWD/src/widget/navigationview                                     \
    $$PWD/src/widget/compositwidget                                     \
    $$PWD/src/widget/tageditdialog                                      \
    $$PWD/src/widget/bookmarkeditdialog                                 \


HEADERS        +=                                                       \
    $$PWD/src/bookmarkimport/bookmarkimportchromium.h                   \
    $$PWD/src/consts.h                                                  \
    $$PWD/src/version.h                                                 \
    $$PWD/src/settings.h \
    $$PWD/src/common/hash_functions.h \
    $$PWD/src/common/singleton.h \
    $$PWD/src/browser/browser.h \
    $$PWD/src/icontheme/icontheme.h \
    $$PWD/src/mainwindow.h \
    $$PWD/src/widget/aboutdialog/aboutdialog.h \
    $$PWD/src/widget/bookmarkeditdialog/bookmarkeditdialog.h \
    $$PWD/src/widget/tageditdialog/tageditdialog.h \
    $$PWD/src/widget/bookmarkview/bookmarkbooleanicondelegate.h \
    $$PWD/src/widget/bookmarkview/bookmarkheaderview.h \
    $$PWD/src/widget/bookmarkview/bookmarkitemmodel.h \
    $$PWD/src/widget/bookmarkview/bookmarkratingdelegate.h \
    $$PWD/src/widget/bookmarkview/bookmarkview.h \
    $$PWD/src/widget/navigationview/navanchoritemmodel.h \
    $$PWD/src/widget/navigationview/navigationview.h \
    $$PWD/src/widget/navigationview/navtagitemmodel.h \
    $$PWD/src/widget/navigationview/navtagview.h \
    $$PWD/src/bookmarkimportwizard/bookmarkimportwizard.h \
    $$PWD/src/common/indexlist.h \
    $$PWD/src/project/prj.h \
    $$PWD/src/project/prjxml.h \
    $$PWD/src/widget/navigationview/actinterface.h \
    $$PWD/src/manager/abstractbookmarkdatamodel.h \
    $$PWD/src/manager/abstractbookmarkfilter.h \
    $$PWD/src/manager/bookmark.h \
    $$PWD/src/manager/bookmarkfilter.h \
    $$PWD/src/manager/bookmarkfilterdatamodel.h \
    $$PWD/src/manager/bookmarkitem.h \
    $$PWD/src/manager/bookmarkmgr.h \
    $$PWD/src/manager/manager.h \
    $$PWD/src/manager/tag.h \
    $$PWD/src/manager/tagitem.h \
    $$PWD/src/manager/checkedtagitemmodel.h \
    $$PWD/src/manager/tagitemmodel.h \
    $$PWD/src/manager/tagmgr.h \
    $$PWD/src/manager/tagsortfilterproxymodel.h \
    $$PWD/src/network/abstractdownloadreply.h \
    $$PWD/src/network/abstractdownloadrequest.h \
    $$PWD/src/network/checkurlreply.h \
    $$PWD/src/network/checkurlrequest.h \
    $$PWD/src/network/downloadfaviconreply.h \
    $$PWD/src/network/downloadfaviconrequest.h \
    $$PWD/src/network/downloadmgr.h \
    $$PWD/src/network/downloadtobytearrayreply.h \
    $$PWD/src/network/downloadtobytearrayrequest.h \
    $$PWD/src/network/downloadtofilereply.h \
    $$PWD/src/network/downloadtofilerequest.h \
    $$PWD/src/network/downloadwebpageinforeply.h \
    $$PWD/src/network/downloadwebpageinforequest.h \
    $$PWD/src/network/iconmgr.h \
    $$PWD/src/network/networkmgr.h \
    $$PWD/src/network/webfiledownloader.h \
    $$PWD/src/network/webpagedownloader.h \
    $$PWD/src/network/webpagescreenshot.h


SOURCES        +=                                                       \
    $$PWD/src/bookmarkimport/bookmarkimportchromium.cpp                 \
    $$PWD/src/version.cpp                                               \
    $$PWD/src/common/hash_functions.cpp \
    $$PWD/src/browser/browser.cpp \
    $$PWD/src/icontheme/icontheme.cpp \
    $$PWD/src/mainwindow.cpp \
    $$PWD/src/widget/aboutdialog/aboutdialog.cpp \
    $$PWD/src/widget/bookmarkeditdialog/bookmarkeditdialog.cpp \
    $$PWD/src/widget/tageditdialog/tageditdialog.cpp \
    $$PWD/src/widget/bookmarkview/bookmarkbooleanicondelegate.cpp \
    $$PWD/src/widget/bookmarkview/bookmarkheaderview.cpp \
    $$PWD/src/widget/bookmarkview/bookmarkitemmodel.cpp \
    $$PWD/src/widget/bookmarkview/bookmarkratingdelegate.cpp \
    $$PWD/src/widget/bookmarkview/bookmarkview.cpp \
    $$PWD/src/widget/navigationview/navanchoritemmodel.cpp \
    $$PWD/src/widget/navigationview/navtagview.cpp \
    $$PWD/src/widget/navigationview/navtagitemmodel.cpp \
    $$PWD/src/widget/navigationview/navigationview.cpp \
    $$PWD/src/bookmarkimportwizard/bookmarkimportwizard.cpp \
    $$PWD/src/common/indexlist.cpp \
    $$PWD/src/project/prj.cpp \
    $$PWD/src/project/prjxml.cpp \
    $$PWD/src/manager/tagsortfilterproxymodel.cpp \
    $$PWD/src/manager/tagmgr.cpp \
    $$PWD/src/manager/tagitemmodel.cpp \
    $$PWD/src/manager/tagitem.cpp \
    $$PWD/src/manager/tag.cpp \
    $$PWD/src/manager/manager.cpp \
    $$PWD/src/manager/checkedtagitemmodel.cpp \
    $$PWD/src/manager/bookmarkmgr.cpp \
    $$PWD/src/manager/abstractbookmarkdatamodel.cpp \
    $$PWD/src/manager/abstractbookmarkfilter.cpp \
    $$PWD/src/manager/bookmark.cpp \
    $$PWD/src/manager/bookmarkfilter.cpp \
    $$PWD/src/manager/bookmarkfilterdatamodel.cpp \
    $$PWD/src/manager/bookmarkitem.cpp \
    $$PWD/src/network/abstractdownloadreply.cpp \
    $$PWD/src/network/abstractdownloadrequest.cpp \
    $$PWD/src/network/checkurlreply.cpp \
    $$PWD/src/network/checkurlrequest.cpp \
    $$PWD/src/network/downloadfaviconreply.cpp \
    $$PWD/src/network/downloadfaviconrequest.cpp \
    $$PWD/src/network/downloadmgr.cpp \
    $$PWD/src/network/downloadtobytearrayreply.cpp \
    $$PWD/src/network/downloadtobytearrayrequest.cpp \
    $$PWD/src/network/downloadtofilereply.cpp \
    $$PWD/src/network/downloadtofilerequest.cpp \
    $$PWD/src/network/downloadwebpageinforeply.cpp \
    $$PWD/src/network/downloadwebpageinforequest.cpp \
    $$PWD/src/network/iconmgr.cpp \
    $$PWD/src/network/networkmgr.cpp \
    $$PWD/src/network/webfiledownloader.cpp \
    $$PWD/src/network/webpagedownloader.cpp \
    $$PWD/src/network/webpagescreenshot.cpp


FORMS          +=                                                       \
    $$PWD/src/widget/aboutdialog/aboutdialog.ui \
    $$PWD/src/widget/bookmarkeditdialog/bookmarkeditdialog.ui \
    $$PWD/src/widget/tageditdialog/tageditdialog.ui


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

