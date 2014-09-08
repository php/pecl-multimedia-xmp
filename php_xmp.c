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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "php_xmp.h"

/* True global resources - no need for thread safety here */
static int le_xmp;

#ifdef COMPILE_DL_XMP
ZEND_GET_MODULE(xmp)
#endif

/* {{{ xmp_get_format_list()
 * Returns an array of supported formats */
PHP_FUNCTION(xmp_get_format_list)
{
	char **formats;
	int i = 0;

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	formats = xmp_get_format_list();

	array_init(return_value);

	while (formats[i]) {
		add_next_index_string(return_value, formats[i], 1);
		i++;
	}
}
/* }}} */

/* {{{ xmp_create_context()
 * Returns a resource on success */
PHP_FUNCTION(xmp_create_context)
{
	xmp_context *xmp_ptr, xmp;

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	xmp = xmp_create_context();

	if (xmp) {
		xmp_ptr = emalloc(sizeof(xmp_context));
		memcpy(xmp_ptr, &xmp, sizeof(xmp_context));
		ZEND_REGISTER_RESOURCE(return_value, xmp_ptr, le_xmp);
	} else {
		RETURN_FALSE;
	}
}
/* }}} */

/* {{{ xmp_free_context(resource)
 * Frees the specified resource */
PHP_FUNCTION(xmp_free_context)
{
	xmp_context *xmp_ptr;
	zval *ctx;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &ctx) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(xmp_ptr, xmp_context *, &ctx, -1, "xmp_context", le_xmp);
	if (*xmp_ptr) {
		xmp_free_context(*xmp_ptr);
		*xmp_ptr = NULL;
	}

}
/* }}} */

/* {{{ xmp_test_module(path)
 * Retrieve name and module type */
PHP_FUNCTION(xmp_test_module)
{
	long ret;
	char *path;
	int path_len;
	struct xmp_test_info ti;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &path, &path_len) == FAILURE) {
		return;
	}

	ret = xmp_test_module(path, &ti);
	if (ret) {
		RETVAL_LONG(ret);
	} else {
		array_init(return_value);
		add_assoc_string_ex(return_value, "name", sizeof("name")-1, ti.name, 1);
		add_assoc_string_ex(return_value, "type", sizeof("type")-1, ti.type, 1);
	}
}
/* }}} */

/* {{{ xmp_load_module(resource, path)
 * Load module for the specified resource */
PHP_FUNCTION(xmp_load_module)
{
	xmp_context *xmp_ptr;
	zval *ctx;
	int ret;
	char *path;
	int path_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rs", &ctx, &path, &path_len) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(xmp_ptr, xmp_context *, &ctx, -1, "xmp_context", le_xmp);
	ret = xmp_load_module(*xmp_ptr, path);
	RETVAL_LONG(ret);
}
/* }}} */

/* {{{ xmp_load_module_from_memory(resource, string)
 * Load module for the specified resource from string */
PHP_FUNCTION(xmp_load_module_from_memory)
{
	xmp_context *xmp_ptr;
	zval *ctx;
	char *data;
	int ret, length;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rs", &ctx, &data, &length) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(xmp_ptr, xmp_context *, &ctx, -1, "xmp_context", le_xmp);
	ret = xmp_load_module_from_memory(*xmp_ptr, data, length);
	RETVAL_LONG(ret);
}
/* }}} */

/* {{{ xmp_release_module(resource)
 * Release module for the specified resource */
PHP_FUNCTION(xmp_release_module)
{
	zval *ctx;
	xmp_context *xmp_ptr;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &ctx) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(xmp_ptr, xmp_context *, &ctx, -1, "xmp_context", le_xmp);
	xmp_release_module(*xmp_ptr);
}
/* }}} */

/* {{{ xmp_get_module_info(resource)
 * Returns an array of information about the module */
