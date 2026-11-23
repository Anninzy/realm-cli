#include <algorithm>
#include <cpprealm/sdk.hpp>

namespace realm {
struct RulesetSetting {
    std::optional<std::string> RulesetName;
    int64_t Variant;
    std::string Key;
    std::string Value;
};
REALM_SCHEMA(RulesetSetting, RulesetName, Variant, Key, Value)

struct KeyBinding {
    realm::primary_key<realm::uuid> ID;
    std::optional<std::string> RulesetName;
    std::optional<int64_t> Variant;
    int64_t Action;
    std::optional<std::string> KeyCombination;
};
REALM_SCHEMA(KeyBinding, ID, RulesetName, Variant, Action, KeyCombination)
}

int main()
{
    auto config = realm::db_config();
    config.set_path(std::filesystem::path(std::getenv("HOME")) / ".local/share/osu/client.realm");
    config.set_schema_version(52);
    auto realm = realm::db(config);

    auto rulesetSettings = realm.objects<realm::RulesetSetting>();
    auto keyBinding = realm.objects<realm::KeyBinding>();

    auto clear = [&realm](auto objects) {
        while (objects.size() > 0) {
            auto object = objects[0];
            realm.remove(object);
        }
    };

    auto addRulesetSetting = [&realm](auto rulesetName, auto key, auto value) {
        realm.add(realm::RulesetSetting {
            .RulesetName = rulesetName,
            .Variant = 0,
            .Key = key,
            .Value = value });
    };

    auto addKeyBinding = [&realm](auto id, auto rulesetName, auto variant, auto action, auto keyCombination) {
        realm.add(realm::KeyBinding {
            .ID = realm::uuid(id),
            .RulesetName = rulesetName,
            .Variant = variant,
            .Action = action,
            .KeyCombination = keyCombination });
    };
    auto addOsuBinding = [&realm, addKeyBinding](auto id, auto action, auto keyCombination) {
        addKeyBinding(id, "osu", 0, action, std::to_string(keyCombination));
    };
    auto addManiaBinding = [&realm, addKeyBinding](auto id, auto action, auto keyCombination) {
        addKeyBinding(id, "mania", 10, action, std::to_string(keyCombination));
    };
    auto addBinding = [&realm, addKeyBinding](auto id, auto action, auto keyCombination) {
        addKeyBinding(id, std::nullopt, std::nullopt, action, keyCombination);
    };

    realm.write([clear, rulesetSettings, keyBinding, addRulesetSetting, addOsuBinding, addManiaBinding, addBinding] {
        clear(rulesetSettings);
        clear(keyBinding);

        addRulesetSetting("osu", "HitAnimations", "False");
        addRulesetSetting("taiko", "HitAnimations", "False");
        addRulesetSetting("mania", "ScrollSpeed", "16.6");

        addOsuBinding("b66de2d4-0aff-480a-8bed-d7d6f4b7470c", 0, 110); // 1
        addOsuBinding("5f4e2c90-75e6-463c-af33-020dc4b3f8ef", 1, 111); // 2
        addOsuBinding("aa3a1b2a-a253-44bf-8d07-db559911239d", 2, 112); // 3

        addManiaBinding("0540e695-959e-40fa-97bf-f6c589110c05", 0, 110); // 1
        addManiaBinding("33d65534-91c8-4d35-9f50-39af96a6f41e", 1, 111); // 2
        addManiaBinding("9ef5ceba-4f5a-4959-a68c-6cc0f432d1d3", 2, 112); // 3
        addManiaBinding("14e699a1-fae5-4b8d-9fe1-597ca86d0364", 3, 113); // 4
        addManiaBinding("8004ea15-0b79-41ab-a2db-6b1eb8384956", 4, 85); // C
        addManiaBinding("68f7b106-683e-491e-961d-1766f0385f84", 5, 126); // ,
        addManiaBinding("acd8cc23-d836-4553-916f-46715ec9a2fa", 6, 118); // 9
        addManiaBinding("2598f8b6-c04f-486d-ae2d-dd6bb815d2ed", 7, 109); // 0
        addManiaBinding("7bdd9665-2b0c-4a00-9d93-5d9ba4cfa349", 8, 120); // -
        addManiaBinding("7d83ae18-be58-45eb-a993-286631cea1e3", 9, 121); // =

        addBinding("52d2e05b-eee4-41b2-a41c-cb5d6e9a0de7", 36, "3,100"); // Random CtrlR
        addBinding("9a149ee2-ee1b-4ef8-b91f-4cf6a1005af4", 37, "1,3,100"); // Rewind CtrlShiftR
        addBinding("65b3c394-d1fa-480b-a6c1-65c903194797", 32, ""); // RandomSkin collides with Rewind
        addBinding("4d65ae6b-35af-4834-8a8c-2feeac3172cd", 16, "51"); // Select Space (Default)
        addBinding("3fabfd10-e91e-45cf-9c1c-45d62d2994f2", 16, "49"); // Select Enter (Default)
        addBinding("c8b323f9-6f90-4b6c-aa73-ee173884b991", 16, "52"); // Select Tab
    });

    realm.close();
}
