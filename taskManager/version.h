/*
 * version.h
 *
 *  Created on: 2018/5/22
 *      Author: wyb
 */

#ifndef _VERSION_H_
#define _VERSION_H_

#define VERSION_MAJOR   0
#define VERSION_MINOR   1
#define VERSION_MICRO   1

#define VERSION_INT(a, b, c) (a << 16 | b << 8 | c)
#define VERSION_DOT(a, b, c) a##.##b##.##c
#define VERSION(a, b, c) VERSION_DOT(a, b, c)

#define STRINGIFY(s) TOSTRING(s)
#define TOSTRING(s) #s

#define VERSION_STR STRINGIFY(VERSION(VERSION_MAJOR, VERSION_MINOR, VERSION_MICRO))

#define VERSION_NUM VERSION_INT(VERSION_MAJOR, VERSION_MINOR, VERSION_MICRO)

#endif // !_VERSION_H_