PHP_FUNCTION(xmp_get_module_info)
{
	struct xmp_module_info mi;
	xmp_context *xmp_ptr;
	struct xmp_channel *xc;
	struct xmp_sample *xs;
	struct xmp_pattern *xp;
	struct xmp_track *xt;
	struct xmp_instrument *xi;
	zval *ctx, *mod = 0, *seq_data = 0, *xxc = 0, *cur = 0, *curr = 0, *currr = 0, *xxs = 0, *xxt = 0, *event = 0, *xxp = 0, *xxi = 0;
	int i;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &ctx) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(xmp_ptr, xmp_context *, &ctx, -1, "xmp_context", le_xmp);
	xmp_get_module_info(*xmp_ptr, &mi);

	array_init(return_value);
	add_assoc_stringl_ex(return_value, "md5", sizeof("md5")-1, (char *)mi.md5, 16, 1);
	add_assoc_long_ex(return_value, "vol_base", sizeof("vol_base")-1, mi.vol_base);

	MAKE_STD_ZVAL(mod);
	array_init(mod);
	{
		add_assoc_string_ex(mod, "name", sizeof("name")-1, mi.mod->name, 1);
		add_assoc_string_ex(mod, "type", sizeof("type")-1, mi.mod->type, 1);
		add_assoc_long_ex(mod, "pat", sizeof("pat")-1, mi.mod->pat);
		add_assoc_long_ex(mod, "trk", sizeof("trk")-1, mi.mod->trk);
		add_assoc_long_ex(mod, "chn", sizeof("chn")-1, mi.mod->chn);
		add_assoc_long_ex(mod, "ins", sizeof("ins")-1, mi.mod->ins);
		add_assoc_long_ex(mod, "smp", sizeof("smp")-1, mi.mod->smp);
		add_assoc_long_ex(mod, "spd", sizeof("spd")-1, mi.mod->spd);
		add_assoc_long_ex(mod, "bpm", sizeof("bpm")-1, mi.mod->bpm);
		add_assoc_long_ex(mod, "len", sizeof("len")-1, mi.mod->len);
		add_assoc_long_ex(mod, "rst", sizeof("rst")-1, mi.mod->rst);
		add_assoc_long_ex(mod, "gvl", sizeof("gvl")-1, mi.mod->gvl);

		MAKE_STD_ZVAL(xxp);
		array_init(xxp);
		{
			for (i=0;i<mi.mod->pat;i++) {
				MAKE_STD_ZVAL(cur);
				array_init(cur);
				xp = mi.mod->xxp[i];
				add_assoc_long_ex(cur, "rows", sizeof("rows")-1, xp->rows);
				add_assoc_long_ex(cur, "index", sizeof("index")-1, xp->index[0]);
				add_next_index_zval(xxp, cur);
			}
			add_assoc_zval_ex(mod, "xxp", sizeof("xxp")-1, xxp);
		}

		MAKE_STD_ZVAL(xxt);
		array_init(xxt);
		{
			for (i=0;i<mi.mod->trk;i++) {
				MAKE_STD_ZVAL(cur);
				array_init(cur);
				xt = mi.mod->xxt[i];
				add_assoc_long_ex(cur, "rows", sizeof("rows")-1, xt->rows);
				MAKE_STD_ZVAL(event);
				array_init(event);
				{
					add_assoc_long_ex(event, "note", sizeof("note")-1, xt->event[0].note);
					add_assoc_long_ex(event, "ins", sizeof("ins")-1, xt->event[0].ins);
					add_assoc_long_ex(event, "vol", sizeof("vol")-1, xt->event[0].vol);
					add_assoc_long_ex(event, "fxt", sizeof("fxt")-1, xt->event[0].fxt);
					add_assoc_long_ex(event, "fxp", sizeof("fxp")-1, xt->event[0].fxp);
					add_assoc_long_ex(event, "f2t", sizeof("f2t")-1, xt->event[0].f2t);
					add_assoc_long_ex(event, "f2p", sizeof("f2p")-1, xt->event[0].f2p);
					add_assoc_long_ex(event, "_flag", sizeof("_flag")-1, xt->event[0]._flag);
				}
				add_next_index_zval(cur, event);
				add_next_index_zval(xxt, cur);
			}
			add_assoc_zval_ex(mod, "xxt", sizeof("xxt")-1, xxt);
		}

		MAKE_STD_ZVAL(xxi);
		array_init(xxi);
		{
			for (i=0;i<mi.mod->ins;i++) {
				xi = &mi.mod->xxi[i];
				MAKE_STD_ZVAL(cur);
				array_init(cur);
				add_assoc_string_ex(cur, "name", sizeof("name")-1, xi->name, 1);
				add_assoc_long_ex(cur, "vol", sizeof("vol")-1, xi->vol);
				add_assoc_long_ex(cur, "nsm", sizeof("nsm")-1, xi->nsm);
				add_assoc_long_ex(cur, "rls", sizeof("rls")-1, xi->rls);
				MAKE_STD_ZVAL(curr);
				array_init(curr);
				{
					add_assoc_long_ex(curr, "flg", sizeof("flg")-1, xi->aei.flg);
					add_assoc_long_ex(curr, "npt", sizeof("npt")-1, xi->aei.npt);
					add_assoc_long_ex(curr, "scl", sizeof("scl")-1, xi->aei.scl);
					add_assoc_long_ex(curr, "sus", sizeof("sus")-1, xi->aei.sus);
					add_assoc_long_ex(curr, "sue", sizeof("sue")-1, xi->aei.sue);
					add_assoc_long_ex(curr, "lps", sizeof("lps")-1, xi->aei.lps);
					add_assoc_long_ex(curr, "lpe", sizeof("lpe")-1, xi->aei.lpe);
					add_assoc_stringl_ex(curr, "data", sizeof("data")-1, (char *)xi->aei.data, XMP_MAX_ENV_POINTS * 2, 1);
					add_assoc_zval_ex(cur, "aei", sizeof("aei")-1, curr);
				}
				MAKE_STD_ZVAL(curr);
				array_init(curr);
				{
					add_assoc_long_ex(curr, "flg", sizeof("flg")-1, xi->pei.flg);
					add_assoc_long_ex(curr, "npt", sizeof("npt")-1, xi->pei.npt);
					add_assoc_long_ex(curr, "scl", sizeof("scl")-1, xi->pei.scl);
					add_assoc_long_ex(curr, "sus", sizeof("sus")-1, xi->pei.sus);
					add_assoc_long_ex(curr, "sue", sizeof("sue")-1, xi->pei.sue);
					add_assoc_long_ex(curr, "lps", sizeof("lps")-1, xi->pei.lps);
					add_assoc_long_ex(curr, "lpe", sizeof("lpe")-1, xi->pei.lpe);
					add_assoc_stringl_ex(curr, "data", sizeof("data")-1, (char *)xi->pei.data, XMP_MAX_ENV_POINTS * 2, 1);
					add_assoc_zval_ex(cur, "pei", sizeof("pei")-1, curr);
				}
				MAKE_STD_ZVAL(curr);
				array_init(curr);
				{
					add_assoc_long_ex(curr, "flg", sizeof("flg")-1, xi->fei.flg);
					add_assoc_long_ex(curr, "npt", sizeof("npt")-1, xi->fei.npt);
					add_assoc_long_ex(curr, "scl", sizeof("scl")-1, xi->fei.scl);
					add_assoc_long_ex(curr, "sus", sizeof("sus")-1, xi->fei.sus);
					add_assoc_long_ex(curr, "sue", sizeof("sue")-1, xi->fei.sue);
					add_assoc_long_ex(curr, "lps", sizeof("lps")-1, xi->fei.lps);
					add_assoc_long_ex(curr, "lpe", sizeof("lpe")-1, xi->fei.lpe);
					add_assoc_stringl_ex(curr, "data", sizeof("data")-1, (char *)xi->fei.data, XMP_MAX_ENV_POINTS * 2, 1);
					add_assoc_zval_ex(cur, "fei", sizeof("fei")-1, curr);
				}
				MAKE_STD_ZVAL(curr);
				array_init(curr);
				{
					for (i=0;i<XMP_MAX_KEYS;i++) {
						MAKE_STD_ZVAL(currr);
						array_init(currr);
						add_assoc_long_ex(currr, "ins", sizeof("ins")-1, xi->map[i].ins);
						add_assoc_long_ex(currr, "xpo", sizeof("xpc")-1, xi->map[i].xpo);
						add_next_index_zval(curr, currr);
					}
					add_assoc_zval_ex(cur, "map", sizeof("map")-1, curr);
				}
				MAKE_STD_ZVAL(curr);
				array_init(curr);
				{
					add_assoc_long_ex(curr, "vol", sizeof("vol")-1, xi->sub->vol);
					add_assoc_long_ex(curr, "gvl", sizeof("gvl")-1, xi->sub->gvl);
					add_assoc_long_ex(curr, "pan", sizeof("pan")-1, xi->sub->pan);
					add_assoc_long_ex(curr, "xpo", sizeof("xpo")-1, xi->sub->xpo);
					add_assoc_long_ex(curr, "fin", sizeof("fin")-1, xi->sub->fin);
					add_assoc_long_ex(curr, "vwf", sizeof("vwf")-1, xi->sub->vwf);
					add_assoc_long_ex(curr, "vde", sizeof("vde")-1, xi->sub->vde);
					add_assoc_long_ex(curr, "vra", sizeof("vra")-1, xi->sub->vra);
					add_assoc_long_ex(curr, "vsw", sizeof("vsw")-1, xi->sub->vsw);
					add_assoc_long_ex(curr, "rvv", sizeof("rvv")-1, xi->sub->rvv);
					add_assoc_long_ex(curr, "sid", sizeof("sid")-1, xi->sub->sid);
					add_assoc_long_ex(curr, "nna", sizeof("nna")-1, xi->sub->nna);
					add_assoc_long_ex(curr, "dct", sizeof("dct")-1, xi->sub->dct);
					add_assoc_long_ex(curr, "dca", sizeof("dca")-1, xi->sub->dca);
					add_assoc_long_ex(curr, "ifc", sizeof("ifc")-1, xi->sub->ifc);
					add_assoc_long_ex(curr, "ifr", sizeof("ifr")-1, xi->sub->ifr);
					add_assoc_zval_ex(cur, "sub", sizeof("sub")-1, curr);
				}
				add_next_index_zval(xxi, cur);
			}
			add_assoc_zval_ex(mod, "xxi", sizeof("xxi")-1, xxi);
		}

		MAKE_STD_ZVAL(xxs);
		array_init(xxs);
		{
			xs = mi.mod->xxs;
			add_assoc_string_ex(xxs, "name", sizeof("name")-1, xs->name, 1);
			add_assoc_long_ex(xxs, "len", sizeof("len")-1, xs->len);
			add_assoc_long_ex(xxs, "lps", sizeof("lps")-1, xs->lps);
			add_assoc_long_ex(xxs, "lpe", sizeof("lpe")-1, xs->lpe);
			add_assoc_long_ex(xxs, "flg", sizeof("flg")-1, xs->flg);
			if (xs->data) {
				add_assoc_stringl_ex(xxs, "data", sizeof("data")-1, (char *)xs->data, xs->len, 1);
			} else {
				add_assoc_string_ex(xxs, "data", sizeof("data")-1, "", 1);
			}
			add_assoc_zval_ex(mod, "xxs", sizeof("xxs")-1, xxs);
		}

		MAKE_STD_ZVAL(xxc);
		array_init(xxc);
		{
			for (i=0;i<64;i++) {
				xc = &mi.mod->xxc[i];
				MAKE_STD_ZVAL(cur);
				array_init(cur);
				add_assoc_long_ex(cur, "pan", sizeof("pan")-1, xc->pan);
				add_assoc_long_ex(cur, "vol", sizeof("vol")-1, xc->vol);
				add_assoc_long_ex(cur, "flg", sizeof("flg")-1, xc->flg);
				add_next_index_zval(xxc, cur);
			}
			add_assoc_zval_ex(mod, "xxc", sizeof("xxc")-1, xxc);
		}

		add_assoc_string_ex(mod, "xxo", sizeof("xxo")-1, (char *)mi.mod->xxo, 1);
		add_assoc_zval_ex(return_value, "mod", sizeof("mod")-1, mod);
	}

	if (mi.comment) {
		add_assoc_string_ex(return_value, "comment", sizeof("comment")-1, mi.comment, 1);
	} else {
		add_assoc_string_ex(return_value, "comment", sizeof("comment")-1, "", 1);
	}
	add_assoc_long_ex(return_value, "num_sequences", sizeof("num_sequences")-1, mi.num_sequences);

	MAKE_STD_ZVAL(seq_data);
	array_init(seq_data);
	{
		add_assoc_long_ex(seq_data, "entry_point", sizeof("entry_point")-1, mi.seq_data->entry_point);
		add_assoc_long_ex(seq_data, "duration", sizeof("duration")-1, mi.seq_data->duration);
		add_assoc_zval_ex(return_value, "seq_data", sizeof("seq_data")-1, seq_data);
	}
}
/* }}} */

