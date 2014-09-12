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

#if PHP_VERSION_ID < 50400
#define ARG_PATH "s"
#else
#define ARG_PATH "p"
#endif

#ifdef COMPILE_DL_XMP
ZEND_GET_MODULE(xmp)
#endif

#define FETCH_XMP \
	do { \
		ZEND_FETCH_RESOURCE(xmp, xmp_context, &ctx, -1, "xmp_context", le_xmp); \
	} while(0);

ZEND_BEGIN_ARG_INFO_EX(arginfo_xmp_none, 0, 0, 0)
ZEND_END_ARG_INFO()

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
	xmp_context xmp;

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	xmp = xmp_create_context();

	if (xmp) {
		ZEND_REGISTER_RESOURCE(return_value, xmp, le_xmp);
	} else {
		RETURN_FALSE;
	}
}
/* }}} */

ZEND_BEGIN_ARG_INFO_EX(arginfo_xmp_resource, 0, 0, 1)
        ZEND_ARG_INFO(0, resource)
ZEND_END_ARG_INFO()

/* {{{ xmp_free_context(resource)
 * Frees the specified resource */
PHP_FUNCTION(xmp_free_context)
{
	xmp_context xmp = NULL;
	zval *ctx;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &ctx) == FAILURE) {
		return;
	}

	FETCH_XMP;
	if (xmp) {
		xmp_free_context(xmp);
		zend_list_delete(Z_RESVAL_P(ctx));
	}

}
/* }}} */

ZEND_BEGIN_ARG_INFO_EX(arginfo_xmp_test_module, 0, 0, 1)
        ZEND_ARG_INFO(0, path)
ZEND_END_ARG_INFO()

/* {{{ xmp_test_module(path)
 * Retrieve name and module type */
PHP_FUNCTION(xmp_test_module)
{
	long ret;
	char *path;
	int path_len;
	struct xmp_test_info ti;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, ARG_PATH, &path, &path_len) == FAILURE) {
		return;
	}

	ret = xmp_test_module(path, &ti);
	if (ret) {
		RETVAL_LONG(ret);
	} else {
		array_init(return_value);
		add_assoc_string_ex(return_value, "name", sizeof("name"), ti.name, 1);
		add_assoc_string_ex(return_value, "type", sizeof("type"), ti.type, 1);
	}
}
/* }}} */

ZEND_BEGIN_ARG_INFO_EX(arginfo_xmp_load_module, 0, 0, 2)
        ZEND_ARG_INFO(0, resource)
        ZEND_ARG_INFO(0, path)
ZEND_END_ARG_INFO()

/* {{{ xmp_load_module(resource, path)
 * Load module for the specified resource */
PHP_FUNCTION(xmp_load_module)
{
	xmp_context xmp;
	zval *ctx;
	int ret;
	char *path;
	int path_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rs", &ctx, &path, &path_len) == FAILURE) {
		return;
	}

	FETCH_XMP;
	ret = xmp_load_module(xmp, path);
	RETVAL_LONG(ret);
}
/* }}} */

ZEND_BEGIN_ARG_INFO_EX(arginfo_xmp_load_module_from_memory, 0, 0, 2)
        ZEND_ARG_INFO(0, resource)
        ZEND_ARG_INFO(0, string)
ZEND_END_ARG_INFO()

/* {{{ xmp_load_module_from_memory(resource, string)
 * Load module for the specified resource from string */
PHP_FUNCTION(xmp_load_module_from_memory)
{
	xmp_context xmp;
	zval *ctx;
	char *data;
	int ret, length;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rs", &ctx, &data, &length) == FAILURE) {
		return;
	}

	FETCH_XMP;
	ret = xmp_load_module_from_memory(xmp, data, length);
	RETVAL_LONG(ret);
}
/* }}} */

/* {{{ xmp_release_module(resource)
 * Release module for the specified resource */
PHP_FUNCTION(xmp_release_module)
{
	zval *ctx;
	xmp_context xmp;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &ctx) == FAILURE) {
		return;
	}

	FETCH_XMP;
	xmp_release_module(xmp);
}
/* }}} */

/* {{{ xmp_get_module_info(resource)
 * Returns an array of information about the module */
