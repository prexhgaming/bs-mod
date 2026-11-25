#pragma once
// ============================================================
// Class.h — Bloodstrike SDK Data Reference (C++ Port)
// Converted from Python SDK dump:
//   gclient.util.gun_skin_util
//   gclient.data.gun_skin_template_base_data
//   gclient.data.gun_skin_template_diff_data
//   gclient.util.gun_ui_util
//   gclient.gamesystem.uigunsmith.gunsmith_unlock_condition_list
// ============================================================

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cstdint>

// ─────────────────────────────────────────────────────────────────────────────
// SECTION 1 — GUN SKIN SHADER PARAMS
// Source: gclient.util.gun_skin_util → GUN_SKIN_SHADER_PARAMS
// ─────────────────────────────────────────────────────────────────────────────
namespace GunSkinShader {

    // Shader parameter names valid untuk GunSkin material
    inline const std::unordered_set<std::string> PARAMS = {
        "cMetallic",
        "OverlayMetallic",
        "OverlayMaskTiling",
        "cOverlayNormalBumpIns",
        "OverlayNormalTiling",
        "EnableMatcap",
        "OverlayMipBias",
        "OverlayOffsetY",
        "OverlayTimeScaleY",
        "NormalMipBias",
        "cBloomScale",
        "EnableOverlayMap",
        "OverlayColor",
        "NoiseStepMin",
        "EnableMixmap",
        "OverlayRoughness",
        "OverlayOffsetX",
        "cEnvStrength",
        "cAO",
        "EnableOverlayNormal",
        "OverlayTimeScaleX",
        "EnableOverlayMatChange",
        "OverlayNormalTimeScaleX",
        "EnableDecal",
        "BasemapMipBias",
        "EnableOverlay",
        "EnableNoiseMap",
    };

    // Texture parameter names
    inline const std::unordered_set<std::string> TEXTURE_PARAMS = {
        "tOverlayNormal",
        "tOverlayMap",
    };

    // Texture → switch param mapping
    // Source: TextureSwitches dict
    inline const std::unordered_map<std::string, std::string> TEXTURE_SWITCHES = {
        { "tOverlayMap",    "EnableOverlayMap"    },
        { "tOverlayNormal", "EnableOverlayNormal" },
    };

    // Skin ID yang tidak butuh EMI
    // Source: GUN_SKIN_NOT_NEED_EMI
    inline const std::vector<uint64_t> NO_EMI_SKIN_IDS = {
        5100310190ULL,
    };

    inline bool IsValidShaderParam(const std::string& param) {
        return PARAMS.count(param) > 0;
    }
    inline bool IsTextureParam(const std::string& param) {
        return TEXTURE_PARAMS.count(param) > 0;
    }
    inline std::string GetTextureSwitch(const std::string& tex_param) {
        auto it = TEXTURE_SWITCHES.find(tex_param);
        return (it != TEXTURE_SWITCHES.end()) ? it->second : "";
    }

} // namespace GunSkinShader


// ─────────────────────────────────────────────────────────────────────────────
// SECTION 2 — GUN SKIN TEMPLATE DATA (base + diff)
// Source: gun_skin_template_base_data + gun_skin_template_diff_data
// ─────────────────────────────────────────────────────────────────────────────
namespace GunSkinTemplate {

    // Tipe skin
    enum class SkinType : int {
        BASE     = 0,  // skin biasa / default
        PREMIUM  = 1,  // skin premium (punya model replace dll)
    };

    // Struktur satu entry skin template
    struct SkinTemplateEntry {
        uint64_t    id              = 0;
        int         gun_id          = 0;
        std::string skin_name;
        SkinType    gun_skin_type   = SkinType::BASE;
        std::string dlc_id;
        std::vector<int> dlc_pack_ids;

        // Part lists
        std::vector<int> gun_skin_part_type_list;
        std::vector<int> gun_skin_part_mat_list;
        std::vector<int> gun_skin_part_tiling_list;
        std::vector<int> gun_skin_part_model_list;
        std::vector<int> gun_skin_attacment_id_list;
        std::vector<int> gun_skin_present_attachment_id_list;