/* {{{ xmp_start_player(resource, rate, format)
 * Starts playing the currently loaded module */
PHP_FUNCTION(xmp_start_player)
{
	xmp_context *xmp_ptr;
	zval *ctx;
	long rate, format, ret;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rll", &ctx, &rate, &format) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(xmp_ptr, xmp_context *, &ctx, -1, "xmp_context", le_xmp);
	ret = xmp_start_player(*xmp_ptr, rate, format);

	RETVAL_LONG(ret);
}
/* }}} */

/* {{{ xmp_play_frame(resource)
 * Plays one frame from the currently loaded module */
PHP_FUNCTION(xmp_play_frame)
{
	xmp_context *xmp_ptr;
	zval *ctx;
	long ret;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &ctx) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(xmp_ptr, xmp_context *, &ctx, -1, "xmp_context", le_xmp);
	ret = xmp_play_frame(*xmp_ptr);

	RETVAL_LONG(ret);
}
/* }}} */

/* {{{ xmp_get_frame_info(resource)
 * Returns an array of information about the current frame */
PHP_FUNCTION(xmp_get_frame_info)
{
	struct xmp_frame_info fi;
	struct xmp_channel_info *ci;
	xmp_context *xmp_ptr;
	zval *ctx, *cur = 0, *event = 0, *channel_info = 0;
	int i;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &ctx) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(xmp_ptr, xmp_context *, &ctx, -1, "xmp_context", le_xmp);
	xmp_get_frame_info(*xmp_ptr, &fi);

	array_init(return_value);
	add_assoc_long_ex(return_value, "pos", sizeof("pos")-1, fi.pos);
	add_assoc_long_ex(return_value, "pattern", sizeof("pattern")-1, fi.pattern);
	add_assoc_long_ex(return_value, "row", sizeof("row")-1, fi.row);
	add_assoc_long_ex(return_value, "num_rows", sizeof("num_rows")-1, fi.num_rows);
	add_assoc_long_ex(return_value, "frame", sizeof("frame")-1, fi.frame);
	add_assoc_long_ex(return_value, "speed", sizeof("speed")-1, fi.speed);
	add_assoc_long_ex(return_value, "bpm", sizeof("bpm")-1, fi.bpm);
	add_assoc_long_ex(return_value, "time", sizeof("time")-1, fi.time);
	add_assoc_long_ex(return_value, "total_time", sizeof("total_time")-1, fi.total_time);
	add_assoc_long_ex(return_value, "frame_time", sizeof("frame_time")-1, fi.frame_time);
	add_assoc_stringl_ex(return_value, "buffer", sizeof("buffer")-1, (char *)fi.buffer, fi.buffer_size, 1);
	add_assoc_long_ex(return_value, "buffer_size", sizeof("buffer_size")-1, fi.buffer_size);
	add_assoc_long_ex(return_value, "total_size", sizeof("total_size")-1, fi.total_size);
	add_assoc_long_ex(return_value, "volume", sizeof("volume")-1, fi.volume);
	add_assoc_long_ex(return_value, "loop_count", sizeof("loop_count")-1, fi.loop_count);
	add_assoc_long_ex(return_value, "virt_channels", sizeof("virt_channels")-1, fi.virt_channels);
	add_assoc_long_ex(return_value, "virt_used", sizeof("virt_used")-1, fi.virt_used);
	add_assoc_long_ex(return_value, "sequence", sizeof("sequence")-1, fi.sequence);
	add_assoc_long_ex(return_value, "pattern", sizeof("pattern")-1, fi.pattern);

	MAKE_STD_ZVAL(channel_info);
	array_init(channel_info);
	{
		for (i=0;i<XMP_MAX_CHANNELS;i++) {
			ci = &fi.channel_info[i];
			MAKE_STD_ZVAL(cur);
			array_init(cur);
			{
				add_assoc_long_ex(cur, "period", sizeof("period")-1, ci->period);
				add_assoc_long_ex(cur, "position", sizeof("position")-1, ci->position);
				add_assoc_long_ex(cur, "pitchbend", sizeof("pitchbend")-1, ci->pitchbend);
				add_assoc_long_ex(cur, "note", sizeof("note")-1, ci->note);
				add_assoc_long_ex(cur, "instrument", sizeof("instrument")-1, ci->instrument);
				add_assoc_long_ex(cur, "sample", sizeof("sample")-1, ci->sample);
				add_assoc_long_ex(cur, "volume", sizeof("volume")-1, ci->volume);
				add_assoc_long_ex(cur, "pan", sizeof("pan")-1, ci->pan);
				add_assoc_long_ex(cur, "reserved", sizeof("reserved")-1, ci->reserved);
				{
					MAKE_STD_ZVAL(event);
					array_init(event);
					{
						add_assoc_long_ex(cur, "note", sizeof("note")-1, ci->event.note);
						add_assoc_long_ex(cur, "ins", sizeof("ins")-1, ci->event.ins);
						add_assoc_long_ex(cur, "vol", sizeof("vol")-1, ci->event.vol);
						add_assoc_long_ex(cur, "fxt", sizeof("fxt")-1, ci->event.fxt);
						add_assoc_long_ex(cur, "fxp", sizeof("fxp")-1, ci->event.fxp);
						add_assoc_long_ex(cur, "f2t", sizeof("f2t")-1, ci->event.f2t);
						add_assoc_long_ex(cur, "f2p", sizeof("f2p")-1, ci->event.f2p);
						add_assoc_long_ex(cur, "_flag", sizeof("_flag")-1, ci->event._flag);
					}
					add_assoc_zval_ex(cur, "event", sizeof("event")-1, event);
				}
				add_next_index_zval(channel_info, cur);
			}
		}
		add_assoc_zval_ex(return_value, "channel_info", sizeof("channel_info")-1, channel_info);
	}
}
/* }}} */

