<?php
$ctx = xmp_create_context();
xmp_load_module($ctx, dirname(__FILE__)."/../tests/3d.mod");
xmp_start_player($ctx, 44100, 0);

for ($i=0;$i<300;$i++) {
    xmp_play_frame($ctx);
    $info = xmp_get_frame_info($ctx);
    file_put_contents("test.tmp", $info['buffer'], FILE_APPEND);
}
echo "Dumped sound to `test.tmp`\n";

xmp_end_player($ctx);
xmp_release_module($ctx);
xmp_free_context($ctx);

echo "Start playing...\n";
exec("aplay test.tmp --rate=44100 -f cd");
echo "Deleting file...\n";
unlink("test.tmp");
?>
