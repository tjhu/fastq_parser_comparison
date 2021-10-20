#include <iostream>
#include <fcntl.h>
#include <chrono>
#include <unistd.h>
#include <string_view>

#include <glog/logging.h>
#include <absl/flags/flag.h>
#include <absl/flags/parse.h>

#include "readfq/kseq.h"


KSEQ_INIT(int, read)

ABSL_FLAG(std::string, input_file, "SRR072006.fastq", "input file");

typedef void(*benchmark_fn_t)(std::string_view);

void read(std::string_view input_file) {
    int fd = open(input_file.data(), O_RDONLY);
    PCHECK(fd >= 0) << "Failed to open file: " << input_file;

    constexpr size_t BUFF_SIZE = 4096;
    char buff[BUFF_SIZE];
    while (read(fd, &buff, BUFF_SIZE) > 0) {
        // noop
    }

    close(fd);
}

void readfq(std::string_view input_file) {
    int fd = open(input_file.data(), O_RDONLY);
    PCHECK(fd >= 0) << "Failed to open file: " << input_file;

    kseq_t *seq = kseq_init(fd);
    while (kseq_read(seq) >= 0) {
        // noop
    }
    kseq_destroy(seq);

    close(fd);
}

void benchmark(std::string_view name, benchmark_fn_t fn, std::string_view input_file) {
    const auto begin = std::chrono::steady_clock::now();

    fn(input_file);

    const auto end = std::chrono::steady_clock::now();
    const auto time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    LOG(INFO) << "Time elapsed for " << name << ": " << time_elapsed / 1E3 << "s";
}


int main(int argc, char** argv) {
    // Initialize Google’s logging library.
    google::InitGoogleLogging(argv[0]);
    // Always log.
    FLAGS_logtostderr = 1;

    // Parse command line
    absl::ParseCommandLine(argc, argv);

    std::string input_file = absl::GetFlag(FLAGS_input_file);
    CHECK_NE(input_file, "") << "Must specify a input file";

    benchmark("read", read, input_file);
    benchmark("readfq", readfq, input_file);

    return 0;
}