        // Model replace IDs
        int model_id1_replace_id        = 0;
        int optic_b_model_replace_id    = 0;
        int optic_b_off_model_replace_id = 0;
        int optic_f_off_model_replace_id = 0;
        int skin_optic_id               = 0;
        int skin_lobby_default_optic    = 0;

        // Animation names
        std::string SkinInspectAnimName;
        std::string SkinReloadAnimName;
        std::string SkinEmptyReloadAnimName;
        std::string SkinAdsReloadAnimName;
        std::string SkinAdsEmptyReloadAnimName;
        std::string SkinRaiseNewAnimName;
        std::string SkinIdleActivePoseAnimName;
        std::string SkinIdlePoseYawAnimName;
        std::string SkinLobbyIdleActivePoseAnimName;
        std::string SkinReloadVirtualAnimName;
        std::string SkinEmptyReloadVirtualAnimName;
        std::string AdsUpAnimName;
        std::string AdsDownAnimName;
        std::string ExtraModelKillTeamAnimName;
        std::string TeamDieActionPose;
        std::string SpecialDieActionPose;

        // FX / SFX
        int         new_fire_sfx_id          = 0;
        int         new_fire_ads_sfx_id      = 0;
        std::vector<int> fire_sfx;
        int         fps_tracer_bullet_id      = 0;
        int         fps_simple_tracer_bullet_id = 0;
        int         fps_adstracer_bullet_id   = 0;
        int         fps_simple_adstracer_bullet_id = 0;
        int         tps_tracer_bullet_id      = 0;
        int         tps_simple_tracer_bullet_id = 0;
        int         optic_b_fx               = 0;

        // Icons & kill tip
        uint64_t    icon_outline_replace     = 0;
        uint64_t    kill_tip_icon_replace    = 0;
        int         kill_team_extra_model_id = 0;
        int         kill_team_extra_hexie_model_id = 0;

        // Flags
        bool        fps_trace_bullet_from_tip = false;
        bool        CanInspectionPlayActive   = false;
        int         forbidden_decal           = 0;
        int         recevier_show_ammo        = 0;
        int         use_dynamic_box           = 0;
        std::string IsReducedRecoil;

        // Offsets
        std::vector<float> skin_iron_sight_bone_pos;
        std::vector<float> skin_high_bone_sight_offset;
        std::vector<float> skin_sprint_offset;

        // Misc
        float       gun_smith_zoom_scale     = 1.0f;
        float       screenshot_scale         = 1.0f;
        float       SkinInspectPlaybackSpeed = 1.0f;
        float       team_die_action_disappear_time = 0.0f;
        int         skin_upper_rail_model_id = 0;
        std::vector<int> hide_upper_rail_model_part_list;
        std::vector<int> extra_ballistic_shoot_idx_list;
        int         extra_ballistic_tracer_bullet_id = 0;
        std::string Team_kill_mat;
        float       TeamDieActionPoseTime    = 0.0f;
        std::vector<int> dlc_attach_model_list;
        std::vector<int> gun_skin_hexie_part_model_list;
    };

