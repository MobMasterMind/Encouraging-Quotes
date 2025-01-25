#include <Geode/Geode.hpp>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <filesystem>

using namespace geode::prelude;

// --- Make Lowercase ---
std::string toLower(const std::string &str) {
    std::string lowerStr = str;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), [](unsigned char c) { return std::tolower(c); });
    return lowerStr;
}

// --- Count Contributions ---
int getUserContributionCount(const std::string &username) {
    int contributionCount = 0;
    std::vector<std::string> fileNames = {
        "quotes.txt",
        "jokequotes.txt",
        "kysquotes.txt"
    };

    for (const auto& fileName : fileNames) {
        auto filePath = Mod::get()->getResourcesDir() / fileName;
        std::ifstream file(filePath);

        if (!file.is_open()) {
            log::error("Failed to open quote file: {}", fileName);
            continue;
        }

        std::string line;
        while (std::getline(file, line)) {
            if (!line.empty() && line.find('-') != std::string::npos) {
                auto lastDashPos = line.find_last_of('-');
                std::string author = line.substr(lastDashPos + 1);

                author.erase(author.find_last_not_of(" \t\n\r\f\v") + 1);
                author.erase(0, author.find_first_not_of(" \t\n\r\f\v"));

                if (toLower(author) == toLower(username)) {
                    contributionCount++;
                }
            }
        }
    }

    return contributionCount;
}

std::vector<std::string> contributors;

// --- Contributor Badges ---
std::vector<std::string> getContributorsFromFiles() {
    std::vector<std::string> fileNames = {
        "quotes.txt", 
        "jokequotes.txt", 
        "kysquotes.txt"
    };

    for (const auto& fileName : fileNames) {
        auto filePath = Mod::get()->getResourcesDir() / fileName;
        std::ifstream file(filePath);

        if (!file.is_open()) {
            log::error("Failed to open quote file: {}", fileName);
            continue;
        }

        std::string line;
        while (std::getline(file, line)) {
            if (!line.empty() && line.find('-') != std::string::npos) {
                auto lastDashPos = line.find_last_of('-');
                std::string author = line.substr(lastDashPos + 1);
                
                author.erase(author.find_last_not_of(" \t\n\r\f\v") + 1);
                author.erase(0, author.find_first_not_of(" \t\n\r\f\v"));

                author = toLower(author);

                if (!author.empty() && 
                    std::find(contributors.begin(), contributors.end(), author) == contributors.end()) {
                    contributors.push_back(author);
                }
            }
        }
    }

    return contributors;
}

bool isContributor(const std::string &username) {
    static std::vector<std::string> contributors = getContributorsFromFiles();
    std::string lowerUsername = toLower(username);
    return std::find_if(contributors.begin(), contributors.end(), [&lowerUsername](const std::string &contributor) {
               return toLower(contributor) == lowerUsername;
           }) != contributors.end();
}

void showContributorInfo(const std::string &username) {
    int contributionCount = getUserContributionCount(username);
    std::string description = fmt::format("{} is a Contributor to the Encouraging Quotes Mod\nWisdom Points: {}", username, contributionCount);

    FLAlertLayer::create("Quotes Contributor", description.c_str(), "OK")->show();
}
