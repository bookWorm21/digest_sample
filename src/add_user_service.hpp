#pragma once

#include "auth_digest.hpp"
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
    class AddUserHandler final : public server::handlers::HttpHandlerBase {
    public:
    static constexpr std::string_view kName = "handler-add-user";

    using HttpHandlerBase::HttpHandlerBase;
    
    AddUserHandler(const components::ComponentConfig& config,
                    const components::ComponentContext& context) : HttpHandlerBase(config, context), 
                    pg_cluster_(context.FindComponent<components::Postgres>("auth-database")
                            .GetCluster())
                            {}

    std::string HandleRequestThrow(
        const server::http::HttpRequest& request,
        server::request::RequestContext& context) const override {
        auto username = request.GetArg("username");
        auto ha1 = request.GetArg("ha1");
        pg_cluster_->Execute(storages::postgres::ClusterHostType::kMaster, kInsertCreds, 
        username, 
        "some-old-nonce",
        ha1);
        return "Success insert new user data";
    }

    private:
    storages::postgres::ClusterPtr pg_cluster_;
    };
} // namespace digest_sample