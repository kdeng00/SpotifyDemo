#ifndef _SPOTIFYCALL_H_
#define _SPOTIFYCALL_H_

#include <string>
#include <regex>
#include <utility>
#include <vector>

#include "cpr/cpr.h"
#include "nlohmann/json.hpp"

#include "Models.h"

namespace Spotify
{
    template<typename Tok = Models::TokenResp<std::string>>
    class SpotifyCall
    {
    public:
        SpotifyCall() = default;
        SpotifyCall(Tok& tok) : token(tok)
        {
        }
        SpotifyCall(Tok&& tok) : token(std::move(tok))
        {
        }


        template<typename Str = std::string>
        auto search_songs(Str&& title)
        {
            constexpr auto limit = 49;

            std::vector<Models::Album<>> albums;
            albums.reserve(limit);

            std::string search_query("name:");
            search_query.append(delimit_search_query(title));
            

            const auto uri = search_uri();
            const auto authorization = bearer_authorization();

            Str limit_str(std::to_string(limit));

            auto resp = cpr::Get(cpr::Url{ uri },
                cpr::Parameters{ {"type", "track"},
                    {"limit", limit_str},
                    {"q", search_query} },
                cpr::Header{ {"Authorization", authorization.c_str()} });

            if (resp.status_code == 200) {
                parse_albums(albums, std::move(resp.text));
            }


            return albums;
        }

        template<typename Cont = std::vector<Models::Album<std::string>>>
        void print_songs_to_console(Cont &albums)
        {
            std::cout << "Printing songs to the console\n";

            std::for_each(albums.begin(), albums.end(), [](auto album) {
                auto song = album.songs.at(0);

                std::cout << "Title: " << song.title << "\n";
                std::cout << "Artist: " << song.artist << "\n";
                std::cout << "Album: " << album.title << "\n";
                std::cout << "Released Date: " << song.released << "\n";
                std::cout << "Duration (ms): " << song.duration << "\n";
                std::cout << "\n";
                });

            std::cout << "\n";
        }

    private:
        template<typename Str = std::string>
        auto delimit_search_query(Str &&title)
        {
            return std::regex_replace(title, std::regex(" "), "%20");
        }

        auto bearer_authorization()
        {
            std::string authorization("Bearer ");
            authorization.append(this->token.access_token);

            return authorization;
        }

        template<typename Alb = Models::Album<>, typename Vec = std::vector<Alb>,
            typename Str = std::string>
        void parse_albums(Vec &alb, Str &&resp)
        {
            auto obj = nlohmann::json::parse(resp);

            for (auto& item : obj["tracks"]["items"]) {
                auto album_json = item["album"];
                auto artists_json = item["artists"];

                Alb album;
                album.title = album_json["name"].get<std::string>();
                album.released = album_json["release_date"].get<std::string>();

                Models::Song<> song;
                song.title = item["name"].get<std::string>();
                song.duration = item["duration_ms"].get<long>();
                song.artist = artists_json[0]["name"].get<std::string>();
                song.released = album.released;
                song.album = album.title;

                std::vector<Models::Song<>> songs;
                songs.push_back(song);

                album.songs = songs;

                alb.push_back(album);
            }
        }

        auto saved_albums_response_body()
        {
            nlohmann::json obj;

            std::string uri(base_uri());
            uri.append("/");
            uri.append(album_saved());

            std::string auth_header("Bearer ");
            auth_header.append(this->token.access_token);

            auto resp = cpr::Get(cpr::Url(uri),
                cpr::Header{{"Authorization", auth_header.c_str()}}
            );

            if (resp.status_code == 200) {
                obj = nlohmann::json::parse(resp.text);
            }


            return obj;
        }

        template<typename Str = std::string>
        auto search_uri()
        {
            Str uri(base_uri());
            uri.append("/");
            uri.append(search_item_endpoint());

            return uri;
        }

        constexpr auto base_uri() noexcept
        {
            return "https://api.spotify.com/v1";
        }

        constexpr auto album_saved() noexcept
        {
            return "me/albums";
        }
        constexpr auto search_item_endpoint() noexcept
        {
            return "search";
        }

        Tok token;
    };
}

#endif
