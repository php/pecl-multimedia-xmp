/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2014 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Boro Sitnikovski (buritomath@yahoo.com)                      |
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

PHP_FUNCTION(xmp_get_format_list);
PHP_FUNCTION(xmp_create_context);
PHP_FUNCTION(xmp_free_context);
PHP_FUNCTION(xmp_test_module);
PHP_FUNCTION(xmp_load_module);
PHP_FUNCTION(xmp_load_module_from_memory);
PHP_FUNCTION(xmp_release_module);
PHP_FUNCTION(xmp_get_module_info);
PHP_FUNCTION(xmp_start_player);
PHP_FUNCTION(xmp_play_frame);
PHP_FUNCTION(xmp_get_frame_info);
PHP_FUNCTION(xmp_play_buffer);
PHP_FUNCTION(xmp_end_player);
PHP_FUNCTION(xmp_next_position);
PHP_FUNCTION(xmp_prev_position);
PHP_FUNCTION(xmp_set_position);
PHP_FUNCTION(xmp_stop_module);
PHP_FUNCTION(xmp_restart_module);
PHP_FUNCTION(xmp_seek_time);
PHP_FUNCTION(xmp_channel_mute);
PHP_FUNCTION(xmp_channel_vol);
PHP_FUNCTION(xmp_inject_event);
PHP_FUNCTION(xmp_set_instrument_path);
PHP_FUNCTION(xmp_get_player);
PHP_FUNCTION(xmp_set_player);
PHP_FUNCTION(xmp_start_smix);
PHP_FUNCTION(xmp_smix_play_instrument);
PHP_FUNCTION(xmp_smix_play_sample);
PHP_FUNCTION(xmp_smix_channel_pan);
PHP_FUNCTION(xmp_smix_load_sample);
PHP_FUNCTION(xmp_smix_release_sample);
PHP_FUNCTION(xmp_end_smix);

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
