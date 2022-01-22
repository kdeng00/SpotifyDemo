#ifndef _TOKENRETRIEVAL_H_
#define _TOKENRETRIEVAL_H_

#include <string>
#include <vector>
#include <algorithm>
#include <utility>

#include "cppcodec/base64_rfc4648.hpp"
#include "cpr/cpr.h"
#include "nlohmann/json.hpp"

#include "Models.h"

namespace Manager
{
    template<typename Str = std::string>
    class TokenRetrieval
    {
    public:
        TokenRetrieval() = default;
        TokenRetrieval(Models::ClientInformation<Str> &info) :
            client_information(std::move(info))
        {
        }
        TokenRetrieval(Models::ClientInformation<Str> &&info) :
            client_information(info)
        {
        }

        template<typename TokResp>
        auto retrieve_token()
        {
            if (this->client_information.is_empty()) {
                return TokResp();
            }

            auto token = make_call<TokResp>();

            if (token.is_empty())
            {
                return TokResp();
            }


            return token;
        }


    private:
        template<typename TokResp>
        auto make_call()
        {
            TokResp resp;

            Str authorization("Basic ");
            authorization.append(encoded_authorization());


            const auto uri = construct_uri(spotify_uri(), spotify_token_endpoint());
            auto r = cpr::Post(cpr::Url{ uri },
                cpr::Parameters{ {"grant_type", "client_credentials" }},
                cpr::Header{ {"Authorization", authorization.c_str()} }
            );

            if (r.status_code == 200) {
                auto token = nlohmann::json::parse(r.text);
                resp.access_token = token["access_token"].get<Str>();
                resp.token_type = token["token_type"].get<Str>();
                resp.expires_in = token["expires_in"].get<int>();
                // resp.scope = token["scope"].get<Str>();
            }


            return resp;
        }

        auto encoded_authorization()
        {
            Str unencoded(client_information.client_id);
            unencoded.append(":");
            unencoded.append(client_information.client_secret);

            std::vector<char> v(unencoded.begin(), unencoded.end());

            return cppcodec::base64_rfc4648::encode(v);
        }

        template<typename Ch = char>
        auto construct_uri(Ch *base, Ch *endpoint)
        {
            Str uri(base);

            if (uri.at(uri.size() - 1) != '/') {
                uri.append("/");
            }

            uri.append(endpoint);

            return uri;
        }

        auto constexpr spotify_uri() noexcept
        {
            return "https://accounts.spotify.com";
        }

        auto constexpr spotify_token_endpoint() noexcept
        {
            return "api/token";
        }

        Models::ClientInformation<Str> client_information;
    };

}

#endif
