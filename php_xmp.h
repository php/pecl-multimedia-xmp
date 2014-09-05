/*
  +----------------------------------------------------------------------+
  | xmp                                                                  |
  +----------------------------------------------------------------------+
  | This source file is subject to version 2.0 of the Apache license,    |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.apache.org/licenses/LICENSE-2.0.html                      |
  | If you did not receive a copy of the Apache2.0 license and are unable|
  | to obtain it through the world-wide-web, please send a note to       |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Boro Sitnikovski <buritomath@yahoo.com>                      |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifndef PHP_XMP_H
#define PHP_XMP_H

#include <xmp.h>
#include <php.h>

extern zend_module_entry xmp_module_entry;
#define phpext_xmp_ptr &xmp_module_entry

#define PHP_XMP_VERSION "4.2.0"

PHP_MINIT_FUNCTION(xmp);
PHP_MSHUTDOWN_FUNCTION(xmp);
PHP_MINFO_FUNCTION(xmp);

extern zend_module_entry xmp_module_entry;

#ifdef PHP_WIN32
#	define PHP_XMP_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_XMP_API __attribute__ ((visibility("default")))
#else
#	define PHP_XMP_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

#ifdef ZTS
#define XMP_G(v) TSRMG(xmp_globals_id, zend_xmp_globals *, v)
#else
#define XMP_G(v) (xmp_globals.v)
#endif

#endif	/* PHP_XMP_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
