SendScript("lua/vampiric_cl.lua")

local function PlayerDamaged(self,inflictor,attacker,damage,meansOfDeath)
	local force = nil
	if(self != nil and attacker != nil) then
	if(self == attacker) then return end
	local atk_tab = GetEntityTable(attacker);
		atk_tab.give = atk_tab.give or 0
		local hp = attacker:GetInfo()["health"]
		local hp2 = self:GetInfo()["health"]

		if(hp2 > 0 and (hp2 - damage) <= -40) then
			local give = 20
			atk_tab.give = atk_tab.give + give;
			attacker:SendString("damagegiven " .. give)
			attacker:SendString("target " .. hp2-damage .. " body")
			atk_tab.wait = 10
			force = 1000
		else
			atk_tab.wait = 0
		end
		
		if(hp and hp2 > 0) then
			if(damage > hp2) then damage = hp2 end
			local give = math.ceil(damage/4);
			atk_tab.give = atk_tab.give + give;
			attacker:SendString("damagegiven " .. give)
			attacker:SendString("target " .. hp2-damage .. " " .. self:GetInfo()["name"])
			atk_tab.wait = atk_tab.wait + 20
		end
	end
	if(force != nil) then
		return force
	end
end
hook.add("PlayerDamaged","Vampiric",PlayerDamaged)

local function plThink()
	local tab = GetAllPlayers()
	for k,v in pairs(tab) do
		local hp = v:GetInfo(v)["health"]
		local tab = GetEntityTable(v);
		if(tab) then tab.wait = tab.wait or 0 end
		if(tab and tab.give and tab.give > 0) then
			if(tab.wait > 0) then 
				tab.wait = tab.wait - 1
			else
				local giverate = 1
				if(tab.give < 10) then tab.wait = 1 end
				if(tab.give > 10) then giverate = 2 end
				if(tab.give > 30) then giverate = 3 end
				if(hp < 300) then
					v:SetInfo(PLAYERINFO_HEALTH,hp + giverate)
					hp = v:GetInfo()["health"]
					tab.give = tab.give - giverate
					v:SendString("sub " .. giverate)
					if(tab.give <= 0) then tab.wait = 10 end
				else
					v:SetInfo(PLAYERINFO_HEALTH,300)
					v:SendString("sub " .. tab.give)
					tab.give = 0
					tab.wait = 4
				end
			end
		else
			if(tab.wait > 0) then 
				tab.wait = tab.wait - 1
			else
				if(hp >= 200) then
					v:SetMaxHealth(hp+1) --Block pain sounds
					v:SetInfo(PLAYERINFO_HEALTH,hp-1)
				else
					v:SetMaxHealth(0) --Revert to default max health
				end
			end
		end
	end
end
hook.add("Think","Vampiric",plThink)