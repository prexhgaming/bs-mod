HERO_SKIN_IDS = {
    3000000030: "Novara",  # Novara Hero Skin
}

@HOOK(CombatAvatar, 0)
def OnHeroSkinApply(self, *args, **kwargs):
   
    result = self._OnHeroSkinApply(*args, **kwargs)
    
    if sync_attrs.get('bHeroSkin', False):
        try:
            hero_skin_id = sync_attrs.get('iHeroSkinID', 3000000030)
            
            # Apply Novara skin
            if hero_skin_id == 3000000030:
                if hasattr(self, 'ApplyHeroSkin'):
                    self.ApplyHeroSkin(hero_skin_id)
                elif hasattr(self, 'SetHeroSkin'):
                    self.SetHeroSkin(hero_skin_id)
                elif hasattr(self, 'SetSkin'):
                    self.SetSkin(hero_skin_id)
                
                # Force update hero model
                if hasattr(self, 'UpdateHeroModel'):
                    self.UpdateHeroModel()
                
                print(f"[HERO SKIN] Novara skin applied (ID: {hero_skin_id})")
                
        except Exception as e:
            print(f"[HERO SKIN] Error: {e}")
    
    return result

@HOOK(CombatAvatar, 0)
def OnHeroSpawn(self, *args, **kwargs):
    """Hook kapag nag-spawn ang hero - auto apply skin"""
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
                
                print(f"[HERO SKIN] Novara applied on spawn")
                
        except Exception as e:
            print(f"[HERO SKIN] Spawn error: {e}")
    
    return result

# ===== HERO SKIN TOGGLES =====
sync_attrs.setdefault('bHeroSkin', False)
sync_attrs.setdefault('iHeroSkinID', 3000000030)

print("[HERO SKIN] Novara (ID: 3000000030) hook loaded!")