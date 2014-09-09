--TEST--
Test xmp_inject_event
--CREDITS--
Boro Sitnikovski <buritomath@yahoo.com>
--SKIPIF--
<?php if (!extension_loaded("xmp")) print "skip"; ?>
--FILE--
<?php
$ctx = xmp_create_context();
xmp_load_module($ctx, dirname(__FILE__)."/../3d.mod");
xmp_start_player($ctx, 44100, 0);
xmp_play_frame($ctx);

$info = xmp_get_frame_info($ctx);
var_dump($info['channel_info'][0]['volume']);

xmp_inject_event($ctx, 0,
    array(
        'vol' => 33,
    )
);

xmp_play_frame($ctx);
$info = xmp_get_frame_info($ctx);
var_dump($info['channel_info'][0]['volume']);

xmp_end_player($ctx);
xmp_release_module($ctx);
xmp_free_context($ctx);
?>
--EXPECT--
int(1)
int(32)
