--TEST--
Test xmp_get_frame_info
--CREDITS--
Boro Sitnikovski <buritomath@yahoo.com>
--SKIPIF--
<?php if (!extension_loaded("xmp")) print "skip"; ?>
--FILE--
<?php
$ctx = xmp_create_context();
xmp_load_module($ctx, dirname(__FILE__)."/../3d.mod");
xmp_start_player($ctx, 44100, 0);
$info = xmp_get_frame_info($ctx);
var_dump($info['frame']);
xmp_play_frame($ctx);
$info = xmp_get_frame_info($ctx);
var_dump($info['frame']);
xmp_play_frame($ctx);
$info = xmp_get_frame_info($ctx);
var_dump($info['frame']);
xmp_end_player($ctx);
xmp_release_module($ctx);
xmp_free_context($ctx);
?>
--EXPECT--
int(-1)
int(0)
int(1)