/* {{{ xmp_end_player(resource)
 * Release player memory from resource */
PHP_FUNCTION(xmp_end_player)
{
	xmp_context *xmp_ptr;
	zval *ctx;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &ctx) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(xmp_ptr, xmp_context *, &ctx, -1, "xmp_context", le_xmp);
	xmp_end_player(*xmp_ptr);
}
/* }}} */

/* {{{ xmp_next_position(resource)
 * Skip replay to the start of the next position */
PHP_FUNCTION(xmp_next_position)
{
	xmp_context *xmp_ptr;
	zval *ctx;
	long ret;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &ctx) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(xmp_ptr, xmp_context *, &ctx, -1, "xmp_context", le_xmp);
	ret = xmp_next_position(*xmp_ptr);
	RETVAL_LONG(ret);
}
/* }}} */

/* {{{ xmp_prev_position(resource)
 * Skip replay to the start of the previous position */
PHP_FUNCTION(xmp_prev_position)
{
	xmp_context *xmp_ptr;
	zval *ctx;
	long ret;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &ctx) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(xmp_ptr, xmp_context *, &ctx, -1, "xmp_context", le_xmp);
	ret = xmp_prev_position(*xmp_ptr);
	RETVAL_LONG(ret);
}
/* }}} */

/* {{{ xmp_set_position(resource, pos)
 * Skip replay to the start of the specified position */
