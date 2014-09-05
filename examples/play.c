// build with: gcc play.c -lxmp
#include <xmp.h>
#include <stdio.h>

int main() {
    xmp_context ctx;
    struct xmp_frame_info fi;
    int i, j;

    FILE *t = fopen("test.tmp", "wb+");

    ctx = xmp_create_context();
    xmp_load_module(ctx, "../tests/3d.mod");
    xmp_start_player(ctx, 44100, 0);

    for (i=0;i<300;i++) {
        xmp_play_frame(ctx);
        xmp_get_frame_info(ctx, &fi);
        for (j=0;j<fi.buffer_size;j++) {
            fputc(((char *)fi.buffer)[j], t);
        }
    }
    printf("Dumped sound to `test.tmp`\n");
    xmp_end_player(ctx);
    xmp_release_module(ctx);
    xmp_free_context(ctx);

    fclose(t);

    printf("Start playing...\n");
    system("aplay test.tmp --rate=44100 -f cd");
    printf("Deleting file...\n");
    unlink("test.tmp");

    return 0;
}
