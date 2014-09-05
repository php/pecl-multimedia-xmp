--TEST--
Test xmp_get_module_info
--CREDITS--
Boro Sitnikovski <buritomath@yahoo.com>
--SKIPIF--
<?php if (!extension_loaded("xmp")) print "skip"; ?>
--FILE--
<?php
$ctx = xmp_create_context();
xmp_load_module($ctx, dirname(__FILE__)."/../3d.mod");
$info = xmp_get_module_info($ctx);
var_dump($info['seq_data']['duration']);
xmp_release_module($ctx);
xmp_free_context($ctx);
?>
--EXPECT--
int(209040)
