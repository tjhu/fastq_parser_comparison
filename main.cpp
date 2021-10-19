#include <iostream>
#include <fcntl.h>
#include <unistd.h>

#include <glog/logging.h>
#include <absl/flags/flag.h>
#include <absl/flags/parse.h>

#include "readfq/kseq.h"


KSEQ_INIT(int, read)

ABSL_FLAG(std::string, input_file, "SRR072006.fastq", "input file");

int main(int argc, char** argv) {
    // Initialize Google’s logging library.
    google::InitGoogleLogging(argv[0]);

    // Parse command line
    absl::ParseCommandLine(argc, argv);

    std::string input_file = absl::GetFlag(FLAGS_input_file);
    CHECK_NE(input_file, "") << "Must specify a input file";

    int fd = open(input_file.c_str(), O_RDONLY);
    if (fd < 0) {
        perror("Failed to open file.");
        CHECK_GE(fd, 0);
    }

    kseq_t *seq = kseq_init(fd);
    while (kseq_read(seq) >= 0) puts(seq->seq.s);
    kseq_destroy(seq);

    close(fd);

    return 0;
}