    // Sample data — 2 entry dari base_data sebagai contoh
    // Tambah entry lain sesuai kebutuhan
    inline const std::vector<SkinTemplateEntry> BASE_TEMPLATES = {
        {
            .id             = 510100180ULL,
            .gun_id         = 51,
            .skin_name      = "HK416 - 冰蓝蜜桃",
            .gun_skin_type  = SkinType::BASE,
            .gun_skin_part_type_list  = {1,2,3,4,5,6,7,8},
            .gun_skin_part_tiling_list= {1,1,1,1,1,1,1,1},
            .gun_skin_part_mat_list   = {288,288,288,288,288,288,288,288},
        },
        {
            .id             = 350100620ULL,
            .gun_id         = 35,
            .skin_name      = "Uzi - 浮纹深蓝",
            .gun_skin_type  = SkinType::BASE,
            .gun_skin_part_type_list  = {1,2,3,4,5,6,7,8},
            .gun_skin_part_tiling_list= {1,1,1,1,1,1,1,1},
            .gun_skin_part_mat_list   = {728,728,728,728,728,728,728,728},
        },
        {
            .id             = 3911990281ULL,
            .gun_id         = 39,
            .skin_name      = "QBZ95 - V7驾驶舱占位 - 至臻 LV3",
            .gun_skin_type  = SkinType::PREMIUM,
            .dlc_id         = "dlcid_gunskintemplate_3911990281",
            .dlc_pack_ids   = {2001},
            .gun_skin_part_type_list  = {1,2,4,5,7,8},
            .gun_skin_part_model_list = {9390283,9390285,0,9390287,0,0},
            .gun_skin_attacment_id_list         = {390101,390201,390401,390501,390701,390801},
            .gun_skin_present_attachment_id_list= {390101,390204,390311,390403,390504,390707,390810,391202},
            .model_id1_replace_id    = 93902810,
            .optic_b_model_replace_id= 93902811,
            .skin_optic_id           = 2390282,
            .skin_lobby_default_optic= 390301,
            .SkinInspectAnimName     = "QBZ95HD_1_inspect",
            .SkinReloadAnimName      = "QBZ95v26_1_reload",
            .SkinEmptyReloadAnimName = "QBZ95v26_1_reload_empty",
            .SkinAdsReloadAnimName   = "QBZ95v26_1_reload_ads",
            .SkinAdsEmptyReloadAnimName = "QBZ95v26_1_reload_empty_ads",
            .SkinRaiseNewAnimName    = "QBZ95v26_1_raise_new",
            .SkinIdleActivePoseAnimName = "QBZ95v26_1_idle_active",
            .SkinIdlePoseYawAnimName    = "QBZ95v26_1_idle_pose_yaw",
            .SkinLobbyIdleActivePoseAnimName = "QBZ95v26_1_idle_active_lobby_low",
            .SkinReloadVirtualAnimName      = "QBZ95v26_1_reload_virtual",
            .SkinEmptyReloadVirtualAnimName = "QBZ95v26_1_reload_empty_virtual",
            .AdsUpAnimName   = "QBZ95v26_1_ads_up",
            .AdsDownAnimName = "QBZ95v26_1_ads_down",
            .ExtraModelKillTeamAnimName = "tps_mp_QBZ95v26_Killed_finisher",
            .TeamDieActionPose  = "tps_mp_QBZ95v26_Killed_victim",
            .SpecialDieActionPose= "tps_mp_AR01v11_Killed",
            .new_fire_sfx_id        = 13902813,
            .new_fire_ads_sfx_id    = 13902814,
            .fire_sfx               = {77700083,77700084,77700096},
            .fps_tracer_bullet_id   = 1390287,
            .fps_simple_tracer_bullet_id = 1390289,
            .fps_adstracer_bullet_id= 1390288,
            .fps_simple_adstracer_bullet_id = 13902810,
            .tps_simple_tracer_bullet_id    = 1390286,
            .optic_b_fx             = 13902816,
            .icon_outline_replace   = 51003902807ULL,
            .kill_tip_icon_replace  = 51003902808ULL,
            .kill_team_extra_model_id      = 93902818,
            .kill_team_extra_hexie_model_id= 93902825,
            .fps_trace_bullet_from_tip     = true,
            .CanInspectionPlayActive= true,
            .forbidden_decal        = 1,
            .recevier_show_ammo     = 1,
            .use_dynamic_box        = 1,
            .IsReducedRecoil        = "1",
            .skin_iron_sight_bone_pos     = {0.17f, 0.015f, 0.0f},
            .skin_high_bone_sight_offset  = {0.12f, -0.0465f, 0.0f},
            .skin_sprint_offset     = {0, 0.03f, 0},
            .gun_smith_zoom_scale   = 1.1f,
            .screenshot_scale       = 1.75f,
            .SkinInspectPlaybackSpeed = 1.0f,
            .team_die_action_disappear_time = 2.3f,
            .skin_upper_rail_model_id       = 93902815,
            .hide_upper_rail_model_part_list= {390301},
            .extra_ballistic_shoot_idx_list = {0,1,0,2,0,3},
            .extra_ballistic_tracer_bullet_id = 13902843,
            .Team_kill_mat  = "Effect/Effect_f/jinbi_die_mat_qbzv26_adv",
            .TeamDieActionPoseTime  = 2.3f,
            .dlc_attach_model_list  = {93902816},
            .gun_skin_hexie_part_model_list = {93902821,9390285,0,9390287,0,0},
        },
        // diff data entry
        {
            .id             = 1411990211ULL,
            .gun_id         = 14,
            .skin_name      = "KAG-6 - 球员联动投影 LV4",
            .gun_skin_type  = SkinType::PREMIUM,
            .dlc_id         = "dlcid_gunskintemplate_1411990211",
            .dlc_pack_ids   = {2001},
            .gun_skin_part_type_list  = {1,2,4,5,6,7,8},
            .gun_skin_part_model_list = {9140214,9140215,0,9140217,0,0,0},
            .gun_skin_attacment_id_list         = {140101,140201,140401,140501,140601,140701,140801},
            .gun_skin_present_attachment_id_list= {140101,140201,140401,140501,140601,140701,140801},
            .model_id1_replace_id     = 91402111,
            .optic_b_model_replace_id = 91402112,
            .skin_optic_id            = 2140211,
            .SkinInspectAnimName      = "AR01v37_1_inspect",
            .SkinEmptyReloadAnimName  = "AR01v37_1_reload_empty",
            .SkinAdsEmptyReloadAnimName = "AR01v37_1_reload_empty_ads",
            .SkinRaiseNewAnimName     = "AR01v37_1_raise_new",
            .SkinIdleActivePoseAnimName = "AR01v37_1_idle_active",
            .SkinIdlePoseYawAnimName    = "AR01v37_1_idle_pose_yaw",
            .SkinLobbyIdleActivePoseAnimName = "AR01v37_1_idle_active_lobby",
            .SkinEmptyReloadVirtualAnimName  = "AR01v37_1_reload_empty_virtual",
            .SpecialDieActionPose     = "tps_mp_AR01v11_Killed",
            .new_fire_sfx_id          = 11402113,
            .new_fire_ads_sfx_id      = 11402114,
            .fps_tracer_bullet_id     = 1140217,
            .fps_simple_tracer_bullet_id = 1140219,
            .fps_adstracer_bullet_id  = 1140218,
            .fps_simple_adstracer_bullet_id = 11402110,
            .tps_tracer_bullet_id     = 1140215,
            .tps_simple_tracer_bullet_id = 1140216,
            .optic_b_fx               = 11402116,
            .icon_outline_replace     = 51001402106ULL,
            .kill_tip_icon_replace    = 51001402107ULL,
            .fps_trace_bullet_from_tip= true,
            .forbidden_decal          = 1,
            .recevier_show_ammo       = 1,
            .skin_iron_sight_bone_pos       = {0.0202f, 0.019f, 0.0f},
            .skin_high_bone_sight_offset    = {0.0205f, 0.017f, 0.0f},
        },
    };

