#include "cg_local.h"

int qlua_getitemicon(lua_State *L) {
	int icon = 0;
	int size = sizeof(cg_items) / sizeof(cg_items[0]);
	luaL_checktype(L,1,LUA_TNUMBER);

	icon = lua_tointeger(L,1);

	if(icon <= 0 || icon > size) {
		lua_pushstring(L,"Item Index Out Of Bounds\n");
		lua_error(L);
		return 1;
	}
	CG_RegisterItemVisuals( icon );
	lua_pushinteger(L,cg_items[icon].icon);
	return 1;
}

int qlua_getitemmodel(lua_State *L) {
	int icon = 0;
	int size = sizeof(cg_items) / sizeof(cg_items[0]);
	luaL_checktype(L,1,LUA_TNUMBER);

	icon = lua_tointeger(L,1);

	if(icon <= 0 || icon > size) {
		lua_pushstring(L,"Item Index Out Of Bounds\n");
		lua_error(L);
		return 1;
	}
	CG_RegisterItemVisuals( icon );
	lua_pushinteger(L,cg_items[icon].models[0]);
	return 1;
}

int qlua_getitemname(lua_State *L) {
	int icon = 0;
	int size = sizeof(cg_items) / sizeof(cg_items[0]);
	luaL_checktype(L,1,LUA_TNUMBER);

	icon = lua_tointeger(L,1);

	if(icon <= 0 || icon > size) {
		lua_pushstring(L,"Item Index Out Of Bounds\n");
		lua_error(L);
		return 1;
	}
	if(bg_itemlist[icon].pickup_name != NULL) {
		lua_pushstring(L,bg_itemlist[icon].pickup_name);
	}
	return 1;
}

int qlua_getweaponicon(lua_State *L) {
	int icon = 0;
	int size = WP_NUM_WEAPONS;
	qboolean ammo = qfalse;

	luaL_checktype(L,1,LUA_TNUMBER);

	icon = lua_tointeger(L,1);

	if(icon < 0 || icon >= size) {
		lua_pushstring(L,"Weapon Index Out Of Bounds\n");
		lua_error(L);
		return 1;
	}

	if(lua_type(L,2) == LUA_TBOOLEAN && lua_toboolean(L,2) == qtrue) {
		lua_pushinteger(L,cg_weapons[ icon ].ammoIcon);
	} else {
		lua_pushinteger(L,cg_weapons[ icon ].weaponIcon);
	}
	return 1;
}

int qlua_getweaponname(lua_State *L) {
	int icon = 0;
	int size = WP_NUM_WEAPONS;
	gitem_t *item;

	luaL_checktype(L,1,LUA_TNUMBER);

	icon = lua_tointeger(L,1);

	if(icon < 0 || icon >= size) {
		lua_pushstring(L,"Weapon Index Out Of Bounds\n");
		lua_error(L);
		return 1;
	}
	item = cg_weapons[ icon ].item;

	if(item != NULL) {
		lua_pushstring(L,item->pickup_name);
	} else {
		lua_pushstring(L,"");
	}
	return 1;
}

int qlua_lockmouse (lua_State *L) {
	luaL_checktype(L,LUA_TBOOLEAN,1);
	trap_LockMouse(lua_toboolean(L,1));
	return 0;
}

int qlua_loadskin(lua_State *L) {
	const char *filename = "";
	int out = 0;

	luaL_checkstring(L,1);
	filename = lua_tostring(L,1);
	out = trap_R_RegisterSkin( filename );

	if (!out) {
		Com_Printf( "Skin load failure: %s\n", filename );
	} else {
		lua_pushinteger(L,out);
		return 1;
	}

	return 0;
}

int qlua_createmark(lua_State *L) {
	int shader;
	vec3_t origin;
	vec3_t plane;
	float orient;
	float red;
	float green;
	float blue;
	float alpha;
	qboolean alphafade = qfalse;
	float radius;
	
	luaL_checkint(L,1);
	luaL_checktype(L,2,LUA_TVECTOR);
	luaL_checktype(L,3,LUA_TVECTOR);
	luaL_checknumber(L,4);
	luaL_checknumber(L,5);
	luaL_checknumber(L,6);
	luaL_checknumber(L,7);
	luaL_checknumber(L,8);
	luaL_checknumber(L,9);

	shader = lua_tointeger(L,1);
	lua_tovector(L,2,origin);
	lua_tovector(L,3,plane);
	orient = lua_tonumber(L,4);
	red = lua_tonumber(L,5);
	green = lua_tonumber(L,6);
	blue = lua_tonumber(L,7);
	alpha = lua_tonumber(L,8);
	radius = lua_tonumber(L,9);

	if(lua_type(L,10) == LUA_TBOOLEAN) {
		alphafade = lua_toboolean(L,10);
	}

	CG_ImpactMark(shader,origin,plane,orient,red,green,blue,alpha,alphafade,radius,qfalse);

	return 0;
}

static const luaL_reg Util_methods[] = {
  {"GetItemIcon",	qlua_getitemicon},
  {"GetItemModel",	qlua_getitemmodel},
  {"GetItemName",	qlua_getitemname},
  {"GetWeaponIcon",	qlua_getweaponicon},
  {"GetWeaponName",	qlua_getweaponname},
  {"LockMouse",		qlua_lockmouse},
  {"LoadSkin",		qlua_loadskin},
  {"CreateMark",	qlua_createmark},
  {0,0}
};

void CG_InitLuaUtil(lua_State *L) {
	luaL_openlib(L, "util", Util_methods, 0);
}