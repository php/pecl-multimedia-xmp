--TEST--
Test xmp_smix_channel_pan
--CREDITS--
Boro Sitnikovski <buritomath@yahoo.com>
--SKIPIF--
<?php if (!extension_loaded("xmp")) print "skip"; ?>
--FILE--
<?php
$ctx = xmp_create_context();
xmp_load_module($ctx, dirname(__FILE__)."/../3d.mod");
xmp_start_smix($ctx, 1, 1);
xmp_start_player($ctx, 44100, 0);
var_dump(xmp_smix_channel_pan($ctx, 0, 0));
xmp_end_player($ctx);
xmp_release_module($ctx);
xmp_free_context($ctx);
?>
--EXPECT--
int(0)