    // Lookup by skin ID
    inline const SkinTemplateEntry* FindById(uint64_t id) {
        for (const auto& e : BASE_TEMPLATES)
            if (e.id == id) return &e;
        return nullptr;
    }

    // Lookup by gun ID — return semua skin untuk gun tsb
    inline std::vector<const SkinTemplateEntry*> FindByGunId(int gun_id) {
        std::vector<const SkinTemplateEntry*> res;
        for (const auto& e : BASE_TEMPLATES)
            if (e.gun_id == gun_id) res.push_back(&e);
        return res;
    }

} // namespace GunSkinTemplate


// ─────────────────────────────────────────────────────────────────────────────
// SECTION 3 — CLASS INTERFACE STUBS
// Source: gclient.util.gun_skin_util class definitions
// Ini adalah interface reference — buat IDE autocomplete / hook reference
// Bukan implementasi nyata (implementasi ada di Python runtime)
// ─────────────────────────────────────────────────────────────────────────────
namespace GameClasses {

    // IHallItem — item data class (skin, senjata, dll di inventory)
    struct IHallItem {
        // Properties yang diexpose Python SDK
        bool        is_permanent         = false;
        bool        is_experience_item   = false;
        bool        is_bind_qualifying   = false;
        bool        expired              = false;
        bool        is_begin             = false;
        bool        can_disassemble      = false;
        bool        can_pileup           = false;
        bool        can_reuse            = false;

