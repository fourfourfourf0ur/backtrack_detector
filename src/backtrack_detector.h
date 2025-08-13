#ifndef BACKTRACK_DETECTOR_H_
#define BACKTRACK_DETECTOR_H_

class BacktrackDetector {
private:
  static int last_acknowledged_[MAX_CLIENTS + 1];
  static bool is_punished_[MAX_CLIENTS + 1];
  static cvar_t *cmd_to_execute_;

  static char *PrepareCmdForExecute(IGameClient *client);
  static void HandleNetCommand(IRehldsHook_HandleNetCommand *chain,
                               IGameClient *client, int8 opcode);

public:
  BacktrackDetector();
  ~BacktrackDetector();

  static qboolean Connect(edict_t *entity, const char *name,
                          const char *address, char *reject_reason);
};

extern std::unique_ptr<BacktrackDetector> backtrack_detector;

#endif  // !BACKTRACK_DETECTOR_H_
