#include "factory.hpp"

#include <cstdlib>
#include <functional>
#include <iostream>
#include <list>
#include <string>
#include <unordered_map>
#include <vector>

namespace StronglyCoupled
{
    class MusicApp
    {
    public:
        MusicApp() = default;

        void play(const std::string& track_title)
        {
            // creation of product
            SpotifyService music_service("spotify_user", "rjdaslf276%2", 45);

            // usage of product
            std::optional<Track> track = music_service.get_track(track_title);

            if (track)
            {
                std::cout << "Playing track: ";
                for (const auto& note : *track)
                    std::cout << note << ".";
                std::cout << "|\n";
            }
            else
            {
                std::cout << "Track not found!\n";
            }
        }
    };
}

namespace Canonical
{
    class MusicApp
    {
        std::shared_ptr<MusicServiceCreator> music_service_creator_;

    public:
        MusicApp(std::shared_ptr<MusicServiceCreator> music_service_creator)
            : music_service_creator_(music_service_creator)
        {
        }

        void play(const std::string& track_title)
        {
            // creation of the object
            std::unique_ptr<MusicService> music_service = music_service_creator_->create_music_service();

            // usage of the object
            std::optional<Track> track = music_service->get_track(track_title);

            if (track)
            {
                std::cout << "Playing track: ";
                for (const auto& note : *track)
                    std::cout << note << ".";
                std::cout << "|\n";
            }
            else
            {
                std::cout << "Track not found!\n";
            }
        }
    };

    // parametrized factory
    // using MusicServiceFactory = std::unordered_map<std::string, std::shared_ptr<MusicServiceCreator>>;

    class MusicServiceFactory
    {
        std::unordered_map<std::string, std::shared_ptr<MusicServiceCreator>> creators_;

    public:
        bool register_creator(const std::string& id, std::shared_ptr<MusicServiceCreator> creator)
        {
            return creators_.emplace(id, creator).second;
        }

        std::shared_ptr<MusicServiceCreator> create(const std::string& id)
        {
            return creators_.at(id);
        }
    };
}

namespace ModernCpp
{
    class MusicApp
    {
        MusicServiceCreator music_service_creator_;

    public:
        MusicApp(MusicServiceCreator music_service_creator)
            : music_service_creator_(music_service_creator)
        {
        }

        void play(const std::string& track_title)
        {
            // creation of the object
            std::unique_ptr<MusicService> music_service = music_service_creator_();

            // usage of the object
            std::optional<Track> track = music_service->get_track(track_title);

            if (track)
            {
                std::cout << "Playing track: ";
                for (const auto& note : *track)
                    std::cout << note << ".";
                std::cout << "|\n";
            }
            else
            {
                std::cout << "Track not found!\n";
            }
        }
    };

    //parametrized factory

    class MusicServiceFactory
    {
        std::unordered_map<std::string, MusicServiceCreator> creators_;

    public:
        bool register_creator(const std::string& id, MusicServiceCreator creator)
        {
            return creators_.emplace(id, creator).second;
        }

        template<typename TCreator>
        bool register_creator(TCreator creator)
        {
            return creators_.emplace(TCreator::id, creator).second;
        }

        MusicServiceCreator create(const std::string& id)
        {
            return creators_.at(id);
        }
    };
}

std::string parse_config_file()
{
    return "Tidal";
}

int main()
{
    using namespace ModernCpp;

    MusicServiceFactory music_service_factory;
    music_service_factory.register_creator(TidalServiceCreator("tidal_user", "KJH8324d&df"));
    music_service_factory.register_creator("Spotify", SpotifyServiceCreator("spotify_user", "rjdaslf276%2", 45));
    music_service_factory.register_creator("Filesystem", []() { return std::make_unique<FilesystemMusicService>("/music"); });

    // std::string id_from_config = parse_config_file();
    // MusicApp app(music_service_factory.create(id_from_config));
    // app.play("Would?");

    auto spotify_creator = SpotifyServiceCreator("spotify_user", "rjdaslf276%2", 45);
    MusicApp app(spotify_creator);
    app.play("Enter Sandman");
}