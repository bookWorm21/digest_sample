#pragma once

#include <userver/utest/using_namespace_userver.hpp>

/// [auth checker factory decl]
#include <userver/server/handlers/auth/auth_checker_factory.hpp>

namespace digest_sample {

class CheckerFactory final
    : public server::handlers::auth::AuthCheckerFactoryBase {
 public:
  server::handlers::auth::AuthCheckerBasePtr operator()(
      const ::components::ComponentContext& context,
      const server::handlers::auth::HandlerAuthConfig& auth_config,
      const server::handlers::auth::AuthCheckerSettings&) const override;
};

class CheckerProxyFactory final
    : public server::handlers::auth::AuthCheckerFactoryBase {
 public:
  server::handlers::auth::AuthCheckerBasePtr operator()(
      const ::components::ComponentContext& context,
      const server::handlers::auth::HandlerAuthConfig& auth_config,
      const server::handlers::auth::AuthCheckerSettings&) const override;
};

}  // namespace digest_sample
/// [auth checker factory decl]
