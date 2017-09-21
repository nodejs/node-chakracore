#ifndef SRC_CHAKRA_TTD_H_
#define SRC_CHAKRA_TTD_H_

#if defined(NODE_ENGINE_CHAKRACORE)
#define ENABLE_TTD_NODE 1
#else
#define ENABLE_TTD_NODE 0
#endif

#if defined(ENABLE_TTD_NODE) && ENABLE_TTD_NODE
extern bool s_doTTRecord;
extern bool s_doTTReplay;
extern bool s_doTTDebug;
#endif

#endif  // SRC_CHAKRA_TTD_H_
