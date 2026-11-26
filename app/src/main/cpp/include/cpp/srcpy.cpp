#include "../obfuscate.h"

const char* srcpy = O(R"srcpy(
import json, threading, functools, time, socket, os

# ============================================================
# 🔥 RUNTIME DATA (file-based, read-only from our side)
# ============================================================
# The C++ side owns data.json: the menu writes a flat {key: value} snapshot of
# the live toggles and we only read it. No port, no handshake, nothing lost if a
# datagram is dropped. Python still needs one socket to push entity data back.
#
# data.json is NOT config.json. config.json is the menu's own nested
# {"bool":..,"float":..,"int":..} settings file that has to survive a restart;
# we must never write it, and the C++ side keeps the two apart on purpose.
# The C++ side writes into GetConfigDir(), which is the *internal* app data
# directory (ANativeActivity::internalDataPath + "/Nexora/") whenever the
# activity is up - no storage permission needed, and unlike /storage/emulated/0
# it is guaranteed to exist and be writable. We must look in the same place.
# The exact path depends on how the sandbox rewrites it, so probe a few
# candidates and keep whichever exists. Getting this wrong is not cosmetic:
# sync_attrs stays empty, fWidth/fHeight fall back to 1080x720, and C++ then
# rescales every bone by Height/720 - the ESP drifts far off the players and
# wobbles as the camera pans.
def _proc_pkg():
    try:
        with open('/proc/self/cmdline', 'rb') as f:
            name = f.read().split(b'\x00')[0].decode('utf-8', 'replace')
    except Exception:
        return 'com.netease.newspike'
    # A sandboxed process is often "<pkg>:<suffix>"; the data dir is per package.
    return name.split(':')[0] or 'com.netease.newspike'

_PKG = _proc_pkg()
_DATA_CANDIDATES = [
    '/data/data/%s/files/Nexora/data.json'   % _PKG,
    '/data/user/0/%s/files/Nexora/data.json' % _PKG,
    '/data/data/%s/Nexora/data.json'         % _PKG,
    '/storage/emulated/0/Nexora/data.json',
]
DATA_PATH     = _DATA_CANDIDATES[0]
UDP_RECV_PORT = 50001   # Python -> C++ (ESP entity data)

_send_sock = None
_udp_initialized = False

def _init_udp():
    global _send_sock, _udp_initialized
    if _udp_initialized:
        return
    try:
        _send_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        _send_sock.setsockopt(socket.SOL_SOCKET, socket.SO_SNDBUF, 2*1024*1024)
        _send_sock.setsockopt(socket.IPPROTO_IP, socket.IP_TOS, 0x10)
        _send_sock.settimeout(0.001)
        _udp_initialized = True
    except Exception as e:
        print(f"[UDP] Init error: {e}")

def _write_udp(data):
    """Send ESP data to C++"""
    global _send_sock
    if not _udp_initialized:
        _init_udp()
    try:
        if isinstance(data, str):
            data = data.encode('utf-8')
        _send_sock.sendto(data, ("127.0.0.1", UDP_RECV_PORT))
    except Exception:
        pass

# ============================================================
# 🔥 DATA READER
# ============================================================
# sync_attrs is kept as a plain dict so every existing
# sync_attrs.get('bESP_...') call site works unchanged.
sync_attrs = {}
_data_mtime = None
_data_lock = threading.Lock()

def _read_data():
    """Re-read data.json if it changed on disk. Cheap: one stat per tick."""
    global sync_attrs, _data_mtime, DATA_PATH
    try:
        st = os.stat(DATA_PATH)
    except Exception:
        # Not at the current path yet (or it moved): re-probe the candidates so
        # we pick the file up as soon as C++ creates it.
        for p in _DATA_CANDIDATES:
            try:
                os.stat(p)
                DATA_PATH = p
                _data_mtime = None
                print(f"[CFG] data.json found at {p}")
                break
            except Exception:
                continue
        else:
            return
        try:
            st = os.stat(DATA_PATH)
        except Exception:
            return
    try:
        mtime = st.st_mtime_ns
        if mtime == _data_mtime:
            return
        with open(DATA_PATH, 'r') as f:
            parsed = json.load(f)
        if isinstance(parsed, dict):
            with _data_lock:
                sync_attrs.update(parsed)
            _data_mtime = mtime
    except Exception:
        pass

def _data_thread():
    while True:
        _read_data()
        time.sleep(0.1)

# ============================================================
# 🔥 START DATA READER
# ============================================================
_read_data()
if sync_attrs:
    print(f"[CFG] data.json = {DATA_PATH} ({len(sync_attrs)} keys)")
else:
    print(f"[CFG] data.json not readable yet; pkg={_PKG} candidates={_DATA_CANDIDATES}")
threading.Thread(target=_data_thread, daemon=True).start()

# ============================================================
# 🔥 GAME IMPORTS — only what aim / visuals / weapon need
# ============================================================
from gclient.framework.entities.space import Space
from gclient.framework.util.replay_util import IsPlayerTeammate
from gclient.framework.camera.fps_placer import FpsPlacer
from gclient.gameplay.logic_base.entities.combat_avatar import CombatAvatar, PlayerCombatAvatar
from gclient.gameplay.logic_base.equips.weapon_case import WeaponCase
from gshare.formula import Distance3D

# Weapon display names for the ESP panel, read lazily from the game's own equip
# table. The old build_auto_wep_dictionary() walked every entry of
# gun_skin_template_base_data at import time to build four dicts (WEP,
# WEP_NAMES, WEP_SKIN_INFO, SKIN_TO_EQUIP) that only the skin hack used, and
# kept them alive for the whole session. This touches one dict entry per weapon
# actually on screen, and the table is one the game already has loaded.
try:
    import gclient.data.equip_data as _equip_data
except Exception:
    _equip_data = None

# Loot ESP labels. This used to be imported inside the hero-skin block, so the
# loot path depended on that block's try/except succeeding; it is a plain
# module-level import now that the skin code is gone.
try:
    import gclient.data.lobby_item_data as _lobby_item_data
except Exception:
    _lobby_item_data = None

def _wep_name(equip_id):
    try:
        info = _equip_data.data.get(equip_id) if _equip_data else None
        if info:
            name = info.get('name')
            if name:
                return name
    except Exception:
        pass
    return 'Punk'

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
    _bHealth = sync_attrs.get('bESP_Health', True)
    _bWeapon = sync_attrs.get('bESP_Weapon', True)
    _bSkeleton = sync_attrs.get('bESP_Skeleton', False)

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
                            item_proto = _lobby_item_data.data.get(item_id, {}) if (item_id and _lobby_item_data) else {}
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

            if _bSkeleton and is_on_screen:
                for key, bone_name in _ESP_BONES:
                    s = get_bone_screen(model, bone_name)
                    if s:
                        bones[key] = s

            player_name  = getattr(entity, 'name', 'Unknown')
            is_bot       = bool(entity.IsRobotCombatAvatar)
            is_knocked = bool(entity.is_dying_state)
            is_visible = IsVisible(_cam_pos, entityHeadPos, entity.id) if _bVisCheck else True
            entry = {
                "name": player_name,
                "is_zombie": is_zombie,
                "is_bot": is_bot,
                "distance": dist_e,
                "is_on_screen": is_on_screen,
                "bones": bones,
                "is_visible": is_visible,
                "is_alive": True,
                "is_knocked": is_knocked,
            }
            # Only gather what the enabled C++ overlay actually draws. The
            # parser on the other side is j.value(key, default), so a key that
            # is left out simply falls back to its default - and the GetCurWeapon
            # lookup below is an engine call per entity per frame, so it is worth
            # skipping when the weapon label is off.
            if _bHealth:
                entry["is_health"] = float(getattr(entity, 'hp', 0))
                entry["cur_maxhp"] = float(getattr(entity, 'cur_maxhp', 100))
                entry["is_armor"] = float(getattr(entity, 'armor', 0.0))
            if _bWeapon:
                try:
                    w = entity.GetCurWeapon()
                    if w:
                        entry["weapon_name"] = _wep_name(int(w.equip_id))
                except: pass
            entities.append(entry)
        except:
            continue

    try:
        _cam_euler = _cam.euler_angles if hasattr(_cam, 'euler_angles') else None
        _cam_yaw   = float(_cam_euler.y) if _cam_euler else 0.0
        _payload = {
            "players": entities,
            "world_name": getattr(inst, 'world_name', 'Unknown'),
            "real_world_name": getattr(inst, 'real_world_name', ''),
            "is_moba_map": getattr(inst, 'is_moba_map', False),
            "is_replaying": getattr(inst, 'is_replaying', False),
            "total_entities": len(inst.entities),
            "cam_yaw": _cam_yaw,
        }
        # Only claim a resolution when it came from the config. Sending a guess
        # here is what made C++ rescale bones by Height/720 and draw the ESP
        # nowhere near the players; omitting the keys makes C++ draw 1:1.
        if 'fWidth' in sync_attrs and 'fHeight' in sync_attrs:
            _payload["screen_w"] = float(sync_attrs['fWidth'])
            _payload["screen_h"] = float(sync_attrs['fHeight'])
        _write_udp(json.dumps(_payload, separators=(",", ":")) + "\n")
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
    global FpsPlacerInstance, _ai_tune_frame, _aim_pos_track
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
        # No guessing here: a wrong screen centre silently misplaces the whole
        # FOV circle (cy was 360 instead of ~1200 on a 1080x2400 panel). The
        # values come from the menu via data.json, so wait for them.
        sw = sync_attrs.get('fWidth', 0)
        sh = sync_attrs.get('fHeight', 0)
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
        # Prune aim tracker to prevent unbounded growth
        aim_ids = {c[4] for c in candidates}
        _aim_pos_track = {k: v for k, v in _aim_pos_track.items() if k in aim_ids}
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
    except:
        pass

@HOOK(PlayerCombatAvatar, 0)
def on_enter_space(self, *args, **kwargs):
    result = PlayerCombatAvatar._on_enter_space(self, *args, **kwargs)
    Space._instance.AddSpacePostTick(on_space_tick)
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