PHP_FUNCTION(xmp_get_module_info)
{
	struct xmp_module_info mi;
	xmp_context xmp;
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

	FETCH_XMP;
	xmp_get_module_info(xmp, &mi);

	array_init(return_value);
	add_assoc_stringl_ex(return_value, "md5", sizeof("md5"), (char *)mi.md5, 16, 1);
	add_assoc_long_ex(return_value, "vol_base", sizeof("vol_base"), mi.vol_base);

	MAKE_STD_ZVAL(mod);
	array_init(mod);
	{
		add_assoc_string_ex(mod, "name", sizeof("name"), mi.mod->name, 1);
		add_assoc_string_ex(mod, "type", sizeof("type"), mi.mod->type, 1);
		add_assoc_long_ex(mod, "pat", sizeof("pat"), mi.mod->pat);
		add_assoc_long_ex(mod, "trk", sizeof("trk"), mi.mod->trk);
		add_assoc_long_ex(mod, "chn", sizeof("chn"), mi.mod->chn);
		add_assoc_long_ex(mod, "ins", sizeof("ins"), mi.mod->ins);
		add_assoc_long_ex(mod, "smp", sizeof("smp"), mi.mod->smp);
		add_assoc_long_ex(mod, "spd", sizeof("spd"), mi.mod->spd);
		add_assoc_long_ex(mod, "bpm", sizeof("bpm"), mi.mod->bpm);
		add_assoc_long_ex(mod, "len", sizeof("len"), mi.mod->len);
		add_assoc_long_ex(mod, "rst", sizeof("rst"), mi.mod->rst);
		add_assoc_long_ex(mod, "gvl", sizeof("gvl"), mi.mod->gvl);

		MAKE_STD_ZVAL(xxp);
		array_init(xxp);
		{
			for (i=0;i<mi.mod->pat;i++) {
				MAKE_STD_ZVAL(cur);
				array_init(cur);
				xp = mi.mod->xxp[i];
				add_assoc_long_ex(cur, "rows", sizeof("rows"), xp->rows);
				add_assoc_long_ex(cur, "index", sizeof("index"), xp->index[0]);
				add_next_index_zval(xxp, cur);
			}
			add_assoc_zval_ex(mod, "xxp", sizeof("xxp"), xxp);
		}

		MAKE_STD_ZVAL(xxt);
		array_init(xxt);
		{
			for (i=0;i<mi.mod->trk;i++) {
				MAKE_STD_ZVAL(cur);
				array_init(cur);
				xt = mi.mod->xxt[i];
				add_assoc_long_ex(cur, "rows", sizeof("rows"), xt->rows);
				MAKE_STD_ZVAL(event);
				array_init(event);
				{
					add_assoc_long_ex(event, "note", sizeof("note"), xt->event[0].note);
					add_assoc_long_ex(event, "ins", sizeof("ins"), xt->event[0].ins);
					add_assoc_long_ex(event, "vol", sizeof("vol"), xt->event[0].vol);
					add_assoc_long_ex(event, "fxt", sizeof("fxt"), xt->event[0].fxt);
					add_assoc_long_ex(event, "fxp", sizeof("fxp"), xt->event[0].fxp);
					add_assoc_long_ex(event, "f2t", sizeof("f2t"), xt->event[0].f2t);
					add_assoc_long_ex(event, "f2p", sizeof("f2p"), xt->event[0].f2p);
					add_assoc_long_ex(event, "_flag", sizeof("_flag"), xt->event[0]._flag);
				}
				add_next_index_zval(cur, event);
				add_next_index_zval(xxt, cur);
			}
			add_assoc_zval_ex(mod, "xxt", sizeof("xxt"), xxt);
		}

		MAKE_STD_ZVAL(xxi);
		array_init(xxi);
		{
			for (i=0;i<mi.mod->ins;i++) {
				xi = &mi.mod->xxi[i];
				MAKE_STD_ZVAL(cur);
				array_init(cur);
				add_assoc_string_ex(cur, "name", sizeof("name"), xi->name, 1);
				add_assoc_long_ex(cur, "vol", sizeof("vol"), xi->vol);
				add_assoc_long_ex(cur, "nsm", sizeof("nsm"), xi->nsm);
				add_assoc_long_ex(cur, "rls", sizeof("rls"), xi->rls);
				MAKE_STD_ZVAL(curr);
				array_init(curr);
				{
					add_assoc_long_ex(curr, "flg", sizeof("flg"), xi->aei.flg);
					add_assoc_long_ex(curr, "npt", sizeof("npt"), xi->aei.npt);
					add_assoc_long_ex(curr, "scl", sizeof("scl"), xi->aei.scl);
					add_assoc_long_ex(curr, "sus", sizeof("sus"), xi->aei.sus);
					add_assoc_long_ex(curr, "sue", sizeof("sue"), xi->aei.sue);
					add_assoc_long_ex(curr, "lps", sizeof("lps"), xi->aei.lps);
					add_assoc_long_ex(curr, "lpe", sizeof("lpe"), xi->aei.lpe);
					add_assoc_stringl_ex(curr, "data", sizeof("data"), (char *)xi->aei.data, XMP_MAX_ENV_POINTS * 2, 1);
					add_assoc_zval_ex(cur, "aei", sizeof("aei"), curr);
				}
				MAKE_STD_ZVAL(curr);
				array_init(curr);
				{
					add_assoc_long_ex(curr, "flg", sizeof("flg"), xi->pei.flg);
					add_assoc_long_ex(curr, "npt", sizeof("npt"), xi->pei.npt);
					add_assoc_long_ex(curr, "scl", sizeof("scl"), xi->pei.scl);
					add_assoc_long_ex(curr, "sus", sizeof("sus"), xi->pei.sus);
					add_assoc_long_ex(curr, "sue", sizeof("sue"), xi->pei.sue);
					add_assoc_long_ex(curr, "lps", sizeof("lps"), xi->pei.lps);
					add_assoc_long_ex(curr, "lpe", sizeof("lpe"), xi->pei.lpe);
					add_assoc_stringl_ex(curr, "data", sizeof("data"), (char *)xi->pei.data, XMP_MAX_ENV_POINTS * 2, 1);
					add_assoc_zval_ex(cur, "pei", sizeof("pei"), curr);
				}
				MAKE_STD_ZVAL(curr);
				array_init(curr);
				{
					add_assoc_long_ex(curr, "flg", sizeof("flg"), xi->fei.flg);
					add_assoc_long_ex(curr, "npt", sizeof("npt"), xi->fei.npt);
					add_assoc_long_ex(curr, "scl", sizeof("scl"), xi->fei.scl);
					add_assoc_long_ex(curr, "sus", sizeof("sus"), xi->fei.sus);
					add_assoc_long_ex(curr, "sue", sizeof("sue"), xi->fei.sue);
					add_assoc_long_ex(curr, "lps", sizeof("lps"), xi->fei.lps);
					add_assoc_long_ex(curr, "lpe", sizeof("lpe"), xi->fei.lpe);
					add_assoc_stringl_ex(curr, "data", sizeof("data"), (char *)xi->fei.data, XMP_MAX_ENV_POINTS * 2, 1);
					add_assoc_zval_ex(cur, "fei", sizeof("fei"), curr);
				}
				MAKE_STD_ZVAL(curr);
				array_init(curr);
				{
					for (i=0;i<XMP_MAX_KEYS;i++) {
						MAKE_STD_ZVAL(currr);
						array_init(currr);
						add_assoc_long_ex(currr, "ins", sizeof("ins"), xi->map[i].ins);
						add_assoc_long_ex(currr, "xpo", sizeof("xpc"), xi->map[i].xpo);
						add_next_index_zval(curr, currr);
					}
					add_assoc_zval_ex(cur, "map", sizeof("map"), curr);
				}
				MAKE_STD_ZVAL(curr);
				array_init(curr);
				{
					add_assoc_long_ex(curr, "vol", sizeof("vol"), xi->sub->vol);
					add_assoc_long_ex(curr, "gvl", sizeof("gvl"), xi->sub->gvl);
					add_assoc_long_ex(curr, "pan", sizeof("pan"), xi->sub->pan);
					add_assoc_long_ex(curr, "xpo", sizeof("xpo"), xi->sub->xpo);
					add_assoc_long_ex(curr, "fin", sizeof("fin"), xi->sub->fin);
					add_assoc_long_ex(curr, "vwf", sizeof("vwf"), xi->sub->vwf);
					add_assoc_long_ex(curr, "vde", sizeof("vde"), xi->sub->vde);
					add_assoc_long_ex(curr, "vra", sizeof("vra"), xi->sub->vra);
					add_assoc_long_ex(curr, "vsw", sizeof("vsw"), xi->sub->vsw);
					add_assoc_long_ex(curr, "rvv", sizeof("rvv"), xi->sub->rvv);
					add_assoc_long_ex(curr, "sid", sizeof("sid"), xi->sub->sid);
					add_assoc_long_ex(curr, "nna", sizeof("nna"), xi->sub->nna);
					add_assoc_long_ex(curr, "dct", sizeof("dct"), xi->sub->dct);
					add_assoc_long_ex(curr, "dca", sizeof("dca"), xi->sub->dca);
					add_assoc_long_ex(curr, "ifc", sizeof("ifc"), xi->sub->ifc);
					add_assoc_long_ex(curr, "ifr", sizeof("ifr"), xi->sub->ifr);
					add_assoc_zval_ex(cur, "sub", sizeof("sub"), curr);
				}
				add_next_index_zval(xxi, cur);
			}
			add_assoc_zval_ex(mod, "xxi", sizeof("xxi"), xxi);
		}

		MAKE_STD_ZVAL(xxs);
		array_init(xxs);
		{
			xs = mi.mod->xxs;
			add_assoc_string_ex(xxs, "name", sizeof("name"), xs->name, 1);
			add_assoc_long_ex(xxs, "len", sizeof("len"), xs->len);
			add_assoc_long_ex(xxs, "lps", sizeof("lps"), xs->lps);
			add_assoc_long_ex(xxs, "lpe", sizeof("lpe"), xs->lpe);
			add_assoc_long_ex(xxs, "flg", sizeof("flg"), xs->flg);
			if (xs->data) {
				add_assoc_stringl_ex(xxs, "data", sizeof("data"), (char *)xs->data, xs->len, 1);
			} else {
				add_assoc_string_ex(xxs, "data", sizeof("data"), "", 1);
			}
			add_assoc_zval_ex(mod, "xxs", sizeof("xxs"), xxs);
		}

		MAKE_STD_ZVAL(xxc);
		array_init(xxc);
		{
			for (i=0;i<64;i++) {
				xc = &mi.mod->xxc[i];
				MAKE_STD_ZVAL(cur);
				array_init(cur);
				add_assoc_long_ex(cur, "pan", sizeof("pan"), xc->pan);
				add_assoc_long_ex(cur, "vol", sizeof("vol"), xc->vol);
				add_assoc_long_ex(cur, "flg", sizeof("flg"), xc->flg);
				add_next_index_zval(xxc, cur);
			}
			add_assoc_zval_ex(mod, "xxc", sizeof("xxc"), xxc);
		}

		add_assoc_string_ex(mod, "xxo", sizeof("xxo"), (char *)mi.mod->xxo, 1);
		add_assoc_zval_ex(return_value, "mod", sizeof("mod"), mod);
	}

	if (mi.comment) {
		add_assoc_string_ex(return_value, "comment", sizeof("comment"), mi.comment, 1);
	} else {
		add_assoc_string_ex(return_value, "comment", sizeof("comment"), "", 1);
	}
	add_assoc_long_ex(return_value, "num_sequences", sizeof("num_sequences"), mi.num_sequences);

	MAKE_STD_ZVAL(seq_data);
	array_init(seq_data);
	{
		add_assoc_long_ex(seq_data, "entry_point", sizeof("entry_point"), mi.seq_data->entry_point);
		add_assoc_long_ex(seq_data, "duration", sizeof("duration"), mi.seq_data->duration);
		add_assoc_zval_ex(return_value, "seq_data", sizeof("seq_data"), seq_data);
	}
}
/* }}} */

