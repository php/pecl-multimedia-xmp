--TEST--
Check for xmp presence
--CREDITS--
Boro Sitnikovski <buritomath@yahoo.com>
--SKIPIF--
<?php if (!extension_loaded("xmp")) print "skip"; ?>
--FILE--
<?php 
echo "xmp extension is available";
?>
--EXPECT--
xmp extension is available
