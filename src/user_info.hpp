#pragma once

#include <chrono>
#include <string>

#include <userver/storages/postgres/io/chrono.hpp>

namespace digest_sample {

struct UserDbInfo {
  std::string username;
  std::string nonce;
  userver::storages::postgres::TimePointTz timestamp;
  int nonce_count{};
  std::string ha1;
};

}  // namespace digest_sample
