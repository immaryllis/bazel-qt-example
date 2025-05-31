load("@rules_qt//:qt.bzl", "qt_cc_binary", "qt_cc_library")

qt_cc_library(
    name = "lib",
    srcs = [
        "mainwindow.cpp",
    ],
    hdrs = ["mainwindow.h"],
    deps = [
        "@rules_qt//:qt_core",
        "@rules_qt//:qt_gui",
        "@rules_qt//:qt_widgets",
        "@rules_qt//:qt_sql",
    ],
    linkopts = ["-Wl,--allow-multiple-definition"],
)

qt_cc_binary(
    name = "linguine",
    srcs = ["main.cpp"],
    deps = [
        ":lib",
        "@rules_qt//:qt_core",
        "@rules_qt//:qt_widgets",
        "@rules_qt//:qt_sql",
    ],
)