ZEND_BEGIN_ARG_INFO_EX(arginfo_xmp_start_player, 0, 0, 3)
        ZEND_ARG_INFO(0, resource)
        ZEND_ARG_INFO(0, rate)
        ZEND_ARG_INFO(0, format)
ZEND_END_ARG_INFO()

/* {{{ xmp_start_player(resource, rate, format)
 * Starts playing the currently loaded module */
PHP_FUNCTION(xmp_start_player)
{
	xmp_context xmp;
	zval *ctx;
	long rate, format, ret;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rll", &ctx, &rate, &format) == FAILURE) {
		return;
	}

	FETCH_XMP;
	ret = xmp_start_player(xmp, rate, format);

	RETVAL_LONG(ret);
}
/* }}} */

/* {{{ xmp_play_frame(resource)
 * Plays one frame from the currently loaded module */
PHP_FUNCTION(xmp_play_frame)
{
	xmp_context xmp;
	zval *ctx;
	long ret;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &ctx) == FAILURE) {
		return;
	}

	FETCH_XMP;
	ret = xmp_play_frame(xmp);

	RETVAL_LONG(ret);
}
/* }}} */

/* {{{ xmp_get_frame_info(resource)
 * Returns an array of information about the current frame */
PHP_FUNCTION(xmp_get_frame_info)
{
	struct xmp_frame_info fi;
	struct xmp_channel_info *ci;
	xmp_context xmp;
	zval *ctx, *cur = 0, *event = 0, *channel_info = 0;
	int i;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &ctx) == FAILURE) {
		return;
	}

	FETCH_XMP;
	xmp_get_frame_info(xmp, &fi);

	array_init(return_value);
	add_assoc_long_ex(return_value, "pos", sizeof("pos"), fi.pos);
	add_assoc_long_ex(return_value, "pattern", sizeof("pattern"), fi.pattern);
	add_assoc_long_ex(return_value, "row", sizeof("row"), fi.row);
	add_assoc_long_ex(return_value, "num_rows", sizeof("num_rows"), fi.num_rows);
	add_assoc_long_ex(return_value, "frame", sizeof("frame"), fi.frame);
	add_assoc_long_ex(return_value, "speed", sizeof("speed"), fi.speed);
	add_assoc_long_ex(return_value, "bpm", sizeof("bpm"), fi.bpm);
	add_assoc_long_ex(return_value, "time", sizeof("time"), fi.time);
	add_assoc_long_ex(return_value, "total_time", sizeof("total_time"), fi.total_time);
	add_assoc_long_ex(return_value, "frame_time", sizeof("frame_time"), fi.frame_time);
	add_assoc_stringl_ex(return_value, "buffer", sizeof("buffer"), (char *)fi.buffer, fi.buffer_size, 1);
	add_assoc_long_ex(return_value, "buffer_size", sizeof("buffer_size"), fi.buffer_size);
	add_assoc_long_ex(return_value, "total_size", sizeof("total_size"), fi.total_size);
	add_assoc_long_ex(return_value, "volume", sizeof("volume"), fi.volume);
	add_assoc_long_ex(return_value, "loop_count", sizeof("loop_count"), fi.loop_count);
	add_assoc_long_ex(return_value, "virt_channels", sizeof("virt_channels"), fi.virt_channels);
	add_assoc_long_ex(return_value, "virt_used", sizeof("virt_used"), fi.virt_used);
	add_assoc_long_ex(return_value, "sequence", sizeof("sequence"), fi.sequence);
	add_assoc_long_ex(return_value, "pattern", sizeof("pattern"), fi.pattern);

	MAKE_STD_ZVAL(channel_info);
	array_init(channel_info);
	{
		for (i=0;i<XMP_MAX_CHANNELS;i++) {
			ci = &fi.channel_info[i];
			MAKE_STD_ZVAL(cur);
			array_init(cur);
			{
				add_assoc_long_ex(cur, "period", sizeof("period"), ci->period);
				add_assoc_long_ex(cur, "position", sizeof("position"), ci->position);
				add_assoc_long_ex(cur, "pitchbend", sizeof("pitchbend"), ci->pitchbend);
				add_assoc_long_ex(cur, "note", sizeof("note"), ci->note);
				add_assoc_long_ex(cur, "instrument", sizeof("instrument"), ci->instrument);
				add_assoc_long_ex(cur, "sample", sizeof("sample"), ci->sample);
				add_assoc_long_ex(cur, "volume", sizeof("volume"), ci->volume);
				add_assoc_long_ex(cur, "pan", sizeof("pan"), ci->pan);
				add_assoc_long_ex(cur, "reserved", sizeof("reserved"), ci->reserved);
				{
					MAKE_STD_ZVAL(event);
					array_init(event);
					{
						add_assoc_long_ex(cur, "note", sizeof("note"), ci->event.note);
						add_assoc_long_ex(cur, "ins", sizeof("ins"), ci->event.ins);
						add_assoc_long_ex(cur, "vol", sizeof("vol"), ci->event.vol);
						add_assoc_long_ex(cur, "fxt", sizeof("fxt"), ci->event.fxt);
						add_assoc_long_ex(cur, "fxp", sizeof("fxp"), ci->event.fxp);
						add_assoc_long_ex(cur, "f2t", sizeof("f2t"), ci->event.f2t);
						add_assoc_long_ex(cur, "f2p", sizeof("f2p"), ci->event.f2p);
						add_assoc_long_ex(cur, "_flag", sizeof("_flag"), ci->event._flag);
					}
					add_assoc_zval_ex(cur, "event", sizeof("event"), event);
				}
				add_next_index_zval(channel_info, cur);
			}
		}
		add_assoc_zval_ex(return_value, "channel_info", sizeof("channel_info"), channel_info);
	}
}
/* }}} */

