--TEST--
Test context creation and 2 deletions
--CREDITS--
Boro Sitnikovski <buritomath@yahoo.com>
--SKIPIF--
<?php if (!extension_loaded("xmp")) print "skip"; ?>
--FILE--
<?php 
$ctx = xmp_create_context();
var_dump($ctx);
xmp_free_context($ctx);
var_dump($ctx);
xmp_free_context($ctx);
?>
--EXPECTF--
resource(%d) of type (xmp context)
resource(%d) of type (Unknown)

Warning: xmp_free_context(): %d is not a valid xmp_context resource in %s on line %d
