#include "precompiled.h"

constexpr char CFG_FILE[] = "btd.cfg";

int BacktrackDetector::last_acknowledged_[MAX_CLIENTS + 1];
bool BacktrackDetector::is_punished_[MAX_CLIENTS + 1];
cvar_t* BacktrackDetector::cmd_to_execute_;

// https://github.com/rehlds/rechecker/blob/387adbc6340a8aa1b85ab8255b42fa0109d2f9fe/src/cmdexec.cpp#L60
char* BacktrackDetector::PrepareCmdForExecute(IGameClient* client) {
  if (cmd_to_execute_->string[0] == '\0') return nullptr;

  static char cmd[256];
  std::strcpy(cmd, cmd_to_execute_->string);

  const auto net = client->GetNetChan()->GetRemoteAdr();

  const auto userid = g_engfuncs.pfnGetPlayerUserId(client->GetEdict());

  StringReplace(cmd, "[id]", UTIL_VarArgs("%i", client->GetId() + 1));
  StringReplace(cmd, "[userid]", UTIL_VarArgs("#%u", userid));
  StringReplace(
      cmd, "[steamid]",
      UTIL_VarArgs("%s", g_engfuncs.pfnGetPlayerAuthId(client->GetEdict())));
  StringReplace(cmd, "[ip]",
                UTIL_VarArgs("%i.%i.%i.%i", net->ip[0], net->ip[1], net->ip[2],
                             net->ip[3]));
  StringReplace(cmd, "[name]", client->GetName());

  // Valid commands all have a ';' or newline '\n' as their last character
  if (cmd[0] != '\0') {
    const auto length = std::strlen(cmd);
    if (length < sizeof(cmd) - 2)
      std::strcat(cmd, "\n");
    else
      cmd[length - 1] = '\n';
  }

  return cmd;
}

void BacktrackDetector::HandleNetCommand(IRehldsHook_HandleNetCommand* chain,
                                         IGameClient* client, int8 opcode) {
  chain->callNext(client, opcode);
  const auto edict = client->GetEdict();
  if (opcode == clc_move && edict != nullptr && edict->v.deadflag == DEAD_NO) {
    const auto id = client->GetId();
    const auto host_client = g_RehldsSvs->GetClient_t(id);
    if (host_client != nullptr && host_client->fully_connected) {
      if (host_client->netchan.incoming_acknowledged < last_acknowledged_[id]) {
        const auto cmd_to_execute = PrepareCmdForExecute(client);
        if (cmd_to_execute && cmd_to_execute[0] != '\0' && !is_punished_[id]) {
          is_punished_[id] = true;
          SERVER_COMMAND(cmd_to_execute);
          SERVER_EXECUTE();
        }
      }

      last_acknowledged_[id] = host_client->netchan.incoming_acknowledged;
    }
  }
}

BacktrackDetector::BacktrackDetector() {
  cvar_t cmd_to_execute = {"btd_cmd_to_execute",
                           "kick [userid] Backtrack detected",
                           FCVAR_SERVER | FCVAR_PROTECTED, 0.0f, nullptr};
  CVAR_REGISTER(&cmd_to_execute);
  cmd_to_execute_ = CVAR_GET_POINTER(cmd_to_execute.name);

  const auto game_dir = GET_GAME_INFO(PLID, GINFO_GAMEDIR);
  const auto plugin_dir = GET_PLUGIN_PATH(PLID);

  char relative_path[MAX_PATH];
  std::strncpy(relative_path, &plugin_dir[strlen(game_dir) + 1],
               sizeof(relative_path) - 1);
  relative_path[sizeof(relative_path) - 1] = '\0';
  NormalizePath(relative_path);

  auto pos = std::strrchr(relative_path, '/');
  if (pos) *(pos + 1) = '\0';

  char execute_config_cmd[MAX_PATH];
  std::snprintf(execute_config_cmd, sizeof(execute_config_cmd),
                "exec \"%s%s\"\n", relative_path, CFG_FILE);
  SERVER_COMMAND(execute_config_cmd);
  SERVER_EXECUTE();

  g_RehldsHookchains->HandleNetCommand()->registerHook(
      &BacktrackDetector::HandleNetCommand, HC_PRIORITY_DEFAULT + 1);
}

BacktrackDetector::~BacktrackDetector() {
  g_RehldsHookchains->HandleNetCommand()->unregisterHook(
      &BacktrackDetector::HandleNetCommand);
}

qboolean BacktrackDetector::Connect(edict_t* entity, const char* name,
                                    const char* address, char* reject_reason) {
  const auto id = ENTINDEX(entity) - 1;
  last_acknowledged_[id] = 0;
  is_punished_[id] = false;

  return true;
}