/* {{{ xmp_end_player(resource)
 * Release player memory from resource */
PHP_FUNCTION(xmp_end_player)
{
	xmp_context xmp;
	zval *ctx;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &ctx) == FAILURE) {
		return;
	}

	FETCH_XMP;
	xmp_end_player(xmp);
}
/* }}} */

/* {{{ xmp_next_position(resource)
 * Skip replay to the start of the next position */
PHP_FUNCTION(xmp_next_position)
{
	xmp_context xmp;
	zval *ctx;
	long ret;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &ctx) == FAILURE) {
		return;
	}

	FETCH_XMP;
	ret = xmp_next_position(xmp);
	RETVAL_LONG(ret);
}
/* }}} */

/* {{{ xmp_prev_position(resource)
 * Skip replay to the start of the previous position */
PHP_FUNCTION(xmp_prev_position)
{
	xmp_context xmp;
	zval *ctx;
	long ret;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &ctx) == FAILURE) {
		return;
	}

	FETCH_XMP;
	ret = xmp_prev_position(xmp);
	RETVAL_LONG(ret);
}
/* }}} */

ZEND_BEGIN_ARG_INFO_EX(arginfo_xmp_set_position, 0, 0, 2)
        ZEND_ARG_INFO(0, resource)
        ZEND_ARG_INFO(0, pos)
