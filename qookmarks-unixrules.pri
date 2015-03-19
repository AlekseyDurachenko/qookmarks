# release rules
build_pass:CONFIG(release, debug|release):unix {
    LIBS += 
}


# debug rules
build_pass:CONFIG(debug, debug|release):unix {
    LIBS += 
}
