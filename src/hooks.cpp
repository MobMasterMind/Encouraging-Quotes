#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/ProfilePage.hpp>
#include "quotes.cpp"
#include "badges.cpp"

using namespace geode::prelude;

class $modify(MyMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) {
            return false;
        }

        auto menu = this->getChildByID("bottom-menu");

        if (Mod::get()->getSettingValue<bool>("enableButtoninMenuLayer")) {
            auto myButton = CCMenuItemSpriteExtra::create(
                CCSprite::createWithSpriteFrameName("GJ_chatBtn_001.png"),
                this,
                menu_selector(MyMenuLayer::onMyButton)
            );
            myButton->setID("quote-button"_spr);
            menu->addChild(myButton);
            menu->updateLayout();
        }

        return true;
    }

    void onMyButton(CCObject *) {
        FLAlertLayer::create("Quote", getRandomQuote().c_str(), "Alright")->show();
    }
};

class $modify(MyPauseLayer, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();

        auto myButton = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("GJ_chatBtn_001.png"),
            this,
            menu_selector(MyPauseLayer::onMyButton)
        );
        myButton->setID("quote-button"_spr);

        auto menu = this->getChildByID("left-button-menu");
        menu->addChild(myButton);
        menu->updateLayout();
    }

    void onMyButton(CCObject *) {
        FLAlertLayer::create("Quote", getRandomQuote().c_str(), "Alright")->show();
    }
};

class $modify(CustomProfilePage, ProfilePage) {
    struct Fields {
        bool m_hasBadge;
    };

    void loadPageFromUserInfo(GJUserScore* score) {
        ProfilePage::loadPageFromUserInfo(score);

        // Check if badges are enabled
        bool badgesEnabled = Mod::get()->getSettingValue<bool>("showcontributorbadges");

        if (!badgesEnabled) {
            return;
        }

        if (m_fields->m_hasBadge) return;

        auto usernameMenu = m_mainLayer->getChildByID("username-menu");
        if (!usernameMenu) return;

        auto contributorBadge = CCSprite::create("quotecontributorbadge.png"_spr);
        contributorBadge->setID("contributorbadge-badge");

        auto badgeButton = CCMenuItemSpriteExtra::create(
            contributorBadge,
            this,
            menu_selector(CustomProfilePage::onBadgeClicked)
        );
        badgeButton->setTag(0);
        badgeButton->setID("contributorbadge-badge"_spr);

        usernameMenu->addChild(badgeButton);
        usernameMenu->updateLayout();

        m_fields->m_hasBadge = true;
        
    }

    void onBadgeClicked(CCObject* sender) {
        auto username = m_score->m_userName;
        showContributorInfo(username);
    }
};