ZEND_END_ARG_INFO()

/* {{{ xmp_set_position(resource, pos)
 * Skip replay to the start of the specified position */
PHP_FUNCTION(xmp_set_position)
{
	xmp_context xmp;
	zval *ctx;
	long ret, pos;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rl", &ctx, &pos) == FAILURE) {
		return;
	}

	FETCH_XMP;
	ret = xmp_set_position(xmp, pos);
	RETVAL_LONG(ret);
}
/* }}} */

/* {{{ xmp_stop_module(resource)
 * Stops the module */
PHP_FUNCTION(xmp_stop_module)
{
	xmp_context xmp;
	zval *ctx;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &ctx) == FAILURE) {
		return;
	}

	FETCH_XMP;
	xmp_stop_module(xmp);
}
/* }}} */

/* {{{ xmp_restart_module(resource)
 * Restarts the module */
PHP_FUNCTION(xmp_restart_module)
{
	xmp_context xmp;
	zval *ctx;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &ctx) == FAILURE) {
		return;
	}

	FETCH_XMP;
	xmp_restart_module(xmp);
}
/* }}} */

ZEND_BEGIN_ARG_INFO_EX(arginfo_xmp_seek_time, 0, 0, 2)
        ZEND_ARG_INFO(0, resource)
        ZEND_ARG_INFO(0, time)
