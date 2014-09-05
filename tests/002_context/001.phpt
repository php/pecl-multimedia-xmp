--TEST--
Test context creation and deletion
--CREDITS--
Boro Sitnikovski <buritomath@yahoo.com>
--SKIPIF--
<?php if (!extension_loaded("xmp")) print "skip"; ?>
--FILE--
<?php 
$ctx = xmp_create_context();
var_dump($ctx);
xmp_free_context($ctx);
?>
--EXPECTF--
resource(%d) of type (xmp context)
