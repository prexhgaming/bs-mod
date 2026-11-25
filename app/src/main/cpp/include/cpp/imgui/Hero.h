

@HOOK(CombatAvatar, 0)
def OnHeroSkinApply(self, *args, **kwargs):
    result = self._OnHeroSkinApply(*args, **kwargs)
    if sync_attrs.get('bHeroSkin', False):
        try:
            hero_skin_id = sync_attrs.get('iHeroSkinID', 3000000030)
            if hero_skin_id == 3000000030:
                if hasattr(self, 'ApplyHeroSkin'):
                    self.ApplyHeroSkin(hero_skin_id)
                elif hasattr(self, 'SetHeroSkin'):
                    self.SetHeroSkin(hero_skin_id)
                elif hasattr(self, 'SetSkin'):
                    self.SetSkin(hero_skin_id)
                if hasattr(self, 'UpdateHeroModel'):
                    self.UpdateHeroModel()
                print(f"[HERO SKIN] Novara applied")
        except Exception as e:
            print(f"[HERO SKIN] Error: {e}")
    return result

@HOOK(CombatAvatar, 0)
def OnHeroSpawn(self, *args, **kwargs):
    result = self._OnHeroSpawn(*args, **kwargs)
    if sync_attrs.get('bHeroSkin', False):
        try:
            hero_skin_id = sync_attrs.get('iHeroSkinID', 3000000030)
            if hero_skin_id == 3000000030:
                if hasattr(self, 'SetHeroSkin'):
                    self.SetHeroSkin(hero_skin_id)
                elif hasattr(self, 'ApplyHeroSkin'):
                    self.ApplyHeroSkin(hero_skin_id)
                if hasattr(self, 'UpdateHeroModel'):
                    self.UpdateHeroModel()
                print(f"[HERO SKIN] Novara on spawn")
        except Exception as e:
            print(f"[HERO SKIN] Spawn error: {e}")
    return result

# Initialize toggles
sync_attrs.setdefault('bHeroSkin', False)
sync_attrs.setdefault('iHeroSkinID', 3000000030)

print("[HERO SKIN] Novara (ID: 3000000030) loaded!")