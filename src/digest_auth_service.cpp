#include "auth_digest.hpp"
#include "add_user_service.hpp"
#include "queries.hpp"
#include "user_info.hpp"

#include <userver/clients/dns/component.hpp>
#include <userver/clients/http/component.hpp>
#include <userver/components/minimal_server_component_list.hpp>
#include <userver/server/handlers/auth/digest/auth_checker_settings_component.hpp>
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/server/handlers/tests_control.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/storages/secdist/component.hpp>
#include <userver/storages/secdist/provider_component.hpp>
#include <userver/testsuite/testsuite_support.hpp>
#include <userver/utils/daemon_run.hpp>

namespace digest_sample {

/// [request context]
class Hello final : public server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-hello";

  using HttpHandlerBase::HttpHandlerBase;

  std::string HandleRequestThrow(
      const server::http::HttpRequest&,
      server::request::RequestContext&) const override {
    return "Hello world";
  }
};
/// [request context]

class AddDefaultUserHandler final : public server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-add-default-user";

  using HttpHandlerBase::HttpHandlerBase;

  AddDefaultUserHandler(const components::ComponentConfig& config,
                        const components::ComponentContext& context)
      : HttpHandlerBase(config, context),
        pg_cluster_(context.FindComponent<components::Postgres>("auth-database")
                        .GetCluster()) {}

  std::string HandleRequestThrow(
      const server::http::HttpRequest& request,
      server::request::RequestContext& context) const override {
    pg_cluster_->Execute(
        storages::postgres::ClusterHostType::kSlave, kInsertCreds,
        "username-sha256", "some-old-nonce",
        "88569f704e305126abe6afb3c6051405bb99852f332a768686bb6502ffe1667a");
    return "Inserting";
  }

 private:
  storages::postgres::ClusterPtr pg_cluster_;
};

}  // namespace digest_sample

/// [auth checker registration]
int main(int argc, const char* const argv[]) {
  server::handlers::auth::RegisterAuthCheckerFactory(
      "digest", std::make_unique<digest_sample::CheckerFactory>());
  /// [auth checker registration]

  /// [main]
  const auto component_list =
      components::MinimalServerComponentList()
          .Append<components::Postgres>("auth-database")
          .Append<digest_sample::Hello>()
          .Append<components::TestsuiteSupport>()
          .Append<components::HttpClient>()
          .Append<server::handlers::TestsControl>()
          .Append<clients::dns::Component>()
          .Append<components::Secdist>()
          .Append<components::DefaultSecdistProvider>()
          .Append<
              server::handlers::auth::digest::AuthCheckerSettingsComponent>()
          .Append<digest_sample::AddDefaultUserHandler>()
          .Append<digest_sample::AddUserHandler>();
  return utils::DaemonMain(argc, argv, component_list);
  /// [main]
}
/// [Postgres digest auth service sample - main]
