#ifndef QLABELLING_CONFIG_HPP
#define QLABELLING_CONFIG_HPP

#include <QColor>

//
//  Caution, this is the only QLabelling header that is guarenteed
//  to change with every QLabelling release, including this header
//  will cause a recompile every time a new QLabelling version is
//  released.
//
//  QLABELLING_VERSION % 100 is the patch level
//  QLABELLING_VERSION / 100 % 1000 is the minor version
//  QLABELLING_VERSION / 100000 is the major version

#define QLABELLING_VERSION 100000

//
//  QLABELLING_VERSION_STRING must be defined to be the same as QLABELLING_VERSION
//  but as a *string* in the form "x_y[_z]" where x is the major version
//  number, y is the minor version number, and z is the patch level if not 0.

#define QLABELLING_VERSION_STRING "1_0_0"

//
//  QLABELLING_ORGANIZATION_STRING defines the Organization name used in settings and About

#define QLABELLING_ORGANIZATION_STRING "IMAGINE"

//
//  QLABELLING_NAME_STRING defines the Application name used in settings and About

#define QLABELLING_NAME_STRING "QLabelling"

//
//  QLABELLING_DOMAIN_STRING defines the Application domain used in settings and About

#define QLABELLING_DOMAIN_STRING "imagine.enpc.fr"

//
//  QLABELLING_TOOLBUTTON_ICON_SIZE: guess what it defines?

#define QLABELLING_TOOLBUTTON_ICON_SIZE 16

//
//  QLABELLING_TOOLBUTTON_STYLESHEET_STRING: guess what it defines?

#define QLABELLING_TOOLBUTTON_STYLESHEET_STRING "QToolButton{ background: %1; border: 2px solid #8f8f91; border-radius: 6px;}"

//
//  QLABELLING_UNKNOW_LABEL_STRING

#define QLABELLING_UNKNOW_LABEL_STRING "UNKNOW"

//
//  QLABELLING_UNKNOW_LABEL_BRUSH_COLOR

#define QLABELLING_UNKNOW_LABEL_BRUSH_COLOR QColor(0,0,0,0)

#endif // QLABELLING_CONFIG_HPP