ZEND_END_ARG_INFO()

/* {{{ xmp_seek_time(resource, time)
 * Skip replay to the specified time */
PHP_FUNCTION(xmp_seek_time)
{
	xmp_context xmp;
	zval *ctx;
	long ret, time;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rl", &ctx, &time) == FAILURE) {
		return;
	}

	FETCH_XMP;
	ret = xmp_seek_time(xmp, time);
	RETVAL_LONG(ret);
}
/* }}} */

ZEND_BEGIN_ARG_INFO_EX(arginfo_xmp_channel_mute, 0, 0, 3)
        ZEND_ARG_INFO(0, resource)
        ZEND_ARG_INFO(0, channel)
        ZEND_ARG_INFO(0, status)
ZEND_END_ARG_INFO()

/* {{{ xmp_channel_mute(resource, channel, status)
 * Mute or unmute the specified channel */
PHP_FUNCTION(xmp_channel_mute)
{
	xmp_context xmp;
	zval *ctx;
	long ret, channel, status;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rll", &ctx, &channel, &status) == FAILURE) {
		return;
	}

	FETCH_XMP;
	ret = xmp_channel_mute(xmp, channel, status);
	RETVAL_LONG(ret);
}
/* }}} */

ZEND_BEGIN_ARG_INFO_EX(arginfo_xmp_channel_vol, 0, 0, 3)
        ZEND_ARG_INFO(0, resource)
        ZEND_ARG_INFO(0, channel)
        ZEND_ARG_INFO(0, vol)
ZEND_END_ARG_INFO()

/* {{{ xmp_channel_vol(resource, channel, vol)
 * Set or retrieve the volume of the specified channel. */
PHP_FUNCTION(xmp_channel_vol)
{
	xmp_context xmp;
	zval *ctx;
	long ret, channel, vol;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rll", &ctx, &channel, &vol) == FAILURE) {
		return;
	}

	FETCH_XMP;
	ret = xmp_channel_vol(xmp, channel, vol);
	RETVAL_LONG(ret);
}
/* }}} */

ZEND_BEGIN_ARG_INFO_EX(arginfo_xmp_inject_event, 0, 0, 3)
        ZEND_ARG_INFO(0, resource)
        ZEND_ARG_INFO(0, channel)
        ZEND_ARG_ARRAY_INFO(0, event, 0)
ZEND_END_ARG_INFO()

/* {{{ xmp_inject_event(resource, channel, event)
 * Dynamically insert a new event into a playing module. */
PHP_FUNCTION(xmp_inject_event)
{
	xmp_context xmp;
	zval *ctx, *array;
	long channel;
	struct xmp_event event;
	zval **tmp, option;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rla", &ctx, &channel, &array) == FAILURE) {
		return;
	}
	FETCH_XMP;

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

	xmp_inject_event(xmp, channel, &event);
}
/* }}} */

ZEND_BEGIN_ARG_INFO_EX(arginfo_xmp_set_instrument_path, 0, 0, 2)
        ZEND_ARG_INFO(0, resource)
        ZEND_ARG_INFO(0, path)
ZEND_END_ARG_INFO()

/* {{{ xmp_set_instrument_path(resource, path)
 * Set the path to retrieve external instruments or samples */
PHP_FUNCTION(xmp_set_instrument_path)
{
	xmp_context xmp;
	zval *ctx;
	char *path;
	long ret, path_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r" ARG_PATH, &ctx, &path, &path_len) == FAILURE) {
		return;
	}

	FETCH_XMP;
	ret = xmp_set_instrument_path(xmp, path);
	RETVAL_LONG(ret);
}
/* }}} */

ZEND_BEGIN_ARG_INFO_EX(arginfo_xmp_get_player, 0, 0, 2)
        ZEND_ARG_INFO(0, resource)
        ZEND_ARG_INFO(0, param)
