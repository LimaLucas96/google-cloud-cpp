// Copyright 2019 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef GOOGLE_CLOUD_CPP_GOOGLE_CLOUD_STORAGE_BENCHMARKS_BENCHMARK_UTILS_H_
#define GOOGLE_CLOUD_CPP_GOOGLE_CLOUD_STORAGE_BENCHMARKS_BENCHMARK_UTILS_H_

#include "google/cloud/internal/random.h"
#include <chrono>
#include <functional>
#include <string>
#if GOOGLE_CLOUD_CPP_HAVE_GETRUSAGE
#include <sys/resource.h>
#endif  // GOOGLE_CLOUD_CPP_HAVE_GETRUSAGE

namespace google {
namespace cloud {
namespace storage_benchmarks {
/**
 * Create a random bucket name.
 *
 * Most benchmarks need to create a bucket to storage their data. Using a random
 * bucket name makes it possible to run different instances of the benchmark
 * without interacting with previous or concurrent instances.
 */
std::string MakeRandomBucketName(google::cloud::internal::DefaultPRNG& gen,
                                 std::string const& prefix);

/// Create a random object name.
std::string MakeRandomObjectName(google::cloud::internal::DefaultPRNG& gen);

/// Create a random local filename.
std::string MakeRandomFileName(google::cloud::internal::DefaultPRNG& gen);

/// Create a random chunk of data of a prescribed size.
std::string MakeRandomData(google::cloud::internal::DefaultPRNG& gen,
                           std::size_t desired_size);

constexpr std::int64_t kKiB = 1024;
constexpr std::int64_t kMiB = 1024 * kKiB;
constexpr std::int64_t kGiB = 1024 * kMiB;
constexpr std::int64_t kTiB = 1024 * kGiB;

constexpr std::int64_t kKB = 1000;
constexpr std::int64_t kMB = 1000 * kKB;
constexpr std::int64_t kGB = 1000 * kMB;
constexpr std::int64_t kTB = 1000 * kGB;

/// Parse a string as a byte size, with support for unit suffixes.
std::int64_t ParseSize(std::string const& val);

/// Parse a string as a duration with support for hours (h), minutes (m), or
/// second (s) suffixes.
std::chrono::seconds ParseDuration(std::string const& val);

/// Parse a string as a boolean, with a default value if the string is empty.
bool ParseBoolean(std::string const& val, bool default_value);

/// Defines a command-line option.
struct OptionDescriptor {
  using OptionParser = std::function<void(std::string const&)>;

  std::string option;
  std::string help;
  OptionParser parser;
};

/// Build the `Usage` string from a list of command-line option descriptions.
std::string BuildUsage(std::vector<OptionDescriptor> const& desc,
                       std::string const& command_path);

/**
 * Parse @p argv using the descriptions in @p desc, return unparsed arguments.
 */
std::vector<std::string> OptionsParse(std::vector<OptionDescriptor> const& desc,
                                      std::vector<std::string> argv);

class SimpleTimer {
 public:
  SimpleTimer() = default;

  /// Start the timer, call before the code being measured.
  void Start();

  /// Stop the timer, call after the code being measured.
  void Stop();

  //@{
  /**
   * @name Measurement results.
   *
   * @note The values are only valid after calling Start() and Stop().
   */
  std::chrono::microseconds elapsed_time() const { return elapsed_time_; }
  std::chrono::microseconds cpu_time() const { return cpu_time_; }
  std::string const& annotations() const { return annotations_; }
  //@}

  static bool SupportPerThreadUsage();

 private:
  std::chrono::steady_clock::time_point start_;
  std::chrono::microseconds elapsed_time_;
  std::chrono::microseconds cpu_time_;
#if GOOGLE_CLOUD_CPP_HAVE_GETRUSAGE
  struct rusage start_usage_;
#endif  // GOOGLE_CLOUD_CPP_HAVE_GETRUSAGE
  std::string annotations_;
};

}  // namespace storage_benchmarks
}  // namespace cloud
}  // namespace google

#endif  // GOOGLE_CLOUD_CPP_GOOGLE_CLOUD_STORAGE_BENCHMARKS_BENCHMARK_UTILS_H_
