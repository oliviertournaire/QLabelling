message(STATUS)
message(STATUS "[Begin] QLabelling install cmake")

include(BundleUtilities)

install( TARGETS QArrangementLabelling
         BUNDLE DESTINATION . COMPONENT Application
         RUNTIME DESTINATION bin COMPONENT Application )

# See documentation at: http://www.cmake.org/cmake/help/v2.8.10/cmake.html#module:DeployQt4
include(DeployQt4)
install_qt4_executable( "bin/QArrangementLabelling.exe"
                        ""                            # <qt_plugins>
                        ""                            # <libs>
                        ""                            # <dirs>
                        ""                            # <plugins_dir>
                        "False"                       # <request_qt_conf>
                        "Application" # <component>
                      )

message(STATUS "* QLabelling install directory:")
message(STATUS "    # " ${CMAKE_INSTALL_PREFIX})

message(STATUS "[End] QLabelling install cmake")
message(STATUS)