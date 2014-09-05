--TEST--
Test xmp_load_module_from_memory
--CREDITS--
Boro Sitnikovski <buritomath@yahoo.com>
--SKIPIF--
<?php if (!extension_loaded("xmp")) print "skip"; ?>
--FILE--
<?php
$filename = dirname(__FILE__)."/../3d.mod";
$handle = fopen($filename, "r");
$contents = fread($handle, filesize($filename));
fclose($handle);

$ctx = xmp_create_context();
$mod = xmp_load_module_from_memory($ctx, $contents);
var_dump($mod);
xmp_release_module($ctx);
xmp_free_context($ctx);
?>
--EXPECT--
int(0)
