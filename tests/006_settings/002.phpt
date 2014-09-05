--TEST--
Test xmp_get_player
--CREDITS--
Boro Sitnikovski <buritomath@yahoo.com>
--SKIPIF--
<?php if (!extension_loaded("xmp")) print "skip"; ?>
--FILE--
<?php
$ctx = xmp_create_context();
var_dump(xmp_get_player($ctx, 7));
xmp_free_context($ctx);
?>
--EXPECT--
int(-8)
