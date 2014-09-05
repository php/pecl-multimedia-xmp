--TEST--
Test xmp_set_instrument_path
--CREDITS--
Boro Sitnikovski <buritomath@yahoo.com>
--SKIPIF--
<?php if (!extension_loaded("xmp")) print "skip"; ?>
--FILE--
<?php
$ctx = xmp_create_context();
var_dump(xmp_set_instrument_path($ctx, "/tmp"));
xmp_free_context($ctx);
?>
--EXPECT--
int(0)
