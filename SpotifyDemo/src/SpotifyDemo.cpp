// SpotifyDemo.cpp : Defines the entry point for the application.
//
#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>
#include <algorithm>

#include "cpr/cpr.h"
#include "nlohmann/json.hpp"

#include "Models.h"
#include "SpotifyCall.hpp"
#include "TokenRetrieval.hpp"

using namespace Models;


template<typename Conf, typename Str>
auto retrieve_client_information(Str &&path)
{
    std::fstream config_file(path, std::ios::in);

    std::stringstream buffer;
    buffer << config_file.rdbuf();

    config_file.close();

    auto client_json = nlohmann::json::parse(buffer.str());
    std::string id(client_json["client_id"].get<std::string>());
    std::string secret(client_json["client_secret"].get<std::string>());

    Conf client_information(std::move(id), std::move(secret));

    return client_information;
}


int main(int argc, char **argv)
{
    std::cout << "Starting SpotfyDemo\n";

    if (argc < 2) {
        std::cout << "Provide path to the client information json file\n";

        return -1;
    }

    std::string path(std::move(argv[1]));

    auto client_info = retrieve_client_information<ClientInformation<std::string>,
        std::string>(std::move(path));

    Manager::TokenRetrieval<> tok_mgr(std::move(client_info));
    auto resp = tok_mgr.retrieve_token<Models::TokenResp<std::string>>();

    Spotify::SpotifyCall<> spotify(std::move(resp));
    std::string song_title("Heave");

    auto albums = spotify.search_songs(song_title);
    spotify.print_songs_to_console(albums);


    return 0;
}
