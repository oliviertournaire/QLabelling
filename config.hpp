#ifndef QLABELLING_CONFIG_HPP
#define QLABELLING_CONFIG_HPP

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

#endif // QLABELLING_CONFIG_HPP
