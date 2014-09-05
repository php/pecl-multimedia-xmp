--TEST--
Test xmp_test_module
--CREDITS--
Boro Sitnikovski <buritomath@yahoo.com>
--SKIPIF--
<?php if (!extension_loaded("xmp")) print "skip"; ?>
--FILE--
<?php
var_dump(xmp_test_module(dirname(__FILE__)."/../3d.mod"));
var_dump(xmp_test_module(dirname(__FILE__)."/../3d.modd"));
?>
--EXPECT--
array(2) {
  ["name"]=>
  string(16) "3d demo tune(mf)"
  ["type"]=>
  string(27) "Amiga Protracker/Compatible"
}
int(-6)