PHP_FUNCTION(xmp_set_position)
{
	xmp_context *xmp_ptr;
	zval *ctx;
	long ret, pos;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rl", &ctx, &pos) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(xmp_ptr, xmp_context *, &ctx, -1, "xmp_context", le_xmp);
	ret = xmp_set_position(*xmp_ptr, pos);
	RETVAL_LONG(ret);
}
/* }}} */

/* {{{ xmp_stop_module(resource)
 * Stops the module */
PHP_FUNCTION(xmp_stop_module)
{
	xmp_context *xmp_ptr;
	zval *ctx;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &ctx) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(xmp_ptr, xmp_context *, &ctx, -1, "xmp_context", le_xmp);
	xmp_stop_module(*xmp_ptr);
}
/* }}} */

/* {{{ xmp_restart_module(resource)
 * Restarts the module */
PHP_FUNCTION(xmp_restart_module)
{
	xmp_context *xmp_ptr;
	zval *ctx;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &ctx) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(xmp_ptr, xmp_context *, &ctx, -1, "xmp_context", le_xmp);
	xmp_restart_module(*xmp_ptr);
}
/* }}} */

/* {{{ xmp_seek_time(resource, time)
 * Skip replay to the specified time */
PHP_FUNCTION(xmp_seek_time)
{
	xmp_context *xmp_ptr;
	zval *ctx;
	long ret, time;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rl", &ctx, &time) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(xmp_ptr, xmp_context *, &ctx, -1, "xmp_context", le_xmp);
	ret = xmp_seek_time(*xmp_ptr, time);
	RETVAL_LONG(ret);
}
/* }}} */

