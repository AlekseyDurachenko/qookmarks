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
    $$PWD/src/network/cwebfiledownloader.h \
    $$PWD/src/network/cwebpagedownloader.h \
    $$PWD/src/network/cwebpagescreenshot.h \
    $$PWD/src/network/cabstractdownloadreply.h \
    $$PWD/src/network/cabstractdownloadrequest.h \
    $$PWD/src/network/cdownloadtobytearrayrequest.h \
    $$PWD/src/network/cdownloadtobytearrayreply.h \
    $$PWD/src/network/cdownloadtofilerequest.h \
    $$PWD/src/network/cdownloadtofilereply.h \
    $$PWD/src/network/cdownloadmgr.h \
    $$PWD/src/network/ccheckurlrequest.h \
    $$PWD/src/network/ccheckurlreply.h \
    $$PWD/src/network/ciconmgr.h \
    $$PWD/src/common/hash_functions.h \
    $$PWD/src/common/singleton.h \
    $$PWD/src/network/cnetworkmgr.h \
    $$PWD/src/network/cdownloadfaviconrequest.h \
    $$PWD/src/network/cdownloadfaviconreply.h \
    $$PWD/src/network/cdownloadwebpageinforeply.h \
    $$PWD/src/network/cdownloadwebpageinforequest.h \
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
    $$PWD/src/manager/tagsortfilterproxymodel.h


SOURCES        +=                                                       \
    $$PWD/src/bookmarkimport/bookmarkimportchromium.cpp                 \
    $$PWD/src/version.cpp                                               \
    $$PWD/src/network/cwebfiledownloader.cpp \
    $$PWD/src/network/cwebpagedownloader.cpp \
    $$PWD/src/network/cwebpagescreenshot.cpp \
    $$PWD/src/network/cabstractdownloadreply.cpp \
    $$PWD/src/network/cabstractdownloadrequest.cpp \
    $$PWD/src/network/cdownloadtobytearrayrequest.cpp \
    $$PWD/src/network/cdownloadtobytearrayreply.cpp \
    $$PWD/src/network/cdownloadtofilerequest.cpp \
    $$PWD/src/network/cdownloadtofilereply.cpp \
    $$PWD/src/network/cdownloadmgr.cpp \
    $$PWD/src/network/ccheckurlrequest.cpp \
    $$PWD/src/network/ccheckurlreply.cpp \
    $$PWD/src/network/ciconmgr.cpp \
    $$PWD/src/common/hash_functions.cpp \
    $$PWD/src/network/cnetworkmgr.cpp \
    $$PWD/src/network/cdownloadfaviconrequest.cpp \
    $$PWD/src/network/cdownloadfaviconreply.cpp \
    $$PWD/src/network/cdownloadwebpageinforeply.cpp \
    $$PWD/src/network/cdownloadwebpageinforequest.cpp \
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
    $$PWD/src/manager/bookmarkitem.cpp


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

