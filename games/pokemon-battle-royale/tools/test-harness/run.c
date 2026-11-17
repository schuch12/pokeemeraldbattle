// headless mGBA runner: run <rom> <script> ; script lines: run N | press KEYS N | shot file.ppm | savefile path (before start)
#include <mgba/core/core.h>
#include <mgba/core/config.h>
#include <mgba/core/log.h>
#include <mgba/gba/core.h>
#include <mgba/internal/gba/input.h>
#include <mgba-util/vfs.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define VA (core->busRead32(core, pa) + 0x139C + vi * 2)
static void nolog(struct mLogger* l, int c, enum mLogLevel lv, const char* f, va_list a) {}
static struct mLogger logger = { .log = nolog };
static int keymask(const char* s) {
    const char* names[] = {"A","B","SELECT","START","RIGHT","LEFT","UP","DOWN","R","L"};
    int m = 0; char buf[128]; strncpy(buf, s, 127); buf[127]=0;
    for (char* t = strtok(buf, "+"); t; t = strtok(NULL, "+"))
        for (int i = 0; i < 10; i++) if (!strcmp(t, names[i])) m |= 1 << i;
    return m;
}
int main(int argc, char** argv) {
    mLogSetDefaultLogger(&logger);
    struct mCore* core = GBACoreCreate();
    core->init(core);
    mCoreInitConfig(core, NULL);
    unsigned w, h; core->desiredVideoDimensions(core, &w, &h);
    color_t* buf = malloc(w * h * sizeof(color_t));
    core->setVideoBuffer(core, buf, w);
    mCoreLoadFile(core, argv[1]);
    if (argc > 3) { struct VFile* vf = VFileOpen(argv[3], O_CREAT | O_RDWR); core->loadSave(core, vf); }
    core->reset(core);
    FILE* f = fopen(argv[2], "r"); char cmd[64], arg[256]; int n;
    char line[512];
    while (fgets(line, sizeof line, f)) {
        if (sscanf(line, "%63s", cmd) != 1 || cmd[0]=='#') continue;
        if (!strcmp(cmd, "run")) { sscanf(line, "%*s %d", &n); for (int i = 0; i < n; i++) core->runFrame(core); }
        else if (!strcmp(cmd, "press")) { n = 4; sscanf(line, "%*s %255s %d", arg, &n); core->setKeys(core, keymask(arg)); for (int i = 0; i < n; i++) core->runFrame(core); core->setKeys(core, 0); for (int i = 0; i < 12; i++) core->runFrame(core); }
        else if (!strcmp(cmd, "shot")) { sscanf(line, "%*s %255s", arg); FILE* o = fopen(arg, "wb"); fprintf(o, "P6\n%u %u\n255\n", w, h);
            for (unsigned i = 0; i < w * h; i++) { unsigned c = buf[i]; unsigned char p[3] = { c & 0xFF, (c >> 8) & 0xFF, (c >> 16) & 0xFF }; fwrite(p, 1, 3, o); } fclose(o); }
        else if (!strcmp(cmd, "walk")) { n = 1; sscanf(line, "%*s %255s %d", arg, &n); unsigned pa = strtoul(getenv("SB1PTR"), 0, 16);
            for (int t = 0; t < n; t++) { unsigned p = core->busRead32(core, pa); int x0 = core->busRead16(core, p), y0 = core->busRead16(core, p + 2), k = 0;
                core->setKeys(core, keymask(arg));
                for (k = 0; k < 60; k++) { core->runFrame(core); p = core->busRead32(core, pa); if (core->busRead16(core, p) != x0 || core->busRead16(core, p + 2) != y0) break; }
                if (k == 60) { printf("walk %s blocked at %d,%d (step %d)\n", arg, x0, y0, t); break; } }
            core->setKeys(core, 0); for (int i = 0; i < 24; i++) core->runFrame(core); }
        else if (!strcmp(cmd, "pos")) { unsigned pa = strtoul(getenv("SB1PTR"), 0, 16); unsigned p = core->busRead32(core, pa); printf("pos %d,%d map %d.%d\n", core->busRead16(core, p), core->busRead16(core, p + 2), core->busRead8(core, p + 4), core->busRead8(core, p + 5)); }
        else if (!strcmp(cmd, "money")) { sscanf(line, "%*s %d", &n); unsigned pa = strtoul(getenv("SB1PTR"), 0, 16); unsigned p = core->busRead32(core, pa); unsigned p2 = core->busRead32(core, pa + 4);
            unsigned key = core->busRead32(core, p2 + 0xAC); core->busWrite32(core, p + 0x490, (unsigned) n ^ key); }
        else if (!strcmp(cmd, "mash")) { int gap = 30; n = 10; sscanf(line, "%*s %255s %d %d", arg, &n, &gap);
            for (int t = 0; t < n; t++) { core->setKeys(core, keymask(arg)); for (int i = 0; i < 4; i++) core->runFrame(core); core->setKeys(core, 0); for (int i = 0; i < gap; i++) core->runFrame(core); } }
        else if (!strcmp(cmd, "getmoney")) { unsigned pa = strtoul(getenv("SB1PTR"), 0, 16); unsigned p = core->busRead32(core, pa); unsigned p2 = core->busRead32(core, pa + 4);
            printf("money %u vars tier=%u num=%u cleared=%u\n", core->busRead32(core, p + 0x490) ^ core->busRead32(core, p2 + 0xAC), core->busRead16(core, p + 0x139C + 0xF7*2), core->busRead16(core, p + 0x139C + 0xF8*2), core->busRead16(core, p + 0x139C + 0xF9*2)); }
        else if (!strcmp(cmd, "poke8")) { unsigned a, v; sscanf(line, "%*s %x %x", &a, &v); core->busWrite8(core, a, v); }
        else if (!strcmp(cmd, "poke16")) { unsigned a, v; sscanf(line, "%*s %x %x", &a, &v); core->busWrite16(core, a, v); }
        else if (!strcmp(cmd, "mashuntil")) { unsigned a; int gap = 30; sscanf(line, "%*s %255s %x %d", arg, &a, &gap); unsigned v0 = core->busRead32(core, a); int t;
            for (t = 0; t < 200 && core->busRead32(core, a) == v0; t++) { core->setKeys(core, keymask(arg)); for (int i = 0; i < 4; i++) core->runFrame(core); core->setKeys(core, 0); for (int i = 0; i < gap && core->busRead32(core, a) == v0; i++) core->runFrame(core); }
            printf("mashuntil: %d presses, %08x -> %08x\n", t, v0, core->busRead32(core, a)); }
        else if (!strcmp(cmd, "name")) { unsigned pa = strtoul(getenv("SB1PTR"), 0, 16); unsigned p2 = core->busRead32(core, pa + 4); printf("name:"); for (int i = 0; i < 8; i++) printf(" %02x", core->busRead8(core, p2 + i)); printf("\n"); }
        else if (!strcmp(cmd, "opts")) { unsigned pa = strtoul(getenv("SB1PTR"), 0, 16); unsigned p2 = core->busRead32(core, pa + 4); core->busWrite8(core, p2 + 0x15, core->busRead8(core, p2 + 0x15) | 6); core->busWrite8(core, p2 + 0x14, (core->busRead8(core, p2 + 0x14) & 0xF8) | 2); }
        else if (!strcmp(cmd, "mashvar")) { unsigned vi; int gap = 30; sscanf(line, "%*s %255s %x %d", arg, &vi, &gap); unsigned pa = strtoul(getenv("SB1PTR"), 0, 16);
            unsigned v0 = core->busRead16(core, VA); int t;
            for (t = 0; t < 400 && core->busRead16(core, VA) == v0; t++) { core->setKeys(core, keymask(arg)); for (int i = 0; i < 4; i++) core->runFrame(core); core->setKeys(core, 0); for (int i = 0; i < gap; i++) core->runFrame(core);
                FILE* bf = fopen(getenv("BUFF") ? getenv("BUFF") : "buff2.txt", "r"); char bl[128]; unsigned ba, bv; while (bf && fgets(bl, sizeof bl, bf)) { if (sscanf(bl, "poke16 %x %x", &ba, &bv) == 2) core->busWrite16(core, ba, bv); else if (sscanf(bl, "poke8 %x %x", &ba, &bv) == 2) core->busWrite8(core, ba, bv); } if (bf) fclose(bf); }
            printf("mashvar: %d presses, var %x: %u -> %u\n", t, vi, v0, core->busRead16(core, VA)); }
        else if (!strcmp(cmd, "setvar")) { unsigned vi, v; sscanf(line, "%*s %x %u", &vi, &v); unsigned pa = strtoul(getenv("SB1PTR"), 0, 16); core->busWrite16(core, VA, v); }
        else if (!strcmp(cmd, "peek")) { unsigned a, len=1; sscanf(line, "%*s %x %u", &a, &len); printf("%08x:", a); for (unsigned i=0;i<len;i++) printf(" %02x", core->busRead8(core, a+i)); printf("\n"); }
    }
    return 0;
}
