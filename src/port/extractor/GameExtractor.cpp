#include "GameExtractor.h"

#include <fstream>

#include "Context.h"
#include "spdlog/spdlog.h"
#include "portable-file-dialogs.h"
#include <port/Engine.h>

std::unordered_map<std::string, std::string> mGameList = {
    { "d8b1088520f7c5f81433292a9258c1184afa1457", "Star Fox 64 (U) (V1.0)" },
    { "63b69f0ef36306257481afc250f9bc304c7162b2", "Star Fox 64 (U) (V1.0) (Uncompressed)" },
    { "09f0d105f476b00efa5303a3ebc42e60a7753b7a", "Star Fox 64 (U) (V1.1)" },
    { "f7475fb11e7e6830f82883412638e8390791ab87", "Star Fox 64 (U) (V1.1) (Uncompressed)" },
};

bool GameExtractor::SelectGameFromUI() {
#if !defined(__IOS__) || !defined(__ANDROID__) || !defined(__SWITCH__)
    auto selection = pfd::open_file("Select a file", ".", { "N64 Roms", "*.z64 *.n64 *.v64" }).result();

    if (selection.empty()) {
        return false;
    }

    this->mGamePath = selection[0];
#else
    this->mGamePath = Ship::Context::GetPathRelativeToAppDirectory("baserom.us.rev1.z64");
#endif

    std::ifstream file(this->mGamePath, std::ios::binary);
    this->mGameData = std::vector<uint8_t>( std::istreambuf_iterator( file ), {} );
    file.close();
    return true;
}

std::optional<std::string> GameExtractor::ValidateChecksum() const {
    const auto rom = new N64::Cartridge(this->mGameData);
    rom->Initialize();
    auto hash = rom->GetHash();
    
    if (mGameList.find(hash) == mGameList.end()) {
        return std::nullopt;
    }

    return mGameList[hash];
}

bool GameExtractor::GenerateOTR() const {
    Companion::Instance = new Companion(this->mGameData, ArchiveType::O2R, false);

    try {
        Companion::Instance->Init(ExportType::Binary);
    } catch (const std::exception& e) {
        return false;
    }

    return true;
}