/* {{{ xmp_channel_mute(resource, channel, status)
 * Mute or unmute the specified channel */
PHP_FUNCTION(xmp_channel_mute)
{
	xmp_context *xmp_ptr;
	zval *ctx;
	long ret, channel, status;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rll", &ctx, &channel, &status) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(xmp_ptr, xmp_context *, &ctx, -1, "xmp_context", le_xmp);
	ret = xmp_channel_mute(*xmp_ptr, channel, status);
	RETVAL_LONG(ret);
}
/* }}} */

/* {{{ xmp_channel_vol(resource, channel, vol)
 * Set or retrieve the volume of the specified channel. */
PHP_FUNCTION(xmp_channel_vol)
{
	xmp_context *xmp_ptr;
	zval *ctx;
	long ret, channel, vol;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rll", &ctx, &channel, &vol) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(xmp_ptr, xmp_context *, &ctx, -1, "xmp_context", le_xmp);
	ret = xmp_channel_vol(*xmp_ptr, channel, vol);
	RETVAL_LONG(ret);
}
/* }}} */

/* {{{ xmp_inject_event(resource, channel, event)
 * Dynamically insert a new event into a playing module. */
PHP_FUNCTION(xmp_inject_event)
{
	xmp_context *xmp_ptr;
	zval *ctx, *array;
	long channel;
	struct xmp_event event;
	zval **tmp, option;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rla", &ctx, &channel, &array) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(xmp_ptr, xmp_context *, &ctx, -1, "xmp_context", le_xmp);

#define SET_EVENT_VALUE(x, x_str) \
	do { \
		if (zend_hash_find(HASH_OF(array), x_str, sizeof(x_str), (void **)&tmp) != SUCCESS) { \
			x = 0; \
		} else { \
			option = **tmp; \
			zval_copy_ctor(&option); \
			convert_to_long(&option); \
			x = Z_LVAL(option); \
		} \
	} while(0);

	SET_EVENT_VALUE(event.note, "note");
	SET_EVENT_VALUE(event.ins, "ins");
	SET_EVENT_VALUE(event.vol, "vol");
	SET_EVENT_VALUE(event.fxt, "fxt");
	SET_EVENT_VALUE(event.fxp, "fxp");
	SET_EVENT_VALUE(event.f2t, "f2t");
	SET_EVENT_VALUE(event.f2p, "f2p");
	SET_EVENT_VALUE(event._flag, "_flag");

	xmp_inject_event(*xmp_ptr, channel, &event);
}
/* }}} */

