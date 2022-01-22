#ifndef _MODELS_H_
#define _MODELS_H_

#include <string>
#include <utility>

namespace Models
{
    template<typename Str = std::string>
    class SpotifyVendorBase
    {
    protected:
        void vendorInitialize()
        {
            disclaimer.assign("By Spotify");
        }

        Str disclaimer;
    };

    template<typename Str = std::string,
        typename Num = int,
        typename Dur = long>
    class Song : public SpotifyVendorBase<Str>
    {
    public:
        Song()
        {
            vendorInitialize();
        }

        Str title;
        Str artist;
        Str album;
        Str released;
        Dur duration;
    };

    template<typename Str = std::string,
        typename Num = int>
    class Album : public SpotifyVendorBase<Str>
    {
    public:
        Album()
        {
            vendorInitialize();
        }

        Str title;
        Str released;
        std::vector<Song<>> songs;
    };

    template<typename Str = std::string>
    class ClientInformation
    {
    public:
        ClientInformation() = default;
        ClientInformation(Str& id, Str& secret) : 
            client_id(std::move(id)), client_secret(std::move(secret))
        {
        }
        ClientInformation(Str&& id, Str&& secret) :
            client_id(id), client_secret(secret)
        {
        }

        auto is_empty()
        {
            return (client_id.empty() && client_secret.empty()) ? true : false;
        }

        Str client_id;
        Str client_secret;
    };

    template<typename Str = std::string,
        typename Num = int>
    class TokenResp
    {
    public:
        auto is_empty() noexcept
        {
            auto result = this->access_token.empty();
            return result;
        }

        Str access_token;
        Str token_type;
        Num expires_in;
        // Not being used as of 2022-01-21
        [[deprecated("This field will not be used")]]
        Str scope;
    };
}

#endif
