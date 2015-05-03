# release rules
build_pass:CONFIG(release, debug|release):unix {
    LIBS +=
    CONFIG += link_pkgconfig
    PKGCONFIG += libxml-2.0
}


# debug rules
build_pass:CONFIG(debug, debug|release):unix {
    LIBS +=
    CONFIG += link_pkgconfig
    PKGCONFIG += libxml-2.0
}
