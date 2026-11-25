#include "../obfuscate.h"

const char* srcpy = O(R"srcpy(
import os, json, threading, functools, math, sys, inspect, time, socket, struct

PACKAGE_NAME = "com.netease.newspike"

# ============================================================
# 🔥 UDP CONFIGURATION
# ============================================================
UDP_SEND_PORT = 50000   # C++ -> Python (sync commands)
UDP_RECV_PORT = 50001   # Python -> C++ (ESP data)
UDP_SKIN_PORT = 50002   # Python -> C++ (skin info)
UDP_HERO_PORT = 50003   # Python -> C++ (hero info)

# UDP Sockets
_send_sock = None
_recv_sock = None
_skin_sock = None
_hero_sock = None
_udp_initialized = False

def _init_udp():
    global _send_sock, _recv_sock, _skin_sock, _hero_sock, _udp_initialized
    if _udp_initialized:
        return
    
    try:
        # Send socket (Python -> C++)
        _send_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        _send_sock.setsockopt(socket.SOL_SOCKET, socket.SO_SNDBUF, 2*1024*1024)
        _send_sock.setsockopt(socket.IPPROTO_IP, socket.IP_TOS, 0x10)
        _send_sock.settimeout(0.001)
        
        # Receive socket (C++ -> Python)
        _recv_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        _recv_sock.setsockopt(socket.SOL_SOCKET, socket.SO_RCVBUF, 2*1024*1024)
        _recv_sock.bind(("127.0.0.1", UDP_SEND_PORT))
        _recv_sock.settimeout(0.001)
        
        # Skin info socket
        _skin_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        _skin_sock.setsockopt(socket.SOL_SOCKET, socket.SO_SNDBUF, 2*1024*1024)
        _skin_sock.settimeout(0.001)
        
        # Hero info socket
        _hero_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        _hero_sock.setsockopt(socket.SOL_SOCKET, socket.SO_SNDBUF, 2*1024*1024)
        _hero_sock.settimeout(0.001)
        
        _udp_initialized = True
    except Exception as e:
        print(f"[UDP] Init error: {e}")

def _write_udp(data):
    """Kirim data via UDP ke C++"""
    global _send_sock
    if not _udp_initialized:
        _init_udp()
    try:
        if isinstance(data, str):
            data = data.encode('utf-8')
        _send_sock.sendto(data, ("127.0.0.1", UDP_RECV_PORT))
    except Exception as e:
        pass

def _write_udp_skin(data):
    """Kirim skin info via UDP"""
    global _skin_sock
    if not _udp_initialized:
        _init_udp()
    try:
        if isinstance(data, str):
            data = data.encode('utf-8')
        _skin_sock.sendto(data, ("127.0.0.1", UDP_SKIN_PORT))
    except Exception:
        pass

def _write_udp_hero(data):
    """Kirim hero info via UDP"""
    global _hero_sock
    if not _udp_initialized:
        _init_udp()
    try:
        if isinstance(data, str):
            data = data.encode('utf-8')
        _hero_sock.sendto(data, ("127.0.0.1", UDP_HERO_PORT))
    except Exception:
        pass

# ============================================================
# 🔥 UDP RECEIVER THREAD (dari C++)
# ============================================================
sync_attrs = {}

def _read_udp_thread():
    global _recv_sock, sync_attrs
    if not _udp_initialized:
        _init_udp()
    
    while True:
        try:
            data, addr = _recv_sock.recvfrom(65536)
            if data:
                try:
                    parsed = json.loads(data.decode('utf-8'))
                    if isinstance(parsed, dict):
                        sync_attrs.update(parsed)
                        _load_skinsel()
                except:
                    pass
        except socket.timeout:
            pass
        except Exception:
            pass

# ============================================================
# 🔥 START UDP RECEIVER
# ============================================================
_init_udp()
threading.Thread(target=_read_udp_thread, daemon=True).start()

# ============================================================
# 🔥 SKIN INFO BUILDER & SENDER (via UDP)
# ============================================================
from gclient.framework.entities.space import Space
from gclient.framework.util.replay_util import IsPlayerTeammate
from gclient.framework.camera.fps_placer import FpsPlacer
from gclient.gameplay.logic_base.entities.combat_avatar import CombatAvatar, PlayerCombatAvatar
from gclient.gameplay.logic_base.equips.equip_case import EquipCaseFactory
from gclient.gameplay.logic_base.equips.weapon_case import WeaponCase, WeaponCaseBase
from gshare.formula import Distance3D
from gclient.gamesystem.entities.hall_doll import Doll
from gclient.gamesystem.models.hall_show_model import HallShowModel

# FPS Unlock
try:
    import gclient.framework.util.performance_util as perf_util
    if hasattr(perf_util, 'EFpsLimit'):
        for _k in list(perf_util.EFpsLimit.keys()):
            perf_util.EFpsLimit[_k] = 999
    if hasattr(perf_util, 'EFpsLimitPC'):
        for _k in list(perf_util.EFpsLimitPC.keys()):
            perf_util.EFpsLimitPC[_k] = 999
except Exception as e:
    pass

def HOOK(clazz, bLog=False, _name=None):
    def decorator(func, _name=_name):
        name = func.__name__
        _name = _name or '_' + name
        setattr(clazz, _name, getattr(clazz, name, None))
        @functools.wraps(getattr(clazz, name, func))
        def wrapper(*args, **kwargs):
            try:
                return func(*args, **kwargs)
            except:
                pass
        setattr(clazz, name, wrapper)
        return func
    return decorator

def TRY(_func=None, *, bLog=False):
    def decorator(original):
        @functools.wraps(original)
        def wrapper(*args, **kwargs):
            try:
                return original(*args, **kwargs)
            except:
                pass
        return wrapper
    return decorator(_func) if _func else decorator

WEP = {}
WEP_NAMES = {}
WEP_SKIN_INFO = {}
SKIN_TO_EQUIP = {}

try:
    import gclient.data.effect_data as efd
except:
    efd = None

def _resolve_effect(effect_id):
    if not efd or not effect_id:
        return None
    try:
        info = efd.data.get(effect_id)
        if not info:
            return None
        return info.get('path') or info.get('effect_string')
    except:
        return None

def _skin_effect_id(skin_info):
    fx = skin_info.get('optic_b_fx')
    if fx:
        return fx
    return None

try:
    import gclient.data.equip_data as ed
    import gclient.data.gun_skin_template_base_data as gst
    import re

    def _base_skin_name_and_level(name):
        if not name:
            return name, 0
        m = re.search(r'\s*-?\s*LV(\d+)\s*$', name)
        if not m:
            return name, 0
        level = int(m.group(1))
        base = name[:m.start()].strip()
        return base, level

    def build_auto_wep_dictionary():
        global WEP, WEP_NAMES, WEP_SKIN_INFO, SKIN_TO_EQUIP
        equip_names = {}
        gun_to_equip_map = {}
        for equip_id, equip_info in ed.data.items():
            equip_names[equip_id] = equip_info.get('name') or str(equip_id)
            if gun_id := equip_info.get('gun_id'):
                gun_to_equip_map[gun_id] = equip_id
        melee_case_map = {'Melee': 33, 'DualMelee': 97, 'Heirloom': 141}
        melee_id_map = {2: 84, 3: 86, 4: 89, 5: 107}
        temp_skins = {}
        for skin_id, skin_info in gst.data.items():
            target_equip_id = None
            replace_id = skin_info.get('replace_equip_id')
            if replace_id:
                orig_equip = ed.data.get(replace_id, {})
                m_id = orig_equip.get('melee_weapon_id')
                e_case = orig_equip.get('equip_case')
                if m_id == 1:
                    target_equip_id = melee_case_map.get(e_case)
                else:
                    target_equip_id = melee_id_map.get(m_id)
            else:
                if gun_id := skin_info.get('gun_id'):
                    target_equip_id = gun_to_equip_map.get(gun_id)
            if not target_equip_id:
                continue
            if (skin_info.get('gun_skin_type') or 0) != 1:
                continue
            if target_equip_id not in temp_skins:
                temp_skins[target_equip_id] = {}
            skin_name = skin_info.get('skin_name')
            base_name, level = _base_skin_name_and_level(skin_name)
            dedup_key = base_name if base_name else skin_id
            existing = temp_skins[target_equip_id].get(dedup_key)
            if existing and existing[0] >= level:
                continue
            temp_skins[target_equip_id][dedup_key] = (level, skin_id)
            eff_id = _skin_effect_id(skin_info)
            WEP_SKIN_INFO[skin_id] = {
                'name': skin_name,
                'effect_id': eff_id,
                'effect_path': _resolve_effect(eff_id),
                'optic_b_fx': skin_info.get('optic_b_fx'),
                'optic_b_fx_path': _resolve_effect(skin_info.get('optic_b_fx')),
                'skin_optic_id': skin_info.get('skin_optic_id'),
                'kill_mat': skin_info.get('kill_mat'),
                'team_kill_mat': skin_info.get('Team_kill_mat'),
                'kill_mat_start_time': skin_info.get('kill_mat_start_time'),
                'team_kill_mat_start_time': skin_info.get('Team_kill_mat_start_time'),
                'kill_team_extra_model_id': skin_info.get('kill_team_extra_model_id'),
                'kill_team_extra_hexie_model_id': skin_info.get('kill_team_extra_hexie_model_id'),
                'kill_tip_icon_replace': skin_info.get('kill_tip_icon_replace'),
                'extra_model_kill_team_anim_name': skin_info.get('ExtraModelKillTeamAnimName'),
                'team_die_action_pose': skin_info.get('TeamDieActionPose'),
                'team_die_action_pose_time': skin_info.get('TeamDieActionPoseTime'),
                'special_die_action_pose': skin_info.get('SpecialDieActionPose'),
                'fps_tracer_bullet_id': skin_info.get('fps_tracer_bullet_id'),
                'fps_adstracer_bullet_id': skin_info.get('fps_adstracer_bullet_id'),
                'tps_tracer_bullet_id': skin_info.get('tps_tracer_bullet_id'),
                'fps_simple_tracer_bullet_id': skin_info.get('fps_simple_tracer_bullet_id'),
                'extra_ballistic_tracer_bullet_id': skin_info.get('extra_ballistic_tracer_bullet_id'),
            }
        HARDCORE_SKINS = {33: [21000009,91000001,91000002,91000003,11000018,21000005,11000007,31000010,21000008,31000007,11000002,811990203,141199023]}
        for equip_id, hardcore_list in HARDCORE_SKINS.items():
            if equip_id not in temp_skins:
                temp_skins[equip_id] = {}
            for skin_id in hardcore_list:
                dedup_key = f"hardcore_{skin_id}"
                temp_skins[equip_id][dedup_key] = (99, skin_id)
                WEP_SKIN_INFO[skin_id] = {
                    'name': f"Skin_{skin_id}",
                    'effect_id': None,
                    'effect_path': None,
                    'optic_b_fx': None,
                    'optic_b_fx_path': None,
                    'skin_optic_id': None,
                    'kill_mat': None,
                    'team_kill_mat': None,
                    'kill_mat_start_time': None,
                    'team_kill_mat_start_time': None,
                    'kill_team_extra_model_id': None,
                    'kill_team_extra_hexie_model_id': None,
                    'kill_tip_icon_replace': None,
                    'extra_model_kill_team_anim_name': None,
                    'team_die_action_pose': None,
                    'team_die_action_pose_time': None,
                    'special_die_action_pose': None,
                    'fps_tracer_bullet_id': None,
                    'fps_adstracer_bullet_id': None,
                    'tps_tracer_bullet_id': None,
                    'fps_simple_tracer_bullet_id': None,
                    'extra_ballistic_tracer_bullet_id': None,
                }
        for equip_id, entries in temp_skins.items():
            skin_ids = [sid for _, sid in entries.values()]
            if skin_ids:
                name = equip_names.get(equip_id, str(equip_id))
                WEP[equip_id] = [name] + skin_ids
                WEP_NAMES[equip_id] = name
                for sid in skin_ids:
                    SKIN_TO_EQUIP[sid] = equip_id

    build_auto_wep_dictionary()

    def _build_skin_info_lines():
        lines = []
        for equip_id, entries in WEP.items():
            weapon_name = entries[0]
            lines.append("W|%d|%s" % (equip_id, weapon_name))
            for idx, skin_id in enumerate(entries[1:], start=1):
                skin_name = (WEP_SKIN_INFO.get(skin_id) or {}).get('name') or str(skin_id)
                lines.append("S|%d|%d|%s" % (equip_id, idx, skin_name))
        lines.append("END")
        return ("\n".join(lines) + "\n")

    def _send_skin_info_loop():
        payload = _build_skin_info_lines()
        for _ in range(15):
            try:
                _write_udp_skin(payload)
                break
            except:
                time.sleep(2)

    threading.Thread(target=_send_skin_info_loop, daemon=True).start()

    try:
        from gclient.gameplay.uicomponents.hud_killrecord_comp import HudKillRecordComp, HudKillRecordCompPC, HudKillRecordCompV2

        def _weapon_name_and_wids(weapon_id):
            try:
                weapon_id = int(weapon_id or 0)
                entry = WEP.get(weapon_id)
                if not entry:
                    return None, []
                return entry[0], entry[1:]
            except Exception:
                return None, []

        def _get_selected_skin_for_weapon(weapon_id):
            try:
                name, wids = _weapon_name_and_wids(weapon_id)
                if not name or not wids:
                    return 0
                idx = int(sync_attrs.get(f'iSkin_{int(weapon_id)}', 0) or 0)
                if 1 <= idx <= len(wids):
                    return wids[idx - 1]
            except Exception:
                pass
            return 0

        def _get_case_skin(wc):
            if not wc:
                return 0
            try:
                wid = getattr(wc, 'weapon_id', 0) or getattr(wc, 'equip_id', 0)
                if wid:
                    sid = _get_selected_skin_for_weapon(wid)
                    if sid:
                        return sid
            except Exception:
                pass
            try:
                sid = getattr(wc, '_selected_skin_id', 0)
                if sid:
                    return sid
            except Exception:
                pass
            try:
                sid = getattr(wc, 'guise_id', 0) or getattr(wc, 'guise_item_id', 0) or getattr(wc, 'guise_template_id', 0)
                if sid:
                    return sid
            except Exception:
                pass
            try:
                d = getattr(wc, '__dict__', {})
                sid = d.get('_selected_skin_id', 0) or d.get('guise_id', 0) or d.get('guise_item_id', 0) or d.get('guise_template_id', 0)
                if sid:
                    return sid
            except Exception:
                pass
            return 0

        def _get_weapon_case_from_info(ca, info):
            if not ca or not isinstance(info, dict):
                return None
            try:
                wguid = info.get('weapon_guid')
                if wguid and hasattr(ca, 'GetWeaponCase'):
                    wc = ca.GetWeaponCase(wguid)
                    if wc:
                        return wc
            except Exception:
                pass
            return None

        def _get_kill_skin(info=None):
            try:
                sp = getattr(Space, '_instance', None)
                owner = getattr(sp, 'owner', None) if sp else None
                ca = getattr(owner, 'combat_avatar', None) if owner else None
                if not ca:
                    return 0, None
                wc = _get_weapon_case_from_info(ca, info)
                if wc:
                    sid = _get_case_skin(wc)
                    if sid:
                        return sid, wc
                try:
                    weapon = ca.GetCurWeapon()
                    if weapon and hasattr(ca, 'GetWeaponCase'):
                        guid = getattr(weapon, 'guid', None)
                        if guid:
                            wc = ca.GetWeaponCase(guid)
                            if wc:
                                sid = _get_case_skin(wc)
                                if sid:
                                    return sid, wc
                except Exception:
                    pass
                try:
                    weapon = ca.GetCurWeapon()
                    if weapon:
                        wid = getattr(weapon, 'weapon_id', 0) or getattr(weapon, 'equip_id', 0)
                        sid = _get_selected_skin_for_weapon(wid)
                        if sid:
                            return sid, None
                except Exception:
                    pass
            except Exception:
                pass
            return 0, None

        def _get_skin_data(skin_id):
            template = None
            feature = None
            try:
                from gclient.data import gun_skin_template_data
                data = getattr(gun_skin_template_data, 'data', None)
                if isinstance(data, dict):
                    value = data.get(skin_id)
                    if isinstance(value, dict):
                        template = value
            except Exception:
                pass
            try:
                from gclient.data import item_feature_data
                data = getattr(item_feature_data, 'data', None)
                if isinstance(data, dict):
                    value = data.get(skin_id)
                    if isinstance(value, dict):
                        feature = value
            except Exception:
                pass
            return template, feature

        def _copy_skin_kill_broadcast_data(info, skin_id):
            if not isinstance(info, dict) or not skin_id:
                return
            template, feature = _get_skin_data(skin_id)
            info['guise_id'] = skin_id
            info['guise_item_id'] = skin_id
            info['guise_template_id'] = skin_id
            info['hud_kill_ui'] = skin_id
            prefixes = ('kill', 'broadcast', 'racer')
            exact_keys = {
                'kill_tip_icon', 'kill_tip_icon_replace', 'kill_icon', 'kill_icon_replace',
                'kill_fx', 'kill_fx_id', 'kill_effect', 'kill_effect_id', 'kill_effect_type',
                'kill_avatar_effect', 'kill_avatar_effect_id', 'kill_avatar_effect_id_replace',
                'kill_racer_effect', 'kill_racer_effect_id', 'kill_racer_effect_id_replace',
                'kill_broadcast', 'kill_broadcast_id', 'kill_broadcast_id_replace',
                'broadcast_effect', 'broadcast_effect_id', 'broadcast_effect_id_replace',
                'racer_effect', 'racer_effect_id',
            }
            for source in (template, feature):
                if not isinstance(source, dict):
                    continue
                for key, value in source.items():
                    if value is None:
                        continue
                    key_text = str(key)
                    lower = key_text.lower()
                    if key_text in exact_keys or lower.startswith(prefixes):
                        info[key] = value
            if isinstance(template, dict):
                icon = template.get('kill_tip_icon_replace') or template.get('kill_tip_icon') or template.get('kill_icon_replace') or template.get('kill_icon')
                if icon:
                    info['weapon_icon'] = icon
                    info['kill_tip_icon'] = icon

        _force_kill_hud_skin = [None]

        def _install_kill_feed(cls):
            try:
                if cls is None:
                    return
                backup = '_actual_skin_original_push_kill_record'
                if hasattr(cls, backup):
                    return
                original_push = getattr(cls, 'PushKillRecord', None)
                if not callable(original_push):
                    return
                setattr(cls, backup, original_push)

                def PushKillRecordWithActualSkin(self, info, *args, **kwargs):
                    is_my_kill = False
                    try:
                        if isinstance(info, dict):
                            sp = getattr(Space, '_instance', None)
                            owner = getattr(sp, 'owner', None) if sp else None
                            ca = getattr(owner, 'combat_avatar', None) if owner else None
                            if ca:
                                my_id = getattr(ca, 'id', None)
                                caster_id = info.get('caster_id')
                                if my_id is not None and caster_id == my_id:
                                    skin_id, _wc = _get_kill_skin(info)
                                    if skin_id:
                                        _copy_skin_kill_broadcast_data(info, skin_id)
                                        _force_kill_hud_skin[0] = skin_id
                                        is_my_kill = True
                    except Exception:
                        pass
                    try:
                        return original_push(self, info, *args, **kwargs)
                    finally:
                        if is_my_kill:
                            _force_kill_hud_skin[0] = None

                setattr(cls, 'PushKillRecord', PushKillRecordWithActualSkin)
            except Exception:
                pass

        _install_kill_feed(HudKillRecordComp)
        _install_kill_feed(HudKillRecordCompPC)
        _install_kill_feed(HudKillRecordCompV2)

        try:
            import gclient.gameplay.uicomponents.hud_killrecord_comp as hkrc_mod
            if not hasattr(hkrc_mod, '_orig_GetKillHudConfigForGameplay'):
                hkrc_mod._orig_GetKillHudConfigForGameplay = hkrc_mod.GetKillHudConfigForGameplay

                def _hooked_GetKillHudConfigForGameplay(guise_item_id, *args, **kwargs):
                    use_id = guise_item_id
                    forced = _force_kill_hud_skin[0]
                    if forced:
                        use_id = forced
                    return hkrc_mod._orig_GetKillHudConfigForGameplay(use_id, *args, **kwargs)

                hkrc_mod.GetKillHudConfigForGameplay = _hooked_GetKillHudConfigForGameplay
        except Exception as e:
            pass

    except Exception as e:
        pass

    try:
        from gclient.gameplay.logic_base.models.avatar_model import AvatarModel
        _optional_skin_selection = {}

        def _get_active_guise_id(info):
            try:
                sp = getattr(Space, '_instance', None)
                if not sp:
                    return None
                lo = getattr(sp, 'owner', None)
                ca = getattr(lo, 'combat_avatar', None) if lo else None
                if not ca:
                    return None
                try:
                    cur_weapon = ca.GetCurWeapon() if hasattr(ca, 'GetCurWeapon') else None
                    if cur_weapon:
                        wid = getattr(cur_weapon, 'weapon_id', None) or getattr(cur_weapon, 'equip_id', None)
                        if wid:
                            skin = _optional_skin_selection.get(wid, 0)
                            if skin:
                                return skin
                            try:
                                idx = int(sync_attrs.get(f'iSkin_{wid}', 0) or 0)
                                wids = WEP.get(wid, [wid])[1:]
                                if wids and 0 < idx <= len(wids):
                                    return wids[idx - 1]
                            except Exception:
                                pass
                except Exception:
                    pass
                try:
                    wguid = info.get('weapon_guid') if isinstance(info, dict) else None
                    if wguid and hasattr(ca, 'GetWeaponCase'):
                        wc = ca.GetWeaponCase(wguid)
                        if wc:
                            guise_id = getattr(wc, 'guise_id', None) or getattr(wc, 'guise_item_id', None) or wc.__dict__.get('guise_id') or wc.__dict__.get('guise_item_id')
                            if guise_id:
                                return guise_id
                            wid = getattr(wc, 'weapon_id', 0)
                            if wid:
                                skin = _optional_skin_selection.get(wid, 0)
                                if skin:
                                    return skin
                except Exception:
                    pass
                try:
                    if hasattr(ca, 'weapon_cases') and ca.weapon_cases:
                        for wc in ca.weapon_cases.values():
                            guise_id = getattr(wc, 'guise_id', None) or getattr(wc, 'guise_item_id', None) or wc.__dict__.get('guise_id') or wc.__dict__.get('guise_item_id')
                            if guise_id:
                                return guise_id
                            wid = getattr(wc, 'weapon_id', 0)
                            if wid:
                                skin = _optional_skin_selection.get(wid, 0)
                                if skin:
                                    return skin
                except Exception:
                    pass
            except Exception:
                pass
            return None

        def _ensure_kill_effect_data(guise_id):
            if not guise_id:
                return
            try:
                from gclient.data import item_feature_data
                if hasattr(item_feature_data, 'data'):
                    if guise_id not in item_feature_data.data:
                        item_feature_data.data[guise_id] = {}
                    item_feature_data.data[guise_id]['kill_effect'] = True
            except Exception:
                pass
            try:
                from gclient.data import gun_skin_template_data
                if hasattr(gun_skin_template_data, 'data'):
                    if guise_id not in gun_skin_template_data.data:
                        gun_skin_template_data.data[guise_id] = {
                            'dlc_id': None,
                            'kill_fx': True,
                            'die_action_attach_model_id': None,
                            'die_action_attach_model_time': 0.0,
                            'die_action_disappear_time': 0.0,
                            'SpecialDieActionPose': None,
                        }
                    else:
                        entry = gun_skin_template_data.data[guise_id]
                        if isinstance(entry, dict):
                            if 'kill_fx' not in entry:
                                entry['kill_fx'] = True
            except Exception:
                pass

        @HOOK(AvatarModel, bLog=False)
        def PlaySkinDeadAnimAndEffect(self, info, *args, **kwargs):
            try:
                if sync_attrs.get('bSkinHack', False):
                    guise_id = _get_active_guise_id(info)
                    if guise_id:
                        _ensure_kill_effect_data(guise_id)
                        try:
                            owner = getattr(self, 'owner', None)
                            if owner and hasattr(owner, 'final_kill_info'):
                                import _pickle as cPickle
                                raw_fki = owner.final_kill_info
                                if raw_fki:
                                    fki = cPickle.loads(raw_fki)
                                    if isinstance(fki, dict):
                                        fki['guise_item_id'] = guise_id
                                        fki['guise_template_id'] = guise_id
                                        fki.pop('kill_team_guise_item_id', None)
                                        fki['kill_effect'] = True
                                        owner.final_kill_info = cPickle.dumps(fki)
                        except Exception:
                            pass
                        if isinstance(info, dict):
                            info['guise_item_id'] = guise_id
                            info['guise_template_id'] = guise_id
                            info['kill_effect'] = True
            except Exception:
                pass
            return self._PlaySkinDeadAnimAndEffect(info, *args, **kwargs)

        @HOOK(AvatarModel, bLog=False)
        def PlaySkinDeadEffect(self, sfx_info, material, time_offset, info, *args, **kwargs):
            try:
                if sync_attrs.get('bSkinHack', False):
                    guise_id = _get_active_guise_id(info)
                    if guise_id and isinstance(info, dict):
                        _ensure_kill_effect_data(guise_id)
                        info['guise_item_id'] = guise_id
                        info['guise_template_id'] = guise_id
                        info['kill_effect'] = True
            except Exception:
                pass
            return self._PlaySkinDeadEffect(sfx_info, material, time_offset, info, *args, **kwargs)

    except Exception:
        pass

except Exception:
    WEP = {1: ['M4A1', 11100002]}
    WEP_NAMES = {1: 'M4A1'}

_skinsel = {}

def _load_skinsel():
    global _skinsel
    _skinsel = {}
    for key, value in sync_attrs.items():
        if key.startswith('iSkin_'):
            equip_id = key.replace('iSkin_', '')
            if value > 0:
                _skinsel[str(equip_id)] = value

def _pick_wep_skin(equip_id):
    try:
        _load_skinsel()
        sel = _skinsel.get(str(int(equip_id)))
        if sel:
            return sel
        return None
    except:
        return None

def _get_forced_skin(equip_id):
    if equip_id not in WEP:
        return None
    wids = WEP[equip_id][1:]
    idx = sync_attrs.get(f'iSkin_{equip_id}', 0)
    return wids[idx - 1] if wids and 0 < idx <= len(wids) else None

@HOOK(EquipCaseFactory, 0)
def Create(wid, *args, **kwargs):
    args_list = list(args)
    forced_skin = None
    if sync_attrs.get('bSkinHack') and wid in WEP:
        skin_index = sync_attrs.get(f'iSkin_{wid}', 0)
        wids = WEP[wid][1:]
        if wids and 0 < skin_index <= len(wids):
            forced_skin = wids[skin_index - 1]
            if len(args_list) > 4:
                args_list[4] = forced_skin
    result = EquipCaseFactory._Create(wid, *args_list, **kwargs)
    if result and sync_attrs.get('bSkinHack'):
        try:
            if hasattr(result, 'upgrade_level'):
                result.upgrade_level = 10
        except:
            pass
        if forced_skin is not None:
            try:
                if hasattr(result, 'ClearGuiseItemCache'):
                    result.ClearGuiseItemCache()
            except:
                pass
            try:
                if hasattr(result, 'ChangeWeaponGuise'):
                    result.ChangeWeaponGuise(forced_skin, guise_item_id=forced_skin)
            except:
                pass
            skin_data = WEP_SKIN_INFO.get(forced_skin, {})
            try:
                if hasattr(result, 'ChangeWeaponOpticReticle') and skin_data.get('skin_optic_id'):
                    result.ChangeWeaponOpticReticle(skin_data['skin_optic_id'])
            except Exception:
                pass
            for attr_name, attr_val in (
                ('show_guise_bullet_trace', True),
                ('show_guise_hit_effect', True),
                ('guise_item_id', forced_skin),
                ('guise_id', forced_skin),
                ('skin_id', forced_skin),
            ):
                if attr_val is None:
                    continue
                try:
                    setattr(result, attr_name, attr_val)
                except Exception:
                    pass
            for refresh_method in (
                'RefreshGuiseEffect',
                'RefreshGuise3PEffectInCombat',
                'RefreshRacerForChangeGuise',
                'RefreshFireSfxId',
                'RefreshFireSfxIdForGunPart',
                'RefreshKillCounterForChangeGuise',
            ):
                try:
                    if hasattr(result, refresh_method):
                        getattr(result, refresh_method)()
                except Exception:
                    pass
    return result

@HOOK(Doll, 0)
def AddWeaponForDoll(self, equip_id=None, skin_id=None, guise_id=None, part_slots=None, extra=None, *args, **kwargs):
    if sync_attrs.get('bSkinHack') and equip_id is not None and equip_id in WEP:
        try:
            idx = sync_attrs.get(f'iSkin_{equip_id}', 0)
            wids = WEP[equip_id][1:]
            if wids and 0 < idx <= len(wids):
                new_skin = wids[idx - 1]
                if guise_id is not None:
                    guise_id = new_skin
                elif skin_id is not None:
                    skin_id = new_skin
        except:
            pass
    return Doll._AddWeaponForDoll(self, equip_id, skin_id, guise_id, part_slots, extra, *args, **kwargs)

@HOOK(Doll, 0)
def AddWeaponByEquipId(self, equip_id=None, skin_id=None, guise_id=None, part_slots=None, principle_role=False, extra=None, *args, **kwargs):
    if sync_attrs.get('bSkinHack') and equip_id is not None and equip_id in WEP:
        try:
            idx = sync_attrs.get(f'iSkin_{equip_id}', 0)
            wids = WEP[equip_id][1:]
            if wids and 0 < idx <= len(wids):
                new_skin = wids[idx - 1]
                if guise_id is not None:
                    guise_id = new_skin
                elif skin_id is not None:
                    skin_id = new_skin
        except:
            pass
    return Doll._AddWeaponByEquipId(self, equip_id, skin_id, guise_id, part_slots, principle_role, extra, *args, **kwargs)

@HOOK(Doll, 0)
def _DoAddWeapon(self, equip_id, skin_id, guise_id, part_slots, extra=None, is_reload=False, *args, **kwargs):
    if sync_attrs.get('bSkinHack') and equip_id is not None and equip_id in WEP:
        try:
            idx = sync_attrs.get(f'iSkin_{equip_id}', 0)
            wids = WEP[equip_id][1:]
            if wids and 0 < idx <= len(wids):
                new_skin = wids[idx - 1]
                if guise_id is not None:
                    guise_id = new_skin
                elif skin_id is not None:
                    skin_id = new_skin
        except:
            pass
    return Doll.__DoAddWeapon(self, equip_id, skin_id, guise_id, part_slots, extra, is_reload, *args, **kwargs)

@HOOK(Doll, 0)
def ChangeWeaponSkinId(self, skin_id, *args, **kwargs):
    if sync_attrs.get('bSkinHack'):
        try:
            equip_id = getattr(self, 'equip_id', 0)
            if equip_id in WEP:
                idx = sync_attrs.get(f'iSkin_{equip_id}', 0)
                wids = WEP[equip_id][1:]
                if wids and 0 < idx <= len(wids):
                    skin_id = wids[idx - 1]
        except:
            pass
    return Doll._ChangeWeaponSkinId(self, skin_id, *args, **kwargs)

# ============================================================
# HERO SKIN HACK
# ============================================================

HERO = {}
HERO_SKIN_INFO = {}

try:
    import gclient.data.hero_data as hd
    import gclient.data.lobby_item_data as lid
    from gclient.framework.entities.avatar import PlayerAvatar
    from gclient.gamesystem.entities.hall_doll import Doll, DollLike, CalculationDoll, BigHeadCalculationDoll
    from gclient.gamesystem.models.hall_show_model import HallShowModel
    from gclient.gameplay.logic_base.entities.combat_avatar import CombatAvatar
    import time, threading

    for hero_id, hero_proto in hd.data.items():
        HERO[hero_id] = [hero_proto.get('hero_name', str(hero_id))]

    for item_id, item_proto in lid.data.items():
        is_hero_skin = (
            item_proto.get('sub_type') == 300 or
            item_proto.get('short_desc') == 'Striker' or
            (item_proto.get('hero_id') and item_proto.get('hero_skin_id') is not None)
        )
        if not is_hero_skin:
            continue
        hero_id = item_proto.get('hero_id')
        if not hero_id or hero_id not in HERO:
            continue
        if item_id == hd.data.get(hero_id, {}).get('hero_item_id'):
            continue
        HERO_SKIN_INFO[item_id] = item_proto
        if item_id not in HERO[hero_id]:
            HERO[hero_id].append(item_id)

    def _build_hero_info_lines():
        lines = []
        for hero_id, entries in HERO.items():
            lines.append(f"H|{hero_id}|{entries[0]}")
            for idx, skin_id in enumerate(entries[1:], start=1):
                skin_proto = HERO_SKIN_INFO.get(skin_id, {})
                skin_name = skin_proto.get('name', f"Skin_{skin_id}") if isinstance(skin_proto, dict) else str(skin_proto)
                lines.append(f"K|{hero_id}|{idx}|{skin_name}")
        lines.append("END")
        return "\n".join(lines)

    def _send_hero_info():
        try:
            payload = _build_hero_info_lines() + "\n"
            _write_udp_hero(payload)
            for _ in range(3):
                time.sleep(0.1)
                _write_udp_hero("\n")
        except:
            pass

    _send_hero_info()

    try:
        with open("/storage/emulated/0/Nexora/nexora_herobuild_debug.txt", "a") as f:
            f.write(
                "HERO dict count=%d sample=%r\nHERO_SKIN_INFO count=%d sample=%r\nhd.data count=%d\nlid.data count=%d\n"
                % (
                    len(HERO), dict(list(HERO.items())[:3]),
                    len(HERO_SKIN_INFO), dict(list(HERO_SKIN_INFO.items())[:2]),
                    len(hd.data), len(lid.data),
                )
            )
    except Exception as _hbe:
        try:
            with open("/storage/emulated/0/Nexora/nexora_herobuild_debug.txt", "a") as f:
                f.write("BUILD LOG ERROR: %r\n" % (_hbe,))
        except Exception:
            pass

    def _hero_sender_loop():
        while True:
            time.sleep(2)
            _send_hero_info()

    threading.Thread(target=_hero_sender_loop, daemon=True).start()

    def _get_forced_skin(hero_id):
        if hero_id not in HERO:
            return None
        sids = HERO[hero_id][1:]
        idx = sync_attrs.get(f'iHeroSkin_{hero_id}', 0)
        if sids and 0 < idx <= len(sids):
            return sids[idx - 1]
        return None

    def _get_forced_skin_proto(hero_id):
        skin_id = _get_forced_skin(hero_id)
        if skin_id and skin_id in lid.data:
            return lid.data[skin_id]
        return None

    def _get_forced_model_id(hero_id):
        proto = _get_forced_skin_proto(hero_id)
        if proto:
            return proto.get('model_id')
        return None

    def _hook_hero_skin_proto(cls):
        orig_prop = cls.__dict__.get('hero_skin_proto')
        if orig_prop is None or not isinstance(orig_prop, property):
            return
        orig_get = orig_prop.fget
        
        def _getter(self):
            result = orig_get(self)
            if sync_attrs.get('bHeroSkinHack', False):
                hero_id = None
                if hasattr(self, 'GetHeroId'):
                    hero_id = self.GetHeroId()
                elif hasattr(self, 'hero_id'):
                    hero_id = self.hero_id
                if hero_id:
                    forced = _get_forced_skin_proto(hero_id)
                    if forced:
                        return forced
            return result
        
        setattr(cls, 'hero_skin_proto', property(_getter))

    for cls in (PlayerAvatar, CombatAvatar, Doll, DollLike, CalculationDoll, BigHeadCalculationDoll):
        try:
            _hook_hero_skin_proto(cls)
        except:
            pass

    @HOOK(PlayerAvatar, 0)
    def GetHeroSkinProto(self, hero_id, skin_item_id, *args, **kwargs):
        if sync_attrs.get('bHeroSkinHack', False):
            if hero_id is None or hero_id == 0:
                owner = getattr(self, 'owner', None)
                if owner and hasattr(owner, 'GetHeroId'):
                    hero_id = owner.GetHeroId()
            if hero_id:
                forced = _get_forced_skin(hero_id)
                if forced and forced in lid.data:
                    skin_item_id = forced
                    return lid.data[forced]
        return PlayerAvatar._GetHeroSkinProto(self, hero_id, skin_item_id, *args, **kwargs)

    @HOOK(HallShowModel, 0)
    def Load(self, data, done=None, **create_kwargs):
        try:
            if sync_attrs.get('bHeroSkinHack', False):
                hero_id = None
                if hasattr(self, 'owner') and self.owner:
                    if hasattr(self.owner, 'GetHeroId'):
                        hero_id = self.owner.GetHeroId()
                if hero_id:
                    model_id = _get_forced_model_id(hero_id)
                    if model_id and isinstance(data, dict):
                        data['model_id'] = model_id
                        data['id'] = model_id
        except:
            pass
        return HallShowModel._Load(self, data, done, **create_kwargs)

    @HOOK(HallShowModel, 0)
    def Reload(self, data, done=None, use_ready_to_appear=True):
        try:
            if sync_attrs.get('bHeroSkinHack', False):
                hero_id = None
                if hasattr(self, 'owner') and self.owner:
                    if hasattr(self.owner, 'GetHeroId'):
                        hero_id = self.owner.GetHeroId()
                if hero_id:
                    model_id = _get_forced_model_id(hero_id)
                    if model_id and isinstance(data, dict):
                        data['model_id'] = model_id
                        data['id'] = model_id
        except:
            pass
        return HallShowModel._Reload(self, data, done, use_ready_to_appear)

    @HOOK(Doll, 0)
    def GetModelUnitID(self, _download_info=None):
        try:
            if sync_attrs.get('bHeroSkinHack', False):
                hero_id = None
                if hasattr(self, 'GetHeroId'):
                    hero_id = self.GetHeroId()
                if hero_id:
                    model_id = _get_forced_model_id(hero_id)
                    if model_id:
                        return model_id
        except:
            pass
        return Doll._GetModelUnitID(self, _download_info)

    @HOOK(Doll, 0)
    def GetModelComponentList(self, _download_info=None):
        try:
            if sync_attrs.get('bHeroSkinHack', False):
                hero_id = None
                if hasattr(self, 'GetHeroId'):
                    hero_id = self.GetHeroId()
                if hero_id:
                    model_id = _get_forced_model_id(hero_id)
                    if model_id:
                        if _download_info is None:
                            _download_info = {}
                        _download_info['model_id'] = model_id
                        _download_info['forced_model_id'] = model_id
        except:
            pass
        return Doll._GetModelComponentList(self, _download_info)

    for cls in (Doll, DollLike, CalculationDoll, BigHeadCalculationDoll):
        @HOOK(cls, 0)
        def ShowHeroSkinModel(self, item_proto, *args, **kwargs):
            if sync_attrs.get('bHeroSkinHack', False) and item_proto is not None:
                hero_id = item_proto.get('hero_id') if hasattr(item_proto, 'get') else None
                if hero_id is None and hasattr(self, 'GetHeroId'):
                    hero_id = self.GetHeroId()
                if hero_id:
                    skin_proto = _get_forced_skin_proto(hero_id)
                    if skin_proto:
                        item_proto = skin_proto
            return cls._ShowHeroSkinModel(self, item_proto, *args, **kwargs)

    @HOOK(Doll, 0)
    def ShowDisplayModel(self, item_proto, display_loop=False, callback=None, *args, **kwargs):
        if sync_attrs.get('bHeroSkinHack', False) and item_proto is not None:
            hero_id = item_proto.get('hero_id') if hasattr(item_proto, 'get') else None
            if hero_id is None and hasattr(self, 'GetHeroId'):
                hero_id = self.GetHeroId()
            if hero_id:
                skin_proto = _get_forced_skin_proto(hero_id)
                if skin_proto:
                    item_proto = skin_proto
        return Doll._ShowDisplayModel(self, item_proto, display_loop, callback, *args, **kwargs)

    @HOOK(HallShowModel, 0)
    def ClearHeroSkinInfo(self, smooth=False, *args, **kwargs):
        result = HallShowModel._ClearHeroSkinInfo(self, smooth, *args, **kwargs)
        if sync_attrs.get('bHeroSkinHack', False):
            try:
                owner = getattr(self, 'owner', None)
                if owner and hasattr(owner, 'GetHeroId'):
                    hero_id = owner.GetHeroId()
                    if hero_id:
                        skin_proto = _get_forced_skin_proto(hero_id)
                        if skin_proto and hasattr(self, 'ShowHeroSkinModel'):
                            self.ShowHeroSkinModel(skin_proto, is_idle=True)
            except:
                pass
        return result

    @HOOK(HallShowModel, 0)
    def CueChangeHeroSkin(self, *args, **kwargs):
        result = HallShowModel._CueChangeHeroSkin(self, *args, **kwargs)
        if sync_attrs.get('bHeroSkinHack', False):
            try:
                owner = getattr(self, 'owner', None)
                if owner and hasattr(owner, 'GetHeroId'):
                    hero_id = owner.GetHeroId()
                    if hero_id:
                        skin_proto = _get_forced_skin_proto(hero_id)
                        if skin_proto and hasattr(self, 'ShowHeroSkinModel'):
                            self.ShowHeroSkinModel(skin_proto, is_idle=True)
            except:
                pass
        return result

except Exception as _hero_section_error:
    HERO = {1: ['Default']}
    try:
        with open("/storage/emulated/0/Nexora/nexora_herobuild_debug.txt", "a") as f:
            f.write("WHOLE SECTION CRASHED: %r\n" % (_hero_section_error,))
    except Exception:
        pass

_XRAY_COLORS = ((1,0,0), (0,1,0), (1,1,1), (0,0.5,1), (1,0,1))

_ESP_BONES = [
    ('head',      'biped Head'),
    ('neck',      'biped Neck'),
    ('spine',     'biped Spine'),
    ('spine1',    'biped Spine1'),
    ('pelvis',    'biped Pelvis'),
    ('lshoulder', 'biped L UpperArm'),
    ('rshoulder', 'biped R UpperArm'),
    ('lforearm',  'biped L Forearm'),
    ('rforearm',  'biped R Forearm'),
    ('lhand',     'biped L Hand'),
    ('rhand',     'biped R Hand'),
    ('lthigh',    'biped L Thigh'),
    ('rthigh',    'biped R Thigh'),
    ('lknee',     'biped L Calf'),
    ('rknee',     'biped R Calf'),
    ('lfoot',     'biped L Foot'),
    ('rfoot',     'biped R Foot'),
]

def _ab_safe(inst):
    return True

def WorldToScreenPoint(worldpos):
    return Space._instance.camera.engine_camera.GetScreenPointFromWorldPoint(worldpos)

def IsVisible(local, target, targetId, distance=100):
    raycast = Space._instance.RawRaycast(local, distance, 19, with_trigger=False, to_pos=target)
    ownerid = getattr(raycast.Body, 'ownerid', None)
    if raycast.Body and ownerid:
        return ownerid == targetId
    return raycast.Flags == 7

def Distance(a, b):
    dx = b[0] - a[0]; dy = b[1] - a[1]; dz = b[2] - a[2]
    return (dx*dx + dy*dy + dz*dz) ** 0.5

def _get_item_id(entity):
    for attr in ('item_id', 'gid', 'id', 'proto_id'):
        try:
            v = getattr(entity, attr, None)
            if v:
                return int(v)
        except Exception:
            continue
    return 0

@TRY
def EspUpdate(*_, **__):
    if not sync_attrs.get('bESP', True):
        return
    inst = genv.space
    if not isinstance(inst, Space) or not inst.entities or not genv.avatar:
        return
    if not _ab_safe(inst):
        return
    localEntity = genv.avatar.combat_avatar
    if not localEntity or not hasattr(localEntity, 'model'):
        return
    _cam            = Space._instance.camera
    _cp             = _cam.position
    cam_pos         = (_cp.x, _cp.y, _cp.z)
    _cam_pos        = _cp
    _eng_cam        = _cam.engine_camera
    _bXray          = sync_attrs.get('bXray', False)
    _bVisCheck      = sync_attrs.get('bVisCheck', False)
    _bFilterZombie  = sync_attrs.get('bFilterZombie', False)
    _bZombieHL      = sync_attrs.get('bZombieHighlight', False)
    _iXrayColor     = sync_attrs.get('iXrayColor', 0)
    xray_c          = _XRAY_COLORS[_iXrayColor if _iXrayColor < len(_XRAY_COLORS) else 0]

    def get_bone_screen(model, bone_name):
        try:
            wp = model.GetBoneWorldPosition(bone_name)
            if not wp:
                return None
            sp = _eng_cam.GetScreenPointFromWorldPoint(wp)
            return [sp.x, sp.y] if sp.z > 0.0 else None
        except:
            return None

    _MAX_DIST = float(sync_attrs.get('fESP_MaxDist', 500.0))
    entities = []
    _bLootESP = sync_attrs.get('bLootESP', False)
    _LOOT_MAX_DIST = float(sync_attrs.get('fLootESP_MaxDist', 120.0))

    for entity_id, entity in list(inst.entities.items()):
        try:
            if _bLootESP and getattr(entity, 'IsItem', False):
                try:
                    pos = getattr(entity, 'position', None) or getattr(entity, 'pos', None)
                    if pos:
                        wpos = (pos.x, pos.y, pos.z)
                        dist_i = Distance(cam_pos, wpos)
                        if dist_i <= _LOOT_MAX_DIST:
                            screen = _eng_cam.GetScreenPointFromWorldPoint(pos)
                            is_on_screen_i = screen.z > 0.0
                            bones_i = {"head": [screen.x, screen.y]} if is_on_screen_i else {}
                            item_id = _get_item_id(entity)
                            item_proto = lid.data.get(item_id, {}) if item_id else {}
                            item_label = item_proto.get('name', 'Unknown') if hasattr(item_proto, 'get') else 'Unknown'
                            item_count = 1
                            try:
                                item_count = int(getattr(entity, 'count', 0) or getattr(entity, 'item_count', 0) or 1)
                            except Exception:
                                pass
                            entities.append({
                                "is_item": True,
                                "item_label": item_label,
                                "item_count": item_count,
                                "distance": dist_i,
                                "is_on_screen": is_on_screen_i,
                                "bones": bones_i,
                            })
                except Exception:
                    pass
                continue

            if _bXray and isinstance(entity, CombatAvatar) and hasattr(entity, 'model'):
                entity.model.UseTechHighLightXray(param=xray_c, param2=(0,0,0,0), color2=xray_c)
            if not isinstance(entity, CombatAvatar):
                continue
            if not getattr(entity, 'model', None):
                continue
            if not entity.is_alive:
                continue
            if IsPlayerTeammate(entity_id):
                continue

            is_zombie = getattr(entity, 'IsRobotCombatAvatarZombie', False)
            if _bFilterZombie and is_zombie:
                continue
            if _bZombieHL and is_zombie:
                if hasattr(entity.model, 'UseTechHighLightXray'):
                    entity.model.UseTechHighLightXray(param=(1.0,0.5,0.0), param2=(0,0,0,0), color2=(1.0,0.5,0.0))

            entityHeadPos = entity.model.GetBoneWorldPosition('biped Head')
            if not entityHeadPos:
                continue

            _ehp   = (entityHeadPos.x, entityHeadPos.y, entityHeadPos.z)
            dist_e = Distance(cam_pos, _ehp)
            if dist_e > _MAX_DIST:
                continue

            model = entity.model
            head_screen = _eng_cam.GetScreenPointFromWorldPoint(entityHeadPos)
            is_on_screen = head_screen.z > 0.0

            bones = {}
            if is_on_screen:
                bones["head"] = [head_screen.x, head_screen.y]

            _bSkeleton = sync_attrs.get('bESP_Skeleton', False)
            if _bSkeleton and is_on_screen:
                for key, bone_name in _ESP_BONES:
                    s = get_bone_screen(model, bone_name)
                    if s:
                        bones[key] = s

            player_name  = getattr(entity, 'name', 'Unknown')
            is_bot       = bool(entity.IsRobotCombatAvatar)
            hp          = float(getattr(entity, 'hp', 0))
            maxhp       = float(getattr(entity, 'cur_maxhp', 100))
            armor       = float(getattr(entity, 'armor', 0.0))
            is_titan    = maxhp >= 2000.0
            is_knocked = bool(entity.is_dying_state)
            weapon_name = 'Punk'
            try:
                w = entity.GetCurWeapon()
                if w:
                    weapon_name = WEP_NAMES.get(int(w.equip_id), 'Punk')
            except: pass
            is_visible = IsVisible(_cam_pos, entityHeadPos, entity.id) if _bVisCheck else True

            entities.append({
                "name": player_name,
                "is_zombie": is_zombie,
                "is_bot": is_bot,
                "distance": dist_e,
                "is_on_screen": is_on_screen,
                "bones": bones,
                "is_visible": is_visible,
                "is_alive": True,
                "is_knocked": is_knocked,
                "is_health": hp,
                "cur_maxhp": maxhp,
                "is_armor": armor,
                "is_titan": is_titan,
                "weapon_name": weapon_name,
            })
        except:
            continue

    try:
        _cam_euler = _cam.euler_angles if hasattr(_cam, 'euler_angles') else None
        _cam_yaw   = float(_cam_euler.y) if _cam_euler else 0.0
        payload = json.dumps({
            "players": entities,
            "world_name": getattr(inst, 'world_name', 'Unknown'),
            "real_world_name": getattr(inst, 'real_world_name', ''),
            "is_moba_map": getattr(inst, 'is_moba_map', False),
            "is_replaying": getattr(inst, 'is_replaying', False),
            "total_entities": len(inst.entities),
            "cam_yaw": _cam_yaw,
            "screen_w": float(sync_attrs.get('fWidth', 1080)),
            "screen_h": float(sync_attrs.get('fHeight', 720)),
        }, separators=(",", ":")) + "\n"
        _write_udp(payload)
    except:
        pass

FpsPlacerInstance   = None
_ai_last_hash       = 0
_ai_idle_frames     = 0
_ai_tune_frame      = 0
_AI_TUNE_INTERVAL   = 30
_aim_pos_track     = {}
_AIM_MAX_LEAD_TIME = 0.6
_AIM_MIN_DT        = 0.02
_AIM_MAX_DT        = 0.25

_AI_MODE_INT = {
    'idle':0,'close':1,'mid':2,'long':3,'sniper':4,
    'smg':5,'cqc':6,'rush':7,'shotgun':8,'melee':9,
    'sky':10,'vehicle':11,'dmr':3,
}

_AIM_BONE_MAP = {
    0:'biped Head', 1:'biped Neck', 2:'biped Spine1',
    3:'biped Spine', 4:'biped Pelvis',
}

_AIM_SNIPER_IDS  = frozenset({39,47,77,103,122,45,133})
_AIM_SHOTGUN_IDS = frozenset({34,41,79,126})
_AIM_SMG_IDS     = frozenset({2,65,67,90,110,129})
_AIM_MELEE_IDS   = frozenset({33,84,95,96,97,106,107,112,131,134})

_AIM_PRESETS = {
    'sniper':  dict(snap=0.7,  fov=45.0,  pred=True,  bone=0, bspd=900.0),
    'long':    dict(snap=1.2,  fov=80.0,  pred=True,  bone=0, bspd=700.0),
    'mid':     dict(snap=2.0,  fov=120.0, pred=True,  bone=1, bspd=650.0),
    'smg':     dict(snap=3.0,  fov=150.0, pred=True,  bone=2, bspd=500.0),
    'close':   dict(snap=2.5,  fov=140.0, pred=True,  bone=1, bspd=600.0),
    'shotgun': dict(snap=3.5,  fov=170.0, pred=False, bone=2, bspd=400.0),
    'cqc':     dict(snap=4.5,  fov=190.0, pred=False, bone=2, bspd=400.0),
    'rush':    dict(snap=5.0,  fov=200.0, pred=False, bone=2, bspd=500.0),
    'melee':   dict(snap=6.0,  fov=200.0, pred=False, bone=4, bspd=300.0),
    'sky':     dict(snap=2.0,  fov=160.0, pred=True,  bone=1, bspd=600.0),
    'vehicle': dict(snap=3.5,  fov=180.0, pred=True,  bone=2, bspd=700.0),
}

def _cond_hash(mode, ec, ads, sprint, pose, hp_low, veh, sky):
    return hash((mode, min(ec, 6), ads, sprint, pose, hp_low, veh, sky))

@HOOK(FpsPlacer, 0)
def OnStaticTick(self, *_, **__):
    global FpsPlacerInstance
    if self:
        FpsPlacerInstance = self

def AIM_AutoTune(localEntity, enemy_data):
    global _ai_last_hash, _ai_idle_frames
    if not sync_attrs.get('bAIM_AutoTune', False):
        return
    try:
        if not enemy_data:
            _ai_idle_frames += 1
            if _ai_idle_frames > 10:
                sync_attrs.update({'iAI_Mode':0,'iAI_EnemyCount':0,'bAI_Active':False,'bAI_Changed':False})
                _aim_pos_track.clear()
            return
        _ai_idle_frames = 0
        dists       = [e['dist'] for e in enemy_data]
        avg_dist    = sum(dists) / len(dists)
        min_dist    = min(dists)
        enemy_count = len(enemy_data)
        try:
            w = localEntity.GetCurWeapon()
            wid = int(w.equip_id) if w else 0
        except: wid = 0
        is_ads    = bool(getattr(localEntity, 'is_ads', False))
        is_sprint = bool(getattr(localEntity, 'is_sprint', False))
        is_sky    = bool(getattr(localEntity, 'is_on_sky', False))
        is_veh    = bool(getattr(localEntity, 'is_vehicle_driver', False)) or bool(getattr(localEntity, 'is_vehicle_passenger', False))
        pose_type = int(getattr(localEntity, 'pose_type', 0))
        my_hp     = float(getattr(localEntity, 'hp', 100))
        my_maxhp  = float(getattr(localEntity, 'cur_maxhp', 100))
        hp_low    = (my_hp / max(my_maxhp, 1)) < 0.35
        try: optic = int(localEntity.GetCurWeaponPartOpticType() or 0)
        except: optic = 0
        if wid in _AIM_MELEE_IDS:   mode = 'melee'
        elif is_sky:                 mode = 'sky'
        elif is_veh:                 mode = 'vehicle'
        elif wid in _AIM_SNIPER_IDS and optic >= 4: mode = 'sniper'
        elif wid in _AIM_SHOTGUN_IDS:               mode = 'shotgun'
        elif wid in _AIM_SMG_IDS and min_dist < 20: mode = 'cqc'
        elif wid in _AIM_SMG_IDS:                   mode = 'smg'
        elif min_dist < 10:          mode = 'cqc'
        elif enemy_count >= 4 and avg_dist < 50: mode = 'rush'
        elif avg_dist > 120:         mode = 'long'
        elif avg_dist > 60:          mode = 'mid'
        else:                        mode = 'close'
        p = dict(_AIM_PRESETS.get(mode, _AIM_PRESETS['mid']))
        mod_flags = 0
        if enemy_count >= 3:
            p['fov'] = min(p['fov'] + 30.0, 220.0); mod_flags |= (1 << 4)
        if is_sprint:
            p['snap'] = min(p['snap'] * 1.5, 12.0); mod_flags |= (1 << 0)
        if is_ads and mode not in ('sniper', 'long'):
            p['snap'] = max(p['snap'] * 0.8, 0.5);  mod_flags |= (1 << 1)
        if pose_type == 2:
            p['snap'] = max(p['snap'] * 0.7, 0.4)
            p['fov']  = max(p['fov']  - 20.0, 30.0); mod_flags |= (1 << 2)
        if hp_low:
            p['snap'] = min(p['snap'] * 1.3, 12.0); mod_flags |= (1 << 3)
        cur_hash = _cond_hash(mode, enemy_count, is_ads, is_sprint, pose_type, hp_low, is_veh, is_sky)
        changed  = (cur_hash != _ai_last_hash)
        _ai_last_hash = cur_hash
        sync_attrs.update({
            'fAIM_SnapStrength': round(p['snap'], 2),
            'fAIM_Fov':          round(p['fov'], 1),
            'bAIM_Prediction':   p['pred'],
            'iAIM_TargetBone':   p['bone'],
            'fAIM_BulletSpeed':  p['bspd'],
            'iAI_Mode':          _AI_MODE_INT.get(mode, 0),
            'iAI_EnemyCount':    enemy_count,
            'fAI_AvgDist':       round(avg_dist, 1),
            'fAI_MinDist':       round(min_dist, 1),
            'iAI_ModFlags':      mod_flags,
            'bAI_Active':        True,
            'bAI_Changed':       changed,
            'fAI_SnapTuned':     round(p['snap'], 2),
            'fAI_FovTuned':      round(p['fov'], 1),
            'iAI_BoneTuned':     p['bone'],
        })
    except: pass

@TRY
def AIMUpdate(*_, **__):
    global FpsPlacerInstance, _ai_tune_frame
    try:
        if not sync_attrs.get('bAIM', False) or not genv.avatar:
            return
        localEntity = genv.avatar.combat_avatar
        if not localEntity or not localEntity.is_alive:
            return
        trigger = sync_attrs.get('iAIM_Trigger', 2)
        if trigger == 0 and not localEntity.is_shooting: return
        if trigger == 1 and not localEntity.is_ads:      return
        if trigger == 2 and not (localEntity.is_ads or localEntity.is_shooting): return
        if not FpsPlacerInstance:
            try:
                cam = Space._instance.camera
                if hasattr(cam, 'fps_placer'):
                    FpsPlacerInstance = cam.fps_placer
            except: return
        if not FpsPlacerInstance:
            return
        sw = sync_attrs.get('fWidth', 1080)
        sh = sync_attrs.get('fHeight', 720)
        if not (0 < sw <= 5000 and 0 < sh <= 5000):
            return
        cx         = sw * 0.5
        cy         = sh * 0.5
        max_fov    = min(sync_attrs.get('fAIM_Fov', 180.0), 400.0)
        max_dist   = min(sync_attrs.get('fAIM_MaxDist', 300.0), 500.0)
        check_vis  = sync_attrs.get('bAIM_CheckVisibility', False)
        ignore_bots    = sync_attrs.get('bAIM_IgnoreBots', False)
        ignore_knocked = sync_attrs.get('bAIM_IgnoreKnocked', True)
        smoothing      = 0.00021 * sync_attrs.get('fAIM_SnapStrength', 1.5)
        selected_bone  = _AIM_BONE_MAP.get(sync_attrs.get('iAIM_TargetBone', 1), 'biped Neck')
        use_pred       = sync_attrs.get('bAIM_Prediction', True)
        bullet_speed   = max(sync_attrs.get('fAIM_BulletSpeed', 600.0), 50.0)
        try:
            camera_pos    = Space._instance.camera.position
            entities_dict = Space._instance.entities
            if not camera_pos or not entities_dict:
                return
        except: return
        _ai_tune_frame += 1
        if _ai_tune_frame >= _AI_TUNE_INTERVAL or sync_attrs.get('bAI_Changed', False):
            if _ai_tune_frame >= _AI_TUNE_INTERVAL:
                _ai_tune_frame = 0
            temp_enemies = []
            for entityId, entity in list(entities_dict.items()):
                try:
                    if not isinstance(entity, CombatAvatar) or not entity.is_alive: continue
                    try:
                        if IsPlayerTeammate(entityId): continue
                    except: continue
                    if ignore_bots:
                        try:
                            if entity.IsRobotCombatAvatar: continue
                        except: pass
                    dist3d = Distance3D(localEntity.position, entity.position)
                    if dist3d <= max_dist:
                        temp_enemies.append({"dist": dist3d})
                except: continue
            AIM_AutoTune(localEntity, temp_enemies)
        candidates = []
        for entityId, entity in list(entities_dict.items()):
            try:
                if not hasattr(entity, 'is_alive') or not isinstance(entity, CombatAvatar): continue
                if not entity.is_alive: continue
                try:
                    if IsPlayerTeammate(entityId): continue
                except: continue
                if ignore_bots:
                    try:
                        if entity.IsRobotCombatAvatar: continue
                    except: pass
                if ignore_knocked:
                    try:
                        if entity.is_dying_state: continue
                    except: pass
                dist3d = Distance3D(localEntity.position, entity.position)
                if dist3d > max_dist: continue
                try:
                    bone_pos = entity.model.GetBoneWorldPosition(selected_bone) or entity.model.GetBoneWorldPosition('biped Head')
                    if not bone_pos: continue
                except: continue
                if check_vis:
                    try:
                        head_pos = entity.model.GetBoneWorldPosition('biped Head')
                        if not head_pos or not IsVisible(camera_pos, head_pos, entity.id): continue
                    except: continue
                try:
                    sp = WorldToScreenPoint(bone_pos)
                    if not hasattr(sp, 'z') or sp.z <= 0.01: continue
                except: continue
                pred_x, pred_y = sp.x, sp.y
                now_t = time.time()
                prev = _aim_pos_track.get(entityId)
                if use_pred and prev:
                    ptype = type(bone_pos)
                    px, py, pz, pt = prev
                    dt = now_t - pt
                    if _AIM_MIN_DT < dt < _AIM_MAX_DT:
                        try:
                            prev_world = ptype(px, py, pz)
                            prev_screen = WorldToScreenPoint(prev_world)
                            if hasattr(prev_screen, 'z') and prev_screen.z > 0.01:
                                vx = (sp.x - prev_screen.x) / dt
                                vy = (sp.y - prev_screen.y) / dt
                                lead_time = min(dist3d / bullet_speed, _AIM_MAX_LEAD_TIME)
                                lx = vx * lead_time
                                ly = vy * lead_time
                                lead_cap = max_fov * 1.5
                                lmag = (lx*lx + ly*ly) ** 0.5
                                if lmag > lead_cap and lmag > 0.0001:
                                    scale = lead_cap / lmag
                                    lx *= scale; ly *= scale
                                pred_x = sp.x + lx
                                pred_y = sp.y + ly
                        except: pass
                _aim_pos_track[entityId] = (bone_pos.x, bone_pos.y, bone_pos.z, now_t)
                dx = pred_x - cx; dy = pred_y - cy
                fov_dist = (dx*dx + dy*dy) ** 0.5
                if fov_dist <= max_fov:
                    candidates.append((fov_dist, pred_x, pred_y, dist3d, entityId))
            except: continue
        if not candidates:
            return
        candidates.sort(key=lambda x: x[0])
        _, tx, ty, _, _ = candidates[0]
        dx = tx - cx; dy = ty - cy
        if abs(dx) < 1.5 and abs(dy) < 1.5:
            return
        yaw   = max(min(dx * smoothing, 5.0), -5.0)
        pitch = max(min(-dy * smoothing, 5.0), -5.0)
        try:
            FpsPlacerInstance.Rotate(yaw, pitch)
        except: pass
    except: pass

def on_space_tick(dtime):
    try:
        if sync_attrs.get('bESP', True):
            EspUpdate()
        if sync_attrs.get('bAIM', False):
            AIMUpdate()
        PlayerMovementUpdate()
    except:
        pass

def PlayerMovementUpdate():
    try:
        if not genv.avatar:
            return
        localEntity = genv.avatar.combat_avatar
        if not localEntity:
            return

        if sync_attrs.get('bFly', False):
            try:
                cur_pos = localEntity.position
                y = sync_attrs.get('fFlyHeight', 50.0)
                if isinstance(cur_pos, (tuple, list)):
                    cx, cz = cur_pos[0], cur_pos[2]
                else:
                    cx, cz = cur_pos.x, cur_pos.z
                new_pos = (cx, y, cz)
                if hasattr(localEntity, 'ClientAdjustPosition'):
                    localEntity.ClientAdjustPosition(new_pos)
                else:
                    localEntity.position = new_pos
            except Exception:
                pass

        if sync_attrs.get('bTeleport', False):
            try:
                x = sync_attrs.get('fTeleportX', 0.0)
                y = sync_attrs.get('fTeleportY', 5.0)
                z = sync_attrs.get('fTeleportZ', 0.0)
                if hasattr(localEntity, 'ClientAdjustPosition'):
                    localEntity.ClientAdjustPosition((x, y, z))
                else:
                    localEntity.position = (x, y, z)
            except Exception:
                pass
            sync_attrs['bTeleport'] = False
    except Exception:
        pass

@HOOK(PlayerCombatAvatar, 0)
def on_enter_space(self, *args, **kwargs):
    result = PlayerCombatAvatar._on_enter_space(self, *args, **kwargs)
    Space._instance.AddSpacePostTick(on_space_tick)
    return result

try:
    from gclient.gameplay.logic_base.entities.combatavatarmembers import cimp_report
    if not hasattr(cimp_report.PlayerCombatAvatarMember, '_orig_generate_report'):
        cimp_report.PlayerCombatAvatarMember._orig_generate_report = cimp_report.PlayerCombatAvatarMember.GenerateReportDataInGame

    @HOOK(cimp_report.PlayerCombatAvatarMember, 0)
    def GenerateReportDataInGame(self, *args, **kwargs):
        if sync_attrs.get('bBlockReport', False):
            return {}
        return cimp_report.PlayerCombatAvatarMember._orig_generate_report(self, *args, **kwargs)

except Exception:
    pass

# ============================================================
# PLAYER MODS
# ============================================================

from gclient.gameplay.logic_base.entities.combat_avatar import PlayerCombatAvatar

@HOOK(PlayerCombatAvatar, 0)
def OnDead(self, *args, **kwargs):
    if sync_attrs.get('bGodMode', False):
        if hasattr(self, 'hp'):
            self.hp = self.cur_maxhp
        return
    return PlayerCombatAvatar._OnDead(self, *args, **kwargs)

@HOOK(PlayerCombatAvatar, 0)
def OnDying(self, is_dying, *args, **kwargs):
    if sync_attrs.get('bGodMode', False) or sync_attrs.get('bAntiKnock', False):
        if is_dying:
            return
    return PlayerCombatAvatar._OnDying(self, is_dying, *args, **kwargs)

@HOOK(PlayerCombatAvatar, 0)
def ApplyDamageImpact(self, *args, **kwargs):
    if sync_attrs.get('bGodMode', False):
        return
    if sync_attrs.get('bNoFallDamage', False):
        try:
            if args and len(args) > 0:
                damage_data = args[0]
                if hasattr(damage_data, 'fall_damage') and damage_data.fall_damage:
                    return
        except:
            pass
    return PlayerCombatAvatar._ApplyDamageImpact(self, *args, **kwargs)

@HOOK(PlayerCombatAvatar, 0)
def AddBuffSpell(self, spell_id, spell_level, item_guid, *args, **kwargs):
    result = PlayerCombatAvatar._AddBuffSpell(self, spell_id, spell_level, item_guid, *args, **kwargs)
    if sync_attrs.get('bNoCooldown', False):
        if hasattr(self, 'skill_cooldown'):
            self.skill_cooldown = 0.0
        if hasattr(self, 'skill_timer'):
            self.skill_timer = 0.0
    return result

@HOOK(PlayerCombatAvatar, 0)
def AddCameraEffect(self, effect_id, keep_time=-1, color=None, holding=False, *args, **kwargs):
    if sync_attrs.get('bNoFlash', False):
        if effect_id == 7:
            return
    return PlayerCombatAvatar._AddCameraEffect(self, effect_id, keep_time, color, holding, *args, **kwargs)

@HOOK(PlayerCombatAvatar, 0)
def GetRealJumpVecY(self, jump_time_interval, accumulated_time=0, *args, **kwargs):
    result = PlayerCombatAvatar._GetRealJumpVecY(self, jump_time_interval, accumulated_time, *args, **kwargs)
    if sync_attrs.get('bSuperJump', False) and result is not None:
        try:
            mult = sync_attrs.get('fJumpMult', 2.0)
            return result * mult
        except Exception:
            pass
    return result

@HOOK(PlayerCombatAvatar, 0)
def CheckSuitableExecuteTargetPos(self, target_id, *args, **kwargs):
    if sync_attrs.get('bExecuteRangeHack', False):
        try:
            max_range = sync_attrs.get('fExecuteRange', 1.5)
            sp = getattr(Space, '_instance', None)
            target = sp.entities.get(target_id) if sp and hasattr(sp, 'entities') else None
            if target is not None:
                sm = getattr(self, 'model', None)
                tm = getattr(target, 'model', None)
                if sm and tm:
                    sp_pos = self.position
                    tp_pos = target.position
                    sx = sp_pos[0] if isinstance(sp_pos, (tuple, list)) else sp_pos.x
                    sz = sp_pos[2] if isinstance(sp_pos, (tuple, list)) else sp_pos.z
                    tx = tp_pos[0] if isinstance(tp_pos, (tuple, list)) else tp_pos.x
                    tz = tp_pos[2] if isinstance(tp_pos, (tuple, list)) else tp_pos.z
                    dist = ((sx - tx) ** 2 + (sz - tz) ** 2) ** 0.5
                    if dist <= max_range:
                        return True
        except Exception:
            pass
    return PlayerCombatAvatar._CheckSuitableExecuteTargetPos(self, target_id, *args, **kwargs)

@HOOK(PlayerCombatAvatar, 0)
def CheckCanExecuteForCollision(self, target_id, *args, **kwargs):
    if sync_attrs.get('bExecuteRangeHack', False):
        return True
    return PlayerCombatAvatar._CheckCanExecuteForCollision(self, target_id, *args, **kwargs)

@HOOK(PlayerCombatAvatar, 0)
def _OnChangeToCloseParachute(self, old, *args, **kwargs):
    if sync_attrs.get('bFly', False):
        return
    return PlayerCombatAvatar.__OnChangeToCloseParachute(self, old, *args, **kwargs)

@HOOK(PlayerCombatAvatar, 0)
def on_setattr(self, key, old, new, *args, **kwargs):
    result = PlayerCombatAvatar._on_setattr(self, key, old, new, *args, **kwargs)
    try:
        if sync_attrs.get('bInfiniteStamina', False):
            if key == 'stamina':
                self.stamina = 100.0
        
        if sync_attrs.get('bSpeedHack', False):
            if key == 'speed':
                mult = sync_attrs.get('fSpeedMult', 1.5)
                self.speed = new * mult
        
        if sync_attrs.get('bWaterWalk', False):
            if key == 'position' or key == 'pos':
                water_level = sync_attrs.get('fWaterLevel', 0.0)
                if new.y < water_level:
                    if hasattr(self, 'position'):
                        self.position = (new.x, water_level + 0.5, new.z)
                    else:
                        self.pos = (new.x, water_level + 0.5, new.z)
        
        if sync_attrs.get('bAirStuck', False):
            if key == 'position' or key == 'pos':
                if hasattr(self, 'is_on_sky') and self.is_on_sky:
                    height = sync_attrs.get('fAirStuckHeight', 30.0)
                    if hasattr(self, 'position'):
                        self.position = (new.x, height, new.z)
                    else:
                        self.pos = (new.x, height, new.z)
        
        if sync_attrs.get('bNoGravity', False):
            if key == 'gravity':
                self.gravity = 0.0
    except:
        pass
    return result

# ============================================================
# WEAPON MODS
# ============================================================

@HOOK(WeaponCase, 0)
def GetWeaponAttrValue(self, name, default=None):
    result = WeaponCase._GetWeaponAttrValue(self, name, default)
    try:
        if sync_attrs.get('bNoRecoil', False):
            if name in ('HorizontalRecoilAnimScale', 'VerticalRecoilAnimScale'):
                return 0.0
            if 'Recoil' in name:
                return 0.0
        
        if sync_attrs.get('bNoSpread', False):
            if 'Scatter' in name or 'Spread' in name:
                return 0.0
        
        if sync_attrs.get('bNoSway', False):
            if name == 'SightStabilize' or 'Sway' in name:
                return 0.0
        
        if sync_attrs.get('bRapidFire', False):
            if 'FireIntervalTimeConf' in name:
                mult = sync_attrs.get('fFireRateMult', 2.0)
                if mult != 1.0:
                    return result / max(mult, 0.1)
            if 'BurstIntervalTimeConf' in name:
                mult = sync_attrs.get('fFireRateMult', 2.0)
                if mult != 1.0:
                    return result / max(mult, 0.1)
        
        if sync_attrs.get('bFastReload', False):
            if 'ReloadAnimTimeConf' in name:
                mult = sync_attrs.get('fReloadSpeedMult', 2.0)
                if mult != 1.0:
                    return result / max(mult, 0.1)
            if 'EmptyReloadAnimTimeConf' in name:
                mult = sync_attrs.get('fReloadSpeedMult', 2.0)
                if mult != 1.0:
                    return result / max(mult, 0.1)
        
        if sync_attrs.get('bInstantSwitch', False):
            if name == 'ChangeWeaponSpeed':
                mult = sync_attrs.get('fSwitchSpeedMult', 3.0)
                if mult != 1.0:
                    return result / max(mult, 0.1)
        
        if sync_attrs.get('bInstantScope', False):
            if 'AdsUpAnimTimeConf' in name:
                mult = sync_attrs.get('fADSSpeedMult', 3.0)
                if mult != 1.0:
                    return result / max(mult, 0.1)
            if 'AdsUpFovTime' in name:
                mult = sync_attrs.get('fADSSpeedMult', 3.0)
                if mult != 1.0:
                    return result / max(mult, 0.1)
        
        if sync_attrs.get('bInfiniteAmmo', False):
            if 'Ammo' in name:
                return 9999
    except:
        pass
    return result

@HOOK(WeaponCase, 0)
def GetWeaponAttrValueImmediately(self, name, default=None):
    result = WeaponCase._GetWeaponAttrValueImmediately(self, name, default)
    try:
        if sync_attrs.get('bRapidFire', False):
            if 'FireIntervalTimeConf' in name:
                mult = sync_attrs.get('fFireRateMult', 2.0)
                if mult != 1.0:
                    return result / max(mult, 0.1)
            if 'BurstIntervalTimeConf' in name:
                mult = sync_attrs.get('fFireRateMult', 2.0)
                if mult != 1.0:
                    return result / max(mult, 0.1)
        
        if sync_attrs.get('bFastReload', False):
            if 'ReloadAnimTimeConf' in name:
                mult = sync_attrs.get('fReloadSpeedMult', 2.0)
                if mult != 1.0:
                    return result / max(mult, 0.1)
            if 'EmptyReloadAnimTimeConf' in name:
                mult = sync_attrs.get('fReloadSpeedMult', 2.0)
                if mult != 1.0:
                    return result / max(mult, 0.1)
        
        if sync_attrs.get('bInstantSwitch', False):
            if name == 'ChangeWeaponSpeed':
                mult = sync_attrs.get('fSwitchSpeedMult', 3.0)
                if mult != 1.0:
                    return result / max(mult, 0.1)
        
        if sync_attrs.get('bInstantScope', False):
            if 'AdsUpAnimTimeConf' in name:
                mult = sync_attrs.get('fADSSpeedMult', 3.0)
                if mult != 1.0:
                    return result / max(mult, 0.1)
            if 'AdsUpFovTime' in name:
                mult = sync_attrs.get('fADSSpeedMult', 3.0)
                if mult != 1.0:
                    return result / max(mult, 0.1)
    except:
        pass
    return result

@HOOK(WeaponCase, 0)
def GetAmmo(self):
    if sync_attrs.get('bInfiniteAmmo', False):
        return 9999
    return WeaponCase._GetAmmo(self)

@HOOK(WeaponCase, 0)
def GetMaxAmmo(self):
    if sync_attrs.get('bInfiniteAmmo', False):
        return 9999
    return WeaponCase._GetMaxAmmo(self)

@HOOK(WeaponCase, 0)
def NeedReload(self):
    if sync_attrs.get('bInfiniteAmmo', False):
        return False
    return WeaponCase._NeedReload(self)

print("[ALL FEATURES] Loaded successfully!")
print("  - Weapon: No Recoil, No Spread, No Sway, Rapid Fire, Fast Reload, Instant Switch, Instant Scope, Infinite Ammo")
print("  - Player: God Mode, Fly, Teleport, Speed Hack, Super Jump, No Cooldown, Infinite Stamina, No Fall Damage, Water Walk, Air Stuck, No Gravity, No Flash")
print("  - Skin: Weapon Skin Hack, Hero Skin Hack")
print("  - Visual: ESP, Loot ESP, Skeleton ESP")
print("  - Aimbot: Auto Aim, Prediction, Auto Tune")
print("  - Misc: FPS Unlock, Kill Broadcast, Report Blocker")

)srcpy").chr();
