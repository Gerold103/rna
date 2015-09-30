extern "C" {
	#include <tarantool.h>
	#include <lua.h>
	#include <lauxlib.h>
	#include <lualib.h>
	#include "sqlite3.h"
	#include "sqliteInt.h"
}
#include <iostream>
#include <vector>
#include "logger/logger.h"

struct SQLiteRes {
	std::vector<std::string> names;
	std::vector<std::vector<std::string> > values;
};

extern "C" {
	static int lbox_net_sqlite_connect(struct lua_State *L);
	static inline sqlite3 *lua_check_sqliteconn(struct lua_State *L, int index);
	static int lua_sqlite_pushresult(struct lua_State *L, sqlite3 *db, const SQLiteRes &rows);
	static int callback(void *data, int argc, char **argv, char **azColName);
	static int lua_sqlite_execute(struct lua_State *L);
	static int lua_sqlite_close(struct lua_State *L);
	LUA_API int luaopen_sqlite_driver(lua_State *L);
}

static int lbox_net_sqlite_connect(struct lua_State *L)
{
	LogFL(DEBUG) << "lbox_net_sqlite_connect(): lua_gettop = " << lua_gettop(L) << "\n";
	if (lua_gettop(L) < 1) {
		return luaL_error(L, "Usage: sqlite.connect(<database_name>)");
	}
	const char *db_name = luaL_checkstring(L, 1);
	LogFL(DEBUG) << "lbox_net_sqlite_connect(): db_name = " << ((db_name == NULL) ? "NULL" : db_name) << "\n";
	sqlite3 *db = NULL;
	int rc = sqlite3_open(db_name, &db);
	if (rc != SQLITE_OK) {
		LogFL(DEBUG) << "lbox_net_sqlite_connect(): rc != SQLITE_OK\n";
		luaL_error(L, "Error: error during opening database <%s>", db_name);
	}
	LogFL(DEBUG) << "lbox_net_sqlite_connect(): rc == SQLITE_OK\n";
	lua_pushboolean(L, 1);
	sqlite3 **ptr = (sqlite3 **)lua_newuserdata(L, sizeof(sqlite3 *));
	*ptr = db;
	luaL_getmetatable(L, "sqlite");
	lua_setmetatable(L, -2);
	return 2;
}

static inline sqlite3 *lua_check_sqliteconn(struct lua_State *L, int index)
{
	sqlite3 *conn = *(sqlite3 **) luaL_checkudata(L, index, "sqlite");
	if (conn == NULL)
		luaL_error(L, "Attempt to use closed connection");
	return conn;
}

static int lua_sqlite_pushresult(struct lua_State *L, sqlite3 *db, const SQLiteRes &rows)
{
	lua_createtable(L, 0, 1 + rows.values.size());
	int tid = lua_gettop(L);
	//adding array of names
	lua_createtable(L, rows.names.size(), 0);
	int names_id = lua_gettop(L);
	for (size_t i = 0, size = rows.names.size(); i < size; ++i) {
		lua_pushstring(L, rows.names[i].c_str());
		lua_rawseti(L, names_id, i + 1);
	}
	lua_rawseti(L, tid, 1);
	for (size_t i = 0, size = rows.values.size(); i < size; ++i) {
		lua_createtable(L, rows.values[i].size(), 0);
		int vals_id = lua_gettop(L);
		for (size_t j = 0, sizej = rows.values[i].size(); j < sizej; ++j) {
			lua_pushstring(L, rows.values[i][j].c_str());
			lua_rawseti(L, vals_id, j + 1);
		}
		lua_rawseti(L, tid, i + 2);
	}
	lua_pushboolean(L, 1);
	return 2;
}

static int callback(void *data, int argc, char **argv, char **azColName)
{
	SQLiteRes *res = (SQLiteRes *)data;
	if (res->names.size() == 0) {
		for (int i = 0; i < argc; ++i) {
			res->names.push_back(std::string(azColName[i]));
		}
	}
	std::vector<std::string> tmp;
	for (int i = 0; i < argc; ++i) {
		tmp.push_back((argv[i] == NULL) ? (std::string("NULL")) : (std::string(argv[i])));
	}
	res->values.push_back(tmp);
	return 0;
}

static int lua_sqlite_execute(struct lua_State *L)
{
	LogFL(DEBUG) << "lua_sqlite_execute(): lua_gettop = " << lua_gettop(L) << "\n";
	sqlite3 *db = lua_check_sqliteconn(L, 1);
	size_t len;
	const char *sql = lua_tolstring(L, 2, &len);
	LogFL(DEBUG) << "lua_sqlite_execute(): sql = " << ((sql == NULL) ? "NULL" : sql) << "\n";

	char *errMsg = NULL;
	SQLiteRes res;
	int rc = sqlite3_exec(db, sql, callback, (void *)&res, &errMsg);
	if (rc != SQLITE_OK) {
		std::string tmp(errMsg);
		sqlite3_free(errMsg);
		luaL_error(L, "Error: error while executing query `%s`\nError message: %s", sql, tmp.c_str());
	}
	
	return lua_sqlite_pushresult(L, db, res);
}

static int lua_sqlite_close(struct lua_State *L)
{
	sqlite3 **conn = (sqlite3 **) luaL_checkudata(L, 1, "sqlite");
	if (*conn == NULL) {
		lua_pushboolean(L, 0);
		return 1;
	}
	sqlite3_close(*conn);
	*conn = NULL;
	lua_pushboolean(L, 1);
	return 1;
}
extern "C" {
static int test_func(struct lua_State *L)
{
	LogFL(DEBUG) << "test_func(): lua_gettop = " << lua_gettop(L) << "\n";
	const char *db_name = luaL_checkstring(L, 1);
	LogFL(DEBUG) << "test_func(): db_name = " << ((db_name == NULL) ? "NULL" : db_name) << "\n";
	lua_pushboolean(L, 1);
	return 1;
}
}

LUA_API int luaopen_sqlite_driver(lua_State *L)
{
	LogFL(DEBUG) << "luaopen_sqlite_driver()\n";
	static const struct luaL_reg methods [] = {
		{"execute",	lua_sqlite_execute},
		{"close",	lua_sqlite_close},
		{NULL, NULL}
	};
	luaL_newmetatable(L, "sqlite");
	lua_pushvalue(L, -1);
	luaL_register(L, NULL, methods);
	lua_setfield(L, -2, "__index");
	lua_pushstring(L, "sqlite");
	lua_setfield(L, -2, "__metatable");
	lua_pop(L, 1);

	lua_newtable(L);
	static const struct luaL_reg meta [] = {
		{"connect", lbox_net_sqlite_connect},
		{"test_func", test_func},
		{NULL, NULL}
	};
	luaL_register(L, NULL, meta);
	return 1;
}