// SPDX-License-Identifier: GPL-2.0-or-later

#include <stdio.h>

#include <linux/version.h>
#include <sys/sysinfo.h>

#include <argp.h>
#include <glib.h>

#ifndef __linux__
#error PCSBench must be built for Linux.
#endif

#ifndef LINUX_VERSION_CODE
#warning Cannot check Linux version. Proactive compaction may not be supported.
#elifndef KERNEL_VERSION
#warning Compiler does not support Linux kernel version checking. Proactive compaction may not be supported.
#else
#if LINUX_VERSION_CODE < KERNEL_VERSION(5,9,0)
#warning Your kernel version does not support proactive compaction. PCSBench will fail.
#endif
#endif

#define PRGM_NAME   "pcsbench"
#define PRGM_VERS   "v0.1.0"
#define PAGE_SIZE   (1 << 12)

#define likely(x)   __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)
#define perr()      perror(PRGM_NAME);

// BEGIN ARGP

const char *argp_program_version = PRGM_NAME " " PRGM_VERS;
const char *argp_program_bug_address = "<amy@amyip.net>";
const char doc[] = "test Linux proactive memory compaction";
const char args_doc[] = "";

static struct argp_option options[] = {
    {"bench", 128, "num", OPTION_ARG_OPTIONAL, "Select a proactive compaction model"},
    {0}
};

enum Benchmarks { BENCH_DEFAULT, BENCH_DRG };

struct arguments {
    enum Benchmarks bench;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    struct arguments *arguments = state->input;
    switch (key) {
        case 128:
            if (!strcmp("drg", arg))
                arguments->bench = BENCH_DRG;
            else
                arguments->bench = BENCH_DEFAULT;
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static struct argp argp = { options, parse_opt, args_doc, doc };

// END ARGP

int main(int argc, char **argv) {
    struct sysinfo si;
    struct arguments args = { .bench = BENCH_DEFAULT };

    argp_parse(&argp, argc, argv, 0, 0, &args);

    if (sysinfo(&si))
        perr();

    printf("pcs");

    return 0;
}
