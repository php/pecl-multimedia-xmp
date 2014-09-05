--TEST--
Check that the list of formats is not empty
--CREDITS--
Boro Sitnikovski <buritomath@yahoo.com>
--SKIPIF--
<?php if (!extension_loaded("xmp")) print "skip"; ?>
--FILE--
<?php 
var_dump(count(xmp_get_format_list()) > 0);
?>
--EXPECT--
bool(true)