ZEND_END_ARG_INFO()

/* {{{ xmp_get_player(resource, param)
 * Retrieve current value of the specified player parameter. */
PHP_FUNCTION(xmp_get_player)
{
	xmp_context xmp;
	zval *ctx;
	long ret, param;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rl", &ctx, &param) == FAILURE) {
		return;
	}

	FETCH_XMP;
	ret = xmp_get_player(xmp, param);
	RETVAL_LONG(ret);
}
/* }}} */

ZEND_BEGIN_ARG_INFO_EX(arginfo_xmp_set_player, 0, 0, 3)
        ZEND_ARG_INFO(0, resource)
        ZEND_ARG_INFO(0, param)
        ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

/* {{{ xmp_set_player(resource, param, val)
 * Set player parameter with the specified value. */
PHP_FUNCTION(xmp_set_player)
{
	xmp_context xmp;
	zval *ctx;
	long ret, param, val;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rll", &ctx, &param, &val) == FAILURE) {
		return;
	}

	FETCH_XMP;
	ret = xmp_set_player(xmp, param, val);
	RETVAL_LONG(ret);
}
/* }}} */

ZEND_BEGIN_ARG_INFO_EX(arginfo_xmp_start_smix, 0, 0, 3)
        ZEND_ARG_INFO(0, resource)
        ZEND_ARG_INFO(0, nchannels)
        ZEND_ARG_INFO(0, nsamples)
ZEND_END_ARG_INFO()

/* {{{ xmp_start_smix(resource, nch, nsmp)
* Initialize the mix subsystem with the given number of reserved channels and samples. */
PHP_FUNCTION(xmp_start_smix)
{
	xmp_context xmp;
	zval *ctx;
	long ret, nch, nsmp;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rll", &ctx, &nch, &nsmp) == FAILURE) {
		return;
	}

	FETCH_XMP;
	ret = xmp_start_smix(xmp, nch, nsmp);
	RETVAL_LONG(ret);
}
/* }}} */

ZEND_BEGIN_ARG_INFO_EX(arginfo_xmp_smix_play_instrument, 0, 0, 5)
        ZEND_ARG_INFO(0, resource)
        ZEND_ARG_INFO(0, instrument)
        ZEND_ARG_INFO(0, note)
        ZEND_ARG_INFO(0, vol)
        ZEND_ARG_INFO(0, channel)
ZEND_END_ARG_INFO()

/* {{{ xmp_smix_play_instrument(resource, ins, note, vol, chn)
* Play a note using an instrument from the currently loaded module in one of the reserved sound mix channels. */
PHP_FUNCTION(xmp_smix_play_instrument)
{
	xmp_context xmp;
	zval *ctx;
	long ret, ins, note, vol, chn;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rllll", &ctx, &ins, &note, &vol, &chn) == FAILURE) {
		return;
	}

	FETCH_XMP;
	ret = xmp_smix_play_instrument(xmp, ins, note, vol, chn);
	RETVAL_LONG(ret);
}
/* }}} */

ZEND_BEGIN_ARG_INFO_EX(arginfo_xmp_smix_play_sample, 0, 0, 5)
        ZEND_ARG_INFO(0, resource)
        ZEND_ARG_INFO(0, instrument)
        ZEND_ARG_INFO(0, note)
        ZEND_ARG_INFO(0, vol)
        ZEND_ARG_INFO(0, channel)
ZEND_END_ARG_INFO()

/* {{{ xmp_smix_play_sample(resource, ins, note, vol, chn)
* Play an external sample file in one of the reserved sound channels */
PHP_FUNCTION(xmp_smix_play_sample)
{
	xmp_context xmp;
	zval *ctx;
	long ret, ins, note, vol, chn;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rllll", &ctx, &ins, &note, &vol, &chn) == FAILURE) {
		return;
	}

	FETCH_XMP;
	ret = xmp_smix_play_sample(xmp, ins, note, vol, chn);
	RETVAL_LONG(ret);
}
/* }}} */

ZEND_BEGIN_ARG_INFO_EX(arginfo_xmp_smix_channel_pan, 0, 0, 3)
        ZEND_ARG_INFO(0, resource)
        ZEND_ARG_INFO(0, channel)
        ZEND_ARG_INFO(0, pan)
ZEND_END_ARG_INFO()

/* {{{ xmp_smix_channel_pan(resource, chn, pan)
* Set the channel pan value. */
PHP_FUNCTION(xmp_smix_channel_pan)
{
	xmp_context xmp;
	zval *ctx;
	long ret, chn, pan;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rll", &ctx, &chn, &pan) == FAILURE) {
		return;
	}

	FETCH_XMP;
	ret = xmp_smix_channel_pan(xmp, chn, pan);
	RETVAL_LONG(ret);
}
/* }}} */

