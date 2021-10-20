#include <absl/flags/flag.h>
#include <absl/flags/parse.h>
#include <fcntl.h>
#include <glog/logging.h>
#include <unistd.h>
#include <zlib.h>

#include <chrono>
#include <iostream>
#include <string_view>

#include "readfq/kseq.h"
#include "fxtract/kseq.hpp"

ABSL_FLAG(std::string, input_file, "SRR072006.fastq.gz", "input file");

typedef void (*benchmark_fn_t)(std::string_view);

KSEQ_INIT(gzFile, gzread)
void readfq(std::string_view input_file) {
  gzFile file = gzopen(input_file.data(), "r");

  kseq_t* seq = kseq_init(file);
  while (kseq_read(seq) >= 0) {
    // noop
    // puts(seq->seq.s);
  }
  kseq_destroy(seq);

  gzclose(file);
}

void fxtract1(std::string_view input_file) {
  using namespace fxtract;
  gzFile file = gzopen(input_file.data(), "r");
  kstream ks(file, gzread);
  kseq seq;
  while (ks.read(seq) > 0) {
    // noop
    // std::cout << seq.seq;
  }
  gzclose(file);
}

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

void gzread(std::string_view input_file) {
  gzFile file = gzopen(input_file.data(), "r");

  constexpr size_t BUFF_SIZE = 4096;
  char buff[BUFF_SIZE];
  while (gzread(file, &buff, BUFF_SIZE) > 0) {
    // noop
  }

  gzclose(file);
}

void benchmark(std::string_view name, benchmark_fn_t fn,
               std::string_view input_file) {
  const auto begin = std::chrono::steady_clock::now();

  fn(input_file);

  const auto end = std::chrono::steady_clock::now();
  const auto time_elapsed =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - begin)
          .count();
  LOG(INFO) << "Time elapsed for " << name << ": " << time_elapsed / 1E3 << "s";
}

int main(int argc, char** argv) {
  // Initialize Google’s logging library.
  google::InitGoogleLogging(argv[0]);
  // Always log.
  FLAGS_logtostderr = 1;

  // Parse command line.
  absl::ParseCommandLine(argc, argv);

  // Validate arguments.
  std::string input_file = absl::GetFlag(FLAGS_input_file);
  CHECK_NE(input_file, "") << "Must specify a input file";

  // Run benchmarks.
  benchmark("warmup", read, input_file);
  benchmark("read", read, input_file);
  benchmark("gzread", gzread, input_file);
  benchmark("readfq", readfq, input_file);
  benchmark("fxtract", fxtract1, input_file);

  return 0;
}