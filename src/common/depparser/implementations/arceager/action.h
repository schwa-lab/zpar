#ifndef DEPPARSER_ACTION_H
#define DEPPARSER_ACTION_H

#include <schwa/hashtable.h>

namespace action {

// SH RE AL AR PR [AL+LABEL] AR+LABEL
enum StackAction : schwa::Label {
  NO_ACTION = 0,
  SHIFT,
  REDUCE,
  ARC_LEFT,
  ARC_RIGHT,
  POP_ROOT,
  LABEL_AL_FIRST,
  LABEL_AR_FIRST = LABEL_AL_FIRST + DEP_LABEL_COUNT - 1,
  MAX = LABEL_AR_FIRST + DEP_LABEL_COUNT - 1,
};

#ifdef LABELED
static StackAction encodeAction(const StackAction action, const schwa::Label label=0) {
   assert(label<DEP_LABEL_COUNT);
   if (action==ARC_LEFT) {
      if (label==0)
         return ARC_LEFT;
      else
         return static_cast<StackAction>(LABEL_AL_FIRST + label - 1);
   }
   else if (action==ARC_RIGHT) {
      if (label==0)
         return ARC_RIGHT;
      else
         return static_cast<StackAction>(LABEL_AR_FIRST + label - 1);
   }
   else
      assert(label==0);
   return action;
}

static StackAction getUnlabeledAction(const StackAction action) {
   assert(action < MAX);
   if (action < LABEL_AL_FIRST)
      return action;
   else if (action<LABEL_AR_FIRST)
      return ARC_LEFT;
   else
      return ARC_RIGHT;
}

static schwa::Label getLabel(const StackAction action) {
   assert(action<MAX);
   if (action < LABEL_AL_FIRST)
      return 0;
   else if (action < LABEL_AR_FIRST)
      return action-LABEL_AL_FIRST+1;
   else
      return action-LABEL_AR_FIRST+1;
}
#endif

struct CScoredAction {
   StackAction action;
   SCORE_TYPE score;

   inline bool operator < (const CScoredAction &a) const {
      return score < a.score;
   }
   inline bool operator > (const CScoredAction &a) const {
      return score > a.score;
   }
};

}//namespace
#endif
