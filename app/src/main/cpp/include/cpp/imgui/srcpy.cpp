#include "../obfuscate.h"

const char *srcpy = O(R"srcpy(
PACKAGE_NAME = "com.netease.newspike"
sync_attrs = {}
REGISTER_UPDATES = []

import sys, traceback, time, os
import inspect

def represent(obj, detailed=True):
    if isinstance(obj, type):
        if obj.__module__ != 'builtins': return f"{obj.__module__}.{obj.__qualname__}"
    else:
        cls = type(obj)
        if getattr(obj, '__module__', 'builtins') != 'builtins' and cls.__repr__ == object.__repr__:
            return f"{(getattr(cls, '__module__', '') + '.') if detailed else ''}{cls.__name__}({hex(id(obj))})"
    
    try: return repr(obj)
    except: return f"{(getattr(cls, '__module__', '') + '.') if detailed else ''}{cls.__name__}({hex(id(obj))})"

def irepr(*args, **kwargs):
    sargs = ([represent(args[0], False)] + [represent(a) for a in args[1:]]) if args else []
    return ', '.join(sargs + [f"{k}={represent(v)}" for k, v in kwargs.items()])

import functools
from typing import Union

def HOOK(clazz, bLog: Union[int, bool] = True, _name=None):
    def decorator(func, _name=_name):
        name = func.__name__
        _name = _name or '_' + name
        
        original = getattr(clazz, name, None)
        setattr(clazz, _name, original)
        
        import functools
        @functools.wraps(original)
        def wrapper(*args, **kwargs):
            try:
                if bLog: print(f"HENTR: {clazz.__name__}.{name}({irepr(*args, **kwargs)})")
                result = func(*args, **kwargs)
                if bLog: print(f"HEXIT: [bold magenta]{clazz.__name__}.{name}[/bold magenta] -> {represent(result)}")
                return result
            except: print(__import__('traceback').format_exc())
        
        setattr(clazz, name, wrapper)
        return func
    return decorator

def TRY(_func=None, *, bLog=False):
    def decorator(original):
        @functools.wraps(original)
        def wrapper(*args, **kwargs):
            try:
                if bLog: print(f"TRY: {original.__name__}({irepr(args, kwargs)})")
                result = original(*args, **kwargs)
                if bLog: print(f"TRY: [bold magenta]{original.__name__}[/bold magenta] -> {represent(result)}")
                return result
            except Exception: print(__import__('traceback').format_exc())
        return wrapper
    return decorator(_func) if _func else decorator

def HOOKO(clazz, name, replace = None, log = False):
    if not replace: replace = lambda *_, **__: None
    _name = '_' + name
    func = getattr(clazz, name)
    setattr(clazz, _name, func)
    def _(*_, **__):
        try:
            if log: print(f"LOG: {clazz.__name__}.{name}({irepr(*args, **kwargs)})")
            return replace(*_, **__)
        except: print(__import__('traceback').format_exc())
    setattr(clazz, name, _)

def LOGC(clazz, exclude_methods=None):
    import inspect, types
    from functools import wraps
    
    exclude_methods = set(exclude_methods or ''.split('.'))
    
    for name in clazz.__dict__:
        if name in exclude_methods or (name != '__init__' and name.startswith('__')): continue
        attr = clazz.__dict__[name]
        if not isinstance(attr, (classmethod, staticmethod, types.FunctionType)): continue
        original_func = attr.__func__ if isinstance(attr, (classmethod, staticmethod)) else attr
        cm = isinstance(attr, classmethod)
        sm = isinstance(attr, staticmethod)
        is_async = inspect.iscoroutinefunction(original_func)
        
        def create_wrapper(func, name, cm, sm, is_async):
            @wraps(func)
            def sync_wrapper(*args, **kwargs):
                try:
                    bNone = set(''.split('.'))
                    bFalse = set(''.split('.'))
                    bTrue = set(''.split('.'))
                    bLog = name not in set(''.split('.')) | bFalse | bNone
                    if bLog: print(f"ENTR: {clazz.__name__}.{name}({irepr(*args, **kwargs)})")
                    result = func(*args, **kwargs)
                    if bLog: print(f"EXIT: [bold magenta]{clazz.__name__}.{name}[/bold magenta] -> {represent(result)}")
                    return result
                except Exception as e:
                    print(traceback.format_exc())
                    print(f"ERRR in {clazz.__name__}.{name}: {str(e)}")
                    raise
            @wraps(func)
            async def async_wrapper(*args, **kwargs):
                try:
                    print(f"ENTR_ASYNC: {clazz.__name__}.{name}({irepr(*args, **kwargs)})")
                    result = await func(*args, **kwargs)
                    print(f"EXIT_ASYNC: [bold magenta]{clazz.__name__}.{name}[/bold magenta] -> {represent(result)}")
                    return result
                except Exception as e:
                    print(traceback.format_exc())
                    print(f"ERRR_ASYNC in {clazz.__name__}.{name}: {str(e)}")
                    raise
            return async_wrapper if is_async else sync_wrapper
        wrapper = create_wrapper(original_func, name, cm, sm, is_async)
        if cm: setattr(clazz, name, classmethod(wrapper))
        elif sm: setattr(clazz, name, staticmethod(wrapper))
        else: setattr(clazz, name, wrapper)

def LOGM(module, exclude_clazz=None, blab=False):
    import inspect
    exclude_clazz = set(exclude_clazz or 'Singleton.enum.StoryTick'.split('.'))
    for name, clazz in inspect.getmembers(module, inspect.isclass):
        if name in exclude_clazz: continue
        if blab:
            if clazz.__module__ != getattr(module, '__name__', clazz.__module__): continue
        LOGC(clazz)

def LOG(obj):
    LOGM(obj)
    LOGC(obj)

from gclient.framework.entities.space import Space

def WorldToScreenPoint(worldpos):
    return Space._instance.camera.engine_camera.GetScreenPointFromWorldPoint(worldpos)

def IsVisible(local, target, targetId, distance=100):
    raycast = Space._instance.RawRaycast(local, distance, 19, with_trigger=False, to_pos=target)
    ownerid = getattr(raycast.Body, 'ownerid', None)
    if raycast.Body and ownerid:
        if ownerid == targetId: return True
    elif raycast.Flags == 7: return True
    return False

def igetattr(*_, **__):
    try: return getattr(*_, **__)
    except: return 'igetattr.error'

import inspect
def idir(obj): return {k: igetattr(obj, k, 'undefined') for k in dir(obj)}

def stacktrace():
    for frame_info in reversed(inspect.stack()):
        print(f"File: {frame_info.filename}, Line: {frame_info.lineno}, Function: {frame_info.function}")

def trace_handler(frame, event, arg):
    if event == 'call':
        with open('kk.log', 'a') as w:
            w.write(f'Calling function: {frame.f_code.co_name} at line {frame.f_lineno}\n')
    elif event == 'line':
        with open('kk.log', 'a') as a:
            a.write(f'Executing line {frame.f_lineno} in {frame.f_code.co_name}\n')
    elif event == 'return':
        with open('kk.log', 'a') as a:
            a.write(f'Returning from function: {frame.f_code.co_name} with value {arg}\n')
    return trace_handler

import random
def trueChance(probability): return random.random() < probability
def randomHitOffset():
    return random.uniform(0.05, 0.1) * random.choice([1, -1])

def V3Minus(a, b): return (a.x - b.x, a.y - b.y, a.z - b.z)
def V32T(v):
    try: return v.x, v.y, v.z
    except: return v[0], v[1], v[2]

def normalize(x, y, z):
    mag = (x**2 + y**2 + z**2) ** 0.5
    epsilon = 1e-8
    if mag < epsilon: return (x, y, z)
    return (x/mag, y/mag, z/mag)

def Distance(a, b):
    dx = b[0] - a[0]
    dy = b[1] - a[1]
    dz = b[2] - a[2]
    return (dx**2 + dy**2 + dz**2) ** 0.5

def v2Distance(a, b):
    dx = b[0] - a[0]
    dy = b[1] - a[1]
    return (dx**2 + dy**2) ** 0.5

from gshare.formula import Distance2D, Normalize2D
from gshare.formula import Distance3D, Normalize3D

class Vector2:
    def __init__(self, x: float = 0, y: float = 0):
        self.x = float(x); self.y = float(y)
    def __getitem__(self, key):
        if key in (0, -2): return self.x
        elif key in (1, -1): return self.y
        else: raise IndexError(f"Vector2 index out of range {key=}")
    def __setitem__(self, key, value):
        if key in (0, -2): self.x = float(value)
        elif key in (1, -1): self.y = float(value)
        else: raise IndexError(f"Vector2 index out of range {key=}")
    def __repr__(self): return f"Vector2({self.x}, {self.y})"
    def __add__(self, other):
        if isinstance(other, Vector2): return Vector2(self.x + other.x, self.y + other.y)
        elif isinstance(other, tuple) and len(other) == 2: return Vector2(self.x + other[0], self.y + other[1])
        elif isinstance(other, (int, float)): return Vector2(self.x + other, self.y + other)
        else: raise TypeError(f"Cannot add Vector2 and {type(other)}")
    def __radd__(self, other): return self.__add__(other)
    def __sub__(self, other):
        if isinstance(other, Vector2): return Vector2(self.x - other.x, self.y - other.y)
        elif isinstance(other, tuple) and len(other) == 2: return Vector2(self.x - other[0], self.y - other[1])
        elif isinstance(other, (int, float)): return Vector2(self.x - other, self.y - other)
        else: raise TypeError(f"Cannot subtract {type(other)} from Vector2")
    def __rsub__(self, other):
        if isinstance(other, tuple) and len(other) == 2: return Vector2(other[0] - self.x, other[1] - self.y)
        elif isinstance(other, (int, float)): return Vector2(other - self.x, other - self.y)
        else: raise TypeError(f"Cannot subtract Vector2 from {type(other)}")
    def __mul__(self, other):
        if isinstance(other, Vector2): return Vector2(self.x * other.x, self.y * other.y)
        elif isinstance(other, tuple) and len(other) == 2: return Vector2(self.x * other[0], self.y * other[1])
        elif isinstance(other, (int, float)): return Vector2(self.x * other, self.y * other)
        else: raise TypeError(f"Cannot multiply Vector2 and {type(other)}")
    def __rmul__(self, other): return self.__mul__(other)
    def __truediv__(self, other):
        if isinstance(other, Vector2):
            if other.x == 0 or other.y == 0: raise ZeroDivisionError("Division by zero in Vector2")
            return Vector2(self.x / other.x, self.y / other.y)
        elif isinstance(other, tuple) and len(other) == 2:
            if 0 in other: raise ZeroDivisionError("Division by zero in Vector2")
            return Vector2(self.x / other[0], self.y / other[1])
        elif isinstance(other, (int, float)):
            if other == 0: raise ZeroDivisionError("Division by zero in Vector2")
            return Vector2(self.x / other, self.y / other)
        else: raise TypeError(f"Cannot divide Vector2 by {type(other)}")
    def __rtruediv__(self, other):
        if isinstance(other, tuple) and len(other) == 2:
            if self.x == 0 or self.y == 0: raise ZeroDivisionError("Division by zero in Vector2")
            return Vector2(other[0] / self.x, other[1] / self.y)
        elif isinstance(other, (int, float)):
            if self.x == 0 or self.y == 0: raise ZeroDivisionError("Division by zero in Vector2")
            return Vector2(other / self.x, other / self.y)
        else: raise TypeError(f"Cannot divide {type(other)} by Vector2")
    def __neg__(self): return Vector2(-self.x, -self.y)
    def __eq__(self, other):
        if not isinstance(other, Vector2): return False
        return self.x == other.x and self.y == other.y
    def __ne__(self, other): return not self.__eq__(other)
    def __iter__(self):
        yield self.x; yield self.y

class Vector3:
    def __init__(self, x: float = 0, y: float = 0, z: float = 0):
        self.x = float(x); self.y = float(y); self.z = float(z)
    def __getitem__(self, key):
        if key in (0, -3): return self.x
        elif key in (1, -2): return self.y
        elif key in (2, -1): return self.z
        else: raise IndexError("Vector3 index out of range")
    def __setitem__(self, key, value):
        if key in (0, -3): self.x = float(value)
        elif key in (1, -2): self.y = float(value)
        elif key in (2, -1): self.z = float(value)
        else: raise IndexError("Vector3 index out of range")
    def __repr__(self): return f"Vector3({self.x}, {self.y}, {self.z})"
    def __add__(self, other):
        if isinstance(other, Vector3): return Vector3(self.x + other.x, self.y + other.y, self.z + other.z)
        elif isinstance(other, tuple) and len(other) == 3: return Vector3(self.x + other[0], self.y + other[1], self.z + other[2])
        elif isinstance(other, (int, float)): return Vector3(self.x + other, self.y + other, self.z + other)
        else: raise TypeError(f"Cannot add Vector3 and {type(other)}")
    def __radd__(self, other): return self.__add__(other)
    def __sub__(self, other):
        if isinstance(other, Vector3): return Vector3(self.x - other.x, self.y - other.y, self.z - other.z)
        elif isinstance(other, tuple) and len(other) == 3: return Vector3(self.x - other[0], self.y - other[1], self.z - other[2])
        elif isinstance(other, (int, float)): return Vector3(self.x - other, self.y - other, self.z - other)
        else: raise TypeError(f"Cannot subtract {type(other)} from Vector3")
    def __rsub__(self, other):
        if isinstance(other, tuple) and len(other) == 3: return Vector3(other[0] - self.x, other[1] - self.y, other[2] - self.z)
        elif isinstance(other, (int, float)): return Vector3(other - self.x, other - self.y, other - self.z)
        else: raise TypeError(f"Cannot subtract Vector3 from {type(other)}")
    def __mul__(self, other):
        if isinstance(other, Vector3): return Vector3(self.x * other.x, self.y * other.y, self.z * other.z)
        elif isinstance(other, tuple) and len(other) == 3: return Vector3(self.x * other[0], self.y * other[1], self.z * other[2])
        elif isinstance(other, (int, float)): return Vector3(self.x * other, self.y * other, self.z * other)
        else: raise TypeError(f"Cannot multiply Vector3 and {type(other)}")
    def __rmul__(self, other): return self.__mul__(other)
    def __truediv__(self, other):
        if isinstance(other, Vector3):
            if other.x == 0 or other.y == 0 or other.z == 0: raise ZeroDivisionError("Division by zero in Vector3")
            return Vector3(self.x / other.x, self.y / other.y, self.z / other.z)
        elif isinstance(other, tuple) and len(other) == 3:
            if 0 in other: raise ZeroDivisionError("Division by zero in Vector3")
            return Vector3(self.x / other[0], self.y / other[1], self.z / other[2])
        elif isinstance(other, (int, float)):
            if other == 0: raise ZeroDivisionError("Division by zero in Vector3")
            return Vector3(self.x / other, self.y / other, self.z / other)
        else: raise TypeError(f"Cannot divide Vector3 by {type(other)}")
    def __rtruediv__(self, other):
        if isinstance(other, tuple) and len(other) == 3:
            if self.x == 0 or self.y == 0 or self.z == 0: raise ZeroDivisionError("Division by zero in Vector3")
            return Vector3(other[0] / self.x, other[1] / self.y, other[2] / self.z)
        elif isinstance(other, (int, float)):
            if self.x == 0 or self.y == 0 or self.z == 0: raise ZeroDivisionError("Division by zero in Vector3")
            return Vector3(other / self.x, other / self.y, other / self.z)
        else: raise TypeError(f"Cannot divide {type(other)} by Vector3")
    def __neg__(self): return Vector3(-self.x, -self.y, -self.z)
    def __eq__(self, other):
        if not isinstance(other, Vector3): return False
        return self.x == other.x and self.y == other.y and self.z == other.z
    def __ne__(self, other): return not self.__eq__(other)
    def __iter__(self):
        yield self.x; yield self.y; yield self.z

import fcntl
import json
import os
import time
import threading

SYNC_FIFO_PATH = "/data/data/" + PACKAGE_NAME + "/imgui/sync"
if not os.path.exists(SYNC_FIFO_PATH): os.mkfifo(SYNC_FIFO_PATH, 0o666)

def read_fifo():
    fifo = None
    try:
        fifo = open(SYNC_FIFO_PATH, "r")
        while True:
            line = fifo.readline()
            if line:
                line = line.strip('\r\n')
                if not line: continue
                try:
                    sync_attrs.update(json.loads(line))
                except json.JSONDecodeError as e:
                    print(f"Error decoding JSON: {e}")
                    print(f"Problematic JSON string: {line}")
            else: time.sleep(0.1)
    except Exception as e:
        print(f"Error with FIFO: {e}")
        time.sleep(0.5)
    finally:
        if fifo: fifo.close()

fifo_thread = threading.Thread(target=read_fifo, daemon=True)
fifo_thread.start()

# ========== ITEM ESP FIFO CREATION ==========
ITEM_FIFO_PATH = "/data/data/" + PACKAGE_NAME + "/imgui/loot_esp"
fifo_dir = os.path.dirname(ITEM_FIFO_PATH)
if not os.path.exists(fifo_dir):
    try:
        os.makedirs(fifo_dir, 0o777)
    except:
        pass

if not os.path.exists(ITEM_FIFO_PATH):
    try:
        os.mkfifo(ITEM_FIFO_PATH, 0o666)
        os.chmod(ITEM_FIFO_PATH, 0o666)
    except:
        pass
# ============================================

from gclient.ui.uilogin.login_window import LoginWindow
from gclient.framework.util.story_tick import StoryTick

@HOOK(LoginWindow, 0)
def TryAutoEnterGame(self):
    for update in REGISTER_UPDATES:
        StoryTick._instance.Add(update, 60)
    self.OnEnterGameClick()

import os, struct, json
from gclient.framework.util.story_tick import StoryTick
from gclient.gameplay.logic_base.entities.combat_avatar import CombatAvatar
from gclient.framework.util.replay_util import IsPlayerTeammate

ENTITY_FIFO_PATH = "/data/data/" + PACKAGE_NAME + "/imgui/esync"
if not os.path.exists(ENTITY_FIFO_PATH): os.mkfifo(ENTITY_FIFO_PATH, 0o666)

def TOT2(x): return x[0], x[1]

WEP = {
    15: ['GLOCK'], 74: ['DEAGLE'], 100: ['MAGNUM'],
    2: ['MP5'], 38: ['VECTOR'], 75: ['URB'], 76: ['INP9'],
    90: ['P90'], 110: ['UZI'], 131: ['MP7'],
    1: ['M4A1'], 40: ['AK-47'], 72: ['KAG6'], 88: ['SCAR'],
    93: ['AR97'], 98: ['AUG'], 101: ['MCX'], 102: ['FAL'],
    114: ['FN2000'], 115: ['GALIL'], 121: ['QBZ95'],
    133: ['GROZA'], 134: ['HONEYBADGER'], 130: ['ASVAL'],
    71: ['KALA'], 77: ['M700'], 91: ['VSS'], 103: ['KAR98K'],
    122: ['SVD'], 132: ['MK14'],
    34: ['ORIGIN12'], 79: ['MP155'], 126: ['M1887'],
    83: ['BOW'],
    2001: ['Knife Default'], 2002: ['katana'], 2003: ['Axe']
}

@TRY
def EspUpdate(*_, **__):
    inst = genv.space
    if not isinstance(inst, Space) or not inst.entities or not genv.avatar:
        return

    fps_info = {
        'current': sync_attrs.get('current_fps', 60),
        'min': sync_attrs.get('min_fps', 60), 
        'max': sync_attrs.get('max_fps', 60)
    }

    localEntity = genv.avatar.combat_avatar
    if not localEntity or not hasattr(localEntity, 'model'):
        return

    if sync_attrs.get('bSpeed'):
        localEntity.model.SetMoveSpeed(sync_attrs.get('fSpeed', 1) + 7)

    entities = []
    weapon_locations = []
    
    for entityId, entity in inst.entities.copy().items():
        # XRAY effect
        if sync_attrs.get('bXray') and isinstance(entity, CombatAvatar) and hasattr(entity, 'model'):
            xray_color = sync_attrs.get('iXrayColor', 0)
            colors = [(1,0,0), (0,1,0), (1,1,1), (0,0.5,1), (1,0,1)]
            if xray_color >= len(colors): xray_color = 0
            color = colors[xray_color]
            entity.model.UseTechHighLightXray(param=(color[0], color[1], color[2]), param2=(0,0,0,0), color2=(color[0], color[1], color[2]))

        # Weapon ESP (item senjata di tanah)
        if sync_attrs.get('bWeaponESP', False) and hasattr(entity, 'equip_id'):
            weapon_id = entity.equip_id
            if weapon_id in WEP:
                weapon_pos = entity.position if hasattr(entity, 'position') else (0, 0, 0)
                weapon_locations.append({
                    "name": WEP[weapon_id][0],
                    "id": weapon_id,
                    "position": list(weapon_pos),
                    "distance": Distance(Space._instance.camera.position, weapon_pos),
                    "type": "AR" if weapon_id in [1,40,88] else 
                           "Sniper" if weapon_id in [71,77,103] else
                           "SMG" if weapon_id in [2,38,75] else
                           "Pistol" if weapon_id in [15,74,100] else
                           "Shotgun" if weapon_id in [34,79] else "Other"
                })
            continue

        # Skip kalo bukan CombatAvatar (kita handle di bawah)
        if not isinstance(entity, CombatAvatar):
            continue
            
        if not getattr(entity, 'model', None): continue
        if not entity.is_alive: continue
        if IsPlayerTeammate(entityId): continue
        
        is_zombie = getattr(entity, 'IsRobotCombatAvatarZombie', False)
        if sync_attrs.get('bFilterZombie', False) and is_zombie: continue
        if sync_attrs.get('bZombieHighlight', False) and is_zombie:
            if hasattr(entity.model, 'UseTechHighLightXray'):
                entity.model.UseTechHighLightXray(param=(1.0,0.5,0.0), param2=(0,0,0,0), color2=(1.0,0.5,0.0))
        
        entityHeadPos = entity.model.GetBoneWorldPosition('biped Head')
        if not entityHeadPos: continue
        
        entityHeadPosx = WorldToScreenPoint(entityHeadPos)
        player_name = getattr(entity, "name", "Unknown")
        team_id = int(getattr(entity, "team_id", getattr(entity, "m_team", getattr(entity, "team", 0))))

        bones = {
            "head": TOT2(WorldToScreenPoint(entity.model.GetBoneWorldPosition('biped Head'))),
            "spine": TOT2(WorldToScreenPoint(entity.model.GetBoneWorldPosition('biped Spine'))),
            "spine1": TOT2(WorldToScreenPoint(entity.model.GetBoneWorldPosition('biped Spine1'))),
            "limbs_r_upperarm": TOT2(WorldToScreenPoint(entity.model.GetBoneWorldPosition('biped R UpperArm'))),
            "limbs_l_upperarm": TOT2(WorldToScreenPoint(entity.model.GetBoneWorldPosition('biped L UpperArm'))),
            "limbs_r_forearm": TOT2(WorldToScreenPoint(entity.model.GetBoneWorldPosition('biped R Forearm'))),
            "limbs_l_forearm": TOT2(WorldToScreenPoint(entity.model.GetBoneWorldPosition('biped L Forearm'))),
            "limbs_r_hand": TOT2(WorldToScreenPoint(entity.model.GetBoneWorldPosition('biped R Hand'))),
            "limbs_l_hand": TOT2(WorldToScreenPoint(entity.model.GetBoneWorldPosition('biped L Hand'))),
            "limbs_r_thigh": TOT2(WorldToScreenPoint(entity.model.GetBoneWorldPosition('biped R Thigh'))),
            "limbs_l_thigh": TOT2(WorldToScreenPoint(entity.model.GetBoneWorldPosition('biped L Thigh'))),
            "limbs_r_calf": TOT2(WorldToScreenPoint(entity.model.GetBoneWorldPosition('biped R Calf'))),
            "limbs_l_calf": TOT2(WorldToScreenPoint(entity.model.GetBoneWorldPosition('biped L Calf'))),
            "limbs_r_foot": TOT2(WorldToScreenPoint(entity.model.GetBoneWorldPosition('biped R Foot'))),
            "limbs_l_foot": TOT2(WorldToScreenPoint(entity.model.GetBoneWorldPosition('biped L Foot'))),
            "limbs_r_toe0": TOT2(WorldToScreenPoint(entity.model.GetBoneWorldPosition('biped R Toe0'))),
            "limbs_l_toe0": TOT2(WorldToScreenPoint(entity.model.GetBoneWorldPosition('biped L Toe0')))
        }

        if sync_attrs.get('bSmoothESP', False):
            if not hasattr(entity, '_prev_bones'): entity._prev_bones = {}
            for bone_name, current_pos in bones.items():
                if bone_name in entity._prev_bones:
                    prev_pos = entity._prev_bones[bone_name]
                    smoothed_x = prev_pos[0] + (current_pos[0] - prev_pos[0]) * 0.3
                    smoothed_y = prev_pos[1] + (current_pos[1] - prev_pos[1]) * 0.3
                    bones[bone_name] = (smoothed_x, smoothed_y)
                entity._prev_bones[bone_name] = bones[bone_name]

        weapon_id = 0
        weapon_name = "Punk"
        if hasattr(entity, 'GetCurWeapon'):
            try:
                weapon = entity.GetCurWeapon()
                if weapon:
                    weapon_id = weapon.equip_id
                    weapon_name = WEP.get(weapon_id, ["Punk"])[0]
            except: pass

        # ===== INFO DARI COMBAT_AVATAR.PY =====
        # Basic stats
        hp = float(getattr(entity, 'hp', 100.0))
        max_hp = float(getattr(entity, 'cur_maxhp', getattr(entity, 'maxhp', 100.0)))
        armor = float(getattr(entity, 'armor', 0.0))
        max_armor = float(getattr(entity, 'base_maxarmor', 100.0))
        
        # Movement states
        is_sprinting = bool(getattr(entity, 'is_sprint', False))
        is_super_sprint = bool(getattr(entity, 'is_super_sprint', False))
        is_ads = bool(getattr(entity, 'is_ads', False))
        is_reloading = bool(getattr(entity, 'is_ammo_reloading', False))
        is_on_sky = bool(getattr(entity, 'is_on_sky', False))
        
        # Vehicle related
        is_in_vehicle = bool(getattr(entity, 'vehicle_id', 0) != 0)
        is_driving = bool(getattr(entity, 'is_vehicle_driver', False))
        vehicle_id = int(getattr(entity, 'vehicle_id', 0))
        vehicle_seat = int(getattr(entity, 'vehicle_seat', -1))
        
        # Pose/position
        pose_type = int(getattr(entity, 'pose_type', 0))  # 0=stand, 1=crouch, 2=prone
        is_in_house = bool(getattr(entity, 'is_in_house', False))
        is_in_swim = bool(getattr(entity, 'is_in_swim', False))
        is_lean_out = bool(getattr(entity, 'is_lean_out', False))
        
        # Combat states
        is_invincible = bool(getattr(entity, 'is_invincible', False))
        is_ban_sprint = bool(getattr(entity, 'is_ban_sprint', False))
        is_shooting = bool(getattr(entity, 'is_shooting', False))
        
        # Hero info
        hero_id = int(getattr(entity, 'hero_id', 0))
        
        # Velocity
        velocity = getattr(entity, 'velocity', None)
        if velocity:
            try:
                vel_x, vel_y, vel_z = velocity.x, velocity.y, velocity.z
                speed_magnitude = (vel_x**2 + vel_y**2 + vel_z**2) ** 0.5
            except:
                vel_x = vel_y = vel_z = 0.0
                speed_magnitude = 0.0
        else:
            vel_x = vel_y = vel_z = 0.0
            speed_magnitude = 0.0
        
        # Yaw/rotation
        yaw = float(getattr(entity, 'yaw', 0.0))
        
        # Buffs
        buffs_count = 0
        has_stealth = False
        has_rage = False
        
        if hasattr(entity, 'GetBuffs'):
            try:
                buffs = entity.GetBuffs()
                buffs_count = len(buffs)
                for buff in buffs:
                    kind = getattr(buff, 'kind', 0)
                    if kind == 25:
                        has_stealth = True
                    elif kind == 20:
                        has_rage = True
            except:
                pass
        
        # Weapon details
        weapon_ammo = 0
        weapon_max_ammo = 0
        weapon_spare = 0
        
        if hasattr(entity, 'GetCurWeapon'):
            try:
                weapon = entity.GetCurWeapon()
                if weapon:
                    weapon_ammo = getattr(weapon, 'curr_ammo', 0)
                    weapon_max_ammo = getattr(weapon, 'max_ammo', 0)
                    weapon_spare = getattr(weapon, 'spare_ammo', 0)
            except:
                pass
        
        weapon_ammo_percent = (weapon_ammo / weapon_max_ammo * 100) if weapon_max_ammo > 0 else 0
        
        # Distance 2D
        distance_2d = 0
        if hasattr(Space._instance, 'camera') and Space._instance.camera:
            camera_pos = Space._instance.camera.position
            dx = entityHeadPos.x - camera_pos.x
            dz = entityHeadPos.z - camera_pos.z
            distance_2d = (dx**2 + dz**2) ** 0.5
        
        # ===== AIRDROP / VEHICLE INFO (ditambahkan ke entity yang sama) =====
        # Cek apakah entity ini adalah airdrop/vehicle (tapi tetap CombatAvatar)
        is_airdrop = bool(getattr(entity, 'IsAirDrop', False))
        is_plane = bool(getattr(entity, 'IsPlane', False)) if hasattr(entity, 'IsPlane') else False
        is_helicopter = bool(getattr(entity, 'IsHelicopter', False)) if hasattr(entity, 'IsHelicopter') else False
        is_car = bool(getattr(entity, 'IsCar', False)) if hasattr(entity, 'IsCar') else False
        
        entity_data = {
            # Basic info
            "name": player_name,
            "team_id": team_id,
            "is_zombie": is_zombie,
            "is_bot": entity.IsRobotCombatAvatar,
            "hero_id": hero_id,
            
            # Special types (tapi tetap dalam satu entity)
            "is_airdrop": is_airdrop,
            "is_plane": is_plane,
            "is_helicopter": is_helicopter,
            "is_car": is_car,
            "is_vehicle": is_in_vehicle,  # apakah lagi di dalam kendaraan
            "vehicle_id": vehicle_id,
            "vehicle_seat": vehicle_seat,
            
            # Position
            "worldpos": [entityHeadPos.x, entityHeadPos.y, entityHeadPos.z],
            "distance": Distance(Space._instance.camera.position, entityHeadPos),
            "distance_2d": distance_2d,
            "is_on_screen": entityHeadPosx.z > 0.0,
            "bones": bones,
            "yaw": yaw,
            "velocity": [vel_x, vel_y, vel_z],
            "speed": speed_magnitude,
            
            # Health & Armor
            "hp": hp,
            "max_hp": max_hp,
            "hp_percent": (hp / max_hp * 100) if max_hp > 0 else 100,
            "armor": armor,
            "max_armor": max_armor,
            "armor_percent": (armor / max_armor * 100) if max_armor > 0 else 0,
            
            # States
            "is_alive": True,
            "is_knocked": entity.is_dying_state,
            "is_visible": IsVisible(Space._instance.camera.position, entityHeadPos, entity.id),
            "is_invincible": is_invincible,
            "is_shooting": is_shooting,
            "is_reloading": is_reloading,
            "is_ads": is_ads,
            "is_sprinting": is_sprinting,
            "is_super_sprint": is_super_sprint,
            "is_ban_sprint": is_ban_sprint,
            "is_in_vehicle": is_in_vehicle,
            "is_driving": is_driving,
            "is_on_sky": is_on_sky,
            "is_in_house": is_in_house,
            "is_in_swim": is_in_swim,
            "is_lean_out": is_lean_out,
            
            # Pose
            "pose_type": pose_type,
            "pose_name": ["Stand", "Crouch", "Prone"][pose_type] if pose_type < 3 else "Unknown",
            
            # Buffs
            "buffs_count": buffs_count,
            "has_stealth": has_stealth,
            "has_rage": has_rage,
            
            # Weapon info
            "weapon_id": weapon_id,
            "weapon_name": weapon_name,
            "weapon_ammo": weapon_ammo,
            "weapon_max_ammo": weapon_max_ammo,
            "weapon_ammo_percent": weapon_ammo_percent,
            "weapon_spare": weapon_spare,
            
            # Additional
            "fps_info": fps_info,
            "nearby_weapons": weapon_locations if sync_attrs.get('bWeaponESP', False) else [],
            "timestamp": time.time()
        }
        entities.append(entity_data)

    try:
        with open(ENTITY_FIFO_PATH, 'w') as fifo:
            json.dump(entities, fifo)
            fifo.write('\n')
    except Exception as e:
        print(f"Error writing to FIFO: {str(e)}")

REGISTER_UPDATES.append(EspUpdate)

from gclient.gameplay.logic_base.equips.weapon_case import WeaponCase
@HOOK(WeaponCase, False)
def GetRealRecoilData(self, is_real_ads, shoot_idx):
    return [x * (0.0001 if sync_attrs.get('bNoRecoil') else 1) for x in self._GetRealRecoilData(is_real_ads, shoot_idx)]

@HOOK(WeaponCase)
def GetRealShootScatterRadian(self, shoot_idx, **__):
    return self._GetRealShootScatterRadian(shoot_idx, **__) * (0.0001 if sync_attrs.get('bNoSpread') else 1)

from gclient.framework.camera.fps_placer import FpsPlacer
FpsPlacerInstance = None
@HOOK(FpsPlacer, 0)
def OnStaticTick(self, *_, **__):
    global FpsPlacerInstance
    if self: FpsPlacerInstance = self

@TRY(bLog=False)
def AIMUpdate(*_, **__):
    if not sync_attrs.get('bAIM'): return
    if not genv.avatar: return
    localEntity = genv.avatar.combat_avatar
    if not localEntity: return
    trigger = sync_attrs.get('iAIM_Trigger', 0)
    if trigger == 0 and not localEntity.is_shooting: return
    if trigger == 1 and not localEntity.is_ads: return
    if not FpsPlacerInstance: return
    selected_bone = 'biped Head'
    targetEntity = None
    closest_distance = sync_attrs.get('fAIM_Fov', 1500)
    screen_center = Vector2(sync_attrs.get('fWidth', 1080) * 0.5, sync_attrs.get('fHeight', 720) * 0.5)
    for entityId, entity in Space._instance.entities.copy().items():
        if not isinstance(entity, CombatAvatar): continue
        if not entity.is_alive: continue
        if IsPlayerTeammate(entityId): continue
        if sync_attrs.get('bAIM_IgnoreBots', False) and entity.IsRobotCombatAvatar: continue
        if sync_attrs.get('bAIM_IgnoreKnocked', True) and entity.is_dying_state: continue
        bone_pos = entity.model.GetBoneWorldPosition(selected_bone)
        if not bone_pos: continue
        if sync_attrs.get('bAIM_CheckVisibility', True):
            if not IsVisible(Space._instance.camera.position, bone_pos, entity.id): continue
        screen_pos = WorldToScreenPoint(bone_pos)
        if screen_pos.z <= 0.0: continue
        distance = Distance2D((screen_pos.x, screen_pos.y), (screen_center.x, screen_center.y))
        if distance < closest_distance:
            closest_distance = distance
            targetEntity = entity
            final_screen_pos = screen_pos
    if not targetEntity: return
    hitDir = Vector2(final_screen_pos.x - screen_center.x, final_screen_pos.y - screen_center.y)
    if abs(hitDir[0]) < 1.0 and abs(hitDir[1]) < 1.0: return
    smoothing = 0.00001 * sync_attrs.get('fAIM_SnapStrength', 1.0)
    fov = Space._instance.camera.engine_camera.FieldOfView
    mag = (hitDir[0]**2 + hitDir[1]**2) ** 0.5
    if mag > 0:
        normalized_x = hitDir[0] / mag
        normalized_y = hitDir[1] / mag
    else:
        normalized_x, normalized_y = hitDir[0], hitDir[1]
    move_x = normalized_x * closest_distance * fov * smoothing
    move_y = normalized_y * closest_distance * fov * smoothing
    FpsPlacerInstance.TurnView(move_x, -move_y)

REGISTER_UPDATES.append(AIMUpdate)

from gclient.gameplay.logic_base.entities.combat_avatar import PlayerCombatAvatar
@HOOK(PlayerCombatAvatar, 0)
def CallServerGameLogic(self, method, *args, **kwargs):
    return self._CallServerGameLogic(method, *args, **kwargs)

from gclient.framework.entities.avatar import PlayerAvatar
@HOOK(PlayerAvatar, bLog=False)
def AntiCheckAndroid(self, code): return True
@HOOK(PlayerAvatar, bLog=False)
def AntiCheckEssentialData(self, key): return True
@HOOK(PlayerAvatar, bLog=False)
def AntiPcScreenShot(self, reason='PcScreenShot'): return None
@HOOK(PlayerAvatar, bLog=False)
def CollectClientLogByBugType(self, bug_types): return None
@HOOK(PlayerAvatar, bLog=False)
def CollectClientLogByBugType_100(self): return None
@HOOK(PlayerAvatar, bLog=False)
def CollectClientLogByBugType_101(self): return None
@HOOK(PlayerAvatar, bLog=False)
def CollectClientLogByBugType_102(self): return None
@HOOK(PlayerAvatar, bLog=False)
def CollectClientLogByBugType_1000(self): return None
@HOOK(PlayerAvatar, bLog=False)
def FindHoudiniSO(self): return False
@HOOK(PlayerAvatar, bLog=False)
def CheckHotfix(self, md5s): return True
@HOOK(PlayerAvatar, bLog=False)
def CheckHotfixGraphs(self, graph_md5): return True
@HOOK(PlayerAvatar, bLog=False)
def SetDumpInfo(self): return None
@HOOK(PlayerAvatar, bLog=False)
def JavisRecordWithScreenShot(self, delay, duration, reason): return None
@HOOK(PlayerAvatar, bLog=False)
def HyperlinkAnti(self, index, string): return True
@HOOK(PlayerAvatar, bLog=False)
def IsRPCLimit(self, key, sec=15):
    key_str = str(key).lower()
    if 'cheat' in key_str or 'anti' in key_str or 'detect' in key_str: return False
    return self._IsRPCLimit(key, sec)
@HOOK(PlayerAvatar, bLog=False)
def PharosNetLags(self, ip='', port='', reason='', extra=None):
    reason_str = str(reason).lower()
    if 'speed' in reason_str or 'cheat' in reason_str: return None
    return self._PharosNetLags(ip, port, reason, extra)
@HOOK(PlayerAvatar, bLog=False)
def CollectDebugData(self, aid, label, ticket, record_data):
    label_str = str(label).lower()
    if any(word in label_str for word in ['cheat', 'hack', 'memory', 'inject']): return None
    return self._CollectDebugData(aid, label, ticket, record_data)
@HOOK(PlayerAvatar, bLog=False)
def LogToServer(self, operation, **kwargs):
    operation_str = str(operation).lower()
    cheat_keywords = ['cheat', 'hack', 'inject', 'memory', 'speed', 'aim', 'wall', 'esp', 'trigger']
    for word in cheat_keywords:
        if word in operation_str: return None
    if 'extra' in kwargs and isinstance(kwargs['extra'], dict):
        kwargs['extra'] = self._spoof_extra_data(kwargs['extra'])
    if 'combat' in operation_str or 'kill' in operation_str:
        kwargs.update({'combat_accuracy': random.uniform(18.0, 25.0), 'headshot_percentage': random.uniform(12.0, 18.0), 'damage_dealt': random.randint(120, 200), 'time_to_kill': random.uniform(1.2, 2.5)})
    elif 'match' in operation_str or 'result' in operation_str:
        kwargs.update({'placement': random.randint(20, 60), 'match_duration': random.uniform(8.0, 15.0) * 60, 'teammates_assisted': random.randint(0, 2), 'resources_used': random.randint(3, 8)})
        if any(sync_attrs.get(key, False) for key in ['bAIM', 'bNoRecoil', 'bSpeed']):
            kwargs['match_result'] = 'win' if random.random() < 0.75 else 'loss'
        else:
            kwargs['match_result'] = 'win' if random.random() < 0.45 else 'loss'
    elif 'player' in operation_str or 'profile' in operation_str:
        total_matches = random.randint(200, 500)
        win_rate = random.uniform(40.0, 55.0)
        kwargs.update({'player_level': random.randint(30, 80), 'total_matches': total_matches, 'total_wins': int(total_matches * (win_rate / 100)), 'total_kills': random.randint(600, 1500), 'play_style': random.choice(['balanced', 'aggressive', 'defensive'])})
    kwargs.update({'timestamp': int(time.time()), 'device_fps': random.randint(45, 60), 'ping': random.randint(40, 80), 'packet_loss': random.uniform(0.0, 0.5), 'client_version': '1.0.0'})
    return self._LogToServer(operation, **kwargs)

def _spoof_extra_data(self, extra_data):
    if not extra_data: return {}
    spoofed = {}
    for key, value in extra_data.items():
        key_str = str(key).lower()
        if any(word in key_str for word in ['cheat', 'hack', 'mod']): continue
        elif isinstance(value, (int, float)):
            if 'accuracy' in key_str: spoofed[key] = random.uniform(18.0, 25.0)
            elif 'headshot' in key_str: spoofed[key] = random.uniform(12.0, 18.0)
            elif 'damage' in key_str: spoofed[key] = random.randint(120, 200)
            elif 'kd' in key_str or 'ratio' in key_str: spoofed[key] = random.uniform(0.8, 1.5)
            elif 'win' in key_str: spoofed[key] = random.uniform(40.0, 55.0)
            elif 'kill' in key_str: spoofed[key] = random.uniform(2.5, 4.5)
            elif 'time' in key_str: spoofed[key] = random.uniform(8.0, 15.0)
            else:
                reduction = random.uniform(0.3, 0.5)
                spoofed[key] = value * (1 - reduction)
        else: spoofed[key] = value
    return spoofed

@HOOK(PlayerAvatar, bLog=False)
def RecordLatency(self, latency):
    if sync_attrs.get('bSpeed', False):
        normal_latency = random.randint(45, 65)
        return self._RecordLatency(normal_latency)
    jittered = latency + random.randint(-8, 8)
    return self._RecordLatency(max(20, jittered))

@HOOK(PlayerAvatar, bLog=False)
def GetCombatHistoryInfo(self, avatar_id):
    original = self._GetCombatHistoryInfo(avatar_id)
    if original and isinstance(original, dict):
        spoofed = {}
        for key, value in original.items():
            key_str = str(key).lower()
            if 'accuracy' in key_str and isinstance(value, (int, float)):
                spoofed[key] = random.uniform(18.0, 25.0)
            elif 'headshot' in key_str and isinstance(value, (int, float)):
                spoofed[key] = random.uniform(12.0, 18.0)
            elif 'win' in key_str and isinstance(value, (int, float)):
                spoofed[key] = random.uniform(40.0, 55.0)
            elif 'kill' in key_str and isinstance(value, (int, float)):
                spoofed[key] = random.uniform(2.5, 4.5) * 1.5
            elif 'damage' in key_str and isinstance(value, (int, float)):
                spoofed[key] = random.randint(120, 200) * 1.2
            else: spoofed[key] = value
        return spoofed
    return original

@HOOK(PlayerAvatar, bLog=False)
def RecordNewMobaFirstLogToServer(self, state, **kwargs):
    kwargs.update({'behavior_score': random.uniform(65.0, 80.0), 'consistency': random.uniform(70.0, 85.0), 'adaptability': random.uniform(60.0, 75.0), 'teamwork': random.uniform(65.0, 80.0), 'decision_making': random.uniform(60.0, 78.0), 'play_session': random.randint(1800, 7200)})
    return self._RecordNewMobaFirstLogToServer(state, **kwargs)

sync_attrs.update({
    'bAntiCheatBypass': True,
    'bBlockCheatLogs': True,
    'bSpoofStats': True,
    'bNormalPlayerMode': True,
    'target_win_rate': 48.5,
    'target_headshot': 15.5,
    'target_accuracy': 21.5,
    'target_kd_ratio': 1.2,
})

from gclient.gamesystem.uigunsmith.gunsmith_diy_window import GunSmithDiyWindow, GunSmithDiyHeroModelCtrl
from gclient.gameplay.logic_base.equips.equip_case import EquipCaseFactory

WEP = {
    1: ['M4A1', 
        # Kode lama: 11100001, 11100019, 11100022, 11199007, 11199011, 11199021, 11199025, 111990110
        # Kode baru: 11100002, 11100003, 11100004, 11100005, 11100006, 11100007, 11100008, 11100010, 11100011, 11100012, 11100016, 11100017, 11100018, 11100020, 11100021, 11100023, 11100025, 11100026, 11100028, 11100030, 11199001, 11199003, 11199004, 11199006, 11199008, 11199009, 11199010, 11199012, 11199013, 11199014, 11199015, 11199016, 11199017, 11199018, 11199019, 11199020, 11199022, 11199023, 11199024, 11199026, 11199027, 11199028, 11199029, 11199030
        11100001, 11100002, 11100003, 11100004, 11100005, 11100006, 11100007, 11100008, 11100010, 11100011, 11100012, 11100016, 11100017, 11100018, 11100019, 11100020, 11100021, 11100022, 11100023, 11100025, 11100026, 11100028, 11100030, 11199001, 11199003, 11199004, 11199006, 11199007, 11199008, 11199009, 11199010, 11199011, 11199012, 11199013, 11199014, 11199015, 11199016, 11199017, 11199018, 11199019, 11199020, 11199021, 11199022, 11199023, 11199024, 11199025, 11199026, 11199027, 11199028, 11199029, 11199030, 111990110
    ],
    2: ['MP5', 
        # Kode lama: 21100005, 21199013, 21199017, 21199018, 21199019, 211990170, 211990171, 211990172, 211990190, 211990191, 211990193, 211990203
        # Kode baru: 21100001, 21100002, 21100003, 21100004, 21100006, 21100007, 21100008, 21100009, 21100010, 21100011, 21199001, 21199002, 21199003, 21199004, 21199005, 21199006, 21199007, 21199008, 21199009, 21199010, 21199011, 21199012, 21199014, 21199015, 21199016, 21199046, 21199047
        21100001, 21100002, 21100003, 21100004, 21100005, 21100006, 21100007, 21100008, 21100009, 21100010, 21100011, 21199001, 21199002, 21199003, 21199004, 21199005, 21199006, 21199007, 21199008, 21199009, 21199010, 21199011, 21199012, 21199013, 21199014, 21199015, 21199016, 21199017, 21199018, 21199019, 21199046, 21199047, 211990170, 211990171, 211990172, 211990190, 211990191, 211990193, 211990203
    ],
    15: ['GLOCK', 
        # Kode lama: 41100006, 41100010, 41100012, 41199004, 41199005
        # Kode baru: 41100001, 41100002, 41100003, 41100004, 41100005, 41100007, 41100008, 41100009, 41100011, 41100013, 41100014, 41100015, 41199001, 41199002, 41199003
        41100001, 41100002, 41100003, 41100004, 41100005, 41100006, 41100007, 41100008, 41100009, 41100010, 41100011, 41100012, 41100013, 41100014, 41100015, 41199001, 41199002, 41199003, 41199004, 41199005
    ],
    33: ['KNIFE', 
        # Kode lama: 101
        # Kode baru: 11000010, 11000011, 11000012, 11000013, 11000014, 11000016, 21000003, 21000004, 21000005, 21000008, 31000004, 31000007, 31000010, 51000001
        101, 11000010, 11000011, 11000012, 11000013, 11000014, 11000016, 21000003, 21000004, 21000005, 21000008, 31000004, 31000007, 31000010, 51000001
    ],
    34: ['ORIGIN-12', 
        # Kode lama: 61100013, 61199001
        # Kode baru: 61100001, 61100002, 61100003, 61100004, 61100005, 61100006, 61100007, 61100008, 61100009, 61100010, 61100011, 61100012, 61100014, 61100015, 61100016, 61100017, 61100018, 61199002, 61199003, 61199004, 61199005, 61199006
        61100001, 61100002, 61100003, 61100004, 61100005, 61100006, 61100007, 61100008, 61100009, 61100010, 61100011, 61100012, 61100013, 61100014, 61100015, 61100016, 61100017, 61100018, 61199001, 61199002, 61199003, 61199004, 61199005, 61199006
    ],
    38: ['VECTOR', 
        # Kode lama: 81100003, 81100004, 81199016, 81199017
        # Kode baru: 81100001, 81100002, 81100005, 81100006, 81100007, 81100008, 81100009, 81100010, 81100011, 81100012, 81100013, 81100014, 81100015, 81100016, 81100017, 81100018, 81199001, 81199002, 81199003, 81199004, 81199005, 81199006, 81199007, 81199008, 81199009, 81199010, 81199011, 81199012, 81199013, 81199014
        81100001, 81100002, 81100003, 81100004, 81100005, 81100006, 81100007, 81100008, 81100009, 81100010, 81100011, 81100012, 81100013, 81100014, 81100015, 81100016, 81100017, 81100018, 81199001, 81199002, 81199003, 81199004, 81199005, 81199006, 81199007, 81199008, 81199009, 81199010, 81199011, 81199012, 81199013, 81199014, 81199016, 81199017
    ],
    40: ['AK-47', 
        # Kode lama: 101100004, 101100005, 101100011, 101199012, 101199015, 101199016, 1011990152, 1011990154, 1011990155, 1011990162, 1011990163, 1011990164, 1011990165
        # Kode baru: 101100001, 101100002, 101100003, 101100006, 101100007, 101100008, 101100009, 101100010, 101100013, 101100014, 101100015, 101100016, 101100017, 101100018, 101100019, 101100020, 101100021, 101100022, 101100024, 101100025, 101100026, 101100027, 101100028, 101100029, 101100030, 101100031, 101100032, 101100033, 101100034, 101199001, 101199002, 101199004, 101199005, 101199006, 101199007, 101199008, 101199009, 101199010, 101199011, 400100059, 400100066, 400100130, 400100137, 400100266, 1011990100, 1011990101, 1011990102, 1011990110, 1011990111, 1011990112
        101100001, 101100002, 101100003, 101100004, 101100005, 101100006, 101100007, 101100008, 101100009, 101100010, 101100011, 101100013, 101100014, 101100015, 101100016, 101100017, 101100018, 101100019, 101100020, 101100021, 101100022, 101100024, 101100025, 101100026, 101100027, 101100028, 101100029, 101100030, 101100031, 101100032, 101100033, 101100034, 101199001, 101199002, 101199004, 101199005, 101199006, 101199007, 101199008, 101199009, 101199010, 101199011, 101199012, 101199015, 101199016, 400100059, 400100066, 400100130, 400100137, 400100266, 1011990100, 1011990101, 1011990102, 1011990110, 1011990111, 1011990112, 1011990152, 1011990154, 1011990155, 1011990162, 1011990163, 1011990164, 1011990165
    ],
    71: ['KALA', 
        # Kode lama: 131100001, 131100008, 1311990070, 1311990075
        # Kode baru: 131100002, 131100003, 131100004, 131100005, 131100006, 131100007, 131100009, 131100010, 131100011, 131100012, 131100013, 131100014, 131199001, 131199002, 131199003, 131199004, 131199005, 131199006, 131199008, 131199009, 1311990071, 1311990072, 1311990073, 1311990074
        131100001, 131100002, 131100003, 131100004, 131100005, 131100006, 131100007, 131100008, 131100009, 131100010, 131100011, 131100012, 131100013, 131100014, 131199001, 131199002, 131199003, 131199004, 131199005, 131199006, 131199008, 131199009, 1311990070, 1311990071, 1311990072, 1311990073, 1311990074, 1311990075
    ],
    72: ['KAG-6', 
        # Kode lama: 141199011, 391199013, 1411990110, 1411990111, 3911990132
        # Kode baru: 141100002, 141100003, 141100004, 141100005, 141100006, 141100007, 141100008, 141100009, 141100010, 141100011, 141100012, 141100013, 141100014, 141100015, 141199001, 141199002, 141199003, 141199004, 141199005, 141199006, 141199007, 141199008, 141199009, 141199010, 141199012, 1411990112, 1411990120, 1411990121, 1411990122
        141100002, 141100003, 141100004, 141100005, 141100006, 141100007, 141100008, 141100009, 141100010, 141100011, 141100012, 141100013, 141100014, 141100015, 141199001, 141199002, 141199003, 141199004, 141199005, 141199006, 141199007, 141199008, 141199009, 141199010, 141199011, 141199012, 391199013, 1411990110, 1411990111, 1411990112, 1411990120, 1411990121, 1411990122, 3911990132
    ],
    74: ['DEAGLE', 
        # Kode baru: 151100001, 151100002, 151100003, 151100004, 151100005, 151100006, 151100007, 151100008, 151100009, 151100010, 151100011, 151100012, 151100013, 151199001, 151199002, 151199003, 151199004, 151199005
        151100001, 151100002, 151100003, 151100004, 151100005, 151100006, 151100007, 151100008, 151100009, 151100010, 151100011, 151100012, 151100013, 151199001, 151199002, 151199003, 151199004, 151199005
    ],
    75: ['URB', 
        # Kode baru: 161100001, 161100002, 161100003, 161100004, 161100005, 161100006, 161100007, 161100008, 161100009, 161100010, 161100011, 161100012, 161100013, 161100014, 161100015, 161100016, 161100017, 161100018, 161100019, 161100020, 161100021, 161100022, 161100023, 161100024, 161100025, 161100026, 161100027, 161100028, 161100029, 161100030, 161199001, 161199002, 161199003, 161199004, 161199005, 161199006, 161199007, 161199008, 161199009, 161199010, 161199011, 161199012, 161199013, 161199014, 161199015, 161199016, 161199017, 161199018, 161199019, 161199020, 161199021, 161199022, 161199023, 161199024, 161199025, 161199026, 161199027, 161199028, 161199029, 161199030
        161100001, 161100002, 161100003, 161100004, 161100005, 161100006, 161100007, 161100008, 161100009, 161100010, 161100011, 161100012, 161100013, 161100014, 161100015, 161100016, 161100017, 161100018, 161100019, 161100020, 161100021, 161100022, 161100023, 161100024, 161100025, 161100026, 161100027, 161100028, 161100029, 161100030, 161199001, 161199002, 161199003, 161199004, 161199005, 161199006, 161199007, 161199008, 161199009, 161199010, 161199011, 161199012, 161199013, 161199014, 161199015, 161199016, 161199017, 161199018, 161199019, 161199020, 161199021, 161199022, 161199023, 161199024, 161199025, 161199026, 161199027, 161199028, 161199029, 161199030
    ],
    76: ['INP9', 
        # Kode lama: 171199004
        # Kode baru: 171100001, 171100002, 171100003, 171100004, 171100005, 171100006, 171100007, 171100008, 171100009, 171100010, 171100011, 171100012, 171100013, 171100014, 171100015, 171100016, 171100017, 171100018, 171100019, 171100020, 171100021, 171100022, 171100023, 171100024, 171100025, 171100026, 171100027, 171100028, 171100029, 171100030, 171199001, 171199002, 171199003, 171199005, 171199006, 171199007, 171199008, 171199009, 171199010, 171199011, 171199012, 171199013, 171199014, 171199015, 171199016, 171199017, 171199018, 171199019, 171199020, 171199021, 171199022, 171199023, 171199024, 171199025, 171199026, 171199027, 171199028, 171199029, 171199030
        171100001, 171100002, 171100003, 171100004, 171100005, 171100006, 171100007, 171100008, 171100009, 171100010, 171100011, 171100012, 171100013, 171100014, 171100015, 171100016, 171100017, 171100018, 171100019, 171100020, 171100021, 171100022, 171100023, 171100024, 171100025, 171100026, 171100027, 171100028, 171100029, 171100030, 171199001, 171199002, 171199003, 171199004, 171199005, 171199006, 171199007, 171199008, 171199009, 171199010, 171199011, 171199012, 171199013, 171199014, 171199015, 171199016, 171199017, 171199018, 171199019, 171199020, 171199021, 171199022, 171199023, 171199024, 171199025, 171199026, 171199027, 171199028, 171199029, 171199030
    ],
    77: ['M700', 
        # Kode lama: 181100018
        # Kode baru: 181100001, 181100002, 181100003, 181100004, 181100005, 181100006, 181100007, 181100008, 181100009, 181100010, 181100011, 181100012, 181100013, 181100014, 181100015, 181100016, 181100017, 181100019, 181100020, 181199001, 181199002, 181199003, 181199004, 181199005, 181199006, 181199007, 181199011
        181100001, 181100002, 181100003, 181100004, 181100005, 181100006, 181100007, 181100008, 181100009, 181100010, 181100011, 181100012, 181100013, 181100014, 181100015, 181100016, 181100017, 181100018, 181100019, 181100020, 181199001, 181199002, 181199003, 181199004, 181199005, 181199006, 181199007, 181199011
    ],
    79: ['MP155', 
        # Kode lama: 201100001, 201100002
        # Kode baru: 201100003, 201100004, 201100005, 201100006, 201100007, 201100008, 201100009, 201100010, 201100011, 201100012, 201100013, 201100014, 201199001, 201199002, 201199003, 201199004, 201199005, 201199006
        201100001, 201100002, 201100003, 201100004, 201100005, 201100006, 201100007, 201100008, 201100009, 201100010, 201100011, 201100012, 201100013, 201100014, 201199001, 201199002, 201199003, 201199004, 201199005, 201199006
    ],
    82: ['RPG-7', 
        # Kode baru: 211100001, 211100002, 211100003, 211100004, 211100005
        211100001, 211100002, 211100003, 211100004, 211100005
    ],
    83: ['BOW', 
        # Kode lama: 221100002
        # Kode baru: 221100001, 221100003, 221100004, 221100005, 221100006, 221100007, 221100008, 221100009
        221100001, 221100002, 221100003, 221100004, 221100005, 221100006, 221100007, 221100008, 221100009
    ],
    84: ['Axe', 
        # Kode baru: 21000001, 21000002, 21000003, 21000004, 21000005, 21000006, 21000007, 21000008, 21000009, 21000010, 21000011, 21000012, 21000013, 21000014, 21000015, 21000016, 21000017, 21000018, 21000019, 21000020
        21000001, 21000002, 21000003, 21000004, 21000005, 21000006, 21000007, 21000008, 21000009, 21000010, 21000011, 21000012, 21000013, 21000014, 21000015, 21000016, 21000017, 21000018, 21000019, 21000020
    ],
    86: ['KAT4NA1', 
        # Kode lama: 31000017
        # Kode baru: 21000007, 21000008, 31000004, 31000005, 31000007, 31000010, 51000001
        21000007, 21000008, 31000004, 31000005, 31000007, 31000010, 31000017, 51000001
    ],
    88: ['SCAR', 
        # Kode lama: 231100004, 231100013, 231100023, 231100026
        # Kode baru: 231100001, 231100002, 231100003, 231100005, 231100006, 231100007, 231100008, 231100009, 231100010, 231100011, 231100012, 231100014, 231100015, 231100016, 231100017, 231100018, 231100019, 231100020, 231100021, 231100022, 231100024, 231100025, 231100027, 231100028, 231100029, 231100030, 231199001, 231199002, 231199003, 231199004, 231199005, 231199006, 231199007, 231199008, 231199009
        231100001, 231100002, 231100003, 231100004, 231100005, 231100006, 231100007, 231100008, 231100009, 231100010, 231100011, 231100012, 231100013, 231100014, 231100015, 231100016, 231100017, 231100018, 231100019, 231100020, 231100021, 231100022, 231100023, 231100024, 231100025, 231100026, 231100027, 231100028, 231100029, 231100030, 231199001, 231199002, 231199003, 231199004, 231199005, 231199006, 231199007, 231199008, 231199009
    ],
    90: ['P90', 
        # Kode lama: 241199009
        # Kode baru: 241100001, 241100002, 241100003, 241100004, 241100005, 241100006, 241100007, 241100008, 241100009, 241100010, 241100011, 241100012, 241100013, 241100014, 241100015, 241100016, 241100017, 241100018, 241100019, 241100020, 241100021, 241100022, 241100023, 241100024, 241100025, 241199001, 241199002, 241199003, 241199004, 241199005, 241199006, 241199007, 241199008, 241199010, 241199011, 241199012, 241199013, 241199014, 241199015, 241199016, 241199017, 241199018, 241199019, 241199020, 241199021, 241199022, 241199023, 241199024, 241199025
        241100001, 241100002, 241100003, 241100004, 241100005, 241100006, 241100007, 241100008, 241100009, 241100010, 241100011, 241100012, 241100013, 241100014, 241100015, 241100016, 241100017, 241100018, 241100019, 241100020, 241100021, 241100022, 241100023, 241100024, 241100025, 241199001, 241199002, 241199003, 241199004, 241199005, 241199006, 241199007, 241199008, 241199009, 241199010, 241199011, 241199012, 241199013, 241199014, 241199015, 241199016, 241199017, 241199018, 241199019, 241199020, 241199021, 241199022, 241199023, 241199024, 241199025
    ],
    91: ['VSS', 
        # Kode baru: 251100001, 251100002, 251100003, 251100004, 251100005, 251100006, 251100007, 251100008, 251100009, 251100010, 251100011, 251100012, 251100013, 251100014, 251100015, 251100016, 251199001, 251199002, 251199003, 251199004
        251100001, 251100002, 251100003, 251100004, 251100005, 251100006, 251100007, 251100008, 251100009, 251100010, 251100011, 251100012, 251100013, 251100014, 251100015, 251100016, 251199001, 251199002, 251199003, 251199004
    ],
    93: ['AR97', 
        # Kode lama: 271199013
        # Kode baru: 271100001, 271100002, 271100003, 271100004, 271100005, 271100006, 271100007, 271199001, 271199002, 271199003, 271199004
        271100001, 271100002, 271100003, 271100004, 271100005, 271100006, 271100007, 271199001, 271199002, 271199003, 271199004, 271199013
    ],
    94: ['MINIGUN', 
        # Kode baru: 281100001, 281100002, 281100003, 281100004, 281100005, 281100006, 281100007, 281100008, 281100009, 281100010, 281100011, 281100012, 281100013, 281100014, 281100015, 281100016, 281100017, 281100018, 281100019, 281100020, 281100021, 281100022, 281100023, 281100024, 281100025, 281100026, 281100027, 281100028, 281100029, 281100030, 281199001, 281199002, 281199003, 281199004, 281199005, 281199006, 281199007, 281199008, 281199009, 281199010, 281199011, 281199012, 281199013, 281199014, 281199015, 281199016, 281199017, 281199018, 281199019, 281199020, 281199021, 281199022, 281199023, 281199024, 281199025, 281199026, 281199027, 281199028, 281199029, 281199030
        281100001, 281100002, 281100003, 281100004, 281100005, 281100006, 281100007, 281100008, 281100009, 281100010, 281100011, 281100012, 281100013, 281100014, 281100015, 281100016, 281100017, 281100018, 281100019, 281100020, 281100021, 281100022, 281100023, 281100024, 281100025, 281100026, 281100027, 281100028, 281100029, 281100030, 281199001, 281199002, 281199003, 281199004, 281199005, 281199006, 281199007, 281199008, 281199009, 281199010, 281199011, 281199012, 281199013, 281199014, 281199015, 281199016, 281199017, 281199018, 281199019, 281199020, 281199021, 281199022, 281199023, 281199024, 281199025, 281199026, 281199027, 281199028, 281199029, 281199030
    ],
    95: ['KATANA', 
        # Kode baru: 51000001, 51000002, 51000003, 51000004, 51000005, 51000006, 51000007, 51000008, 51000009, 51000010, 51000011, 51000012, 51000013, 51000014, 51000015, 51000016, 51000017, 51000018, 51000019, 51000020
        51000001, 51000002, 51000003, 51000004, 51000005, 51000006, 51000007, 51000008, 51000009, 51000010, 51000011, 51000012, 51000013, 51000014, 51000015, 51000016, 51000017, 51000018, 51000019, 51000020
    ],
    96: ['Great Axe', 
        # Kode baru: 96000001, 96000002, 96000003, 96000004, 96000005, 96000006, 96000007, 96000008, 96000009, 96000010, 96000011, 96000012, 96000013, 96000014, 96000015, 96000016, 96000017, 96000018, 96000019, 96000020
        96000001, 96000002, 96000003, 96000004, 96000005, 96000006, 96000007, 96000008, 96000009, 96000010, 96000011, 96000012, 96000013, 96000014, 96000015, 96000016, 96000017, 96000018, 96000019, 96000020
    ],
    97: ['DUAL KNIFE', 
        # Kode lama: 11000007
        # Kode baru: 97000001, 97000002, 97000003, 97000004, 97000005, 97000006, 97000007, 97000008, 97000009, 97000010, 97000011, 97000012, 97000013, 97000014, 97000015, 97000016, 97000017, 97000018, 97000019, 97000020
        11000007, 97000001, 97000002, 97000003, 97000004, 97000005, 97000006, 97000007, 97000008, 97000009, 97000010, 97000011, 97000012, 97000013, 97000014, 97000015, 97000016, 97000017, 97000018, 97000019, 97000020
    ],
    98: ['AUG', 
        # Kode lama: 291199009, 291199015, 291199017, 291199020, 2911990095, 2911990203
        # Kode baru: 291100001, 291100002, 291100003, 291100004, 291100005, 291100006, 291100007, 291100008, 291100009, 291100010, 291100011, 291199001, 291199003, 291199004, 291199005, 291199006, 291199007, 291199008, 291199011, 291199012, 291199013, 291199014, 2911990091, 2911990092, 2911990093, 2911990094
        291100001, 291100002, 291100003, 291100004, 291100005, 291100006, 291100007, 291100008, 291100009, 291100010, 291100011, 291199001, 291199003, 291199004, 291199005, 291199006, 291199007, 291199008, 291199009, 291199011, 291199012, 291199013, 291199014, 291199015, 291199017, 291199020, 2911990091, 2911990092, 2911990093, 2911990094, 2911990095, 2911990203
    ],
    100: ['MAGNUM', 
        # Kode lama: 301199010
        # Kode baru: 301199001, 301199002, 301199003, 301199004, 301199005, 301199006, 301199007
        301199001, 301199002, 301199003, 301199004, 301199005, 301199006, 301199007, 301199010
    ],
    101: ['MCX', 
        # Kode lama: 311199017, 311199020, 311199024, 3111990200, 3111990201, 3111990203
        # Kode baru: 311100001, 311199001, 311199002, 311199003, 311199004, 311199005, 311199006, 311199007, 311199008, 311199009, 311199010, 311199011, 311199012, 311199013, 311199014, 311199015, 311199016
        311100001, 311199001, 311199002, 311199003, 311199004, 311199005, 311199006, 311199007, 311199008, 311199009, 311199010, 311199011, 311199012, 311199013, 311199014, 311199015, 311199016, 311199017, 311199020, 311199024, 3111990200, 3111990201, 3111990203
    ],
    102: ['FAL', 
        # Kode lama: 321199009, 321199020
        # Kode baru: 321100001, 321199001, 321199002, 321199003, 321199004, 321199005, 321199006, 321199007, 321199008, 321199010, 321199011, 321199012, 321199013, 321199014, 321199015, 321199016, 321199017, 321199018
        321100001, 321199001, 321199002, 321199003, 321199004, 321199005, 321199006, 321199007, 321199008, 321199009, 321199010, 321199011, 321199012, 321199013, 321199014, 321199015, 321199016, 321199017, 321199018, 321199020
    ],
    103: ['KAR98K', 
        # Kode lama: 331100001
        # Kode baru: 331100002, 331100003, 331199001, 331199002, 331199003, 331199004, 331199005, 331199006, 331199007, 331199008, 331199009, 331199010, 331199011, 331199012
        331100001, 331100002, 331100003, 331199001, 331199002, 331199003, 331199004, 331199005, 331199006, 331199007, 331199008, 331199009, 331199010, 331199011, 331199012
    ],
    106: ['SABER', 
        # Kode baru: 10600021, 10600022, 10600023, 10600024, 10600025, 10600026, 10600027, 10600028, 10600029, 10600030, 10600031, 10600032, 10600033, 10600034, 10600035, 10600036, 10600037, 10600038, 10600039, 10600040
        10600021, 10600022, 10600023, 10600024, 10600025, 10600026, 10600027, 10600028, 10600029, 10600030, 10600031, 10600032, 10600033, 10600034, 10600035, 10600036, 10600037, 10600038, 10600039, 10600040
    ],
    107: ['DUAL', 
        # Kode lama: 21000008
        # Kode baru: 10700021, 10700022, 10700023, 10700024, 10700025, 10700026, 10700027, 10700028, 10700029, 10700030, 10700031, 10700032, 10700033, 10700034, 10700035, 10700036, 10700037, 10700038, 10700039, 10700040
        10700021, 10700022, 10700023, 10700024, 10700025, 10700026, 10700027, 10700028, 10700029, 10700030, 10700031, 10700032, 10700033, 10700034, 10700035, 10700036, 10700037, 10700038, 10700039, 10700040, 21000008
    ],
    108: ['PKM', 
        # Kode lama: 341199012
        # Kode baru: 341199001, 341199002, 341199003, 341199004, 341199005, 341199006, 341199007, 341199008, 341199009, 341199010
        341199001, 341199002, 341199003, 341199004, 341199005, 341199006, 341199007, 341199008, 341199009, 341199010, 341199012
    ],
    110: ['UZI', 
        # Kode lama: 351199014, 351199016, 3511990141, 3511990142
        # Kode baru: 351199001, 351199002, 351199003, 351199004, 351199005, 351199006, 351199007, 351199008, 351199009, 351199010, 351199011, 351199012, 351199013, 351199015, 351199017, 351199018
        351199001, 351199002, 351199003, 351199004, 351199005, 351199006, 351199007, 351199008, 351199009, 351199010, 351199011, 351199012, 351199013, 351199014, 351199015, 351199016, 351199017, 351199018, 3511990141, 3511990142
    ],
    112: ['BLADEZ', 
        # Kode lama: 31000007
        # Kode baru: 11200001, 11200002, 11200003, 11200004, 11200005, 11200006, 11200007, 11200008, 11200009, 11200010, 11200011, 11200012, 11200013, 11200014, 11200015, 11200016, 11200017, 11200018, 11200019, 11200020
        11200001, 11200002, 11200003, 11200004, 11200005, 11200006, 11200007, 11200008, 11200009, 11200010, 11200011, 11200012, 11200013, 11200014, 11200015, 11200016, 11200017, 11200018, 11200019, 11200020, 31000007
    ],
    114: ['FN2000', 
        # Kode lama: 371199010, 3711990102
        # Kode baru: 371199001, 371199002, 371199003, 371199004, 371199005, 371199006
        371199001, 371199002, 371199003, 371199004, 371199005, 371199006, 371199010, 3711990102
    ],
    115: ['GALIL', 
        # Kode lama: 381199010, 3811990105
        # Kode baru: 381199001, 381199002, 381199003, 381199004, 381199005, 381199006, 381199007, 381199008, 381199009, 381199011, 381199012, 3811990031, 3811990032, 3811990033, 3811990034, 3811990035, 3811990061, 3811990062, 3811990063
        381199001, 381199002, 381199003, 381199004, 381199005, 381199006, 381199007, 381199008, 381199009, 381199010, 381199011, 381199012, 3811990031, 3811990032, 3811990033, 3811990034, 3811990035, 3811990061, 3811990062, 3811990063, 3811990105
    ],
    121: ['QBZ95', 
        # Kode lama: 391199010, 391199016, 391199021, 3911990102, 3911990103, 3911990104, 3911990105, 3911990106, 3911990162
        # Kode baru: 31000004, 391199012, 431199002, 3911990120, 3911990124
        31000004, 391199010, 391199012, 391199016, 391199021, 431199002, 3911990102, 3911990103, 3911990104, 3911990105, 3911990106, 3911990120, 3911990124, 3911990162
    ],
    122: ['SVD', 
        # Kode lama: 401199004, 401199009
        # Kode baru: 401199001, 401199002, 401199003, 401199005, 401199006
        401199001, 401199002, 401199003, 401199004, 401199005, 401199006, 401199009
    ],
    125: ['RPK', 
        # Kode lama: 411199009, 411199011
        # Kode baru: 411199001, 411199002, 411199003
        411199001, 411199002, 411199003, 411199009, 411199011
    ],
    126: ['M1887', 
        # Kode lama: 421199014, 4211990142
        # Kode baru: 421199001, 421199012
        421199001, 421199012, 421199014, 4211990142
    ],
    129: ['UMP', 
        # Kode lama: 431199005, 431199006, 431199007
        # Kode baru: 431199002, 4311990032
        431199002, 431199005, 431199006, 431199007, 4311990032
    ],
    131: ['SPEAR', 
        # Kode lama: 441199007, 4411990070, 4411990071, 4411990072
        # Kode baru: 441199009
        441199007, 441199009, 4411990070, 4411990071, 4411990072
    ],
    133: ['BARU', 
        # Kode lama: 451199001, 451199004, 4511990010, 4511990011, 4511990012
        # Kode baru: 451199003
        451199001, 451199003, 451199004, 4511990010, 4511990011, 4511990012
    ],
    137: ['BARU', 
        # Kode baru: 11000016
        11000016
    ],
}


@HOOK(EquipCaseFactory, 0)
def Create(wid, *args, **kwargs):
    args = list(args)
    if sync_attrs.get('bSkinHack') and wid in WEP:
        skin_index = sync_attrs.get(f'iSkin_{wid}', 0)
        wids = WEP[wid][1:]
        if wids and skin_index < len(wids):
            args[4] = wids[skin_index]
            sync_attrs[f'activeSkin_{wid}'] = wids[skin_index]
            sync_attrs[f'activeSkinName_{wid}'] = WEP[wid][0]
    result = EquipCaseFactory._Create(wid, *args, **kwargs)
    if sync_attrs.get('bSkinHack') and result:  
        result.show_guise_bullet_trace = True  
        result.show_guise_hit_effect = True  
        result.show_guise_kill_icon = True  
        result.show_guise_kill_msg = True  
        result.show_guise_weapon_icon = True  
        result._support_kill_counter = True  
        result._upgrade_satisfy_kill_counter = True  
        result.skin_hit_object_effect_dict = {}  
    return result

@HOOK(GunSmithDiyWindow)
def ChangeGunGuise(self, *_, **__):
    return self._ChangeGunGuise(*_, **__)
    
@HOOK(GunSmithDiyHeroModelCtrl)
def AddWeaponByEquipId(self, *_, **__):
    return self._AddWeaponByEquipId(*_, **__)

# ========== ITEM ESP UPDATE ==========
from gclient.gameplay.logic_base.entities.combat_item import CombatItemEntity, CombatItemType
from gshare.formula import Distance3D

ITEM_FIFO_PATH = "/data/data/" + PACKAGE_NAME + "/imgui/loot_esp"

@TRY
def ItemESPUpdate(*_, **__):
    space = genv.space
    if not isinstance(space, Space) or not space.camera:
        return
        
    if not sync_attrs.get('bItemESP', True):
        return
        
    camera_pos = space.camera.position
    ground_loot = []
    
    for entityId, entity in space.entities.copy().items():
        try:
            if not isinstance(entity, CombatItemEntity):
                continue
                
            if not hasattr(entity, 'position') or not entity.position:
                continue
                
            pos = entity.position
            distance = Distance3D(camera_pos, pos)
            
            max_dist = sync_attrs.get('fItemESPMaxDist', 150.0)
            if distance > max_dist:
                continue
                
            screen_pos = WorldToScreenPoint(Vector3(*pos))
            if screen_pos.z <= 0.0:
                continue
            
            # ===== DATA ITEM DALAM DICTIONARY =====
            data_item = {
                "x": screen_pos.x,
                "y": screen_pos.y,
                "z": screen_pos.z,
                "dist": distance,
                "world_x": pos[0],
                "world_y": pos[1],
                "world_z": pos[2],
                "quality": getattr(entity, 'quality', 0),
                "item_type": getattr(entity, 'item_type', 0),
                "is_gun": getattr(entity, 'is_gun', False),
                "is_box": getattr(entity, 'IsBox', False),
                "is_airdrop": getattr(entity, 'IsAirDrop', False),
                "is_ammo": getattr(entity, 'IsAmmoBox', False),
                "is_supply": getattr(entity, 'IsSupply', False),
                "owner": getattr(entity, 'owner_name', ''),
                "equip_id": getattr(entity, 'equip_id', 0),
                "equip_name": getattr(entity.equip_proto, 'name', '') if hasattr(entity, 'equip_proto') and entity.equip_proto else '',
                "item_id": getattr(entity, 'id', 0)
            }
            
            # ===== PROCESS DATA ITEM =====
            item_result = {
                "x": data_item["x"],
                "y": data_item["y"],
                "z": data_item["z"],
                "dist": data_item["dist"],
                "world_x": data_item["world_x"],
                "world_y": data_item["world_y"],
                "world_z": data_item["world_z"],
                "quality": data_item["quality"]
            }
            
            # CEK AIRDROP
            if data_item["is_airdrop"]:
                item_result["name"] = "Airdrop"
                item_result["type"] = "Airdrop"
                ground_loot.append(item_result)
                
            # CEK BOX
            elif data_item["is_box"]:
                if data_item["owner"] and data_item["owner"] != 'Unknown':
                    item_result["name"] = data_item["owner"] + "'s Box"
                else:
                    item_result["name"] = "Loot Box"
                item_result["type"] = "Box"
                ground_loot.append(item_result)
                
            # CEK SENJATA
            elif data_item["is_gun"]:
                if data_item["equip_name"]:
                    weapon_name = data_item["equip_name"]
                else:
                    weapon_name = "Weapon_" + str(data_item["equip_id"])
                    
                quality_text = " +" + str(data_item["quality"]) if data_item["quality"] > 0 else ""
                item_result["name"] = weapon_name + quality_text
                item_result["type"] = "Weapon"
                ground_loot.append(item_result)
                
            # CEK AMMO/SUPPLY
            elif data_item["is_ammo"] or data_item["is_supply"]:
                item_result["name"] = "Supply Box"
                item_result["type"] = "Supply"
                ground_loot.append(item_result)
                
            # CEK ZOMBIE DROP
            elif data_item["item_id"] == 9999:
                item_result["name"] = "Zombie Crystal"
                item_result["type"] = "ZombieDrop"
                ground_loot.append(item_result)
                
        except:
            continue
            
    if ground_loot:
        try:
            fifo_dir = os.path.dirname(ITEM_FIFO_PATH)
            if not os.path.exists(fifo_dir):
                os.makedirs(fifo_dir, 0o777)
                
            if not os.path.exists(ITEM_FIFO_PATH):
                os.mkfifo(ITEM_FIFO_PATH, 0o666)
                os.chmod(ITEM_FIFO_PATH, 0o666)
                
            with open(ITEM_FIFO_PATH, 'w') as fifo:
                json.dump(ground_loot, fifo)
                fifo.write('\n')
        except:
            pass

REGISTER_UPDATES.append(ItemESPUpdate)

sync_attrs.update({
    'bItemESP': True,
    'bItemESP_ShowDistance': True,
    'bItemESP_Airdrop': True,
    'bItemESP_Boxes': True,
    'bItemESP_Weapons': True,
    'bItemESP_Supply': True,
    'bItemESP_Zombie': True,
    'fItemESPMaxDist': 150.0
})

)srcpy").chr();