/* {{{ xmp_set_instrument_path(resource, path)
 * Set the path to retrieve external instruments or samples */
PHP_FUNCTION(xmp_set_instrument_path)
{
	xmp_context *xmp_ptr;
	zval *ctx;
	char *path;
	long ret, path_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rs", &ctx, &path, &path_len) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(xmp_ptr, xmp_context *, &ctx, -1, "xmp_context", le_xmp);
	ret = xmp_set_instrument_path(*xmp_ptr, path);
	RETVAL_LONG(ret);
}
/* }}} */

/* {{{ xmp_get_player(resource, param)
 * Retrieve current value of the specified player parameter. */
PHP_FUNCTION(xmp_get_player)
{
	xmp_context *xmp_ptr;
	zval *ctx;
	long ret, param;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rl", &ctx, &param) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(xmp_ptr, xmp_context *, &ctx, -1, "xmp_context", le_xmp);
	ret = xmp_get_player(*xmp_ptr, param);
	RETVAL_LONG(ret);
}
/* }}} */

/* {{{ xmp_set_player(resource, param, val)
 * Set player parameter with the specified value. */
PHP_FUNCTION(xmp_set_player)
{
	xmp_context *xmp_ptr;
	zval *ctx;
	long ret, param, val;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rll", &ctx, &param, &val) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(xmp_ptr, xmp_context *, &ctx, -1, "xmp_context", le_xmp);
	ret = xmp_set_player(*xmp_ptr, param, val);
	RETVAL_LONG(ret);
}
/* }}} */

/* {{{ xmp_start_smix(resource, nch, nsmp)
* Initialize the mix subsystem with the given number of reserved channels and samples. */
PHP_FUNCTION(xmp_start_smix)
{
	xmp_context *xmp_ptr;
	zval *ctx;
	long ret, nch, nsmp;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rll", &ctx, &nch, &nsmp) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(xmp_ptr, xmp_context *, &ctx, -1, "xmp_context", le_xmp);
	ret = xmp_start_smix(*xmp_ptr, nch, nsmp);
	RETVAL_LONG(ret);
}
/* }}} */

/* {{{ xmp_smix_play_instrument(resource, ins, note, vol, chn)
* Play a note using an instrument from the currently loaded module in one of the reserved sound mix channels. */
PHP_FUNCTION(xmp_smix_play_instrument)
{
	xmp_context *xmp_ptr;
	zval *ctx;
	long ret, ins, note, vol, chn;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rllll", &ctx, &ins, &note, &vol, &chn) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(xmp_ptr, xmp_context *, &ctx, -1, "xmp_context", le_xmp);
	ret = xmp_smix_play_instrument(*xmp_ptr, ins, note, vol, chn);
	RETVAL_LONG(ret);
}
/* }}} */

/* {{{ xmp_smix_play_sample(resource, ins, vol, chn)
* Play an external sample file in one of the reserved sound channels */
PHP_FUNCTION(xmp_smix_play_sample)
{
	xmp_context *xmp_ptr;
	zval *ctx;
	long ret, ins, note, vol, chn;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rllll", &ctx, &ins, &note, &vol, &chn) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(xmp_ptr, xmp_context *, &ctx, -1, "xmp_context", le_xmp);
	ret = xmp_smix_play_sample(*xmp_ptr, ins, note, vol, chn);
	RETVAL_LONG(ret);
}
/* }}} */

/* {{{ xmp_smix_channel_pan(resource, chn, pan)
* Set the channel pan value. */
PHP_FUNCTION(xmp_smix_channel_pan)
{
	xmp_context *xmp_ptr;
	zval *ctx;
	long ret, chn, pan;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rll", &ctx, &chn, &pan) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(xmp_ptr, xmp_context *, &ctx, -1, "xmp_context", le_xmp);
	ret = xmp_smix_channel_pan(*xmp_ptr, chn, pan);
	RETVAL_LONG(ret);
}
/* }}} */

