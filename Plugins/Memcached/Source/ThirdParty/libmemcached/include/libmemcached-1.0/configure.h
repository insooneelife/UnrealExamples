/*
    +--------------------------------------------------------------------+
    | libmemcached-awesome - C/C++ Client Library for memcached          |
    +--------------------------------------------------------------------+
    | Redistribution and use in source and binary forms, with or without |
    | modification, are permitted under the terms of the BSD license.    |
    | You should have received a copy of the license in a bundled file   |
    | named LICENSE; in case you did not receive a copy you can review   |
    | the terms online at: https://opensource.org/licenses/BSD-3-Clause  |
    +--------------------------------------------------------------------+
    | Copyright (c) 2006-2014 Brian Aker   https://datadifferential.com/ |
    | Copyright (c) 2020-2021 Michael Wallner        https://awesome.co/ |
    +--------------------------------------------------------------------+
*/

#pragma once

#define LIBMEMCACHED_ENABLE_DEPRECATED 0
#define LIBMEMCACHED_WITH_SASL_SUPPORT 0

/* #undef HAVE_VISIBILITY */

/* #undef HAVE_NETDB_H */

/* #undef HAVE_IN_PORT_T */
/* #undef HAVE_PID_T */
/* #undef HAVE_SSIZE_T */

#define LIBMEMCACHED_AWESOME 1
#define LIBMEMCACHED_VERSION_STRING "1.1.4"
#define LIBMEMCACHED_VERSION_HEX    0x001001004