ZEND_BEGIN_ARG_INFO_EX(arginfo_xmp_smix_load_sample, 0, 0, 3)
        ZEND_ARG_INFO(0, resource)
        ZEND_ARG_INFO(0, num)
        ZEND_ARG_INFO(0, path)
ZEND_END_ARG_INFO()

/* {{{ xmp_smix_load_sample(resource, num, path)
* Load a sound sample from a file. Samples should be in mono WAV (RIFF) format. */
PHP_FUNCTION(xmp_smix_load_sample)
{
	xmp_context xmp;
	zval *ctx;
	char *path;
	long ret, num, path_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rl" ARG_PATH, &ctx, &num, &path, &path_len) == FAILURE) {
		return;
	}

	FETCH_XMP;
	ret = xmp_smix_load_sample(xmp, num, path);
	RETVAL_LONG(ret);
}
/* }}} */

ZEND_BEGIN_ARG_INFO_EX(arginfo_xmp_smix_release_sample, 0, 0, 2)
        ZEND_ARG_INFO(0, resource)
        ZEND_ARG_INFO(0, num)
ZEND_END_ARG_INFO()

/* {{{ xmp_smix_release_sample(resource, num)
* Release a sound sample */
PHP_FUNCTION(xmp_smix_release_sample)
{
	xmp_context xmp;
	zval *ctx;
	long ret, num;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rl", &ctx, &num) == FAILURE) {
		return;
	}

	FETCH_XMP;
	ret = xmp_smix_release_sample(xmp, num);
	RETVAL_LONG(ret);
}
/* }}} */

/* {{{ xmp_end_smix(resource)
* Release the mix subsystem */
PHP_FUNCTION(xmp_end_smix)
{
	xmp_context xmp;
	zval *ctx;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &ctx) == FAILURE) {
		return;
	}

	FETCH_XMP;
	xmp_end_smix(xmp);
}
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */

PHP_MINIT_FUNCTION(xmp)
{
	le_xmp = zend_register_list_destructors_ex(NULL, NULL, "xmp context", module_number);
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
	PHP_FE(xmp_get_format_list,         arginfo_xmp_none)
	PHP_FE(xmp_create_context,          arginfo_xmp_none)
	PHP_FE(xmp_free_context,            arginfo_xmp_resource)
	PHP_FE(xmp_test_module,             arginfo_xmp_test_module)
	PHP_FE(xmp_load_module,             arginfo_xmp_load_module)
	PHP_FE(xmp_load_module_from_memory, arginfo_xmp_load_module_from_memory)
	PHP_FE(xmp_release_module,          arginfo_xmp_resource)
	PHP_FE(xmp_get_module_info,         arginfo_xmp_resource)
	PHP_FE(xmp_start_player,            arginfo_xmp_start_player)
	PHP_FE(xmp_play_frame,              arginfo_xmp_resource)
	PHP_FE(xmp_get_frame_info,          arginfo_xmp_resource)
	PHP_FE(xmp_end_player,              arginfo_xmp_resource)
	PHP_FE(xmp_next_position,           arginfo_xmp_resource)
	PHP_FE(xmp_prev_position,           arginfo_xmp_resource)
	PHP_FE(xmp_set_position,            arginfo_xmp_set_position)
	PHP_FE(xmp_stop_module,             arginfo_xmp_resource)
	PHP_FE(xmp_restart_module,          arginfo_xmp_resource)
	PHP_FE(xmp_seek_time,               arginfo_xmp_seek_time)
	PHP_FE(xmp_channel_mute,            arginfo_xmp_channel_mute)
	PHP_FE(xmp_channel_vol,             arginfo_xmp_channel_vol)
	PHP_FE(xmp_inject_event,            arginfo_xmp_inject_event)
	PHP_FE(xmp_set_instrument_path,     arginfo_xmp_set_instrument_path)
	PHP_FE(xmp_get_player,              arginfo_xmp_get_player)
	PHP_FE(xmp_set_player,              arginfo_xmp_set_player)
	PHP_FE(xmp_start_smix,              arginfo_xmp_start_smix)
	PHP_FE(xmp_smix_play_instrument,    arginfo_xmp_smix_play_instrument)
	PHP_FE(xmp_smix_play_sample,        arginfo_xmp_smix_play_sample)
	PHP_FE(xmp_smix_channel_pan,        arginfo_xmp_smix_channel_pan)
	PHP_FE(xmp_smix_load_sample,        arginfo_xmp_smix_load_sample)
	PHP_FE(xmp_smix_release_sample,     arginfo_xmp_smix_release_sample)
	PHP_FE(xmp_end_smix,                arginfo_xmp_resource)
#ifdef  PHP_FE_END
	PHP_FE_END	/* Must be the last line in xmp_functions[] */
#else
	{NULL, NULL, NULL}
#endif
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
