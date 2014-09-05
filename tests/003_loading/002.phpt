--TEST--
Test xmp_load_module
--CREDITS--
Boro Sitnikovski <buritomath@yahoo.com>
--SKIPIF--
<?php if (!extension_loaded("xmp")) print "skip"; ?>
--FILE--
<?php
$ctx = xmp_create_context();
$ret = xmp_load_module($ctx, dirname(__FILE__)."/../3d.mod");
var_dump($ret);
xmp_release_module($ctx);
$ret = xmp_load_module($ctx, dirname(__FILE__)."/../3d.modd");
var_dump($ret);
xmp_free_context($ctx);
?>
--EXPECT--
int(0)
int(-6)