        int         equip_id             = 0;
        int         gun_id               = 0;
        int         skin_id              = 0;
        int         melee_id             = 0;
        int         hangings_id          = 0;
        int         guise_id             = 0;
        int         reticle_id           = 0;
        int         reticle_template_id  = 0;
        int         skin_optic_id        = 0;
        int         skin_optic_part_id   = 0;
        int         quality              = 0;
        int         sub_type             = 0;
        int         mall_id              = 0;

        int64_t     begin_time                   = 0;
        int64_t     get_time                     = 0;
        int64_t     bind_qualifying_expire_time  = 0;
    };

    // GunSkinDefaultItem — skin default (tanpa random)
    struct GunSkinDefaultItem {};

    // GunSkinRandomItem — skin random/gacha
    struct GunSkinRandomItem {};

    // GunAnimDefaultItem — default anim item
    struct GunAnimDefaultItem {};

    // GunDecorateDefaultItem — decal/decorate item
    struct GunDecorateDefaultItem {};

    // GunOpticReticleDefaultItem — reticle default
    struct GunOpticReticleDefaultItem {};

    // GunOpticReticleRandomItem : GunOpticReticleDefaultItem
    struct GunOpticReticleRandomItem : GunOpticReticleDefaultItem {};

    // BombSkinRandomItem
    struct BombSkinRandomItem {};

} // namespace GameClasses


// ─────────────────────────────────────────────────────────────────────────────
// SECTION 4 — GUNSMITH UI CONSTANTS
// Source: gclient.util.gun_ui_util + gunsmith_unlock_condition_list
// ─────────────────────────────────────────────────────────────────────────────
namespace GunSmithUI {

    // GunSmith level entity names (di-spawn di level Gunsmith)
    inline const std::vector<std::string> LEVEL_ENT_NAMES = {
        "Login_Gunshow_01_001",
        "fx_InterfaceSmoke_pp_003",
        "fx_worldroom_wall01",
        "fx_worldroom_wall01_1",
        "fx_worldroom_wall02_1",
        "US_gunshow_background_001",
    };

    // GunSkinMissionNode icon IDs
    // Source: gunsmith_unlock_condition_list.GunSkinMissionNode
    namespace MissionNode {
        constexpr int TASK_DEFAULT_ICON = 563;
        constexpr int TASK_FINISH_ICON  = 564;
        constexpr int TASK_LOCK_ICON    = 565;
        constexpr int TIMER_COUNTER     = 1;
    }

    // Konstanta lain dari gun_ui_util
    constexpr double PI = 3.141592653589793;

} // namespace GunSmithUI


// ─────────────────────────────────────────────────────────────────────────────
// SECTION 5 — SKIN TYPE ENUMS
// Derived dari sub_type / WarehouseItemType constants
// ─────────────────────────────────────────────────────────────────────────────
namespace SkinItemType {
    constexpr int GUN_SKIN    = 0;
    constexpr int GUN_GUISE   = 100;  // guise/illusion skin
    constexpr int MELEE_SKIN  = 1;
    constexpr int OPTIC_SKIN  = 2;
    constexpr int BOMB_SKIN   = 3;
    constexpr int DECORATE    = 4;
    constexpr int ANIM        = 5;
    constexpr int RETICLE     = 6;
} // namespace SkinItemType
