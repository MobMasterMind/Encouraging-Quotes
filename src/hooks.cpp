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
    void loadPageFromUserInfo(GJUserScore* score) {
        ProfilePage::loadPageFromUserInfo(score);

        // Check if badges are enabled
        bool badgesEnabled = Mod::get()->getSettingValue<bool>("showcontributorbadges");

        if (!badgesEnabled) {
            return;
        }

        auto layer = m_mainLayer;
        CCMenu* username_menu = static_cast<CCMenu*>(layer->getChildByIDRecursive("username-menu"));

        if (isContributor(score->m_userName)) {
            auto contributorBadge = CCSprite::create("quotecontributorbadge.png"_spr);
            contributorBadge->setID("contributorbadge-badge");

            auto badgeButton = CCMenuItemSpriteExtra::create(
                contributorBadge,
                this,
                menu_selector(CustomProfilePage::onBadgeClicked)
            );
            badgeButton->setTag(0);

            username_menu->addChild(badgeButton, 10);
            username_menu->updateLayout();
        }
    }

    void onBadgeClicked(CCObject* sender) {
        auto username = m_score->m_userName;
        showContributorInfo(username);
    }
};