/* {{{ xmp_smix_load_sample(resource, num, path)
* Load a sound sample from a file. Samples should be in mono WAV (RIFF) format. */
PHP_FUNCTION(xmp_smix_load_sample)
{
	xmp_context *xmp_ptr;
	zval *ctx;
	char *path;
	long ret, num, path_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rls", &ctx, &num, &path, &path_len) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(xmp_ptr, xmp_context *, &ctx, -1, "xmp_context", le_xmp);
	ret = xmp_smix_load_sample(*xmp_ptr, num, path);
	RETVAL_LONG(ret);
}
/* }}} */

/* {{{ xmp_smix_release_sample(resource, num)
* Release a sound sample */
PHP_FUNCTION(xmp_smix_release_sample)
{
	xmp_context *xmp_ptr;
	zval *ctx;
	long ret, num;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rl", &ctx, &num) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(xmp_ptr, xmp_context *, &ctx, -1, "xmp_context", le_xmp);
	ret = xmp_smix_release_sample(*xmp_ptr, num);
	RETVAL_LONG(ret);
}
/* }}} */

/* {{{ xmp_end_smix(resource)
* Release the mix subsystem */
PHP_FUNCTION(xmp_end_smix)
{
	xmp_context *xmp_ptr;
	zval *ctx;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &ctx) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(xmp_ptr, xmp_context *, &ctx, -1, "xmp_context", le_xmp);
	xmp_end_smix(*xmp_ptr);
}
/* }}} */



/* {{{ _free_xmp_context
 */
static void _free_xmp_context(zend_rsrc_list_entry *rsrc TSRMLS_DC)
{
	xmp_context *xmp_ptr = (xmp_context *)rsrc->ptr;

	if (*xmp_ptr) {
		xmp_free_context(*xmp_ptr);
		efree(xmp_ptr);
	}

}
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */

PHP_MINIT_FUNCTION(xmp)
{
	le_xmp = zend_register_list_destructors_ex(_free_xmp_context, NULL, "xmp context", module_number);
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(xmp)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(xmp)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "xmp support", "enabled");
	php_info_print_table_end();
}
/* }}} */

/* {{{ xmp_functions[]
 *
 * Every user visible function must have an entry in xmp_functions[].
 */
const zend_function_entry xmp_functions[] = {
	PHP_FE(xmp_get_format_list, NULL)
	PHP_FE(xmp_create_context, NULL)
	PHP_FE(xmp_free_context, NULL)
	PHP_FE(xmp_test_module, NULL)
	PHP_FE(xmp_load_module, NULL)
	PHP_FE(xmp_load_module_from_memory, NULL)
	PHP_FE(xmp_release_module, NULL)
	PHP_FE(xmp_get_module_info, NULL)
	PHP_FE(xmp_start_player, NULL)
	PHP_FE(xmp_play_frame, NULL)
	PHP_FE(xmp_get_frame_info, NULL)
	PHP_FE(xmp_end_player, NULL)
	PHP_FE(xmp_next_position, NULL)
	PHP_FE(xmp_prev_position, NULL)
	PHP_FE(xmp_set_position, NULL)
	PHP_FE(xmp_stop_module, NULL)
	PHP_FE(xmp_restart_module, NULL)
	PHP_FE(xmp_seek_time, NULL)
	PHP_FE(xmp_channel_mute, NULL)
	PHP_FE(xmp_channel_vol, NULL)
	PHP_FE(xmp_inject_event, NULL)
	PHP_FE(xmp_set_instrument_path, NULL)
	PHP_FE(xmp_get_player, NULL)
	PHP_FE(xmp_set_player, NULL)
	PHP_FE(xmp_start_smix, NULL)
	PHP_FE(xmp_smix_play_instrument, NULL)
	PHP_FE(xmp_smix_play_sample, NULL)
	PHP_FE(xmp_smix_channel_pan, NULL)
	PHP_FE(xmp_smix_load_sample, NULL)
	PHP_FE(xmp_smix_release_sample, NULL)
	PHP_FE(xmp_end_smix, NULL)
	PHP_FE_END	/* Must be the last line in xmp_functions[] */
};
/* }}} */

/* {{{ xmp_module_entry
 */
zend_module_entry xmp_module_entry = {
	STANDARD_MODULE_HEADER,
	"xmp",
	xmp_functions,
	PHP_MINIT(xmp),
	PHP_MSHUTDOWN(xmp),
	NULL,
	NULL,
	PHP_MINFO(xmp),
	PHP_XMP_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
